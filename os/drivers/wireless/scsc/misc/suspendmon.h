/*****************************************************************************
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

/* Baroque suspend/resume handler registration interface */

#ifndef _SUSPENDMON_H
#define _SUSPENDMON_H

#include "mxman.h"

struct suspendmon;

void suspendmon_init(struct suspendmon *suspendmon, struct scsc_mx *mx);
void suspendmon_deinit(struct suspendmon *suspendmon);

struct suspendmon {
	struct scsc_mx *mx;
};

#endif							/* _SUSPENDMON_H */
