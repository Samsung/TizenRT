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

#pragma once

#include <stdio.h>
#include <driver/hal/hal_uart_types.h>
#include <components/system.h>

#ifdef __cplusplus
extern "C" {
#endif

#define warning_prf                    bk_printf
#define fatal_prf                      bk_printf
#define null_prf                       bk_null_printf

void uart0_isr(void);
void uart1_isr(void);
void uart2_isr(void);
void uart3_isr(void);
void print_hex_dump(const char *prefix, const void *b, int len);
uint32_t uart_get_length_in_buffer(uart_id_t id);
uint32_t uart_id_to_pm_uart_id(uint32_t id);
bk_err_t uart_write_ready(uart_id_t id);
bk_err_t uart_write_byte(uart_id_t id, uint8_t data);
void uart_write_byte_for_ate(uart_id_t id, uint8_t *data, uint8_t cnt);
bk_err_t uart_write_string(uart_id_t id, const char *string);
uint32_t uart_wait_tx_over(void);
bk_err_t uart_read_ready(uart_id_t id);
int uart_read_byte(uart_id_t id);
int uart_read_byte_ex(uart_id_t id, uint8_t *ch);
uint32_t uart_get_int_enable_status(uart_id_t id);
uint32_t uart_get_interrupt_status(uart_id_t id);
void uart_clear_interrupt_status(uart_id_t id, uint32_t int_status);

#ifdef __cplusplus
}
#endif

