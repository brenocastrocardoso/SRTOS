/*
 * Serial.h
 *
 *  Created on: 18 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_SERIAL_H_
#define RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_SERIAL_H_

#include <Drivers/Peripheral/Serial.h>
#include <Hal/Peripheral/SerialDefs.h>
#include <IO/OStreamEngine.h>
#include <RTOS.h>
#include <Utils/Buffer.h>
#include <Kernel/Traits.h>

#include STRING_INCLUDE

namespace HAL
{
	class Serial : public HAL::SerialDefs
	{
		typedef DRIVERS::DriverSerial SerialDriver;
	public:

		Serial(uint32 baudrate = Traits<T_SERIAL_COMMUNICATION>::defaultBaudRate, CHANNELS channel = CHANNEL_1) : _serial(baudrate, channel)
		{}

		void write(uint8* payload, uint8 payloadSize)
		{
			_serial.write(payload, payloadSize);
		}

		template<class T>
		void write(T payload)
		{
			_serial.write(payload);
		}

	private:
		SerialDriver _serial;
	};
}



#endif /* RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_SERIAL_H_ */
