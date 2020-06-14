#include <cmath>
#include "Activation.h"

/**
 * returns the sum of all exp(elements) of a matrix
 * i.e sum_ij [ exp(m_ij) ]
 * @param m some matrix
 * @return sum of all items m_ij
 */
float Activation::_matExpSum(const Matrix &m)
{
	float sum = 0;
	int rows = m.getRows();
	int cols = m.getCols();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			sum += std::exp(m(i, j));
		}
	}
	return sum;
}

/**
 * implementation of relu function
 * @param m some matrix
 * @return relu(m)
 */
Matrix Activation::_fRelu(const Matrix &m)
{

	int rows = m.getRows();
	int cols = m.getCols();
	Matrix outMat = Matrix(m);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (m(i, j) < 0)
			{
				outMat(i, j) = 0;
			}
		}
	}
	return outMat;
}

/**
 * softmax activation function - constucts a new matrix such that
 * each element corresponds to (1/sum)*exp(m_ij)
 * @param m some matrix
 * @return softmax(m)
 */
Matrix Activation::_fSoftmax(const Matrix &m)
{
	float expSum = _matExpSum(m);
	int rows = m.getRows();
	int cols = m.getCols();
	Matrix outMat = Matrix(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			float expVal = std::exp(m(i, j));
			outMat(i, j) = (1 / expSum) * expVal;
		}
	}
	return outMat;
}

/**
 * constructor for type
 * @param actType - relu or softmax
 */
Activation::Activation(ActivationType actType) : _actType(actType)
{}

/**
 * type getter
 * @return relu or softmax
 */
ActivationType Activation::getActivationType() const
{
	return _actType;
}

/**
 * activates relu or softmax on matrix m according to actType
 * @param m some matrix
 * @return relu(m) or softmax(m)
 */
Matrix Activation::operator()(const Matrix &m) const
{
	if (_actType == Relu)
	{
		return _fRelu(m);
	}
	else if (_actType == Softmax)
	{
		return _fSoftmax(m);
	}
	std::cout << "no such act type" << std::endl;
	exit(EXIT_FAILURE);
}

