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
#ifndef _COMMON_MONITOR_INTERFACE_H
#define _COMMON_MONITOR_INTERFACE_H
#include <stdint.h>
#define PROC_BUFFER_LEN 128
#define PROC_MOUNTPOINT "/proc"

int readonce(FAR const char *filename, FAR char *buffer, uint8_t buflen);
int readfile(FAR const char *filename, FAR char *buffer, uint8_t buflen);
void mount_procfs(void);
#endif

