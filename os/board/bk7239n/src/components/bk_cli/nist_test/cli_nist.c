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
#include <ctype.h>
#include <os/os.h>
#include "cli.h"
#include "cli_nist.h"


#define NIST_FRAME_TYPE_PROFILE 0x00
#define NIST_FRAME_TYPE_DATA 0x01
#define NIST_FRAME_TYPE_ACK 0x02
#define NIST_FRAME_TYPE_VERIFY 0x03
#define NIST_FRAME_TYPE_REQUEST_DATA_PROFILE 0x04
#define NIST_FRAME_TYPE_REQUEST_DATA_PROFILE_ACK 0x05
#define NIST_FRAME_TYPE_REQUEST_DATA 0x06
#define NIST_FRAME_TYPE_REQUEST_DATA_ACK 0x07
#define NIST_FRAME_TYPE_REQUEST_OVER 0x08

uint8_t *get_param_by_index(uint32_t index, uint16_t *len);

static void cli_nist_help(void)
{
}

int hex_str_to_uint8(const char *hex_str, uint8_t *out_buf, int buf_len)
{
    if (hex_str == NULL || out_buf == NULL || buf_len <= 0) {
        return -1;
    }

    int hex_len = strlen(hex_str);
    if (hex_len % 2 != 0) {
        return -1;
    }

    int byte_count = hex_len / 2;
    if (byte_count > buf_len) {
        return -1;
    }

    for (int i = 0; i < byte_count; i++) {
        char c1 = hex_str[2 * i];
        char c2 = hex_str[2 * i + 1];
        uint8_t val = 0;

        if (isdigit(c1)) {
            val += (c1 - '0') << 4;
        } else if (tolower(c1) >= 'a' && tolower(c1) <= 'f') {
            val += (tolower(c1) - 'a' + 10) << 4;
        } else {
            return -1;
        }

        if (isdigit(c2)) {
            val += (c2 - '0');
        } else if (tolower(c2) >= 'a' && tolower(c2) <= 'f') {
            val += (tolower(c2) - 'a' + 10);
        } else {
            return -1;
        }

        out_buf[i] = val;
    }

    return byte_count;
}

static uint16_t crc16(const uint8_t *data, uint32_t length)
{
    uint32_t crc = 0xFFFFFFFF;
    const uint16_t poly = 0x8005;

    for (uint32_t i = 0; i < length; i++) {
        crc ^= (uint32_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ poly;
            } else {
                crc = crc << 1;
            }
        }
    }
    return (uint16_t)(crc & 0xFFFF);
}
/* ================== nist data start =================== */

struct nist_frame {
    uint8_t head;
    uint8_t case_type;
    uint8_t frame_type;
    uint8_t end_flag;     // 0: not end, 1: end
    uint16_t len;
    uint8_t data[0];
};
typedef struct nist_frame nist_frame_t;
static uint8_t s_nist_buff[64] = { 0 };

static nist_handle_t s_nist_handle = { 0 };

struct profile_info {
    uint8_t sub_type;
    uint8_t data_num;
    uint16_t total_len;
    uint16_t offset[0];
};
typedef struct profile_info profile_info_t;

uint8_t *get_param_by_index(uint32_t index, uint16_t *len)
{
    if (index >= s_nist_handle.param_num) {
    BK_LOGI(NULL, "the index is invalid \n");
        return NULL;
    }
    *len = s_nist_handle.param_len[index];
    if (s_nist_handle.param_len[index] == 0) {
    BK_LOGI(NULL, "now it is NULL \n");
        return NULL;
    }
    return s_nist_handle.data + s_nist_handle.offset[index];
}

