#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include "cli.h"
#include "sys_driver.h"
#include "gpio_driver.h"
#include "scr_driver.h"
#include "driver/scr_types.h"

typedef struct {
	uint32_t sc_clk_div;
	uint32_t baud_clk_div;
	uint32_t baud_tune;
	uint32_t tx_repeat;
	uint32_t rx_repeat;
	uint32_t c2c_limit;
	uint32_t act_deact_time;
	uint32_t reset_duration;
	uint32_t ATR_start_limit;
	uint32_t intr;
} scr_config_t;

typedef struct {
	uint8_t data[SCR_MAX_LEN];
	uint8_t len;
} scr_data_t;

extern void delay(int num);

static gpio_scr_map_group_t g_scr_chan = 0;

static int volatile g_scr_ATR_flag = 0;
static int volatile g_scr_c2c_flag = 0;
static int volatile g_scr_fast_act_flag = 0;

static void scr_fast_active(void)
{
	scr_config_t scr_config;

	scr_config.sc_clk_div = bk_scr_get_sc_clk_div(); //0x0D
	scr_config.baud_clk_div = bk_scr_get_baud_clk_div(); //0x0E
	scr_config.baud_tune = bk_scr_get_baud_tune(); //0x19
	scr_config.tx_repeat = bk_scr_get_repeat(BK_SCR_TYPE_TX); //0x0C
	scr_config.rx_repeat = bk_scr_get_repeat(BK_SCR_TYPE_RX); //0x0C
	scr_config.c2c_limit = bk_scr_get_c2c_delay_limit(); //0x13, 0x1A
	scr_config.act_deact_time = bk_scr_get_act_deact_time(); //0x10
	scr_config.reset_duration = bk_scr_get_reset_duration(); //0x11
	scr_config.ATR_start_limit = bk_scr_get_ATR_start_limit(); //0x12
	scr_config.intr = bk_scr_get_intr(BK_SCR_INTR_ALL); //0x07 0x14

	bk_scr_set_control(BK_SCR_CTRL_RX_EN, 0);
	bk_scr_gpio_config(g_scr_chan, BK_SCR_GPIO_PAUSE);

	sys_hal_module_power_ctrl(POWER_MODULE_NAME_AHBP, POWER_MODULE_STATE_OFF);
	delay(5000);
	sys_hal_module_power_ctrl(POWER_MODULE_NAME_AHBP, POWER_MODULE_STATE_ON);
	delay(1000);

	bk_scr_soft_reset();

	bk_scr_set_sc_clk_div(scr_config.sc_clk_div);
	bk_scr_set_baud_clk_div(scr_config.baud_clk_div);
	bk_scr_set_baud_tune(scr_config.baud_tune);
	bk_scr_set_repeat(scr_config.tx_repeat, BK_SCR_TYPE_TX);
	bk_scr_set_repeat(scr_config.rx_repeat, BK_SCR_TYPE_RX);
	bk_scr_set_c2c_delay_limit(scr_config.c2c_limit);
	bk_scr_set_act_deact_time(scr_config.act_deact_time);
	bk_scr_set_reset_duration(scr_config.reset_duration);
	bk_scr_set_ATR_start_limit(scr_config.ATR_start_limit);
	bk_scr_set_intr(BK_SCR_INTR_ALL, scr_config.intr, NULL, NULL);

	bk_scr_set_control(BK_SCR_CTRL_TX_EN, 1);
	bk_scr_set_control(BK_SCR_CTRL_RX_EN, 1);
	bk_scr_set_control(BK_SCR_CTRL_ATR_START_FLUSH_FIFO, 1);
	bk_scr_set_control(BK_SCR_CTRL_GLOBAL_INTR_EN, 1);

	delay(1000);

	bk_scr_set_control(BK_SCR_CTRL_VCC, 1);
	bk_scr_set_control(BK_SCR_CTRL_ACT_FAST, 1);

	bk_scr_gpio_config(g_scr_chan, BK_SCR_GPIO_STORE);
	bk_scr_set_control(BK_SCR_CTRL_RX_EN, 1);

	bk_scr_set_control(BK_SCR_CTRL_CLK_STOP, 0);
}

void scr_rx_fifo_full_cb(void *param)
{
	uint32_t ret;
	scr_data_t *p_data = (scr_data_t *)param;
	ret = bk_scr_read_data(p_data->data + p_data->len, SCR_MAX_LEN - p_data->len);
	p_data->len += ret;
}

void scr_ATR_done_cb(void *param)
{
	g_scr_ATR_flag = 1;
}

