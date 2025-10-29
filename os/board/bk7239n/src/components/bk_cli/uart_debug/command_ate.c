#include "command_ate.h"
#include "command_line.h"

#ifdef CONFIG_ATE_TEST
#include "bk_uart.h"
#include "bk_phy_ate.h"
#include "bk_wifi.h"
//#include "phy.h"
//#include "bk_rf_ps.h"
//#include "bk_tx_evm.h"
//#include "bk_rx_sensitivity.h"
#include <os/mem.h>
#include <driver/otp.h>
#include <driver/gpio.h>
#include <driver/efuse.h>
#include <driver/psram.h>
#include <components/ate.h>

#include "bk_phy.h"
//#include "scan.h"
#include "gpio_hal.h"
#include "sys_hal.h"
//#include "bk_cal.h"
#include "bk_rf_internal.h"
#include "aon_pmu_hal.h"
//#include "reg_mdm_cfg.h"
//#if (CONFIG_SOC_BK7256XX)
//#include "rc_driver.h"
//#endif


enum mac_chan_bandwidth {
	PHY_CHNL_BW_20,
	PHY_CHNL_BW_40,
	PHY_CHNL_BW_80,
	PHY_CHNL_BW_160,
	PHY_CHNL_BW_80P80,
	PHY_CHNL_BW_OTHER,
};

#if defined(CONFIG_SOC_BK7236XX)
#define ATE_GPIO_ID                     (6)// GPIO9 use for extern32k
#else
#define ATE_GPIO_ID                     (9)//according the test to select the gpio id
#endif
#define MAC_ADDR_LEN 6

typedef unsigned int                clock_time_t;
extern UINT32 g_rxsens_start;
extern UINT32 g_single_carrier;
extern UINT32 g_rate;
beken2_timer_t rx_sens_ate_tmr      = {0};

#define TX_ENCRPYT_EN_PIN(x)        //(*((uint32_t *)(0x00802800 + TX_ENCRPYT_EN*4))     = (x))
#define TX_ENCRPYT_RESULT_PIN(x)    tx_verify_test_result(x);
#define AX_DEFUALT_GI_TYPE          (0x1)
#define DEFUALT_GI_TYPE             (0)
static UINT8 hw_key_idx             = 0;

UINT32 device_id = 0;

#if ((defined(CONFIG_SOC_BK7256XX)) ||(defined(CONFIG_SOC_BK7235)) ||(defined(CONFIG_SOC_BK7237)))
#define REG_SYS_CTRL_CHIP_ID         (sys_hal_get_chip_id())
#define REG_SYS_CTRL_DEVICE_ID       (aon_pmu_ll_get_reg7c_value())

#define ATE_GPIO_DEV_UART1_RXD       10
#define ATE_GPIO_DEV_UART1_TXD       11

#define RCB_POWER_TABLE_ADDR         (0x4980C400)
#elif (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
#define REG_SYS_CTRL_CHIP_ID         (sys_hal_get_chip_id())
#define REG_SYS_CTRL_DEVICE_ID       (aon_pmu_hal_get_chipid())
#define ATE_GPIO_DEV_UART1_RXD       10
#define ATE_GPIO_DEV_UART1_TXD       11
#else
#define SOC_SYSTEM_REG_BASE          (0x00800000)
#define REG_SYS_CTRL_CHIP_ID_ADDR    (SOC_SYSTEM_REG_BASE + 0x0 * 4)
#define REG_SYS_CTRL_CHIP_ID         (*((volatile unsigned long *) REG_SYS_CTRL_CHIP_ID_ADDR))
#define REG_SYS_CTRL_DEVICE_ID_ADDR  (SOC_SYSTEM_REG_BASE + 0x1 * 4)
#define REG_SYS_CTRL_DEVICE_ID       (*((volatile unsigned long *) REG_SYS_CTRL_DEVICE_ID_ADDR))

#define RC_BEKEN_BASE                0x01050000
#define RCB_POWER_TABLE_ADDR         0x01050200
#endif

extern bk_err_t uart_write_byte(uart_id_t id, uint8_t data);
extern void rwnx_cal_update_pregain(INT32 new_pwr_gain_bias);
extern void sctrl_cali_dpll(UINT8 flag);
extern void manual_cal_show_txpwr_tab(void);
extern void bk7011_max_rxsens_setting(void);
extern int bk7011_reduce_vdddig_for_rx(int reduce);
extern void rwnx_cal_set_bw_i2v(int enable);
extern void mpb_set_txdelay(UINT32 delay_us);
extern uint32_t bk_wifi_get_mpb_ctrl(void);
extern void bk_wifi_set_mpb_ctrl(uint32_t value);
extern void evm_stop_bypass_mac_for_ate (void);
extern void evm_start_bypass_mac_for_ate(void);
extern void evm_bypass_mac_test_for_ate(UINT32 channel, UINT32 bandwidth);

extern void evm_bypass_mac_test(UINT32 channel, wifi_band_t band, UINT32 bandwidth);
extern UINT32 rs_test(UINT32 channel, wifi_band_t band, UINT32 mode);

extern UINT32 evm_bypass_mac_set_tx_data_length_for_ate(UINT32 modul_format, \
                                                   UINT32 len, UINT32 rate, \
                                                   UINT32 bandwidth, UINT32 need_change);
extern UINT32 evm_bypass_mac_set_rate_mformat_for_ate(UINT32 ppdu_rate, UINT32 m_format);
extern uint8_t evm_add_key(void);
extern void evm_del_key(uint8_t key_idx);
extern void ble_dut_start(uint8_t uart_id);

extern uint32_t bk_otp_fully_flow_test();

#if (!defined(CONFIG_SOC_BK7256XX))
extern void sctrl_dpll_int_open(void);
#endif

void uart_send_byte_for_ate(UINT8 data)
{
	uart_write_byte(bk_get_printf_port(), data);
}

void uart_send_bytes_for_ate(UINT8* pData, UINT8 cnt)
{
    int i;
    for(i = 0; i < cnt; i ++)
    {
        uart_send_byte_for_ate(pData[i]);
    }
}

void ate_time_delay(volatile uint32_t times)
{
    while (times--) ;
}

void tx_verify_test_result(UINT8 res)
{
    UINT8 tx_buffer[3];

    tx_buffer[0] = 0x55;
    tx_buffer[1] = 0x23;
    if(res == 2)
        tx_buffer[2] = 0x01;
    else
        tx_buffer[2] = 0x00;
    uart_send_bytes_for_ate(tx_buffer, 3);
}

const UINT8 tx_golden[] = {
    0x88,0x40,0x00,0x00,0x6c,0xe8,
    0x5c,0xaa,0x6c,0x8a,0xc8,0x47,0x8c,0x08,0xab,0x49,0xc8,0x47,0x8c,0x08,0xab,0x49,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x35,0x54,0x53,0x7f,
    0xdf,0x88,0x1e,0x80,0xc5,0xd9,0x44,0x92,0x90,0xce,0x39,0xe1,0x93,0x08,0x9e,0x6c,
    0x9e,0xf8,0x8c,0xa6,0xc8,0x74,0xa7,0x3e,0x18,0xc1,0xd3,0x6f,0xab,0xea,0xf4,0x28,
    0x68,0x41,0x27,0x45,0x71,0x86,0x2e,0x5c,0xc6,0xa8,0x9a,0x07,0x2d,0xda,0xf3,0xc7,
    0x1f,0x15,0xb1,0xb5,0xf1,0xa7,0x0d,0xed,0x24,0xe9,0x1c,0x58,0xc1,0xe7,0x41,0xd0,
    0x56,0x47,0x66,0xf8,0xe3,0x0f,0xea,0xa2,0x41,0x2e,0x49,0x77,0xd6,0xef,0xf2,0x7f,
    0xdf,0x36,0x73,0x90,0xcd,0x80,0x6b,0x38,0x60,0xc8,0x53,0xd9,0xd3,0x03,0x87,0x3f,
    0xe0,0x2e,0xcc,0x50,0x17,0xf9,0x52,0xfc,0x16,0x73,0x8d,0xb2,0x81,0xd4,0x06,0x3c,
    0x37,0x1e,0xd4,0xb3,0x7c,0x0c,0x8c,0x95,0x1e,0x50,0x89,0xda,0x02,0x97,0x2d,0xea,
    0x76,0xa4,0x78,0x91,0x5e,0xd2,0x60,0xa6,0x49,0xd0,0x70,0x98,0x17,0x3e,0x74,0x69,
    0x57,0x92,0x01,0x19,0xd4,0x92,0x04,0xb1,0xb7,0x93,0x66,0x10,0xf1,0xa6,0x8c,0xe5,
    0xb8,0x67,0x30,0x33,0x27,0x42,0xfc,0x2f,0x8d,0xab,0x6f,0x81,0xb0,0x44,0x20,0xcc,
    0x9e,0xa6,0xb4,0x48,0xd3,0x41,0x63,0x98,0xc8,0x4a,0xa0,0xf1,0x5f,0x5e,0xeb,0x21,
    0x94,0x3a,0x3b,0x69,0x33,0xee,0x1d,0xb2,0xcb,0x35,0x4c,0xf2,0x83,0x0d,0x9e,0x3f,
    0x9f,0xcb,0x94,0xd5,0xc2,0x76,0x25,0x48,0x9d,
};

