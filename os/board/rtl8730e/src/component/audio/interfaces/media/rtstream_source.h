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

/**
 * @addtogroup Media
 * @{
 *
 * @brief Declares APIs for RTStreamSource.
 *
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file rtstream_source.h
 *
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTSTREAM_SOURCE_H
#define AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTSTREAM_SOURCE_H

#include <stddef.h>

#include "osal_c/osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RTStreamSource RTStreamSource;

struct RTStreamSource {

	void (*SetBuffers)(const RTStreamSource *source, size_t buffer_num, size_t buffer_size);

	bool (*OnBufferAvailable)(const RTStreamSource *source, size_t index);

	void (*QueueBuffer)(const RTStreamSource *source, void *data, size_t size);
};

#ifdef __cplusplus
}
#endif

#endif // AMEBA_FWK_MEDIA_PLAYBACK_MEDIA_RTSTREAM_SOURCE_H
/** @} */
