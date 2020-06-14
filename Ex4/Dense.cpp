#include "Dense.h"

/**
 * inits a new layer with given parameters
 * @param w - weights matrix
 * @param b - bias vector (matrix)
 * @param actType - Relu or Softmax
 */
Dense::Dense(Matrix w, Matrix b, ActivationType actType) :
		_w(w), _b(b), _actType(actType)
{}

/**
 * matrix getter
 * @return w
 */
const Matrix &Dense::getWeights() const
{
	return _w;
}

/**
 * bias getter
 * @return b
 */
const Matrix &Dense::getBias() const
{
	return _b;
}

/**
 * act type getter
 * @return actType
 */
ActivationType Dense::getActivation() const
{
	return _actType;
}

/**
 * performs the function relu or softmax on the vector w*x+b
 * where w is the weights matrix and b is the bias vector
 * @param x - a picture ( vectorized matrix )
 * @return new matrix after all operations specified above was made
 */
Matrix Dense::operator()(const Matrix &x)
{
	Activation activ = Activation(_actType);
	Matrix outMat = Matrix(x);
	outMat = (_w * x) + _b;
	outMat = activ(outMat); // r = relu/softmax(w * x + b)
	return outMat;
}