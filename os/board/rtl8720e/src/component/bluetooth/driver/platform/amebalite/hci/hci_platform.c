/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#include "osif.h"
#include "hci_config.h"
#include "hci_common.h"
#include "hci_uart.h"
#include "hci_platform.h"
#include "hci_dbg.h"
#include "ameba.h"
#include "dlist.h"

#define HCI_LGC_EFUSE_LEN          0x50
#define HCI_PHY_EFUSE_LEN          0x70
#define HCI_PHY_EFUSE_BASE         0x740
#define HCI_LGC_EFUSE_OFFSET       0x1B0
#define HCI_MAC_ADDR_LEN           6
#define HCI_CONFIG_SIGNATURE       0x8723ab55
#define HCI_CONFIG_HDR_LEN         6
#define HCI_PATCH_FRAG_SIZE        252
#define HCI_PATCH_FLASH_ADDRESS    0x080F8000

#define HCI_CFG_BAUDRATE           BIT0
#define HCI_CFG_FLOWCONTROL        BIT1
#define HCI_CFG_BD_ADDR            BIT2
#define LEFUSE(x)                  ((x)-HCI_LGC_EFUSE_OFFSET)

#define LE_ARRAY_TO_UINT16(u16, a)          \
	{                                       \
		u16 = ((uint16_t)(*(a + 0)) << 0) + \
			  ((uint16_t)(*(a + 1)) << 8);  \
	}

#define LE_ARRAY_TO_UINT32(u32, a)           \
	{                                        \
		u32 = ((uint32_t)(*(a + 0)) << 0) +  \
			  ((uint32_t)(*(a + 1)) << 8) +  \
			  ((uint32_t)(*(a + 2)) << 16) + \
			  ((uint32_t)(*(a + 3)) << 24);  \
	}

uint32_t hci_cfg_sw_val = 0xFF;    // Open BT Trace log & FW log use 0xDD

extern const unsigned char rtlbt_fw[];
extern unsigned int rtlbt_fw_len;
extern const unsigned char rtlbt_mp_fw[];
extern unsigned int rtlbt_mp_fw_len;

//static const uint8_t hci_patch_buf[] = {0xff, 0xff, 0xff, 0xff};
//static uint32_t hci_patch_buf_len    = sizeof(hci_patch_buf);
static uint8_t hci_phy_efuse[HCI_PHY_EFUSE_LEN]  = {0};
static uint8_t hci_lgc_efuse[HCI_LGC_EFUSE_LEN]  = {0};
static uint8_t hci_chipid_in_fw  = 0;
static uint8_t hci_key_id = 0;
unsigned char hci_init_config[] = {
	/* Header */
	0x55, 0xAB, 0x23, 0x87,

	/* Length */
	//0x3C, 0x00,
	0x40, 0x00,

	/* BT MAC address */
	0x30, 0x00, 0x06, 0x8A, 0xD5, 0x23, 0x4C, 0xE0, 0x00,

	/* LOG Uart Baudrate 115200 */
	0x08, 0x00, 0x04, 0x00, 0xC2, 0x01, 0x00,

#ifdef CONFIG_MP_INCLUDED
	/* HCI Uart Baudrate 115200 */
	0x0C, 0x00, 0x04, 0x1D, 0x70, 0x00, 0x00,
#else
	/* HCI Uart Baudrate 3M */
	0x0C, 0x00, 0x04, 0x01, 0x80, 0x92, 0x04,
	/* Uart Flow Control */
	0x18, 0x00, 0x01, 0x5C,
#endif

	/* BT Wake Host */
	0x42, 0x00, 0x01, 0x09,

	/* Host Wake BT */
	0x43, 0x00, 0x01, 0x07,

	/* RF: phy_flow_ctrl_para(1), iqm_txgaink_module(1), iqm_txgain_flatk_module(4), iqm_txgain_10dBm_raw_index(1), lbt_ant_gain(1) */
	0x78, 0x02, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x07,

	/* RF: iqm_max_txgain_1M(1), iqm_max_txgain_2M(1), iqm_max_txgain_3M(1), iqm_max_txgain_LE1M(1), iqm_max_txgain_LE2M(1), iqm_max_txgain_LE1M_adv(1), iqm_max_txgain_LE2M_adv(1), iqm_max_txgain_LR(1)*/
	0x82, 0x02, 0x08, 0x41, 0x45, 0x45, 0x41, 0x41, 0x45, 0x41, 0x41,

	/* RF: iqm_max_txgain_LE1M_2402(1), iqm_max_txgain_LE1M_2480(1), iqm_max_txgain_LE2M_2402(1), iqm_max_txgain_LE2M_2480(1)*/
	0x8A, 0x02, 0x04, 0x45, 0x45 , 0x45, 0x45
};

