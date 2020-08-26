/*
 * AlarmManager.cpp
 *
 *  Created on: 14 de jul de 2020
 *      Author: breno
 */


#include <Utils/TimeKeeper/Alarm.h>
#include <MemoryManagement/StaticAllocator.h>
#include <MemoryManagement/list.h>
#include <Utils/TimeKeeper/Alarm/AlarmManager.h>


RTOS::list<Alarm, StaticAllocator<Alarm, AlarmManager::MAXIMUM_ALARMS>> AlarmManager::_alarmQueue;
