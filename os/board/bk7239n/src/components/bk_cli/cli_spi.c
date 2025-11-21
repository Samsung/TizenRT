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

#include <os/os.h>
#include "cli.h"
#include <driver/spi.h>
#include <driver/trng.h>
#include <driver/uart.h>
#include <driver/dma.h>

static void cli_spi_help(void)
{
	CLI_LOGI("spi_driver {init|deinit}\r\n");
	CLI_LOGI("spi {id} {init} [mode] [bit_width] [bit_width] [cpol] [cpha] [wire_mode] [baud_rate] [bit_order]\r\n");
	CLI_LOGI("spi {id} {deinit} \r\n");
	CLI_LOGI("spi {id} {write} [buf_len]\r\n");
	CLI_LOGI("spi_data_test {id} {master|slave} {baud_rate} {start|stop} {uart2|uart3} {exchange}\r\n");
	CLI_LOGI("spi_data_test {id} {master|slave} {send} {buf_len}\r\n");
	CLI_LOGI("spi_flash {id} {readid|erase|read|write} {addr} {len}\r\n");
}

static void cli_spi_rx_isr(spi_id_t id, void *param)
{
	CLI_LOGI("spi_rx_isr(%d)\n", id);
}

static void cli_spi_tx_isr(spi_id_t id, void *param)
{
	CLI_LOGI("spi_tx_isr(%d)\n", id);
}

static void cli_spi_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_spi_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_spi_driver_init());
		CLI_LOGI("spi driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_spi_driver_deinit());
		CLI_LOGI("spi driver deinit\n");
	} else {
		cli_spi_help();
		return;
	}
}

static void cli_spi_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_spi_help();
		return;
	}
	uint32_t spi_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		spi_config_t config = {0};
		if (os_strcmp(argv[3], "master") == 0) {
			config.role = SPI_ROLE_MASTER;
		} else {
			config.role = SPI_ROLE_SLAVE;
		}
		if (os_strtoul(argv[4], NULL, 10) == 8) {
			config.bit_width = SPI_BIT_WIDTH_8BITS;
		} else {
			config.bit_width = SPI_BIT_WIDTH_16BITS;
		}
		config.polarity = os_strtoul(argv[5], NULL, 10);
		config.phase = os_strtoul(argv[6], NULL, 10);
		if (os_strtoul(argv[7], NULL, 10) == 3) {
			config.wire_mode = SPI_3WIRE_MODE;
		} else {
			config.wire_mode = SPI_4WIRE_MODE;
		}
		config.baud_rate = os_strtoul(argv[8], NULL, 10);
		if (os_strcmp(argv[9], "LSB") == 0) {
			config.bit_order = SPI_LSB_FIRST;
		} else {
			config.bit_order = SPI_MSB_FIRST;
		}
#if (defined(CONFIG_SPI_BYTE_INTERVAL))
		config.byte_interval = 1;
#endif
#if defined(CONFIG_SPI_DMA)
		config.dma_mode = os_strtoul(argv[10], NULL, 10);
		config.spi_tx_dma_chan = bk_dma_alloc(DMA_DEV_GSPI0);
		config.spi_rx_dma_chan = bk_dma_alloc(DMA_DEV_GSPI0_RX);
		if (os_strtoul(argv[4], NULL, 10) == 8) {
			config.spi_tx_dma_width = DMA_DATA_WIDTH_8BITS;
			config.spi_rx_dma_width = DMA_DATA_WIDTH_8BITS;
		} else {
			config.spi_tx_dma_width = DMA_DATA_WIDTH_16BITS;
			config.spi_rx_dma_width = DMA_DATA_WIDTH_16BITS;
		}
