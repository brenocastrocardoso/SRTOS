
#ifndef MUTEX_H
#define MUTEX_H

#import <Kernel/Interrupt.h>

namespace RTOS
{

    class Mutex {
		bool locked;
        public:
    	Mutex() {
    		locked = false;
    	}
        void lock(){
        	 // call suspend after enabling
        	Interrupt::AtomicFunction atomic;
        	while(tryLock() == false) {
        		Scheduller::getCurrentThread().suspend();
        	}
        }

        void release(){
        	Interrupt::AtomicFunction atomic;
			locked = false;
        }

        private:
        bool tryLock(){
        	volatile bool lockValue = true;
        	{
        		Interrupt::AtomicFunction atomic;
        		lockValue = locked;
        		locked = true;
        	}
        	return (lockValue == false);
        }


    };
};

#endif