typedef struct {
	uint8_t *fw_buf;
	uint8_t fw_is_alloced;
	uint32_t fw_len;
	uint8_t *config_buf;
	uint8_t cfg_is_alloced;
	uint16_t config_len;
	uint16_t cur_index;
	uint16_t end_index;
	uint8_t last_pkt;
	uint32_t sent_len;
	uint8_t *patch_buf;
	uint32_t patch_len;
} HCI_PATCH_INFO;
static HCI_PATCH_INFO *hci_patch_info = NULL;

typedef struct {
	struct list_head list;
	uint8_t eco;
	uint8_t priority;
	uint8_t key_id;
	uint8_t reserve;
	uint32_t length;
	uint8_t *payload;
} HCI_PATCH_NODE;

static uint8_t  hci_cfg_bd_addr[HCI_MAC_ADDR_LEN] = {0};
static uint32_t hci_cfg_init_uart_baudrate        = 115200;
static uint32_t hci_cfg_work_uart_baudrate        = 3000000;
static uint8_t  hci_cfg_work_bt_baudrate[4]       = {0x01, 0x80, 0x92, 0x04};
static uint32_t hci_cfg_flag                      = 0;

extern void rltk_coex_bt_enable(u8 enable);

static uint32_t cal_bit_shift(uint32_t Mask)
{
	uint32_t i;
	for (i = 0; i < 31; i++) {
		if (((Mask >> i) & 0x1) == 1) {
			break;
		}
	}
	return (i);
}

static void set_reg_value(uint32_t reg_address, uint32_t Mask, uint32_t val)
{
	uint32_t shift = 0;
	uint32_t data = 0;
	data = HAL_READ32(reg_address, 0);
	shift = cal_bit_shift(Mask);
	data = ((data & (~Mask)) | (val << shift));
	HAL_WRITE32(reg_address, 0, data);
	data = HAL_READ32(reg_address, 0);
}

void hci_platform_bt_log_init(void)
{
	LOGUART_Relay_InitTypeDef LOGUART_Relay_InitStruct;
	LOGUART_Relay_StructInit(&LOGUART_Relay_InitStruct);
	LOGUART_Relay_ClearRxFifo(LOGUART_DEV);
	LOGUART_Relay_SetFormat(LOGUART_DEV, &LOGUART_Relay_InitStruct);
	LOGUART_Relay_SetBaud(LOGUART_DEV, 115200);
	LOGUART_Relay_RxCmd(LOGUART_DEV, ENABLE);
	LOGUART_AGGCmd(LOGUART_DEV, ENABLE);  /* No need on testchip, because AGG is default enable */
}

void hci_platform_bt_log_deinit(void)
{
	LOGUART_Relay_RxCmd(LOGUART_DEV, DISABLE);
}

void hci_platform_bt_fw_log_open(void)
{
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_5, ENABLE);
}

void hci_platform_bt_fw_log_close(void)
{
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_5, DISABLE);
}

void hci_platform_bt_trace_log_open(void)
{
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_3, ENABLE);
}

void hci_platform_bt_trace_log_close(void)
{
	LOGUART_AGGPathCmd(LOGUART_DEV, LOGUART_PATH_INDEX_3, DISABLE);
}

void hci_platform_cfg_bd_addr(uint8_t *bdaddr)
{
	for (uint8_t i = 0; i < HCI_MAC_ADDR_LEN; i++) {
		hci_cfg_bd_addr[i] = bdaddr[i];
	}

	hci_cfg_flag |= HCI_CFG_BD_ADDR;
}

static void hci_platform_convert_baudrate(uint32_t *bt_baudrate, uint32_t *uart_baudrate, uint8_t bt_to_uart)
{
	uint8_t i;

	const struct {
		uint32_t bt_baudrate;
		uint32_t uart_baudrate;
	} baudrate_map[] = {
		{0x0000701d, 115200},
		{0x0252C00A, 230400},
		{0x03F75004, 921600},
		{0x05F75004, 921600},
		{0x00005004, 1000000},
		{0x04928002, 1500000},
		{0x00005002, 2000000},
		{0x0000B001, 2500000},
		{0x04928001, 3000000},
		{0x052A6001, 3500000},
		{0x00005001, 4000000},
	};

	const uint32_t baudrate_map_len = sizeof(baudrate_map) / sizeof(baudrate_map[0]);

	if (bt_to_uart) {
		for (i = 0; i < baudrate_map_len; i++) {
			if (*bt_baudrate == baudrate_map[i].bt_baudrate) {
				break;
			}
		}

		if (i == baudrate_map_len) {
			HCI_ERR("Wrong Baudrate Selection! Use Default 115200!");
			i = 0;
		}
		*uart_baudrate = baudrate_map[i].uart_baudrate;
	} else {
		for (i = 0; i < baudrate_map_len; i++) {
			if (*uart_baudrate == baudrate_map[i].uart_baudrate) {
				break;
			}
		}

		if (i == baudrate_map_len) {
			HCI_ERR("Wrong Baudrate Selection! Use Default 115200!");
			i = 0;
		}
		*bt_baudrate = baudrate_map[i].bt_baudrate;
	}
}

