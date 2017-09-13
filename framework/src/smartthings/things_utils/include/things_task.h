#pragma once

#include <things_common.h>

#ifndef DOXYGEN
	#include <pthread.h>
#endif


/**
 * @file things_task.h
 * @brief task API
 */


/**
 * @defgroup task task
 * @brief <b> task API </b>
 *
 * 1. things task structure
 *	- things task prioriry
 *  - task create, destroy,
 *
 * @brief <b> task module example </b>
 * @includelineno  ex_task.c
 *
 * @{
 */

/**
 * @brief sleep task
 *
 * @param[in] milliseconds  :  sleep task (milliseconds)
 *
 * @return THINGS_SUCCESS       :  success
 * @return THINGS_FAIL          :  fail
 * @return THINGS_INVALID_ARGS  :  input parameter invalid
*/
things_result things_task_sleep( unsigned int milliseconds );


/**
 @}
 */ // end task group
