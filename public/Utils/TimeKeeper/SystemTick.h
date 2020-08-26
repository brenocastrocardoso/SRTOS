/*
 * SystemTick.h
 *
 *  Created on: 7 de dez de 2019
 *      Author: breno
 */

#ifndef RTOS_SYSTEMTICK_H_
#define RTOS_SYSTEMTICK_H_

#include <Kernel/Interrupt.h>
#include <Kernel/Traits.h>


class SystemTick
{
public:

	typedef unsigned long Tick;

	static constexpr unsigned long TickFrequency = Traits<T_TIME>::TickFrequency;

	static void SystickHandler(void);
	static void countTick(void);
	static Tick getTick();

private:
	static Tick _tickCounter;
};



#endif /* RTOS_SYSTEMTICK_H_ */
