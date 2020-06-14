//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Digit.h"
#include "Matrix.h"
#include "Dense.h"

#define MLP_SIZE 4
#define R4_LEN 10 // size of r4 - last layer
# define START_IDX 0 // index of number guessed my MLP
# define START_PROB 0 // probability of MLP guess

const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784},
								  {64,  128},
								  {20,  64},
								  {10,  20}};
const MatrixDims biasDims[] = {{128, 1},
							   {64,  1},
							   {20,  1},
							   {10,  1}};
/**
 * mlpnetwork class is made to arrange all layers to a network
 * and will enable inserting input to the network and extracting
 * the relevant output.
 */
class MlpNetwork
{
public:
	/**
	 * constructor initializes all 4 layers
	 * @param weights array containing 4 weight matrices
	 * @param biases array containing 4 bias matrices
	 */
	MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]);
	/**
	 * operator () runs the entire network on given imaage x
	 * @param x vectorized image matrix
	 * @return digit struct representing probabilities and value guessed
	 */
	Digit operator()(const Matrix &x);

private:
	Dense _layer1, _layer2, _layer3, _layer4;
};

#endif // MLPNETWORK_H
