/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * bpGraphAdjMat_AA - ARRAY-ARRAY implementation of the adjacency matrix
 *                    representation
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "commonDefs.h"
#include "memoryUtil.h"
#include "bipartiteGraph.h"


struct implBipartGraph_t /* bpGraph_t */
{
    /* High vertex number in partites */
    int vertNumP1;
    int vertNumP2;

    /* Dynamically allocated array of char arrays of neighbours for partites */
    char **vpVertsP1;
    char **vpVertsP2;

    /* Dynamically allocated array of chars to indicate if vertex exists */
    char *vVertExistP1;
    char *vVertExistP2;
};


static void deleteVertEdges(bpGraph_t *pGraph, int vertID, int partite);
static void printNeighbours(char *neighbours, int srcVertID, int numVerts);


bpGraph_t* bipartGraphCreate(int vertNumP1, int vertNumP2)
{
    int i;

    bpGraph_t *pGraph = safeMalloc(sizeof *pGraph);
    pGraph->vertNumP1 = vertNumP1;
    pGraph->vertNumP2 = vertNumP2;

    /* Initialise array of arrays and existence array for partite 1 */
    pGraph->vpVertsP1 = safeMalloc(vertNumP1 * sizeof *pGraph->vpVertsP1);
    pGraph->vVertExistP1 = safeMalloc(vertNumP1 * sizeof *pGraph->vVertExistP1);
    /* Initialise all elements to no neighbours and true for existance */
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        pGraph->vpVertsP1[i] = safeCalloc(vertNumP2, sizeof *(pGraph->vpVertsP1[i]));
        pGraph->vVertExistP1[i] = 1;
    }

    /* Initialise array of arrays and existence array for partite 2 */
    pGraph->vpVertsP2 = safeMalloc(vertNumP2 * sizeof *pGraph->vpVertsP2);
    pGraph->vVertExistP2 = safeMalloc(vertNumP2 * sizeof *pGraph->vVertExistP2);
    /* Initialise all elements to no neighbours and true for existance */
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        pGraph->vpVertsP2[i] = safeCalloc(vertNumP1, sizeof *(pGraph->vpVertsP2[i]));
        pGraph->vVertExistP2[i] = 1;
    }

    return pGraph;
} /* end of bipartGraphDestroy() */


void bipartGraphDestroy(bpGraph_t* pGraph)
{
    int i;

    /* Free the existence arrays */
    safeFree(pGraph->vVertExistP1,
             pGraph->vertNumP1 * sizeof *pGraph->vVertExistP1);
    safeFree(pGraph->vVertExistP2,
             pGraph->vertNumP2 * sizeof *pGraph->vVertExistP2);

    /* Free the edge arrays */
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        safeFree(pGraph->vpVertsP1[i],
                 pGraph->vertNumP2 * sizeof *pGraph->vpVertsP1[i]);
    }
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        safeFree(pGraph->vpVertsP2[i],
                 pGraph->vertNumP1 * sizeof *pGraph->vpVertsP2[i]);
    }

    /* Free the outer arrays */
    safeFree(pGraph->vpVertsP1,
             pGraph->vertNumP1 * sizeof *pGraph->vpVertsP1);
    safeFree(pGraph->vpVertsP2,
             pGraph->vertNumP2 * sizeof *pGraph->vpVertsP2);

    /* Free the graph itself */
    safeFree(pGraph, sizeof *pGraph);

    /* Make sure we can't accidentally use graph */
    pGraph = NULL;
} /* end of bipartGraphDestroy() */