int nist_set_response_total_len(uint16_t len)
{
    BK_LOGI(NULL, "nist_set_response_total_len, len=%u\n", len);
    s_nist_handle.response_data_len = len;
    s_nist_handle.response_set_offset = 0;
    s_nist_handle.response_set_index = 0;
    s_nist_handle.response_param_num = 0;
    s_nist_handle.response_data = (uint8_t *)os_malloc(len);
    if (s_nist_handle.response_data == NULL) {
        BK_LOGI(NULL, "ERROR,os_malloc failed, len=%u\n", len);
        return NIST_MALLOC_ERROR;
    }
    return NIST_OK;
}

int nist_add_response_data(uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0) {
        return NIST_PARAM_NULL;
    }
    if (s_nist_handle.response_set_offset + len > s_nist_handle.response_data_len) {
        BK_LOGI(NULL, "ERROR,response set overflow\n");
        return NIST_RESPONSE_SET_OVERFLOW;
    }
    s_nist_handle.response_offset[s_nist_handle.response_set_index] = s_nist_handle.response_set_offset;
    memcpy(s_nist_handle.response_data + s_nist_handle.response_set_offset, data, len);
    s_nist_handle.response_set_offset += len;
    s_nist_handle.response_set_index++;
    s_nist_handle.response_param_num++;
    return NIST_OK;
}

/* ================== nist data end =================== */
/* ================== rsponse frame start =================== */
struct nist_ack_frame {
    uint8_t head;
    uint8_t case_type;
    uint8_t frame_type;
    uint8_t state;
    uint16_t crc;
};
typedef struct nist_ack_frame nist_ack_frame_t;

static void nist_send_data(uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0) {
        return;
    }
    char buf[80] = { 0 };
    for (uint16_t i = 0; i < len; i++) {
        sprintf(buf + (2 * i), "%02x", data[i]);
    }
    CLI_LOGI("%s\n", buf);
}

static void nist_ack_frame(uint8_t state)
{
    nist_ack_frame_t frame = {
        .head = 0xEA,
        .case_type = s_nist_handle.case_type,
        .frame_type = NIST_FRAME_TYPE_ACK,
        .state = state
    };
    uint16_t crc = crc16((const uint8_t *)&frame, sizeof(nist_ack_frame_t) - 2);
    frame.crc = crc;
    nist_send_data((uint8_t *)&frame, sizeof(nist_ack_frame_t));
}

static void nist_response_data_frame(uint8_t frame_type, uint8_t end_flag, uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0) {
        return;
    }
    // uint8_t frame_data[64] = { 0 };

    nist_frame_t *frame = (nist_frame_t *)s_nist_buff;
    frame->head = 0xEA;
    frame->case_type = s_nist_handle.case_type;
    frame->frame_type = frame_type;
    frame->end_flag = end_flag;
    frame->len = len;
    memcpy(frame->data, data, len);
    uint16_t crc = crc16((const uint8_t *)s_nist_buff, sizeof(nist_frame_t) + len);
    *(uint16_t *)(s_nist_buff + sizeof(nist_frame_t) + len) = crc;
    nist_send_data(s_nist_buff, sizeof(nist_frame_t) + len + sizeof(uint16_t));
}

static inline bool nist_check_case_type(uint8_t case_type)
{
    return get_algo_verify_callback(case_type) != NULL;
}
/* ================== algo verify callback end   =================== */

static int nist_check_frame(uint8_t *data, uint32_t data_len)
{
    nist_frame_t *frame = (nist_frame_t *)data;
    if (frame->head != 0xEA) {
        BK_LOGI(NULL, "ERROR,head = %02x\n", frame->head);
        nist_ack_frame(NIST_FRAME_HEAD_ERROR);
        return -1;
    }
    // check frame len
    if (data_len != sizeof(nist_frame_t) + frame->len + 2) {
        BK_LOGI(NULL, "ERROR,data_len = %d\n", data_len);
        nist_ack_frame(NIST_LEN_ERROR);
        return -1;
    }
    // check crc
    uint16_t crc = *(uint16_t *)(frame->data + frame->len);
    if (crc != crc16((const uint8_t *)frame, sizeof(nist_frame_t) + frame->len)) {
        BK_LOGI(NULL, "ERROR,crc = %04x\n", crc);
        nist_ack_frame(NIST_CRC_ERROR);
        return -1;
    }
    return 0;
}

