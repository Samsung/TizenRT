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

#include <os/mem.h>
#include "spi_hw.h"
#include "spi_statis.h"

#if defined(CONFIG_SPI_STATIS)

#define TAG "spi_statis"

static spi_statis_t s_spi_statis[SOC_SPI_UNIT_NUM] = {0};

bk_err_t spi_statis_init(void)
{
	os_memset(&s_spi_statis, 0, sizeof(s_spi_statis));
	return BK_OK;
}

spi_statis_t* spi_statis_get_statis(spi_id_t id)
{
	return &(s_spi_statis[id]);
}

void spi_statis_dump(spi_id_t id)
{
	BK_LOGI(TAG, "dump spi statis:\r\n");
	BK_LOGI(TAG, "spi_isr_cnt:           %d\r\n", s_spi_statis[id].spi_isr_cnt);
	BK_LOGI(TAG, "tx_fifo_isr_cnt:       %d\r\n", s_spi_statis[id].tx_fifo_isr_cnt);
	BK_LOGI(TAG, "rx_fifo_isr_cnt:       %d\r\n", s_spi_statis[id].rx_fifo_isr_cnt);
	BK_LOGI(TAG, "tx_finish_isr_cnt:     %d\r\n", s_spi_statis[id].tx_finish_isr_cnt);
	BK_LOGI(TAG, "rx_finish_isr_cnt:     %d\r\n", s_spi_statis[id].rx_finish_isr_cnt);
	BK_LOGI(TAG, "tx_underflow_isr_cnt:  %d\r\n", s_spi_statis[id].tx_underflow_isr_cnt);
	BK_LOGI(TAG, "rx_overflow_isr_cnt:   %d\r\n", s_spi_statis[id].rx_overflow_isr_cnt);
	BK_LOGI(TAG, "slave_release_isr_cnt: %d\r\n", s_spi_statis[id].slave_release_isr_cnt);
}

#endif
