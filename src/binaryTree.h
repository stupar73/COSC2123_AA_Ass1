/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * binaryTree - Binary tree module
 ******************************************************************************/

#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_


#include <stdlib.h>
#include <string.h>


/*
 * Binary tree node data type.
 */
typedef struct binTreeNode
{
    int key;
    void *data; /* pointer to the data that the node holds */
    struct binTreeNode *left, *right; /* Left and right children subtrees */
    void (*freeFunc)(void *); /* Generic free function */
} binTreeNode_t;

/* 
 * Create a tree.
 */
binTreeNode_t* createTreeNode(int key, void *data, void (*freeFunc)(void*));


/* 
 * Destroy a tree.
 */
void destroyTree(binTreeNode_t *tree);


/* 
 * Destroy a node
 */
void destroyTreeNode(binTreeNode_t *node);


/*
 * Insert a value.
 * NOTE: Duplicates are allowed.
 */
int insertTreeNode(binTreeNode_t **pTree, binTreeNode_t *pNewNode);


/*
 * Delete the first node found with value.
 */
void deleteTreeNode(binTreeNode_t **ppTree, binTreeNode_t *pDelNode,
        binTreeNode_t *pDelParent, int bleftChild);


/*
 * Find node with key, and also return by reference that node's parent and
 * whether it is a left or right child of the parent node.
 */
binTreeNode_t* searchdeleteListNode(binTreeNode_t *pTree, int key,
        binTreeNode_t **pParent, int *pLeftChild);


/*
 * find node with 'key'
 */
binTreeNode_t* getTreeNode(binTreeNode_t *pTree, int key);


/*
 * Display the tree using preorder traversal.
 */
void printTree(binTreeNode_t *ppTree);


#endif