int hci_platform_get_iqk_data(uint8_t *data, uint8_t len)
{
	memcpy(data, hci_phy_efuse, len);
	return HCI_SUCCESS;
}

static uint8_t hci_platform_read_efuse(void)
{
	uint8_t i, *pbuf;

	/* Read Logic Efuse */
	pbuf = osif_mem_alloc(RAM_TYPE_DATA_ON, 1024);
	if (!pbuf || _FAIL == EFUSE_LMAP_READ(pbuf)) {
		HCI_ERR("EFUSE_LMAP_READ failed");
		if (pbuf) {
			osif_mem_free(pbuf);
		}
		return HCI_FAIL;
	}

	memcpy(hci_lgc_efuse, pbuf + HCI_LGC_EFUSE_OFFSET, HCI_LGC_EFUSE_LEN);

	/* Read Physical Efuse */
	for (i = 0; i < HCI_PHY_EFUSE_LEN; i++) {
		EFUSE_PMAP_READ8(0, HCI_PHY_EFUSE_BASE + i, hci_phy_efuse + i, L25EOUTVOLTAGE);
	}
#if 0
	HCI_DBG("\n\rRead Logic Efuse: \n\r");
	for (i = 0; i < HCI_LGC_EFUSE_LEN; i++) {
		HCI_DBG("%x:", hci_lgc_efuse[i]);
	}
	HCI_DBG("\n\rRead Phy Efuse: \n\r");
	for (i = 0; i < HCI_PHY_EFUSE_LEN; i++) {
		HCI_DBG("%x:", hci_phy_efuse[i]);
	}
	HCI_DBG("\n\r");
#endif
	if (pbuf) {
		osif_mem_free(pbuf);
	}

	return HCI_SUCCESS;
}

extern uint8_t bt_manual_tx_power_gain_enable;
extern uint8_t bt_manual_gain_index;

