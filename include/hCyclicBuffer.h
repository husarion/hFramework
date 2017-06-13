/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __CYCLIC_BUFFER_H__
#define __CYCLIC_BUFFER_H__

#include <hFramework.h>

namespace hFramework
{

/**
 * @brief Provides cyclic buffer based on existing array.
 *
 * @tparam T - Element type
 */
template<class T>
class hCyclicBuffer
{
private:
	T *values;
	uint16_t elements;
	uint16_t iter;
public:
	/**
	 * @brief Create cyclic buffer based on specified table.
	 * @param elements number of elements in the table
	 * @param table array to be used by the buffer
	 */
	hCyclicBuffer(uint16_t elements, T* table)
	{
		this->elements = elements;
		this->values = table;
		for (uint16_t i = 0; i < elements; ++i)
			values[i] = 0;
		iter = 0;
	}
	/**
	 * @brief Insert element at the end of the queue and return last element.
	 * @param val element to be inserted
	 */
	T push_back(T val)
	{
		T tmp = values[iter];
		values[iter] = val;
		++iter;
		if (iter >= elements)
		{
			iter = 0;
		}
		return tmp;
	}

	int size() const { return elements; }

	/**
	 * @brief Return element at the specified index.
	 *
	 * Oldest elements is at position 0.
	 * @param val element to be inserted
	 */
	T operator[](uint16_t index)
	{
		return this->values[(iter + index + elements) % elements];
	}
};

}

#endif // __CYCLIC_BUFFER_H__
