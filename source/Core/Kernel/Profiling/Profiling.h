/*
 * RTC.h
 *
 *  Created on: 21 de jul de 2020
 *      Author: breno
 */

#ifndef PROFILING_H
#define PROFILING_H

#include <Kernel/PeriodicThread.h>
#include <Utils/TimeKeeper/SystemTick.h>

namespace RTOS {
	class Profiling
	{
//		typedef RTOS::SystemTick::Tick Tick;
//
//		typedef RTOS::PeriodicThread::Microseconds Microseconds;
//		static constexpr Microseconds MicrosecondsInASeconds = 1000000;
//
//	public:
//		typedef Timepoint::Timestamp Timestamp;
//
//		RealTimeClock(Timestamp calibrateTime) : _time(calibrateTime), _thread(RealTimeClockHandler, this, 150, MicrosecondsInASeconds)
//		{
//			_thread.start();
//		}
//
//		static void RealTimeClockHandler(void* obj)
//		{
//			while(1)
//			{
//				++(((RealTimeClock*)obj)->_time);
//				RTOS::PeriodicThread::waitNext();
//			}
//		}
//
//		Timepoint getTime()
//		{
//			return _time;
//		}
	};
};


#endif /* PROFILING_H */
