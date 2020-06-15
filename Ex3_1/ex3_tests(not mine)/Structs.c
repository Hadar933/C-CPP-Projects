#include <string.h>
#include <stdlib.h>
#include "Structs.h"
#include <stdio.h>


#define FAILURE 0;
#define SUCCESS 1;
int gEQ = 0; //equal
int gLT = -1; //less than
int gGT = 1;// greater than

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{

	return strcmp((const char *) a, (const char *) b);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
	char *newLine = strcat((char *) word, "\n");
	strcat(pConcatenated, newLine);
	if (pConcatenated == NULL)
	{
		return FAILURE;
	}
	return SUCCESS
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
	free(s);
	s = NULL;
}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
	const Vector *vector1 = (const Vector *) a;
	const Vector *vector2 = (const Vector *) b;
	for (int i = 0; i < vector1->len; i++)
	{
		if (vector1->vector[i] > vector2->vector[i])
		{
			return gGT;
		}
		else if (vector1->vector[i] < vector2->vector[i])
		{
			return gLT;
		}
	}
	if (vector1->len != vector2->len)
	{
		return (vector1->len > vector2->len) ? gGT : gLT;
	}
	return gEQ;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
	free(((Vector *) pVector)->vector);
	free(pVector);
}

double normSquared(Vector *vec)
{
	double normalSquared = 0;
	for (int i = 0; i < (vec->len); i++)
	{
		normalSquared += (vec->vector[i]) * (vec->vector[i]);
	}
	return normalSquared;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
	if (pVector == NULL || pMaxVector == NULL)
	{
		return FAILURE;
	}
	Vector *vec = (Vector *) pVector;
	Vector *maxVec = (Vector *) pMaxVector;
	double pVecNorm = normSquared(vec);
	double MaxVecNorm = normSquared(maxVec);
	if (pVecNorm > MaxVecNorm && maxVec->vector == NULL)
	{
		((Vector *) pMaxVector)->vector = (double *) malloc(
				sizeof(double) * ((Vector *) pVector)->len);
		((Vector *) pMaxVector)->vector =
				(double *) memcpy(((Vector *) pMaxVector)->vector,
								  ((Vector *) pVector)->vector,
								  sizeof(double) *
								  ((const Vector *) pVector)->len);
		((Vector *) pMaxVector)->len = ((const Vector *) pVector)->len;
	}
	return SUCCESS;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
	if (tree == NULL || tree->root->data == NULL)
	{
		return NULL;
	}
	else
	{    //allocating pointer to a new vector:
		Vector *pMaxVector = (Vector *) malloc(sizeof(Vector));
		if (pMaxVector == NULL)
		{
			exit(EXIT_FAILURE);
		}
		// allocating all the vector's values according to it's size:
		int numOfValeus = ((Vector *) tree->root->data)->len;
		pMaxVector->vector = (double *) malloc(sizeof(double) * numOfValeus);
		if (pMaxVector->vector == NULL)
		{
			exit(EXIT_FAILURE);
		}
		// initializing vector with root's values:
		double *valuesData = ((Vector *) tree->root->data)->vector;
		int lenData = ((Vector *) tree->root->data)->len;
		// copying root's data to pMaxVector:
		pMaxVector->vector = (double *) memcpy(pMaxVector->vector, valuesData,
											   sizeof(double) * numOfValeus);
		pMaxVector->len = lenData;
		// now repeating for all nodes:
		forEachRBTree(tree, copyIfNormIsLarger, (void *) pMaxVector);
		return pMaxVector;
	}
}
