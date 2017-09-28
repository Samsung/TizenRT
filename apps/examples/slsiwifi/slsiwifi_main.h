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

#ifndef __SLSIWIFI_MAIN_H
#define __SLSIWIFI_MAIN_H

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <slsi_wifi/slsi_wifi_api.h>
#include "nettest_functions.h"

void sw_linkUpHandler(slsi_reason_t *reason);
void sw_linkDownHandler(slsi_reason_t *reason);

#endif
