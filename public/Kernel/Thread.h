/**
 * Project Untitled
 */


#ifndef _THREAD_H
#define _THREAD_H

#include <Kernel/Traits.h>
#include <RTOSCodeStyle.h>
#include <Utils/TimeKeeper/Alarm.h>

class ThreadEngine {

public:
	typedef enum
	{
		RUNNING,
		WAITING,
		SUSPENDED
	}THREAD_STATE;

	typedef void(*ThreadFunc)(void*);
	typedef uint8 StackType;

	const uint32 ThreadStackSignature = 0x44C0577C;

	THREAD_STATE _state;
	StackType* _stackPtrTop;
	int _priority;

	ThreadEngine(ThreadFunc handler,void* param, int priority, StackType* stackPointer);

	StackType** getStackPointer()
	{
		return &_stackPtrTop;
	}

	void start()
	{
		_state = WAITING;
	}


	void join();

	void resume();

	void info();

	static ThreadEngine& getCurrent(void);

	static void suspend();

	static void sleep(Alarm::Microseconds toSleep);


	static void wakeUp(void* threadParam);

	virtual bool checkStackOverflow() = 0;

	void stackInfo();

	virtual ~ThreadEngine()
	{
		//remove thread from scheduller here
	}
};

template<int stackSize = Traits<T_THREAD>::defaultStackSize>
class Thread: public ThreadEngine
{
	StackType _stack[stackSize + sizeof(ThreadStackSignature)];
public:
	typedef Thread::StackType StackType;

	Thread(ThreadFunc handler, int priority): ThreadEngine(handler, nullptr, priority,  &_stack[stackSize+ sizeof(ThreadStackSignature)])
	{
		memcpy(_stack, &ThreadStackSignature, sizeof(ThreadStackSignature));
		Serial.println((unsigned long)&(_stack), HEX);
	}

	virtual bool checkStackOverflow()
	{
		return memcmp(_stack, &ThreadStackSignature, sizeof(ThreadStackSignature));
	}

	Thread(ThreadFunc handler,void* param, int priority): ThreadEngine(handler,param, priority, &_stack[stackSize]){}
};

#endif //_THREAD_H
