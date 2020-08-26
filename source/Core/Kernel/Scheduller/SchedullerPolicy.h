/**
 * Project Untitled
 */


#ifndef _SCHEDULLERPOLICY_H
#define _SCHEDULLERPOLICY_H

#include <Kernel/Thread.h>
#include <Kernel/Traits.h>
#include <MemoryManagement/list.h>
#include <MemoryManagement/StaticAllocator.h>
#include <RTOS.h>

class SchedullerPolicyBase
{
public: 

	static constexpr uint8 MAXIMUM_THREADS = Traits<T_SCHEDULLER>::maximumThreads;

	typedef RTOS::list<ThreadEngine*, StaticAllocator<ThreadEngine*, MAXIMUM_THREADS>> ThreadList;
    
};

class SchedullerPolicyPriority: public SchedullerPolicyBase
{
public:
	ThreadEngine* chooseNext(ThreadList& threadlist)
	{
			ThreadEngine* choosen = *(threadlist.begin());

			for(ThreadEngine* currentThread : threadlist)
			{
				if (currentThread->_state != ThreadEngine::SUSPENDED)
				{
					if (currentThread->_priority >  choosen->_priority ||
						choosen->_state == ThreadEngine::SUSPENDED)
					{
						choosen = currentThread;
					}
				}
			}
			return choosen;
	}
};

template<int schedullerPolicy>
class SchedullerPolicyEngine {};
template<>
class SchedullerPolicyEngine<Traits<T_SCHEDULLER>::PRIORITY>: public SchedullerPolicyPriority {};

class SchedullerPolicy : public SchedullerPolicyEngine<Traits<T_SCHEDULLER>::schedullePolicy>
{};



#endif //_SCHEDULLERPOLICY_H
