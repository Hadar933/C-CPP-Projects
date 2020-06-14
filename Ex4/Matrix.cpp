#include "Matrix.h"
#include <string>
#include <iostream>

#define INPUT_ERR_MSG  "One of the inputs is invalid - Exiting..."
#define MEMORY_ERR_MSG  "Memory initialization falied - Exiting..."
#define INITIAL_SIZE 1

/**
 * prints an error message to cerr and exists with exit failure code
 */
void Matrix::_inputError()
{
	std::cerr << INPUT_ERR_MSG << std::endl;
	exit(EXIT_FAILURE);
}

/**
 * prints an error message to cerr and exists with exit failure code
 */
void Matrix::_memoryError()
{
	std::cerr << INPUT_ERR_MSG << std::endl;
	exit(EXIT_FAILURE);
}

/**
 * * used for matrices multiplication. returns the value of c_ij according to
 * c_ij = a_ik * b_kj , where k is a summation index
 * @param a - some matrix
 * @param b - some matrix
 * @param k - amount to sum
 * @param i - current row
 * @param j -current column
 * @return c_ij as specified
 */
float Matrix::_getSum(const Matrix &a, const Matrix &b, int k, int i, int j)
{
	float c_ij = 0;
	for (int l = 0; l < k; l++)
	{
		c_ij += a(i, l) * b(l, j);
	}
	return c_ij;
}

/**
 *  constructor for martix instance, initializes all matrix values to zero
 * @param rows - number of rows
 * @param cols - number of columns
 */
Matrix::Matrix(int rows, int cols) :
		_rows(rows), _cols(cols), _mat(nullptr)
{
	if (rows <= 0 || cols <= 0) // checking valid input
	{
		_inputError();
	}
	_mat = new(std::nothrow) float[rows * cols]; // allocating memory
	if (_mat == nullptr)
	{
		_memoryError();
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			(*this)(i, j) = 0;
		}
	}
}

/**
 *default constructor - initializes a matrix of size 1x1
 */
Matrix::Matrix() : Matrix(INITIAL_SIZE, INITIAL_SIZE)
{}

/**
 * copy constructor
 * @param m some matrix
 */
Matrix::Matrix(const Matrix &m) : Matrix(m._rows, m._cols)
{
	*this = m;
}

/**
 * destructor for class Matrix - frees _mat from memory
 */
Matrix::~Matrix()
{
	delete[] _mat;
	_mat = nullptr;
}

/**
 * @return amount of rows as int
 */
int Matrix::getRows() const
{
	return _rows;
}

/**
 * @return amount of columns as int
 */
int Matrix::getCols() const
{
	return _cols;
}

/**
 * converts a matrix into a column vector
 * @return
 */
Matrix &Matrix::vectorize()
{
	int newCols = INITIAL_SIZE; // 1 column
	int newRows = _cols * _rows;
	_rows = newRows;
	_cols = newCols;
	return *this;
}

/**
 * prints this matrix
 */
void Matrix::plainPrint() const
{
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			std::cout << (*this)(i, j) << " ";
		}
		std::cout << std::endl;
	}
}

/**
 * assignment opeartor - this = b
 * @param b some matrix
 * @return this matrix after assignment to b
 */
Matrix &Matrix::operator=(const Matrix &b)
{
	if (this != &b) // b is not this
	{
		delete[] _mat;
		_mat = nullptr;
		_mat = new(std::nothrow) float[b._rows * b._cols];
		if (_mat == nullptr)
		{
			_memoryError();
		}
		for (int i = 0; i < b._rows * b._cols; i++)
		{
			_mat[i] = b._mat[i];
		}
		_rows = b._rows;
		_cols = b._cols;
	}
	return *this;
}

/**
 * returns this matrix multiplied by c (const)
 * notice that this is a member function, performed on this matrix
 * @param c - some value
 * @return M*c
 */
Matrix &Matrix::operator*(float c)
{
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			(*this)(i, j) *= c;
		}
	}
	return *this;
}

/**
 * returns c*M. notice this is not a member function (does not operates on this matrix)
 * @param c - some value
 * @param m - some matrix
 * @return
 */
