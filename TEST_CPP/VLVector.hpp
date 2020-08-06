#ifndef TEST_CPP_VLVECTOR_HPP
#define TEST_CPP_VLVECTOR_HPP

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdexcept>

#define DEFAULT_SIZE 16
#define INITIAL_SIZE 0
#define START 0
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
		// iterator traits
		typedef T value_type;
		typedef T &reference;
		typedef T *pointer;
		typedef T difference_type;
		typedef std::random_access_iterator_tag iterator_category;

		/**
		 * constructor for Iterator
		 * @param curr - some value to use in initialization
		 */
		Iterator(T *curr) : _curr(curr)
		{}

		/**
		 * @return reference (object can be changed)
		 */
		reference operator*() const
		{
			return *_curr;
		}

		/**
		 * post increment
		 * @return iterator after ++ was performed
		 */
		Iterator &operator++()
		{
			_curr++;
			*this;
		}

		/**
		 * pre increment (moves to next item)
		 * @return iterator before ++ was performed
		 */
		Iterator operator++(int)
		{
			Iterator temp = *this;
			_curr++;
			return temp;
		}

		/**
		 * post decrement
		 * @return iterator after -- was performed
		 */
		Iterator &operator--()
		{
			_curr--;
			return *this;
		}

		/**
		 * pre decrement
		 * @return iterator before -- was performed
		 */
		Iterator operator--(int)
		{
			Iterator temp = *this;
			_curr--;
			return temp;
		}

		/**
		 * operator + that given some value of difference type,
		 * returns a new Iterator, with value that is the sum of this iterator and val
		 * @param val - some value to increment in
		 * @return new iterator which is curr+other
		 */
		Iterator &operator+(const difference_type &val) const
		{
			return Iterator(_curr + val);
		}

		/**
		 * operator + that given some Iterator, advances this iterator by a factor
		 * of the other value
		 * @param other - some iterator
		 * @return difference type
		 */
		difference_type operator+(const Iterator &other) const
		{
			return _curr + other._curr;
		}

		/**
		 * operator - that given some value of difference type,
		 * returns a new Iterator, with value that is the difference of this iterator and val
		 * @param val - some value to increment in
		 * @return new iterator which is curr-other
		 */
		Iterator &operator-(const difference_type &val) const
		{
			return Iterator(_curr - val);
		}

		/**
		 * operator - that given some Iterator, advances this iterator by a factor
		 * of the minus other value
		 * @param other - some iterator
		 * @return difference type
		 */
		difference_type operator-(const Iterator &other) const
		{
			return _curr - other._curr;
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
		T *_curr;
	public:
		// iterator traits
		typedef T value_type;
		typedef T &reference;
		typedef T *pointer;
		typedef T difference_type;
		typedef std::random_access_iterator_tag iterator_category;

		/**
		 * constructor for constIterator
		 * @param curr - some value to use in initialization
		 */
		constIterator(T *curr) : _curr(curr)
		{}

		/**
	 	* @return value_type (object can not be changed)
	 	*/
		value_type operator*() const
		{
			return *_curr;
		}

		/**
		 * post increment
		 * @return constIterator after ++ was performed
		 */
		constIterator &operator++()
		{
			_curr++;
			*this;
		}

		/**
		 * pre increment
		 * @return const iterator before ++ was performed
		 */
		constIterator operator++(int)
		{
			Iterator temp = *this;
			_curr++;
			return temp;
		}

		/**
		 * post decrement
		 * @return const iterator after -- was performed
		 */
		Iterator &operator--()
		{
			_curr--;
			return *this;
		}

		/**
		 * pre decrement
		 * @return const iterator before -- was performed
		 */
		Iterator operator--(int)
		{
			Iterator temp = *this;
			_curr--;
			return temp;
		}

		/**
		 * operator + that given some value of difference type,
		 * returns a new Iterator, with value that is the sum of this iterator and val
		 * @param val - some value to increment in
		 * @return new const iterator which is curr+other
		 */
		constIterator &operator+(const difference_type &val) const
		{
			return constIterator(_curr + val);
		}

		/**
		 * operator + that given some Iterator, advances this iterator by a factor
		 * of the other value
		 * @param other - some iterator
		 * @return difference type
		 */
		difference_type operator+(const constIterator &other) const
		{
			return _curr + other._curr;
		}

		/**
		 * operator - that given some value of difference type,
		 * returns a new Iterator, with value that is the difference of this iterator and val
		 * @param val - some value to increment in
		 * @return new iterator which is curr-other
		 */
		constIterator &operator-(const difference_type &val) const
		{
			return constIterator(_curr - val);
		}

		/**
		 * operator - that given some Iterator, advances this iterator by a factor
		 * of the minus other value
		 * @param other - some const iterator
		 * @return difference type
		 */
		difference_type operator-(const constIterator &other) const
		{
			return _curr - other._curr;
		}

		/**
		 * @param val - some value to add
		 * @return pointer to this vec
		 */
		Iterator&operator+=(const difference_type& val)
		{
			_curr+=val;
			return *this;
		}

		/**
		 * @param val - some value to add
		 * @return pointer to this vec
		 */
		Iterator&operator-=(const difference_type& val)
		{
			_curr-=val;
			return *this;
		}

		/**
		 * @param other - some const-iterator
		 * @return 1 if this._curr = other._curr, 0 otherwise
		 */
		bool operator==(const constIterator &other) const
		{
			return this->_curr == other._curr;
		}

		/**
		 * @param other - some const iterator
		 * @return 1 if this._curr != other._curr, 0 otherwise
		 */
		bool operator!=(const constIterator &other) const
		{
			return this->_curr != other._curr;
		}

		/**
		 * @param other - some const iterator
		 * @return 1 if this._curr < other._curr, 0 otherwise
		 */
		bool operator<(const constIterator &other) const
		{
			return this->_curr < other._curr;
		}

		/**
		 * @param other - some const iterator
		 * @return 1 if this._curr > other._curr, 0 otherwise
		 */
		bool operator>(const constIterator &other) const
		{
			return this->_curr > other._curr;
		}

		/**
		 * @param other - some const iterator
		 * @return 1 if this._curr <= other._curr, 0 otherwise
		 */
		bool operator<=(const constIterator &other) const
		{
			return this->_curr <= other._curr;
		}

		/**
		 * @param other - some const iterator
		 * @return 1 if this._curr >= other._curr, 0 otherwise
		 */
		bool operator>=(const constIterator &other) const
		{
			return this->_curr >= other._curr;
		}


	};

