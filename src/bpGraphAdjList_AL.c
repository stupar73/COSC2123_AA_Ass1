/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * bpGraphAdjList_AL - ARRAY-LINKED LIST implementation of the adjacency list
 *                     representation
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

    /* Dynamically allocated array of linked lists of neighbours for partites */
    linkedList_t **vpVertsP1;
    linkedList_t **vpVertsP2;

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

    /* initialise array of linked lists and existence array for partite 1 */
    pGraph->vpVertsP1 = safeMalloc(vertNumP1 * sizeof *pGraph->vpVertsP1);
    pGraph->vVertExistP1 = safeMalloc(vertNumP1 * sizeof *pGraph->vVertExistP1);
    /* initialise all elements to null for neighbours and true for existance */
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        pGraph->vpVertsP1[i] = createList(sizeof(int), NULL, cmpInt);
        pGraph->vVertExistP1[i] = 1;
    }

    /* initialise array of linked lists and existence array for partite 2 */
    pGraph->vpVertsP2 = safeMalloc(vertNumP2 * sizeof *pGraph->vpVertsP2);
    pGraph->vVertExistP2 = safeMalloc(vertNumP2 * sizeof *pGraph->vVertExistP2);
    /* initialise all elements to null for neighbours and true for existance */
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        pGraph->vpVertsP2[i] = createList(sizeof(int), NULL, cmpInt);
        pGraph->vVertExistP2[i] = 1;
    }

    return pGraph;
} /* end of bipartGraphCreate() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
    int i;

    /* free the existence arrays */
    safeFree(pGraph->vVertExistP1,
             (sizeof *(pGraph->vVertExistP1)) * pGraph->vertNumP1);
    safeFree(pGraph->vVertExistP2,
             (sizeof *(pGraph->vVertExistP2)) * pGraph->vertNumP2);

    /* free the linked lists */
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        if (pGraph->vpVertsP1[i] != NULL)
        {
            destroyList(pGraph->vpVertsP1[i]);
        }
    }
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        if (pGraph->vpVertsP2[i] != NULL)
        {
            destroyList(pGraph->vpVertsP2[i]);
        }
    }

    /* free the arrays */
    safeFree(pGraph->vpVertsP1,
             (sizeof *(pGraph->vpVertsP1)) * pGraph->vertNumP1);
    safeFree(pGraph->vpVertsP2,
             (sizeof *(pGraph->vpVertsP2)) * pGraph->vertNumP2);
    /* free the graph itself */
    safeFree(pGraph, sizeof *pGraph);
    /* make sure we can't accidentally use graph */
    pGraph = NULL;
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertID, int partite)
{
    int i;

    /* insert into partite 1 */
    if (partite == 1)
    {
        /* check if 'vertID' is an existing vertex */
        if (vertID < pGraph->vertNumP1)
        {
            if (pGraph->vVertExistP1[vertID])
            {
                return EXISTING_VERTEX;
            }
            else
            {
                pGraph->vpVertsP1[vertID] = createList(sizeof(int), NULL, cmpInt);
                pGraph->vVertExistP1[vertID] = 1;
                return NEW_VERTEX;
            }
        }
        else
        {
            /* must be a new vertex, so we need to allocate more space */
            linkedList_t **pTemp = safeRealloc(pGraph->vpVertsP1,
                    (vertID+1) * sizeof *pGraph->vpVertsP1,
                    (vertID+1 - pGraph->vertNumP1) * sizeof *pGraph->vpVertsP1);
            pGraph->vpVertsP1 = pTemp;
            char *pTemp2 = safeRealloc(pGraph->vVertExistP1,
                    (vertID+1) * sizeof *pGraph->vVertExistP1,
                    (vertID+1 - pGraph->vertNumP1) * sizeof *pGraph->vVertExistP1);
            pGraph->vVertExistP1 = pTemp2;
            /* initialise all new elements to NULL */
            for (i = pGraph->vertNumP1; i < vertID; ++i)
            {
                pGraph->vpVertsP1[i] = NULL;
                pGraph->vVertExistP1[i] = 0;
            }
            /* create edge list for new vertex */
            pGraph->vpVertsP1[vertID] = createList(sizeof(int), NULL, cmpInt);
            pGraph->vVertExistP1[vertID] = 1;

            pGraph->vertNumP1 = vertID+1;
            return NEW_VERTEX;
        }
    }
    /* insert into partite 2 */
    else if (partite == 2)
    {
        /* check if 'vertID' is an existing vertex */
        if (vertID < pGraph->vertNumP2)
        {
            if (pGraph->vVertExistP2[vertID])
            {
                return EXISTING_VERTEX;
            }
            else
            {
                pGraph->vpVertsP1[vertID] = createList(sizeof(int), NULL, cmpInt);
                pGraph->vVertExistP2[vertID] = 1;
                return NEW_VERTEX
            }
        }
        else
        {
            /* must be a new vertex, so we need to allocate more space */
            linkedList_t **pTemp = safeRealloc(pGraph->vpVertsP2,
                    (vertID+1) * sizeof *pGraph->vpVertsP2,
                    (vertID+1-pGraph->vertNumP2) * sizeof *pGraph->vpVertsP2);
            pGraph->vpVertsP2 = pTemp;
            char *pTemp2 = safeRealloc(pGraph->vVertExistP2,
                    (vertID+1) * sizeof *pGraph->vVertExistP2,
                    (vertID+1 - pGraph->vertNumP2) * sizeof *pGraph->vVertExistP2);
            pGraph->vVertExistP2 = pTemp2;
            /* initialise all new elements to NULL */
            for (i = pGraph->vertNumP2; i < vertID; ++i)
            {
                pGraph->vpVertsP2[i] = NULL;
                pGraph->vVertExistP2[i] = 0;
            }
            pGraph->vpVertsP2[vertID] = createList(sizeof(int), NULL, cmpInt);
            pGraph->vVertExistP2[vertID] = 1;

            pGraph->vertNumP2 = vertID+1;
            return NEW_VERTEX;
        }
    }

    /* Error, unknown partite! */
    return ERROR_VALUE;
} /* end of bipartGraphInsertVertex() */


