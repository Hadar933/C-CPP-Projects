#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "RBTree.h"
#include "utilities/RBUtilities.h"

int isProperRBTreePT1(Node* root, CompareFunc cmp)
{
    if (root == NULL)
    {   // leaf
        return true;
    }
    if (root->parent == NULL )
    {
        if (root->color == RED)
        {   // root is red - failed
            printf("FAIL: Root is red\n");
            return false;
        }
    }
    if (root->left != NULL)
    {
        if (cmp(root->left->data, root->data) >= 0) // left >= root - failed
        {
            printf("FAIL: BST violation\n");
            return false;
        }
    }
    if (root->right != NULL)
    {
        if (cmp(root->right->data, root->data) <= 0) // right >= root - failed
        {
            printf("FAIL: BST violation\n");
            return false;
        }
    }
    if (root->right == NULL && root->left == NULL)
    {   // two prev conditions not met, subtree is a sigle black Node
        return true;
    }
    if (root->color == RED)
    {
        if (root->right != NULL && root->right->color == RED)
        {
            printf("FAIL: Double red\n");
            return false;
        }
        if (root->left != NULL && root->left->color == RED)
        {
            printf("FAIL: Double red\n");
            return false;
        }
    }

    return isProperRBTreePT1(root->left, cmp) && isProperRBTreePT1(root->right, cmp);
}

int validateBlackCount(Node* root, int pathCount, int globalCount)
{
    if (root == NULL)
    {
        pathCount ++;
        if (globalCount == pathCount)
        {
            return true;
        } else
        {
            return false;
        }
    }
    if (root->color == BLACK)
    {
        pathCount ++;
    }
    return validateBlackCount(root->right, pathCount, globalCount) &&
            validateBlackCount(root->left, pathCount, globalCount);
}

int isProperRBTreePT2(Node* root)
{
    // count the black nodes on one path, and see that all paths count the same.
    int blackCount = 0;
    Node* N = root;
    while (N != NULL)
    {
        if ( N->color == BLACK )
        {
            blackCount ++;
        }
        N = N->left;
    }
    blackCount += 1; // one for the null node
    if (validateBlackCount(root, 0, blackCount) == false)
    {
        printf("FAIL: black count violation\n");
    }
}

int isProperRBtree(RBTree* T)
{
    return
    isProperRBTreePT1(T->root, T->compFunc) &&
    isProperRBTreePT2(T->root);
}
//
//int cmp(const void* a, const void* b)
//{
//    int* i = (int*)a;
//    int* j = (int*)b;
//
//    if (*i > *j) return 1;
//    if (*i < *j) return -1;
//    if (*i == *j) return 0;
//}
//
//void freeint(void* n) {}
//
//void testInsert(int n)
//{
//    int *input = (int*)calloc(n,sizeof(int));
//    for(int i=0;i<n;i++)
//    {
//        input[i] = i;
//    }
//    FreeFunc f = &freeint;
//    CompareFunc c = &cmp;
//    RBTree* T = newRBTree(c, f);
//    for(int i=0;i<n;i++)
//    {
//        printf("insertion attempt %d: insert %d\n", i, input[i]);
//        insertToRBTree(T, &input[i]);
//        isProperRBtree(T);
//    }
//    if (isProperRBtree(T))
//    {
//        printRBTree(T->root);
//        printf("Insertion successful! :D");
//    }
//
//}
//
//int main()
//{
//    testInsert(40);
//}