/*
 * Timer.h
 *
 *  Created on: 7 de dez de 2019
 *      Author: breno
 */

#ifndef RTOS_TIMER_H_
#define RTOS_TIMER_H_


#include <RTOS.h>
#include <Utils/TimeKeeper/SystemTick.h>
#include <Utils/TimeKeeper/TickTimer.h>

/*
 *  @brief public Interface that changes from tick to microseconds the timer API
 */
class Timer : public TickTimer
{
public:
	typedef uint32 Seconds;
	typedef uint32 Microseconds;
	typedef uint32 Miliseconds;

private:
	static constexpr uint32 MicrosecondsResolution = 1000000;

	static constexpr uint32 MicrosecondsInATick = MicrosecondsResolution/TickFrequency;

	static SystemTick::Tick convertMicrosecondsInTick(Microseconds microseconds)
	{
		return microseconds/MicrosecondsInATick;
	}

	static Microseconds convertTickInMicroseconds(SystemTick::Tick tick)
	{
		return tick*MicrosecondsInATick;
	}

public:

	Timer() {}

	/*
	 * @brief Constructor of Timer class, this class make a object that will count down until its completition
	 */
	Timer(Microseconds microsecondsToCount) : TickTimer(convertMicrosecondsInTick(microsecondsToCount))
	{}

	/*
	 * @brief Set the time the timer will count until completition
	 *
	 * @param The time in Microseconds the Time will count until completition
	 *
	 */
	void setTimer(Microseconds microsecondsToCount)
	{
		TickTimer::setTimer(convertMicrosecondsInTick(microsecondsToCount));
	}

	/*
	 * @brief Returns the remaining time until the time stops count
	 *
	 * @return The remaining time in microseconds to timer stops count
	 */
	Microseconds timeToEnd() const
	{
		return convertTickInMicroseconds(TickTimer::timeToEnd());
	}

	/*
	 * @brief Static Function to convert Miliseconds in Microseconds
	 *
	 * @param The value to be converted in microseconds
	 *
	 * @return The converted value of the parameter in microseconds
	 */
	static constexpr Microseconds convertMilisecondsInMicroseconds(Miliseconds miliseconds)
	{
		return miliseconds*1000;
	}

	/*
	 * @brief Static Function to convert Seconds in Microseconds
	 *
	 * @param The value to be converted in seconds
	 *
	 * @return The converted value of the parameter in microseconds
	 */
	static constexpr Miliseconds convertSecondsInMicroseconds(Seconds seconds)
	{
		return seconds*1000000;
	}
};






#endif /* RTOS_TIMER_H_ */
