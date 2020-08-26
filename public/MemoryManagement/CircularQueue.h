/*
 * Queue.h
 *
 *  Created on: 6 de dez de 2019
 *      Author: breno
 */

#ifndef RTOS_UTILITY_CIRCULARQUEUE_H_
#define RTOS_UTILITY_CIRCULARQUEUE_H_

#include <RTOSCodeStyle.h>

template <class DataType, class Container>
class CircularQueue : public Container
{
public:

	CircularQueue() : Container()
	{
		_toPop = 0;
		_toPush = 0;
	}

	bool push(const DataType& element)
	{
		if (full() != true)
		{
			Container::at(_toPush) = element;
			++_toPush;
			_toPush%=Container::size();
			return true;
		}
		return false;
	}

	bool empty(void) const
	{
		return (size()==0);
	}

	bool full(void) const
	{
		return (size() >= (Container::size()-1));
	}

	uint8 size(void) const
	{
		return ( ( Container::size() + _toPush - _toPop ) % Container::size() );
	}

	DataType& front(void)
	{
		return Container::at(_toPop);
	}

	DataType& back(void) const
	{
		uint8 indexValue = _toPush-1+Container::size();
		indexValue %= Container::size();
		return Container::at(indexValue);
	}

	bool pop(void)
	{
		if (empty() != true)
		{
			++_toPop;
			_toPop%=Container::size();
			return true;
		}
		return false;
	}

	template<typename STREAM>
	void publish (STREAM& stream) const
	{
		stream << STREAM::tab << "queue size    :"   << (int)size() << STREAM::newline;
	}

private:
	uint8 _toPush;
	uint8 _toPop;
};


#endif /* RTOS_UTILITY_CIRCULARQUEUE_H_ */
