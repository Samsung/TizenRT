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
 * @file      Damalloc.c
 * @brief     Describes about the Damalloc module
 *
 *******************************************************************/

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include "thingsmalloc.h"

//#define ENABLE_MALLOC_DEBUG
#include "thingslogger.h"

// Enable extra debug logging for malloc.  Comment out to disable
#ifdef ENABLE_MALLOC_DEBUG
#define TAG "DAMalloc"
#endif

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Internal API function
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private internal function prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public APIs
//-----------------------------------------------------------------------------
#ifdef ENABLE_MALLOC_DEBUG
static uint32_t count;
#endif

#ifdef ENABLE_THINGS_MALLOC
void *DAMalloc(size_t size)
{
    if (0 == size)
    {
        return NULL;
    }

	/* Allocating Memory for given size using malloc API
	 */
#ifdef ENABLE_MALLOC_DEBUG
    void *ptr = malloc(size);
    count++;
    THINGS_LOG_D(THINGS_DEBUG, TAG, "malloc: ptr=%p, size=%u, count=%u", ptr, size, count);
    return ptr;
#else
    return malloc(size);
#endif
}

void *DACalloc(size_t num, size_t size)
{
    if (0 == size || 0 == num)
    {
        return NULL;
    }

	/* Allocating Memory for given size using calloc API
	 */
#ifdef ENABLE_MALLOC_DEBUG
    void *ptr = calloc(num, size);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "calloc: ptr=%p, num=%u, size=%u", ptr, num, size);
    return ptr;
#else
    return calloc(num, size);
#endif
}

void *DARealloc(void* ptr, size_t size)
{
    // Override realloc() behavior for NULL pointer which normally would
    // work as per malloc(), however we suppress the behavior of possibly
    // returning a non-null unique pointer.
    if (ptr == NULL)
    {
        return DAMalloc(size);
    }

    // Otherwise leave the behavior up to realloc() itself:

#ifdef ENABLE_MALLOC_DEBUG
    void* newptr = realloc(ptr, size);
    THINGS_LOG_D(THINGS_DEBUG, TAG, "realloc: ptr=%p, newptr=%p, size=%u", ptr, newptr, size);
    // Very important to return the correct pointer here, as it only *somtimes*
    // differs and thus can be hard to notice/test:
    return newptr;
#else
    return realloc(ptr, size);
#endif
}

void DAFree(void *ptr)
{
#ifdef ENABLE_MALLOC_DEBUG
    // Since DAMalloc() did not increment count if it returned NULL,
    // guard the decrement:
    if (ptr)
    {
        count--;
    }
    THINGS_LOG_D(THINGS_DEBUG, TAG, "free: ptr=%p, count=%u", ptr, count);
#endif

    free(ptr);
    ptr = NULL;
}
#endif