int bipartGraphInsertEdge(bpGraph_t* pGraph, int srcVertID, int tarVertID,
        int srcPartite)
{
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
        if (findListElement(pGraph->vpVertsP1[srcVertID], tarVertID))
        {
            return EXISTING_EDGE;
        }

        /* Else must be new edge */
        insertListNodeHead(pGraph->vpVertsP1[srcVertID], &tarVertID, 1);
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
        if (findListElement(pGraph->vpVertsP2[srcVertID], tarVertID))
        {
            return EXISTING_EDGE;
        }

        /* Else must be new edge */
        insertListNodeHead(pGraph->vpVertsP2[srcVertID], &tarVertID, 1);
        return NEW_EDGE;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */



int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertID, int partite)
{
    if (partite == 1)
    {
        /* check if vertex exists */
        if (pGraph->vVertExistP1[vertID])
        {
            /* delete neighbours */
            destroyList(pGraph->vpVertsP1[vertID]);
            pGraph->vpVertsP1[vertID] = NULL;
            pGraph->vVertExistP1[vertID] = 0;

            /* Search for edges to this vertex from the other partite and delete them */
            deleteVertEdges(pGraph, vertID, 2);

            return FOUND;
        }

        /* vertex not in graph */
        return NOT_FOUND;
    }
    else if (partite == 2)
    {
        /* check if vertex exists */
        if (pGraph->vVertExistP2[vertID])
        {
            /* delete neighbours */
            destroyList(pGraph->vpVertsP2[vertID]);
            pGraph->vpVertsP2[vertID] = NULL;
            pGraph->vVertExistP2[vertID] = 0;

            /* search the in neighbours and delete them */
            deleteVertEdges(pGraph, vertID, 1);

            return FOUND;
        }

        return NOT_FOUND;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphDeleteVertex() */


int bipartGraphDeleteEdge(bpGraph_t* pGraph, int srcVertID, int tarVertID,
        int srcPartite)
{
    if (srcPartite == 1)
    {
        /* The requested deletion should be within the vertices id limits. */
        if (srcVertID >= pGraph->vertNumP1
            || tarVertID >= pGraph->vertNumP2
            || !pGraph->vVertExistP1[srcVertID]
            || !pGraph->vVertExistP2[tarVertID])
        {
            return ERROR_VALUE;
        }

        /* Attempt to delete linked list node */
        if (!deleteListNode(pGraph->vpVertsP1[srcVertID], &tarVertID))
        {
            return NOT_FOUND;
        }

        return FOUND;
    }
    else if (srcPartite == 2)
    {
        /* The requested deletion should be within the vertices id limits. */
        if (srcVertID >= pGraph->vertNumP2
            || tarVertID >= pGraph->vertNumP1
            || !pGraph->vVertExistP2[srcVertID]
            || !pGraph->vVertExistP1[tarVertID])
        {
            return ERROR_VALUE;
        }

        /* delete linked list node */
        if (!deleteListNode(pGraph->vpVertsP2[srcVertID], &tarVertID))
        {
            return NOT_FOUND;
        }

        return FOUND;
    }

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
    if (partite == 2)
    {
        if (vertID >= pGraph->vertNumP2 || !pGraph->vVertExistP2[vertID])
        {
            return NOT_FOUND;
        }
        return FOUND;
    }

    /* unknown partite */
    return ERROR_VALUE;
} /* end of bipartGraphFindVertex() */


int bipartGraphFindEdge(bpGraph_t* pGraph, int srcVertID, int tarVertID,
        int srcPartite)
{
    /* check if vertices exists */
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
        return findListElement(pGraph->vpVertsP1[srcVertID], tarVertID);
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
        return findListElement(pGraph->vpVertsP2[srcVertID], tarVertID);
    }

    return ERROR_VALUE;
} /* end of bipartGraphFindEdge() */


void bipartGraphPrint(bpGraph_t *pGraph)
{
    int i;

    /* print vertices */
    printf("Vertices:\n");
    printf("Part 1:\n");
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        if (pGraph->vVertExistP1[i])
        {
            printf("%d ", i);
        }
    }
    printf("\n");

    printf("Part 2:\n");
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
    /* partite 1 to partite 2 edges. */
    printf("Part 1 to 2:\n");
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        if (pGraph->vVertExistP1[i])
        {
            printNeighbours(pGraph->vpVertsP1[i], i);
        }
    }

    /* partite 2 to partite 1 edges. */
    printf("Part 2 to 1:\n");
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        if (pGraph->vVertExistP2[i])
        {
            printNeighbours(pGraph->vpVertsP2[i], i);
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
                deleteListNode(pGraph->vpVertsP1[i], &vertID);
            }
        }
    }
    else if (partite == 2)
    {
        for (i = 0; i < pGraph->vertNumP2; ++i)
        {
            if (pGraph->vVertExistP2[i])
            {
                deleteListNode(pGraph->vpVertsP2[i], &vertID);
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
