/*
 * RTCEvent.h
 *
 *  Created on: 23 de jul de 2020
 *      Author: breno
 */

#ifndef _RTC_RTCEVENT_H_
#define _RTC_RTCEVENT_H_

#include <Utils/Debug/Debug.h>
#include <Utils/Handler.h>
#include <Utils/RTC/RTC.h>

namespace RTOS {

	template<int eventsNumber>
	class RTCEvent
	{
	public:

		typedef enum EVENT
		{
			EVERY_YEAR,
			EVERY_MONTH,
			EVERY_DAY,
			EVERY_HOUR,
			EVERY_MINUTE,
			EVERY_SECOND,
			EVENT_NULL,
		}EVENT_TYPE;

		struct Event
		{
			EVENT_TYPE _event;
			Timepoint _timestamp;

			Event(EVENT_TYPE event, Timepoint timestamp) : _event(event), _timestamp(timestamp)
			{}

			Event() : _event(EVENT_NULL), _timestamp(0)
			{}
		};

		RTCEvent(RTOS::Handler eventHandler, RTC& clock) : _handler(eventHandler), _clock(clock)
		{
			_eventsCount = 0;
		}

		void addEvent(EVENT_TYPE event, Timepoint timestamp)
		{
			addEvent(Event(event, Timepoint(timestamp)));
		}

		void addEvent(Event event)
		{
			if(_eventsCount < eventsNumber)
			{
				_events[_eventsCount] = event;
				_eventsCount++;
			}
		}

		void handler()
		{
			for(uint8 index = 0; index < _eventsCount; ++index)
			{
				if (_events[index]._timestamp <= _clock.getTime())
				{
					_handler();
					switch(_events[index]._event)
					{
						case EVERY_YEAR:
//							_events[index]._timestamp.
							break;
						case EVERY_MONTH:
//							_events[index]._timestamp
							break;
						case EVERY_DAY:
							++(_events[index]._timestamp.Day());
							break;
						case EVERY_HOUR:
							++(_events[index]._timestamp.Hour());
							break;
						case EVERY_MINUTE:
							++(_events[index]._timestamp.Minute());
							break;
						case EVERY_SECOND:
							++(_events[index]._timestamp);
							break;
						default:
							break;
					}
				}
			}
		}


	private:
		RTOS::Handler _handler;
		Event _events[eventsNumber];
		uint8 _eventsCount;
		RTC& _clock;
	};

}



#endif /* RTOS_PUBLIC_UTILS_RTC_RTCEVENT_H_ */
