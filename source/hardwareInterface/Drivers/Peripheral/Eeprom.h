/*
 * Eeprom.h
 *
 *  Created on: 26 de jul de 2020
 *      Author: breno
 */

#ifndef _DRIVERS_PERIPHERAL_EEPROM_H_
#define _DRIVERS_PERIPHERAL_EEPROM_H_


#include <RTOS.h>
#include <Arduino.h>
#include <EEPROM.h>

namespace DRIVERS
{
	class DriverEEprom
	{
	public:

		typedef uint16 MemoryAddress;

		DriverEEprom()
		{}

		void write(MemoryAddress initialAddress, uint8* payload, uint8 payloadSize)
		{
			MemoryAddress lastAddress = payloadSize + initialAddress;
			uint8 payloadAddress = 0;
			for(MemoryAddress addressIndex = initialAddress; addressIndex < lastAddress; ++addressIndex, ++payloadAddress)
			{
				write(addressIndex, payload[payloadAddress]);
			}
		}

		void write(MemoryAddress address, uint8 value)
		{
			EEPROM[address] = value;
		}

		uint8 read(MemoryAddress address)
		{
			return EEPROM[address];
		}

	};
}


#endif /* RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_EEPROM_H_ */
