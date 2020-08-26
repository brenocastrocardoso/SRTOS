/*
 * RTC.h
 *
 *  Created on: 21 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_PUBLIC_UTILS_RTC_RTC_H_
#define RTOS_PUBLIC_UTILS_RTC_RTC_H_

#include "DateTime.h"
#include <Kernel/PeriodicThread.h>

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
        ++(((RealTimeClock*)obj)->_time);
    }

    Timepoint getTime()
    {
        return _time;
    }
};

typedef RealTimeClock RTC;


#endif /* RTOS_PUBLIC_UTILS_RTC_RTC_H_ */
