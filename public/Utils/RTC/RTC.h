/*
 * RTC.h
 *
 *  Created on: 21 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_PUBLIC_UTILS_RTC_RTC_H_
#define RTOS_PUBLIC_UTILS_RTC_RTC_H_

#include <Kernel/PeriodicThread.h>
#include <Utils/RTC/DateTime.h>

class RealTimeClock
{
    Timepoint _time;
    RTOS::PeriodicThread _thread;

    typedef RTOS::PeriodicThread::Microseconds Microseconds;
    static constexpr Microseconds MicrosecondsInASeconds = 1000000;

public:
    typedef Timepoint::Timestamp Timestamp;

    RealTimeClock(Timestamp calibrateTime) : _time(calibrateTime), _thread(RealTimeClockHandler, this, 150, MicrosecondsInASeconds)
    {
        _thread.start();
    }

    static void RealTimeClockHandler(void* obj)
    {
    	while(1)
    	{
			++(((RealTimeClock*)obj)->_time);
			RTOS::PeriodicThread::waitNext();
    	}
    }

    Timepoint getTime()
    {
        return _time;
    }
};

typedef RealTimeClock RTC;


#endif /* RTOS_PUBLIC_UTILS_RTC_RTC_H_ */
