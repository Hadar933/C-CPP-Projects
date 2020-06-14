// Matrix.h

#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>

/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
	int rows, cols;
} MatrixDims;

/**
 * class matrix is responsible for creating instances of matrices and
 * vectors that are crucial for representing weights, biases and binary files
 */
class Matrix
{
public:
	/**
	 *  constructor for martix instance, initializes all matrix values to zero
	 * @param rows - number of rows
	 * @param cols - number of columns
	 */
	Matrix(int rows, int cols);

	/**
	 *default constructor - initializes a matrix of size 1x1
	 */
	Matrix();

	/**
	 * copy constructor
	 * @param m some matrix
	 */
	Matrix(const Matrix &m);

	/**
	 * destructor for class Matrix - frees _mat from memory
	 */
	~Matrix();

	/**
	 * @return amount of rows as int
	 */
	int getRows() const;

	/**
	 * @return amount of columns as int
	 */
	int getCols() const;

	/**
	 * converts a matrix into a column vector
	 * @return
	 */
	Matrix &vectorize();

	/**
	 * prints this matrix
	 */
	void plainPrint() const;

	/**
	 * assignment opeartor - this = b
	 * @param b some matrix
	 * @return this matrix after assignment to b
	 */
	Matrix &operator=(const Matrix &b);

	/**
	 * returns this matrix multiplied by c
	 * notice that this is a member function, performed on this matrix
	 * @param c - some value
	 * @return M*c
	 */
	Matrix &operator*(float c);

	/**
	 * returns c*M. notice this is not a member function (does not operates on this matrix)
	 * @param c - some value
	 * @param m - some matrix
	 * @return
	 */
	friend Matrix &operator*(float c, Matrix &m);

	/**
	 * adds two matrices together
	 * @param a some matrix
	 * @param b some matrix
	 * @return a+b
	 */
	friend Matrix operator+(const Matrix &a, const Matrix &b);

	/**
	 * multiplies two matrices
	 * @param a - some matrix (must be m*n)
	 * @param b - some matrix (must be n*p)
	 * @return new matrix a*b (m*p)
	 */
	friend Matrix operator*(const Matrix &a, const Matrix &b);

	/**
	 * for every element of this matrix perfoms += with element of b
	 * i.e this_ij = b_ij
	 * @param b some matrix
	 * @return new this after +=
	 */
	Matrix &operator+=(const Matrix &b);

	/**
	 * fetches the i'th row and j'th column item of this matrix,
	 * dealing with some edge cases
	 * @param i row index
	 * @param j column index
	 * @return this_ij
	 */
	float &operator()(int i, int j);

	/**
	 * fetches the i'th element of this matrix. since this is
	 * represented by a single array, this function simply returns matrix[i]
	 * @param i
	 * @return i'th element if this matrix
	 */
	float &operator[](int i);

	/**
	 * same as the non- const operator (), just for const instances
	 * @param i row index
	 * @param j column index
	 * @return this_ij
	 */
	float &operator()(int i, int j) const;

	/**
	 * just like the operator [], but for const instances
	 * @param i - index
	 * @return i'th element of array _mat
	 */
	float &operator[](int i) const;

	/**
	 * << operator, as specified in part 3.4 of the exercise
	 * prints the quessed number to the screen
	 * @param os output stream
	 * @param m matrix
	 * @return os
	 */
	friend std::ostream &operator<<(std::ostream &os, const Matrix &m);

	/**
	 * >> operator, reads the input and inserts it into the matrix
	 * @param is input stream
	 * @param m matrix
	 * @return is
	 */
	friend std::istream &operator>>(std::istream &is, const Matrix &m);

private:
	int _rows, _cols;
	float *_mat;

	/**
	 * prints an error message to cerr and exists with exit failure code
	 */
	static void _inputError();

	/**
	 * prints an error message to cerr and exists with exit failure code
	 */
	static void _memoryError();

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
	static float _getSum(const Matrix &a, const Matrix &b, int k, int i, int j);

};

#endif //MATRIX_H
