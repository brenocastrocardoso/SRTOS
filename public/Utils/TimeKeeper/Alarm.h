/*
 * Alarm.h
 *
 *  Created on: 16 de dez de 2019
 *      Author: breno
 */

#ifndef ALARM_H_
#define ALARM_H_



#include <MemoryManagement/StaticAllocator.h>
#include <Utils/TimeKeeper/Timer.h>


class Alarm : public Timer
{
public:

	typedef Timer::Microseconds Microseconds;

	typedef void(*Handler)(void*);

	Alarm(Microseconds toCount, Handler alarmHandler, void* paramPointer = nullptr);

	bool operator>(const Alarm& alarm) const;

	void operator()() const;

	Alarm();

	~Alarm();

private:
	void* _paramPointer;
	Handler _handler;
};

#endif /* ALARM_H_ */
