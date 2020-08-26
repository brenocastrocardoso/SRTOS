/*
 * Kernel.cpp
 *
 *  Created on: 14 de jul de 2020
 *      Author: breno
 */

#include <Arduino.h>
#include <Hal/Archtecture/SchedullerHardwareInterface.h>
#include <Kernel/Kernel.h>
#include <Kernel/Traits.h>
#include <Utils/TimeKeeper/Alarm/AlarmManager.h>
#include <Utils/TimeKeeper/TickChronometer.h>
#include <Utils/TimeKeeper/TickTimer.h>
#include <Utils/TimeKeeper/Timer.h>

using namespace RTOS;

void appThread(void*);

bool Kernel::bootUpCompleted = false;

Kernel::KernelOstream Kernel::cout;
Kernel::KernelEEprom* Kernel::UFat;
Kernel* Kernel::KernelDebug;


void Kernel::startUp() {
	Kernel rtosKernel;

	Kernel::bootUpCompleted = true;

	rtosKernel._scheduller.start();
}

void Idle(void*)
{
	Timer printTime(500000);
	while(1)
	{
		if(printTime.getState() == Timer::TIMER_DONE)
		{
			printTime.restart();
			hdebug << "Idle" << RTOS::STREAM::endl;
		}
	}
}

void Kernel::KernelSystickHandler(void)
{
	if (Kernel::bootUpCompleted)
	{
		static Timer alarmCheckTimer(1000);
		if (alarmCheckTimer.getState() == Timer::TIMER_DONE)
		{
			alarmCheckTimer.restart();
			AlarmManager::AlarmManagerHandler();
		}

		Kernel::KernelDebug->_scheduller.runScheduller();

	}

}

Kernel::Kernel() :
        _idleThread(Idle, nullptr, 0),
        _appThread(appThread, 100),
        _stdSerial(),
        _stdOstream(_stdSerial),
        _stdEeprom(),
        _stdUFat(_stdEeprom, 0)
{
	cout.setStream(&_stdOstream);

    KernelDebug = this;
    if (_stdUFat.isuFat() == false)
    {
        _stdUFat.FormatDevice(KernelEEprom::BootSection::EEPROM, 16, 56, "RTOS EEprom");
    }

    UFat = &_stdUFat;

    _idleThread.start();
	_appThread.start();

	cout << "Kernel Start" << STREAM::endl;
}

