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

#include "cli_nist_common.h"

uint8_t nist_rng_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;

    uint16_t count_len = 0;
    uint8_t *count_data = get_param_by_index(0, &count_len);
    uint32_t count = (count_data) ? *(uint32_t *)count_data : 0;

    int ret;
    hal_data random = {0};

    ret = nist_set_response_total_len(sub_type);
    RETURN_IF_FAIL(ret);

    BK_LOGI(NULL, "len = %u\n", sub_type);
    BK_LOGI(NULL, "count = %u\n", count);

    hal_nist_init_buffer(&random);
    random.data_len = sub_type;
    ret = hal_nist_malloc_buffer(&random, random.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "malloc failed\n");
        return NIST_FAIL;
    }

    ret = armino_hal_generate_random(sub_type, &random);
    if (ret != HAL_SUCCESS) {
        BK_LOGI(NULL, "generate random failed\n");
        hal_nist_free_buffer(&random);
        return NIST_FAIL;
    }

    print_hex(random.data, random.data_len);

    ret = nist_add_response_data(random.data, random.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "add response data failed\n");
        hal_nist_free_buffer(&random);
        return NIST_FAIL;
    }

    hal_nist_free_buffer(&random);

    return NIST_OK;
}

