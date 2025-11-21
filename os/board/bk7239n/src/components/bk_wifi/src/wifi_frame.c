// Copyright 2020-2025 Beken
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

#include "../wpa_supplicant-2.10/src/utils/common.h"
#include "../wpa_supplicant-2.10/src/common/ieee802_11_common.h"

uint32_t bk_wifi_find_ie(uint32_t addr,
                     uint16_t buflen,
                     uint8_t ie_id)
{
	return (uint32_t)get_ie((uint8_t *)addr, buflen, ie_id);
}
