/*
 * Copyright (c) 2021 Realtek, LLC.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @addtogroup HAL
 * @{
 *
 * @brief Declares the structures and interfaces for the Hardware Abstraction Layer (HAL) module.
 *
 * @since 1.0
 * @version 1.0
 */

/**
 * @file audio_hw_types.h
 *
 * @brief Declares structures and enums for audio types.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_TYPES_H
#define AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum AudioHwFormat {
	AUDIO_HW_FORMAT_INVALID             = 0xFFFFFFFFu,
	AUDIO_HW_FORMAT_DEFAULT             = 0,
	AUDIO_HW_FORMAT_PCM                 = 0x00000000u,
	AUDIO_HW_FORMAT_MAIN_MASK           = 0xFF000000u,
	AUDIO_HW_FORMAT_SUB_MASK            = 0x00FFFFFFu,

	AUDIO_HW_FORMAT_PCM_8_BIT  = 0x1u,       /**< 8-bit PCM */
	AUDIO_HW_FORMAT_PCM_16_BIT = 0x2u,       /**< 16-bit PCM */
	AUDIO_HW_FORMAT_PCM_24_BIT = 0x3u,       /**< 24-bit PCM */
	AUDIO_HW_FORMAT_PCM_32_BIT = 0x4u,       /**< 32-bit PCM */
	AUDIO_HW_FORMAT_PCM_FLOAT  = 0x5u,       /**< float PCM */
	AUDIO_HW_FORMAT_PCM_24_BIT_PACKED = 0x6u,/**< 24-bit PCM */
	AUDIO_HW_FORMAT_PCM_8_24_BIT = 0x7u,     /**< 8-24-bit PCM */

} AudioHwFormat;

/**
 * @brief Enumerates the audio channel mask.
 *
 * A mask describes an audio channel position.
 */
typedef enum AudioHwChannelMask {
	AUDIO_HW_CHANNEL_NONE                      = 0x0u,

	AUDIO_HW_CHANNEL_FRONT_LEFT  = 0x1u,  /**< Front left channel */
	AUDIO_HW_CHANNEL_FRONT_RIGHT = 0x2u,  /**< Front right channel */
	AUDIO_HW_CHANNEL_MONO        = 0x1u, /**< Mono channel */
	AUDIO_HW_CHANNEL_STEREO      = 0x3u, /**< Stereo channel, consisting of front left and front right channels */
} AudioHwChannelMask;

enum {
	AUDIO_HW_IO_ID_INVALID = -1,
};

typedef int32_t AudioHwAdapterId;
typedef int32_t AudioIoId;

/**
 * @brief Defines the audio port direction.
 */
enum AudioHwPortDirection {
	AUDIO_HW_PORT_OUT    = 0x1u, /**< Output port */
	AUDIO_HW_PORT_IN     = 0x2u, /**< Input port */
	AUDIO_HW_PORT_OUT_IN = 0x3u, /**< Input/output port, supporting both audio input and output */
};

/**
 * @brief Defines the audio port.
 */
struct AudioHwPort {
	enum AudioHwPortDirection dir; /**< Audio port type. For details, see {@link AudioHwPortDirection} */
	uint32_t port_id;             /**< Audio port ID */
	const char *port_name;        /**< Audio port name */
};

/**
 * @brief Defines the audio adapter descriptor.
 *
 * An audio adapter is a set of port drivers for a sound card, including the output and input ports.
 * One port corresponds to multiple pins, and each pin belongs to a physical component (such as a
 * speaker).
 */
struct AudioHwAdapterDescriptor {
	const char *adapter_name; /**< Name of the audio adapter */
	uint32_t port_num;        /**< Number of ports supported by an audio adapter */
	struct AudioHwPort *ports; /**< List of ports supported by an audio adapter */
};

/**
 * @brief Enumerates the pin of an audio adapter.
 */
