/*
 * TickChronometer.h
 *
 *  Created on: 7 de dez de 2019
 *      Author: breno
 */

#ifndef RTOS_TICKCHRONOMETER_H_
#define RTOS_TICKCHRONOMETER_H_

#include <RTOS.h>
#include <Utils/TimeKeeper/SystemTick.h>

/**
 * @brief This class implements a Chronometer that is always running
 */
class TickChronometer
{
public:
	static constexpr uint32 TickFrequency = SystemTick::TickFrequency;

	TickChronometer()
	{
		_tickReference = SystemTick::getTick();
	}

	SystemTick::Tick getPassedTicks() const
	{
		return SystemTick::getTick() - _tickReference;
	}

	void restart()
	{
		_tickReference = SystemTick::getTick();
	}

private:
	SystemTick::Tick _tickReference;
};


#endif /* RTOS_TICKCHRONOMETER_H_ */