static void nist_handle_profile_frame(nist_frame_t *frame)
{
    memset(&s_nist_handle, 0, sizeof(nist_handle_t));
    BK_LOGI(NULL, "profile frame, init handle\n");
    s_nist_handle.data_handling = 1;
    profile_info_t *profile_info = (profile_info_t *)frame->data;
    s_nist_handle.param_num = profile_info->data_num;
    s_nist_handle.data_len = profile_info->total_len;
    s_nist_handle.case_type = frame->case_type;
    s_nist_handle.sub_type = profile_info->sub_type;

    // check case type
    if (!nist_check_case_type(s_nist_handle.case_type)) {
        BK_LOGI(NULL, "ERROR,case type = %u\n", s_nist_handle.case_type);
        nist_ack_frame(NIST_CASE_TYPE_ERROR);
        return;
    }

    for (int i = 0; i < profile_info->data_num; i++) {
        s_nist_handle.offset[i] = profile_info->offset[i];
        if (i == profile_info->data_num - 1) {
            s_nist_handle.param_len[i] = profile_info->total_len - profile_info->offset[i];
        } else {
            s_nist_handle.param_len[i] = profile_info->offset[i + 1] - profile_info->offset[i];
        }
    }
    if (s_nist_handle.data != NULL) {
        BK_LOGI(NULL, "W:data ptr is not NULL\n");
        os_free(s_nist_handle.data);
        s_nist_handle.data = NULL;
    }
    if (s_nist_handle.response_data != NULL) {
        BK_LOGI(NULL, "W:response_data ptr is not NULL\n");
        os_free(s_nist_handle.response_data);
        s_nist_handle.response_data = NULL;
        s_nist_handle.response_data_len = 0;
        s_nist_handle.response_tx_offset = 0;
    }
    if (s_nist_handle.data_len) {
        s_nist_handle.data = (uint8_t *)os_malloc(s_nist_handle.data_len);
        if (s_nist_handle.data == NULL) {
            BK_LOGI(NULL, "ERROR,os_malloc failed, len=%u\n", s_nist_handle.data_len);
            nist_ack_frame(NIST_MALLOC_ERROR);
            return;
        }
    } else {
        s_nist_handle.data_handling = 0;
    }
    memcpy(s_nist_handle.data, frame->data + profile_info->total_len, s_nist_handle.data_len);
    nist_ack_frame(NIST_OK);
}

static void nist_handle_data_frame(nist_frame_t *frame)
{
    if (s_nist_handle.data_handling == 0) {
        BK_LOGI(NULL, "ERROR,handling = 0\n");
        nist_ack_frame(NIST_HANDLE_STATE_ERROR);
        return;
    }
    if (s_nist_handle.data_len != 0 && s_nist_handle.data == NULL) {
        BK_LOGI(NULL, "ERROR,data = NULL\n");
        nist_ack_frame(NIST_DATA_PTR_NULL);
        return;
    }

    if (!nist_check_case_type(s_nist_handle.case_type)) {
        BK_LOGI(NULL, "ERROR,case type = %u\n", s_nist_handle.case_type);
        nist_ack_frame(NIST_CASE_TYPE_ERROR);
        return;
    }
    memcpy(s_nist_handle.data + s_nist_handle.rx_offset, frame->data, frame->len);
    s_nist_handle.rx_offset += frame->len;
    if (frame->end_flag == 1) {
        s_nist_handle.data_handling = 0;
    }
    nist_ack_frame(NIST_OK);
}

