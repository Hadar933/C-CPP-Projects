#ifndef TEST_CPP_VLVECTOR_HPP
#define TEST_CPP_VLVECTOR_HPP

#include <iostream>
#include <cmath>

#define DEFAULT_SIZE 16
#define INITIAL_SIZE 0

template<class T, size_t StaticCapacity = DEFAULT_SIZE>
class VLVector
{

private:
	T *_vec= nullptr; // stores the values of the vector
	size_t _size;
	size_t _statCap;

public:
	/**
	 * constructor that initializes a VLVector instance
	 * @param vec
	 * @param size
	 */
	VLVector(size_t size, size_t statCap) :
			_size(size), _statCap(statCap)
	{

	}

	VLVector() : VLVector(INITIAL_SIZE,INITIAL_SIZE) {}

	/**
	 * copy ctor
	 * @param vlv some vlvector
	 */
	VLVector(const VLVector &vlv) : VLVector(vlv._vec, vlv._size, vlv._statCap)
	{
		*this = vlv;
	}

	/**
	 *
	 * @tparam InputIterator - segment [first,last) of T values
	 * @param first
	 * @param last
	 */
	template <class InputIterator>
	VLVector(InputIterator& first, InputIterator &last)
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
	 * @return - number of elements in the vector
	 */
	size_t getSize() const
	{
		return _size;
	}

	size_t getStatCap() const
	{
		return _statCap;
	}

	/**
	 * @return - vector's capacity, as given by the provided formula
	 */
	size_t capacity() const
	{
		size_t C = getStatCap();
		size_t s = getSize();
		if(s+1 <= C)
		{
			return C;
		}
		else
		{
			unsigned int val = 3 * (s + 1) / 2;
			return floor(val);
		}
	}

	/**
	 * checks if the vector is empty
	 * @return 1 - empty, 0 - not empty
	 */
	bool empty() const
	{
		return getSize()==INITIAL_SIZE;
	}

	



};


#endif //TEST_CPP_VLVECTOR_HPP