static uint8_t hci_platform_parse_config(void)
{
	uint8_t *p, i;
	uint16_t entry_offset, entry_len;
	uint32_t temp_baud = 0;
	//uint16_t tx_flatk;

	if (sizeof(hci_init_config) <= HCI_CONFIG_HDR_LEN) {
		return HCI_IGNORE;
	}

	p = hci_init_config;
	if (HCI_CONFIG_SIGNATURE != *(uint32_t *)(p)) {
		return HCI_FAIL;
	}

	if (*(uint16_t *)(p + 4) != (uint16_t)(sizeof(hci_init_config) - HCI_CONFIG_HDR_LEN)) {
		/* Fix the len, just avoid the length is not corect */
		*(uint16_t *)(p + 4) = (uint16_t)(sizeof(hci_init_config) - HCI_CONFIG_HDR_LEN);
	}

	p += HCI_CONFIG_HDR_LEN;
	while (p < hci_init_config + sizeof(hci_init_config)) {
		memcpy((void *)&entry_offset, (void *)p, sizeof(uint16_t));
		entry_len = *(uint8_t *)(p + 2);
		p += 3;

		switch (entry_offset) {
		case 0x000c:
			memcpy((void *)&temp_baud, (void *)p, sizeof(uint32_t));
			/* MP Mode, Use Default: 115200 */
			if ((!CHECK_CFG_SW(EFUSE_SW_UPPERSTACK_SWITCH))) {
				hci_platform_convert_baudrate((uint32_t *)&temp_baud, &hci_cfg_init_uart_baudrate, 0);
			}
			hci_platform_convert_baudrate((uint32_t *)&temp_baud, &hci_cfg_work_uart_baudrate, 1);
			hci_platform_convert_baudrate((uint32_t *)hci_cfg_work_bt_baudrate, &hci_cfg_work_uart_baudrate, 0);
			/* TODO: Config BaudRate */
			memcpy((void *)p, (void *)&temp_baud, sizeof(uint32_t));
			break;
		case 0x0018:
			/* MP Mode, Close Flow Control */
			if ((!CHECK_CFG_SW(EFUSE_SW_UPPERSTACK_SWITCH))) {
				p[0] = p[0] & (~BIT2);
			}
			/* TODO: Config Flow Control */
			break;
		case 0x0030:
			/* Set ConfigBuf MacAddr, Use Customer Assign or Efuse */
			if (hci_cfg_flag & HCI_CFG_BD_ADDR) {
				for (i = 0; i < HCI_MAC_ADDR_LEN; i++) {
					p[i] = hci_cfg_bd_addr[i];
				}
			}
			else {
				if ((hci_lgc_efuse[4] != 0xff) || (hci_lgc_efuse[5] != 0xff) || (hci_lgc_efuse[6] != 0xff) || \
					(hci_lgc_efuse[7] != 0xff) || (hci_lgc_efuse[8] != 0xff) || (hci_lgc_efuse[9] != 0xff)) {
					for (i = 0; i < HCI_MAC_ADDR_LEN; i++) {
						p[i] = hci_lgc_efuse[4 + HCI_MAC_ADDR_LEN - 1 - i];
					}
				}
			}
			HCI_PRINT("Bluetooth init BT_ADDR in cfgbuf [%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
						p[5], p[4], p[3], p[2], p[1], p[0]);
			break;
		case 0x0278:

			if (hci_lgc_efuse[LEFUSE(0x1be)] == 0xff) {
				if (!(hci_phy_efuse[2] & BIT0)) {
					//tx_flatk = hci_phy_efuse[0x0a] | hci_phy_efuse[0x0b] << 8;
					//bt_flatk_8730d(tx_flatk);
				}
				break;
			} else {
				p[0] = hci_lgc_efuse[LEFUSE(0x1be)];
				if (hci_lgc_efuse[LEFUSE(0x1be)] & BIT1) {
					p[1] = hci_lgc_efuse[LEFUSE(0x1bf)];
				}

				if (hci_lgc_efuse[LEFUSE(0x1be)] & BIT2) {
					p[2] = hci_lgc_efuse[LEFUSE(0x1c0)];
					p[3] = hci_lgc_efuse[LEFUSE(0x1c1)];
					p[4] = hci_lgc_efuse[LEFUSE(0x1c2)];
					p[5] = hci_lgc_efuse[LEFUSE(0x1c3)];
					//tx_flatk = hci_lgc_efuse[LEFUSE(0x1a0)] | hci_lgc_efuse[LEFUSE(0x1a1)] << 8;
					//bt_flatk_8730d(tx_flatk);
				} else {
					if (!(hci_phy_efuse[2] & BIT0)) {
						//tx_flatk = hci_phy_efuse[0xa] | hci_phy_efuse[0xb] << 8;
						//bt_flatk_8730d(tx_flatk);
					}
				}

				if (hci_lgc_efuse[LEFUSE(0x1be)] & BIT5) {
					p[6] = hci_lgc_efuse[LEFUSE(0x1c4)];
					p[7] = hci_lgc_efuse[LEFUSE(0x1c5)];

				}
			}
			break;
		case 0x0282:
			for (i = 0; i < entry_len; i++) {
				if (bt_manual_tx_power_gain_enable) {
					printf("gain %02x ", bt_manual_gain_index);
					p[i] = bt_manual_gain_index;
				} else {
					if (hci_lgc_efuse[LEFUSE(0x1c8 + i)] != 0xff) {
						p[i] = hci_lgc_efuse[LEFUSE(0x1c8 + i)];
					}
				}
			}
			break;
		case 0x028a:
			for (i = 0; i < entry_len; i++) {
				if (bt_manual_tx_power_gain_enable) {
					printf("gain %02x ", bt_manual_gain_index);
					p[i] = bt_manual_gain_index;
				} else {
					if (hci_lgc_efuse[LEFUSE(0x1d0 + i)] != 0xff) {
						p[i] = hci_lgc_efuse[LEFUSE(0x1d0 + i)];
					}
				}
			}
			break;
		default:
			break;
		}

		p +=  entry_len;
	}

	return HCI_SUCCESS;
}

static void bt_power_on(void)
{
	set_reg_value(0x4100C200, BIT1, 1);				//enable BT Power Cut
	osif_delay(5);
	set_reg_value(0x4100C200, BIT17, 0);			//disable ISO of BT
	osif_delay(5);
	set_reg_value(0x4100C214, BIT2, 1);				//enable clk_osc4m_wlbt
	osif_delay(5);
	set_reg_value(0x4100C208, BIT2, 1);				//enable WL RFAFE control circuit
	osif_delay(5);
	set_reg_value(0x4100C480, BIT5 | BIT6, 3);		//enable RFAFE
	osif_delay(5);
	set_reg_value(0x4100C208, BIT1, 1);				//release BTON reset
	osif_delay(5);
	set_reg_value(0x4100C214, BIT2, 0);				//disable clk_osc4m_wlbt
	/* workaround for bt lps bug (keep active) */
	if (1) {
		osif_delay(5);
		set_reg_value(0x4100c280, BIT14, 1);  		//enable HOST_WAKE_BT No GPIO
		osif_delay(5);
		set_reg_value(0x4100c280, BIT13, 1);  		//HOST_WAKE_BT
	}

}

