#pragma once

/**
 * @file things_common.h
 * @brief THINGS constant variable header (error, bool, etc...)
 */


/**
 * @defgroup common common
 * @brief <b> THINGS constant variable header (error, bool, etc...) </b>
 *
 * @{
 */


/**
 * @brief boolean constant
 *
 */
typedef enum {
	THINGS_FALSE = 0,  /**<  bool FALSE constant  */
	THINGS_TRUE = 1    /**<  bool TRUE constant   */
} things_bool;


/**
 * @brief time out option (used by message queue, uart, semaphore, mutex)
 *
 */
typedef enum {
	THINGS_NO_WAIT = 0,   /**<  no wait contant          */
	THINGS_FOREVER = -1,  /**<  wait until job finished  */
} things_timeout_option;



/**
 * @brief function result constant
 *
 */
typedef enum {
	THINGS_FAIL = -1,      	  /**<  fail 						  */
	THINGS_SUCCESS = 0,    	  /**<  success  					  */
	THINGS_INVALID_ARGS = 1,   /**<  invalid parameter (argument)  */
	THINGS_TIMEOUT = 2,    	  /**<  occur time out  			  */
	THINGS_RESOURCE_BUSY = 3,  /**<  resource using another task   */
} things_result;


#define __FILENAME__ (strrchr(__FILE__, '/') ? (char*)(strrchr(__FILE__, '/') + 1) : __FILE__)
#define FILE_NAME   __FILENAME__ /** @def source file name macro for debug log */
#define LINE_NUM	__LINE__	 /** @def source file line number macro for debug log */
#define FUNC_NAME	__func__	 /** @def function name macro for debug log */

/**
 @}
 */ // end log group
