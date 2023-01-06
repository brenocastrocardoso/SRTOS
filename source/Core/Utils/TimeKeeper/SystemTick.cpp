/*
 * SystemTick.cpp
 *
 *  Created on: 7 de dez de 2019
 *      Author: breno
 */


#include <Kernel/Kernel.h>
#include <Utils/TimeKeeper/SystemTick.h>


SystemTick::Tick SystemTick::_tickCounter = 0;

void SystemTick::countTick(void)
{
	Interrupt::AtomicFunction atomic;
	//if ticks overflow it will restart from 0
	++_tickCounter;
}

SystemTick::Tick SystemTick::getTick()
{
	Tick retval;
	Interrupt::AtomicFunction atomic;
	switch (Traits<T_TIME>::TimerEngine)
	{
		case Traits<T_TIME>::TIMER_MILLIS:
//			retval = millis();
			break;
		case Traits<T_TIME>::TIMER_INTERNAL:
		case Traits<T_TIME>::TIMER_SYSTICK:
			retval = _tickCounter;
			break;
		default:
			break;
	}
	return retval;
}

void SystemTick::SystickHandler(void)
{
	//TODO make it be on compile time
	if (Traits<T_TIME>::TimerEngine == Traits<T_TIME>::TIMER_SYSTICK)
	{
		countTick();
	}

	if (Traits<T_KERNEL>::kernelEngine == Traits<T_KERNEL>::SYSTICK_ENGINE)
    {
	    RTOS::Kernel::KernelSystickHandler();
    }

}
