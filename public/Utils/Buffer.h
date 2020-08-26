/*
 * Buffer.h
 *
 *  Created on: 18 de jul de 2020
 *      Author: breno
 */

#ifndef RTOS_PUBLIC_UTILS_BUFFER_H_
#define RTOS_PUBLIC_UTILS_BUFFER_H_

#include <RTOS.h>

#include CSTRING_INCLUDE

namespace RTOS
{

	template<int MaxSize, class dataType = uint8, class sizeType = uint8>
	class Buffer
	{
	public:
		dataType _memory[MaxSize];
		sizeType _size;

		Buffer(){}

		Buffer(dataType* memory, sizeType size)
		{
			memcpy(_memory, memory, size);
		}

		dataType* memory()
		{
			return _memory;
		}

		dataType operator[](sizeType indice)
		{
			return _memory[indice];
		}

		void size(sizeType size)
		{
			_size = size < MaxSize ? size : MaxSize;
		}

		sizeType size(void)
		{
			return _size;
		}

		sizeType maxSize()
		{
		    return MaxSize;
		}
	};

}



#endif /* RTOS_PUBLIC_UTILS_BUFFER_H_ */
