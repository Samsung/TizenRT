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

#include <driver/hal/hal_uart_types.h>
#include <components/log.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint32_t size;
	uint32_t in;
	uint32_t out;
	uint32_t last_value;
	uint32_t put_cnt;
	uint32_t full_cnt;
	uint32_t empty_cnt;
} uart_statis_kfifo_t;

typedef struct {
	uint32_t uart_isr_cnt;
	uint32_t rx_isr_cnt;
	uint32_t rx_fifo_cnt;
	uint32_t recv_timeout_cnt;
	uart_statis_kfifo_t kfifo_status;
} uart_statis_t;

#if CONFIG_UART_STATIS

#define UART_STATIS_DEC()  uart_statis_t* uart_statis = NULL
#define UART_STATIS_GET(_statis, _id) (_statis) = uart_statis_get_statis((_id))

#define UART_STATIS_SET(_statis, _v) do{\
		(_statis) = (_v);\
	} while(0)

#define UART_STATIS_ADD(_statis, _v) do{\
		(_statis) += (_v);\
	} while(0)

bk_err_t uart_statis_init(void);
bk_err_t uart_statis_id_init(uart_id_t id);
uart_statis_t* uart_statis_get_statis(uart_id_t id);
void uart_statis_dump(uart_id_t id);
#else
#define UART_STATIS_DEC() 
#define UART_STATIS_GET(_statis, _id) 
#define UART_STATIS_SET(_id, _v)
#define UART_STATIS_ADD(_statis, _v)
#define uart_statis_init()
#define uart_statis_id_init(id)
#define uart_statis_get_statis(id) NULL
#define	uart_statis_dump(id)
#endif

#define UART_STATIS_INC(_statis) UART_STATIS_ADD((_statis), 1)

#ifdef __cplusplus
}
#endif
