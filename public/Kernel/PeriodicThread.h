/*
 * PeriodicThread.h
 *
 *  Created on: 18 de jul de 2020
 *      Author: breno
 */

#ifndef _SCHEDULLER_PERIODICTHREAD_H_
#define _SCHEDULLER_PERIODICTHREAD_H_

#include <Kernel/Thread.h>

namespace RTOS
{
	class PeriodicThread : public Thread<200>
	{
	public:
		typedef Alarm::Microseconds Microseconds;
		PeriodicThread(ThreadFunc handler, int priority, Microseconds period) : Thread(handler, priority), _period(period), _timer(period)
		{}

		PeriodicThread(ThreadFunc handler, void* param, int priority, Microseconds period) : Thread(handler, param, priority), _period(period), _timer(period)
        {}

		static PeriodicThread& getCurrent(void)
		{
			return *(PeriodicThread*)&(Scheduller::getScheduller().getCurrentThread());
		}

		static void waitNext()
		{
			Interrupt::AtomicFunction atomic;
			PeriodicThread& thread = getCurrent();
			Microseconds timeToEnd = thread._timer.timeToEnd();
			thread._timer.setTimer(timeToEnd+thread._period);
			sleep(timeToEnd);
		}

		virtual ~PeriodicThread()
		{
		}

	private:
		Microseconds _period;
		Timer _timer;
	};


};
#endif /* RTOS_SOURCE_CORE_KERNEL_SCHEDULLER_PERIODICTHREAD_H_ */
