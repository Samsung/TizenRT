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

typedef enum {
	SYS_BUCK_ANALOG = 0,
	SYS_BUCK_DIGITAL,
	SYS_BUCK_PA,
} sys_buck_type_t;

typedef enum {
	SYS_LDO_AON = 0,
	SYS_LDO_IO,
	SYS_LDO_ANALOG,
	SYS_LDO_DIGITAL,
	SYS_LDO_CORE,
} sys_ldo_type_t;

typedef enum {
	SYS_AON_POWER_VDDIO = 0,
	SYS_AON_POWER_VDDANA,
} sys_aon_power_t;

typedef enum {
	SYS_LPO_SRC_26M32K = 0,
	SYS_LPO_SRC_EXTERNAL_32K,
	SYS_LPO_SRC_ROSC,
} sys_lpo_src_t;

typedef enum {
        SYS_ROSC_CALI_AUTO = 0,
        SYS_ROSC_CALI_MANUAL,
        SYS_ROSC_CALI_STOP,
} sys_rosc_cali_mode_t;

