//Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
	Relu,
	Softmax
};

/**
 * implementing the activation functions
 */
class Activation
{
public:
	/**
	 * constructor for type
	 * @param actType - relu or softmax
	 */
	Activation(ActivationType actType);

	/**
	 * activates relu or softmax on matrix m according to actType
	 * @param m some matrix
	 * @return relu(m) or softmax(m)
	 */
	Matrix operator()(const Matrix &m) const;

	/**
	 * type getter
	 * @return relu or softmax
	 */
	ActivationType getActivationType() const;

private:
	ActivationType _actType;

	/**
	 * implementation of relu function
	 * @param m some matrix
	 * @return relu(m)
	 */
	static Matrix _fRelu(const Matrix &m);

	/**
	 * softmax activation function - constucts a new matrix such that
	 * each element corresponds to (1/sum)*exp(m_ij)
	 * @param m some matrix
	 * @return softmax(m)
	 */
	static Matrix _fSoftmax(const Matrix &m);

	/**
	 * returns the sum of all exp(elements) of a matrix
	 * i.e sum_ij [ exp(m_ij) ]
	 * @param m some matrix
	 * @return sum of all items m_ij
	 */
	static float _matExpSum(const Matrix &m);


};

#endif //ACTIVATION_H
