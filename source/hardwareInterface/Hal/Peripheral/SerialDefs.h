/*
 * Serial.h
 *
 *  Created on: 18 de jul de 2020
 *      Author: breno
 */

#ifndef _SERIALDEFS_H_
#define _SERIALDEFS_H_

#include <RTOS.h>

#include <Drivers/Peripheral/Serial.h>
#include <IO/OStreamEngine.h>
#include <Utils/Buffer.h>

namespace HAL
{
	class SerialDefs
	{
	public:
		enum CHANNELS{
			CHANNEL_1,
			CHANNEL_2,
			CHANNEL_3,
			CHANNEL_4,
			CHANNEL_5,
		};
	};
}



#endif /* RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_PERIPHERAL_SERIAL_H_ */
