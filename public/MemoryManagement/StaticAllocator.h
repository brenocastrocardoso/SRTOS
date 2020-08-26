/*
 * StaticAllocator.h
 *
 * @brief this module manager a space of memory with an bitmap and a so it is possible to use this as an allocated space on the memory
 *
 *  Created on: 16 de dez de 2019
 *      Author: breno
 */

#ifndef STATICALLOCATOR_H_
#define STATICALLOCATOR_H_

#include <MemoryManagement/Bitmap.h>
#include <RTOSCodeStyle.h>

template <class T, int size>
class StaticAllocator
{
	Bitmap<size> _bitmap;
	T _memory[size];
public:
    // naming tradition
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef uint8 size_type;
//    typedef std::ptrdiff_t difference_type;

    template <typename U>
    struct rebind
    {
        typedef StaticAllocator<U, size> other;
    };

    StaticAllocator(){}
    StaticAllocator(const StaticAllocator &){}

    StaticAllocator &operator=(const StaticAllocator &);
    ~StaticAllocator() = default;

	pointer address(reference r) { return &r; }
    const_pointer address(const_reference cr) { return &cr; }

    size_type max_size() { return (size);}

    bool operator==(const StaticAllocator &) const { return true; }
    bool operator!=(const StaticAllocator &) const { return false;}

    // allocation and deallocation
    pointer allocate(size_type n) {
    	//allocate the space in the bitmap
    	uint8 nextFree = _bitmap.getNext(false);
    	_bitmap.set(nextFree);
        return static_cast<pointer>(&(_memory[nextFree]));
    }

    pointer allocate(size_type n, pointer ptr) {
        return allocate(n);
    }

    void deallocate(pointer ptr, size_type n = 1) {
    	uint8 position = (ptr - _memory);
    	_bitmap.clear(position);
    }



    // constrution valu_type and destruction
    void construct(pointer ptr, const value_type &t) {
        new(ptr) value_type(t);
    }
    void destroy(pointer ptr) {
        ptr->~value_type();
    }
};

#endif /* STATICALLOCATOR_H_ */
