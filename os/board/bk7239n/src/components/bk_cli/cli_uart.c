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
#include <driver/uart.h>
#include <driver/trng.h>
#include "uart_statis.h"
#include "bk_misc.h"

#define CLI_UART_RECV_BUF_LEN  1024

static void cli_uart_help(void)
{
	CLI_LOGI("uart_driver init\n");
	CLI_LOGI("uart_driver deinit\n");
	CLI_LOGI("uart {id} {init|deinit|write|read|write_string|dump_statis} [...]\n");
	CLI_LOGI("uart {id} {init} [baud_rate][data_bits:0~3 means 5~8bits][parity:0 none,1 odd, 2 even][stopbits:0 means 1bit, 1 means 2bits][flow ctrl]\n");
	CLI_LOGI("uart_int {id} {enable|disable|reg} {tx|rx}\n");
	CLI_LOGI("uart_test {idle_start|idle_stop} {uart1|uart2|uart3}\n");
}

static void cli_uart_driver_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_uart_help();
		return;
	}

	if (os_strcmp(argv[1], "init") == 0) {
		BK_LOG_ON_ERR(bk_uart_driver_init());
		CLI_LOGI("uart driver init\n");
	} else if (os_strcmp(argv[1], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_uart_driver_deinit());
		CLI_LOGI("uart driver deinit\n");
	} else {
		cli_uart_help();
		return;
	}
}

