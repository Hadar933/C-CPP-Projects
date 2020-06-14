#include "Matrix.h"
#include "Activation.h"

#ifndef EX4_DENSE_H
#define EX4_DENSE_H

/**
 * class dense represents a layer and we will use it in order
 * to define and perform the operations in the different layers
 */
class Dense
{
public:
	/**
	 * inits a new layer with given parameters
	 * @param w - weights matrix
	 * @param b - bias vector (matrix)
	 * @param actType - Relu or Softmax
	 */
	Dense(Matrix w, Matrix b, ActivationType actType);

	/**
	 * matrix getter
	 * @return w
	 */
	const Matrix &getWeights() const;

	/**
	 * bias getter
	 * @return b
	 */
	const Matrix &getBias() const;

	/**
	 * act type getter
	 * @return actType
	 */
	ActivationType getActivation() const;

	/**
	 * performs the function relu or softmax on the vector w*x+b
	 * where w is the weights matrix and b is the bias vector
	 * @param x - a picture ( vectorized matrix )
	 * @return new matrix after all operations specified above was made
	 */
	Matrix operator()(const Matrix &x);

private:
	Matrix _w, _b;
	ActivationType _actType;
};


#endif //EX4_DENSE_H
