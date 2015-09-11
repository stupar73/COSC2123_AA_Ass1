/*******************************************************************************
 * COSC2123 - Algorithms and Analysis
 * Semester 2 2015 Assignment #1
 * Full Name        : Stuart Parker
 * Student Number   : s3390317
 * Course Code      : COSC2123 - Algorithms and Analysis
 * Program Code     : BP094 - Bachelor of Computer Science
 * Skeleton code provided by Jeffrey Chan
 *
 * memoryUtil - Memory allocation/de-allocation functions
 ******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memoryUtil.h"


/*
 * Amount of memory currently dynamically allocated.
 */
static size_t sg_allocatedMemory;


size_t pollMemoryAllocated()
{
    return sg_allocatedMemory;
} /* end of dynMemoryAllocated() */


void memoryFreed(size_t freedMemory)
{
    if (sg_allocatedMemory <= freedMemory)
    {
        
        sg_allocatedMemory = 0;
    }
    else
    {
        sg_allocatedMemory -= freedMemory;
    }
} /* end of memoryFreed() */


void *safeMalloc(size_t size)
{
    void *pMem = NULL;
    if ((pMem = malloc(size)) == NULL)
    {
        fprintf(stderr, "Error: safeMalloc() cannot allocate "
            "memory of size %lu.\n", size);
        exit(EXIT_FAILURE);
    }

    sg_allocatedMemory += size;

    return pMem;
} /* end of safeMalloc() */


void *safeCalloc(size_t nmemb, size_t size)
{
    void *pMem = NULL;
    size_t totalSize = nmemb * size;

    if((pMem = calloc(nmemb, size)) == NULL)
    {
        fprintf(stderr, "Error: safeCalloc() cannot allocate "
            "memory of size %lu.\n", size);
        exit(EXIT_FAILURE);
    }

    memset(pMem, 0, totalSize);

    sg_allocatedMemory += totalSize;

    return pMem;
} /* end of safeCalloc() */


void *safeRealloc(void *pMem, size_t newSize, size_t extraMem)
{
    if ((pMem = realloc(pMem, newSize)) == NULL)
    {
        fprintf(stderr, "Error: safeRealloc() cannot allocate "
            "new memory of size %lu.\n", newSize);
        exit(EXIT_FAILURE);
    }

    sg_allocatedMemory += extraMem;

    return pMem;
} /* end of safeRealloc() */

void *safeReallocZero(void *pMem, size_t newSize, size_t extraMem)
{
    if ((pMem = realloc(pMem, newSize)) == NULL)
    {
        fprintf(stderr, "Error: safeReallocZero() cannot allocate "
            "new memory of size %lu.\n", newSize);
        exit(EXIT_FAILURE);
    }

    sg_allocatedMemory += extraMem;

    /* Start of new memory block */
    void *pStart = pMem + newSize - extraMem;
    /* Initialise new memory to all bits zero */
    memset(pStart, 0, extraMem);

    return pMem;
} /* end of safeReallocZero() */


void safeFree(void *pMem, size_t memSize)
{
    free(pMem);
    memoryFreed(memSize);
} /* end of safeFree() */