static void bt_power_off(void)
{
	set_reg_value(0x4100C208, BIT1, 0);				//release BTON reset
	osif_delay(5);
	set_reg_value(0x4100C200, BIT17, 1);			//enable ISO of BT
	osif_delay(5);
	set_reg_value(0x4100C200, BIT1, 0);				//disable BT Power Cut
}

void hci_platform_controller_reset(void)
{
	/* BT Controller Power */
	bt_power_on();
	osif_delay(5);

	/* GNT_BT */
	// set_reg_value(0x40000764, BIT9 | BIT10 | BIT11 | BIT12, 15);
	// osif_delay(5);

	HCI_DBG("BT Reset OK!");
}

bool rtk_bt_pre_enable(void)
{
	if (!(wifi_is_running(WLAN0_IDX) || wifi_is_running(WLAN1_IDX))) {
		HCI_ERR("WiFi is OFF! Please Restart BT after Wifi on!");
		return false;
	}

	return true;
}

bool rtk_bt_post_disable(void)
{
	return true;
}

uint8_t hci_platform_init(void)
{
	if (rtk_bt_pre_enable() == false) {
		HCI_ERR("rtk_bt_pre_enable fail!");
		return HCI_FAIL;
	}

	if (!CHECK_CFG_SW(EFUSE_SW_BT_FW_LOG)) {
		if (CHECK_CFG_SW(EFUSE_SW_TRACE_SWITCH)) {
			hci_platform_bt_log_init();
		}
		hci_platform_bt_fw_log_open();
		HCI_INFO("FW LOG OPEN");
	}

#if 1
	/* Read Efuse and Parse Configbuf */
	if (HCI_FAIL == hci_platform_read_efuse()) {
		return HCI_FAIL;
	}

	if (HCI_FAIL == hci_platform_parse_config()) {
		return HCI_FAIL;
	}
#endif

	/* BT Controller Reset */
	hci_platform_controller_reset();

	/* Export BT FW log to PA22 */
	// if (!CHECK_CFG_SW(EFUSE_SW_BT_FW_LOG)) {
	// 	Pinmux_Config(_PA_22, PINMUX_FUNCTION_BT_UART);
	// 	PAD_PullCtrl(_PA_22, GPIO_PuPd_UP);
	// }

	/* UART Init */
	if (HCI_FAIL == hci_uart_open()) {
		return HCI_FAIL;
	}

	/* Coex: TODO */
	//if (!wifi_driver_is_mp())
		//rltk_coex_bt_enable(1);

	return HCI_SUCCESS;
}

uint8_t hci_platform_deinit(void)
{
	/* BT Controller Power Off */
	bt_power_off();

	/* UART Deinit */
	hci_uart_close();

	if (!CHECK_CFG_SW(EFUSE_SW_BT_FW_LOG)) {
		hci_platform_bt_fw_log_close();
		if (CHECK_CFG_SW(EFUSE_SW_TRACE_SWITCH)) {
			hci_platform_bt_log_deinit();
		}
	}

	/* Coex */
	//if (!wifi_driver_is_mp())
		//rltk_coex_bt_enable(0);

	/* PowerSaving */
	rtk_bt_post_disable();

	return HCI_SUCCESS;
}

void hci_platform_record_chipid(uint8_t chipid)
{
	hci_chipid_in_fw = chipid;
}

void hci_platform_get_baudrate(uint8_t *baudrate, uint8_t len)
{
	/* memcpy */
	for (uint8_t i = 0; i < len; i++) {
		baudrate[i] = hci_cfg_work_bt_baudrate[i];
	}
}

uint8_t hci_platform_set_baudrate(void)
{
	hci_uart_set_bdrate(hci_cfg_work_uart_baudrate);
	osif_delay(10);

	return HCI_SUCCESS;
}

uint8_t hci_platform_dl_patch_init(void)
{
	hci_patch_info = osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(HCI_PATCH_INFO));
	if (!hci_patch_info) {
		return HCI_FAIL;
	}

	memset(hci_patch_info, 0, sizeof(HCI_PATCH_INFO));

	return HCI_SUCCESS;
}

void hci_platform_dl_patch_done(void)
{
	if (hci_patch_info->fw_is_alloced && hci_patch_info->fw_buf) {
		osif_mem_free(hci_patch_info->fw_buf);
	}
	hci_patch_info->fw_buf = NULL;

	if (hci_patch_info) {
		osif_mem_free(hci_patch_info);
	}
	hci_patch_info = NULL;
}

