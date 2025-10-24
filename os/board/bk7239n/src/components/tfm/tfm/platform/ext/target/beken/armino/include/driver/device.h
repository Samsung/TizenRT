// Copyright 2022-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <stdint.h>
#include <stddef.h>

struct pm_device_t;

struct device_t {
	const char *name;
#if CONFIG_DM
	const void *config;
	const void *api;
	//device_state_t *state;
	void *data;
#endif
#if CONFIG_PM
	struct pm_device_t *pm;
#endif
};
typedef struct device_t device_t;
typedef uint8_t device_handle_t;

#define DEVICE_MAX_HANDLE (__device_end - __device_start)
#define DEVICE_INVALID_HANDLE 0xFF
extern const device_t __device_start[];
extern const device_t __device_end[];

#define DEVICE_MAX_NAME_LEN CONFIG_DEVICE_MAX_NAME_LEN

#define A_DEVICE_STATE_NAME(device_id) BK_CONCAT(__devicestate_, device_id)
#define A_INIT_ENTRY_NAME(init_id) BK_CONCAT(__init_, init_id)

#define DEVICE_ID2NAME(device_id) BK_CONCAT(__deviceobj_, device_id)
#define DEVICE_ID2PTR(device_id) &DEVICE_ID2NAME(device_id)
#define DEVICE_PTR2HANDLE(device) device_get_handle(device)
#define DEVICE_HANDLE2PTR(handle) device_get_ptr_by_handle(handle)

#define DEVICE_STATE_DEFINE(x) 

#define A_DEVICE_NAME_CHECK(name) \
	BK_BUILD_ASSERT(sizeof(BK_STRINGIFY(name)) <= DEVICE_MAX_NAME_LEN, \
		BK_STRINGIFY(DEVICE_ID2NAME(name)) " too long")

#define A_DEVICE_SECTION(level, prio)                                          \
	__attribute__((__section__(".a_deviceobj_" #level BK_STRINGIFY(prio) "_")))

#define A_INIT_ENTRY_SECTION(level, prio)                                          \
	__attribute__((__section__(".a_init_entry_" #level BK_STRINGIFY(prio) "_")))

#define A_DEVICE_INIT(name_, pm_, data_, config_, api_, state_)                \
	{                                                                      \
		.name = name_,                                                 \
		BK_IF_ENABLED(CONFIG_DM, (.data = (data_),))                   \
		BK_IF_ENABLED(CONFIG_DM, (.config = (config_),))               \
		BK_IF_ENABLED(CONFIG_DM, (.api = (api_),))                     \
		BK_IF_ENABLED(CONFIG_DM, (.state = (state_),))                 \
		BK_IF_ENABLED(CONFIG_PM, (.pm = (pm_),))                       \
	}

//TODO define it in init.h
#define A_INIT_ENTRY_DEFINE(init_id, init_fn, device_, level, prio, pd_)       \
        static const BK_DECL_ALIGN(init_entry_t)                               \
                A_INIT_ENTRY_SECTION(level, prio) __used                       \
                A_INIT_ENTRY_NAME(init_id) = {                                 \
                        .init = (init_fn),                                     \
                        .device = (device_),                                   \
                        .pd = (pd_),                                           \
        };

#define A_DEVICE_HANDLES_DEFINE(...)

#define A_DEVICE_BASE_DEFINE(device_id, name, pm, data, config, level,            \
			     prio, api, state)                                    \
	const BK_DECL_ALIGN(device_t) DEVICE_ID2NAME(device_id)                   \
		A_DEVICE_SECTION(level, prio) __used =                            \
		A_DEVICE_INIT(name, pm, data, config, api, state)


#define A_DEVICE_INIT_ENTRY_DEFINE(device_id, init_fn, level, prio, pd_)          \
	A_INIT_ENTRY_DEFINE(DEVICE_ID2NAME(device_id), init_fn,                   \
		    (DEVICE_ID2PTR(device_id)), level, prio, pd_)

#define A_DEVICE_HANDLES_NAME(...)

#define A_DEVICE_DEFINE(device_id, name, init_fn, pm, data, config,               \
			level, prio, api, state, pd_, ...)                        \
	A_DEVICE_NAME_CHECK(name);                                                \
	A_DEVICE_HANDLES_DEFINE(device_id, __VA_ARGS__);                          \
	A_DEVICE_BASE_DEFINE(device_id, name, pm, data, config, level,  prio, api, state);\
	A_DEVICE_INIT_ENTRY_DEFINE(device_id, init_fn, level, prio, pd_);

#define DEVICE_DEFINE(device_id, name, init_fn, pm, data, config, level, prio, api, pd_) \
	DEVICE_STATE_DEFINE(device_id); \
	A_DEVICE_DEFINE(device_id, name, init_fn, pm, data, config, level, prio, api, \
			&A_DEVICE_STATE_NAME(device_id), pd_)

#define DEVICE_FOREACH(device) for (const device_t *device = __device_start; device != __device_end; device++)

#define DEVICE_EXPORT(device_id) extern const device_t DEVICE_ID2NAME(device_id)

static inline const char* device_get_name(const device_t *device)
{
	return device ? device->name : "null";
}

static inline device_handle_t device_get_handle(const device_t *device)
{
	if (device) {
		return (device - __device_start);
	}

	return DEVICE_INVALID_HANDLE;
}

static inline const device_t* device_get_ptr_by_handle(uint32_t handle)
{
	if (handle >= DEVICE_MAX_HANDLE) {
		return NULL;
	}

	return &__device_start[handle];
}

static inline uint32_t device_get_count(void)
{
	return (__device_end - __device_start);
}

const device_t* device_get_ptr_by_name(const char* name);
