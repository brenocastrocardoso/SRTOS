/**
 * Project Untitled
 */

#ifndef _SCHEDULLER_H
#define _SCHEDULLER_H

#include <HardwareSerial.h>
#include <Arduino.h>
#include <Hal/Archtecture/SchedullerHardwareInterface.h>
#include <Kernel/Scheduller/SchedullerPolicy.h>
#include <Kernel/Thread.h>
#include <Kernel/Traits.h>
#include <MemoryManagement/list.h>


extern volatile HAL::SchedullerHardwareInterface::TCB * volatile pxCurrentTCB;


class Scheduller: public SchedullerPolicy
{
public:

	void addThread(ThreadEngine& thread)
	{
		if (_threadList.empty())
		{
			_currentThread = &thread;
		}
		_threadList.push_back(&thread);
	}

	void runScheduller()
	{
		if (_reschedulleTimer.getState() == Timer::TIMER_DONE)
		{
			_reschedulleTimer.restart();
//			Serial.println("reschedulle");
			reschedulle();
		}
	}

	void reschedulle()
	{
		ThreadEngine* next = chooseNext(_threadList);

		if (next != _currentThread)
		{
			if (_currentThread->_state == ThreadEngine::RUNNING)
			{
				_currentThread->_state = ThreadEngine::WAITING;
			}
			contextSwitch(next);
			if(_currentThread->checkStackOverflow())
			{
				//TODO FIX THAT
				_currentThread->info();
				_currentThread->stackInfo();
				Serial.println("WARNING");
			}
			_currentThread->_state = ThreadEngine::RUNNING;

			applicationSpace();
		}
	}

//	void schedulleToIdle()
//	{
//		_currentThread->_state = ThreadEngine::SUSPENDED;
//		_currentThread = threadVectorDebug[0];
//		_currentThread->_state = ThreadEngine::RUNNING;
//		HAL::SchedullerHardwareInterface::contextSwitch((void**)threadVectorDebug[0]->getStackPointer());
//	}

	void contextSwitch(ThreadEngine* next)
	{
		_currentThread = next;
	}

	void applicationSpace()
	{
		HAL::SchedullerHardwareInterface::contextSwitch((void**)_currentThread->getStackPointer());
//		_currentThread->run();
	}

	void start()
	{
		_reschedulleTimer.setTimer(Traits<T_SCHEDULLER>::tickPeriod);

		ThreadEngine* next = chooseNext(_threadList);
		contextSwitch(next);
		_currentThread->_state = ThreadEngine::RUNNING;

		auto iterator = _threadList.begin();
		Serial.println((unsigned long)(*iterator)->getStackPointer(), HEX);
		++iterator;
		Serial.println((unsigned long)(*iterator)->getStackPointer(), HEX);
		pxCurrentTCB = (void**)_currentThread->getStackPointer();

		HAL::SchedullerHardwareInterface::startScheduler();
	}

	static ThreadEngine& getCurrentThread()
	{
		return *(_schedullerRef->_currentThread);
	}

	static Scheduller& getScheduller()
	{
		return *(_schedullerRef);
	}

	Scheduller()
	{
		_currentThread = nullptr;
		_schedullerRef = this;
	}

private:
	ThreadList _threadList;
	ThreadEngine* _currentThread;
	Timer _reschedulleTimer;

	static Scheduller* _schedullerRef;
};

#endif //_SCHEDULLER_H
