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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commonDefs.h"
#include "linkedList.h"
#include "memoryUtil.h"


linkedList_t* createList(size_t elemSize, void (*freeFunc)(void*),
                         int (*cmpFunc)(void*, void*))
{
    linkedList_t *pNewList = safeMalloc(sizeof *pNewList);
    
    pNewList->head = NULL;
    pNewList->length = 0;
    pNewList->elemSize = elemSize;
    pNewList->freeFunc = freeFunc;
    pNewList->cmpFunc = cmpFunc;

    return pNewList;
} /* end of createList() */


void destroyList(linkedList_t *pList)
{
    if (pList)
    {
        llNode_t *pCurrNode = pList->head;
        llNode_t *pNextNode;

        while (pCurrNode != NULL)
        {
            pNextNode = pCurrNode->next;
            
            if (pList->freeFunc)
            {
                pList->freeFunc(pCurrNode->data);
            }
            else
            {
                safeFree(pCurrNode->data, pList->elemSize);
            }

            safeFree(pCurrNode, sizeof *pCurrNode);
            
            pCurrNode = pNextNode;
        }

        pList->head = NULL;
        pList->length = 0;
        pList->elemSize = 0;
        pList->freeFunc = NULL;
        safeFree(pList, sizeof *pList);
        pList = NULL;
    }
} /* end of destroyList() */


llNode_t* insertListNodeHead(linkedList_t *pList, void *data, int needToMallocData)
{
    llNode_t *pNewNode = safeMalloc(sizeof *pNewNode);
    pNewNode->next = pList->head;

    if (data != NULL)
    {
        if (needToMallocData)
        {
            /* Non-malloc'd data passed in (e.g. int), must malloc here */
            pNewNode->data = safeMalloc(pList->elemSize);
            memmove(pNewNode->data, data, pList->elemSize);
        }
        else
        {
            pNewNode->data = data;
        }
    }
    else
    {
        pNewNode->data = NULL;
    }

    pList->head = pNewNode;
    pList->length += 1;

    return pNewNode;
}


llNode_t* insertListNodeTail(linkedList_t *pList, void *data, int needToMallocData)
{
    llNode_t *pCurrNode = pList->head;
    llNode_t *pNewNode = safeMalloc(sizeof *pNewNode);
    pNewNode->next = NULL;

    if (data != NULL)
    {
        if (needToMallocData)
        {
            /* Non-malloc'd data passed in (e.g. int), must malloc here */
            pNewNode->data = safeMalloc(pList->elemSize);
            memmove(pNewNode->data, data, pList->elemSize);
        }
        else
        {
            pNewNode->data = data;
        }
    }
    else
    {
        pNewNode->data = NULL;
    }
    
    /* Insert new node at end of list */
    if (pCurrNode == NULL)
    {
        /* List is empty, new node is head */
        pList->head = pNewNode;
    }
    else
    {
        while (pCurrNode->next != NULL)
        {
            pCurrNode = pCurrNode->next;
        }
        pCurrNode->next = pNewNode;
    }

    
    pList->length += 1;

    return pNewNode;
} /* end of insertListNodeTail() */


int deleteListNode(linkedList_t *pList, void *target)
{
    llNode_t *pCurrNode = pList->head;
    llNode_t *pPrevNode = NULL;

    if (pCurrNode != NULL)
    {
        /* Check if head is the target node */
        if (pList->cmpFunc(pCurrNode->data, target) == 0)
        {
            pList->head = pCurrNode->next;

            safeFree(pCurrNode->data, sizeof *pCurrNode);
            safeFree(pCurrNode, sizeof *pCurrNode);

            pList->length -= 1;
            return SUCCESS;
        }

        pPrevNode = pCurrNode;
        pCurrNode = pCurrNode->next;
    }

    while (pCurrNode != NULL)
    {
        if (pList->cmpFunc(pCurrNode->data, target) == 0)
        {
            pPrevNode->next = pCurrNode->next;
            
            safeFree(pCurrNode->data, sizeof *pCurrNode);
            safeFree(pCurrNode, sizeof *pCurrNode);
            
            pList->length -= 1;
            return SUCCESS;
        }
        pPrevNode = pCurrNode;
        pCurrNode = pCurrNode->next;
    }

    /* Can't find node */
    return FAILED;
} /* end of deleteListNode() */


int findListElement(linkedList_t *pList, int elem)
{
    llNode_t *pCurrNode = pList->head;

    while (pCurrNode != NULL)
    {
        if (*(int *)pCurrNode->data == elem)
        {
            return FOUND;
        }
        pCurrNode = pCurrNode->next;
    }

    /* Can't find node */
    return NOT_FOUND;
} /* end of findListElement() */


llNode_t* getListNode(linkedList_t *pList, int n)
{
    llNode_t *pCurrNode = pList->head;
    int i;

    for (i = 0; i < n; ++i)
    {
        pCurrNode = pCurrNode->next;
    }

    return pCurrNode;
} /* end of getListNode() */


void printList(linkedList_t *pList)
{
    llNode_t *pCurrNode = pList->head;

    while (pCurrNode != NULL)
    {
        printf("%d ", *(int *)pCurrNode->data);
        pCurrNode = pCurrNode->next;
    }
    printf("\n");

} /* end of printList() */


void freeList(void *p)
{
    destroyList((linkedList_t *)p);
} /* end of freeList() */


int cmpInt(void *p1, void *p2)
{
    int n1 = *(int *)p1;
    int n2 = *(int *)p2;

    return n1 - n2;
} /* end of cmpInt() */


int cmpList(void *p1, void *p2)
{
    linkedList_t *l1 = (linkedList_t *)p1;
    linkedList_t *l2 = (linkedList_t *)p2;

    if (l1 == l2)
    {
        return 0;
    }
    else /* l1 != l2 */
    {
        return -1;
    }
} /* end of cmpList() */
