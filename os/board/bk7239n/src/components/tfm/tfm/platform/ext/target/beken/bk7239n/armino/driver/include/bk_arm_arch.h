// Copyright 2020-2021 Beken
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

#include <soc/soc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * RD an 8-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 8-bit datum
 */
#define REG_RD8(_pBase, _offset) *((volatile UINT8 *)(_pBase + _offset))

/**
 * RD a 16-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 16-bit datum
 */
#define REG_RD16(_pBase, _offset) *((volatile UINT16 *)(_pBase + _offset))

/**
 * RD a 32-bit register from the core
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @return 32-bit datum
 */
#define REG_RD32(_pBase, _offset) *((volatile UINT32 *)(_pBase + _offset))

/**
 * WR an 8-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 8-bit datum
 */
#define REG_WR8(_pBase, _offset, _data) \
	(*((volatile UINT8 *)(_pBase + _offset)) = _data)

/**
 * WR a 16-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 16-bit datum
 */
#define REG_WR16(_pBase, _offset, _data) \
	(*((volatile UINT16 *)(_pBase + _offset)) = _data)

/**
 * WR a 32-bit core register
 * @param _pBase core base address in memory
 * @param _offset offset into the core's register space
 * @param _data 32-bit datum
 */
#define REG_WR32(_pBase, _offset, _data) \
	(*((volatile UINT32 *)(_pBase + _offset)) = _data)

#ifdef __cplusplus
}
#endif
