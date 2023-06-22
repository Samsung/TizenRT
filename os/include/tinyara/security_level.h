/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __INCLUDE_SECURITY_LEVEL_H
#define __INCLUDE_SECURITY_LEVEL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/********************************************************************************
 * Pre-processor Definitions
 ********************************************************************************/
#define HIGH_SECURITY_LEVEL     1
#define LOW_SECURITY_LEVEL      0

/********************************************************************************
 * Global Type Declarations
 ********************************************************************************/
enum security_level_result_e {
	SECURITY_LEVEL_OK,
	SECURITY_LEVEL_FAIL_READ,
	SECURITY_LEVEL_INVALID_VALUE
};

typedef enum security_level_result_e security_level_result_t;

/****************************************************************************
 * Public Types
 ****************************************************************************/

/********************************************************************************
 * Public Variables
 ********************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/
/****************************************************************************
 * Name: set_security_level
 *
 * Description:
 *   This function writes the level of security (which is saved in secure storage)
 *   in security_level.
 ****************************************************************************/
int set_security_level(void);

/****************************************************************************
 * Name: get_security_level
 *
 * Description:
 *   This function returns the security_level
 *      1 : High level of security. You cannot see some secure information.
 *      0 : Low level of security. You can see all secure information.
 ****************************************************************************/
int get_security_level(void);

#endif