#endif
		BK_LOG_ON_ERR(bk_spi_init(spi_id, &config));
		CLI_LOGI("spi init, spi_id=%d\n", spi_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_spi_deinit(spi_id));
		CLI_LOGI("spi deinit, spi_id=%d\n", spi_id);
	} else if (os_strcmp(argv[2], "write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *send_data = (uint8_t *)os_zalloc(buf_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		for (int i = 0; i < buf_len; i++) {
			send_data[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_spi_write_bytes(spi_id, send_data, buf_len));
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;
		CLI_LOGI("spi write bytes, spi_id=%d, data_len=%d\n", spi_id, buf_len);
	} else if (os_strcmp(argv[2], "read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		os_memset(recv_data, 0xff, buf_len);
		BK_LOG_ON_ERR(bk_spi_read_bytes(spi_id, recv_data, buf_len));
		CLI_LOGI("spi read, spi_id=%d, size:%d\n", spi_id, buf_len);
		for (int i = 0; i < buf_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	} else if (os_strcmp(argv[2], "transmit") == 0) {
		int send_len = os_strtoul(argv[3], NULL, 10);
		int recv_len = os_strtoul(argv[4], NULL, 10);

		uint8_t *send_data = (uint8_t *)os_zalloc(send_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		for (int i = 0; i < send_len; i++) {
			send_data[i] = i & 0xff;
		}
		uint8_t *recv_data = (uint8_t *)os_malloc(recv_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		os_memset(recv_data, 0xff, recv_len);
		int ret = bk_spi_transmit(spi_id, send_data, send_len, recv_data, recv_len);
		if (ret < 0) {
			CLI_LOGE("spi transmit failed, ret:-0x%x\r\n", -ret);
			goto transmit_exit;
		}
		for (int i = 0; i < recv_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\r\n", i, recv_data[i]);
		}
transmit_exit:
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;

		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	}
#if defined(CONFIG_SPI_DMA)
	else if (os_strcmp(argv[2], "dma_write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *send_data = (uint8_t *)os_zalloc(buf_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		for (int i = 0; i < buf_len; i++) {
			send_data[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_spi_dma_write_bytes(spi_id, send_data, buf_len));
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;
		CLI_LOGI("spi dma send, spi_id=%d, data_len=%d\n", spi_id, buf_len);
	} else if (os_strcmp(argv[2], "dma_read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		os_memset(recv_data, 0xff, buf_len);
		BK_LOG_ON_ERR(bk_spi_dma_read_bytes(spi_id, recv_data, buf_len));
		CLI_LOGI("spi dma recv, spi_id=%d, data_len=%d\n", spi_id, buf_len);
		for (int i = 0; i < buf_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	} else if (os_strcmp(argv[2], "dma_duplex") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		uint8_t *send_data = (uint8_t *)os_malloc(buf_len);
		os_memset(recv_data, 0xff, buf_len);
		for (int i = 0; i < buf_len; i++) {
			send_data[i] = i & 0xff;
		}
		bk_spi_dma_duplex_init(spi_id);
		BK_LOG_ON_ERR(bk_spi_dma_duplex_xfer(spi_id, send_data, buf_len, recv_data, buf_len));
		for (int i = 0; i < buf_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
		bk_spi_dma_duplex_deinit(spi_id);
	}
#endif
	else {
		cli_spi_help();
		return;
	}
}

static void cli_spi_config_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t spi_id;

	if (argc < 4) {
		cli_spi_help();
		return;
	}

	spi_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "baud_rate") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t baud_rate = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_spi_set_baud_rate(spi_id, baud_rate));
		CLI_LOGI("spi(%d) config baud_rate:%d\n", spi_id, baud_rate);
	} else if (os_strcmp(argv[2], "mode") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t mode = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_spi_set_mode(spi_id, mode));
		CLI_LOGI("spi(%d) config mode:%d\n", spi_id, mode);
	} else if (os_strcmp(argv[2], "bit_width") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t bit_width = os_strtoul(argv[3], NULL, 10);
		if (bit_width == 16) {
			BK_LOG_ON_ERR(bk_spi_set_bit_width(spi_id, SPI_BIT_WIDTH_16BITS));
		} else {
			bit_width = 8;
			BK_LOG_ON_ERR(bk_spi_set_bit_width(spi_id, SPI_BIT_WIDTH_8BITS));
		}
		CLI_LOGI("spi(%d) config bit_width:%d\n", spi_id, bit_width);
	} else if (os_strcmp(argv[2], "wire_mode") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t wire_mode = os_strtoul(argv[3], NULL, 10);
		if (wire_mode == 3) {
			BK_LOG_ON_ERR(bk_spi_set_wire_mode(spi_id, SPI_3WIRE_MODE));
		} else {
			BK_LOG_ON_ERR(bk_spi_set_wire_mode(spi_id, SPI_4WIRE_MODE));
		}
		CLI_LOGI("spi(%d) config wire_mode:%d\n", spi_id, wire_mode);
	} else if (os_strcmp(argv[2], "bit_order") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		if (os_strcmp(argv[3], "LSB") == 0) {
			BK_LOG_ON_ERR(bk_spi_set_bit_order(spi_id, SPI_LSB_FIRST));
		} else {
			BK_LOG_ON_ERR(bk_spi_set_bit_order(spi_id, SPI_MSB_FIRST));
		}
		CLI_LOGI("spi(%d) config bit_order:%s\n", spi_id, argv[3]);
	} else {
		cli_spi_help();
		return;
	}
}

static void cli_spi_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t spi_id;

	if (argc != 4) {
		cli_spi_help();
		return;
	}

	spi_id = os_strtoul(argv[1], NULL, 10);
	if (os_strcmp(argv[2], "reg") == 0) {
		if (os_strcmp(argv[3], "tx") == 0) {
			BK_LOG_ON_ERR(bk_spi_register_tx_finish_isr(spi_id, cli_spi_tx_isr, NULL));
			CLI_LOGI("spi id:%d register tx finish interrupt isr\n", spi_id);
		} else {
			BK_LOG_ON_ERR(bk_spi_register_rx_isr(spi_id, cli_spi_rx_isr, NULL));
			CLI_LOGI("spi id:%d register rx interrupt isr\n", spi_id);
		}
	} else {
		cli_spi_help();
		return;
	}
}

typedef struct {
	beken_thread_t handle;
	beken_semaphore_t uart_rx_semaphore;
	beken_semaphore_t spi_tx_finish_semaphore;
	spi_role_t role;
	uint32_t baud_rate;
	spi_id_t spi_id;
	uart_id_t uart_id;
	uint32_t data_len;
	uint8_t *data;
	uint8_t *uart_get_data;
	uint8_t *spi_get_buffer;
	dma_id_t spi_tx_dma_chan;  /**< SPI tx dma channel */
	dma_id_t spi_rx_dma_chan;  /**< SPI rx dma channel */
	bool exchange_flag;

} spi_data_test_config_t;

static spi_data_test_config_t s_spi_test;
#define PER_PACKET_MAX_BYTES_SIZE   256

static uint32_t spi_data_test_send(spi_id_t id, uint32_t buf_len)
{
	bool middle_exchange_flag = s_spi_test.exchange_flag;

	while(buf_len > PER_PACKET_MAX_BYTES_SIZE){

		s_spi_test.data = (uint8_t *)os_zalloc(PER_PACKET_MAX_BYTES_SIZE);
		if(s_spi_test.data == NULL) {
			CLI_LOGE("os_zalloc PER_PACKET_MAX_BYTES_SIZE:%u \r\n", PER_PACKET_MAX_BYTES_SIZE);
			return BK_FAIL;
		}
		s_spi_test.exchange_flag = 0;
		s_spi_test.data_len = PER_PACKET_MAX_BYTES_SIZE;

		for (int i = 0; i < PER_PACKET_MAX_BYTES_SIZE; i++) {
#if (defined(CONFIG_TRNG_SUPPORT))
			s_spi_test.data[i] = bk_rand();
#else
			s_spi_test.data[i] = i & 0xff;
#endif
	}

		BK_LOG_ON_ERR(bk_spi_write_bytes(id, s_spi_test.data, PER_PACKET_MAX_BYTES_SIZE));
		buf_len = buf_len - PER_PACKET_MAX_BYTES_SIZE;
	}

	s_spi_test.data = (uint8_t *)os_zalloc(buf_len);
	if(s_spi_test.data == NULL) {
		CLI_LOGE("os_zalloc buf_len:%u \r\n", buf_len);
		return BK_FAIL;
	}
	s_spi_test.exchange_flag = middle_exchange_flag;
	s_spi_test.data_len = buf_len;

	for (int i = 0; i < buf_len; i++) {
#if (defined(CONFIG_TRNG_SUPPORT))
		s_spi_test.data[i] = bk_rand();
#else
		s_spi_test.data[i] = i & 0xff;
#endif
	}

	BK_LOG_ON_ERR(bk_spi_write_bytes(id, s_spi_test.data, buf_len));
	CLI_LOGI("spi MASTER write bytes, spi_id=%d, data_len=%d\n", id, buf_len);

	return BK_OK;
}

static void spi_data_test_tx_finish_isr(uart_id_t id, void *param)
{
	int ret;

	ret = rtos_set_semaphore(&s_spi_test.spi_tx_finish_semaphore);
	if(kNoErr !=ret)
		CLI_LOGI("spi_data_test_Tx_isr: spi set sema failed\r\n");

	return;
}

static void spi_data_test_spi_config(spi_id_t id, spi_role_t role, uint32_t baud_rate)
{
	spi_config_t config = {0};
	config.role = role;
	config.bit_width = SPI_BIT_WIDTH_8BITS;
	config.polarity = 1;
	config.phase = 1;
	config.wire_mode = SPI_4WIRE_MODE;
	config.baud_rate = baud_rate;
	config.bit_order = SPI_MSB_FIRST;
#if (defined(CONFIG_SPI_BYTE_INTERVAL))
	config.byte_interval = 1;
#endif

#if defined(CONFIG_SPI_DMA)
	config.dma_mode = 1;
	s_spi_test.spi_tx_dma_chan = bk_dma_alloc(DMA_DEV_DTCM);
	config.spi_tx_dma_chan = s_spi_test.spi_tx_dma_chan;
	s_spi_test.spi_rx_dma_chan = bk_dma_alloc(DMA_DEV_DTCM);
	config.spi_rx_dma_chan = s_spi_test.spi_rx_dma_chan;
	config.spi_tx_dma_width = DMA_DATA_WIDTH_8BITS;
	config.spi_rx_dma_width = DMA_DATA_WIDTH_8BITS;
#endif

	if(role == SPI_ROLE_MASTER)
		BK_LOG_ON_ERR(bk_spi_register_tx_finish_isr(id, (spi_isr_t)spi_data_test_tx_finish_isr, NULL));

	BK_LOG_ON_ERR(bk_spi_init(id, &config));

}

static void spi_data_test_spi_deconfig(spi_id_t id)
{
	BK_LOG_ON_ERR(bk_spi_unregister_tx_finish_isr(id));
	BK_LOG_ON_ERR(bk_spi_deinit(id));
	bk_dma_free(DMA_DEV_DTCM, s_spi_test.spi_tx_dma_chan);
	bk_dma_free(DMA_DEV_DTCM, s_spi_test.spi_rx_dma_chan);
}

static void spi_data_test_exchange_role(void)
{
	spi_data_test_spi_deconfig(s_spi_test.spi_id);

	if(s_spi_test.role == SPI_ROLE_MASTER)
		s_spi_test.role = SPI_ROLE_SLAVE;
	else
		s_spi_test.role = SPI_ROLE_MASTER;

	spi_data_test_spi_config(s_spi_test.spi_id, s_spi_test.role, s_spi_test.baud_rate);

}

static void cli_spi_data_txrx_test(uint32_t data)
{
	int ret = -1;

	int spi_tx_finish_isr_ret = -1;
	int uart_isr_ret = -1;
	int cnt = 0;
	uint8_t rx_data;

	if(NULL == s_spi_test.uart_rx_semaphore)
	{
		ret = rtos_init_semaphore(&s_spi_test.uart_rx_semaphore, 1);
		if (kNoErr != ret)
			CLI_LOGI("s_spi_test.uart_rx_semaphore failed\r\n");
	}

	if(NULL == s_spi_test.spi_tx_finish_semaphore)
	{
		ret = rtos_init_semaphore(&s_spi_test.spi_tx_finish_semaphore, 1);
		if (kNoErr != ret)
			CLI_LOGI("s_spi_test.spi_tx_finish_semaphore failed\r\n");
	}

	CLI_LOGI("cli_spi_data_txrx_test\r\n");

	while (1) {

		if (s_spi_test.role == SPI_ROLE_MASTER) {

			spi_tx_finish_isr_ret = rtos_get_semaphore(&s_spi_test.spi_tx_finish_semaphore, BEKEN_WAIT_FOREVER);
			if(kNoErr == spi_tx_finish_isr_ret) {
				rtos_delay_milliseconds(10);
				if (s_spi_test.data) {
					BK_LOG_ON_ERR(bk_uart_write_bytes(s_spi_test.uart_id, s_spi_test.data, s_spi_test.data_len));
					for(int j = 0; j < s_spi_test.data_len; j++) {
						CLI_LOGI("s_spi_test.data[%d]: %d\r\n", j, s_spi_test.data[j]);
					}
					CLI_LOGI("spi Tx finish! UART send OK!\r\n");

					os_free(s_spi_test.data);
					s_spi_test.data = NULL;
				}

				if(s_spi_test.exchange_flag) {
					spi_data_test_exchange_role();
					CLI_LOGI("spi EXCHANGE ROLE! MASTER ==> SLAVE\r\n");
				}
			}
		} else {

			uart_isr_ret = rtos_get_semaphore(&s_spi_test.uart_rx_semaphore, 300);
			if(kNoErr == uart_isr_ret) {
				while(1)  /* read all data from rx-FIFO. */
				{
					ret = uart_read_byte_ex(s_spi_test.uart_id, &rx_data);
					if (ret == -1)
						break;
					s_spi_test.uart_get_data[cnt] = rx_data;
					cnt++;
					if(cnt >= PER_PACKET_MAX_BYTES_SIZE)
						break;
				}

				if (cnt <= 0)
					break;

				if (cnt > 0) {
					s_spi_test.data_len = cnt;
					for(int i = 0; i < cnt; i++) {
						if(s_spi_test.spi_get_buffer[i] != s_spi_test.uart_get_data[i])
							CLI_LOGI("ERROR spi_get_buffer[%d]: %d uart_get_data[%d]: %d\r\n",
									 i, s_spi_test.spi_get_buffer[i], i, s_spi_test.uart_get_data[i]);
						else
							CLI_LOGI("OK !!!spi get data == uart get data\r\n");

						s_spi_test.spi_get_buffer[i] = 0;
						s_spi_test.uart_get_data[i] = 0;
					}
					cnt = 0;
				}

				if(s_spi_test.exchange_flag) {
					spi_data_test_exchange_role();
					CLI_LOGI("spi EXCHANGE ROLE! SLAVE ==> MASTER\r\n");
				}
			}else {
				CLI_LOGI("BEKEN_WAIT_FOREVER for spi read\r\n");
				BK_LOG_ON_ERR(bk_spi_read_bytes(s_spi_test.spi_id, s_spi_test.spi_get_buffer, s_spi_test.data_len));
			}
		}

	}

}

static void spi_data_test_uart_rx_isr(uart_id_t id, void *param)
{
	int ret;

	ret = rtos_set_semaphore(&s_spi_test.uart_rx_semaphore);
	if(kNoErr !=ret)
		CLI_LOGI("spi_data_test_uart_rx_isr: uart set sema failed\r\n");
	return;
}

static void spi_data_test_uart_config(void)
{
	uart_config_t config = {0};
	os_memset(&config, 0, sizeof(uart_config_t));
	config.baud_rate = UART_BAUD_RATE;
	config.data_bits = UART_DATA_8_BITS;
	config.parity = UART_PARITY_NONE;
	config.stop_bits = UART_STOP_BITS_1;
	config.flow_ctrl = UART_FLOWCTRL_DISABLE;
	config.src_clk = UART_SCLK_XTAL_26M;

	BK_LOG_ON_ERR(bk_uart_init(s_spi_test.uart_id, &config));
	BK_LOG_ON_ERR(bk_uart_disable_sw_fifo(s_spi_test.uart_id));
	BK_LOG_ON_ERR(bk_uart_register_rx_isr(s_spi_test.uart_id,
				  (uart_isr_t)spi_data_test_uart_rx_isr, NULL));
	BK_LOG_ON_ERR(bk_uart_enable_rx_interrupt(s_spi_test.uart_id));

}

static void spi_data_test_uart_deconfig(void)
{
	BK_LOG_ON_ERR(bk_uart_disable_rx_interrupt(s_spi_test.uart_id));
	BK_LOG_ON_ERR(bk_uart_register_rx_isr(s_spi_test.uart_id, NULL, NULL));
	BK_LOG_ON_ERR(bk_uart_deinit(s_spi_test.uart_id));
	s_spi_test.uart_id = UART_ID_MAX;

}

static void cli_spi_data_txrx_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 4) {
		cli_spi_help();
		return;
	}

	spi_id_t spi_id = os_strtoul(argv[1], NULL, 10);
	spi_role_t role;
	uint32_t baud_rate;

	if (os_strcmp(argv[2], "master") == 0) {
			role = SPI_ROLE_MASTER;
	} else if(os_strcmp(argv[2], "slave") == 0){
			role = SPI_ROLE_SLAVE;
	} else {
		cli_spi_help();
		return;
	}

	baud_rate = os_strtoul(argv[3], NULL, 10);

	if (os_strcmp(argv[4], "start") == 0) {
		s_spi_test.spi_id = spi_id;
		s_spi_test.role = role;
		s_spi_test.baud_rate = baud_rate;
		s_spi_test.uart_get_data = (uint8_t *)os_zalloc(PER_PACKET_MAX_BYTES_SIZE);
		s_spi_test.spi_get_buffer = (uint8_t *)os_zalloc(PER_PACKET_MAX_BYTES_SIZE);

		if (!s_spi_test.handle) {
			if (os_strcmp(argv[5], "uart2")== 0) {
				s_spi_test.uart_id = UART_ID_1;
				CLI_LOGD("cli_spi_test Maybe UART2 for log output!!!\n");
			} else if (os_strcmp(argv[5], "uart3")== 0) {
				s_spi_test.uart_id = UART_ID_2;
				CLI_LOGD("cli_spi_test Maybe UART3 for log output!!!\n");
			} else {
					cli_spi_help();
					return;
			}
		} else {
			CLI_LOGI("spi_data_test TASK WARKING!!!\n");
			return;
		}

		if (os_strcmp(argv[6], "exchange") == 0)
			s_spi_test.exchange_flag = 1;
		else
			s_spi_test.exchange_flag = 0;

		spi_data_test_uart_config();
		spi_data_test_spi_config(spi_id, role, baud_rate);

#if (defined(CONFIG_TRNG_SUPPORT))
		BK_LOG_ON_ERR(bk_trng_driver_init());
		BK_LOG_ON_ERR(bk_trng_start());
#endif

		if(rtos_create_thread(&s_spi_test.handle, 8, "spi_data_test",
					(beken_thread_function_t) cli_spi_data_txrx_test, 2048, 0)) {
				CLI_LOGI("spi_data_txrx_test rtos_create_thread FAILED!\n");
				return;
		}

	}

	if (os_strcmp(argv[4], "stop") == 0) {
		if (os_strcmp(argv[5], "uart2")== 0) {
			if(s_spi_test.uart_id != UART_ID_1) {
				CLI_LOGI("PLEASE enter a correct ID\n");
				return;
			} else
				s_spi_test.uart_id = UART_ID_1;
		} else if (os_strcmp(argv[5], "uart3")== 0) {
			if(s_spi_test.uart_id != UART_ID_2) {
				CLI_LOGI("PLEASE enter a correct ID\n");
				return;
			} else
				s_spi_test.uart_id = UART_ID_2;
		} else {
			cli_spi_help();
			return;
		}

		rtos_delete_thread(&s_spi_test.handle);
		s_spi_test.handle = NULL;
		spi_data_test_uart_deconfig();

		if(s_spi_test.spi_id != spi_id) {
			cli_spi_help();
			return;
		} else
			spi_data_test_spi_deconfig(spi_id);

		if(NULL != s_spi_test.uart_get_data)
			os_free(s_spi_test.uart_get_data);

		if(NULL != s_spi_test.spi_get_buffer)
			os_free(s_spi_test.spi_get_buffer);
		CLI_LOGI("cli_spi_data_txrx_test task stop\n");
	}

	if (os_strcmp(argv[3], "send") == 0) {
		uint32_t buf_len = os_strtoul(argv[4], NULL, 10);
		if(s_spi_test.spi_id != spi_id) {
			cli_spi_help();
			return;
		} else {
			if(s_spi_test.role == SPI_ROLE_MASTER) {
				spi_data_test_send(spi_id, buf_len);
			} else {
				CLI_LOGI("PLEASE use master send\n");
			}
		}
	}
}

