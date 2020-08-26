/*
 * Serial.h
 *
 *  Created on: 18 de jul de 2020
 *      Author: breno
 */

#ifndef DRIVERS_SERIAL_H_
#define DRIVERS_SERIAL_H_

#include <RTOS.h>

#include <Utils/Buffer.h>
#include <IO/OStreamEngine.h>
#include <Hal/Peripheral/SerialDefs.h>

#include <Arduino.h>

namespace DRIVERS
{
	class DriverSerial : public HAL::SerialDefs
	{
	public:
		DriverSerial(uint32 baudrate, CHANNELS channel)
		{
			Serial.begin(baudrate);
		}

		void write(uint8* payload, uint8 payloadSize)
		{
			Serial.write(payload, payloadSize);
		}

		void write(String payload)
		{
			Serial.print(payload);
		}
		template<class T>
		void write(T payload)
		{
		    Serial.print(payload);
		}
	};
}



#endif /* RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_SERIAL_H_ */
