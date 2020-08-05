#ifndef TEST_CPP_VLVECTOR_HPP
#define TEST_CPP_VLVECTOR_HPP

#include <cmath>
#include <cstdlib>
#include <stdexcept>

#define DEFAULT_SIZE 16
#define INITIAL_SIZE 0
#define OUT_OF_RANGE_MSG "index out of range"

template<class T, size_t StaticCapacity = DEFAULT_SIZE>
class VLVector
{

private:
	size_t _staticCap = StaticCapacity; // the static capacity
	T *_staticMemory = nullptr; // initial storage (stack)
	T *_dynamicMemory = nullptr; // storage when surpassing the static capacity threshold (heap)
	size_t capacity; // current maximum amount of elements that can be inserted to the vector
	size_t _currSize; // current amount of items in the vector
	bool isDynamic = false; // boolean representing if were working on static or dynamic memory

public:

	/**
	 * @tparam InputIterator - segment [first,last) of T values
	 * @param first
	 * @param last
	 */
	template<class InputIterator>
	VLVector(InputIterator &first, InputIterator &last)
	{
	}

	/**
	 * destructor for class VLVector - frees _vec from memory
	 */
	~VLVector()
	{

	}
	/**
	 * @return 1 - the current memory usage is dynamic. 0 - static
	 */

	/**
	 * @return - the static capacity of the vector
	 */
	size_t getStaticCap() const
	{
		return _staticCap;
	}

	/**
	 * @return maximum amount of elements that can be inserted to the vector
	 */
	size_t getCapacity() const
	{
		return capacity;
	}

	/**
	 * @return current amount of items in the vector
	 */
	size_t getCurrSize() const
	{
		return _currSize;
	}

	/**
	 * @return: 1 - vector is empty, 0 - otherwise
	 */
	bool empty() const
	{
		return getCurrSize() == INITIAL_SIZE;
	}

	/**
	 * gets an index i and returns vtv[i]
	 * @param index - some index (might be out of range)
	 * @return - object T representing vtv[i]
	 */
	T at(const int index)
	{
		if (index < INITIAL_SIZE || index > getCurrSize()) // invalid index
		{
			throw std::out_of_range(OUT_OF_RANGE_MSG);
		}
		else // index is ok
		{
			if(isDynamic())
			{
				return _dynamicMemory[index];
			}
			else // static memory
			{
				return _staticMemory[index];
			}
		}
	}

	/**
	 * gets an item of generic type T and adds it the the end of the vector
	 * @param item - some item to add
	 */
	void push_back(const T& item)
	{
		// case I - static memory,  enough room
		if(getCurrSize()<getCapacity() && !isDynamic())
		{
			_currSize++;
			_staticMemory[_currSize]=item;
		}

		// case II - static memory, not enough room -> allocate dynamic memory
		else if (getCurrSize()>=getCapacity() && !isDynamic())
		{
			

		}
		// case III - dynamic memory, enough room
		else if (getCurrSize()<getCapacity() && isDynamic())
		{

		}
		// case IV - dynamic memory, not enough room -> re-allocate dynamic memory
		else if  (getCurrSize()>=getCapacity() && isDynamic())
		{

		}

		if(getCurrSize()>getStaticCap()) // this means we need to change to dynamic memory
		{
			isDynamic = true;
		}

	}

};


#endif //TEST_CPP_VLVECTOR_HPP