int bipartGraphInsertVertex(bpGraph_t* pGraph, int vertID, int partite)
{
    int i;

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
                pGraph->vpVertsP1[vertID] = safeCalloc(pGraph->vertNumP2,
                                            sizeof *pGraph->vpVertsP1[vertID]);
                pGraph->vVertExistP1[vertID] = 1;
                return NEW_VERTEX;
            }
        }
        else
        {
            /* Must be a new vertex, so we need to allocate more space */
            char **pTemp = safeRealloc(pGraph->vpVertsP1,
                    (vertID+1) * sizeof *pGraph->vpVertsP1,
                    (vertID+1 - pGraph->vertNumP1) * sizeof *pGraph->vpVertsP1);
            pGraph->vpVertsP1 = pTemp;
            char *pTemp2 = safeRealloc(pGraph->vVertExistP1,
                    (vertID+1) * sizeof *pGraph->vVertExistP1,
                    (vertID+1 - pGraph->vertNumP1) * sizeof *pGraph->vVertExistP1);
            pGraph->vVertExistP1 = pTemp2;
            /* Initialise all new elements to NULL */
            for (i = pGraph->vertNumP1; i < vertID; ++i)
            {
                pGraph->vpVertsP1[i] = NULL;
                pGraph->vVertExistP1[i] = 0;
            }
            pGraph->vpVertsP1[vertID] = safeCalloc(pGraph->vertNumP2,
                                            sizeof *pGraph->vpVertsP1[vertID]);
            pGraph->vVertExistP1[vertID] = 1;

            /* Need to allocate more space for edge lists in other patite */
            for (i = 0; i < pGraph->vertNumP2; ++i)
            {
                if (pGraph->vVertExistP2[i])
                {
                    char *pTemp3 = safeReallocZero(pGraph->vpVertsP2[i],
                            (vertID+1) * sizeof *pGraph->vpVertsP2[i],
                            (vertID+1 - pGraph->vertNumP1) * sizeof *pGraph->vpVertsP2[i]);
                    pGraph->vpVertsP2[i] = pTemp3;
                }
            }

            pGraph->vertNumP1 = vertID+1;
            return NEW_VERTEX;
        }
    }
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
                pGraph->vpVertsP2[vertID] = safeCalloc(pGraph->vertNumP1,
                                            sizeof *pGraph->vpVertsP2[vertID]);
                pGraph->vVertExistP2[vertID] = 1;
                return NEW_VERTEX;
            }
        }
        else
        {
            /* Must be a new vertex, so we need to allocate more space */
            char **pTemp = safeRealloc(pGraph->vpVertsP2,
                    (vertID+1) * sizeof *pGraph->vpVertsP2,
                    (vertID+1 - pGraph->vertNumP2) * sizeof *pGraph->vpVertsP2);
            pGraph->vpVertsP2 = pTemp;
            char *pTemp2 = safeRealloc(pGraph->vVertExistP2,
                    (vertID+1) * sizeof *pGraph->vVertExistP2,
                    (vertID+1 - pGraph->vertNumP2) * sizeof *pGraph->vVertExistP2);
            pGraph->vVertExistP2 = pTemp2;
            /* Initialise all new elements to NULL */
            for (i = pGraph->vertNumP2; i < vertID; ++i)
            {
                pGraph->vpVertsP2[i] = NULL;
                pGraph->vVertExistP2[i] = 0;
            }
            /* Create edge array for new vertex */
            pGraph->vpVertsP2[vertID] = safeCalloc(pGraph->vertNumP1,
                                            sizeof *pGraph->vpVertsP2[vertID]);
            pGraph->vVertExistP2[vertID] = 1;

            /* Need to allocate more space for edge lists in other patite */
            for (i = 0; i < pGraph->vertNumP1; ++i)
            {
                if (pGraph->vVertExistP1[i])
                {
                    char *pTemp3 = safeReallocZero(pGraph->vpVertsP1[i],
                            (vertID+1) * sizeof *pGraph->vpVertsP1[i],
                            (vertID+1 - pGraph->vertNumP2) * sizeof *pGraph->vpVertsP1[i]);
                    pGraph->vpVertsP1[i] = pTemp3;
                }
            }

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
        if (pGraph->vpVertsP1[srcVertID][tarVertID])
        {
            return EXISTING_EDGE;
        }

        /* Else must be new edge */
        pGraph->vpVertsP1[srcVertID][tarVertID] = 1;
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
        if (pGraph->vpVertsP2[srcVertID][tarVertID])
        {
            return EXISTING_EDGE;
        }
        
        /* Else must be new edge */
        pGraph->vpVertsP2[srcVertID][tarVertID] = 1;
        return NEW_EDGE;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /* end of bipartGraphInsertEdge() */


int bipartGraphDeleteVertex(bpGraph_t* pGraph, int vertID, int partite)
{
    if (partite == 1)
    {
        /* Check if vertex exists */
        if (pGraph->vVertExistP1[vertID])
        {
            /* Delete neighbours */
            safeFree(pGraph->vpVertsP1[vertID], sizeof *pGraph->vpVertsP1[vertID]);
            pGraph->vpVertsP1[vertID] = NULL;
            pGraph->vVertExistP1[vertID] = 0;

            /* Search for edges to this vertex from the other partite and delete them */
            deleteVertEdges(pGraph, vertID, 2);

            return FOUND;
        }

        /* Vertex not in graph */
        return NOT_FOUND;
    }
    else if (partite == 2)
    {
        /* Check if vertex exists */
        if (pGraph->vVertExistP2[vertID])
        {
            /* Delete neighbours */
            safeFree(pGraph->vpVertsP2[vertID], sizeof *pGraph->vpVertsP2[vertID]);
            pGraph->vpVertsP2[vertID] = NULL;
            pGraph->vVertExistP2[vertID] = 0;

            /* Search for edges to this vertex from the other partite and delete them */
            deleteVertEdges(pGraph, vertID, 1);

            return FOUND;
        }

        /* Vertex not in graph */
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
        /* The requested addition should be within the vertices id limits. */
        if (srcVertID >= pGraph->vertNumP1
            || tarVertID >= pGraph->vertNumP2
            || !pGraph->vVertExistP1[srcVertID]
            || !pGraph->vVertExistP2[tarVertID])
        {
            return ERROR_VALUE;
        }

        if(pGraph->vpVertsP1[srcVertID][tarVertID])
        {
            pGraph->vpVertsP1[srcVertID][tarVertID] = 0;
            return FOUND;
        }
        return NOT_FOUND;
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

        if(pGraph->vpVertsP2[srcVertID][tarVertID])
        {
            pGraph->vpVertsP2[srcVertID][tarVertID] = 0;
            return FOUND;
        }
        return NOT_FOUND;
    }

    /* Invalid partite */
    return ERROR_VALUE;
} /** end of bipartGraphDeleteEdge() */


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
        return pGraph->vpVertsP1[srcVertID][tarVertID];
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
        return pGraph->vpVertsP2[srcVertID][tarVertID];
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

    /* Print edges */
    printf("Edges:\n");
    /* Partite 1 to partite 2 edges. */
    printf("Partite 1 to 2:\n");
    for (i = 0; i < pGraph->vertNumP1; ++i)
    {
        if (pGraph->vVertExistP1[i])
        {
            printNeighbours(pGraph->vpVertsP1[i], i, pGraph->vertNumP2);
        }
    }

    /* Partite 2 to partite 1 edges. */
    printf("Partite 2 to 1:\n");
    for (i = 0; i < pGraph->vertNumP2; ++i)
    {
        if (pGraph->vVertExistP2[i])
        {
            printNeighbours(pGraph->vpVertsP2[i], i, pGraph->vertNumP1);
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
                pGraph->vpVertsP1[i][vertID] = 0;
            }
        }
    }
    else if (partite == 2)
    {
        for (i = 0; i < pGraph->vertNumP2; ++i)
        {
            if (pGraph->vVertExistP2[i])
            {
                pGraph->vpVertsP2[i][vertID] = 0;
            }
        }
    }
} /* end of deleteVertEdges() */


static void printNeighbours(char *neighbours, int srcVertID, int numVerts)
{
    int i;

    for (i = 0; i < numVerts; ++i)
    {
        if(neighbours[i])
        {
            printf("%d %d\n", srcVertID, i);
        }
    }
} /* end of printNeighbours() */
