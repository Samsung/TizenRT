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

#ifdef __cplusplus
extern "C" {
#endif

#define DSP_FAILURE                (1)
#define DSP_SUCCESS                (0)

#define DSP_DEV_NAME                "dsp"

#define DSP_CMD_MAGIC              (0xe560000)
enum
{
    WCMD_DSP_DISABLE = DSP_CMD_MAGIC + 1
};

/*******************************************************************************
* Function Declarations
*******************************************************************************/
void dsp_init(void);
void dsp_exit(void);
uint32_t dsp_is_inited(void);

#ifdef __cplusplus
}
#endif
