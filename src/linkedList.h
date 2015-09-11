/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * linkedList - Linked list module
 ******************************************************************************/

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

/*
 * Linked list node type.
 */
typedef struct linkedListNode llNode_t;

struct linkedListNode
{
    void *data;
    llNode_t *next;
};

/*
 * Linked list type. Only has head pointer - implement tail pointer if you like
 */
typedef struct linkedList linkedList_t;

struct linkedList
{
    llNode_t *head; /* Pointer to head of list */
    int length; /* Number of elements in list */
    size_t elemSize; /* Size of each element (node->data) in list */
    void (*freeFunc)(void *); /* Generic free function */
    int (*cmpFunc)(void *, void *); /* Generic comparison function */
};


/*
 * Create an empty list.
 */
extern linkedList_t* createList(size_t elemSize, void (*freeFunc)(void*),
                                int (*cmpFunc)(void*, void*));


/*
 * Destroy the list, assuming all memory is dynamically allocated.
 * list - pointer to list.
 */
extern void destroyList(linkedList_t *pList);


/*
 * Creates a node with value 'data' and adds the newly created node to
 * the tail of 'list', which points to the head of the list.
 * list - pointer to list.
 * data - the data you want to insert into the list.
 */
extern llNode_t* insertListNodeTail(linkedList_t *pList, void *data, int needToMallocData);


/*
 * Creates a node with value 'data' and adds the newly created node to
 * the head of 'list', which points to the head of the list.
 * list - pointer to list.
 * data - the data you want to insert into the list.
 */
extern llNode_t* insertListNodeHead(linkedList_t *pList, void *data, int needToMallocData);


/*
 * Deletes a node containing target from list 'list'.
 * list - pointer to list.
 * target - the data you want to remove from the list.
 *
 * returns - pointer to the (new) head of the list.
 */
extern int deleteListNode(linkedList_t *pList, void *target);


/*
 * Searches if elem is in the list 'list'.
 * list - pointer to list.
 * elem - the elem you want to search for in the list.
 */
extern int findListElement(linkedList_t *pList, int elem);


/*
 * Returns node n levels deep in list (n=0 --> head of list)
 * list - pointer to list.
 * n - depth in list (similar to array accessor)
 */
extern llNode_t* getListNode(linkedList_t *pList, int n);


/*
 * Print the elements in the list to stdout.
 * list - pointer to list.
 */
extern void printList(linkedList_t *pList);


/*
 * Free function for lists, simply calls destroyList
 * p - pointer to list
 */
extern void freeList(void *p);


/*
 * Integer comparison function
 * 
 * returns - negative if p1 < p2
 *           positive if p1 > p2
 *                  0 if p1 == p2
 */
extern int cmpInt(void *p1, void *p2);


/*
 * List comparison function, lists are considered equal if (and only if) their
 * pointer values are identical (i.e. they are precisely the same list, not
 * that they contain the same elements in the same order)
 * 
 * returns - negative if p1 != p2
 *                  0 if p1 == p2
 */
extern int cmpList(void *p1, void *p2);

#endif /* LINKEDLIST_H_ */
