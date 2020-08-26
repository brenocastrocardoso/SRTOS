/*
 * debug.h
 *
 *  Created on: 2 de set de 2018
 *      Author: breno
 */

#ifndef _OSTREAM_H_
#define _OSTREAM_H_

#include <RTOS.h>

#include <Kernel/Traits.h>
#include <Utils/Buffer.h>

#include STRING_INCLUDE
#include CSTDIO_INCLUDE

namespace RTOS
{

	static const char FORMAT_STRING_INT[] = {"%d"};
	static const char FORMAT_STRING_HEX[] = {"%x"};

	static const char TAB_STRING[] = {"   "};

	class Commum_Stream
	{
	public:
		enum SPECIAL_CHARACTHERES
		{
			endl,
			tab,
			newline,
			hex,
		};
	};

	template<class Engine>
	class Ostream : public Commum_Stream
	{
		static constexpr uint8 SPRINTF_BUFFER_SIZE = 20;
		bool nextIsHex = false;

	public:

		Ostream(Engine& engine): _engine(engine) {}

		template<typename T>
		Ostream & operator<<(T p)
		{
			_engine.write(p);
			return *this;
		}

		Ostream & operator<<(int p)
		{
			RTOS::Buffer<SPRINTF_BUFFER_SIZE> buffer;
			const char* formatString = FORMAT_STRING_INT;
			if ( nextIsHex )
			{
				formatString = FORMAT_STRING_HEX;
				nextIsHex = false;
			}
			buffer.size(snprintf((char*)buffer.memory(), buffer.maxSize(), formatString, p));
			_engine.write(buffer.memory(), buffer.size());

			return *this;
		}

		Ostream & operator<<(SPECIAL_CHARACTHERES p)
		{
			switch (p)
			{
				case endl:
					_engine.write("\n");
					break;
				case tab:
					_engine.write(TAB_STRING);
					break;
				case newline:
					_engine.write("\n");
					break;
				case hex:
					nextIsHex = true;
					break;
			};
			return (*this);
		}
	private:
		Engine& _engine;
	};

	typedef Commum_Stream STREAM;
}



#endif /* _OSTREAM_H_ */
