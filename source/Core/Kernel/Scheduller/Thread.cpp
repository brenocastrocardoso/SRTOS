/**
 * Project Untitled
 */


#include <Hal/Archtecture/SchedullerHardwareInterface.h>
#include <IO/OStream.h>
#include <IO/StdOIStream.h>
#include <Kernel/Interrupt.h>
#include <Kernel/Kernel.h>
#include <Kernel/Scheduller/Scheduller.h>
#include <Kernel/Thread.h>

/**
 * Thread implementation
 */

ThreadEngine::ThreadEngine(ThreadEngine::ThreadFunc handler,void* param, int priority, StackType* stackSize) : _priority(priority)
{
	_state = SUSPENDED;
	Scheduller::getScheduller().addThread((*this));

	_stackPtrTop = (StackType*)stackSize;

	_stackPtrTop = HAL::SchedullerHardwareInterface::initStack(_stackPtrTop, handler, param);
}

ThreadEngine& ThreadEngine::getCurrent(void)
{
	return Scheduller::getScheduller().getCurrentThread();
}

void ThreadEngine::wakeUp(void* threadParam)
{
	Interrupt::AtomicFunction atomic;
	ThreadEngine* threadToWake = (ThreadEngine*)threadParam;
	threadToWake->resume();
}


void ThreadEngine::suspend()
{
	Interrupt::AtomicFunction atomic;
	getCurrent()._state = SUSPENDED;
	Scheduller::getScheduller().reschedulle();
}

void ThreadEngine::sleep(Alarm::Microseconds toSleep)
{
	Interrupt::AtomicFunction atomic;
	ThreadEngine* ptr = &getCurrent();
	Alarm alarm(toSleep, ThreadEngine::wakeUp, ptr);
	ptr->_state = ThreadEngine::SUSPENDED;
	Scheduller::getScheduller().reschedulle();
}


void ThreadEngine::info()
{
	RTOS::Kernel::cout << "state: " << (int)_state << " " << "end: " << RTOS::STREAM::hex << (unsigned long)this << " ";
}


void ThreadEngine::stackInfo()
{
	RTOS::Kernel::cout << "stackPtr: " << (int)_stackPtrTop << RTOS::STREAM::endl;
}