static void cli_uart_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t uart_id;

	if (argc < 2) {
		cli_uart_help();
		return;
	}

	uart_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		uart_config_t config = {0};
		os_memset(&config, 0, sizeof(uart_config_t));
		config.baud_rate = os_strtoul(argv[3], NULL, 10);
		config.data_bits = os_strtoul(argv[4], NULL, 10);
		config.parity = os_strtoul(argv[5], NULL, 10);
		config.stop_bits = os_strtoul(argv[6], NULL, 10);
		if (argc > 7) {
			config.flow_ctrl = os_strtoul(argv[7], NULL, 10);
		}
		if (argc > 8) {
			config.src_clk = os_strtoul(argv[8], NULL, 10);
		}
		if (argc > 9) {
			config.rx_dma_en = os_strtoul(argv[9], NULL, 10);
		}
		if (argc > 10) {
			config.tx_dma_en = os_strtoul(argv[10], NULL, 10);
		}

		BK_LOG_ON_ERR(bk_uart_init(uart_id, &config));
		BK_LOG_ON_ERR(bk_uart_enable_rx_interrupt(uart_id));
		CLI_LOGI("uart init, uart_id=%d\n", uart_id);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_uart_deinit(uart_id));
		CLI_LOGI("uart deinit, uart_id=%d\n", uart_id);
	} else if (os_strcmp(argv[2], "write") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *send_data = (uint8_t *)os_malloc(buf_len);
		if (send_data == NULL) {
			CLI_LOGE("send buffer malloc failed\r\n");
			return;
		}
		os_memset(send_data, 0, buf_len);
		for (int i = 0; i < buf_len; i++) {
			send_data[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_uart_write_bytes(uart_id, send_data, buf_len));
		if (send_data) {
			os_free(send_data);
		}
		send_data = NULL;
		CLI_LOGI("uart write, uart_id=%d, data_len:%d\n", uart_id, buf_len);
	} else if (os_strcmp(argv[2], "read") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		int time_out = os_strtoul(argv[4], NULL, 10);
		if (time_out < 0) {
			time_out = BEKEN_WAIT_FOREVER;
		}
		int data_len = bk_uart_read_bytes(uart_id, recv_data, buf_len, time_out);
		if (data_len < 0) {
			CLI_LOGE("uart read failed, ret:-0x%x\r\n", -data_len);
			goto exit;
		}
		CLI_LOGI("uart read, uart_id=%d, time_out:%x data_len:%d\n", uart_id, time_out, data_len);
		for (int i = 0; i < data_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
exit:
		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	} else if (os_strcmp(argv[2], "write_string") == 0) {
		char send_data[] = "beken uart write string test\r\n";
		BK_LOG_ON_ERR(bk_uart_write_bytes(uart_id, send_data, os_strlen(send_data)));
		CLI_LOGI("uart write string, uart_id=%d, data_len:%d\n", uart_id, os_strlen(send_data));
	}
#if defined(CONFIG_UART_STATIS)
	else if (os_strcmp(argv[2], "dump_statis") == 0) {
		uart_statis_dump(uart_id);
		CLI_LOGI("uart dump statis ok\r\n");
	} else if (os_strcmp(argv[2], "reset_statis") == 0) {
		uart_statis_id_init(uart_id);
		CLI_LOGI("uart reset statis ok\r\n");
	}
#endif
	else {
		cli_uart_help();
		return;
	}
}

static void cli_uart_config_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t uart_id;

	if (argc < 4) {
		cli_uart_help();
		return;
	}

	uart_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "baud_rate") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t baud_rate = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_baud_rate(uart_id, baud_rate));
		CLI_LOGI("uart(%d) config baud_rate:%d\n", uart_id, baud_rate);
	} else if (os_strcmp(argv[2], "data_bits") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t data_bits = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_data_bits(uart_id, data_bits));
		CLI_LOGI("uart(%d) config data_bits:%d\n", uart_id, data_bits);
	} else if (os_strcmp(argv[2], "stop_bits") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t stop_bits = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_stop_bits(uart_id, stop_bits));
		CLI_LOGI("uart(%d) config stop_bits:%d\n", uart_id, stop_bits);
	} else if (os_strcmp(argv[2], "parity") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t parity = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_parity(uart_id, parity));
		CLI_LOGI("uart(%d) config parity:%d\n", uart_id, parity);
	} else if (os_strcmp(argv[2], "flow_ctrl") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t rx_threshold = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_hw_flow_ctrl(uart_id, rx_threshold));
		CLI_LOGI("uart(%d) config flow_ctrl:%d\n", uart_id, rx_threshold);
	} else if (os_strcmp(argv[2], "rx_thresh") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t rx_thresh = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_rx_full_threshold(uart_id, rx_thresh));
		CLI_LOGI("uart(%d) config rx_thresh:%d\n", uart_id, rx_thresh);
	} else if (os_strcmp(argv[2], "tx_thresh") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t tx_thresh = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_tx_empty_threshold(uart_id, tx_thresh));
		CLI_LOGI("uart(%d) config tx_thresh:%d\n", uart_id, tx_thresh);
	} else if (os_strcmp(argv[2], "rx_timeout") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 4);
		uint32_t timeout_thresh = os_strtoul(argv[3], NULL, 10);
		BK_LOG_ON_ERR(bk_uart_set_rx_timeout(uart_id, timeout_thresh));
		CLI_LOGI("uart(%d) config rx_timeout:%d\n", uart_id, timeout_thresh);
	} else {
		cli_uart_help();
		return;
	}
}

static void cli_uart_rx_isr(uart_id_t id, void *param)
{
	CLI_LOGI("uart_rx_isr(%d)\n", id);
}

static void cli_uart_tx_isr(uart_id_t id, void *param)
{
	CLI_LOGI("uart_tx_isr(%d)\n", id);
}

