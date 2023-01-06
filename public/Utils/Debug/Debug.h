/*
 * debug.h
 *
 *  Created on: 2 de set de 2018
 *      Author: breno
 */

#ifndef APPLICATION_DEBUG_H_
#define APPLICATION_DEBUG_H_

#include <Kernel/Kernel.h>
#include <Kernel/Traits.h>
#include <RTOS.h>


static const char DEBUG_HEADER[] 	= {"DEBUG: "};
static const char TRACE_HEADER[] 	= {"TRACE: "};
static const char LOG_HEADER[] 		= {"LOG:   "};
static const char INFO_HEADER[]		= {"INFO:  "};
static const char BLANK_HEADER[]	= {"       "};

static const char* STRING_HEADER_LIST[] =
{
		DEBUG_HEADER,
		TRACE_HEADER,
		LOG_HEADER,
		INFO_HEADER
};

enum HEADER_ORDER
{
	DEBUG_HEADER_INDICE,
	TRACE_HEADER_INDICE,
	LOG_HEADER_INDICE,
	INFO_HEADER_INDICE,
};

class Commum_Debug
{
public:
    enum SPECIAL_CHARACTHERES
    {
        endl,
		tab,
		newline,
    };


};

template<int string_indice>
class Engine_Debug : public Commum_Debug
{

	RTOS::Kernel::KernelOstream& cout = RTOS::Kernel::cout;

	bool headerMark = true;
public:

    template<typename T>
    Engine_Debug & operator<<(T p)
    {
    	if ( headerMark )
    	{
    	    cout << STRING_HEADER_LIST[string_indice];
    		headerMark = false;
    	}
    	cout << p;
    	return *this;
    }

    Engine_Debug & operator<<(SPECIAL_CHARACTHERES p)
    {
    	switch (p)
    	{
    		case endl:
    			cout << RTOS::STREAM::endl;
    			headerMark = true;
    			break;
    		case tab:
    			cout << BLANK_HEADER;
    			break;
    		case newline:
    			cout << "\n";
    			break;
    	};
    	return *this;
    }
};

class Null_Debug : public Commum_Debug
{
public:
    template<typename T>
    Null_Debug & operator<<(const T & o) { return *this; }

    template<typename T>
    Null_Debug & operator<<(const T * o) { return *this; }
};

template<int string_indice, bool debugged>
class Debug: public Engine_Debug<string_indice>  {};
template<int string_indice>
class Debug<string_indice, false>: public Null_Debug {};

typedef Debug<DEBUG_HEADER_INDICE, Traits<T_DEBUG>::debug> DEB_TYPE;
extern DEB_TYPE debug;
#define DEBUG Debug<DEBUG_HEADER_INDICE, Traits<T_DEBUG>::debug>

typedef Debug<LOG_HEADER_INDICE, Traits<T_LOG>::log> LOG_TYPE;
extern LOG_TYPE dlog;
#define LOG Debug<LOG_HEADER_INDICE, Traits<T_LOG>::log>

typedef Debug<INFO_HEADER_INDICE, Traits<T_INFO>::info> INF_TYPE;
extern INF_TYPE dinfo;
#define INFO Debug<INFO_HEADER_INDICE, Traits<T_INFO>::log>

typedef Debug<DEBUG_HEADER_INDICE, Traits<T_DEBUG>::hysterical_debug> HYSTERICAL_DEBUG_TYPE;
extern HYSTERICAL_DEBUG_TYPE hdebug;

typedef Debug<TRACE_HEADER_INDICE, Traits<T_DEBUG>::trace> TRACE_TYPE;
extern TRACE_TYPE trace;
#define TRACE Debug<TRACE_HEADER_INDICE, Traits<T_DEBUG>::trace>

typedef Commum_Debug STREAM;

#endif /* APPLICATION_DEBUG_H_ */
