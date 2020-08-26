/*
 * Interrupt.h
 *
 *  Created on: 7 de dez de 2019
 *      Author: breno
 */

#ifndef DRIVERS_INTERRUPT_H_
#define DRIVERS_INTERRUPT_H_

#include <Arduino.h>

class Interrupt
{
public:
	//This class is hepful so we dont forget to reenable the interrupts
	class AtomicFunction
	{
	public:
		AtomicFunction()
		{
			Interrupt::disableInterrupt();
		}

		~AtomicFunction()
		{
			Interrupt::enableInterrupt();
		}

	};

	static void disableInterrupt(void)
	{
		noInterrupts();
	}

	static void enableInterrupt(void)
	{
		interrupts();
	}

};



#endif /* DRIVERS_INTERRUPT_H_ */
