#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RBTree.h"
#include "Structs.h"

#define SUCCESS 1
#define FAILURE 0
#define INITIAL_SIZE 0 // initial tree size
int gChainPosition = 0; // boolean for insertion, indicating we're in case 4a(I)-0 or 4a(II)-1
int gDecreasedSize = false; // boolean indicating if we already updated size (tree->size -- )
int freedM = false; // boolean indicating we have freed current node
int freedRep = false; // boolean indicating we have freed node replacing current node (in delete)

/**
 *initializes a new RB tree
 * @param compFunc - designated function to compare elements in tree
 * @param freeFunc - degisnated function to free the memory
 * @return pointer to new empty RB tree
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	if (compFunc == NULL || freeFunc == NULL)
		// cannot initialize RBTree
	{
		return NULL;
	}
	else
	{
		// initializing new empty RBTree
		RBTree *newTree;
		newTree = (RBTree *) malloc(sizeof(RBTree));
		if (newTree == NULL)
		{
			exit(EXIT_FAILURE);
		}
		newTree->size = INITIAL_SIZE;
		newTree->compFunc = compFunc;
		newTree->freeFunc = freeFunc;
		newTree->root = NULL;
		return newTree;
	}
}

/**
 * performs right rotation on node x
 * @param tree
 * @param x
 */
void RightRotation(RBTree *tree, Node *x)
{
	Node *c = x->left;
	x->left = c->right;
	if (x->left != NULL)
	{
		x->left->parent = x;
	}
	c->parent = x->parent; //c->parent = y
	if (x->parent == NULL) //updating root
	{
		tree->root = c;
	}
	else if (x == x->parent->left) // x is left child
	{
		x->parent->left = c;
	}
	else //x is right child
	{
		x->parent->right = c;
	}
	c->right = x;
	x->parent = c;
}

/**
 * performs left rotationon node x
 * @param tree
 * @param x
 */
void LeftRotation(RBTree *tree, Node *x)
{

	Node *b = x->right;
	x->right = b->left;
	if (x->right != NULL)
	{
		x->right->parent = x;
	}
	b->parent = x->parent;
	if (x->parent == NULL) //updating root
	{
		tree->root = b;
	}
	else if (x == x->parent->left) // x is left child
	{
		x->parent->left = b;
	}
	else // x is right child
	{
		x->parent->right = b;
	}
	b->left = x;
	x->parent = b;
}

/**
 * checks if node is a left child to it's parent
 * @param node
 * @return 1 is yes, 0 if not
 */
int isLeftChild(const Node *node)
{
	if (node == NULL || node->parent == NULL)
	{
		return false;
	}
	if (node->parent->left == node)
	{
		return true;
	}
	return false;
}

/**
 * cheks if node is right child
 * @param node
 * @return 1 if yes, 0 otherwise
 */
int isRightChild(const Node *node)
{
	if (node == NULL || node->parent == NULL)
	{
		return false;
	}
	if (node->parent->right == node)
	{
		return true;
	}
	return false;
}

/**
 * when rotating, some updated to the root are obliged
 * @param tree
 * @param node
 */
void updateRoot(RBTree *tree, Node *node)
{
	if (node == tree->root)
	{
		tree->root = node->parent;
		tree->root->color = BLACK;
	}
}

/**
 * function that returns a tree to its RED BLACK format after nodes insertions
 * @param tree
 * @param node - node that was inserted
 */
