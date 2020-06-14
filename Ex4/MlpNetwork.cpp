#include <iostream>

#include "MlpNetwork.h"

/**
 * constructor initializes all 4 layers
 * @param weights array containing 4 weight matrices
 * @param biases array containing 4 bias matrices
 */
MlpNetwork::MlpNetwork(Matrix *weights, Matrix *biases) :
		_layer1(weights[0], biases[0], Relu),
		_layer2(weights[1], biases[1], Relu),
		_layer3(weights[2], biases[2], Relu),
		_layer4(weights[3], biases[3], Softmax)
{}

/**
 * operator () runs the entire network on given imaage x
 * @param x vectorized image matrix
 * @return digit struct representing probabilities and value guessed
 */
Digit MlpNetwork::operator()(const Matrix &x)
{
	Digit resultValues;
	Matrix r1 = _layer1(x);
	Matrix r2 = _layer2(r1);
	Matrix r3 = _layer3(r2);
	Matrix r4 = _layer4(r3);

	int maxIdx = START_IDX;
	float maxProb = START_PROB;

	for (int i = 0; i < R4_LEN; i++)
	{
		float val = r4[i];
		if (val > maxProb)
		{
			maxProb = val;
			maxIdx = i;
		}
	}
	resultValues.value = maxIdx;
	resultValues.probability = maxProb;
	return resultValues;

}
