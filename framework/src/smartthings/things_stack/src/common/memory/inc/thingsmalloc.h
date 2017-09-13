/*******************************************************************
 * Project Name: DADeviceCommercialization
 *
 * Copyright 2015 by Samsung Electronics, Inc. All rights reserved.
 *
 * Project Description: OIC Certified Refrigerator which support
 *                      Device discovery/control/monitoring
 *
 *******************************************************************/

/*******************************************************************
 *
 * @file      thingsmalloc.h
 * @brief     Describes about the Damalloc header
 *
 *******************************************************************/

#ifndef THINGS_MALLOC_H_
#define THINGS_MALLOC_H_

// The purpose of this module is to allow custom dynamic memory allocation
// code to easily be added to the TB Stack by redefining the DAMalloc and
// DAFree functions.  Examples of when this might be needed are on TB
// platforms that do not support dynamic allocation or if a memory pool
// needs to be added.
//
// Note that these functions are intended to be used ONLY within the TB
// stack and NOT by the application code.  The application should be
// responsible for its own dynamic allocation.

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>

#ifndef ENABLE_THINGS_MALLOC

#define DAMalloc(x) malloc(x)
#define DAFree(x) free(x)
#define DARealloc(x,y) realloc(x,y)
#define DACalloc(x,y) calloc(x,y)
// #ifdef __cplusplus
// extern "C"
// {
// #endif // __cplusplus
//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
#else
/**
 * Allocates a block of size bytes, returning a pointer to the beginning of
 * the allocated block.
 *
 * NOTE: This function is intended to be used internally by the TB Stack.
 *       It is not intended to be used by applications.
 *
 * @param size - Size of the memory block in bytes, where size > 0
 *
 * @return
 *     on success, a pointer to the allocated memory block
 *     on failure, a null pointer is returned
 */
extern void *DAMalloc(size_t size);

/**
 * Re-allocates a block of memory, pointed to by ptr to the size specified
 * in size.  The returned value contains a pointer to the new location, with
 * all data copied into it.  If the new size of the memory-object require movement,
 * the previous space is freed.  If the new size is larger, the newly allocated
 * area has non-deterministic content. If the space cannot be allocated, the value
 * ptr is left unchanged.
 *
 * @param ptr - Pointer to a block of memory previously allocated by DACalloc,
 *              DAMalloc, or a previous call to this function.  If this value is
 *              NULL, this function will work identically to a call to DAMalloc.
 *
 * @param size - Size of the new memory block in bytes, where size > 0
 *
 * @return
 *      on success, a pointer to the newly sized memory block
 *      on failure, a null pointer is returned, and the memory pointed to by *ptr is untouched
 */
extern void *DARealloc(void *ptr, size_t size);

/**
 * Allocates a block of memory for an array of num elements, each of them
 * size bytes long and initializes all its bits to zero.
 *
 * @param num - The number of elements
 * @param size - Size of the element type in bytes, where size > 0
 *
 * @return
 *     on success, a pointer to the allocated memory block
 *     on failure, a null pointer is returned
 */
extern void *DACalloc(size_t num, size_t size);

/**
 * Deallocate a block of memory previously allocated by a call to OCMalloc
 *
 * NOTE: This function is intended to be used internally by the TB Stack.
 *       It is not intended to be used by applications.
 *
 * @param ptr - Pointer to block of memory previously allocated by DAMalloc.
 *              If ptr is a null pointer, the function does nothing.
 */
extern void DAFree(void *ptr);

// #ifdef __cplusplus
// }
// #endif // __cplusplus

#endif /* THINGS_MALLOC_H_ */

#endif
