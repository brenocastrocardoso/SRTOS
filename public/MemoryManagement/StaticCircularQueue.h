/*
 * Queue.h
 *
 *  Created on: 6 de dez de 2019
 *      Author: breno
 */

#ifndef RTOS_UTILITY_STATICCIRCULARQUEUE_H_
#define RTOS_UTILITY_STATICCIRCULARQUEUE_H_

#include <MemoryManagement/array.h>

#include <MemoryManagement/CircularQueue.h>

template <class DataType, int size>
class StaticCircularQueue : public CircularQueue<DataType, Array<DataType, size>>
{};



#endif /* RTOS_UTILITY_STATICCIRCULARQUEUE_H_ */