static void cli_spi_flash_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_spi_help();
		return;
	}
	uint32_t spi_id = os_strtoul(argv[1], NULL, 10);
	CLI_LOGI("spi_id:%08x\r\n",spi_id);

#ifdef CONFIG_SPI_MST_FLASH
	extern uint32_t spi_flash_read_id(void);
	extern int spi_flash_read(uint32_t addr, uint32_t size, uint8_t *dst);
	extern int spi_flash_write(uint32_t addr, uint32_t size, uint8_t *src);
	extern int spi_flash_erase(uint32_t addr, uint32_t size);
	if (os_strcmp(argv[2], "readid") == 0) {
		spi_flash_read_id();
		return;
	}
	if (argc < 5) {
		cli_spi_help();
		return;
	}
	if (os_strcmp(argv[2], "read") == 0) {
		uint32_t read_addr = os_strtoul(argv[3], NULL, 16);
		uint32_t read_len = os_strtoul(argv[4], NULL, 16);
		uint32_t read_times = 1;
		if (argc >= 6) {
			read_times = os_strtoul(argv[5], NULL, 10);
		}
		CLI_LOGI("read_addr:%08x,read_len:%d, read_times:%d\r\n",read_addr,read_len,read_times);

		uint8_t *buf = (uint8_t *)os_zalloc(read_len);
		if(buf == NULL) {
			CLI_LOGE(" buf os_zalloc read_len:%u \r\n", read_len);
			return;
		}
		for (int i = 0; i < read_times; i++)
			spi_flash_read(read_addr,read_len,buf);
		for(int i=0;i < read_len;i++) {
			os_printf("%02x ",buf[i]);
			if(0 == (i+1)%16)
				os_printf("\r\n");
		}

		if (buf) {
			os_free(buf);
		}
		buf = NULL;
		CLI_LOGI("spi_flash_read finish\r\n");
	} else if (os_strcmp(argv[2], "erase") == 0) {
		uint32_t erase_addr = os_strtoul(argv[3], NULL, 16);
		uint32_t size = os_strtoul(argv[4], NULL, 16);
		CLI_LOGI("erase_addr:%08x,size:%d\r\n",erase_addr);
		spi_flash_erase(erase_addr,size);
		CLI_LOGI("spi_flash_erase finish\r\n");
	} else if (os_strcmp(argv[2], "write") == 0) {
		uint32_t write_addr = os_strtoul(argv[3], NULL, 16);
		uint32_t size = os_strtoul(argv[4], NULL, 16);
		CLI_LOGI("write_addr:%08x,size:%d\r\n",write_addr,size);
		uint32_t page_size = 256;
		uint8_t *buf = (uint8_t *)os_zalloc(page_size);
		if(buf == NULL) {
			CLI_LOGE(" buf os_zalloc page_size:%u \r\n", page_size);
			return;
		}
		for (uint32_t i = 0; i < page_size; i++) {
			buf[i] = i;
		}
		for (uint32_t addr = write_addr; addr < (write_addr + size); addr += page_size) {
			spi_flash_write(addr, page_size, buf);
		}
		if (buf) {
			os_free(buf);
		}
		buf = NULL;

		CLI_LOGI("spi_flash_write finish\r\n");
	} else {
		cli_spi_help();
	}
#endif
	return;
}

#define SPI_CMD_CNT (sizeof(s_spi_commands) / sizeof(struct cli_command))
static const struct cli_command s_spi_commands[] = {
	{"spi_driver", "spi_driver {init|deinit}", cli_spi_driver_cmd},
	{"spi", "spi {init|write|read}", cli_spi_cmd},
	{"spi_config", "spi_config {id} {mode|baud_rate} [...]", cli_spi_config_cmd},
	{"spi_int", "spi_int {id} {reg} {tx|rx}", cli_spi_int_cmd},
	{"spi_data_test", "spi_data_test {id} {master|slave} {baud_rate|send}[...]", cli_spi_data_txrx_test_cmd},
	{"spi_flash", "spi_flash {id} {readid|read|write|erase} {addr} {len}[...]", cli_spi_flash_cmd},
};

int cli_spi_init(void)
{
	BK_LOG_ON_ERR(bk_spi_driver_init());
	return cli_register_commands(s_spi_commands, SPI_CMD_CNT);
}
