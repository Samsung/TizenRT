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


#include <common/bk_include.h>
#include "fft_hal.h"
#include "fft_ll_macro_def.h"
#include <driver/fft_types.h>



bk_err_t fft_hal_status_get(fft_status_t *fft_status)
{
	fft_status->fft_done = (bool)fft_ll_get_status_fft_done();
	fft_status->fir_done = (bool)fft_ll_get_status_fir_done();
	fft_status->self_proc_done = (bool)fft_ll_get_status_self_proc_done();
	fft_status->start_trigger = (bool)fft_ll_get_start_trigger_start_trigger();
	fft_status->bit_ext = fft_ll_get_status_bit_ext();

	return BK_OK;
}

bk_err_t fft_hal_int_enable_status_get(fft_int_status *int_status)
{
	int_status->fft_int_enable = (bool)fft_ll_get_fft_config_fft_int_en();
	int_status->fir_int_enable = (bool)fft_ll_get_fir_config_fir_int_en();

	return BK_OK;
}

bk_err_t fft_hal_fft_mode_set(uint32_t value)
{
	fft_ll_set_fft_config_fft_mode(value);

	return BK_OK;
}

bk_err_t fft_hal_fft_config_reset(uint32_t value)
{
	fft_ll_set_fft_config_value(value);

	return BK_OK;
}

bk_err_t fft_hal_fir_config_reset(uint32_t value)
{
	fft_ll_set_fir_config_value(value);

	return BK_OK;
}

bk_err_t fft_hal_ifft_en(uint32_t value)
{
	fft_ll_set_fft_config_ifft(value);

	return BK_OK;
}

bk_err_t fft_hal_fft_int_en(uint32_t value)
{
	fft_ll_set_fft_config_fft_int_en(value);

	return BK_OK;
}

bk_err_t fft_hal_fft_en(uint32_t value)
{
	fft_ll_set_fft_config_fft_enable(value);

	return BK_OK;
}

bk_err_t fft_hal_fir_length_set(uint32_t value)
{
	fft_ll_set_fir_config_fir_length(value);

	return BK_OK;
}

bk_err_t fft_hal_fir_mode_set(uint32_t value)
{
	fft_ll_set_fir_config_fir_mode(value);

	return BK_OK;
}

bk_err_t fft_hal_fir_int_en(uint32_t value)
{
	fft_ll_set_fir_config_fir_int_en(value);

	return BK_OK;
}

bk_err_t fft_hal_fir_en(uint32_t value)
{
	fft_ll_set_fir_config_fir_enable(value);

	return BK_OK;
}

int32 fft_hal_data_read(void)
{
	return fft_ll_get_data_ports_value();
}

bk_err_t fft_hal_data_write(uint32_t value)
{
	fft_ll_set_data_ports_value(value);

	return BK_OK;
}

bk_err_t fft_hal_fir_coef_write(int32 value)
{
	fft_ll_set_coef_ports_coef_port(value);

	return BK_OK;
}

bk_err_t fft_hal_start_trigger_set(uint32_t value)
{
	fft_ll_set_start_trigger_start_trigger(value);

	return BK_OK;
}