static uint8_t hci_platform_get_patch_project_id(uint8_t *p_buf)
{
	uint8_t opcode;
	uint8_t length;
	uint8_t data;

	opcode = *(--p_buf);

	while (opcode != 0xFF) {
		length = *(--p_buf);
		if (opcode == 0x00) {
			if (length != 1) {
				HCI_ERR("Project ID length error!");
				return 0xFF;
			} else {
				data = *(--p_buf);
				return data;
			}
			break;
		} else {
			p_buf -= length;
			opcode = *(--p_buf);
		}
	}

	HCI_ERR("Project ID not found!");
	return 0xFF;
}

static void hci_platform_insert_patch_queue(struct list_head *head, HCI_PATCH_NODE *p_patch_node)
{
	struct list_head *pos, *next;
	HCI_PATCH_NODE *node;

	if(!head || !p_patch_node) {
		return;
	}

	list_for_each_safe(pos, next, head) {
		node = list_entry(pos, HCI_PATCH_NODE, list);
		if(node->priority >= p_patch_node->priority)
			break;
	}

	__list_add(&p_patch_node->list, pos->prev, pos);
}

static void hci_platform_parse_section(uint8_t *p_buf, uint32_t *p_fw_len, SECTION_OPCODE opcode, bool *p_found_security_header, HCI_PATCH_NODE *p_patch_node_head)
{
	HCI_PATCH_NODE *patch_node;
	uint16_t number, reserve;
	uint8_t *position;
	uint8_t eco;
	uint32_t payload_length;

	LE_ARRAY_TO_UINT16(number, p_buf);

	position = p_buf + sizeof(number) + sizeof(reserve);
	for (uint16_t i = 0; i < number; i++) {
		eco = *(position);
		LE_ARRAY_TO_UINT32(payload_length, position + sizeof(patch_node->eco) + sizeof(patch_node->priority) + \
							sizeof(patch_node->key_id) + sizeof(patch_node->reserve));

		if (eco == hci_chipid_in_fw) {
			patch_node = (HCI_PATCH_NODE *)osif_mem_alloc(RAM_TYPE_DATA_ON, sizeof(HCI_PATCH_NODE));
			if (patch_node == NULL) {
				HCI_ERR("patch_node allocate fail!");
				return;
			}

			patch_node->eco = eco;
			patch_node->priority = *(position + sizeof(patch_node->eco));
			if (opcode == OPCODE_SECURITY_HEADER) {
				patch_node->key_id = *(position + sizeof(patch_node->eco) + sizeof(patch_node->priority));
			}
			patch_node->length = payload_length;
			patch_node->payload = position + sizeof(patch_node->eco) + sizeof(patch_node->priority) + \
									sizeof(patch_node->key_id) + sizeof(patch_node->reserve) + sizeof(patch_node->length);

			if (opcode == OPCODE_PATCH_SNIPPETS || opcode == OPCODE_DUMMY_HEADER) {
				hci_platform_insert_patch_queue(&p_patch_node_head->list, patch_node);
				*p_fw_len += payload_length;
			} else if (opcode == OPCODE_SECURITY_HEADER) {
				if (patch_node->key_id == hci_key_id) {
					hci_platform_insert_patch_queue(&p_patch_node_head->list, patch_node);
					*p_fw_len += payload_length;
					*p_found_security_header = true;
				} else {
					HCI_WARN("patch_node->key_id = 0x%x mismatch hci_key_id = 0x%x", patch_node->key_id, hci_key_id);
					osif_mem_free(patch_node);
				}
			}
		}

		position += sizeof(patch_node->eco) + sizeof(patch_node->priority) + \
					sizeof(patch_node->key_id) + sizeof(patch_node->reserve) + sizeof(patch_node->length) + payload_length;
	}
}