void send_compile_time(void)
{
    uint8_t build_time[] = __DATE__ " " __TIME__;

    uart_send_bytes_for_ate(build_time, sizeof(build_time));
}

void send_chip_id(void)
{
    unsigned long chip_id;
    UINT8 tx_buffer[16];
    chip_id = REG_SYS_CTRL_CHIP_ID;
    tx_buffer[0] = (chip_id >> 24) & 0x00FF;
    tx_buffer[1] = (chip_id >> 16) & 0x00FF;
    tx_buffer[2] = (chip_id >> 8) & 0x00FF;
    tx_buffer[3] = chip_id & 0x00FF;
    uart_send_bytes_for_ate(tx_buffer, 4);

}

void send_device_id(void)
{

    UINT8 tx_buffer[16];

    tx_buffer[0] = (device_id >> 24) & 0x00FF;
    tx_buffer[1] = (device_id >> 16) & 0x00FF;
    tx_buffer[2] = (device_id >> 8) & 0x00FF;
    tx_buffer[3] = device_id & 0x00FF;
    uart_send_bytes_for_ate(tx_buffer, 4);
}

void gpio_voltage_output_high(void)
{
	uint32_t  i = 0;

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD)
			continue;
		bk_gpio_set_value(i, 2);
	}
}

void gpio_voltage_output_low(void)
{
	uint32_t  i = 0;

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD)
			continue;
		bk_gpio_set_value(i, 0);
	}
}

void gpio_voltage_input_high(void)
{
	uint32_t  i = 0;
	uint32_t val = 0;
	uint8_t *data = (uint8_t *)os_zalloc(GPIO_NUM_MAX);
	uint8_t *restore_reg = (uint8_t *)os_zalloc(GPIO_NUM_MAX);

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD)
			continue;
		restore_reg[i] = bk_gpio_get_value(i);
		bk_gpio_set_value(i, 0x2C);
	}

	//uart_write_byte_for_ate(0, (uint8_t *)restore_reg, GPIO_NUM_MAX);

	data[0] = 0x04;
	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD) {
			data[(1 + i/8)] |= (0x1 << (i % 8));
		} else {
			val = bk_gpio_get_value(i);
			if(val & 0x1) {
				data[(1 + i/8)] |= (0x1 << (i % 8));
			} else {
				data[(1 + i/8)] &= ~(0x1 << (i % 8));
			}
		}
	}
	uart_write_byte_for_ate(0, (uint8_t *)data, (1 + GPIO_NUM_MAX/8));
	if (data) {
		os_free(data);
	}

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD)
			continue;
		bk_gpio_set_value(i, restore_reg[i]);
	}
	if (restore_reg) {
		os_free(restore_reg);
	}

}

void gpio_voltage_input_low(void)
{
	uint32_t  i = 0;
	uint32_t val = 0;
	uint8_t *data = (uint8_t *)os_zalloc(GPIO_NUM_MAX);
	uint8_t *restore_reg = (uint8_t *)os_zalloc(GPIO_NUM_MAX);

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD)
			continue;
		restore_reg[i] = bk_gpio_get_value(i);
		bk_gpio_set_value(i, 0x3D);
	}

	//uart_write_byte_for_ate(0, (uint8_t *)restore_reg, GPIO_NUM_MAX);

	data[0] = 0x05;
	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD) {
			data[(1 + i/8)] |= (0x1 << (i % 8));
		} else {
			val = bk_gpio_get_value(i);
			if(!(val & 0x1)) {
				data[(1 + i/8)] |= (0x1 << (i % 8));
			} else {
				data[(1 + i/8)] &= ~(0x1 << (i % 8));
			}
		}
	}
	uart_write_byte_for_ate(0, (uint8_t *)data, (1 + GPIO_NUM_MAX/8));
	if (data) {
		os_free(data);
	}

	for(i = 0; i < GPIO_NUM_MAX; i ++)
	{
		if(i == ATE_GPIO_DEV_UART1_RXD || i == ATE_GPIO_DEV_UART1_TXD)
			continue;
		bk_gpio_set_value(i, restore_reg[i]);
	}
	if (restore_reg) {
		os_free(restore_reg);
	}
}

void get_device_id(void)
{
    device_id = REG_SYS_CTRL_DEVICE_ID;
}

void mpb_tx_mode_close_for_ate(void)
{
	UINT32 reg;
	reg = bk_wifi_get_mpb_ctrl();
	reg &= 0x00;
	bk_wifi_set_mpb_ctrl(reg);
}

/**
 * rate        : reference to evm_translate_tx_rate
 * channel     : [2400, 2655]
 * bandwidth   : reference to mac_chan_bandwidth
 * modul_format: reference to ModulationFormat
 */
static void evm_test(UINT32 rate, UINT32 channel, UINT32 bandwidth, UINT32 modul_format)
{
    UINT32 packet_len;
    UINT32 gi;

    evm_stop_bypass_mac();
#if (!defined(CONFIG_SOC_BK7236XX)) && (!defined(CONFIG_SOC_BK7239XX)) && (!defined(CONFIG_SOC_BK7286XX))
    rwnx_no_use_tpc_set_pwr();
#endif

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)) || (defined(CONFIG_SOC_BK7256XX))
    mdm_set_scramblerctrl(0x83); //change from 0x85 to 0x83 by cunliang
#else
    mdm_set_scramblerctrl(0x85);
#endif
    //set channel before init macbypass, for bk7236 need it.
    evm_bypass_mac_test_for_ate(channel, bandwidth);
    evm_init_bypass_mac();

    if (rate <= 54)
	    modul_format = 0;
    else if (modul_format < 2) {
	    //CL_LOGD("HT/VHT/HE pkts modul_format >= 2\r\n");
	    modul_format = 2;
    }

    packet_len = evm_get_auto_tx_len(rate, modul_format, bandwidth, 0);
    evm_bypass_mac_set_tx_data_length(modul_format, packet_len, rate, bandwidth, 1);

    evm_bypass_mac_set_rate_mformat(rate, modul_format);
    evm_set_bandwidth(bandwidth);

    if (modul_format == 5)
        gi = AX_DEFUALT_GI_TYPE;
    else
        gi = DEFUALT_GI_TYPE;

    evm_bypass_mac_set_guard_i_type(gi);
    if ((modul_format >= 5) && (bandwidth == 1)) {
        //HE-40MHz support LDPC only
        evm_bypass_mac_set_fec_coding(1);
    }

    rwnx_cal_en_extra_txpa();
    rf_module_vote_ctrl(RF_OPEN,RF_BY_TEMP_BIT);
    rwnx_cal_set_txpwr_by_rate(evm_translate_tx_rate(rate), 0);
    rf_module_vote_ctrl(RF_CLOSE,RF_BY_TEMP_BIT);
	//evm_bypass_mac_set_txdelay(txdelay);

    evm_start_bypass_mac();

