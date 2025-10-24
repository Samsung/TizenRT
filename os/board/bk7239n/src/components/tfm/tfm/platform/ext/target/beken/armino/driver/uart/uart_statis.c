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

#include <os/mem.h>
#include "uart_hw.h"
#include "uart_statis.h"

#if CONFIG_UART_STATIS

#define TAG "uart_statis"

static uart_statis_t s_uart_statis[SOC_UART_ID_NUM_PER_UNIT] = {0};

bk_err_t uart_statis_init(void)
{
	os_memset(&s_uart_statis, 0, sizeof(s_uart_statis));
	return BK_OK;
}

bk_err_t uart_statis_id_init(uart_id_t id)
{
	os_memset(&s_uart_statis[id], 0, sizeof(s_uart_statis[id]));
	s_uart_statis[id].kfifo_status.size = CONFIG_KFIFO_SIZE;
	return BK_OK;
}

uart_statis_t* uart_statis_get_statis(uart_id_t id)
{
	return &(s_uart_statis[id]);
}

void uart_statis_dump(uart_id_t id)
{
	BK_LOGI(TAG, "dump uart statis:\r\n");
	BK_LOGI(TAG, "uart_isr_cnt:     %d\r\n", s_uart_statis[id].uart_isr_cnt);
	BK_LOGI(TAG, "rx_isr_cnt:       %d\r\n", s_uart_statis[id].rx_isr_cnt);
	BK_LOGI(TAG, "rx_fifo_cnt:      %d\r\n", s_uart_statis[id].rx_fifo_cnt);
	BK_LOGI(TAG, "recv_timeout_cnt: %d\r\n", s_uart_statis[id].recv_timeout_cnt);
	BK_LOGI(TAG, "kfifo_size:       %d\r\n", s_uart_statis[id].kfifo_status.size);
	BK_LOGI(TAG, "kfifo_in:         %d\r\n", s_uart_statis[id].kfifo_status.in);
	BK_LOGI(TAG, "kfifo_out:        %d\r\n", s_uart_statis[id].kfifo_status.out);
	BK_LOGI(TAG, "kfifo_last_value: %x\r\n", s_uart_statis[id].kfifo_status.last_value);
	BK_LOGI(TAG, "kfifo_put_cnt:    %d\r\n", s_uart_statis[id].kfifo_status.put_cnt);
	BK_LOGI(TAG, "kfifo_full_cnt:   %d\r\n", s_uart_statis[id].kfifo_status.full_cnt);
	BK_LOGI(TAG, "kfifo_empty_cnt:  %d\r\n", s_uart_statis[id].kfifo_status.empty_cnt);
}

#endif