static void cli_uart_int_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t uart_id;

	if (argc != 4) {
		cli_uart_help();
		return;
	}

	uart_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "enable") == 0) {
		if (os_strcmp(argv[3], "tx") == 0) {
			bk_uart_set_enable_tx(uart_id, 1);
			BK_LOG_ON_ERR(bk_uart_enable_tx_interrupt(uart_id));
			CLI_LOGI("uart id:%d enable tx interrupt\n", uart_id);
		} else {
			bk_uart_set_enable_rx(uart_id, 1);
			BK_LOG_ON_ERR(bk_uart_enable_rx_interrupt(uart_id));
			CLI_LOGI("uart id:%d enable rx interrupt\n", uart_id);
		}
	} else if (os_strcmp(argv[2], "disable") == 0) {
		if (os_strcmp(argv[3], "tx") == 0) {
			BK_LOG_ON_ERR(bk_uart_disable_tx_interrupt(uart_id));
			CLI_LOGI("uart id:%d disable tx interrupt\n", uart_id);
		} else {
			BK_LOG_ON_ERR(bk_uart_disable_rx_interrupt(uart_id));
			CLI_LOGI("uart id:%d disable rx interrupt\n", uart_id);
		}
	} else if (os_strcmp(argv[2], "reg") == 0) {
		if (os_strcmp(argv[3], "tx") == 0) {
			BK_LOG_ON_ERR(bk_uart_register_tx_isr(uart_id, cli_uart_tx_isr, NULL));
			CLI_LOGI("uart id:%d register tx interrupt isr\n", uart_id);
		} else {
			BK_LOG_ON_ERR(bk_uart_register_rx_isr(uart_id, cli_uart_rx_isr, NULL));
			CLI_LOGI("uart id:%d register rx interrupt isr\n", uart_id);
		}
	} else {
		cli_uart_help();
		return;
	}
}

#if defined(CONFIG_IDLE_UART_OUT_TEST)
static beken_thread_t idle_uart_out_test_handle = NULL;
static uint16_t idle_uart_out_test_id = 0;
static void cli_idle_uart_out_test_isr(uart_id_t id, void *param)
{
	return;
}

static void cli_idle_uart_out_test(void *arg)
{
	while (1) {
		unsigned long random;
		char tx_buffer[16] = {0};

		random = bk_rand();
		itoa(random, tx_buffer, 14);
		tx_buffer[15] = '\0';
		uart_write_string(idle_uart_out_test_id, tx_buffer);

	}
	rtos_delete_thread(&idle_uart_out_test_handle);
}