static void nist_handle_verify_frame(nist_frame_t *frame)
{
    if (s_nist_handle.data_handling == 1) {
        BK_LOGI(NULL, "ERROR,handling = 1\n");
        nist_ack_frame(NIST_HANDLE_STATE_ERROR);
        return;
    }
    if (!nist_check_case_type(s_nist_handle.case_type)) {
        BK_LOGI(NULL, "ERROR,case type = %u\n", s_nist_handle.case_type);
        nist_ack_frame(NIST_CASE_TYPE_ERROR);
        return;
    }
    algo_verify_callback_t callback = get_algo_verify_callback(s_nist_handle.case_type);
    uint8_t ret = callback(&s_nist_handle);
    if (s_nist_handle.data != NULL) {
        os_free(s_nist_handle.data);
        s_nist_handle.data = NULL;
    }
    nist_ack_frame(ret);
}

static void nist_send_response_data(uint8_t *data, uint16_t len)
{
    if (data == NULL || len == 0) {
        return;
    }
    char buf[80] = { 0 };
    for (uint16_t i = 0; i < len; i++) {
        sprintf(buf + (2 * i), "%02x", data[i]);
    }
    CLI_LOGI("%s\n", buf);
}

#define NIST_RESPONSE_FRAME_MAX_LEN 32
static void nist_handle_request_data_frame(nist_frame_t *frame)
{
    // TODO
    if (s_nist_handle.data_handling == 0) {
        BK_LOGI(NULL, "ERROR,handling = 0\n");
        nist_ack_frame(NIST_HANDLE_STATE_ERROR);
        return;
    }
    if (!nist_check_case_type(s_nist_handle.case_type)) {
        BK_LOGI(NULL, "ERROR,case type = %u\n", s_nist_handle.case_type);
        nist_ack_frame(NIST_CASE_TYPE_ERROR);
        return;
    }

    if (s_nist_handle.response_data == NULL || s_nist_handle.response_data_len == 0) {
        BK_LOGI(NULL, "ERROR,response_data = NULL\n");
        nist_ack_frame(NIST_RESPONSE_DATA_PTR_NULL);
        return;
    }
    uint16_t tx_len = s_nist_handle.response_data_len - s_nist_handle.response_tx_offset > NIST_RESPONSE_FRAME_MAX_LEN
                 ? NIST_RESPONSE_FRAME_MAX_LEN : s_nist_handle.response_data_len - s_nist_handle.response_tx_offset;
    uint8_t *tx_data = s_nist_handle.response_data + s_nist_handle.response_tx_offset;
    uint8_t end_flag = s_nist_handle.response_tx_offset + tx_len == s_nist_handle.response_data_len ? 1 : 0;
    BK_LOGI(NULL, "data_len=%u, tx_offset=%u\n", s_nist_handle.response_data_len, s_nist_handle.response_tx_offset);
    BK_LOGI(NULL, "request_data_frame, tx_len=%u, end_flag=%u\n", tx_len, end_flag);
    nist_response_data_frame(NIST_FRAME_TYPE_REQUEST_DATA_ACK, end_flag, tx_data, tx_len);
    s_nist_handle.last_response_tx_offset = s_nist_handle.response_tx_offset;
    s_nist_handle.response_tx_offset += tx_len;
}

struct response_data_profile {
    uint8_t data_num;
    uint8_t reserved;
    uint16_t crc;
    uint16_t data_len;
    uint16_t offset[0];
};
typedef struct response_data_profile response_data_profile_t;