void fixAfterInsertion(RBTree *tree, Node *node)
{
	Node *P = NULL; //parent
	Node *G = NULL; // grandparent
	//Part 1 of the algorithm, also the recursion base-case:
	if (node)
	{
		if (node->parent == NULL) // node is root
		{
			node->color = BLACK;
			return;
		}
			//part 2 of the algorithm
		else if (node->parent->color == BLACK)
		{
			return;
		}
		// now P !=NULL so we can initialize it and grandparent(G) and uncle(U)
		P = node->parent;
		G = P->parent;
		// either uncle1 or uncle2 is actually P, but we check both either case
		Node *U1 = G->right;
		Node *U2 = G->left;
		// part 3 of the algorithm
		if (P->color == RED && U1 && U2 && U1->color == RED && U2->color == RED)
		{
			G->right->color = BLACK;
			G->left->color = BLACK;
			G->color = RED;
			fixAfterInsertion(tree, G);
		}
			//part 4 of the algorithm
		else //if (P is RED && Uncle is BLACK)
		{
			// CASE I - (node is right child of P AND P is left child of G) ->rotate node left
			if (isRightChild(node) && isLeftChild(P))
			{
				LeftRotation(tree, P);
				updateRoot(tree, P);
				gChainPosition = 1;
			}
				//CASE II - (node is left child of P AND P is right child of G) ->rotate node right
			else if (isLeftChild(node) && isRightChild(P))
			{
				RightRotation(tree, P);
				updateRoot(tree, P);
				gChainPosition = 1;
			}
			if (gChainPosition)
			{
				P = node;
			}
			//CASE III - (node is left child of P AND P is left child of G) -> rotate G right
			if (isLeftChild(node) && isLeftChild(P))
			{
				RightRotation(tree, G);
				updateRoot(tree, G);
			}
				//CASE IV - (node is right child of P AND P is right child of G) -> rotate G left
			else if (isRightChild(node) && isRightChild(P))
			{
				LeftRotation(tree, G);
				updateRoot(tree, G);
			}
			gChainPosition = 0;
			P->color = BLACK;
			G->color = RED;
		}
	}
}

/**
 * returns the node if it is i nthe tree
 * @param tree
 * @param data the wanted nodes data
 * @return Node or null
 */
Node *getNode(const RBTree *tree, const void *data)
{
	Node *currNode = tree->root;
	while (currNode != NULL)
	{
		if ((tree->compFunc(currNode->data, data)) > 0) // curr.data > data
		{
			currNode = currNode->left;
		}
		else if ((tree->compFunc(currNode->data, data)) < 0)// curr.data < data
		{
			currNode = currNode->right;
		}
		else if((tree->compFunc(currNode->data, data)) == 0)
		{
			return currNode;
		}
	}
	return NULL;
}

/**
 * chekcs if tree contains some node with given data
 * @param tree
 * @param data -data of node to be found
 * @return 1 if node is in the tree. 0 otherwise
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
	if (data == NULL || tree == NULL)
	{
		return false;
	}
	if (getNode(tree, data) == NULL)
	{
		return false;
	}
	return true;
}

/**
 * function that initializes a struct node
 * @param parent node's parent
 * @param data node's data
 * @return the new node object
 */
Node *initializeNode(Node *parent, void *data)
{
	Node *node = (Node *) malloc(sizeof(Node));
	if (node == NULL)
	{
		exit(EXIT_FAILURE);
	}
	node->left = NULL;
	node->right = NULL;
	node->data = data;
	node->parent = parent;
	node->color = RED;
	return node;
}

/**
 * inserts a node with the given data to the tree, and fixes it
 * @param tree
 * @param data
 * @return 1 upon success, 0 otherwise
 */
int insertToRBTree(RBTree *tree, void *data)
{
	if (tree == NULL || data == NULL)
	{
		return FAILURE;
	}
	Node *currNode = tree->root;
	Node *parent = currNode;
	int rightSideConnection = true;
	while (currNode != NULL)
	{
		if ((tree->compFunc(currNode->data, data)) > 0) // curr.data > data
		{
			parent = currNode;
			currNode = currNode->left;
			rightSideConnection = false;
		}
		else if ((tree->compFunc(currNode->data, data)) < 0)// curr.data < data
		{
			parent = currNode;
			currNode = currNode->right;
			rightSideConnection = true;
		}
		else //currNode.data = data i.e node to insert already in tree
		{
			return FAILURE;
		}
	}
	// initialize the node:
	Node *nodeToInsert = initializeNode(parent, data);
	// connect the parent to the node:
	if (parent == NULL)
	{
		tree->root = nodeToInsert;
	}
	else
	{
		if (rightSideConnection)
		{
			parent->right = nodeToInsert;
		}
		else
		{
			parent->left = nodeToInsert;
		}
	}
	//fixing the tree after insertion:
	fixAfterInsertion(tree, nodeToInsert);
	tree->size++;
	return SUCCESS;
}