static void cli_uart_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_uart_help();
		return;
	}

	if (os_strcmp(argv[1], "idle_start") == 0) {
		if (!idle_uart_out_test_handle) {
			if (os_strcmp(argv[2], "uart1") == 0) {
#if (CONFIG_UART_PRINT_PORT != 0)
				idle_uart_out_test_id = UART_ID_0;
				CLI_LOGI("idle_uart_out task start: uart_id = UART1\n" );

#else
				CLI_LOGI("cli_uart_test_cmd UART1 for log output!!!\n");
				return;
#endif
			} else if (os_strcmp(argv[2], "uart2")== 0) {
#if (CONFIG_UART_PRINT_PORT != 1)
				idle_uart_out_test_id = UART_ID_1;
				CLI_LOGI("idle_uart_out task start: uart_id = UART2\n" );

#else
				CLI_LOGI("cli_uart_test_cmd UART2 for log output!!!\n");
				return;
#endif
			} else if (os_strcmp(argv[2], "uart3")== 0) {
#if (CONFIG_UART_PRINT_PORT != 2)
				idle_uart_out_test_id = UART_ID_2;
				CLI_LOGI("idle_uart_out task start: uart_id = UART3\n" );

#else
				CLI_LOGI("cli_uart_test_cmd UART3 for log output!!!\n");
				return;
#endif

			} else {
				cli_uart_help();
				return;
			}

			uart_config_t config = {0};
			os_memset(&config, 0, sizeof(uart_config_t));

			config.baud_rate = UART_BAUD_RATE;
			config.data_bits = UART_DATA_8_BITS;
			config.parity = UART_PARITY_NONE;
			config.stop_bits = UART_STOP_BITS_1;
			config.flow_ctrl = UART_FLOWCTRL_DISABLE;
			config.src_clk = UART_SCLK_XTAL_26M;

			BK_LOG_ON_ERR(bk_uart_init(idle_uart_out_test_id, &config));
			BK_LOG_ON_ERR(bk_uart_deinit(idle_uart_out_test_id));

			BK_LOG_ON_ERR(bk_uart_register_tx_isr(idle_uart_out_test_id, cli_idle_uart_out_test_isr, NULL));
			BK_LOG_ON_ERR(bk_uart_enable_tx_interrupt(idle_uart_out_test_id));
			BK_LOG_ON_ERR(bk_uart_init(idle_uart_out_test_id, &config));
			BK_LOG_ON_ERR(bk_trng_driver_init());
			BK_LOG_ON_ERR(bk_trng_start());
			if(rtos_create_thread(&idle_uart_out_test_handle, 8, "idle_uart_out",
					(beken_thread_function_t) cli_idle_uart_out_test, 2048, 0)) {
				CLI_LOGI("cli_uart_test_cmd rtos_create_thread FAILED!\n");
				return;
			}
		}else {
			CLI_LOGI("PLEASE stop the task\n");
		}
		return;
	} else if (os_strcmp(argv[1], "idle_stop") == 0) {

		if (idle_uart_out_test_handle) {
			if (os_strcmp(argv[2], "uart1") == 0) {
				if(idle_uart_out_test_id != UART_ID_0) {
					CLI_LOGI("PLEASE enter a correct ID\n");
					return;
				} else
					idle_uart_out_test_id = UART_ID_0;
			} else if (os_strcmp(argv[2], "uart2")== 0) {
				if(idle_uart_out_test_id != UART_ID_1) {
					CLI_LOGI("PLEASE enter a correct ID\n");
					return;
				} else
					idle_uart_out_test_id = UART_ID_1;
			} else if (os_strcmp(argv[2], "uart3")== 0) {
				if(idle_uart_out_test_id != UART_ID_2) {
					CLI_LOGI("PLEASE enter a correct ID\n");
					return;
				} else
					idle_uart_out_test_id = UART_ID_2;
			} else {
				cli_uart_help();
				return;
			}

			rtos_delete_thread(&idle_uart_out_test_handle);
			idle_uart_out_test_handle = NULL;
			BK_LOG_ON_ERR(bk_uart_disable_tx_interrupt(idle_uart_out_test_id));
			BK_LOG_ON_ERR(bk_uart_register_tx_isr(idle_uart_out_test_id, NULL, NULL));
			BK_LOG_ON_ERR(bk_uart_deinit(idle_uart_out_test_id));
			BK_LOG_ON_ERR(bk_trng_stop());
			CLI_LOGI("idle_uart_out task stop\n");
		} else {
			CLI_LOGI("PLEASE start task FIRST!!!\n");
		}
		return;
	}

}
#endif //CONFIG_IDLE_UART_OUT_TEST

#define UART_CMD_CNT (sizeof(s_uart_commands) / sizeof(struct cli_command))
static const struct cli_command s_uart_commands[] = {
	{"uart_driver", "{init|deinit}", cli_uart_driver_cmd},
	{"uart", "uart {id} {init|deinit|write|read|write_string|dump_statis} [...]", cli_uart_cmd},
	{"uart_config", "uart_config {id} {baud_rate|data_bits} [...]", cli_uart_config_cmd},
	{"uart_int", "uart_int {id} {enable|disable|reg} {tx|rx}", cli_uart_int_cmd},
#if defined(CONFIG_IDLE_UART_OUT_TEST)
	{"uart_test", "{idle_start|idle_stop} {uart1|uart2|uart3}", cli_uart_test_cmd},
#endif //CONFIG_IDLE_UART_OUT_TEST
};

int cli_uart_init(void)
{
	BK_LOG_ON_ERR(bk_uart_driver_init());
	return cli_register_commands(s_uart_commands, UART_CMD_CNT);
}