#if (defined(CONFIG_SOC_BK7231N))
    //CLI_LOGI("pwr_gain:0x%x\r\n", 0x200);
    REG_WRITE((RC_BEKEN_BASE + (0x43 * 4)), 0x200);
#elif (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
    bk7011_stop_tx_pattern();
#endif

#if (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
    bk7011_cal_dpll();
    //CLI_LOGI("cal dpll\r\n");
#else
    //CLI_LOGI("cal dpll and open int\r\n");
    sctrl_cali_dpll(0);
    sctrl_dpll_int_open();
#endif
    uint32_t duty_in_us;
    if (rate < 128)
    {
        /* long preamble always on since MSB of verctor4 in macbypass */
        duty_in_us = hal_machw_frame_duration_ate(bandwidth, modul_format, evm_translate_tx_rate(rate), 1, gi, packet_len);
    }
    else
        duty_in_us = hal_machw_frame_duration_ate(bandwidth, modul_format, rate - 128, 1, gi, packet_len);
    /* keep duty cycle 90% */
    mpb_set_txdelay(duty_in_us/9);
}

static void evm_single_test(UINT32 rate, UINT32 channel, UINT32 bandwidth)
{
    UINT32 modul_format = 0;
    UINT32 packet_len;
    UINT32 gi;
    UINT32 duty_in_us;
    SC_TYPE_T single_carrier_type = SINGLE_CARRIER_11G;

    evm_stop_bypass_mac_for_ate();
    rwnx_no_use_tpc_set_pwr();

#if (defined(CONFIG_SOC_BK7231N)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)) || (defined(CONFIG_SOC_BK7256XX))
    mdm_set_scramblerctrl(0x83); //change from 0x85 to 0x83 by cunliang
#else
    mdm_set_scramblerctrl(0x85);
#endif

    //set channel before init macbypass, for bk7236 need it.
    evm_bypass_mac_test(channel, IEEE80211_BAND_2GHZ, bandwidth);
    evm_init_bypass_mac();

    if (rate <= 54)
        modul_format = 0;
    else if (modul_format < 2) {
        //CLI_LOGI("HT/VHT/HE pkts modul_format >= 2\r\n");
        modul_format = 2;
    }

    packet_len = evm_get_auto_tx_len(rate, modul_format, bandwidth, 0);
    evm_bypass_mac_set_tx_data_length_for_ate(modul_format, packet_len, rate, bandwidth, 1);

    evm_bypass_mac_set_rate_mformat_for_ate(rate, modul_format);
    evm_set_bandwidth(bandwidth);

    if (modul_format == 5)
        gi = AX_DEFUALT_GI_TYPE;
    else
        gi = DEFUALT_GI_TYPE;

    evm_bypass_mac_set_guard_i_type(gi);

    rwnx_cal_en_extra_txpa();
    // set g_single_carrier ahead of set txpwr, for bk7236 need it
    g_single_carrier = 1;

    rf_module_vote_ctrl(RF_OPEN,RF_BY_TEMP_BIT);
    rwnx_cal_set_txpwr_by_rate(evm_translate_tx_rate(rate), 0);
    rf_module_vote_ctrl(RF_CLOSE,RF_BY_TEMP_BIT);;
    //evm_bypass_mac_set_txdelay(txdelay);

    evm_start_bypass_mac_for_ate();

    if (g_rate == 11)
        single_carrier_type = SINGLE_CARRIER_11B;
    else
        single_carrier_type = SINGLE_CARRIER_11G;

    // bk7236,no need do it anymore
#if (defined(CONFIG_SOC_BK7231N))
    UINT32 pwr_gain = REG_READ(RCB_POWER_TABLE_ADDR + (0x34 * 4));
    //CLI_LOGI("pwr_gain:0x%x\r\n", pwr_gain & 0x3FF);
    REG_WRITE((RC_BEKEN_BASE + (0x43 * 4)), pwr_gain & 0x3FF);
#elif (defined(CONFIG_SOC_BK7256XX))
    UINT32 pwr_gain = REG_READ(RCB_POWER_TABLE_ADDR + (0x34 * 4));
    //CLI_LOGI("pwr_gain:0x%x\r\n", pwr_gain & 0x3FF);
    rc_drv_set_tx_pre_gain_2nd(pwr_gain & 0x3FF);
    rwnx_no_use_tpc_set_pwr();
#endif
    evm_bypass_set_single_carrier(single_carrier_type, rate);

#if (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
    bk7011_cal_dpll();
    //CLI_LOGI("cal dpll\r\n");
#else
    //CLI_LOGI("cal dpll and open int\r\n");
    sctrl_cali_dpll(0);
    sctrl_dpll_int_open();
#endif

    if (rate < 128)
    {
        /* long preamble always on since MSB of verctor4 in macbypass */
        duty_in_us = hal_machw_frame_duration_ate(bandwidth, modul_format, evm_translate_tx_rate(rate), 1, gi, packet_len);
    }
    else
        duty_in_us = hal_machw_frame_duration_ate(bandwidth, modul_format, rate - 128, 1, gi, packet_len);
    /* keep duty cycle 10% */
    mpb_set_txdelay(duty_in_us * 9);
}

void tx_verify_test_call_back(void)
{
    uint32_t ret, i, *word_ptr, frame_len = 0, golden_size;
    UINT8* sample_buf = NULL, sample, golden;

    evm_del_key(hw_key_idx);

    ret = bk7011_get_sample_mac_out_for_ate(&sample_buf, &frame_len);
    if(ret != 0)
    {
        //CLI_LOGI("get sample fail %d\r\n", ret);
        bk7011_clear_sample_mac_out_for_ate();
        TX_ENCRPYT_EN_PIN(0);
        return;
    }

    word_ptr = (uint32_t *)sample_buf;
    #if 0
    for(i = 0; i < 6; i ++)
    {
        CLI_LOGI("%02x,", (word_ptr[i] >> 13) & 0xff );
    }
    CLI_LOGI("\r\n");

    for(i = 6; i < frame_len; i ++)
    {
        CLI_LOGI("%02x,", (word_ptr[i] >> 13) & 0xff );
        if((i - 6 + 1) % 16 == 0)
            CLI_LOGI("\r\n");
    }

    bk7011_clear_sample_mac_out();
    TX_ENCRPYT_EN_PIN(0);
    #else
    golden_size = sizeof(tx_golden);
    ret = 0;

    if(golden_size != frame_len)
    {
        //CLI_LOGI("len not match:%d, %d\r\n", golden_size, frame_len);
        ret |= 1;
    }

    for(i = 0; i < golden_size; i ++)
    {
        sample = ((word_ptr[i] >> 13) & 0xff);
        golden = tx_golden[i];

        if(sample != golden)
        {
            ret |= 2;
            //CLI_LOGI("data not match:%d, %02x, %02x\r\n", i, sample, golden);
            break;
        }
    }

    bk7011_clear_sample_mac_out_for_ate();
    if(ret != 0)
    {
        //CLI_LOGI("tx verify fail:%01x\r\n", ret);
        TX_ENCRPYT_RESULT_PIN(0);
    }
    else
    {
        //CLI_LOGI("tx verify pass\r\n");
        TX_ENCRPYT_RESULT_PIN(2);
    }
    TX_ENCRPYT_EN_PIN(0);
    #endif
}

static void tx_verify_test_2442(void)
{
    uint32_t frame_len, ret;

    TX_ENCRPYT_EN_PIN(2);
    frame_len = 255;

    evm_phy_init(2462, IEEE80211_BAND_2GHZ, PHY_CHNL_BW_20);

    hw_key_idx = evm_add_key();
    phy_close_cca();

    ret = bk7011_set_sample_mac_out_for_ate(frame_len);
    if(ret != 0)
    {
        //CLI_LOGI("set sample fail %d\r\n", ret);
        TX_ENCRPYT_EN_PIN(0);
        return;
    }

    ret = evm_req_tx_for_ate(frame_len);
    if(ret != 0)
    {
        //CLI_LOGI("req tx fail %d\r\n", ret);
        bk7011_clear_sample_mac_out_for_ate();
        TX_ENCRPYT_EN_PIN(0);
        return;
    }
}

