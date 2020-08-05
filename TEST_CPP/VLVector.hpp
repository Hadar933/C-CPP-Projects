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
	T *_vec = nullptr; // stores all the values of the vector
	size_t capacity; // current maximum amount of elements that can be inserted to the vector
	size_t _currSize; // current amount of items in the vector

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
		delete[] _vec;
		_vec = nullptr;
	}

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

	T at(int index)
	{
		if (index < INITIAL_SIZE || index > getCurrSize()) // invalid index
		{
			throw std::out_of_range(OUT_OF_RANGE_MSG);
		}
		else // index is ok
		{
			return _vec[index];
		}
	}

	void push_back(T item)
	{
		
	}

};


#endif //TEST_CPP_VLVECTOR_HPP
