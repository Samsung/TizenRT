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
#include <debug.h>

#define DEBUG_NONE	0x0
#define DEBUG_WARN	0x1
#define DEBUG_ERROR	0x2
#define DEBUG_VERBOSE	0x4

#define DEBUG (DEBUG_VERBOSE | DEBUG_WARN | DEBUG_ERROR)

#if (DEBUG & DEBUG_WARN)
#define MPLAYER_WARN(format, ...)	printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define MPLAYER_WARN(...)
#endif

#if (DEBUG & DEBUG_ERROR)
#define MPLAYER_ERROR(format, ...)	printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define MPLAYER_ERROR(...)
#endif

#if (DEBUG & DEBUG_VERBOSE)
#define MPLAYER_VERBOSE(format, ...)	printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define MPLAYER_VERBOSE(...)
#endif