Matrix &operator*(float c, Matrix &m)
{
	for (int i = 0; i < m._rows; i++)
	{
		for (int j = 0; j < m._cols; j++)
		{
			m(i, j) *= c;
		}
	}
	return m;
}

/**
 * adds two matrices together
 * @param a some matrix
 * @param b some matrix
 * @return a+b
 */
Matrix operator+(const Matrix &a, const Matrix &b)
{
	if (a._rows != b._rows || a._cols != b._cols)
	{
		a._inputError(); // matrices must be of same dimension
	}
	int rows = a._rows;
	int cols = a._cols;
	Matrix newMat = Matrix(rows, cols);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			newMat(i, j) = a(i, j) + b(i, j);
		}
	}
	return newMat;
}

/**
 * multiplies two matrices
 * @param a - some matrix (must be m*n)
 * @param b - some matrix (must be n*p)
 * @return new matrix a*b (m*p)
 */
Matrix operator*(const Matrix &a, const Matrix &b)
{
	if (a._cols != b._rows)
	{
		a._inputError(); // matrices must be of dimensions a_(m * n) * b_(n * p)
	}
	// assigning letters to correspond the values in description:
	int m = a._rows; // = c._rows
	int n = a._cols; // = b._rows
	int p = b._cols; // = c._cols

	Matrix c = Matrix(m, p);
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < p; j++)
		{ // c_ij will be calculated using a helper function
			c(i, j) = c._getSum(a, b, n, i, j);
		}
	}
	return c;
}

/**
 * for every element of this matrix perfoms += with element of b
 * i.e this_ij = b_ij
 * @param b some matrix
 * @return new this after +=
 */
Matrix &Matrix::operator+=(const Matrix &b)
{
	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			(*this)(i, j) += b(i, j);
		}
	}
	return *this;
}

/**
 * fetches the i'th row and j'th column item of this matrix,
 * dealing with some edge cases
 * @param i row index
 * @param j column index
 * @return this_ij
 */
float &Matrix::operator()(int i, int j)
{
	if (i * _cols + j < 0 || i >= _rows || j >= _cols || j < 0 || i < 0)
	{
		_inputError();
	}
	return _mat[i * _cols + j];
}

/**
 * fetches the i'th element of this matrix. since this is
 * represented by a single array, this function simply returns matrix[i]
 * @param i
 * @return i'th element if this matrix
 */
float &Matrix::operator[](int i)
{
	if (i < 0 || i > _cols * _rows - 1) // cols * rows - 1 is the last index reachable
	{
		_inputError();
	}
	return _mat[i];
}

/**
 * same as the non- const operator (), just for const instances
 * @param i row index
 * @param j column index
 * @return this_ij
 */
float &Matrix::operator()(int i, int j) const
{
	if (i * _cols + j < 0 || i >= _rows || j >= _cols || j < 0 || i < 0)
	{
		_inputError();
	}
	return _mat[i * _cols + j];
}

/**
 * just like the operator [], but for const instances
 * @param i - index
 * @return i'th element of array _mat
 */
float &Matrix::operator[](int i) const
{
	if (i < 0 || i > _cols * _rows - 1) // cols * rows - 1 is the last index reachable
	{
		_inputError();
	}
	return _mat[i];
}

/**
 * << operator, as specified in part 3.4 of the exercise
 * prints the quessed number to the screen
 * @param os output stream
 * @param m matrix
 * @return os
 */
std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
	int rows = m._rows;
	int cols = m._cols;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (m(i, j) <= 0.1f)
			{
				std::cout << "  ";
			}
			else
			{
				std::cout << "**";
			}
		}
		std::cout << std::endl;
	}
	return os;

}

/**
 * >> operator, reads the input and inserts it into the matrix
 * @param is input stream
 * @param m matrix
 * @return is
 */
std::istream &operator>>(std::istream &is, const Matrix &m)
{
	int rows = m._rows;
	int cols = m._cols;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (!is.good())
			{
				m._inputError();
			}
			is.read((char *) &m(i, j), sizeof(float));
		}
	}
	return is;
}
