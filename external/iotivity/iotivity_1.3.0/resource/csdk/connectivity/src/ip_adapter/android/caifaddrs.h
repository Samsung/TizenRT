/* *****************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/
#include "cacommon.h"

/**
 * CA partial define for the structure ifaddrs.
 */
struct ifaddrs
{
    struct ifaddrs     *ifa_next;
    char               *ifa_name;
    unsigned int       ifa_flags;
    struct sockaddr    *ifa_addr;
};

/**
 * CA internal definition for getifaddrs.
 *
 * @param[out]  ifap    List of ifaddrs.
 * @return  On success, returns CA_STATUS_OK; on error, CA error code is returned.
 */
CAResult_t CAGetIfaddrsUsingNetlink(struct ifaddrs **ifap);

/**
 * CA internal definition for freeifaddrs
 *
 * @param[in]  ifa      List of ifaddrs.
 */
void CAFreeIfAddrs(struct ifaddrs *ifa);

