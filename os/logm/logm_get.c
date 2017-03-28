/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include "logm.h"

volatile int new_logm_bufsize = 0;

/* This will be moved to upper layer or changed for protected build  */
/* Refer logm_param_type_e for getparam values */
int logm_get(enum logm_param_type_e type, void *value)
{
	switch (type) {
	case LOGM_BUFSIZE:
		*((int *)value) = logm_bufsize;
		break;
	case LOGM_INTERVAL:
		*((int *)value) = (int)(logm_print_interval / 1000);
		break;
	case LOGM_NEW_BUFSIZE:
		*((int *)value) = new_logm_bufsize;
		break;
	default:
		break;
	}

	return 0;					// for now, to keep compiler happy
}
