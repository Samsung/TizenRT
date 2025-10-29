#pragma once

#include <tinyara/config.h>
#include <tinyara/security_hal.h>

#define HEX_PRINT_BYTE_MAX 128
static char s_hex_buff[HEX_PRINT_BYTE_MAX * 2 + 1];

void nist_nist_free_buffer(hal_data *data);
int hal_nist_malloc_buffer(hal_data *data, int buf_len);
void hal_nist_init_buffer(hal_data *data);