/**
 * a helper function for foreach function
* @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support)
 * @return: 0 on failure, other on success.
 */
int forEachHelper(const Node *root, forEachFunc func, void *args)
{
	if (root == NULL)
	{
		return SUCCESS;
	}
	int bool1 = forEachHelper(root->left, func, args); // recurse on the left child
	int bool2 = func(root->data, args); // executing func on the current node's data
	int bool3 = forEachHelper(root->right, func, args); //recurse on the right child
	if (bool1 && bool2 && bool3)
	{
		return SUCCESS;
	}
	return FAILURE;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order.
 * if one of the activations of the function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support)
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
	if (tree == NULL || func == NULL)
	{
		return FAILURE;
	}
	return forEachHelper(tree->root, func, args);
}


/**
 * helper function for freeRBtree - frees in order traversal
 * @param currNode starting node to free from
 * @param freefunc function that frees the data
 */
void freeRBhelper(Node **currNode, FreeFunc freefunc)
{
	if (*currNode == NULL)
	{
		return;
	}// freeing data in order:
	freeRBhelper(&(*currNode)->left, freefunc);
	freeRBhelper(&(*currNode)->right, freefunc);
	freefunc((*currNode)->data);
	free(*currNode);
}

/**
 * frees entire RB tree
 * @param tree
 */
void freeRBTree(RBTree **tree)
{
	// **we use pointers to pointers in order work on the original data
	// rather than it's copy (when sent as an argument)
	freeRBhelper(&((*tree)->root), (*tree)->freeFunc); // freeing all data (in-order traversal)
	free(*tree);
}

/**
 * returns smallest node that is bigger than node which it's right child is input node
 * @param node - root of right sub tree of the node we want to find it's successor
 * @return successor
 */
Node *successor(Node *node)
{
	Node *temp = node;
	while (temp->left != NULL)
	{
		temp = temp->left;
	}
	return temp;
}

/**
 * find the node that needs to be replaced with the removed node
 * @param node - node to be removed
 * @return node to be replaced
 */
Node *getReplacingNode(Node *node)
{
	// no children - node is leaf
	if (node->left == NULL && node->right == NULL)
	{
		return NULL;
	}
		// node has 2 children
	else if (node->left != NULL && node->right != NULL)
	{
		return successor(node->right);

	}//only left child
	else if (node->left && node->right == NULL)
	{
		return node->left;
	}
	else//only right child
	{
		return node->right;
	}
}

/**
 * returns the brother of the given nude
 * @param node
 * @return brother node or NULL
 */
Node *getBrother(Node *node)
{
	if (node->parent == NULL)
	{
		return NULL;
	}
	else
	{ // node is left child
		if (node == node->parent->left)
		{
			return node->parent->right;
		}
		else
		{
			return node->parent->left;
		}
	}
}

/**
 * swap two given nodes data
 * @param M - first node
 * @param C -second node
 */
void swapNodes(Node *M, Node *C)
{
	void *tempData = M->data;
	M->data = C->data;
	C->data = tempData;
}

/**
 * when deleting, a double black node position might occur, we need to fix that!
 * @param tree
 * @param node node from which a problem might occur
 */
void fixDoubleBlack(RBTree *tree, Node *node)
{
	if (node == tree->root)
	{
		return;
	}
	Node *B = getBrother(node);
	Node *P = node->parent;
	if (B == NULL) // this means we have at least two connected node that are BLACK
	{
		fixDoubleBlack(tree, P);
	}
	else // there is a brother
	{
		if (B->color == RED)
		{
			P->color = RED;
			B->color = BLACK;
			if (isLeftChild(B))
			{
				RightRotation(tree, P);
			}
			else
			{
				LeftRotation(tree, P);
			}
			fixDoubleBlack(tree, node);
		}
		else
		{
			if ((B->left && B->left->color == RED) || (B->right && B->right->color == RED))
			{ // at least one of B's children is RED
				if (B->left != NULL && B->left->color == RED)
				{ // left child of B is RED
					if (isLeftChild(B))
					{ // left child of left child
						B->left->color = B->color;
						B->color = P->color;
						RightRotation(tree, P);
					}
					else
					{ // left child of right child
						B->left->color = P->color;
						RightRotation(tree, B);
						LeftRotation(tree, P);
					}
				}
				else
				{ // right child of B is RED
					if (isLeftChild(B))
					{
						B->right->color = P->color;
						LeftRotation(tree, B);
						RightRotation(tree, P);
					}
					else
					{ // B is right child
						B->right->color = B->color;
						B->color = P->color;
						LeftRotation(tree, P);
					}
				}
				P->color = BLACK;
			}
			else // B has no red children
			{
				B->color = RED;
				if (P->color == BLACK)
				{
					fixDoubleBlack(tree, P);
				}
				else
				{
					P->color = BLACK;
				}
			}
		}
	}
}