static uint32_t hci_platform_parse_patch(uint8_t *p_buf, HCI_PATCH_NODE *p_patch_node_head)
{
	uint32_t i;
	uint32_t section_num;
	uint8_t *section_start;
	uint32_t opcode, section_length;
	uint32_t fw_len = 0;
	bool found_security_header = false;

	LE_ARRAY_TO_UINT32(section_num, p_buf);

	if (section_num == 0) {
		HCI_ERR("Section num error!");
		return 0;
	} else {
		section_start = p_buf + sizeof(section_num);
		for (i = 0; i < section_num; i++) {
			LE_ARRAY_TO_UINT32(opcode, section_start);
			LE_ARRAY_TO_UINT32(section_length, section_start + sizeof(opcode));

			switch (opcode) {
				case OPCODE_PATCH_SNIPPETS:
					hci_platform_parse_section(section_start + sizeof(opcode) + sizeof(section_length), &fw_len, OPCODE_PATCH_SNIPPETS, NULL, p_patch_node_head);
					break;
				case OPCODE_DUMMY_HEADER:
					if (hci_key_id != 0) {
						HCI_WARN("hci_key_id = 0x%x, ignore", hci_key_id);
						break;
					}
					hci_platform_parse_section(section_start + sizeof(opcode) + sizeof(section_length), &fw_len, OPCODE_DUMMY_HEADER, NULL, p_patch_node_head);
					break;
				case OPCODE_SECURITY_HEADER:
					if (hci_key_id == 0) {
						HCI_WARN("hci_key_id = 0x%x, ignore", hci_key_id);
						break;
					}
					hci_platform_parse_section(section_start + sizeof(opcode) + sizeof(section_length), &fw_len, OPCODE_SECURITY_HEADER, &found_security_header, p_patch_node_head);
					break;
				case OPCODE_OTA_FLAG:
					HCI_WARN("OTA flag not support");
					break;
				default:
					HCI_ERR("Opcode error!");
					break;
			}
			section_start += sizeof(opcode) + sizeof(section_length) + section_length;
		}

		//if has key id but not found security header, parse dummy header again
		if (hci_key_id != 0 && found_security_header == false) {
			section_start = p_buf + sizeof(section_num);
			for (i = 0; i < section_num; i++) {
				LE_ARRAY_TO_UINT32(opcode, section_start);
				LE_ARRAY_TO_UINT32(section_length, section_start + sizeof(opcode));
				if (opcode == OPCODE_DUMMY_HEADER) {
					hci_platform_parse_section(section_start + sizeof(opcode) + sizeof(section_length), &fw_len, OPCODE_DUMMY_HEADER, NULL, p_patch_node_head);
				}
				section_start += sizeof(opcode) + sizeof(section_length) + section_length;
			}
		}
	}

	return fw_len;
}

static uint8_t hci_platform_get_patch_info(void)
{
	const uint8_t patch_sig_v1[]    = {0x52, 0x65, 0x61, 0x6C, 0x74, 0x65, 0x63, 0x68};	//V1 signature: Realtech
	const uint8_t patch_sig_v2[]    = {0x52, 0x54, 0x42, 0x54, 0x43, 0x6F, 0x72, 0x65};	//V2 signature: RTBTCore
	const uint8_t ext_section_sig[] = {0x51, 0x04, 0xFD, 0x77};							//Extension section signature
	HCI_PATCH_INFO *patch_info = hci_patch_info;
	bool ext_section_check;
	uint8_t project_id;
	uint32_t version_date, version_time;
	uint32_t fw_len, tmp_fw_len = 0;

	struct list_head *pos, *next;
	HCI_PATCH_NODE *node;
	HCI_PATCH_NODE patch_node_head;

	if (CHECK_CFG_SW(EFUSE_SW_USE_FLASH_PATCH)) {
		patch_info->patch_buf = (uint8_t *)(void *)rtlbt_fw;
		patch_info->patch_len = rtlbt_fw_len;
		ext_section_check = true;
	} else {
		patch_info->patch_buf = (uint8_t *)HCI_PATCH_FLASH_ADDRESS;
		ext_section_check = false;
	}

	if (ext_section_check) {
		if ((!memcmp(patch_info->patch_buf, patch_sig_v1, sizeof(patch_sig_v1))) && \
			(!memcmp(patch_info->patch_buf + patch_info->patch_len - sizeof(ext_section_sig), ext_section_sig, sizeof(ext_section_sig)))) {
			HCI_WARN("Signature check success: Merge patch v1 not support");
			return HCI_IGNORE;
		} else if ((!memcmp(patch_info->patch_buf, patch_sig_v2, sizeof(patch_sig_v2))) && \
			(!memcmp(patch_info->patch_buf + patch_info->patch_len - sizeof(ext_section_sig), ext_section_sig, sizeof(ext_section_sig)))) {
			HCI_INFO("Signature check success: Merge patch v2");

			project_id = hci_platform_get_patch_project_id(patch_info->patch_buf + patch_info->patch_len - sizeof(ext_section_sig));
			if (project_id != HCI_PATCH_PROJECT_ID) {
				HCI_ERR("Project ID 0x%02x check fail, No available patch!", project_id);
				return HCI_IGNORE;
			}
		} else {
			HCI_ERR("Signature check fail, No available patch!");
			return HCI_IGNORE;
		}
	} else {
		if (!memcmp(patch_info->patch_buf, patch_sig_v1, sizeof(patch_sig_v1))) {
			HCI_WARN("Signature check success: Merge patch v1 not support");
			return HCI_IGNORE;
		} else if (!memcmp(patch_info->patch_buf, patch_sig_v2, sizeof(patch_sig_v2))) {
			HCI_INFO("Signature check success: Merge patch v2");
		} else {
			HCI_ERR("Signature check fail, No available patch!");
			return HCI_IGNORE;
		}
	}

	LE_ARRAY_TO_UINT32(version_date, patch_info->patch_buf + sizeof(patch_sig_v2));
	LE_ARRAY_TO_UINT32(version_time, patch_info->patch_buf + sizeof(patch_sig_v2) + sizeof(version_date));
	HCI_INFO("FW Version: %d%d", version_date, version_time);

	memset(&patch_node_head, 0, sizeof(HCI_PATCH_NODE));
	INIT_LIST_HEAD(&patch_node_head.list);

	fw_len = hci_platform_parse_patch(patch_info->patch_buf + sizeof(patch_sig_v2) + sizeof(version_date) + sizeof(version_time), &patch_node_head);
	if (fw_len == 0) {
		HCI_ERR("Available patch not found!");
		return HCI_IGNORE;
	}

	patch_info->fw_len = fw_len;
	patch_info->fw_buf = osif_mem_alloc(RAM_TYPE_DATA_ON, fw_len);
	if (patch_info->fw_buf == NULL) {
		HCI_ERR("patch_info->fw_buf allocate fail!");
		return HCI_FAIL;
	}
	patch_info->fw_is_alloced = 1;

	list_for_each_safe(pos, next, &patch_node_head.list) {
		node = list_entry(pos, HCI_PATCH_NODE, list);
		memcpy(patch_info->fw_buf + tmp_fw_len, node->payload, node->length);
		tmp_fw_len += node->length;

		list_del_init(pos);
		osif_mem_free(node);
	}

	patch_info->config_buf = hci_init_config;
	patch_info->config_len = sizeof(hci_init_config);

	/* Calculate patch info */
	patch_info->end_index = (patch_info->fw_len + patch_info->config_len - 1) / HCI_PATCH_FRAG_SIZE;
	patch_info->last_pkt = (patch_info->fw_len + patch_info->config_len) % HCI_PATCH_FRAG_SIZE;
	if (patch_info->last_pkt == 0) {
		patch_info->last_pkt = HCI_PATCH_FRAG_SIZE;
	}

	return HCI_SUCCESS;
}

