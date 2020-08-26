/*
 * PayloadHandler.h
 *
 *  Created on: 8 de out de 2019
 *      Author: breno
 */

#ifndef RTOS_HANDLER_H_
#define RTOS_HANDLER_H_

namespace RTOS {

	class Handler
	{
	public:
		typedef void(*FuncHandler)();

		Handler(FuncHandler ptr)
		{
			_hanlderPointer = ptr;
		}

		void operator()()
		{
			_hanlderPointer();
		}


		FuncHandler _hanlderPointer;
	};

}

#endif /* RTOS_HANDLER_H_ */