/**
 * after deleting a node, we need to update the size of the tree
 * @param tree
 */
void decreaseSize(RBTree *tree)
{
	if (!gDecreasedSize) // if we already decreased size in the deletion
		// same run we do nothing
	{
		tree->size--;
		gDecreasedSize = 1;
	}
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTreeHelper(RBTree *tree, void *data, Node *nodeAddress)
{
	freedRep = false; // havent freed replacer node of deleted node
	freedM = false; // havent freed the node to be deleted
	if (data == NULL || tree == NULL)
	{
		return FAILURE;
	}
	gDecreasedSize = 0;
	Node *M = NULL;

	if (nodeAddress == NULL)
	{
		Node* fromTree = getNode(tree,data);
		M = fromTree;
	}
	else // nodeAddress is given if we have swapped two nodes, so we skip finding it again
	{
		M = nodeAddress;
	}
	if (M == NULL)
	{
		return FAILURE;
	}
	Node *P = M->parent; // parent of M
	Node *rep = getReplacingNode(M); //replacer of M
	int bothBlack = false; // rep and M are black
	if ((rep == NULL || rep->color == BLACK) && (M->color == BLACK))
	{
		bothBlack = true;
	}
	if (rep == NULL) //  M is leaf
	{
		if (M == tree->root) // remove the root
		{
			if((M->data)!=NULL)
			{
				tree->freeFunc(M->data);
				tree->root = NULL;
			}
		}
		else
		{
			if (bothBlack)
			{
				fixDoubleBlack(tree, M);
			}
			else // at least one is red
			{
				Node *B = getBrother(M);
				if (B != NULL)
				{
					B->color = RED;
				}
			}
			// we now disconnect M from parent M
			if (isLeftChild(M))
			{
				P->left = NULL;
			}
			else
			{
				P->right = NULL;
			}
		} // free M
		if(tree->root)
		{
			tree->freeFunc(M->data);
		}
		free(M);
		freedM=true;
		decreaseSize(tree);
		return SUCCESS;

	} // rep has one child
	if (M->left == NULL || M->right == NULL)
	{
		if (M == tree->root)
		{
			void* tempData = M->data;
			M->data = rep->data;
			rep->data = tempData;
			M->left = M->right = NULL;
			// free rep:
			tree->freeFunc(rep->data);
			free(rep);
			freedRep = true;
		}
		else//rep is not root
		{
			if (isLeftChild(M))
			{
				P->left = rep;
			}
			else
			{
				P->right = rep;
			}
			// free M
			if(!freedM)
			{
				tree->freeFunc(M->data);
				free(M);
				freedM = true;
			}
			rep->parent = P;
			if (bothBlack)
			{
				fixDoubleBlack(tree, rep);
			}
			else
			{
				rep->color = BLACK;
			}
		}
		decreaseSize(tree);
		return SUCCESS;
	}// we're left with the option of two children to rep
	swapNodes(M, rep);
	deleteFromRBTreeHelper(tree, rep->data, rep);
	gDecreasedSize = 0;
	return SUCCESS;

}

/**
 * the main function calls its helper with nodeAddress = NULL. this adress comes handy
 * when the address of the wanted node for deletion is already known, in cases of switching
 * (as a part of the deletion algorithm).this saven run time later on
 * @param tree
 * @param data - data of node to be removed
 * @return 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
	return deleteFromRBTreeHelper(tree, data, NULL);
}
