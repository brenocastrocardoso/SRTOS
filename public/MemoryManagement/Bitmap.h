/*
 * bitmap.h
 *
 *  Created on: 16 de dez de 2019
 *      Author: breno
 */

#ifndef BITMAP_H_
#define BITMAP_H_

template<int size>
struct MetaMod { static const int result = size % 8; };

template<int size, int mod>
struct MetaArraySizeEng { static const int result = size/8 + 1; };

template<int size>
struct MetaArraySizeEng<size, 0> { static const int result = size/8; };

template <int size>
struct MetaArraySize{ static const int result = MetaArraySizeEng<size, MetaMod<size>::result>::result; };

template <int size>
class Bitmap
{
	typedef unsigned char uint8_t;

	uint8_t _bitmap[MetaArraySize<size>::result];

	#define BIT_SET(byte, bit) 			(byte |= 1<<bit)
	#define BIT_CLEAR(byte, bit)		(byte &= ~(1<<bit))
	#define BIT_TEST(byte, bit)			((byte>>bit)&0x01)

	static constexpr uint8_t INVALID_VALUE = 0xFF;

public:
	Bitmap()
	{
		for(uint8_t index = 0; index<sizeof(_bitmap);++index)
		{
			_bitmap[index] = 0x00;
		}
	}

	void set(uint8_t position)
	{
		uint8_t bit = position%8;
		uint8_t byte = position/8;
		BIT_SET(_bitmap[byte], bit);
	}

	void clear(uint8_t position)
	{
		uint8_t bit = position%8;
		uint8_t byte = position/8;
		BIT_CLEAR(_bitmap[byte], bit);
	}

	bool get(uint8_t position)
	{
		uint8_t bit = position%8;
		uint8_t byte = position/8;
		return (BIT_TEST(_bitmap[byte], bit) ? true : false);
	}

	uint8_t getNext(bool value, uint8_t startPosition = 0)
	{
		for(uint8_t index = startPosition; index < size; index++)
		{
			if (get(index) == value)
			{
				return index;
			}
		}
		return INVALID_VALUE;
	}
};



#endif /* BITMAP_H_ */
