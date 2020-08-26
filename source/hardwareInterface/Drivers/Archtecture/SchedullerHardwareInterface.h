/*
 * SchedullerHardwareInterface.h
 *
 *  Created on: 8 de ago de 2020
 *      Author: breno
 */

#ifndef RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_ARCHTECTURE_SCHEDULLERHARDWAREINTERFACE_H_
#define RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_ARCHTECTURE_SCHEDULLERHARDWAREINTERFACE_H_

#include <Kernel/Thread.h>
#include <RTOS.h>

namespace DRIVERS
{
	class SchedullerHardwareInterface
	{
	public:
		typedef uint8 StackType;
		typedef ThreadEngine::ThreadFunc ThreadFunc;
		typedef void* TCB;

		static void startScheduler( void );
		static __attribute__ ( ( naked ) ) void contextSwitch(TCB* next);
		static StackType* initStack(StackType* stackTop, ThreadFunc func, void * param);
	};
};


#endif /* RTOS_SOURCE_HARDWAREINTERFACE_DRIVERS_ARCHTECTURE_SCHEDULLERHARDWAREINTERFACE_H_ */
