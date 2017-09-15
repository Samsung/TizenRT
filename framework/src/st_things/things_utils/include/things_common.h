#pragma once

/**
 * @file things_common.h
 * @brief THINGS constant variable header (error, bool, etc...)
 */

/**
 * @brief function result constant
 *
 */
typedef enum {
	THINGS_FAIL = -1,		  /**<  fail 						  */
	THINGS_SUCCESS = 0,		  /**<  success  					  */
	THINGS_INVALID_ARGS = 1,   /**<  invalid parameter (argument)  */
	THINGS_TIMEOUT = 2,		  /**<  occur time out  			  */
	THINGS_RESOURCE_BUSY = 3,  /**<  resource using another task   */
} things_result;

/**
 @}
	   	 */// end log group
