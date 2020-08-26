/*
 * StdOIStream.h
 *
 *  Created on: 18 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_PUBLIC_IO_STDOISTREAM_H_
#define RTOS_PUBLIC_IO_STDOISTREAM_H_

#include <Hal/Peripheral/Serial.h>
#include <IO/OStream.h>

namespace RTOS {

	template<class stream = RTOS::Ostream<HAL::Serial>>
	class StdOstream
	{
	public:
		StdOstream() : _stream(nullptr) {}

		void setStream(stream* ptr)
		{
			_stream = ptr;
		}

		template<typename T>
		StdOstream & operator<<(T p)
		{
			if(_stream != nullptr)
			{
				(*_stream) << p;
			}
			return *this;
		}

		StdOstream & operator<<(int p)
		{
			if(_stream != nullptr)
			{
				(*_stream) << p;
			}
			return *this;
		}

		StdOstream & operator<<(Commum_Stream::SPECIAL_CHARACTHERES p)
		{
			if(_stream != nullptr)
			{
				(*_stream) << p;
			}
			return *this;
		}

	private:
		stream* _stream;
	};
}

#endif /* RTOS_PUBLIC_IO_STDOISTREAM_H_ */