uint8_t hci_platform_get_patch_cmd_len(uint8_t *cmd_len)
{
	uint8_t ret;
	HCI_PATCH_INFO *patch_info = hci_patch_info;

	/* Download FW partial patch first time, get patch and info */
	if (0 == patch_info->cur_index) {
		ret = hci_platform_get_patch_info();
		if (HCI_SUCCESS != ret) {
			return ret;
		}
	}

	if (patch_info->cur_index == patch_info->end_index) {
		*cmd_len = patch_info->last_pkt + 1;
		return HCI_SUCCESS;
	}

	*cmd_len = HCI_PATCH_FRAG_SIZE + 1;

	return HCI_SUCCESS;
}

uint8_t hci_platform_get_patch_cmd_buf(uint8_t *cmd_buf, uint8_t cmd_len)
{
	HCI_PATCH_INFO *patch_info = hci_patch_info;
	uint8_t        *data_buf   = &cmd_buf[1];
	uint8_t         data_len   = cmd_len - 1;

	/* first byte is index */
	if (patch_info->cur_index >= 0x80) {
		cmd_buf[0] = (patch_info->cur_index - 0x80) % 0x7f + 1;
	} else {
		cmd_buf[0] = patch_info->cur_index % 0x80;
	}
	if (patch_info->cur_index == patch_info->end_index) {
		cmd_buf[0] |= 0x80;
	}

	if (patch_info->sent_len + data_len <= patch_info->fw_len) {
		/* within fw patch domain */
		memcpy(data_buf, patch_info->fw_buf + patch_info->sent_len, data_len);
	} else if ((patch_info->sent_len < patch_info->fw_len) && (patch_info->sent_len + data_len > patch_info->fw_len)) {
		/* need copy fw patch domain and config domain */
		memcpy(data_buf, patch_info->fw_buf + patch_info->sent_len, patch_info->fw_len - patch_info->sent_len);

		memcpy(data_buf + (patch_info->fw_len - patch_info->sent_len), patch_info->config_buf,
			data_len - (patch_info->fw_len - patch_info->sent_len));
	} else {
		memcpy(data_buf, patch_info->config_buf + (patch_info->sent_len - patch_info->fw_len), data_len);
	}

	patch_info->sent_len += data_len;
	patch_info->cur_index++;

	return HCI_SUCCESS;
}
