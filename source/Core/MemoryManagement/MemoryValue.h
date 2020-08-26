/*
 * MemoryValue.h
 *
 *  Created on: 1 de ago de 2020
 *      Author: breno
 */

#ifndef RTOS_SOURCE_CORE_MEMORYMANAGEMENT_MEMORYVALUE_H_
#define RTOS_SOURCE_CORE_MEMORYMANAGEMENT_MEMORYVALUE_H_

#include <RTOS.h>

namespace RTOS
{
	template<class MemoryEngine, class DataType = uint8>
	class MemoryValue
	{
	public:
		typedef uint32 MemoryAddress;

		MemoryAddress _address;
		MemoryEngine& _memory;

		template<class MemoryPointer_MemoryEngine>
		class MemoryPointer;

		MemoryValue(MemoryAddress address, MemoryEngine& engine): _address(address), _memory(engine)
		{}

        void setAddress(MemoryAddress address)
        {
            _address = address;
        }

//		operator uint32()
//		{
//			uint8 buffer[sizeof(DataType)];
//			_memory.read(_address, buffer, sizeof(DataType));
//			return *((DataType*)buffer);
//		}

		void operator=(const DataType value)
		{
			_memory.write(_address, (uint8*)&value, sizeof(DataType));
		};

		operator DataType()
		{
			uint8 buffer[sizeof(DataType)];
			_memory.read(_address, buffer, sizeof(DataType));
			return *((DataType*)buffer);
		}

		MemoryPointer<MemoryEngine> operator&(void)
		{
			return (_address, _memory);
		};
	};

	template<class MemoryEngine, class pointerType = uint8>
	class MemoryPointer
	{
	public:
		typedef uint32 MemoryAddress;

		MemoryAddress _address;
		MemoryEngine& _memory;

		MemoryPointer(MemoryAddress address, MemoryEngine& engine): _address(address), _memory(engine)
		{}

		void setAddress(MemoryAddress address)
		{
		    _address = address;
		}

		RTOS::MemoryValue<MemoryEngine, pointerType> operator[](const MemoryAddress index)
		{
			return RTOS::MemoryValue<MemoryEngine, pointerType>(_address+(index*sizeof(pointerType)), _memory);
		}

		RTOS::MemoryValue<MemoryEngine, pointerType> operator*(void)
		{
			return RTOS::MemoryValue<MemoryEngine, pointerType>(_address, _memory);
		}

		MemoryPointer operator+(const MemoryAddress value)
		{
			return (_address+(value*sizeof(pointerType)), _memory);
		}

		MemoryPointer& operator++(void)
		{
			_address += sizeof(pointerType);
			return (*this);
		}

		MemoryPointer operator-(const MemoryAddress value)
		{
			return (_address-(value*sizeof(pointerType)), _memory);
		}

		MemoryPointer& operator--(void)
		{
			_address -= sizeof(pointerType);
			return (*this);
		}
	};
}





#endif /* RTOS_SOURCE_CORE_MEMORYMANAGEMENT_MEMORYVALUE_H_ */
