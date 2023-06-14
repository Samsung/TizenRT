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

#include "ameba_soc.h"
#include "audio_hw_debug.h"
#include "audio_hw_osal_errnos.h"
#include "osdep_service.h"
#include "platform_stdlib.h"

#include "hardware/audio/audio_hw_manager.h"

// ----------------------------------------------------------------------------
// AudioHwManager
extern void CloseAudioHwAdapter(struct AudioHwAdapter *adapter);

#define PRIMARY_ADAPTER "primary"

static struct AudioHwPort gPrimaryAudioHwPort[2] = {
	{ AUDIO_HW_PORT_OUT, 0, "stream_out" },
	{ AUDIO_HW_PORT_IN, 1, "stream_in" }
};

static struct AudioHwAdapterDescriptor gAudioHwAdapterDescs[] = {
	{ PRIMARY_ADAPTER, 2, gPrimaryAudioHwPort }
};

static const int32_t ADAPTER_DESCRIPTER_COUNT =
	(sizeof(gAudioHwAdapterDescs) / sizeof(gAudioHwAdapterDescs[0]));



static int ClosePrimaryAudioHwAdapter(struct AudioHwAdapter *adapter)
{
	CloseAudioHwAdapter(adapter);

	return 0;
}

static int CreatePrimaryAudioHwAdapter(struct AudioHwAdapter **adapter)
{
	*adapter = GetAudioHwAdapterFuncs();

	if (*adapter == NULL) {
		return HAL_OSAL_ERR_NO_MEMORY;
	}

	return 0;
}

static int32_t AmebaGetAllAdapters(struct AudioHwManager *manager, struct AudioHwAdapterDescriptor **descs, int32_t *size)
{
	(void) manager;
	*descs = gAudioHwAdapterDescs;
	*size = ADAPTER_DESCRIPTER_COUNT;
	return 0;
}

static int32_t AmebaLoadAdapter(struct AudioHwManager *manager, const struct AudioHwAdapterDescriptor *desc, struct AudioHwAdapter **adapter)
{
	(void) manager;
	if (!desc || !desc->adapter_name) {
		HAL_AUDIO_ERROR("LoadAdapter: invalid adapter description!");
		return -1;
	}

	int ret = -1;
	if (!strcmp(desc->adapter_name, PRIMARY_ADAPTER)) {
		ret = CreatePrimaryAudioHwAdapter(adapter);
		HAL_AUDIO_VERBOSE("LoadAdapter(name:%s) %s", desc->adapter_name, ret ? "failed" : "success");
	} else {
		HAL_AUDIO_WARN("LoadAdapter: unsupported adapter(name:%s)", desc->adapter_name);
	}

	return ret;
}

static void AmebaUnloadAdapter(struct AudioHwManager *manager, const struct AudioHwAdapterDescriptor *desc, struct AudioHwAdapter *adapter)
{
	(void) manager;
	if (!desc || !desc->adapter_name) {
		HAL_AUDIO_ERROR("UnloadAdapter: invalid adapter description!");
		return;
	}

	if (!strcmp(desc->adapter_name, PRIMARY_ADAPTER)) {
		ClosePrimaryAudioHwAdapter(adapter);
		HAL_AUDIO_VERBOSE("UnloadAdapter(name:%s)", desc->adapter_name);
	}
}

struct AudioHwManager *GetAudioHwManagerFuncs(void)
{
	struct AudioHwManager *audio_manager;

	audio_manager = (struct AudioHwManager *)rtw_zmalloc(sizeof(struct AudioHwManager));
	if (!audio_manager) {
		return NULL;
	}

	audio_manager->GetAllAdapters = AmebaGetAllAdapters;
	audio_manager->LoadAdapter = AmebaLoadAdapter;
	audio_manager->UnloadAdapter = AmebaUnloadAdapter;

	return audio_manager;
}

void DestoryAudioHwManager(struct AudioHwManager *manager)
{
	if (manager != NULL) {
		HAL_AUDIO_VERBOSE("free manager");
		rtw_free(manager);
		//manager = NULL;
	}
}
