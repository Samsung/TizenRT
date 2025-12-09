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

#include <stdint.h>
#include <string.h>
#include <os/os.h>
#include "cli.h"
#include "cli_nist.h"
#include "cli_nist_common.h"
#include <tinyara/config.h>
#include <tinyara/security_hal.h>

extern int armino_hal_get_hash(hal_hash_type mode, hal_data *input, hal_data *hash);
extern uint8_t *get_param_by_index(uint32_t index, uint16_t *len);

uint8_t nist_hash_verify_callback(nist_handle_t *nist_handle)
{
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t data_len = 0;
    uint8_t *data = get_param_by_index(0, &data_len);
    uint32_t len = *(uint32_t *)data;
    uint16_t msg_len = 0;
    uint8_t *msg = get_param_by_index(1, &msg_len);
    uint16_t md_len = 0;
    uint8_t *md = get_param_by_index(2, &md_len);
    int ret;
    hal_data input_data, hash;

    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "len = %u\n", len);


    hal_nist_init_buffer(&input_data);
    hal_nist_init_buffer(&hash);

    hash.data_len = md_len;
    ret = hal_nist_malloc_buffer(&hash, hash.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "HASH DATA ALLOC FAIL!\n");
        return HAL_ALLOC_FAIL;
    }

    input_data.data_len = len / 8;
    input_data.data = msg;

    ret = armino_hal_get_hash(sub_type, &input_data, &hash);
    if (ret != 0) {
        BK_LOGI(NULL, "ARMINO HASH FAIL!\n");
        hal_nist_free_buffer(&input_data);
        hal_nist_free_buffer(&hash);
        return ret;
    }

    if (os_memcmp(hash.data, md, hash.data_len) != 0) {
        BK_LOGI(NULL, "THE HASH RESULT IS FAULT!\n");

        print_hex(hash.data, hash.data_len);
        print_hex(md, md_len);

        hal_nist_free_buffer(&input_data);
        hal_nist_free_buffer(&hash);
        return NIST_FAIL;
    }

    hal_nist_free_buffer(&input_data);
    hal_nist_free_buffer(&hash);

    return NIST_OK;
}

