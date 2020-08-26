/*
 * SchedullerHardwareInterface.h
 *
 *  Created on: 8 de ago de 2020
 *      Author: breno
 */

#ifndef RTOS_SOURCE_HARDWAREINTERFACE_HAL_ARCHTECTURE_SCHEDULLERHARDWAREINTERFACE_H_
#define RTOS_SOURCE_HARDWAREINTERFACE_HAL_ARCHTECTURE_SCHEDULLERHARDWAREINTERFACE_H_

#include <Drivers/Archtecture/SchedullerHardwareInterface.h>
#include <RTOSCodeStyle.h>


namespace HAL
{
	class SchedullerHardwareInterface
	{
	public:

		typedef uint8 StackType;
		typedef ThreadEngine::ThreadFunc ThreadFunc;
		typedef void* TCB;

		static void startScheduler( void )
		{
			DRIVERS::SchedullerHardwareInterface::startScheduler();
		}

		static void contextSwitch(TCB* next)
		{
			DRIVERS::SchedullerHardwareInterface::contextSwitch(next);
		}

		static StackType* initStack(StackType* stackTop, ThreadFunc func, void * param)
		{
			return DRIVERS::SchedullerHardwareInterface::initStack(stackTop, func, param);
		}
	};

}


#endif /* RTOS_SOURCE_HARDWAREINTERFACE_HAL_ARCHTECTURE_SCHEDULLERHARDWAREINTERFACE_H_ */
