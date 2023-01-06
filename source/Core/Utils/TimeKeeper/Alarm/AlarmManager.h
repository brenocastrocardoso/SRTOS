/*
 * Alarm.h
 *
 *  Created on: 16 de dez de 2019
 *      Author: breno
 */

#ifndef ALARMMANAGER_H_
#define ALARMMANAGER_H_

#include <MemoryManagement/list.h>
#include <MemoryManagement/StaticAllocator.h>
#include <Utils/Debug/Debug.h>
#include <Utils/TimeKeeper/Alarm.h>

class AlarmManager
{
private:
	static constexpr uint8 MAXIMUM_ALARMS = 20;

public:

	typedef RTOS::list<Alarm, StaticAllocator<Alarm, MAXIMUM_ALARMS>> AlarmList;

	static void AlarmManagerHandler(void)
	{
		while (_alarmQueue.empty() == false &&
			   _alarmQueue.front().getState() != Alarm::TIMER_RUNNING)
		{
			_alarmQueue.front()();
			_alarmQueue.pop_front();
			hdebug << F("_alarmQueue.pop_front()") << DEBUG::endl;
			hdebug << F("_alarmQueue.size()") << (int)_alarmQueue.size() << DEBUG::endl;
		}
	}

	static void EnqueueAlarm(const Alarm& alarm)
	{
		hdebug << F("static void EnqueueAlarm(const Alarm& alarm)") << DEBUG::endl;
		hdebug << F("_alarmQueue.size()") << (int)_alarmQueue.size() << DEBUG::endl;
		AlarmList::Iterator current = _alarmQueue.begin();

		if (_alarmQueue.empty() == true ||
			(*current) > alarm)
		{
			_alarmQueue.push_front(alarm);
			hdebug << F("_alarmQueue.push_front(alarm);") << DEBUG::endl;
		}
		else
		{
			++current;
			//insert item sorted
			for(uint8 index = 1;index<MAXIMUM_ALARMS;index++)
			{
				if (alarm > *(current) &&
				    current != _alarmQueue.end())
				{
					++current;
				}
				else
				{
					_alarmQueue.insert(current, alarm);
					break;
				}
			}

		}
		hdebug << F("_alarmQueue.size()") << (int)_alarmQueue.size() << DEBUG::endl;
	}

private:
	static AlarmList _alarmQueue;
};


#endif /* ALARMMANAGER_H_ */
