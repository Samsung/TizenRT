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
 * @file audio_hw_adapter.h
 *
 * @brief Declares APIs for operations related to the audio adapter.
 *
 * @since 1.0
 * @version 1.0
 */

#ifndef AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_ADAPTER_H
#define AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_ADAPTER_H


#include "hardware/audio/audio_hw_render.h"
#include "hardware/audio/audio_hw_capture.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Provides audio adapter capabilities, including initializing ports, creating rendering and capturing tasks,
 * and obtaining the port capability set.
 *
 * @see AudioHwRender
 * @see AudioHwCapture
 * @since 1.0
 * @version 1.0
 */
struct AudioHwAdapter {
	int (*SetParameters)(struct AudioHwAdapter *adapter, const char *kv_pairs);

	char *(*GetParameters)(const struct AudioHwAdapter *adapter,
						   const char *keys);

	/**
	 * @brief Set volume to <b>AudioHwCapture</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param volume Indicates the volume need set to <b>AudioHwCapture</b> object.
	 * @return Returns <b>0</b> if the volume is set successfully;
	 * returns a negative value otherwise.
	 * @see GetCaptureVolume
	 */
	int (*SetCaptureVolume)(const struct AudioHwAdapter *adapter, float volume);

	/**
	 * @brief Get volume of <b>AudioHwCapture</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param volume Indicates the volume need to get from <b>AudioHwCapture</b> object.
	 * @return Returns <b>0</b> if the volume get successfully;
	 * returns a negative value otherwise.
	 * @see SetCaptureVolume
	 */
	int (*GetCaptureVolume)(const struct AudioHwAdapter *adapter, float *volume);

	/**
	 * @brief Set volume to <b>AudioHwRender</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param volume Indicates the volume need set to <b>AudioHwRender</b> object.
	 * @return Returns <b>0</b> if the volume is set successfully;
	 * returns a negative value otherwise.
	 * @see GetRenderVolume
	 */
	int (*SetRenderVolume)(const struct AudioHwAdapter *adapter, float volume);

	/**
	 * @brief Get volume of <b>AudioHwRender</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param volume Indicates the volume need to get from <b>AudioHwRender</b> object.
	 * @return Returns <b>0</b> if the volume get successfully;
	 * returns a negative value otherwise.
	 * @see SetRenderVolume
	 */
	int (*GetRenderVolume)(const struct AudioHwAdapter *adapter, float *volume);

	/**
	 * @brief Set mute status to <b>AudioHwRender</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param muted Indicates the muted status need to set to hardware.
	 * @return Returns <b>0</b> if the muted is set successfully;
	 * returns a negative value otherwise.
	 * @see GetRenderMute
	 */
	int (*SetRenderMute)(const struct AudioHwAdapter *adapter, bool muted);

	/**
	 * @brief Get current mute status to <b>AudioHwRender</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param muted Indicates the current muted status of the hardware .
	 * @return Returns <b>0</b> if the muted status is got successfully;
	 * returns a negative value otherwise.
	 * @see SetRenderMute
	 */
	int (*GetRenderMute)(const struct AudioHwAdapter *adapter, bool *muted);

	/**
	 * @brief Set mute status to <b>AudioHwCapture</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param muted Indicates the muted status need to set to hardware.
	 * @return Returns <b>0</b> if the muted is set successfully;
	 * returns a negative value otherwise.
	 * @see GetCaptureMute
	 */
	int (*SetCaptureMute)(const struct AudioHwAdapter *adapter, bool muted);

	/**
	 * @brief Get current mute status to <b>AudioHwCapture</b> object of <b>AudioHwAdapter</b> object .
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param muted Indicates the current muted status of the hardware .
	 * @return Returns <b>0</b> if the muted status is got successfully;
	 * returns a negative value otherwise.
	 * @see SetCaptureMute
	 */
	int (*GetCaptureMute)(const struct AudioHwAdapter *adapter, bool *muted);

	size_t (*GetInputBufferSize)(const struct AudioHwAdapter *adapter, const struct AudioHwSampleAttributes *attrs);

	/**
	 * @brief Creates an <b>AudioHwRender</b> object.
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param desc Indicates the pointer to the descriptor of the audio adapter to start.
	 * @param attrs Indicates the pointer to the audio sampling attributes to open.
	 * @param flags Indicates current render flags.
	 * @param render Indicates the double pointer to the <b>AudioHwRender</b> object.
	 * @return Returns <b>0</b> if the <b>AudioHwRender</b> object is created successfully;
	 * returns a negative value otherwise.
	 * @see GetPortCapability
	 * @see DestroyRender
	 */
	int (*CreateRender)(struct AudioHwAdapter *adapter,
						const struct AudioHwDeviceDescriptor *desc,
						const struct AudioHwSampleAttributes *attrs,
						uint32_t flags,
						struct AudioHwRender **render);

	/**
	 * @brief Destroys an <b>AudioHwRender</b> object.
	 *
	 * @attention Do not destroy the object during audio rendering.
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param render Indicates the pointer to the <b>AudioHwRender</b> object to operate.
	 * @return Returns <b>0</b> if the <b>AudioHwRender</b> object is destroyed; returns a negative value otherwise.
	 * @see CreateRender
	 */
	void (*DestroyRender)(struct AudioHwAdapter *adapter,
						  struct AudioHwRender *render);

	/**
	 * @brief Creates an <b>AudioHwCapture</b> object.
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param desc Indicates the pointer to the descriptor of the audio adapter to start.
	 * @param attrs Indicates the pointer to the audio sampling attributes to open.
	 * @param flags Indicates the input flags for current audio capture.
	 * @param capture Indicates the double pointer to the <b>AudioHwCapture</b> object.
	 * @return Returns <b>0</b> if the <b>AudioHwCapture</b> object is created successfully;
	 * returns a negative value otherwise.
	 * @see GetPortCapability
	 * @see DestroyCapture
	 */
	int (*CreateCapture)(struct AudioHwAdapter *adapter,
						 const struct AudioHwDeviceDescriptor *desc,
						 const struct AudioHwSampleAttributes *attrs,
						 uint32_t flags,
						 struct AudioHwCapture **capture);
	//audio_source_t source);

	/**
	 * @brief Destroys an <b>AudioHwCapture</b> object.
	 *
	 * @attention Do not destroy the object during audio capturing.
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param capture Indicates the pointer to the <b>AudioHwCapture</b> object to operate.
	 * @return Returns <b>0</b> if the <b>AudioHwCapture</b> object is destroyed; returns a negative value otherwise.
	 * @see CreateCapture
	 */
	void (*DestroyCapture)(struct AudioHwAdapter *adapter,
						   struct AudioHwCapture *capture);

	/**
	 * @brief Obtains the capability set of the port driver for the audio adapter.
	 *
	 * @param adapter Indicates the pointer to the audio adapter to operate.
	 * @param port Indicates the pointer to the port.
	 * @param capability Indicates the pointer to the capability set to obtain.
	 * @return Returns <b>0</b> if the capability set is successfully obtained; returns a negative value otherwise.
	 */
	int32_t (*GetPortCapability)(struct AudioHwAdapter *adapter, const struct AudioHwPort *port, struct AudioHwPortCapability *capability);
};

typedef struct AudioHwAdapter AudioHwAdapter;

struct AudioHwAdapter *GetAudioHwAdapterFuncs(void);

#ifdef __cplusplus
}
#endif

#endif  // AMEBA_HARDWARE_INTERFACES_HARDWARE_AUDIO_AUDIO_HW_ADAPTER_H
/** @} */
