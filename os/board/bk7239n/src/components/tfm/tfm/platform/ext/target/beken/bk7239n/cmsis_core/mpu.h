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

#ifndef __MPU_H__
#define __MPU_H__

void mpu_clear(uint32_t rnr);

void mpu_cfg(uint32_t rnr, uint32_t rbar, uint32_t rlar);

void mpu_dump(void);

void mpu_enable(void);

void mpu_disable(void);

#endif //#ifndef __MPU_H__