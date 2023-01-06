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

		virtual void operator()()
		{
			_hanlderPointer();
		}


		FuncHandler _hanlderPointer;
	};

	
	// class Function_Handler: public Handler
	// {
	// public:
	// 	Function_Handler(Function * h): _handler(h) {}
	// 	~Function_Handler() {}

	// 	void operator()() { _handler(); }

	// private:
	// 	Function * _handler;
	// };

	// template<typename T>
	class Functor
	{
	public:
		typedef void (Functor_Handler)(void *);

		Functor(Functor_Handler h, void* p): _handler(h), _ptr(p) {}

		void operator()() { _handler(_ptr); }

	private:
		Functor_Handler * _handler;
		void * _ptr;
	};


	// template <class T>
	// class FunctorEngine : public Handler {
	// 	private:
	// 	T* _object;
	// 	public:
	// 	FunctorEngine(FuncHandler ptr, T object) : Handler(ptr), _object(object) {

	// 	}

	// 	virtual void operator()()
	// 	{
	// 		_object->_hanlderPointer();
	// 	}

	// };

	// 	template <class T>
	// class Functor: public Handler
	// {
	// 	private:
	// 		Handler* _object;
	// 	public:
	// 	Functor(FuncHandler ptr, T object) {
	// 		_object = FunctorEngine<T>(ptr, object)
	// 	}

	// 	virtual void operator()()
	// 	{
	// 		_object();
	// 	}		

	// }
	

}

#endif /* RTOS_HANDLER_H_ */