void scr_ATR_fail_cb(void *param)
{
	g_scr_ATR_flag = -1;
}

void scr_c2c_full_cb(void *param)
{
	g_scr_c2c_flag = 1;
}

void scr_clk_stop_run_cb(void *param)
{
	if (g_scr_fast_act_flag == 0) {
		g_scr_fast_act_flag = 1;
		scr_fast_active();
	} else {
		g_scr_fast_act_flag = 0;
	}
}

void cli_scr_cold_reset_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint8_t read_buf[SCR_MAX_LEN];
	uint32_t read_len;

	/*enable scr with channel 0~2*/
	if (argc < 2) {
		SCR_LOGE("need input scr channel\r\n");
		return;
	}
	g_scr_chan = atoi(argv[1]);
	if (g_scr_chan >= GPIO_SCR_MAP_GROUP_MAX) {
		SCR_LOGE("scr channel: %d is error\r\n", g_scr_chan);
		return;
	}
	bk_scr_init(g_scr_chan);

	/*scr init config*/
	bk_scr_set_sc_clk_div(3); //clock = 26M/2/3 = 3.25M
	bk_scr_set_baud_clk_div(123); //baud_div = (3+1)*(372/1)-1 = 1487 baud_rate = 26M/(2*(1487+1)) = 8736
	bk_scr_set_baud_tune(0);
	bk_scr_set_repeat(BK_SCR_TYPE_TX, 3);
	bk_scr_set_repeat(BK_SCR_TYPE_RX, 3);

	/*config control of scr*/
	bk_scr_set_control(BK_SCR_CTRL_TX_EN, 1);
	bk_scr_set_control(BK_SCR_CTRL_RX_EN, 1);
	bk_scr_set_control(BK_SCR_CTRL_T0T1, 0);
	bk_scr_set_control(BK_SCR_CTRL_ATR_START_FLUSH_FIFO, 1);
	bk_scr_set_control(BK_SCR_CTRL_GLOBAL_INTR_EN, 1);

	/*enable ATR interrupt*/
	bk_scr_set_intr(BK_SCR_INTR_ATR_DONE, 1, scr_ATR_done_cb, NULL);
	bk_scr_set_intr(BK_SCR_INTR_ATR_FAIL, 1, scr_ATR_fail_cb, NULL);

	/*cold reset*/
	g_scr_ATR_flag = 0;
	bk_scr_set_control(BK_SCR_CTRL_VCC, 1);
	bk_scr_set_control(BK_SCR_CTRL_ACT, 1);
	while (!g_scr_ATR_flag);

	if (g_scr_ATR_flag == 1) {
		BK_RAW_LOGI(NULL, "ATR done: ");
		read_len = bk_scr_read_data(read_buf, SCR_MAX_LEN);
		for (i = 0; i < read_len; i++) {
			BK_RAW_LOGI(NULL, "%02x ", read_buf[i]);
		}
		BK_RAW_LOGI(NULL, "\r\n");
	} else {
		BK_RAW_LOGI(NULL, "cold reset fail\r\n");
	}
	bk_scr_flush_fifo(BK_SCR_TYPE_RX);
	bk_scr_set_intr(BK_SCR_INTR_ATR_FAIL, 0, NULL, NULL);
	bk_scr_set_intr(BK_SCR_INTR_ATR_DONE, 0, NULL, NULL);
}

void cli_scr_warm_reset_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint8_t read_buf[SCR_MAX_LEN];
	uint32_t read_len;

	bk_scr_set_intr(BK_SCR_INTR_ATR_DONE, 1, scr_ATR_done_cb, NULL);
	bk_scr_set_intr(BK_SCR_INTR_ATR_FAIL, 1, scr_ATR_fail_cb, NULL);

	g_scr_ATR_flag = 0;
	bk_scr_set_control(BK_SCR_CTRL_WARM_RST, 1);
	while (!g_scr_ATR_flag);

	if (g_scr_ATR_flag == 1) {
		BK_RAW_LOGI(NULL, "ATR done: ");
		read_len = bk_scr_read_data(read_buf, SCR_MAX_LEN);
		for (i = 0; i < read_len; i++) {
			BK_RAW_LOGI(NULL, "%02x ", read_buf[i]);
		}
		BK_RAW_LOGI(NULL, "\r\n");
	} else {
		BK_RAW_LOGI(NULL, "warm reset fail\r\n");
	}
	bk_scr_flush_fifo(BK_SCR_TYPE_RX);
	bk_scr_set_intr(BK_SCR_INTR_ATR_FAIL, 0, NULL, NULL);
	bk_scr_set_intr(BK_SCR_INTR_ATR_DONE, 0, NULL, NULL);
}

