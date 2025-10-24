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

enum {
	P_START = 0,
	P_MAIN_START,
	P_PUBKEY_HASH_START,
	P_PUBKEY_HASH_END,
	P_MANIFEST_VERIFY_START,
	P_MANIFEST_VERIFY_END,
	P_IMG_LOAD_START,
	P_IMG_LOAD_END,
	P_IMG_HASH_START,
	P_IMG_HASH_END,
	P_JUMP,
	P_MAX
};

#if CONFIG_PERF
void hal_perf_debug(uint32_t id);
#else
#define hal_perf_debug(id)
#endif