public:
	// to create instances of iterators outside the class:
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
	size_t capacity() const
	{
		return _capacity;
	}

	/**
	 * @return current amount of items in the vector
	 */
	size_t size() const
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
	 * this helper function performs a loop over some index, and given two arrays
	 * in order to shorten the functions (this loop occurs often)
	 * @param toArray - array to copy to
	 * @param fromArray - array to copy from
	 * @param start - start index
	 * @param end - end index
	 * @param up - 1:loop increases ++ or 0: decreases --
	 * @param adj1 - additional index manipulation
	 * @param adj2 - additional index manipulation
	 */
	void _loopProcess(T* &toArray, T* &fromArray, int start, int end, bool up, int adj1, int adj2)
	{
		if(up)
		{
			for(int i=start;i<end;i++)
			{
				toArray[i+adj1] = fromArray[i+adj2];
			}
		}
		else // down
		{
			for(int i=start;i<end;i--)
			{
				toArray[i+adj1] = fromArray[i+adj2];
			}
		}

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
		if (_currSize > _staticCap &&!_isDynamic) // this means we need to change to dynamic memory
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
			if(_currSize!=INITIAL_SIZE)
			{
				delete[] _dynamicMemory;
				_dynamicMemory = nullptr;
			}
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
	T& operator[](int i)
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

	/**
	 * @return the first element of the vector
	 */
	Iterator begin()
	{
		return (_isDynamic) ? _dynamicMemory : _staticMemory;
	}

	/**
	 * @return one past the last element of the vector
	 */
	Iterator end()
	{
		return (_isDynamic) ? &_dynamicMemory[_currSize] : &_staticMemory[_currSize];
	}

	/**
	 * @return the first element of the vector
	 */
	constIterator begin() const
	{
		return (_isDynamic) ? _dynamicMemory : _staticMemory;
	}

	/**
 	* @return one past the last element of the vector
 	*/
	constIterator end() const
	{
		return (_isDynamic) ? &_dynamicMemory[_currSize] : &_staticMemory[_currSize];
	}


	/**
	 * @return the first element of the vector
	 */
	constIterator cbegin() const
	{
		return (_isDynamic) ? _dynamicMemory : _staticMemory;
	}

	/**
	 * @return one past the last element of the vector
	 */
	constIterator cend() const
	{
		return (_isDynamic) ? &_dynamicMemory[_currSize] : &_staticMemory[_currSize];
	}

	/**
	 * range based iteration
	 * @param os - ostream
	 * @param vlv -some VLVector
	 * @return os
	 */
	friend std::ostream &operator<<(std::ostream &os, const VLVector &vlv)
	{
		for (const auto &it:vlv)
		{
			os << it << " ";
		}
		return os;
	}

	/**
	 * erases the item pointed by it iterator
	 * @param it - some vector iterator
	 * @return iterator pointing to the item to the right to the item that had just been removed
	 */
	Iterator erase(const Iterator &it)
	{
		int index = it - begin();
		if (_currSize > _staticCap + 1) // meaning - removing one item doesnt mean going back to
			// static (if the array is currently dynamically allocated)
		{
			if (_isDynamic)
			{
				for (int i = index; i < _currSize - 1; i++)
				{
					_dynamicMemory[i] = _dynamicMemory[i + 1];
				}
			}
			else // static
			{
				for (int i = index; i < _currSize - 1; i++)
				{
					_staticMemory[i] = _staticMemory[i + 1];
				}
				_isDynamic = false;
			}
		}
		else // removing an item means going back to static memory (if we're in dynamic alloc)
		{
			if (_isDynamic)
			{ // we copy the first elements back to the static array
				for (int i = 0; i < index; i++)
				{
					_staticMemory[i] = _dynamicMemory[i];
				}
				// next we shift left from index onwards
				for (int i = index; i < _currSize - 1; i++)
				{
					_staticMemory[i] = _dynamicMemory[i + 1];
				}
				delete[] _dynamicMemory;
				_isDynamic = false;
				_capacity = _staticCap;
			}
			else // static - we copy with a shift all elemtns
			{
				for (int i = index; i < _currSize - 1; i++)
				{
					_staticMemory[i] = _staticMemory[i + 1];
				}
			}
		}
		_currSize--;
		return it;
	}

	/**
	 * erases all items between two iterators.
	 * @param it1,2 - some vector iterators
	 * @return iterator pointing to the item to the right to the segment that had just been removed
	 */
	Iterator erase(const Iterator &it1, const Iterator &it2)
	{
		int index1 = it1 - begin();
		int index2 = it2 - begin();
		int size = index2 - index1;
		if (_currSize > _staticCap + size) // meaning - removing size item doesnt mean going back to
			// static (if the array is currently dynamically allocated)
		{
			if (_isDynamic)
			{
				for (int i = index1; i < _currSize - size; i++)
				{
					_dynamicMemory[i] = _dynamicMemory[i + size];
				}
			}
			else // static
			{
				for (int i = index1; i < _currSize - size; i++)
				{
					_staticMemory[i] = _staticMemory[i + size];
				}
			}
		}
		else // removing items means going back to static memory (if we're in dynamic alloc)
		{
			if (_isDynamic)
			{ // we copy the first elements back to the static array
				for (int i = 0; i < index1; i++)
				{
					_staticMemory[i] = _dynamicMemory[i];
				}
				// next we shift left from index1 onwards
				for (int i = index1; i < _currSize - size; i++)
				{
					_staticMemory[i] = _dynamicMemory[i + size];
				}
				delete[] _dynamicMemory;
				_isDynamic = false;
				_capacity = _staticCap;
			}
			else // static - we copy with a shift all elements
			{
				for (int i = index1; i < _currSize - size; i++)
				{
					_staticMemory[i] = _staticMemory[i + size];
				}
			}
		}
		_currSize = _currSize - size;
		return it2;
	}

	/**
	 * inserts an item to a vector
	 * @param it some iterator pointing to an item
	 * @param item - new item to add to the vector
	 * @return iterator pointing to item
	 */
	Iterator insert(const Iterator &it, const T &item)
	{
		int index = it - begin();
		// case I - static memory,  enough room
		if (_currSize + 1 <= _capacity && !_isDynamic)
		{ // copying backwards so we wont repeat elements //TODO: add or remove loopProcess
			//_loopProcess(_staticMemory,_staticMemory,_currSize-1,index-1,false,1,0)
			for (int i = _currSize - 1; i > index - 1; i--)
			{
				_staticMemory[i + 1] = _staticMemory[i];
			}
			_staticMemory[index] = item;

		}
		// case II - static memory, not enough room
		else if (_currSize + 1 > _capacity && !_isDynamic)
		{
			_capacity = newCapacity();
			_dynamicMemory = new T[_capacity];
			//_loopProcess(_dynamicMemory,_staticMemory,0,index,true,0,0);
			for(int i = 0; i<index; i++) // all elements up to index
			{
				_dynamicMemory[i] = _staticMemory[i];
			}
			//_loopProcess(_dynamicMemory,_staticMemory,index+1,_currSize,true,0,0);
			for(int i = index+1; i<_currSize; i++) // all elements from index+1
			{
				_dynamicMemory[i] = _staticMemory[i];
			}
			_dynamicMemory[index] = item; // adding new item itself
			_isDynamic = true;
		}
		// case III - dynamic memory, enough room
		else if (_currSize<_capacity && _isDynamic)
		{// copying backwards so we wont repeat elements
			//_loopProcess(_dynamicMemory,_dynamicMemory,_currSize,index-1,false,1,0);
			for (int i = _currSize; i > index - 1; i--) //TODO: this works so fix according to
				// this loop if needed!
			{
				_dynamicMemory[i] = _dynamicMemory[i-1];
			}
			_dynamicMemory[index] = item;
		}
		// case IV - dynamic memory, not enough room
		else if (_currSize >=_capacity &&_isDynamic)
		{
			_capacity = newCapacity();
			T *temp = new T[_capacity];
			//_loopProcess(temp,_dynamicMemory,0,index,true,0,0);
			for(int i = 0; i<index; i++) // all elements up to index
			{
				temp[i] = _dynamicMemory[i];
			}
			//_loopProcess(temp,_dynamicMemory,index+1,_currSize,true,0,0);
			for(int i = index+1; i<_currSize; i++) // all elements from index+1
			{
				temp[i] = _dynamicMemory[i];
			}
			temp[index] = item; // adding new item itself
			delete[] _dynamicMemory;
			_dynamicMemory = new T[_capacity];
			//_loopProcess(_dynamicMemory,temp,0,_currSize+1,true,0,0);
			for (int i = 0; i < _currSize + 1; i++)
			{
				_dynamicMemory[i] = temp[i];
			}
		}
		_currSize++;
		if (_currSize > _staticCap &&!_isDynamic) // this means we need to change to dynamic memory
		{
			_isDynamic = true;
		}
		return it; //TODO: this might not always be the case (for ex. if reallocation was made
	}


};


#endif //TEST_CPP_VLVECTOR_HPP
//TODO add const erase and insert