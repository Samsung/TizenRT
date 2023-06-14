/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_AUDIO_DEBUG_H
#define AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_HAL_AUDIO_DEBUG_H

#include "basic_types.h"
#include "osdep_service.h"


/* Debug options */
#define HAL_AUDIO_COMMON_DEBUG 1
#define HAL_AUDIO_VERBOSE_DEBUG 0
#define HAL_AUDIO_PLAYBACK_VERY_VERBOSE_DEBUG 0
#define HAL_AUDIO_CAPTURE_VERY_VERBOSE_DEBUG 0

#if HAL_AUDIO_COMMON_DEBUG
#define HAL_AUDIO_DEBUG(fmt, args...)    printf("=> D/AudioHal:[%s]: " fmt "\n", __func__, ## args)
#define HAL_AUDIO_INFO(fmt, args...)     printf("=> I/AudioHal:[%s]: " fmt "\n", __FUNCTION__, ## args)
#define HAL_AUDIO_WARN(fmt, args...)     printf("=> W/AudioHal:[%s]: " fmt "\n", __FUNCTION__, ## args)
#define HAL_AUDIO_ERROR(fmt, args...)    printf("=> E/AudioHal:[%s]: " fmt "\n", __FUNCTION__, ## args)
#define HAL_AUDIO_ENTER                  printf("=> D/AudioHal:[%s]: enter\n", __FUNCTION__)
#define HAL_AUDIO_EXIT                   printf("=> D/AudioHal:[%s]: exit\n", __FUNCTION__)
#define HAL_AUDIO_EXIT_ERR               printf("=> D/AudioHal:[%s]: error: exit\n", __FUNCTION__)
#define HAL_AUDIO_TRACE                  printf("=> D/AudioHal:[%s]: line:%d\n", __FUNCTION__, __LINE__)
#else
#define HAL_AUDIO_DEBUG(fmt, args...)    do { } while(0)
#define HAL_AUDIO_INFO(fmt, args...)     do { } while(0)
#define HAL_AUDIO_WARN(fmt, args...)     do { } while(0)
#define HAL_AUDIO_ERROR(fmt, args...)    printf("=> E/AudioHal:[%s]: " fmt "\n", __FUNCTION__, ## args)
#define HAL_AUDIO_ENTER                  do { } while(0)
#define HAL_AUDIO_EXIT                   do { } while(0)
#define HAL_AUDIO_EXIT_ERR               do { } while(0)
#define HAL_AUDIO_TRACE                  do { } while(0)
#endif

#if HAL_AUDIO_COMMON_DEBUG && HAL_AUDIO_VERBOSE_DEBUG
#define HAL_AUDIO_VERBOSE(fmt, args...)  printf("=> V/AudioHal:[%s]: " fmt "\n", __func__, ##args)
#else
#define HAL_AUDIO_VERBOSE(fmt, args...)  do { } while(0)
#endif

#if HAL_AUDIO_COMMON_DEBUG && HAL_AUDIO_VERBOSE_DEBUG && HAL_AUDIO_PLAYBACK_VERY_VERBOSE_DEBUG
#define HAL_AUDIO_PVERBOSE(fmt, args...) printf("=> PV/AudioHal:[%s]: " fmt "\n", __FUNCTION__, ## args)
#else
#define HAL_AUDIO_PVERBOSE(fmt, args...) do { } while(0)
#endif

#if HAL_AUDIO_COMMON_DEBUG && HAL_AUDIO_VERBOSE_DEBUG && HAL_AUDIO_CAPTURE_VERY_VERBOSE_DEBUG
#define HAL_AUDIO_CVERBOSE(fmt, args...) printf("=> CV/AudioHal:[%s]: " fmt "\n", __FUNCTION__, ## args)
#else
#define HAL_AUDIO_CVERBOSE(fmt, args...) do { } while(0)
#endif

#endif
