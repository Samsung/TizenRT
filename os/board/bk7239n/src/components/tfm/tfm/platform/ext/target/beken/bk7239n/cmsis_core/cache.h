// Copyright 2023-2028 Beken
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

#ifndef __CACHE_H__
#define __CACHE_H__

typedef enum {
	SRAM_BLOCK_MEM0 = 0,
	SRAM_BLOCK_MEM1 = 1,
	SRAM_BLOCK_MEM2 = 2,
	SRAM_BLOCK_MEM3 = 3,
	SRAM_BLOCK_MEM4 = 4,
	SRAM_BLOCK_COUNT,
}sram_bock_t;

void sram_dcache_map(void);

int show_cache_config_info(void);

void enable_dcache(int enable);

void flush_dcache(void *va, long size);

void flush_all_dcache(void);

void invalidate_icache(void);
#endif //__CACHE_H__