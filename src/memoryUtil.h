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


#ifndef MEMORYUTIL_H_
#define MEMORYUTIL_H_

#include <stddef.h>


/*
 * Poll the dynamic memory requested so far.
 */
size_t pollMemoryAllocated();


/*
 * Update the memory allocation count.
 */
void memoryFreed(size_t freedMemory);


/*
 * creates a new block of memory with error checking
 * returned value should be freed by the calling function
*/
void *safeMalloc(size_t size);


/*
 * creates a new block of memory (initialised to all bits zero) with error
 * checking
 * returned value should be freed by the calling function
*/
void *safeCalloc(size_t nmemb, size_t size);


/*
 * resize a pre-existing block of memory with error checking
 * returned value should be freed by the calling function
*/
void *safeRealloc(void *old_mem, size_t new_size, size_t extraMem);


/*
 * resize a pre-existing block of memory (new area initialised to all bits zero)
 * with error checking
 * returned value should be freed by the calling function
*/
void *safeReallocZero(void *pMem, size_t newSize, size_t extraMem);


/*
 * Frees the dynamically allocated memory pMem and also decrease memory
 * usage by memSize
 */
void safeFree(void *pMem, size_t memSize);


#endif /* MEMORYUTIL_H_ */
