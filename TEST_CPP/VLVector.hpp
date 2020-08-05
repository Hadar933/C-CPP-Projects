#ifndef TEST_CPP_VLVECTOR_HPP
#define TEST_CPP_VLVECTOR_HPP

template<class InputIterator>
class VLVector
{
public:
	/**
	 * default ctor - initializes an empty VLVector
	 */
	VLVector():VLVector(nullptr, nullptr){}

	/**
	 * copy ctor
	 * @param vec
	 */
	VLVector(const VLVector &vec): VLVector(vec._first, vec._last)
	{
		*this = vec;
	}
	/**
	 * constructor for VLVector instance
	 * @param value
	 * @param StaticCapacity
	 */
	VLVector(InputIterator& first, InputIterator& last):
	_first(first), _last(last)
	{

	}




private:
	InputIterator _first;
	InputIterator _last;


};



#endif //TEST_CPP_VLVECTOR_HPP
