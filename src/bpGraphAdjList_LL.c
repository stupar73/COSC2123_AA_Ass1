/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * bpGraphAdjList_LL - LINKED LIST-LINKED LIST implementation of the adjacency
 *                     list representation
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "commonDefs.h"
#include "memoryUtil.h"
#include "bipartiteGraph.h"
#include "linkedList.h"

/*
 * Implementation of the bipartite graph type for this data structure.
 */
struct implBipartGraph_t /* bpGraph_t */
{
    /* High vertex number in partites */
    int vertNumP1;
    int vertNumP2;

    /* Linked list of linked lists of neighbours for partites */
    linkedList_t *vpVertsP1;
    linkedList_t *vpVertsP2;

    /* Dynamically allocated array of chars to indicate if vertex exists */
    char *vVertExistP1;
    char *vVertExistP2;
};


static void deleteVertEdges(bpGraph_t *pGraph, int vertID, int partite);
static void printNeighbours(linkedList_t *list, int srcVertID);


bpGraph_t* bipartGraphCreate(int vertNumP1, int vertNumP2)
{
    int i;
    
    bpGraph_t *pGraph = safeMalloc(sizeof *pGraph);
    pGraph->vertNumP1 = vertNumP1;
    pGraph->vertNumP2 = vertNumP2;
    
    /* Initialise linked list of linked lists and existence array for partite 1 */
    pGraph->vpVertsP1 = createList(sizeof *(pGraph->vpVertsP1), freeList, cmpList);
    pGraph->vVertExistP1 = safeMalloc(vertNumP1 * sizeof *pGraph->vVertExistP1);
    /* Create nodes for elements and set neighbours to empty list and existance
     * to true */
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        insertListNodeTail(pGraph->vpVertsP1, createList(sizeof(int), NULL, cmpInt), 0);
        pGraph->vVertExistP1[i] = 1;
    }

    /* Initialise linked list of linked lists and existence array for partite 2 */
    pGraph->vpVertsP2 = createList(sizeof *(pGraph->vpVertsP2), freeList, cmpList);
    pGraph->vVertExistP2 = safeMalloc(vertNumP2 * sizeof *pGraph->vVertExistP2);
    /* Create nodes for elements and set neighbours to empty list and existance
     * to true */
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        insertListNodeTail(pGraph->vpVertsP2, createList(sizeof(int), NULL, cmpInt), 0);
        pGraph->vVertExistP2[i] = 1;
    }

    return pGraph;
} /* End of bipartGraphCreate() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
    /* Free the existence arrays */
    safeFree(pGraph->vVertExistP1,
             (sizeof *(pGraph->vVertExistP1)) * pGraph->vertNumP1);
    safeFree(pGraph->vVertExistP2,
             (sizeof *(pGraph->vVertExistP1)) * pGraph->vertNumP2);
    
    /* Recursively free the linked lists */
    destroyList(pGraph->vpVertsP1);
    destroyList(pGraph->vpVertsP2);

    /* Free the graph itself */
    safeFree(pGraph, sizeof *pGraph);

    /* Make sure we can't accidentally use graph */
    pGraph = NULL;
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertID, int partite)
{
    int i;

    /* Insert into partite 1 */
    if (partite == 1)
    {
        /* Check if 'vertID' is an existing vertex */
        if (vertID < pGraph->vertNumP1)
        {
            if (pGraph->vVertExistP1[vertID])
            {
                /* Existing vertex with existing neighbours list */
                return EXISTING_VERTEX;
            }
            else
            {
                /* Existing vertex without neighbours list */
                getListNode(pGraph->vpVertsP1, vertID)->data = createList(sizeof(int), NULL, cmpInt);
                pGraph->vVertExistP1[vertID] = 1;
                return NEW_VERTEX;
            }
        }
        else
        {
            char *temp = safeRealloc(pGraph->vVertExistP1,
                    (vertID+1) * sizeof *pGraph->vVertExistP1,
                    (vertID+1-pGraph->vertNumP1) * sizeof *pGraph->vVertExistP1);
            pGraph->vVertExistP1 = temp;
            /* New vertex, add nodes to the outer linked list until we have
             * one less than 'vertID' nodes */
            for (i = pGraph->vertNumP1; i < vertID; ++i)
            {
                /* Initialise nodes to NULL */
                insertListNodeTail(pGraph->vpVertsP1, NULL, 0);
                pGraph->vVertExistP1[i] = 0;
            }
            /* Add final node 'vertID' nodes deep, create empty list */
            insertListNodeTail(pGraph->vpVertsP1, createList(sizeof(int), NULL, cmpInt), 0);
            pGraph->vVertExistP1[vertID] = 1;

            pGraph->vertNumP1 = vertID + 1;
            return NEW_VERTEX;
        }
    }
    /* Insert into partite 2 */
    else if (partite == 2)
    {
        /* Check if 'vertID' is an existing vertex */
        if (vertID < pGraph->vertNumP2)
        {
            if (pGraph->vVertExistP2[vertID])
            {
                /* Existing vertex with existing neighbours list */
                return EXISTING_VERTEX;
            }
            else
            {
                /* Existing vertex without neighbours list */
                getListNode(pGraph->vpVertsP2, vertID)->data = createList(sizeof(int), NULL, cmpInt);
                pGraph->vVertExistP2[vertID] = 1;
                return NEW_VERTEX;
            }
        }
        else
        {
            char *temp = safeRealloc(pGraph->vVertExistP2,
                    (vertID+1) * sizeof *pGraph->vVertExistP2,
                    (vertID+1-pGraph->vertNumP2) * sizeof *pGraph->vVertExistP2);
            pGraph->vVertExistP2 = temp;
            /* New vertex, add nodes to the outer linked list until we have
             * one less than 'vertID' nodes */
            for (i = pGraph->vertNumP2; i < vertID; ++i)
            {
                /* Initialise nodes to NULL */
                insertListNodeTail(pGraph->vpVertsP2, NULL, 0);
                pGraph->vVertExistP2[i] = 0;
            }
            /* Add final node 'vertID' nodes deep, create empty list */
            insertListNodeTail(pGraph->vpVertsP2, createList(sizeof(int), NULL, cmpInt), 0);
            pGraph->vVertExistP2[vertID] = 1;

            pGraph->vertNumP2 = vertID + 1;
            return NEW_VERTEX;
        }
    }

    /* Error, unknown partite! */
    return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertID, int tarVertID,
        int srcPartite)
{
    linkedList_t *srcVertNeighboursList;

    if (srcPartite == 1)
    {
        /* The requested addition should be within the vertices id limits. */
        if (srcVertID >= pGraph->vertNumP1
            || tarVertID >= pGraph->vertNumP2
            || !pGraph->vVertExistP1[srcVertID]
            || !pGraph->vVertExistP2[tarVertID])
        {
            return ERROR_VALUE;
        }

        /* Need to check for duplicates */
        srcVertNeighboursList = getListNode(pGraph->vpVertsP1, srcVertID)->data;
        if (findListElement(srcVertNeighboursList, tarVertID))
        {
            return EXISTING_EDGE;
        }

        /* Else must be new edge */
        insertListNodeHead(srcVertNeighboursList, &tarVertID, 1);
        return NEW_EDGE;
    }
    else if (srcPartite == 2)
    {
        /* The requested addition should be within the vertices id limits. */
        if (srcVertID >= pGraph->vertNumP2
            || tarVertID >= pGraph->vertNumP1
            || !pGraph->vVertExistP2[srcVertID]
            || !pGraph->vVertExistP1[tarVertID])
        {
            return ERROR_VALUE;
        }

        /* Need to check for duplicates */
        srcVertNeighboursList = getListNode(pGraph->vpVertsP2, srcVertID)->data;
        if (findListElement(srcVertNeighboursList, tarVertID))
        {
            return EXISTING_EDGE;
        }

        /* Else must be new edge */
        insertListNodeHead(srcVertNeighboursList, &tarVertID, 1);
        return NEW_EDGE;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertID, int partite)
{
    llNode_t *vertNeighboursListNode;

    if (partite == 1)
    {
        /* Check if vertex exists */
        if (pGraph->vVertExistP1[vertID])
        {
            vertNeighboursListNode = getListNode(pGraph->vpVertsP1, vertID);
            /* Delete neighbours */
            destroyList(vertNeighboursListNode->data);
            vertNeighboursListNode->data = NULL;
            pGraph->vVertExistP1[vertID] = 0;

            /* Search for edges to this vertex from the other partite and delete them */
            deleteVertEdges(pGraph, vertID, 2);

            return FOUND;
        }

        /* Vertex not found */
        return NOT_FOUND;
    }
    else if (partite == 2)
    {
        /* Check if vertex exists */
        if (pGraph->vVertExistP2[vertID])
        {
            vertNeighboursListNode = getListNode(pGraph->vpVertsP2, vertID);
            /* Delete neighbours */
            destroyList(vertNeighboursListNode->data);
            vertNeighboursListNode->data = NULL;
            pGraph->vVertExistP2[vertID] = 0;

            /* Search for edges to this vertex from the other partite and delete them */
            deleteVertEdges(pGraph, vertID, 1);

            return FOUND;
        }

        /* Vertex not found */
        return NOT_FOUND;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph,  int srcVertID, int tarVertID,
        int srcPartite)
{
    if (srcPartite == 1)
    {
        /* The requested deletion should be within the vertices ID limits */
        if (srcVertID >= pGraph->vertNumP1
            || tarVertID >= pGraph->vertNumP2
            || !pGraph->vVertExistP1[srcVertID]
            || !pGraph->vVertExistP2[tarVertID])
        {
            return ERROR_VALUE;
        }

        /* Attempt to delete 'tarVertID' node in 'srcVertID' linked list */
        if (!deleteListNode(getListNode(pGraph->vpVertsP1, srcVertID)->data, &tarVertID))
        {
            return NOT_FOUND;
        }
        return FOUND;
    }
    else if (srcPartite == 2)
    {
        /* The requested deletion should be within the vertices ID limits */
        if (srcVertID >= pGraph->vertNumP2
            || tarVertID >= pGraph->vertNumP1
            || !pGraph->vVertExistP2[srcVertID]
            || !pGraph->vVertExistP1[tarVertID])
        {
            return ERROR_VALUE;
        }

        /* Attempt to delete 'tarVertID' node in 'srcVertID' linked list */
        if (!deleteListNode(getListNode(pGraph->vpVertsP2, srcVertID)->data, &tarVertID))
        {
            return NOT_FOUND;
        }
        return FOUND;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphDeleteEdge() */


int bipartGraphFindVertex(bpGraph_t *pGraph, int vertID, int partite)
{
    if (partite == 1)
    {
        if (vertID >= pGraph->vertNumP1 || !pGraph->vVertExistP1[vertID])
        {
            return NOT_FOUND;
        }
        return FOUND;
    }
    else if (partite == 2)
    {
        if (vertID >= pGraph->vertNumP2 || !pGraph->vVertExistP2[vertID])
        {
            return NOT_FOUND;
        }
        return FOUND;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertID, int tarVertID,
        int srcPartite)
{
    if (srcPartite == 1)
    {
        if (srcVertID >= pGraph->vertNumP1
           || tarVertID >= pGraph->vertNumP2
           || !pGraph->vVertExistP1[srcVertID]
           || !pGraph->vVertExistP2[tarVertID])
        {
            return NOT_FOUND;
        }

        /* search for edge */
        return findListElement(getListNode(pGraph->vpVertsP1, srcVertID)->data, tarVertID);
    }
    else if (srcPartite == 2)
    {
        if (srcVertID >= pGraph->vertNumP2
           || tarVertID >= pGraph->vertNumP1
           || !pGraph->vVertExistP2[srcVertID]
           || !pGraph->vVertExistP1[tarVertID])
        {
            return NOT_FOUND;
        }

        /* search for edge */
        return findListElement(getListNode(pGraph->vpVertsP2, srcVertID)->data, tarVertID);
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
    int i;

    /* Print vertices */
    printf("Vertices:\n");
    printf("Partite 1:\n");
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        if (pGraph->vVertExistP1[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Partite 2:\n");
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        if (pGraph->vVertExistP2[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");

    /* print edges */
    printf("Edges:\n");
    /* Partite 1 to partite 2 edges */
    printf("Partite 1 to 2:\n");
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        if (pGraph->vVertExistP1[i])
        {
            printNeighbours(getListNode(pGraph->vpVertsP1, i)->data, i);
        }
    }

    /* Partite 2 to Partite 1 edges */
    printf("Partite 2 to 1:\n");
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        if (pGraph->vVertExistP2[i])
        {
            printNeighbours(getListNode(pGraph->vpVertsP2, i)->data, i);
        }
    }
} /* end of bipartGraphPrint() */


static void deleteVertEdges(bpGraph_t *pGraph, int vertID, int partite)
{
    int i;

    if (partite == 1)
    {
        for (i = 0; i < pGraph->vertNumP1; ++i)
        {
            if (pGraph->vVertExistP1[i])
            {
                deleteListNode(getListNode(pGraph->vpVertsP1, i)->data, &vertID);
            }
        }
    }
    else if (partite == 2)
    {
        for (i = 0; i < pGraph->vertNumP2; ++i)
        {
            if (pGraph->vVertExistP2[i])
            {
                deleteListNode(getListNode(pGraph->vpVertsP2, i)->data, &vertID);
            }
        }
    }
} /* end of deleteVertEdges() */


static void printNeighbours(linkedList_t *list, int srcVertID)
{
    llNode_t *currNode = list->head;

    while (currNode != NULL)
    {
        printf("%d %d\n", srcVertID, *(int *)currNode->data);
        currNode = currNode->next;
    }
} /* end of printNeighbours() */