void rxsens_ct_hdl_for_ate(void *param)
{
	bk_err_t err;
	rx_get_rx_result_end_for_ate(param, NULL);
	rx_get_rx_result_begin_for_ate();

	if (rx_sens_ate_tmr.handle != NULL) {
		err = rtos_oneshot_reload_timer(&rx_sens_ate_tmr);
		BK_ASSERT(kNoErr == err);
	}
}

static int per_test(uint32_t bandwidth, uint32_t channel, uint32_t duration, uint32_t arg)
{
    UINT8 ret;
    int err;

	//sys_ctrl_0x42[6:4]=SCTRL_DIGTAL_VDD=4
#if (defined(CONFIG_SOC_BK7231N))
	if (bandwidth == 0) {
		/* set MDM_REG206<17:16>=3 for band20 */
		mdm_cfgsmooth_setf(3);
		bk7011_reduce_vdddig_for_rx(1);
		/* recover TRX_REG9<22>=0 for band20 */
		rwnx_cal_set_bw_i2v(0);
    } else {
        /* set MDM_REG206<17:16>=1 for band40 */
        mdm_cfgsmooth_setf(1);
        bk7011_reduce_vdddig_for_rx(0);
        /* set TRX_REG9<22>=1 for band40 */
        rwnx_cal_set_bw_i2v(1);
    }
#elif (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
	// TODO: BK7236 phy karst
#elif (defined(CONFIG_SOC_BK7271))
#else
    if (bandwidth == 0) {
        sys_hal_set_vdd_value(3);
    } else {
        sys_hal_set_vdd_value(5);
    }
#endif

    ret = rs_test(channel, IEEE80211_BAND_2GHZ, bandwidth);
    if (ret)
        return 1;

#if (defined(CONFIG_SOC_BK7231N))
    extern void phy_enable_lsig_intr(void);
    phy_enable_lsig_intr();
    /* set MDM_REG202<23>=1 for rx */
    mdm_cpemode_setf(1);
#elif (defined(CONFIG_SOC_BK7256XX)) || (defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))
    // TODO: BK7236 phy karst
    if (bandwidth == 1)
        rwnx_cal_set_40M_extra_setting(1);
    else
        rwnx_cal_set_40M_extra_setting(0);
#else
    if ((channel == 13) || (channel == 14))
        rwnx_cal_set_reg_adda_ldo(0);
    else
        rwnx_cal_set_reg_adda_ldo(3);

    rwnx_cal_set_reg_rx_ldo();
    bk7011_max_rxsens_setting();
    rwnx_cal_dis_extra_txpa();

    if (bandwidth == 1) {
        if ((channel >= 3) && (channel <= 10))
            rwnx_cal_set_40M_extra_setting(1);
        else
        rwnx_cal_set_40M_extra_setting(0);
    } else
        rwnx_cal_set_40M_extra_setting(0);
#endif

    g_rxsens_start = 1;

    if (duration)
    {
    	rx_get_rx_result_begin_for_ate();

        if (rx_sens_ate_tmr.handle != NULL)
        {
            err = rtos_deinit_oneshot_timer(&rx_sens_ate_tmr);
            BK_ASSERT(kNoErr == err);
            rx_sens_ate_tmr.handle = NULL;
        }

        err = rtos_init_oneshot_timer(&rx_sens_ate_tmr, duration, (timer_2handler_t)rx_get_rx_result_end_for_ate, (void *)arg, NULL);
        BK_ASSERT(kNoErr == err);
        err = rtos_start_oneshot_timer(&rx_sens_ate_tmr);
        BK_ASSERT(kNoErr == err);
    }
    else
    {
        if (rx_sens_ate_tmr.handle != NULL)
        {
            err = rtos_deinit_oneshot_timer(&rx_sens_ate_tmr);
            BK_ASSERT(kNoErr == err);
            rx_sens_ate_tmr.handle = NULL;
        }
    }
    return 0;
}

static void ate_pm_gpio_callback(gpio_id_t gpio_id)
{
	#if defined(CONFIG_SYS_CPU0)
	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,100000000);
	bk_ate_ctrl(1);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_ON);
	extern void start_cpu1_core(void);
	start_cpu1_core();
	#endif
}
static void ate_pm_exit_lvsleep(uint64_t sleep_time, void *args)
{

}
int ate_enter_low_voltage()
{
#if (defined(CONFIG_SYS_CPU0))
	pm_cb_conf_t exit_config = {
	.cb = (pm_cb)ate_pm_exit_lvsleep,
	.args = NULL
	};
	bk_ate_ctrl(0);
    bk_pm_sleep_register_cb(PM_MODE_LOW_VOLTAGE, PM_DEV_ID_DEFAULT, NULL, &exit_config);

	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_BTSP,0x1,100000000);
	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_WIFIP_MAC,0x1,100000000);

	extern void stop_cpu1_core(void);
	stop_cpu1_core();
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_OFF);
#if (CONFIG_CPU_CNT > 2)
	extern void stop_cpu2_core(void);
	stop_cpu2_core();
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU2, PM_POWER_MODULE_STATE_OFF);
#endif

#if defined(CONFIG_SOC_BK7236XX)
#else
	bk_pm_lpo_src_set(PM_LPO_SRC_ROSC);
#endif
	bk_gpio_register_isr(ATE_GPIO_ID, ate_pm_gpio_callback);
	bk_gpio_register_wakeup_source(ATE_GPIO_ID,GPIO_INT_TYPE_HIGH_LEVEL);
	bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

	bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x1,100000000);
#endif
	return 0;
}
int ate_enter_deep_sleep()
{
#if (defined(CONFIG_SYS_CPU0))
	bk_ate_ctrl(0);
	bk_pm_module_vote_power_ctrl(PM_POWER_SUB_MODULE_NAME_BTSP,POWER_MODULE_STATE_OFF);
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_WIFIP_MAC,POWER_MODULE_STATE_OFF);

	extern void stop_cpu1_core(void);
	stop_cpu1_core();
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU1, PM_POWER_MODULE_STATE_OFF);
#if (CONFIG_CPU_CNT > 2)
	extern void stop_cpu2_core(void);
	stop_cpu2_core();
	bk_pm_module_vote_power_ctrl(PM_POWER_MODULE_NAME_CPU2, PM_POWER_MODULE_STATE_OFF);
#endif

    bk_gpio_register_wakeup_source(ATE_GPIO_ID,GPIO_INT_TYPE_HIGH_LEVEL);
	bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

	bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);
#endif
	return 0;
}

#if defined(CONFIG_MAILBOX)
#include <driver/mailbox.h>

#define mailbox_write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define mailbox_get_addr_data(addr)                  *((volatile uint32_t *)(addr))

static int ate_test_cpu1_by_mailbox_cb_flag = 0;
static void ate_test_cpu1_by_mailbox_cb(mailbox_data_t *ate_mbx_data)
{
    if(!ate_test_cpu1_by_mailbox_cb_flag)
        ate_test_cpu1_by_mailbox_cb_flag = 1;

    return;
}

static void ate_test_cpu1_by_mailbox_init(void)
{
    bk_mailbox_init();
    bk_mailbox_recv_callback_register(MAILBOX_CPU1, MAILBOX_CPU0, (mailbox_callback_t)ate_test_cpu1_by_mailbox_cb);
    ate_test_cpu1_by_mailbox_cb_flag = 0;
}

static void ate_test_cpu1_by_mailbox_deinit(void)
{
    bk_mailbox_deinit();
    bk_mailbox_recv_callback_unregister(MAILBOX_CPU1, MAILBOX_CPU0);
}

static void ate_test_cpu1_by_mailbox_case(void)
{
    char tx_buffer[8];

    if(ate_test_cpu1_by_mailbox_cb_flag) {
        tx_buffer[0]='C';//43
        tx_buffer[1]='P';//50
        tx_buffer[2]='U';//55
        tx_buffer[3]='1';//31
        tx_buffer[4]='\0';
        uart_write_string(0, tx_buffer);
    } else
        return;
}

