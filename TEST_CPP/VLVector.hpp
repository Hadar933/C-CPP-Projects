#ifndef TEST_CPP_VLVECTOR_HPP
#define TEST_CPP_VLVECTOR_HPP

#include <cmath>
#include <cstdlib>
#include <stdexcept>

#define DEFAULT_SIZE 16
#define INITIAL_SIZE 0
#define OUT_OF_RANGE_MSG "index out of range"

/**
 * class VLVector, initializes a vector with changing memory segments
 * @tparam T - genetic type
 * @tparam StaticCapacity -predefined constant representing maximum size of static memory
 */
template<class T, size_t StaticCapacity = DEFAULT_SIZE>
class VLVector
{
private:
	size_t _staticCap; // the static capacity (provided in template)
	T _staticMemory[StaticCapacity]; // initial storage (stack)
	T *_dynamicMemory; // storage when surpassing the static capacity threshold (heap)
	size_t _capacity; // current maximum amount of elements that can be inserted to the vector
	size_t _currSize; // current amount of items in the vector
	bool _isDynamic; // boolean representing if were working on static or dynamic memory

	class Iterator
	{
	private:
		T *_curr;
	public:
		/**
		 * constructor for Iterator
		 * @param curr - some value to use in initialization
		 */
		Iterator(T *curr) : _curr(curr)
		{}

		/**
		 * @return dereference
		 */
		Iterator &operator*() const
		{
			return *_curr;
		}

		/**
		 * post increment
		 * @return value ++
		 */
		Iterator &operator++()
		{
			return _curr++;
		}

		/**
		 * pre increment
		 */
		Iterator operator++(int)
		{
			Iterator temp = _curr;
			_curr++;
			return temp;
		}

		/**
		 * post decrement
		 * @return value --
		 */
		Iterator &operator--()
		{
			return _curr--;
		}

		/**
		 * pre decrement
		 */
		Iterator operator--(int)
		{
			Iterator temp = _curr;
			_curr--;
			return temp;
		}

		/**
		 * @param other some iterator
		 * @return new iterator which is curr+other
		 */
		Iterator &operator+(const Iterator &other) const
		{
			return Iterator(_curr + other._curr);
		}

		/**
		 * @param other - some iterator
		 * @return 1 if this._curr = other._curr, 0 otherwise
		 */
		bool operator==(const Iterator &other) const
		{
			return this->_curr == other._curr;
		}

		/**
		 * @param other - some iterator
		 * @return 1 if this._curr != other._curr, 0 otherwise
		 */
		bool operator!=(const Iterator &other) const
		{
			return this->_curr != other._curr;
		}

		/**
		 * @param other - some iterator
		 * @return 1 if this._curr < other._curr, 0 otherwise
		 */
		bool operator<(const Iterator &other) const
		{
			return this->_curr < other._curr;
		}

		/**
		 * @param other - some iterator
		 * @return 1 if this._curr > other._curr, 0 otherwise
		 */
		bool operator>(const Iterator &other) const
		{
			return this->_curr > other._curr;
		}

		/**
		 * @param other - some iterator
		 * @return 1 if this._curr <= other._curr, 0 otherwise
		 */
		bool operator<=(const Iterator &other) const
		{
			return this->_curr <= other._curr;
		}

		/**
		 * @param other - some iterator
		 * @return 1 if this._curr >= other._curr, 0 otherwise
		 */
		bool operator>=(const Iterator &other) const
		{
			return this->_curr >= other._curr;
		}

	};

	class constIterator
	{
	private:
		T *curr;
	public:

	};

public:
	// to create instances of iterators outside of the class:
	typedef Iterator iterator;
	typedef constIterator constIterator;

	/**
	 * default ctor
	 */
	VLVector() : _staticCap(StaticCapacity), _dynamicMemory(nullptr),
				 _capacity(StaticCapacity), _currSize(INITIAL_SIZE), _isDynamic(false)
	{}

	/**
	 * copy ctor
	 * @param vec - some vector to copy its data from
	 */
	VLVector(const VLVector &vec) : VLVector()
	{
		_staticCap = vec._staticCap;
		_capacity = vec._staticCap;
		_currSize = INITIAL_SIZE;
		_isDynamic = vec._isDynamic;
		*this->operator=(vec);

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
		delete[] _dynamicMemory;
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
		return _currSize == INITIAL_SIZE;
	}

	/**
	 * gets an index i and returns vtv[i]
	 * @param index - some index (might be out of range)
	 * @return - object T representing vtv[i]
	 */
	T at(const int index) const
	{
		if (index < INITIAL_SIZE || index > _currSize) // invalid index
		{
			delete[] _dynamicMemory;
			throw std::out_of_range(OUT_OF_RANGE_MSG);
		}
		else // index is ok
		{
			if (_isDynamic)
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
		return floor(3 * (_currSize + 1) / 2);
	}

	/**
	 * gets an item of generic type T and adds it the the end of the vector
	 * based on the provided formula
	 * @param item - some item to add
	 */
	void push_back(const T &item)
	{
		// case I - static memory,  enough room
		if (_currSize + 1 <= _capacity && !_isDynamic)
		{
			_staticMemory[_currSize] = item;
			_currSize++;
		}

			// case II - static memory, not enough room
		else if (_currSize + 1 > _capacity && !_isDynamic)
		{
			_capacity = newCapacity();
			_dynamicMemory = new T[_capacity];
			for (int i = 0; i < _currSize; i++) // now copying from static to dynamic
			{
				_dynamicMemory[i] = _staticMemory[i];
			}
			_dynamicMemory[_currSize] = item;
			_currSize++;

		}
			// case III - dynamic memory, enough room
		else if (_currSize < _capacity && _isDynamic)
		{
			_dynamicMemory[_currSize] = item;
			_currSize++;
		}
			// case IV - dynamic memory, not enough room
		else if (_currSize >= _capacity && _isDynamic)
		{
			_capacity = newCapacity();
			T *temp = new T[_capacity];
			for (int i = 0; i < _currSize; i++)
			{
				temp[i] = _dynamicMemory[i];
			}
			temp[_currSize] = item;
			delete[] _dynamicMemory;
			_dynamicMemory = new T[_capacity];
			for (int i = 0; i < _currSize + 1; i++)
			{
				_dynamicMemory[i] = temp[i];
			}
			_currSize++;

		}
		if (_currSize > _staticCap) // this means we need to change to dynamic memory
		{
			_isDynamic = true;
		}

	}

	/**
	 * removes the last item from the vector
	 */
	void pop_back()
	{
		if (!empty())
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
			delete[] _dynamicMemory;
			_dynamicMemory = nullptr;
			if (vec._isDynamic)
			{
				_dynamicMemory = new T[vec._capacity];
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
		return *this;
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
	 * fetches the i'th element of this vector (const)
	 * @param i
	 * @return i'th element of this vector
	 */
	T operator[](int i) const
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
				if (_dynamicMemory[i] != vec._dynamicMemory[i])
				{
					return false;
				}
			}
		}
		else
		{
			for (int i = 0; i < _currSize; i++)
			{
				if (_staticMemory[i] != vec._staticMemory[i])
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
