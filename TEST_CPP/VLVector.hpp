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
	size_t _staticCap; // the static capacity (provided in template)
	T *_staticMemory; // initial storage (stack)
	T *_dynamicMemory; // storage when surpassing the static capacity threshold (heap)
	size_t _capacity; // current maximum amount of elements that can be inserted to the vector
	size_t _currSize; // current amount of items in the vector
	bool _isDynamic; // boolean representing if were working on static or dynamic memory

public:
	/**
	 * default ctor
	 */
	explicit VLVector() : _staticCap(StaticCapacity), _staticMemory(nullptr),
						  _dynamicMemory(nullptr), _capacity(INITIAL_SIZE), _currSize(INITIAL_SIZE),
						  _isDynamic(false)
	{}

	/**
	 * copy ctor
	 * @param vec - some vector to copy its data from
	 */
	VLVector(const VLVector &vec): VLVector()
	{
		*this = vec;
		_staticCap = vec._staticCap;
		_staticMemory = vec._staticMemory;
		_dynamicMemory = vec._dynamicMemory;
		_capacity = vec._capacity;
		_currSize = vec._currSize;
		_isDynamic = vec._isDynamic;
	}

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
		return _capacity;
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
			if (_isDynamic())
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
	 * calculates cap_C(s)
	 * @return new size of allocation
	 */
	int newCapacity()
	{
		return floor(3 * (getCurrSize() + 1) / 2);
	}

	/**
	 * gets an item of generic type T and adds it the the end of the vector
	 * based on the provided formula
	 * @param item - some item to add
	 */
	void push_back(const T &item)
	{
		// case I - static memory,  enough room
		if (getCurrSize() + 1 <= getCapacity() && !_isDynamic())
		{
			_currSize++;
			_staticMemory[_currSize] = item;
		}

			// case II - static memory, not enough room
		else if (getCurrSize() + 1 > getCapacity() && !_isDynamic())
		{
			_dynamicMemory = new(std::nothrow) T[newCapacity()];
			if (_dynamicMemory == nullptr)
			{ exit(EXIT_FAILURE); }

			for (int i = 0; i < getCurrSize(); i++) // now copying from static to dynamic
			{
				_dynamicMemory[i] = _staticMemory[i];
			}
			_staticMemory = nullptr;
			_currSize++;
			_dynamicMemory[_currSize] = item;
		}
			// case III - dynamic memory, enough room
		else if (getCurrSize() < getCapacity() && _isDynamic())
		{
			_currSize++;
			_dynamicMemory[_currSize] = item;
		}
			// case IV - dynamic memory, not enough room
		else if (getCurrSize() >= getCapacity() && _isDynamic())
		{
			_currSize++;
			T *temp = new(std::nothrow) T[newCapacity()];
			if (temp == nullptr)
			{ exit(EXIT_FAILURE); }
			for (int i = 0; i < _currSize - 1; i++)
			{
				temp[i] = _dynamicMemory[i];
			}
			temp[_currSize] = item;
			delete[] _dynamicMemory;
			_dynamicMemory = temp;
		}
		if (getCurrSize() > getStaticCap()) // this means we need to change to dynamic memory
		{
			_isDynamic = true;
		}
	}

	/**
	 * removes the last item from the vector
	 */
	void pop_back()
	{
		_currSize--;
		if (_currSize <= getStaticCap()) // if so - we need to go back to static
		{
			for (int i = 0; i++; i < _currSize)
			{
				_staticMemory[i] = _dynamicMemory[i];
			}
			free(_dynamicMemory);
			_isDynamic = false;
		}
	}

	/**
	 * removes all item in the vector
	 */
	void clear()
	{
		_currSize = INITIAL_SIZE;
		if (_isDynamic)
		{
			free(_dynamicMemory);
		}
	}

	/**
	 * @return pointer to the array that contains the data (dynamic or static)
	 */
	T *data()
	{
		if (_isDynamic)
		{
			return _dynamicMemory;
		}
		return _staticMemory;
	}

	/**
	 * assignment opeartor - this = vec
	 * @param vec some vl-vector
	 * @return this after assignment to vec
	 */
	VLVector &operator=(const VLVector &vec)
	{
		if (this != &vec) // vec is not this
		{
			if (vec._isDynamic)
			{
				_dynamicMemory = new(std::nothrow) T[vec._currSize];
				if (_dynamicMemory == nullptr)
				{ exit(EXIT_FAILURE); }
				for (int i = 0; i < vec._currSize; i++)
				{
					_dynamicMemory[i] = vec._dynamicMemory[i];
				}
			}
			else // vec is static
			{
				for (int i = 0; i < vec._currSize; i++)
				{
					_staticMemory[i] = vec._staticMemory[i];
				}
			}
			_capacity = vec._capacity;
			_currSize = vec._currSize;
			_isDynamic = vec._isDynamic;

		}
	}


	/**
	 * fetches the i'th element of this vector
	 * @param i
	 * @return i'th element of this vector
	 */
	T operator[](int i)
	{
		if (_isDynamic)
		{
			return _dynamicMemory[i];
		}
		return _staticMemory[i];
	}

	/**
	 * @param vec some vector to be compared with
	 * @return true - this == vec, otherwise false
	 */
	bool operator==(const VLVector &vec)
	{
		if (_isDynamic)
		{
			for (int i = 0; i < _currSize; i++)
			{
				if (_dynamicMemory[i] != vec._dynamicMemory)
				{
					return false;
				}
			}
		}
		else
		{
			for (int i = 0; i < _currSize; i++)
			{
				if (_staticMemory[i] != vec._staticMemory)
				{
					return false;
				}
			}
		}
		return true;
	}

	/**
	 * @param vec some vector to be compared with
	 * @return true - this != vec, otherwise false
	 */
	bool operator!=(const VLVector &vec)
	{
		return !operator==(vec);
	}

};


#endif //TEST_CPP_VLVECTOR_HPP
