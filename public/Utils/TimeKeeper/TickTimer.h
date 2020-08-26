/*
 * TickTimer.h
 *
 *  Created on: 13 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_PUBLIC_UTILS_TIMEKEEPER_TICKTIMER_H_
#define RTOS_PUBLIC_UTILS_TIMEKEEPER_TICKTIMER_H_

#include "../../../public/Utils/TimeKeeper/SystemTick.h"
#include "../../../public/Utils/TimeKeeper/TickChronometer.h"

class TickTimer : public TickChronometer
{
public:
	enum TIMER_STATUS
	{
		TIMER_STOPED,
		TIMER_RUNNING,
		TIMER_DONE,
	};

	TickTimer() : TickChronometer()
	{
		_status = TIMER_STOPED;
		_ticksToCount = 0;
	}

	TickTimer(SystemTick::Tick ticksToCount) : TickChronometer()
	{
		_status = TIMER_RUNNING;
		_ticksToCount = ticksToCount;
	}

	void setTimer(SystemTick::Tick ticksToCount)
	{
		restart();
		_ticksToCount = ticksToCount;
		_status = TIMER_RUNNING;
	}

	SystemTick::Tick timeToEnd() const
	{
		SystemTick::Tick retval = 0;
		if(getPassedTicks() < _ticksToCount)
		{
			retval = _ticksToCount - getPassedTicks();
		}
		return retval;
	}

	TIMER_STATUS getState()
	{
		if (_status == TIMER_RUNNING)
		{
			if (getPassedTicks() >= _ticksToCount)
			{
				_status = TIMER_DONE;
			}
		}
		return _status;
	}

	void restart(void)
	{
		TickChronometer::restart();
		_status = TIMER_RUNNING;
	}

	SystemTick::Tick _ticksToCount;
	TIMER_STATUS _status;
};


#endif /* RTOS_PUBLIC_UTILS_TIMEKEEPER_TICKTIMER_H_ */
