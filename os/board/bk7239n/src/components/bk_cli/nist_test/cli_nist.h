#pragma once

#include <stdint.h>

enum NIST_STATUS{
    NIST_OK = 0,
    NIST_FAIL,
    NIST_CRC_ERROR,
    NIST_FRAME_HEAD_ERROR,
    NIST_LEN_ERROR,
    NIST_FRAME_PARSE_ERROR,
    NIST_COMMON_ERROR = 0x20,
    NIST_CASE_TYPE_ERROR,
    NIST_DATA_PTR_NULL,
    NIST_RESPONSE_DATA_PTR_NULL,
    NIST_RESPONSE_DATA_LEN_ERROR,
    NIST_PARAM_NULL,
    NIST_MALLOC_ERROR,
    NIST_HANDLE_STATE_ERROR,
    NIST_RESPONSE_SET_OVERFLOW,
};

#define NIST_CASE_PARAM_NUM_MAX 20

struct nist_handle {
    uint8_t data_handling;
    uint8_t case_type;
    uint8_t sub_type;
    uint8_t param_num;

    uint16_t offset[NIST_CASE_PARAM_NUM_MAX];
    uint16_t param_len[NIST_CASE_PARAM_NUM_MAX];
    uint16_t rx_offset;
    uint16_t data_len;
    uint8_t *data;

    uint8_t response_param_num;
    uint8_t response_set_index;
    uint16_t response_offset[NIST_CASE_PARAM_NUM_MAX];
    uint16_t response_tx_offset;
    uint16_t last_response_tx_offset;
    uint16_t response_set_offset;
    uint16_t response_data_len;
    uint8_t *response_data;
};
typedef struct nist_handle nist_handle_t;


typedef uint8_t(*algo_verify_callback_t)(nist_handle_t *);

uint32_t get_algo_verify_callback_size(void);
algo_verify_callback_t get_algo_verify_callback(uint8_t case_type);
uint8_t *get_param_by_index(uint32_t index, uint16_t *len);

/*
 * @brief    add response data to response buffer
 *
 * @param[in] data - data to be added
 *
 * @param[in] len - length of data to be added
 *
 * @return   NIST_OK on success, other error code otherwise
 *
 */
int nist_add_response_data(uint8_t *data, uint16_t len);

/*
 * @brief    set total length of response buffer
 *
 * @param[in] len - total length of response buffer
 *
 * @return   NIST_OK on success, other error code otherwise
 *
 */
int nist_set_response_total_len(uint16_t len);

#define RETURN_IF_FAIL(ret) do { if (ret != NIST_OK) return ret; } while (0)


