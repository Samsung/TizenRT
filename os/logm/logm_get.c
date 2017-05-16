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
#include "logm.h"

/* This will be moved to upper layer or changed for protected build  */
/* Refer logm_param_type_e for getparam values */
int logm_get_values(enum logm_param_type_e type, int *value)
{
	switch (type) {
	case LOGM_BUFSIZE:
		*value = logm_bufsize;
		break;
	case LOGM_INTERVAL:
		*value = (int)(logm_print_interval / 1000);
		break;
	default:
		break;
	}

	return 0;					// for now, to keep compiler happy
}
