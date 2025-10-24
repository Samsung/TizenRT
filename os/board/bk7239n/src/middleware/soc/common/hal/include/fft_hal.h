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

#include "hal_config.h"
#include <soc/soc.h>
#include <driver/fft_types.h>

#ifdef __cplusplus
extern "C" {
#endif

bk_err_t fft_hal_status_get(fft_status_t *fft_status);
bk_err_t fft_hal_int_enable_status_get(fft_int_status *int_status);
bk_err_t fft_hal_fft_mode_set(uint32_t value);
bk_err_t fft_hal_fft_config_reset(uint32_t value);
bk_err_t fft_hal_fir_config_reset(uint32_t value);
bk_err_t fft_hal_ifft_en(uint32_t value);
bk_err_t fft_hal_fft_int_en(uint32_t value);
bk_err_t fft_hal_fft_en(uint32_t value);
bk_err_t fft_hal_fir_length_set(uint32_t value);
bk_err_t fft_hal_fir_mode_set(uint32_t value);
bk_err_t fft_hal_fir_int_en(uint32_t value);
bk_err_t fft_hal_fir_en(uint32_t value);
int32 fft_hal_data_read(void);
bk_err_t fft_hal_data_write(uint32_t value);
bk_err_t fft_hal_fir_coef_write(int32 value);
bk_err_t fft_hal_start_trigger_set(uint32_t value);

#if CFG_HAL_DEBUG_FFT
void fft_struct_dump(void);
#else
#define fft_struct_dump()
#endif


#ifdef __cplusplus
}
#endif
