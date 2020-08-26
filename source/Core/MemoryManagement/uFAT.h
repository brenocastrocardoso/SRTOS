/*
 * uFAT.h
 *      _________________
 *     |SIG              |
 *     |  BOOT SECTION   |
 *     |________________ |
 *     |                 |
 *     | FILE ENTRY SEC  |
 *     |________________ |
 *     |                 |
 *     |  DATA (SEC 1)   |
 *     |________________ |
 *     |                 |
 *     |  DATA (SEC 2)   |
 *     |________________ |
 *     |                 |
 *     |       .         |
 *     |       .         |
 *     |       .         |
 *     |________________ |
 *     |                 |
 *     |  DATA (SEC N)   |
 *     |________________ |
 *  Created on: 31 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_SOURCE_CORE_MEMORYMANAGEMENT_UFAT_H_
#define RTOS_SOURCE_CORE_MEMORYMANAGEMENT_UFAT_H_

#include <RTOS.h>

#include STRING_INCLUDE

#include <string.h>
#include "MemoryValue.h"

namespace RTOS
{
	template<class MemoryEngine>
	class uFAT
	{
	public:
		typedef uint32 MemoryAddress;

		static constexpr uint32 uFatSignature = 0x935BF057;

		//section that will have information about the memory block
		//It takes 52 bytes
		struct BootSection
		{
		public:
			static constexpr uint8 NAME_SIZE = 35;

			// 8 bit
			enum MEMORY_TYPE
			{
				EEPROM,
				INTERNAL_FLASH,
				UNKOWN,
			};

			BootSection(): _signature(0), _memoryType(UNKOWN), _numberOfSectors(0), _sectorSize(0), _FileTableAddress(0), _CRC(0)
			{
				memset(_name, 0x00, NAME_SIZE);
			}

			void setName(STRING name)
			{
			    STD_NAMESPACE::strncpy(_name, name.c_str(), NAME_SIZE);
			}

			uint32 calculateCRC()
			{
				uint8* currentPosition = (uint8*)this;
				uint32 CRC_Retval = 0;
				for(uint8 bytesSummed = 0; bytesSummed < (sizeof(BootSection)-sizeof(_CRC)); ++bytesSummed)
				{
					CRC_Retval += *(currentPosition);
					++currentPosition;
				}
				return CRC_Retval;
			}

			bool checkCorruption()
			{
				return calculateCRC()==_CRC;
			}

			void setCRC()
			{
				_CRC = calculateCRC();
			}

			uint32 _signature;
			//1 byte
			uint8 _memoryType;
			//4 bytes
			uint16 _numberOfSectors;
			uint16 _sectorSize;
			//4 bytes
			MemoryAddress _FileTableAddress;
			//39 bytes, use that to make the structure align with 4 bytes structure
			char _name[NAME_SIZE];
			//4 bytes
			uint32 _CRC;
		};

		template<class FileDescriptor_MemoryEngine>
		class FileDescriptor;

		//It takes 4*numberOfSectors + 4
		template<class FileTable_MemoryEngine>
		class FileTable
		{
		public:
			struct FileEntry
			{
				static constexpr uint16 MIDLE_OF_CLUSTER = 0xFFFD;
				static constexpr uint16 FREE_CLUSTER = 0xFFFE;
				static constexpr uint16 END_OF_CLUSTER = 0xFFFF;
				static constexpr uint16 INVALID_SECTOR = 0xFFFF;

				uint16 _FileId;
				uint16 _nextSectorNumber;

				FileEntry(): _FileId(0), _nextSectorNumber(0)
				{}
			};
		private:
			uFAT& _fileSystemUnity;
			RTOS::MemoryPointer<FileTable_MemoryEngine, FileEntry> _startPointer;
			RTOS::MemoryValue<FileTable_MemoryEngine, uint32> _CRC;
		public:

            FileTable(uFAT& fileSystemUnity):
                _fileSystemUnity(fileSystemUnity),
                _startPointer(0, fileSystemUnity._memory),
                _CRC(sizeof(FileEntry), fileSystemUnity._memory)
            {}

            void config(MemoryAddress startAddress)
            {
                _startPointer.setAddress(startAddress);
                _CRC.setAddress(startAddress+_fileSystemUnity._numberOfSectors*sizeof(FileEntry));
            }

			uint32 calculateCRC()
			{
				uint8* currentPosition = (uint8*)this;
				uint32 CRC_Retval = 0;
				for(uint8 bytesSummed = 0; bytesSummed < (sizeof(FileTable)-sizeof(_CRC)); ++bytesSummed)
				{
					CRC_Retval += *(currentPosition);
					++currentPosition;
				}
				return CRC_Retval;
			}

			bool checkCRC()
			{
				return calculateCRC()==_CRC;
			}

			void setCRC()
			{
				_CRC = calculateCRC();
			}

			MemoryAddress getSectorStart(uint16 sectorNumber)
			{
				MemoryAddress dataStartPoint = _startPointer._address + _fileSystemUnity._numberOfSectors*sizeof(FileEntry) + sizeof(uint32);
				return dataStartPoint+ _fileSystemUnity._sectorSize*sectorNumber;
			}

			void format()
			{

				for(MemoryAddress index = 0; index < _fileSystemUnity._numberOfSectors;index++)
				{
				    FileEntry fileEntry;
				    fileEntry._FileId = FileEntry::FREE_CLUSTER;
				    fileEntry._nextSectorNumber = FileEntry::INVALID_SECTOR;
				    _startPointer[index] = fileEntry;
				}
			}

			FileDescriptor<FileTable_MemoryEngine> getFile(uint16 fileId)
			{
				for(uint16 sector = 0; sector < _fileSystemUnity._numberOfSectors; sector++)
				{
					if(((FileEntry)_startPointer[sector])._FileId == fileId)
					{
						return FileDescriptor<FileTable_MemoryEngine>(sector, _fileSystemUnity);
					}
				}
				//did not found
				return FileDescriptor<FileTable_MemoryEngine>(FileDescriptor<FileTable_MemoryEngine>::INVALID_FILE,_fileSystemUnity);
			}

			uint16 getNextSector(uint16 sectorNumber)
			{
				return ((FileEntry)_startPointer[sectorNumber])._nextSectorNumber;
			}

			uint16 allocNewSector(uint16 sectorNumber)
			{
			    FileEntry fileEntry = _startPointer[sectorNumber];
			    fileEntry._nextSectorNumber = findFreeSector();
			    _startPointer[sectorNumber] = fileEntry;

			    fileEntry = _startPointer[fileEntry._nextSectorNumber];
			    fileEntry._FileId = FileEntry::MIDLE_OF_CLUSTER;
                fileEntry._nextSectorNumber = FileEntry::END_OF_CLUSTER;
                _startPointer[fileEntry._nextSectorNumber] = fileEntry;

                return fileEntry._nextSectorNumber;
			}

			uint16 findFreeSector(void)
			{
                for(uint16 sector = 0; sector < _fileSystemUnity._numberOfSectors; sector++)
                {
                    if(((FileEntry)_startPointer[sector])._FileId == FileEntry::FREE_CLUSTER)
                    {
                        return sector;
                    }
                }
                return FileEntry::INVALID_SECTOR;
			}

			FileDescriptor<FileTable_MemoryEngine> createFile(uint16 _FileId)
			{
				FileDescriptor<FileTable_MemoryEngine> file = getFile(_FileId);
				if(file.isValid() == false)
				{
					for(uint16 sector = 0; sector < _fileSystemUnity._numberOfSectors; sector++)
					{
						if(((FileEntry)_startPointer[sector])._FileId == FileEntry::FREE_CLUSTER)
						{
						    FileEntry fileEntry;
						    fileEntry._FileId = _FileId;
						    fileEntry._nextSectorNumber = FileEntry::END_OF_CLUSTER;
						    _startPointer[sector] = fileEntry;
							return FileDescriptor<FileTable_MemoryEngine>(sector, _fileSystemUnity);
						}
					}
				}

				return file;
			}
		};

		//this file descriptor is not a normal file it has an unlimited size, and will allocate memory as needed
		//it doesn't have a end it will always add allocate a sector per time
		template<class FileDescriptor_MemoryEngine>
		class FileDescriptor
		{
		    uint16 _startOfFileSector;
			uFAT& _fileSystemUnity;
		public:
			static constexpr MemoryAddress INVALID_FILE = 0xFFFF;

			FileDescriptor(uint16 sector, uFAT& fileSystemUnity) : _startOfFileSector(sector), _fileSystemUnity(fileSystemUnity)
			{
			}

			FileDescriptor<FileDescriptor_MemoryEngine>& operator=(const FileDescriptor<FileDescriptor_MemoryEngine>& file)
			{
				_startOfFileSector = file._startOfFileSector;
				return *this;
			}

			template<class T = uint8>
			RTOS::MemoryValue<FileDescriptor_MemoryEngine, T> operator[](uint16 index)
			{
			    uint16 relativeSector = index / _fileSystemUnity._sectorSize;
			    uint16 absoluteSector = _startOfFileSector;

			    for(;relativeSector>0;--relativeSector)
			    {
			        uint16 auxSector;
			        auxSector = _fileSystemUnity._fileTable.getNextSector(absoluteSector);
			        if(auxSector != FileTable<FileDescriptor_MemoryEngine>::FileEntry::END_OF_CLUSTER)
			        {
			            absoluteSector = auxSector;
			        }
			        else
			        {
			            absoluteSector = _fileSystemUnity._fileTable.allocNewSector(absoluteSector);
			        }
			    }

				return RTOS::MemoryValue<FileDescriptor_MemoryEngine, T>(_fileSystemUnity._fileTable.getSectorStart(absoluteSector)+index, _fileSystemUnity._memory);
			}

			bool isValid()
			{
				if (_startOfFileSector == INVALID_FILE)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		};


        friend class FileDescriptor<MemoryEngine>;
        friend class FileTable<MemoryEngine>;

		FileDescriptor<MemoryEngine> openFile(uint16 FileId)
		{
			return _fileTable.getFile(FileId);
		}

		FileDescriptor<MemoryEngine> createFile(uint16 FileId)
		{
			return _fileTable.createFile(FileId);
		}


		void FormatDevice(uint8 memoryType,uint16 numberOfSectors,uint16 sectorSize, STRING name)
		{
	        _numberOfSectors = numberOfSectors;
	        _sectorSize = sectorSize;

			BootSection boot;

			boot._signature = uFatSignature;
			boot._sectorSize = sectorSize;
			boot._numberOfSectors = numberOfSectors;
			boot._memoryType = memoryType;
			boot.setName(name);
			boot._FileTableAddress = _startAddress + sizeof(BootSection);
			boot.setCRC();

			_fileTable.config(boot._FileTableAddress);

			_fileTable.format();
			_fileTable.setCRC();


			_memory.write(_startAddress, (uint8*)&boot, sizeof(boot));
		}

		void bootUpDevice()
		{
		    if(isuFat())
		    {
		        BootSection boot = _boot;
		        _fileTable.config(boot._FileTableAddress);

		        _numberOfSectors = boot._numberOfSectors;
		        _sectorSize = boot._sectorSize;
		    }
		}

		bool isuFat()
		{
		    return ((BootSection)(_boot))._signature == uFatSignature;
		}

		uFAT(MemoryEngine& memory, MemoryAddress startAddress) : _memory(memory), _startAddress(startAddress), _boot(_startAddress, _memory), _fileTable(*this)
		{
		    bootUpDevice();
		}

		typedef FileDescriptor<MemoryEngine> uFatFileDescriptor;

	private:
		MemoryEngine& _memory;
		MemoryAddress _startAddress;
		RTOS::MemoryValue<MemoryEngine, BootSection> _boot;
		FileTable<MemoryEngine> _fileTable;

		//cache, here so we dont need to keep reading from memory
        uint16 _numberOfSectors;
        uint16 _sectorSize;
	};
}



#endif /* RTOS_SOURCE_CORE_MEMORYMANAGEMENT_UFAT_H_ */
