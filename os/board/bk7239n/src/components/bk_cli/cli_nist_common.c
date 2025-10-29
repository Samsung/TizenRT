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

#include <tinyara/config.h>
#include <tinyara/security_hal.h>

#include <stdlib.h>
#include "cli_nist_common.h"


void nist_nist_free_buffer(hal_data *data)
{
	if (data == NULL) {
		return;
	}

	if (data->data) {
		free(data->data);
		data->data = NULL;
	}
	if (data->priv) {
		free(data->priv);
		data->priv = NULL;
	}
	data->data_len = 0;
	data->priv_len = 0;
}


int hal_nist_malloc_buffer(hal_data *data, int buf_len)
{
	data->data = (unsigned char *)os_malloc(buf_len);
	if (!data->data) {
		return -1;
	}
	return 0;
}

void hal_nist_init_buffer(hal_data *data)
{
	if (data) {
		data->data = NULL;
		data->data_len = 0;
		data->priv = NULL;
		data->priv_len = 0;
	}
}