static void ate_test_cpu2_by_mailbox_case(void)
{
    char tx_buffer[8];

    //if(ate_test_cpu2_by_mailbox_cb_flag) {
    if(mailbox_get_addr_data(CONFIG_ATE_CPU2_ADDRESS) == CONFIG_ATE_CPU2_VALUE) {
        tx_buffer[0]='C';//43
        tx_buffer[1]='P';//50
        tx_buffer[2]='U';//55
        tx_buffer[3]='2';//32
        tx_buffer[4]='\0';
        uart_write_string(0, tx_buffer);
    } else
        return;
}

#endif
void ate_test_multiple_cpus_init(void)
{
#if defined(CONFIG_MAILBOX)
    ate_test_cpu1_by_mailbox_init();
#endif
}

void ate_test_multiple_cpus_deinit(void)
{
#if defined(CONFIG_MAILBOX)
    ate_test_cpu1_by_mailbox_deinit();
#endif
}

static void ate_test_multiple_cpus_case(void)
{
#if defined(CONFIG_MAILBOX)
    ate_test_cpu1_by_mailbox_case();
    ate_test_cpu2_by_mailbox_case();
#endif
}

#define write_data(addr,val)                 *((volatile uint32_t *)(addr)) = val
#define get_addr_data(addr)                  *((volatile uint32_t *)(addr))

static void psram_function_test(void)
{
	UINT8 tx_buffer;

	tx_buffer = 0x32;

#if defined(CONFIG_PSRAM)

	bk_psram_init();

	uint32_t base_addr = 0x60000000;
	uint32_t value = 0;
	uint32_t word_len = 1024;
	uint8_t  flag_error = 0;

	bk_psram_set_clk(PSRAM_80M);

	for (uint32_t i = 0; i < word_len; i++)
	{
		write_data(base_addr + i * 4, 0x11223344 + i);
	}

	for (uint32_t i = 0; i < word_len; i++)
	{
		value = get_addr_data(base_addr + i * 4);

		if (value != (0x11223344 + i))
		{
			flag_error = 1;
			break;
		}
	}

	if (flag_error)
	{
		tx_buffer = 0x31;
	}
	else
	{
		tx_buffer = 0x30;
	}

#endif

	uart_send_bytes_for_ate(&tx_buffer, 1);
}