enum AudioHwPortPin {
	AUDIO_HW_PIN_NONE        = 0x0u,       /**< Invalid pin */
	AUDIO_HW_PIN_OUT_SPEAKER = 0x1u,       /**< Speaker output pin */
	AUDIO_HW_PIN_OUT_HEADSET = 0x2u,       /**< Wired headset pin for output */
	AUDIO_HW_PIN_OUT_LINEOUT = 0x4u,       /**< Line-out pin */
	AUDIO_HW_PIN_OUT_HDMI    = 0x8u,       /**< HDMI output pin */
	AUDIO_HW_PIN_IN_MIC      = 0x8000001u, /**< Microphone input pin */
	AUDIO_HW_PIN_IN_HS_MIC   = 0x8000002u, /**< Wired headset microphone pin for input */
	AUDIO_HW_PIN_IN_LINEIN   = 0x8000004u, /**< Line-in pin */
	AUDIO_HW_PIN_IN_DMIC_REF_AMIC = 0x8000005u, /**< dmic+ref-amic pin */
};

/**
 * @brief Defines the audio device descriptor.
 */
struct AudioHwDeviceDescriptor {
	uint32_t port_id;        /**< Audio port ID */
	enum AudioHwPortPin pins; /**< Pins of audio ports (input and output). For details, see {@link AudioHwPortPin}. */
	const char *desc;       /**< Audio device name */
};

/**
 * @brief Enumerates the audio category.
 */
enum AudioHwCategory {
	AUDIO_HW_IN_MEDIA = 0,     /**< Media */
	AUDIO_HW_IN_COMMUNICATION, /**< Communications */
	AUDIO_HW_IN_SPEECH,
	AUDIO_HW_IN_BEEP,
	AUDIO_HW_CATEGORY_MAX_NUM,
};

/**
 * @brief Defines audio sampling attributes.
 */
struct AudioHwSampleAttributes {
	enum AudioHwCategory type; /**< Audio type. For details, see {@link AudioHwCategory} */
	bool interleaved;        /**< Interleaving flag of audio data */
	AudioHwFormat format; /**< Audio data format. For details, see {@link AudioHwFormat}. */
	uint32_t sample_rate;     /**< Audio sampling rate */
	uint32_t channel_count;   /**< Number of audio channels. For example, for the mono channel, the value is 1,
                              * and for the stereo channel, the value is 2.
                              */
	uint32_t buffer_bytes;    /**< buffer_bytes per one peroid of audio track */
};

/**
 * @brief Defines the audio port capability.
 */
struct AudioHwPortCapability {
	uint32_t device_type;                     /**< Device type (output or input) */
	uint32_t device_id;                       /**< Device ID used for device binding */
	bool hardware_mode;                       /**< Whether to support device binding */
	uint32_t format_num;                      /**< Number of the supported audio formats */
	AudioHwFormat *formats;               /**< Supported audio formats. For details, see {@link AudioHwFormat}. */
	uint32_t sample_rate_masks;                /**< Supported audio sampling rates (8 kHz, 16 kHz, 32 kHz, and 48 kHz) */
	AudioHwChannelMask channel_masks;      /**< Audio channel layout mask of the device. For details, see {@link AudioHwChannelMask}.*/
	uint32_t channel_count;                   /**< Supported maximum number of audio channels */
};

/**
 * @brief Enumerates the output flags of an audio render.
 */
enum AudioHwOutputFlag {
	AUDIO_HW_OUTPUT_FLAG_NONE         = 0x0u,
	AUDIO_HW_OUTPUT_FLAG_OFFLOAD      = 0x1u,
	AUDIO_HW_OUTPUT_FLAG_NOIRQ        = 0x2u,
};

/**
 * @brief Enumerates the input flags of an audio capture.
 */
enum AudioHwInputFlag {
	AUDIO_HW_INPUT_FLAG_NONE         = 0x0u,
	AUDIO_HW_INPUT_FLAG_OFFLOAD      = 0x1u,
	AUDIO_HW_INPUT_FLAG_NOIRQ        = 0x2u,
};

/**
 * @brief Enumerates the audio hardware DMA mode.
 */
enum AudioHwMode {
	AUDIO_HW_DMA_IRQ_MODE         = 0x0u,
	AUDIO_HW_DMA_NOIRQ_MODE       = 0x1u,
};


#ifdef __cplusplus
}
#endif

#endif  // AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_TYPES_H
/** @} */
