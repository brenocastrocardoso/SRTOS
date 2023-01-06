/*
 * AlarmManager.cpp
 *
 *  Created on: 14 de jul de 2020
 *      Author: breno
 */


#include <MemoryManagement/list.h>
#include <MemoryManagement/StaticAllocator.h>
#include <Utils/TimeKeeper/Alarm/AlarmManager.h>
#include <Utils/TimeKeeper/Alarm.h>


RTOS::list<Alarm, StaticAllocator<Alarm, AlarmManager::MAXIMUM_ALARMS>> AlarmManager::_alarmQueue;