uint8_t nist_hash_monte_verify_callback(nist_handle_t *nist_handle)
{
    static int test_times = 0;
    uint8_t sub_type = nist_handle->sub_type;
    uint16_t seed_len = 0;
    uint8_t *seed = get_param_by_index(0, &seed_len);
    uint16_t checkpoint_idx_len = 0;
    uint8_t *checkpoint_idx_data = get_param_by_index(1, &checkpoint_idx_len);
    uint32_t checkpoint_idx = (checkpoint_idx_data) ? *(uint32_t *)checkpoint_idx_data : 0;
    uint16_t expected_md_len = 0;
    uint8_t *expected_md = get_param_by_index(2, &expected_md_len);

    int ret;
    hal_data input_data, hash;
    static uint8_t *md_buf[3] = {NULL, NULL, NULL};
    uint8_t *msg_buf = NULL;
    uint32_t hash_len = seed_len;

    const uint32_t ITERATIONS_PER_CHECKPOINT = 1000;
    const uint32_t TOTAL_CHECKPOINTS = 100;

    BK_LOGI(NULL, "=== NIST SHA Monte Carlo Test ===\n");
    BK_LOGI(NULL, "sub_type = %u\n", sub_type);
    BK_LOGI(NULL, "checkpoint = %u/%u\n", checkpoint_idx, TOTAL_CHECKPOINTS);
    BK_LOGI(NULL, "seed_len = %u\n", seed_len);

    if (test_times != 0 && (md_buf[0] == NULL || md_buf[1] == NULL || md_buf[2] == NULL)) {
        BK_LOGI(NULL, "TEST TIMES NOT EQUAL TO 0 AND SEED BUFFER IS NULL!\n");
        return NIST_FAIL;
    }

    hal_nist_init_buffer(&input_data);
    hal_nist_init_buffer(&hash);

    hash.data_len = expected_md_len;
    ret = hal_nist_malloc_buffer(&hash, hash.data_len);
    if (ret != 0) {
        BK_LOGI(NULL, "HASH DATA ALLOC FAIL!\n");
        return HAL_ALLOC_FAIL;
    }

    if (test_times == 0) {
        for (int i = 0; i < 3; i++) {
            md_buf[i] = os_malloc(hash_len);
            if (md_buf[i] == NULL) {
                BK_LOGI(NULL, "MD BUFFER %d ALLOC FAIL!\n", i);
                for (int j = 0; j < i; j++) {
                    os_free(md_buf[j]);
                    md_buf[j] = NULL;
                }
                hal_nist_free_buffer(&input_data);
                hal_nist_free_buffer(&hash);
                return HAL_ALLOC_FAIL;
            }
        }
    }

    msg_buf = os_malloc(hash_len * 3);
    if (msg_buf == NULL) {
        BK_LOGI(NULL, "MSG BUFFER ALLOC FAIL!\n");
        for (int i = 0; i < 3; i++) {
            os_free(md_buf[i]);
            md_buf[i] = NULL;
        }
        hal_nist_free_buffer(&input_data);
        hal_nist_free_buffer(&hash);
        return HAL_ALLOC_FAIL;
    }

    if (test_times == 0) {
        BK_LOGI(NULL, "Initializing checkpoint %u...\n", checkpoint_idx);
        memcpy(md_buf[0], seed, seed_len);
        memcpy(md_buf[1], seed, seed_len);
        memcpy(md_buf[2], seed, seed_len);
    }
    test_times++;

    for (uint32_t i = 3; i < ITERATIONS_PER_CHECKPOINT + 3; i++) {
        memcpy(msg_buf, md_buf[0], hash_len);
        memcpy(msg_buf + hash_len, md_buf[1], hash_len);
        memcpy(msg_buf + hash_len * 2, md_buf[2], hash_len);

        input_data.data = msg_buf;
        input_data.data_len = hash_len * 3;

        ret = armino_hal_get_hash(sub_type, &input_data, &hash);
        if (ret != 0) {
            BK_LOGI(NULL, "ARMINO HASH FAIL at iteration %u!\n", i);
            goto error_cleanup;
        }

        memcpy(md_buf[0], md_buf[1], hash_len);
        memcpy(md_buf[1], md_buf[2], hash_len);
        memcpy(md_buf[2], hash.data, hash.data_len);

        if (i % 100 == 0) {
            BK_LOGI(NULL, "  Iteration %u/%u\n", i - 2, ITERATIONS_PER_CHECKPOINT);
        }
    }

    BK_LOGI(NULL, "Checkpoint %u completed.\n", checkpoint_idx);
    BK_LOGI(NULL, "MDj (Checkpoint output):\n");
    print_hex(md_buf[2], hash_len);

    if (expected_md && expected_md_len > 0) {
        if (os_memcmp((char *)md_buf[2], (char *)expected_md, hash_len) != 0) {
            BK_LOGI(NULL, "CHECKPOINT %u VERIFICATION FAILED!\n", checkpoint_idx);
            print_hex(expected_md, expected_md_len);
            print_hex(md_buf[2], hash_len);
            ret = NIST_FAIL;
            goto error_cleanup;
        }
        BK_LOGI(NULL, "CHECKPOINT %u VERIFICATION PASSED!\n", checkpoint_idx);
        memcpy(md_buf[0], md_buf[2], hash_len);
        memcpy(md_buf[1], md_buf[2], hash_len);
    }

    if (checkpoint_idx < TOTAL_CHECKPOINTS - 1) {
        BK_LOGI(NULL, "Next checkpoint seed = MD1002\n");
    } else {
        BK_LOGI(NULL, "=== ALL CHECKPOINTS COMPLETED ===\n");
    }

    os_free(msg_buf);
    if (test_times == TOTAL_CHECKPOINTS) {
        for (int i = 0; i < 3; i++) {
            os_free(md_buf[i]);
            md_buf[i] = NULL;
        }
        test_times = 0;
    }

    hal_nist_free_buffer(&input_data);
    hal_nist_free_buffer(&hash);

    return NIST_OK;

error_cleanup:
    os_free(msg_buf);
    for (int i = 0; i < 3; i++) {
        os_free(md_buf[i]);
        md_buf[i] = NULL;
    }

    hal_nist_free_buffer(&input_data);
    hal_nist_free_buffer(&hash);
    return ret;
}

