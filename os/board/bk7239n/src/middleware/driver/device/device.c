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

#include <driver/device.h>
#include <common/bk_include.h>
#include <os/str.h>

const device_t* device_get_ptr_by_name(const char* name)
{
	DEVICE_FOREACH(device) {
		if (os_strcmp(device->name, name) == 0) {
			return device;
		}
	}

	return NULL;
}
