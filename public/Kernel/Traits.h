/*
 * traits.h
 *
 *  Created on: 2 de set de 2018
 *      Author: breno
 */

#ifndef TRAITS_H_
#define TRAITS_H_

#include <RTOSCodeStyle.h>

enum
{
	T_DEBUG,
	T_LOG,
	T_INFO,
	T_COMMUNICATION_PROTOCOL,
	T_COMMUNICATION_NRF,
	T_TIME,
	T_NETWORK,
	T_KERNEL,
	T_SCHEDULLER,
	T_THREAD,
	T_SERIAL_COMMUNICATION,
	T_NVDATA_MEMORY,
};

template<int T>
struct Traits
{
	static const bool dummy = false;
};

template<> struct Traits<T_DEBUG>
{
	static const bool debug = true;
	static const bool hysterical_debug = false;
	static const bool trace = false;
};

template<> struct Traits<T_LOG>
{
	static const bool log = true;
};

template<> struct Traits<T_INFO>
{
	static const bool info = true;
};

template<> struct Traits<T_COMMUNICATION_PROTOCOL>
{
	static const uint8 address = 1;
	static const uint8 queueSize = 10;
	static const uint32 AckWaitTime = 300000;		//Microseconds
	static const uint8 AckResendTrys = 3;
};

template<> struct Traits<T_COMMUNICATION_NRF>
{
	static const int publicAddress = 1;
};

template<> struct Traits<T_TIME>
{
	enum
	{
		TIMER_INTERNAL,
		TIMER_SYSTICK,
		TIMER_MILLIS,
	};
	static const int TimerEngine = TIMER_SYSTICK;
	static const int TickFrequency = 1000;
};

template<> struct Traits<T_NETWORK>
{
	enum
	{
		GATEWAY,
		NODE,
	};
	static const int networkRole = GATEWAY;
};
template<> struct Traits<T_KERNEL>
{
	enum
	{
		SYSTICK_ENGINE,
		THREAD_ENGINE,
	};
	static const int kernelEngine = SYSTICK_ENGINE;
};
template<> struct Traits<T_SCHEDULLER>
{
	enum
	{
		PRIORITY,
	};
	static const int schedullePolicy = PRIORITY;
	static const unsigned int maximumThreads = 20;
	static const unsigned long int tickPeriod = 100000;
};
template<> struct Traits<T_THREAD>
{
	static const int defaultStackSize = 200;
};
template<> struct Traits<T_SERIAL_COMMUNICATION>
{
	static const uint32 defaultBaudRate = 115200;
};
template<> struct Traits<T_NVDATA_MEMORY>
{
	enum
	{
		EEPROM_STD,
		EEPROM_NONE,
	};
	static const int eepromEngine = EEPROM_STD;
};



#endif /* TRAITS_H_ */