#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
int set_device_id_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    uint8_t device_id_write[EFUSE_DEVICE_ID_BYTE_NUM] = {0};
    uint8_t retry_times = 0;
    int ret = 0;

    tx_buffer[0] = 0x55;
    if (cnt < 7) {
        /* return param num error */
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x1;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    for(int iIndex = 0; iIndex < EFUSE_DEVICE_ID_BYTE_NUM; iIndex++) {
        device_id_write[iIndex] = (uint8_t) (content[2 + iIndex] & 0xFF);
    }

    /* sn2 is out of range */
    if((device_id_write[0] < EFUSE_DEVICE_ID_SN_MINIMUM) || (device_id_write[0] > EFUSE_DEVICE_ID_SN_MAXIMUM)) {
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x2;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    /*  sn1 is out of range */
    if((device_id_write[1] < EFUSE_DEVICE_ID_SN_MINIMUM) || (device_id_write[1] > EFUSE_DEVICE_ID_SN_MAXIMUM)) {
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x3;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    /* wafer id is out of range */
    if((device_id_write[2] < EFUSE_WAFER_ID_MINIMUM) || (device_id_write[2] > EFUSE_WAFER_ID_MAXIMUM)) {
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x4;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    for (; retry_times < 3; retry_times++) {
	    ret = bk_otp_apb_update(OTP_DEVICE_ID, device_id_write, sizeof(device_id_write));
		if (ret == BK_OK) {
			break;
		}
    }

	if (ret != BK_OK) {
        tx_buffer[1] = 0x77;

        if (ret == BK_ERR_OTP_NOT_EMPTY) {
            tx_buffer[2] = 0x5;
        } else if(ret == BK_ERR_OTP_ADDR_OUT_OF_RANGE) {
            tx_buffer[2] = 0x6;
        } else if(ret == BK_ERR_NO_WRITE_PERMISSION) {
            tx_buffer[2] = 0x7;
        } else if(ret == BK_ERR_OTP_UPDATE_NOT_EQUAL) {
            tx_buffer[2] = 0x8;
        } else if(ret == BK_ERR_NO_READ_PERMISSION) {
            tx_buffer[2] = 0x9;
        } else {
            tx_buffer[2] = 0xff;
        }

        bk_otp_apb_read(OTP_DEVICE_ID, tx_buffer + 3, EFUSE_DEVICE_ID_BYTE_NUM);
        uart_send_bytes_for_ate(tx_buffer, 3 + EFUSE_DEVICE_ID_BYTE_NUM);

        return BK_FAIL;
	}

    tx_buffer[1] = 0x88;
    bk_otp_apb_read(OTP_DEVICE_ID, tx_buffer + 2, EFUSE_DEVICE_ID_BYTE_NUM);
    uart_send_bytes_for_ate(tx_buffer, 2 + EFUSE_DEVICE_ID_BYTE_NUM);

    return BK_OK;
}

int set_mac_address_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    uint8_t mac_address_write[MAC_ADDR_LEN] = {0};
    uint8_t retry_times = 0;
    int ret = 0;

    tx_buffer[0] = 0x55;
    if (cnt < 2 + MAC_ADDR_LEN) {
        /* return param num error */
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x1;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    for(int iIndex = 0; iIndex < MAC_ADDR_LEN; iIndex++) {
        mac_address_write[iIndex] = (uint8_t) (content[2 + iIndex] & 0xFF);
    }

    for (; retry_times < 3; retry_times++) {
	    ret = bk_otp_apb_update(OTP_MAC_ADDRESS, mac_address_write, sizeof(mac_address_write));
		if (ret == BK_OK) {
			break;
		}
    }

	if (ret != BK_OK) {
        tx_buffer[1] = 0x77;

        if (ret == BK_ERR_OTP_NOT_EMPTY) {
            tx_buffer[2] = 0x5;
        } else if(ret == BK_ERR_OTP_ADDR_OUT_OF_RANGE) {
            tx_buffer[2] = 0x6;
        } else if(ret == BK_ERR_NO_WRITE_PERMISSION) {
            tx_buffer[2] = 0x7;
        } else if(ret == BK_ERR_OTP_UPDATE_NOT_EQUAL) {
            tx_buffer[2] = 0x8;
        } else if(ret == BK_ERR_NO_READ_PERMISSION) {
            tx_buffer[2] = 0x9;
        } else {
            tx_buffer[2] = 0xff;
        }

        bk_otp_apb_read(OTP_MAC_ADDRESS, tx_buffer + 3, MAC_ADDR_LEN);
        uart_send_bytes_for_ate(tx_buffer, 3 + MAC_ADDR_LEN);

        return BK_FAIL;
	}

    tx_buffer[1] = 0x88;
    bk_otp_apb_read(OTP_MAC_ADDRESS, tx_buffer + 2, MAC_ADDR_LEN);
    uart_send_bytes_for_ate(tx_buffer, 2 + MAC_ADDR_LEN);

    return BK_OK;
}
#else
int set_device_id_to_efuse(const unsigned char *content, int cnt, UINT8 *tx_buffer)
{
    uint8_t device_id_write[EFUSE_DEVICE_ID_BYTE_NUM] = {0};
    uint8_t efuse_addr[EFUSE_DEVICE_ID_BYTE_NUM] = {EFUSE_ADDR_BYTE28, EFUSE_ADDR_BYTE27, EFUSE_ADDR_BYTE26, \
                                                    EFUSE_ADDR_BYTE25, EFUSE_ADDR_BYTE24};
    uint8_t device_id_read[EFUSE_DEVICE_ID_BYTE_NUM] = {0};
    uint8_t retry_times = 0;
    int ret = 0;

    tx_buffer[0] = 0x55;
    if (cnt < 7) {
        /* return param num error */
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x1;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    for(int iIndex = 0; iIndex < EFUSE_DEVICE_ID_BYTE_NUM; iIndex++) {
        device_id_write[iIndex] = (uint8_t) (content[2 + iIndex] & 0xFF);
    }

    /* sn2 is out of range */
    if((device_id_write[0] < EFUSE_DEVICE_ID_SN_MINIMUM) || (device_id_write[0] > EFUSE_DEVICE_ID_SN_MAXIMUM)) {
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x2;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    /*  sn1 is out of range */
    if((device_id_write[1] < EFUSE_DEVICE_ID_SN_MINIMUM) || (device_id_write[1] > EFUSE_DEVICE_ID_SN_MAXIMUM)) {
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x3;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    /* wafer id is out of range */
    if((device_id_write[2] < EFUSE_WAFER_ID_MINIMUM) || (device_id_write[2] > EFUSE_WAFER_ID_MAXIMUM)) {
        tx_buffer[1] = 0x77;
        tx_buffer[2] = 0x4;
        uart_send_bytes_for_ate(tx_buffer, 3);
        return BK_FAIL;
    }

    for(int iIndex = 0; iIndex < EFUSE_DEVICE_ID_BYTE_NUM; iIndex++) {
        retry_times = 0;
retry_efuse_write:
        //efuse write
        ret = bk_efuse_write_byte(efuse_addr[iIndex], device_id_write[iIndex]);
        //efuse read
        bk_efuse_read_byte(efuse_addr[iIndex], &(device_id_read[iIndex]));

        if(device_id_read[iIndex] != device_id_write[iIndex]) {
            if(retry_times < 2) {
                retry_times++;
                goto retry_efuse_write;
            } else {
                tx_buffer[1] = 0x77;

                if (ret == BK_ERR_EFUSE_DRIVER_NOT_INIT) {
                    tx_buffer[2] = 0x5;
                } else if(ret == BK_ERR_EFUSE_ADDR_OUT_OF_RANGE) {
                    tx_buffer[2] = 0x6;
                } else if(ret == BK_ERR_EFUSE_CANNOT_WRTIE) {
                    tx_buffer[2] = 0x7;
                } else if(ret == BK_ERR_EFUSE_WRTIE_NOT_EQUAL) {
                    tx_buffer[2] = 0x8;
                } else if(ret == BK_ERR_EFUSE_READ_FAIL) {
                    tx_buffer[2] = 0x9;
                } else {
                    tx_buffer[2] = 0xff;
                }

                for(int jIndex = 0; jIndex < EFUSE_DEVICE_ID_BYTE_NUM; jIndex++) {
                    bk_efuse_read_byte(efuse_addr[jIndex], &(device_id_read[jIndex]));
                    tx_buffer[3 + jIndex] = device_id_read[jIndex];
                }
                uart_send_bytes_for_ate(tx_buffer, 8);
                return BK_FAIL;
            }
        }
    }

    tx_buffer[1] = 0x88;
    for(int iIndex = 0; iIndex < EFUSE_DEVICE_ID_BYTE_NUM; iIndex++) {
        tx_buffer[2 + iIndex] = device_id_read[iIndex];
    }
    uart_send_bytes_for_ate(tx_buffer, 7);

    return BK_OK;
}
#endif

/**
 * command format 55 xx, range of xx:
 * 0x : device test cmd
 * 1x : evm / per / sleep
 * 2x : evm
 * 3x : rfpll
 * 4x : save vdddig / bandgap into efuse/otp
 * 5x : set registers for cali
 * 6x : memory / flash / otp check
 * 7x : ble
 * 9x :
 */
int bkreg_run_command1(unsigned char *content, int cnt)
{
    UINT8 tx_buffer[16];
    UINT8 addr, data;

    if (cnt < 2)
        return -1;
    if (content[0] == 0x55)
    {
        switch (content[1])
        {
            case 0x00:      // read compilation time
            {
                send_compile_time();
                break;
            }

            case 0x01:      // read chip ID
            {
                send_chip_id();
                break;
            }

            case 0x02:      // Voltage Output High at All GPIO Pins
            {
                gpio_voltage_output_high();
                break;
            }

            case 0x03:      // Voltage Output Low at All GPIO Pins
            {
                gpio_voltage_output_low();
                break;
            }

            case 0x04:      // Input High voltage to some GPIO
            {
                gpio_voltage_input_high();
                break;
            }

            case 0x05:      // Input High voltage to some GPIO
            {
                gpio_voltage_input_low();
                break;
            }
            case 0x06:      // read device ID
            {
                send_device_id();
                break;
            }
            case 0x10:      // Pout_Max_2400    135M MCS7 11n 20M
            {
                uint32_t offset = 0;
                if (cnt > 2)
                {
                    offset = (uint8_t)content[2];
                }
				evm_test(135, 2400 + offset, PHY_CHNL_BW_20, 0);
				break;
            }
            case 0x11:      // Pout_Max_2400    54M OFDM 11g
            {
                uint32_t offset = 0;
                if (cnt > 2)
                {
                    offset = (uint8_t)content[2];
                }
				evm_test(54, 2400 + offset, PHY_CHNL_BW_20, 0);
                break;
            }
            case 0x12:      // Pout_Max_2480    11M DSSS 11b
            {
                uint32_t offset = 80;
                if (cnt > 2)
                {
                    offset = (uint8_t)content[2];
                }
				evm_test(11, 2400 + offset, PHY_CHNL_BW_20, 0);
                break;
            }
            case 0x13:      // Pout_Max_2400    135M MCS7 11ax
            {
                uint32_t offset = 0;
                if (cnt > 2)
                {
                    offset = (uint8_t)content[2];
                }
				evm_test(135, 2400 + offset, PHY_CHNL_BW_20, 5);
                break;
            }
            case 0x20:      // BER_Min_CHxx_20M: 55 20 xx [ms=50]
            {
                uint32_t duration = 50;
                uint32_t channel = 1;
                uint32_t arg = 0x20;
                if (cnt > 3)
                {
                    duration = (uint8_t)content[3];
                    channel = (uint8_t)content[2];
                }
                else if (cnt > 2)
                {
                    channel = (uint8_t)content[2];
                }
                per_test(PHY_CHNL_BW_20, channel, duration, arg);
                break;
            }
            case 0x21:      // BER_Min_CHxx_40M: 55 21 xx [ms=50]
            {
                uint32_t duration = 50;
                uint32_t channel = 1;
                uint32_t arg = 0x21;
                if (cnt > 3)
                {
                    duration = (uint8_t)content[3];
                    channel = (uint8_t)content[2];
                }
                else if (cnt > 2)
                {
                    channel = (uint8_t)content[2];
                }
                per_test(PHY_CHNL_BW_40, channel, duration, arg);
                break;
            }
            case 0x22:      // Pout_Max_2442   135, PHY_CHNL_BW_40
            {
                uint32_t offset = 42;
                if (cnt > 2)
                {
                    offset = (uint8_t)content[2];
                }
				evm_test(135, 2400 + offset, PHY_CHNL_BW_40, 0);
                break;
            }
            case 0x23:      // // not test in ATE temporary,Pout_Max_2462    tx encrypt verify
            {
                tx_verify_test_2442();
                break;
            }
            case 0xA:
                /* 55 0A 01 : read efuse 18,19,20,21 first, return value */
                /* 55 0A 10: set 1Volt and read ADC3, return value */
                /* 55 0A 20: set 2Volt and read ADC3, return value */
                /* 55 0A 15: set 1.5Volt and read ADC3, return value */
                /* 55 0A 02: write efuse 18,19,20,21 */
                ate_cali_adc(content, tx_buffer);
                break;
            case 0xB:
                /* 55 0B 01 : read efuse 18,19,20,21 first, return value */
                /* 55 0B 10: set 1Volt and read ADC3, return value */
                /* 55 0B 20: set 2Volt and read ADC3, return value */
                /* 55 0B 15: set 1.5Volt and read ADC3, return value */
                /* 55 0B 02: write efuse 18,19,20,21 */
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
                ate_cali_sdmadc(content, tx_buffer);
#endif
                break;
            case 0x15:      // SARADC test
            {
                /* cali ADC3/GPIO23  */
                /* 55 15 01: set 1Volt and read ADC3, would save in efuse<18>efuse<19> */
                /* 55 15 02: set 2Volt and read ADC3, would save in efuse<20>efuse<21> */
                /* 55 15 03: verify with 1.5Volt, success: 55 33 failed: 55 CC invalid: 55 AA */
                sctrl_cal_adc(content, tx_buffer);
                break;
            }
            case 0x14:      // PER_Min_2484: 55 14 [ms=50]
            {
                clock_time_t duration;
                uint32_t arg = 0x16;
                if (cnt > 2)
                {
                    duration = content[2];
                }
                else
                {
                    duration = 50;
                }
                per_test(PHY_CHNL_BW_20, 14, duration, arg);
                break;
            }
            case 0x16:      //low voltage
            {
                ate_enter_low_voltage();
                break;
            }
            case 0x17:      //deep sleep
            {
                ate_enter_deep_sleep();
                break;
            }
            case 0x24:      // PER_Min_2442: 55 24 [ms=50]
            {
                clock_time_t duration;
                uint32_t arg = 0x24;
                if (cnt > 2)
                {
                    duration = content[2];
                }
                else
                {
                    duration = 50;
                }
                per_test(PHY_CHNL_BW_40, 7, duration, arg);
                break;
            }
            case 0x31:      // audio_loop test: 55 31 [01|02|00]
            {
                uint32_t state;
                state = (uint32_t)content[2];
                audio_loop_test(state);
                audio_adc_mic2_to_dac_test(state);    /* use for bk7258 mic2 test */
                break;
            }
            case 0x30:      //not test in ATE temporary
            /* 55 3D xx set single tone with channel */
            /* xx channel [00, 7F] */
            {
				//UPDATE PA/PAD
                //addTRX_BEKEN_Reg0x0c  = (addTRX_BEKEN_Reg0x0c & (~(0x0FF0<<0))) | (0x110<<0);
                evm_single_test(54, 2400 + content[2], 0);
                tx_buffer[0] = 0x55;
                ate_time_delay(1000);

				// return tssi, invalid at present
                tx_buffer[1] = 0 & 0x00FF;
                tx_buffer[2] = 0 & 0x00FF;
                uart_send_bytes_for_ate(tx_buffer, 3);
                break;
            }
            case 0x3D:      //not test in ATE temporary
            /* 55 3D xx yy set rate and power_gain */
            /* yy rate: 1,2,5,11 for 11B, 6,9,12,18,24,32,48,54 for 11G, 128,...135 for MCS 0-7 */
            /* xx power_gain: [0, 2E] for 11B; [0, 4D] for 11G */
            {
                UINT32 rate = (UINT8)content[2];
                UINT32 power_gain = (UINT8)content[3];

                if ((rate == 1) || (rate == 2) || (rate == 5) || (rate == 11))
                {
                    rate = 11;
                }
                else
                {
                    rate = 54;
                }

                rwnx_cal_set_txpwr(power_gain, rate);
                tx_buffer[0] = 0x55;
                tx_buffer[1] = 0x33;
                uart_send_bytes_for_ate(tx_buffer, 2);
                break;
            }

#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX))) && (defined(CONFIG_OTP))
            case 0x40: //read otp
            {
                if (content[2] == OTP_MAC_ADDRESS) {
                    data = 6;
                } else if (content[2] == OTP_VDDDIG_BANDGAP) {
                    data = 1;
                } else if (content[2] == OTP_DIA) {
                    data = 1;
                } else if (content[2] == OTP_GADC_CALIBRATION) {
	                data = 4;
                } else {
	                data = 0;
                }
                if (data) {
                    tx_buffer[0] = 0x0E;
                    os_memset(tx_buffer + 1, 0x00, (uint32_t)data);
                    bk_otp_apb_read(content[2], tx_buffer + 1, (uint32_t)data);
                    uart_send_bytes_for_ate(tx_buffer, data + 1);
                }
                break;
            }
#endif
            case 0x41:      //not test in ATE temporary
            {
                addr = 0;
                data = 0x0A;
                tx_buffer[0] = 0x55;
                if (bk_efuse_write_byte(addr, data) == 0)
                {
                    tx_buffer[1] = 0x33;
                }
                else
                {
                    tx_buffer[1] = 0xCC;
                }
                uart_send_bytes_for_ate(tx_buffer, 2);
                break;
            }
            case 0x42:
            {
                if (cnt > 2)
                {
                    addr = ((UINT8)content[2] & 0x1F);
                    data = 0;
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
                    if (addr == 0x11)
                    {
                        //BANDGAP in efuse for legacy
                        bk_otp_apb_read(OTP_VDDDIG_BANDGAP, &data, (uint32_t)sizeof(data));
                        tx_buffer[0] = 0x0E;
                        tx_buffer[1] = data;
                        uart_send_bytes_for_ate(tx_buffer, 2);
                        break;
                    }
#endif
                    bk_efuse_read_byte(addr, &data);
                    uart_send_bytes_for_ate(&data, 1);
                }
                else
                {
                    unsigned char i;
                    for (i=0; i<32; i++)
                    {
                        addr = i;
                        data = 0;
                        bk_efuse_read_byte(addr, &data);
                    }
                }
                break;
            }
            case 0x43:
                /* write 17th bytes in efuse for analog2<28:23> to cali vdddig */
                sctrl_set_bandgap_to_efuse(content, cnt, tx_buffer);
                break;
            case 0x44:
                /* write macaddr in efuse/OTP */
#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
                set_mac_address_to_efuse(content, cnt, tx_buffer);
#endif
                break;
#if 0
            case 0x50:
                if ((cnt > 2) && ((unsigned char) content[2] < 80))
                {
                    rwnx_cal_update_pregain(0 - content[2]);
                }
                break;
            case 0x51:
                if ((cnt > 2) && ((unsigned char) content[2] < 80))
                {
                    rwnx_cal_update_pregain(content[2]);
                }
                break;
#endif
            case 0x52:
                /* set analog2<28:23> to cali vdddig */
                /* 55 52 xx */
                sctrl_set_bandgap_to_reg(content, cnt, tx_buffer);
                break;
            case 0x53:
            /* set vdddig to 1.00V */
            {
                tx_buffer[0] = 0x55;
                if (sctrl_set_vdddig_1voltage() == 0)
                {
                    /* success */
                    tx_buffer[1] = 0x33;
                }
                else
                {
                    tx_buffer[1] = 0xCC;
                }
                uart_send_bytes_for_ate(tx_buffer, 2);
                break;
            }
            case 0x54:
                /* set SCTRL_REG0X4 vhsel_ldodig<31:29> */
                sctrl_set_vdddig_to_reg(content, cnt, tx_buffer);
                break;
#if 0
            case 0x55:
            {
                /* check dpll unlock */
                if (cnt > 2)
                {
                    /* success */
                    sctrl_check_dpll_unlock((unsigned char) content[2], tx_buffer);
                }
                else
                {
                    sctrl_check_dpll_unlock(0, tx_buffer);
                }
                uart_print(tx_buffer, 2);
                break;
            }
#endif	//TODO:temp disable.

            case 0x56:
                psram_function_test();
                break;

            case 0x57:
                /* set xtal: 55 57 xx */
                manual_cal_set_xtal((UINT32)content[2]);
                break;

            case 0x58:
                /* set trxA<31:28>_trxB<17:17> to cali Dia */
                /* 55 58 xx */
                sctrl_set_dia_to_reg(content, cnt, tx_buffer);
                break;

            case 0x59:
                /* save trxA<31:28>_trxB<17:17> to otp */
                /* 55 59 [xx] */
                sctrl_set_dia_to_efuse(content, cnt, tx_buffer);
                break;

            case 0x60:
                /* 55 60 memory check, return 55 33 if success, otherwise 55 CC */
#if defined(CONFIG_RESET_REASON)
                cmd_start_memcheck();
#endif
                break;

            case 0x61:
                tx_buffer[0] = 0x55;
                data = cmd_save_memcheck();
                if (data < 0) {
                    tx_buffer[1] = 0xCC;
                } else {
                    /* success */
                    tx_buffer[1] = 0x33;
                }
                uart_send_bytes_for_ate(tx_buffer, 2);
                break;

            case 0x62:
                /* 55 62 softreset for MBIST/ATPG, return 55 33 if success, otherwise 55 CC */
                if (cnt > 3) {
                    cmd_start_softreset((uint8_t)content[2], (uint8_t)content[3]);
                } else if (cnt > 2) {
                    cmd_start_softreset((uint8_t)content[2], 0xA);
                } else {
                    cmd_start_softreset(0x5, 0xA);
                }
                break;

#if ((defined(CONFIG_SOC_BK7236XX)) || (defined(CONFIG_SOC_BK7239XX)) || (defined(CONFIG_SOC_BK7286XX)))
            case 0x63:
            {
                uint32_t result;
                result = bk_otp_fully_flow_test();
                tx_buffer[0] = 0x55;
                tx_buffer[2] = (result & 0xFF00) >> 8;
                tx_buffer[3] = result & 0xFF;
                if (result == BK_OK) {
                    /* success */
                    tx_buffer[1] = 0x33;
                } else if (result & BK_ERR_OTP_OPERATION_ERROR_MASK) {
                    tx_buffer[1] = 0xC1;
                } else if (result & BK_ERR_OTP_OPERATION_FAIL_MASK) {
                    tx_buffer[1] = 0xC2;
                } else if (result & BK_ERR_OTP_INIT_FAIL_MASK) {
                    tx_buffer[1] = 0xC3;
                } else if (result & BK_ERR_OTP_FULL_ERROR_MASK) {
                    /*will not happen normally*/
                    tx_buffer[1] = 0xC4;
                } else {
                    /*auto repair return fail cnt,see tx_buffer[3] second nibble*/
                    tx_buffer[1] = 0xC5;
                }
                uart_send_bytes_for_ate(tx_buffer, 4);
                break;
            }
#endif
            case 0x70:
            {
                /* BLE DUT */
                evm_stop_bypass_mac();
                ble_dut_start(CONFIG_UART_PRINT_PORT);
                break;
            }

            case 0x80:
            {
                UINT8 usbbackbuffer[2];
                switch (content[2])
                {
                    case 0x01:
                    {
                        bk_usb_ate_voh_vol_test(1);
                        bk_usb_ate_voh_vol_test(2);
                        break;
                    }

                    case 0x02:
                    {
                        bk_usb_ate_voh_vol_test(1);
                        bk_usb_ate_voh_vol_test(3);
                        break;
                    }

                    case 0x03:
                    {
                        bk_usb_ate_rterm_test(1);
                        bk_usb_ate_rterm_test(2);
                        break;
                    }

                    case 0x04:
                    {
                        bk_usb_ate_rx_dc_input_test(1);
                        bk_usb_ate_rx_dc_input_test(2);
                        if(content[3] == 0x01)
                        {
                            if(bk_usb_ate_rx_dc_input_test(3) == BK_OK)
                            {
                                 usbbackbuffer[0] = 0x55;
                                 usbbackbuffer[1] = 0x33;
                            }else {
                                 usbbackbuffer[0] = 0x55;
                                 usbbackbuffer[1] = 0xCC;
                            }
                        }
                        if(content[3] == 0x02)
                        {
                            if(bk_usb_ate_rx_dc_input_test(3) == BK_OK)
                            {
                                 usbbackbuffer[0] = 0x55;
                                 usbbackbuffer[1] = 0x33;
                            }else {
                                 usbbackbuffer[0] = 0x55;
                                 usbbackbuffer[1] = 0xCC;
                            }
                        }
                        uart_send_bytes_for_ate(usbbackbuffer, 2);
                        break;
                    }
                    case 0x05:
                    {
                        if(bk_usb_ate_bist_test(1) == BK_OK)
                        {
                            usbbackbuffer[0] = 0x55;
                            usbbackbuffer[1] = 0x33;
                        }else {
                            usbbackbuffer[0] = 0x55;
                            usbbackbuffer[1] = 0xCC;
                        }
                        uart_send_bytes_for_ate(usbbackbuffer, 2);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case 0x99:
            {
                set_device_id_to_efuse(content, cnt, tx_buffer);
                break;
            }

            default:
                break;
        }
    }
    else
    {
        if ((content[0] == 0x01) && (content[1] == 0xE0) && (content[2] == 0xFC))
        {
            if (content[3] == cnt - 4)
            {
                unsigned long ulAddr;
                unsigned long ulData;
                switch (content[4])
                {
                    case 0x00:
                        tx_buffer[0] = 0x04;
                        tx_buffer[1] = 0x0E;
                        tx_buffer[2] = 0x01;
                        tx_buffer[3] = 0x00;
                        uart_send_bytes_for_ate(tx_buffer, 4);
                        break;

                    case 0x01:
                        ulAddr = ((content[5]<<0)  & 0x000000FFUL)
                               | ((content[6]<<8)  & 0x0000FF00UL)
                               | ((content[7]<<16) & 0x00FF0000UL)
                               | ((content[8]<<24) & 0xFF000000UL);
                        ulData = ((content[9]<<0)   & 0x000000FFUL)
                               | ((content[10]<<8)  & 0x0000FF00UL)
                               | ((content[11]<<16) & 0x00FF0000UL)
                               | ((content[12]<<24) & 0xFF000000UL);
                        *(volatile unsigned long *)ulAddr = ulData;
                        tx_buffer[0] = 0x04;
                        tx_buffer[1] = 0x0E;
                        tx_buffer[2] = 0x0C;
                        tx_buffer[3] = 0x01;
                        tx_buffer[4] = 0xE0;
                        tx_buffer[5] = 0xFC;
                        tx_buffer[6] = 0x01;
                        tx_buffer[7] = content[5];
                        tx_buffer[8] = content[6];
                        tx_buffer[9] = content[7];
                        tx_buffer[10] = content[8];
                        tx_buffer[11] = content[9];
                        tx_buffer[12] = content[10];
                        tx_buffer[13] = content[11];
                        tx_buffer[14] = content[12];
                        uart_send_bytes_for_ate(tx_buffer, 15);
                        break;

                    case 0x03:
                        ulAddr = ((content[5]<<0)  & 0x000000FFUL)
                               | ((content[6]<<8)  & 0x0000FF00UL)
                               | ((content[7]<<16) & 0x00FF0000UL)
                               | ((content[8]<<24) & 0xFF000000UL);
                        ulData = *(volatile unsigned long *)ulAddr;
                        tx_buffer[0] = 0x04;
                        tx_buffer[1] = 0x0E;
                        tx_buffer[2] = 0x0C;
                        tx_buffer[3] = 0x01;
                        tx_buffer[4] = 0xE0;
                        tx_buffer[5] = 0xFC;
                        tx_buffer[6] = 0x03;
                        tx_buffer[7] = content[5];
                        tx_buffer[8] = content[6];
                        tx_buffer[9] = content[7];
                        tx_buffer[10] = content[8];
                        tx_buffer[11] = (ulData >> 0)  & 0x000000FFUL;
                        tx_buffer[12] = (ulData >> 8)  & 0x000000FFUL;
                        tx_buffer[13] = (ulData >> 16) & 0x000000FFUL;
                        tx_buffer[14] = (ulData >> 24) & 0x000000FFUL;
                        uart_send_bytes_for_ate(tx_buffer, 15);
                        break;

                    case 0x37:
                        audio_ap_test_for_ate(content[5], content[6], content[7], content[8], content[9],  content[10]);
                        break;

                    case 0x77:
                        manual_cal_show_txpwr_tab();
                        break;
                    default:
                        break;
                }
            }
        }
    }
    if (content[0] == 0x4D && content[1] == 0x42)
		ate_test_multiple_cpus_case();

    return 0;
}
#else
int bkreg_run_command1(const unsigned char *content, int cnt)
{
    return 0;
}
#endif

