#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::ostream;


class Point {
private:
	int _c[2];

public:
	Point(int x, int y) {
		_c[0] = x;
		_c[1] = y;
	}

	int getX() const { return _c[0]; }
	int getY() const { return _c[1]; }

	/* Operator defined as member function is called on instance of the class.
	   Only the right hand side is passed as a parameter */
	bool operator==(const Point& rhs) const {
		return _c[0] == rhs._c[0] && _c[1] == rhs._c[1];
	}

	// addition of another Point
	Point& operator+=(const Point& rhs) {
		_c[0] += rhs._c[0];
		_c[1] += rhs._c[1];
		return *this;
	}

	// addition of int
	Point& operator+=(int val) {
		_c[0] += val;
		_c[1] += val;
		return *this;
	}

	// return -Point
	Point operator-() {
		_c[0] *= -1;
		_c[1] *= -1;
		return *this;
	}


	// pre increment
	Point operator++() {
		_c[0]++;
		_c[1]++;
		return *this;
	}

	// post increment
	Point operator++(int) {
		Point tmp = *this;
		_c[0]++;
		_c[1]++;
		return tmp;
	}

	/* const access version
	   Acceptable indices 0,1. Others can result in run-time error */
	const int& operator[](const unsigned short coord) const {
		return _c[coord];
	}

	// non-nonst access
	int& operator[](const unsigned short coord) {
		return _c[coord];
	}

	/* Friends */

	// Returns a Point that is the result of addition of two Points.
	friend Point operator+(const Point& lhs, const Point& rhs) {
		return Point(lhs[0] + rhs[0], lhs[1] + rhs[1]);
	}

	// Outputs coordinates delimited by single space.
	friend std::ostream& operator<<(std::ostream& s, const Point& p) {
		return s << p[0] << ' ' << p[1];
	}

	// Inputs coordinates delimited by single space.
	friend std::istream& operator>>(std::istream& s, Point& p) {
		return s >> p[0] >> '  ' >> p[1];
	}
};

/* Operator defined outside as non-member function */
bool operator<(const Point& lhs, const Point& rhs) {
	return lhs[0] < rhs[0] || (lhs[0] == rhs[0] && lhs[1] < rhs[1]);
}