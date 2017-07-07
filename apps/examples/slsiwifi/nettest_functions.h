
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

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

int8_t doAutoTest(char *filename);
int8_t doSanityTest(int iterations);
int8_t doNightlyTest(int iterations);
int slsi_tcp_server(void);
int slsi_tcp_client(void);
int slsi_udp_server(void);
int slsi_udp_client(void);