void cli_scr_process_t0_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int i;
	uint32_t read_len;
	scr_header_t *header_p;
	scr_data_t scr_send_data, scr_recv_data;

	if (argc != 2) {
		SCR_LOGE("need apdu\r\n");
		return;
	}

	if (os_strlen(argv[1]) % 2) {
		SCR_LOGE("apdu: %s error\r\n", argv[1]);
		return;
	}

	scr_send_data.len = os_strlen(argv[1]) / 2;
	if (hexstr2bin(argv[1], scr_send_data.data, scr_send_data.len)) {
		SCR_LOGE("apdu: %s is not hex string\r\n", argv[1]);
		return;
	}

	if (scr_send_data.len < sizeof(scr_header_t) && scr_send_data.data[0] != 0xFF) {
		SCR_LOGE("apdu length: %d is error\r\n", scr_send_data.len);
		return;
	}

	bk_scr_set_intr(BK_SCR_INTR_C2C_FULL, 1, scr_c2c_full_cb, NULL);
	bk_scr_set_intr(BK_SCR_INTR_RX_FIFO_FULL, 1, scr_rx_fifo_full_cb, &scr_recv_data);

	/*send command header*/
	g_scr_c2c_flag = 0;
	scr_recv_data.len = 0;
	bk_scr_write_data(scr_send_data.data, sizeof(scr_header_t));
	scr_send_data.len -= sizeof(scr_header_t);
	while (!g_scr_c2c_flag);

	read_len = bk_scr_read_data(scr_recv_data.data + scr_recv_data.len, SCR_MAX_LEN - scr_recv_data.len);
	scr_recv_data.len += read_len;
	BK_RAW_LOGI(NULL, "recv: ");
	for (i = 0; i < scr_recv_data.len; i++) {
		BK_RAW_LOGI(NULL, "%02x ", scr_recv_data.data[i]);
	}
	BK_RAW_LOGI(NULL, "\r\n");
	bk_scr_flush_fifo(BK_SCR_TYPE_RX);

	/*send command body*/
	if (scr_send_data.len) {
		header_p = (scr_header_t *)scr_send_data.data;
		if (scr_recv_data.len == 1 && scr_recv_data.data[0] == header_p->INS) {
			g_scr_c2c_flag = 0;
			scr_recv_data.len = 0;
			bk_scr_write_data(scr_send_data.data + sizeof(scr_header_t), scr_send_data.len);
			while (!g_scr_c2c_flag);

			read_len = bk_scr_read_data(scr_recv_data.data + scr_recv_data.len, SCR_MAX_LEN - scr_recv_data.len);
			scr_recv_data.len += read_len;
			BK_RAW_LOGI(NULL, "recv: ");
			for (i = 0; i < scr_recv_data.len; i++) {
				BK_RAW_LOGI(NULL, "%02x ", scr_recv_data.data[i]);
			}
			BK_RAW_LOGI(NULL, "\r\n");
			bk_scr_flush_fifo(BK_SCR_TYPE_RX);
		}
	}

	bk_scr_set_intr(BK_SCR_INTR_C2C_FULL, 0, NULL, NULL);
	bk_scr_set_intr(BK_SCR_INTR_RX_FIFO_FULL, 0, NULL, NULL);
}

void cli_scr_power_off_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_scr_set_control(BK_SCR_CTRL_DEACT, 1);
	bk_scr_set_control(BK_SCR_CTRL_VCC, 1);
}

void cli_scr_fast_active_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_scr_set_intr(BK_SCR_INTR_CLK_STOP_RUN, 1, scr_clk_stop_run_cb, NULL);
	bk_scr_set_control(BK_SCR_CTRL_CLK_STOP, 1);
}

#define SCR_CMD_CNT (sizeof(s_scr_commands) / sizeof(struct cli_command))

static const struct cli_command s_scr_commands[] = {
	{"scr_cold_reset", "power up", cli_scr_cold_reset_cmd},
	{"scr_warm_reset", "warm reset", cli_scr_warm_reset_cmd},
	{"scr_process_t0", "send t0 hex string", cli_scr_process_t0_cmd},
	{"scr_power_off", "stop", cli_scr_power_off_cmd},
	{"scr_fast_active", "test fast active", cli_scr_fast_active_cmd},
};

int cli_scr_init(void)
{
	return cli_register_commands(s_scr_commands, SCR_CMD_CNT);
}

