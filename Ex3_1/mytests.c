#include <stdio.h>
#include <stdlib.h>
#include "RBTree.h"
#include "Structs.h"
#include "RBUtilities.h"

int compInt(void *data1, void *data2)
{
	int a = *((int *) data1);
	int b = *((int *) data2);
	if (a == b)
	{
		return 0;
	}
	if (a > b)
	{
		return 1;
	}
	return -1;
}


void insert(RBTree *t, void *data, const int i, const void *arr)
{
	insertToRBTree(t, data);
	if (!isValidRBTree(t))
	{
		printf("ERROR - after the insertion of '%d', the tree is not valid\n", *((int *) data));
		int *arrInt = (int *) arr;
		printf("tried to insert in this order: ");
		for (int j = 0; j < i; j++)
		{
			printf("%d ", arrInt[j]);
		}
		printf("\n");
		printRBTree(t->root);
		freeRBTree(&t);
		exit(EXIT_FAILURE);
	}
}

void delete(RBTree *t, void *data, const int i, const void *arr)
{
	if (deleteFromRBTree(t, data) == 0)
	{
		t->freeFunc(data);
	}

	if (!isValidRBTree(t))
	{
		printf("ERROR - after the deletion of '%d', the tree is not valid\n", *((int *) data));
		int *arrInt = (int *) arr;
		printf("tried to delete in this order: ");
		for (int j = 0; j < i; j++)
		{
			printf("%d ", arrInt[j]);
		}
		printf("\n");
		printRBTree(t->root);
		freeRBTree(&t);
		exit(EXIT_FAILURE);
	}
}

void intFree(void *data)
{
	int stam_to_not_make_error = 0;
}

int main()
{
	int a[] = {3}; //todo - change this array's values!

	RBTree *t = newRBTree((CompareFunc) &compInt, (FreeFunc) free);
	printf("Ints tree with %lu nodes: ", sizeof(a) / sizeof(a[0]));
	for (int j = 0; j < sizeof(a) / sizeof(a[0]); j++)
	{
		insert(t, &a[j], sizeof(a) / sizeof(a[0]), a);
	}
	// finished insertion
	printf("Insert successful\n");
	printRBTree(t->root);
	printf("starting to delete\n");
	for (int j = 0; j < sizeof(a) / sizeof(a[0]); j++)
	{
		printf("tree size before deletion of %d = %d\n", a[j], (int) t->size);
		delete(t, &a[j], sizeof(a) / sizeof(a[0]), a);
		printf("tree size after deletion of %d = %d\n", a[j], (int) t->size);
		printf("tree after deleting %d\n", a[j]);
		printRBTree(t->root);

	}
	if (t->size != 0)
	{
		printf("ERROR - deleted all the nodes from the tree and yet the tree's size if not 0\n");
	}
	freeRBTree(&t);
	printf("passed\n");
	return EXIT_SUCCESS;
}