/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
#include <tinyara/logm.h>
#include <stdio.h>
#include "logm.h"

/* This will be moved to upper layer or changed for protected build  */
/* for setparam types, refer logm_param_type_e  */
int logm_set(enum logm_param_type_e type, int value)
{
	switch (type) {
	case LOGM_BUFSIZE:
		/* Buffer size should be adjusted to multiples of 4 */
		logm_bufsize = (value + 3) & (~0x3);
		break;
	case LOGM_INTERVAL:
		logm_print_interval = value * 1000;
		break;
	case LOGM_NEW_BUFSIZE:
		/* Buffer size should be adjusted to multiples of 4 */
		new_logm_bufsize = (value + 3) & (~0x3);
		break;
	default:
		break;
	}

	return 0;					// for now, to keep compiler happy
}
