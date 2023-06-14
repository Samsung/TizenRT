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
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "audio_hw_debug.h"
#include "audio_hw_osal_errnos.h"
#include "osdep_service.h"

#include "primary_audio_hw_adapter.h"

// ----------------------------------------------------------------------------
// AudioHwAdapter
extern struct AudioHwRender *GetAudioHwRenderFuncs(struct AudioHwAdapter *adapter, const struct AudioHwDeviceDescriptor *desc,
		const struct AudioHwSampleAttributes *attrs, uint32_t flags);
extern void CloseAudioHwRender(struct AudioHwRender *render);
extern struct AudioHwCapture *GetAudioHwCaptureFuncs(struct AudioHwAdapter *adapter, const struct AudioHwDeviceDescriptor *desc,
		const struct AudioHwSampleAttributes *attrs, uint32_t flags);
extern void CloseAudioHwCapture(struct AudioHwCapture *capture);

static int PrimarySetAdapterParameters(struct AudioHwAdapter *adapter, const char *kvpairs)
{
	(void) adapter;
	(void) kvpairs;
	return HAL_OSAL_ERR_INVALID_OPERATION;
}

static char *PrimaryGetAdapterParameters(const struct AudioHwAdapter *adapter,
		const char *keys)
{
	(void) adapter;
	(void) keys;
	return (char *)strdup("");
}

static int PrimarySetCaptureVolume(const struct AudioHwAdapter *adapter, float volume)
{
	//TODO
	(void) adapter;
	(void) volume;
	return 0;
}

static int PrimaryGetCaptureVolume(const struct AudioHwAdapter *adapter, float *volume)
{
	//TODO
	(void) adapter;
	(void) volume;
	return 0;
}

static int PrimarySetRenderVolume(const struct AudioHwAdapter *adapter, float volume)
{
	//TODO
	(void) adapter;
	(void) volume;
	return 0;
}

static int PrimaryGetRenderVolume(const struct AudioHwAdapter *adapter, float *volume)
{
	//TODO
	(void) adapter;
	(void) volume;
	return 0;
}

static int PrimarySetRenderMute(const struct AudioHwAdapter *adapter, bool muted)
{
	//TODO
	(void) adapter;
	(void) muted;
	return 0;
}

static int PrimaryGetRenderMute(const struct AudioHwAdapter *adapter, bool *muted)
{
	//TODO
	(void) adapter;
	(void) muted;
	return 0;
}

static int PrimarySetCaptureMute(const struct AudioHwAdapter *adapter, bool muted)
{
	//TODO
	(void) adapter;
	(void) muted;
	return 0;
}

static int PrimaryGetCaptureMute(const struct AudioHwAdapter *adapter, bool *muted)
{
	//TODO
	(void) adapter;
	(void) muted;
	return 0;
}

static size_t PrimaryGetAdapterInputBufferSize(const struct AudioHwAdapter *adapter, const struct AudioHwSampleAttributes *attrs)
{
	(void) adapter;
	if (attrs == NULL) {
		HAL_AUDIO_ERROR("primaryGetAdapterInputBufferSize AudioHwSampleAttributes is null");
		return HAL_OSAL_ERR_INVALID_PARAM;
	}

	return GetHwInputBufferSize(attrs->sample_rate, attrs->format, attrs->channel_count);
}

static int PrimaryCreateRender(
	struct AudioHwAdapter *adapter,
	const struct AudioHwDeviceDescriptor *desc,
	const struct AudioHwSampleAttributes *attrs,
	uint32_t flags,
	struct AudioHwRender **render)
{
	*render = GetAudioHwRenderFuncs(adapter, desc, attrs, flags);

	if (*render == NULL) {
		return HAL_OSAL_ERR_NO_MEMORY;
	}

	return HAL_OSAL_OK;
}

static void PrimaryDestroyRender(struct AudioHwAdapter *adapter, struct AudioHwRender *render)
{
	(void) adapter;
	CloseAudioHwRender(render);

}


static int PrimaryCreateCapture(
	struct AudioHwAdapter *adapter,
	const struct AudioHwDeviceDescriptor *desc,
	const struct AudioHwSampleAttributes *attrs,
	uint32_t flags,
	struct AudioHwCapture **capture)
{
	*capture = GetAudioHwCaptureFuncs(adapter, desc, attrs, flags);

	if (*capture == NULL) {
		return HAL_OSAL_ERR_NO_MEMORY;
	}

	return HAL_OSAL_OK;

}

static void PrimaryDestroyCapture(struct AudioHwAdapter *adapter, struct AudioHwCapture *capture)
{
	(void) adapter;
	CloseAudioHwCapture(capture);

	return;
}

int32_t PrimaryGetHwPortCapability(struct AudioHwAdapter *adapter, const struct AudioHwPort *port, struct AudioHwPortCapability *capability)
{
	//TODO
	(void) adapter;
	(void) port;
	(void) capability;
	return 0;
}

struct AudioHwAdapter *GetAudioHwAdapterFuncs()
{
	struct PrimaryAudioHwAdapter *adap;
	adap = (struct PrimaryAudioHwAdapter *)rtw_zmalloc(sizeof(struct PrimaryAudioHwAdapter));
	if (!adap) {
		return NULL;
	}

	adap->adapter.SetParameters = PrimarySetAdapterParameters;
	adap->adapter.GetParameters = PrimaryGetAdapterParameters;
	adap->adapter.SetCaptureVolume = PrimarySetCaptureVolume;
	adap->adapter.GetCaptureVolume = PrimaryGetCaptureVolume;
	adap->adapter.SetRenderVolume = PrimarySetRenderVolume;
	adap->adapter.GetRenderVolume = PrimaryGetRenderVolume;
	adap->adapter.SetRenderMute = PrimarySetRenderMute;
	adap->adapter.GetRenderMute = PrimaryGetRenderMute;
	adap->adapter.SetCaptureMute = PrimarySetCaptureMute;
	adap->adapter.GetCaptureMute = PrimaryGetCaptureMute;
	adap->adapter.GetInputBufferSize = PrimaryGetAdapterInputBufferSize;
	adap->adapter.CreateRender = PrimaryCreateRender;
	adap->adapter.DestroyRender = PrimaryDestroyRender;
	adap->adapter.CreateCapture = PrimaryCreateCapture;
	adap->adapter.DestroyCapture = PrimaryDestroyCapture;
	adap->adapter.GetPortCapability = PrimaryGetHwPortCapability;

	rtw_mutex_init(&adap->lock);

	return  &adap->adapter;

}

void CloseAudioHwAdapter(struct AudioHwAdapter *adapter)
{
	struct PrimaryAudioHwAdapter *adap = (struct PrimaryAudioHwAdapter *)(adapter);
	rtw_mutex_free(&adap->lock);

	if (adapter != NULL) {
		rtw_free(adapter);
		//adapter = NULL;
	}
}
