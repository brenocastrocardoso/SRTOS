/*
 * Alarm.cpp
 *
 *  Created on: 17 de dez de 2019
 *      Author: breno
 */

#include <Utils/TimeKeeper/Alarm.h>
#include <Utils/TimeKeeper/Timer.h>
#include <MemoryManagement/StaticAllocator.h>
#include <Utils/TimeKeeper/Alarm/AlarmManager.h>


Alarm::Alarm(Microseconds toCount, Handler alarmHandler, void* paramPointer) : Timer(toCount), _paramPointer(paramPointer), _handler(alarmHandler)
{
	trace << F("Alarm::Alarm(Microseconds toCount, Handler alarmHandler)") << DEBUG::endl;
	AlarmManager::EnqueueAlarm(*this);
}

Alarm::Alarm() : _paramPointer(nullptr), _handler(nullptr) {}

bool Alarm::operator>(const Alarm& alarm) const
{
	trace << F(" operator>(const Alarm& alarm) const") << DEBUG::endl;
	if (Timer::timeToEnd() > alarm.timeToEnd())
	{
		return true;
	}
	return false;
}

void Alarm::operator()() const
{
	_handler(_paramPointer);
}

Alarm::~Alarm()
{
}
