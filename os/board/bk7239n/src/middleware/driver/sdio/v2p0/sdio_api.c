// Copyright 2022-2023 Beken
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
#include "string.h"
#include <common/bk_include.h>
#include "sdio_utils.h"
#include "sdio_slave_driver.h"
#include "sdio_test.h"
#include "sdio_api.h"


int32_t sdio_dev_send_msg (const uint8_t *p_packet, uint32_t n_bytes, uint32_t timeout_ms)
{
	bk_sdio_slave_tx((char*) p_packet, n_bytes, timeout_ms);
}

void sdio_dev_register_tx_callback(sdiod_tx_callback_t callback, void *para)
{
	//this tx callback can be called when bk_sdio_slave_tx return ok
}

void sdio_dev_register_rx_done_callback(sdiod_rx_done_callback_t callback, void *para)
{
	bk_sdio_register_rx_cb((sdio_rx_cb_t) callback);
}

uint32_t sdio_init(void)
{
	bk_sdio_slave_driver_init();
}

void sdio_deinit(void)
{
	sdio_slave_driver_deinit();
}

int sdio_host_is_ready(void)
{
	///TODO:
	return 0;
}

