/*
 * Eeprom.h
 *
 *  Created on: 26 de jul de 2020
 *      Author: breno
 */

#ifndef _HAL_PERIPHERAL_EEPROM_H_
#define _HAL_PERIPHERAL_EEPROM_H_


#include <Drivers/Peripheral/Eeprom.h>
#include <MemoryManagement/MemoryValue.h>
#include <RTOS.h>

namespace HAL
{
	class EEprom
	{
	public:
		typedef uint16 MemoryAddress;
		typedef DRIVERS::DriverEEprom DriverEEprom;

		EEprom()
		{}

		void write(MemoryAddress initialAddress, uint8* payload, uint8 payloadSize)
		{
			_eeprom.write(initialAddress, payload, payloadSize);
		}

		void write(MemoryAddress address, uint8 value)
		{
			_eeprom.write(address, &value, 1);
		}

		void read(MemoryAddress initialAddress, uint8* buffer, uint8 bufferSize)
		{
			MemoryAddress lastAddress = bufferSize + initialAddress;
			uint8 payloadAddress = 0;
			for(MemoryAddress addressIndex = initialAddress; addressIndex < lastAddress; ++addressIndex, ++payloadAddress)
			{
				buffer[payloadAddress] = read(addressIndex);
			}
		}


		uint8 read(MemoryAddress address)
		{
			return _eeprom.read(address);
		}

		RTOS::MemoryValue<EEprom> operator[](MemoryAddress address)
		{
			RTOS::MemoryValue<EEprom> retObj(address, (*this));
			return retObj;
		}

	private:
		DriverEEprom _eeprom;
	};
}


#endif /* RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_EEPROM_H_ */