static void nist_handle_request_data_profile_frame(nist_frame_t *frame)
{
    // TODO
    if (s_nist_handle.data_handling == 1) {
        BK_LOGI(NULL, "ERROR,handling = 1\n");
        nist_ack_frame(NIST_HANDLE_STATE_ERROR);
        return;
    }
    s_nist_handle.data_handling = 1;
    if (!nist_check_case_type(s_nist_handle.case_type)) {
        BK_LOGI(NULL, "ERROR,case type = %u\n", s_nist_handle.case_type);
        nist_ack_frame(NIST_CASE_TYPE_ERROR);
        return;
    }

    if (s_nist_handle.response_data == NULL || s_nist_handle.response_data_len == 0) {
        BK_LOGI(NULL, "ERROR,response_data = NULL\n");
        nist_ack_frame(NIST_RESPONSE_DATA_PTR_NULL);
        return;
    }

    if (s_nist_handle.response_data_len != s_nist_handle.response_set_offset) {
        BK_LOGI(NULL, "ERROR,response_data_len = %u, response_set_offset=%u\n",
            s_nist_handle.response_data_len, s_nist_handle.response_set_offset);
        nist_ack_frame(NIST_RESPONSE_DATA_LEN_ERROR);
        return;
    }

    uint16_t len = sizeof(response_data_profile_t) + s_nist_handle.response_param_num * 2;
    uint8_t data[26] = { 0 };
    response_data_profile_t *data_profile = (response_data_profile_t *)data;
    uint16_t crc = crc16((const uint8_t *)s_nist_handle.response_data, s_nist_handle.response_data_len);
    data_profile->data_num = s_nist_handle.response_param_num;
    data_profile->reserved = 0;
    data_profile->crc = crc;
    data_profile->data_len = s_nist_handle.response_data_len;
    for (int i = 0; i < s_nist_handle.response_param_num; i++) {
        data_profile->offset[i] = s_nist_handle.response_offset[i];
    }
    nist_response_data_frame(NIST_FRAME_TYPE_REQUEST_DATA_PROFILE_ACK, 0, data, len);
}

static void nist_handle_request_over_frame(nist_frame_t *frame)
{
    BK_LOGI(NULL, "nist_handle_request_over_frame\n");
    if (s_nist_handle.response_data != NULL) {
        os_free(s_nist_handle.response_data);
        s_nist_handle.response_data = NULL;
        s_nist_handle.response_data_len = 0;
        s_nist_handle.response_tx_offset = 0;
        s_nist_handle.response_param_num = 0;
        s_nist_handle.response_set_offset = 0;
        s_nist_handle.response_set_index = 0;
    }
    BK_LOGI(NULL, "nist_handle_request_over end\n");
    nist_ack_frame(NIST_OK);
}

static void nist_handle_frame(uint8_t *data, uint32_t data_len)
{
    if (nist_check_frame(data, data_len) != 0) {
        BK_LOGI(NULL, "ERROR,nist_check_frame failed\n");
        return;
    }
    nist_frame_t *frame = (nist_frame_t *)data;
    if (frame->frame_type == NIST_FRAME_TYPE_PROFILE) {
        nist_handle_profile_frame(frame);
    } else if (frame->frame_type == NIST_FRAME_TYPE_DATA) {
        nist_handle_data_frame(frame);
    } else if (frame->frame_type == NIST_FRAME_TYPE_VERIFY) {
        nist_handle_verify_frame(frame);
    } else if (frame->frame_type == NIST_FRAME_TYPE_REQUEST_DATA_PROFILE) {
        nist_handle_request_data_profile_frame(frame);
    } else if (frame->frame_type == NIST_FRAME_TYPE_REQUEST_DATA) {
        nist_handle_request_data_frame(frame);
    } else if (frame->frame_type == NIST_FRAME_TYPE_REQUEST_OVER) {
        nist_handle_request_over_frame(frame);
    }
}


static void cli_nist_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc < 2) {
        cli_nist_help();
        return;
    }

    int frame_len = hex_str_to_uint8(argv[1], s_nist_buff, sizeof(s_nist_buff));
    if (frame_len < 0) {
        nist_ack_frame(NIST_FRAME_PARSE_ERROR);
        BK_LOGI(NULL, "ERROR,ret = %d\n", frame_len);
        return;
    }
    // hanlde frame data
    nist_handle_frame(s_nist_buff, frame_len);
}

#define NIST_CMD_CNT (sizeof(s_nist_commands) / sizeof(struct cli_command))
static const struct cli_command s_nist_commands[] = {
    {"nist", "", cli_nist_cmd},
};

int cli_nist_init(void)
{
    return cli_register_commands(s_nist_commands, NIST_CMD_CNT);
}

