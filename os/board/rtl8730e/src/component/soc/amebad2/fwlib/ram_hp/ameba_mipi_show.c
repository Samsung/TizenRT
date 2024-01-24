#include "ameba_soc.h"
#include "ameba_mipi_display.h"
#include "osdep_service.h"

#define LCDC_TEST_IMG_BUF_X			480
#define LCDC_TEST_IMG_BUF_Y			800
/*
	480*800*4 = 1.47MB
	1024*768*4 = 3M
*/
#define LCDC_IMG_BUF_OFFSET			(10 << 20) /*offset is 10M In DDR */
#define LCDC_IMG_BUF_OFFSET1			(13 << 20) /*offset is 13M In DDR */
#define LCDC_IMG_BUF_OFFSET2			(16 << 20) /*offset is 16M In DDR */
#define LCDC_IMG_BUF_OFFSET3			(19 << 20) /*offset is 19M In DDR */
#define LCDC_IMG_BUF_ALIGNED64B(x)		(((x) & ~0x3F) + 0x40)/*Start Addr shall aligned to 64Byte*/
#define LCDC_IMG_BUF_SIZE			LCDC_IMG_BUF_ALIGNED64B(LCDC_TEST_IMG_BUF_X * LCDC_TEST_IMG_BUF_Y * 4)

#define RGB_RED 			0x00FF0000
#define RGB_GREEN 			0x0000FF00
#define RGB_BLUE 			0x000000FF
#define RGB_CYAN			0x0000FFFF
#define RGB_YELLOW			0x00FFFF00
#define RGB_TOMATO			0x00FF6347
#define RGB_BLACK 			0x00000000

#define RGB888_RED_MASK 		0x00FF0000
#define RGB888_GREEN_MASK 		0x0000FF00
#define RGB888_BLUE_MASK 		0x000000FF
#define RGB888_MASK			0x00FFFFFF
#define RGB666_MASK			0x003F3F3F
#define RGB565_MASK			0x0000FFFF
#define RGB555_MASK			0x00007FFF
#define RGB444_MASK			0x00000FFF

#define REGFLAG_DELAY					0xFC
#define REGFLAG_END_OF_TABLE				0xFD	// END OF REGISTERS MARKER

#define Mhz				1000000UL
#define T_LPX		5
#define T_HS_PREP	6
#define T_HS_TRAIL	8
#define T_HS_EXIT	7
#define T_HS_ZERO	10
#define FONT_WIDTH  8
#define FONT_HEIGHT 16
#define MIPI_DSI_RTNI		2//4
#define MIPI_DSI_HSA		4
#define MIPI_DSI_HBP		30
#define MIPI_DSI_HFP		30

#define MIPI_DSI_VSA		5
#define MIPI_DSI_VBP		20
#define MIPI_DSI_VFP		15

#define MIPI_FRAME_RATE		60
static int g_first_flag = 1;
/*=========MIPI IRQ Info=========*/
typedef struct MIPI_Irq {
	u32 IrqNum;
	u32 IrqData;
	u32 IrqPriority;
} MIPI_IRQInfo;

typedef struct {
	u8 dataid;
	u8 byte0;
	u8 byte1;
} cmd_header_t;

typedef struct LCM_setting_table {
	u8 cmd;
	u8 count;
	u8 para_list[128];
} LCM_setting_table_t;

typedef struct {
	u8 blue;
	u8 green;
	u8 red;
	u8 alpha;
} argb8888_t;

typedef struct {
	u8 blue;
	u8 green;
	u8 red;
} rgb888_t;

/*=========LCDC IRQ Info=========*/
typedef struct LCDC_Irq {
	u32 IrqNum;
	u32 IrqData;
	u32 IrqPriority;
} LCDC_IRQInfo ;

u8 *BackupLcdImgBuffer = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET);
u8 *LcdImgBuffer1 = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET1);
u8 *LcdImgBuffer2 = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET2);
u8 *LcdImgBuffer3 = (u8 *)(DDR_BASE + LCDC_IMG_BUF_OFFSET3);

LCDC_InitTypeDef LCDC_InitStruct;
MIPI_InitTypeDef MIPI_InitStruct_g;

u32 ST7701S_Init_Done_g = 0;
u32 ST7701S_Send_cmd_g = 1;

u32 MIPI_HACT_g = LCDC_TEST_IMG_BUF_X;
u32 MIPI_VACT_g = LCDC_TEST_IMG_BUF_Y;
u32 UnderFlowCnt = 0;
u32 vo_freq;
u32 LCDC_Show_Idx;

/*initialize the MIPI IRQ info*/
MIPI_IRQInfo MipiIrqInfo = {
	.IrqNum = MIPI_DSI_IRQ,
	.IrqPriority = INT_PRI_MIDDLE,
	.IrqData = (u32)MIPI,
};

/*initialize the LCDC IRQ info*/
LCDC_IRQInfo LcdcIrqInfo = {
	.IrqNum = LCDC_IRQ,
	.IrqPriority = INT_PRI_MIDDLE,
	.IrqData = (u32)LCDC,
};

const char font[10][FONT_HEIGHT] = {
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x22, 0x41, 0x49, 0x49, 0x36}, // 3
    {0x0C, 0x0A, 0x09, 0x7F, 0x08}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3E, 0x49, 0x49, 0x49, 0x32}, // 6
    {0x01, 0x01, 0x71, 0x09, 0x07}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x26, 0x49, 0x49, 0x49, 0x3E}  // 9
};

const char fps[3][FONT_HEIGHT] = {
  //  {0x3F, 0x20, 0x3F, 0x20, 0x20},
   // {0xFF, 0x81, 0xFF, 0x81, 0x80},
   // {0xFF, 0x80, 0xFf, 0x01, 0xFF},
   {0x3E, 0x09, 0x09, 0x09, 0x01}, // F
   {0x7E, 0x11, 0x11, 0x11, 0x0E}, // P
   {0x2E, 0x49, 0x49, 0x09, 0x3A} //S
};

/* Resulation : 480x800
 * Inversion  : 2dot
 * Porch      : 125>vbp>=12, vfp>=20
 * Line Time  : 20us
 * Frame Rate : 60hz
 */
static LCM_setting_table_t ST7701S_init_cmd_g[] = {/* DCS Write Long */
	/* ST7701S Reset Sequence */
	/* LCD_Nreset (1); Delayms (1); */
	/* LCD_Nreset (0); Delayms (1); */
	/* LCD_Nreset (1); Delayms (120); */
	{0x11, 0, {0x00}},
	{REGFLAG_DELAY, 120, {}},/* Delayms (120) */

	/* Bank0 Setting */
	/* Display Control setting */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x10}},
	{0xC0, 2, {0x63, 0x00}},
	{0xC1, 2, {0x0C, 0x02}},
	{0xC2, 2, {0x31, 0x08}},
	{0xCC, 1, {0x10}},

	/* Gamma Cluster Setting */
	{0xB0, 16, {0x40, 0x02, 0x87, 0x0E, 0x15, 0x0A, 0x03, 0x0A, 0x0A, 0x18, 0x08, 0x16, 0x13, 0x07, 0x09, 0x19}},
	{0xB1, 16, {0x40, 0x01, 0x86, 0x0D, 0x13, 0x09, 0x03, 0x0A, 0x09, 0x1C, 0x09, 0x15, 0x13, 0x91, 0x16, 0x19}},
	/* End Gamma Setting */
	/* End Display Control setting */
	/* End Bank0 Setting */

	/* Bank1 Setting */
	/* Power Control Registers Initial */
	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x11}},
	{0xB0, 1, {0x4D}},

	/* Vcom Setting */
	{0xB1, 1, {0x64}},
	/* End End Vcom Setting */

	{0xB2, 1, {0x07}},
	{0xB3, 1, {0x80}},
	{0xB5, 1, {0x47}},
	{0xB7, 1, {0x85}},
	{0xB8, 1, {0x21}},
	{0xB9, 1, {0x10}},
	{0xC1, 1, {0x78}},
	{0xC2, 1, {0x78}},
	{0xD0, 1, {0x88}},
	/* End Power Control Registers Initial */
	{REGFLAG_DELAY, 100, {}},/* Delayms (100) */
	/* GIP Setting */
	{0xE0, 3, {0x00, 0x84, 0x02}},
	{0xE1, 11, {0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20}},
	{0xE2, 13, {0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0xE3, 4, {0x00, 0x00, 0x33, 0x33}},
	{0xE4, 2, {0x44, 0x44}},
	{0xE5, 16, {0x09, 0x31, 0xBE, 0xA0, 0x0B, 0x31, 0xBE, 0xA0, 0x05, 0x31, 0xBE, 0xA0, 0x07, 0x31, 0xBE, 0xA0}},
	{0xE6, 4, {0x00, 0x00, 0x33, 0x33}},
	{0xE7, 2, {0x44, 0x44}},
	{0xE8, 16, {0x08, 0x31, 0xBE, 0xA0, 0x0A, 0x31, 0xBE, 0xA0, 0x04, 0x31, 0xBE, 0xA0, 0x06, 0x31, 0xBE, 0xA0}},
	{0xEA, 16, {0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00}},
	{0xEB, 7, {0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00}},
	{0xEC, 2, {0x02, 0x00}},
	{0xED, 16, {0xF5, 0x47, 0x6F, 0x0B, 0x8F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xF8, 0xB0, 0xF6, 0x74, 0x5F}},
	{0xEF, 12, {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
	/* End GIP Setting */

	{0xFF, 5, {0x77, 0x01, 0x00, 0x00, 0x00}},
	{0x29, 0, {0x00}},
	{REGFLAG_END_OF_TABLE, 0x00, {}},
};

void Mipi_LCM_Set_Reset_Pin(u8 Newstatus)
{
	u32 pin_name = _PA_14;

	Pinmux_Swdoff();
	Pinmux_Config(pin_name, PINMUX_FUNCTION_GPIO);

	GPIO_InitTypeDef ResetPin;
	ResetPin.GPIO_Pin = pin_name;
	ResetPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	ResetPin.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(&ResetPin);

	/* Reset */
	if (Newstatus) {
		GPIO_WriteBit(pin_name, 1);
	} else {
		GPIO_WriteBit(pin_name, 0);
	}
}

void MIPI_InitStruct_Config(MIPI_InitTypeDef *MIPI_InitStruct)
{
	u32 vtotal, htotal_bits, bit_per_pixel, overhead_cycles, overhead_bits, total_bits;

	switch (MIPI_InitStruct->MIPI_VideoDataFormat) {
	case MIPI_VIDEO_DATA_FORMAT_RGB565:
		bit_per_pixel = 16;
		break;
	case MIPI_VIDEO_DATA_FORMAT_RGB666_PACKED:
		bit_per_pixel = 18;
		break;
	case MIPI_VIDEO_DATA_FORMAT_RGB666_LOOSELY:
	case MIPI_VIDEO_DATA_FORMAT_RGB888:
	default:
		bit_per_pixel = 24;
		break;
	}

	MIPI_InitStruct->MIPI_LaneNum = 2;
	MIPI_InitStruct->MIPI_FrameRate = MIPI_FRAME_RATE;

	MIPI_InitStruct->MIPI_HSA = MIPI_DSI_HSA * bit_per_pixel / 8 ;//- 10; /* here the unit is pixel but not us */
	if (MIPI_InitStruct->MIPI_VideoModeInterface == MIPI_VIDEO_NON_BURST_MODE_WITH_SYNC_PULSES) {
		MIPI_InitStruct->MIPI_HBP = MIPI_DSI_HBP * bit_per_pixel / 8 ;//- 10;
	} else {
		MIPI_InitStruct->MIPI_HBP = (MIPI_DSI_HSA + MIPI_DSI_HBP) * bit_per_pixel / 8 ;//-10 ;
	}

	MIPI_InitStruct->MIPI_HACT = MIPI_HACT_g;
	MIPI_InitStruct->MIPI_HFP = MIPI_DSI_HFP * bit_per_pixel / 8 ;//-12;

	MIPI_InitStruct->MIPI_VSA = MIPI_DSI_VSA;
	MIPI_InitStruct->MIPI_VBP = MIPI_DSI_VBP;
	MIPI_InitStruct->MIPI_VACT = MIPI_VACT_g;
	MIPI_InitStruct->MIPI_VFP = MIPI_DSI_VFP;

	/*DataLaneFreq * LaneNum = FrameRate * (VSA+VBP+VACT+VFP) * (HSA+HBP+HACT+HFP) * PixelFromat*/
	vtotal = MIPI_InitStruct->MIPI_VSA + MIPI_InitStruct->MIPI_VBP + MIPI_InitStruct->MIPI_VACT + MIPI_InitStruct->MIPI_VFP;
	htotal_bits = (MIPI_DSI_HSA + MIPI_DSI_HBP + MIPI_InitStruct->MIPI_HACT + MIPI_DSI_HFP) * bit_per_pixel;
	overhead_cycles = T_LPX + T_HS_PREP + T_HS_ZERO + T_HS_TRAIL + T_HS_EXIT;
	overhead_bits = overhead_cycles * MIPI_InitStruct->MIPI_LaneNum * 8;
	total_bits = htotal_bits + overhead_bits;

	MIPI_InitStruct->MIPI_VideDataLaneFreq = MIPI_InitStruct->MIPI_FrameRate * total_bits * vtotal / MIPI_InitStruct->MIPI_LaneNum / Mhz + 20;

	MIPI_InitStruct->MIPI_LineTime = (MIPI_InitStruct->MIPI_VideDataLaneFreq * Mhz) / 8 / MIPI_InitStruct->MIPI_FrameRate / vtotal;
	MIPI_InitStruct->MIPI_BllpLen = MIPI_InitStruct->MIPI_LineTime / 2;

	if (MIPI_DSI_HSA + MIPI_DSI_HBP + MIPI_HACT_g + MIPI_DSI_HFP < (512 + MIPI_DSI_RTNI * 16)) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "!!ERROR!!, LCM NOT SUPPORT\n");
	}

	if (MIPI_InitStruct->MIPI_LineTime * MIPI_InitStruct->MIPI_LaneNum < total_bits / 8) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "!!ERROR!!, LINE TIME TOO SHORT!\n");
	}

	if (MIPI_InitStruct->MIPI_VideDataLaneFreq * 2 / 24 >= vo_freq) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "%x %x\n", MIPI_InitStruct->MIPI_VideDataLaneFreq * 2 / 24, vo_freq);
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "!!ERROR!!, VO CLK too slow!\n");
	}

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "DataLaneFreq: %d, LineTime: %d\n", MIPI_InitStruct->MIPI_VideDataLaneFreq, MIPI_InitStruct->MIPI_LineTime);
}

void MipiDsi_ST7701S_isr(void)
{
	MIPI_TypeDef *MIPIx = MIPI;
	u32 reg_val, reg_val2, reg_dphy_err;

	reg_val = MIPI_DSI_INTS_Get(MIPIx);
	MIPI_DSI_INTS_Clr(MIPIx, reg_val);

	reg_val2 = MIPI_DSI_INTS_ACPU_Get(MIPIx);
	MIPI_DSI_INTS_ACPU_Clr(MIPIx, reg_val2);

	if (reg_val & MIPI_BIT_CMD_TXDONE) {
		reg_val &= ~MIPI_BIT_CMD_TXDONE;

		ST7701S_Send_cmd_g = 1;
	}

	if (reg_val & MIPI_BIT_ERROR) {
		reg_dphy_err = MIPIx->MIPI_DPHY_ERR;
		MIPIx->MIPI_DPHY_ERR = reg_dphy_err;
		if (g_first_flag != 1) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "LPTX Error: 0x%x, DPHY Error: 0x%x\n", reg_val, reg_dphy_err);
		}
		if (MIPIx->MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT & MIPI_MASK_DETECT_ENABLE) {
			MIPIx->MIPI_CONTENTION_DETECTOR_AND_STOPSTATE_DT &= ~MIPI_MASK_DETECT_ENABLE;

			MIPIx->MIPI_DPHY_ERR = reg_dphy_err;
			MIPI_DSI_INTS_Clr(MIPIx, MIPI_BIT_ERROR);
			if (g_first_flag != 1) {
				DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "LPTX Error CLR: 0x%x, DPHY: 0x%x\n", MIPIx->MIPI_INTS, MIPIx->MIPI_DPHY_ERR);
			}
		}

		if (MIPIx->MIPI_DPHY_ERR == reg_dphy_err) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "LPTX Still Error\n");
			MIPI_DSI_INT_Config(MIPIx, ENABLE, DISABLE, FALSE);
		}
		reg_val &= ~MIPI_BIT_ERROR;
	}

	if (reg_val) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "LPTX Error Occur: 0x%x\n", reg_val);
	}

	if (reg_val2) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "#\n");
	}
}

void MipiDsi_ST7701S_Send_DCS(MIPI_TypeDef *MIPIx, u8 cmd, u8 payload_len, u8 *para_list)
{
	u32 word0, word1, addr, idx;
	u8 cmd_addr[128];

	if (payload_len == 0) {
		MIPI_DSI_CMD_Send(MIPIx, MIPI_DSI_DCS_SHORT_WRITE, cmd, 0);
		return;
	} else if (payload_len == 1) {
		MIPI_DSI_CMD_Send(MIPIx, MIPI_DSI_DCS_SHORT_WRITE_PARAM, cmd, para_list[0]);
		return;
	}

	cmd_addr[0] = cmd;
	for (idx = 0; idx < payload_len; idx++) {
		cmd_addr[idx + 1 ] = para_list[idx];
	}

	payload_len = payload_len + 1;

	/* the addr payload_len 1 ~ 8 is 0 */
	for (addr = 0; addr < (u32)(payload_len + 7) / 8; addr++) {
		idx = addr * 8;
		word0 = (cmd_addr[idx + 3] << 24) + (cmd_addr[idx + 2] << 16) + (cmd_addr[idx + 1] << 8) + cmd_addr[idx + 0];
		word1 = (cmd_addr[idx + 7] << 24) + (cmd_addr[idx + 6] << 16) + (cmd_addr[idx + 5] << 8) + cmd_addr[idx + 4];

		MIPI_DSI_CMD_LongPkt_MemQWordRW(MIPIx, addr, &word0, &word1, FALSE);
	}

	MIPI_DSI_CMD_Send(MIPIx, MIPI_DSI_DCS_LONG_WRITE, payload_len, 0);
}

void MipiDsi_ST7701S_Send_Cmd(MIPI_TypeDef *MIPIx, LCM_setting_table_t *table)
{
	static u8 send_cmd_idx_s = 0;
	u32 payload_len;
	u8 cmd, send_flag = FALSE;
	u8 *cmd_addr;

	while (1) {
		cmd = table[send_cmd_idx_s].cmd;

		switch (cmd) {
		case REGFLAG_DELAY:
			DelayMs(table[send_cmd_idx_s].count);
			break;
		case REGFLAG_END_OF_TABLE:
			send_cmd_idx_s = 0;
			DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "LPTX CMD Send Done\n");
			ST7701S_Init_Done_g = 1;
			g_first_flag = 0 ;
			return;
		default:
			if (send_flag) {
				return;
			}
			cmd_addr = table[send_cmd_idx_s].para_list;
			payload_len = table[send_cmd_idx_s].count;

			MipiDsi_ST7701S_Send_DCS(MIPIx, cmd, payload_len, cmd_addr);

			send_flag = TRUE;
		}
		send_cmd_idx_s++;
	}
}

void MipiDsi_ST7701S_push_table(MIPI_TypeDef *MIPIx, MIPI_InitTypeDef *MIPI_InitStruct, LCM_setting_table_t *table)
{
	MIPI_DSI_TO1_Set(MIPIx, DISABLE, 0);
	MIPI_DSI_TO2_Set(MIPIx, ENABLE, 0x7FFFFFFF);
	MIPI_DSI_TO3_Set(MIPIx, DISABLE, 0);

	/*register MIPI ISR*/
	InterruptDis(MipiIrqInfo.IrqNum);
	InterruptUnRegister(MipiIrqInfo.IrqNum);
	InterruptRegister((IRQ_FUN)MipiDsi_ST7701S_isr, MipiIrqInfo.IrqNum, (u32)MipiIrqInfo.IrqData, MipiIrqInfo.IrqPriority);
	InterruptEn(MipiIrqInfo.IrqNum, MipiIrqInfo.IrqPriority);
	MIPI_DSI_INT_Config(MIPIx, DISABLE, ENABLE, FALSE);

	MIPI_DSI_init(MIPIx, MIPI_InitStruct);

	ST7701S_Init_Done_g = FALSE;
	ST7701S_Send_cmd_g = TRUE;

	while (1) {
		if (ST7701S_Send_cmd_g) {
			ST7701S_Send_cmd_g = 0;

			if (!ST7701S_Init_Done_g) {
				MipiDsi_ST7701S_Send_Cmd(MIPIx, table);
				//vTaskDelay(1);
				rtw_mdelay_os(1);
				
			} else {
				break;
			}
		}
	}
}

void MipiDsi_ST7701S_lcm_init(void)
{
	MIPI_TypeDef *MIPIx = MIPI;
	MIPI_InitTypeDef *MIPI_InitStruct = &MIPI_InitStruct_g;

	MIPI_StructInit(MIPI_InitStruct);
	MIPI_InitStruct_Config(MIPI_InitStruct);
	MIPI_Init(MIPIx, MIPI_InitStruct);

	/* to prevent electric leakage */
	Mipi_LCM_Set_Reset_Pin(1);
	DelayMs(10);
	Mipi_LCM_Set_Reset_Pin(0);
	DelayMs(10);
	Mipi_LCM_Set_Reset_Pin(1);
	DelayMs(120);

	MipiDsi_ST7701S_push_table(MIPIx, MIPI_InitStruct, ST7701S_init_cmd_g);
	MIPI_DSI_INT_Config(MIPIx, DISABLE, DISABLE, FALSE);
}

void LcdcEnable(void)
{
	LCDC_TypeDef *pLCDC = LCDC;

	/*enable the LCDC*/
	LCDC_Cmd(pLCDC, ENABLE);
	while (!LCDC_CheckLCDCReady(pLCDC));

//	MIPI_TypeDef *MIPIx = MIPI;
//	MIPI_InitTypeDef *MIPI_InitStruct = &MIPI_InitStruct_g;
//	MIPI_StructInit(MIPI_InitStruct);
//	MIPI_InitStruct_Config(MIPI_InitStruct);
//	MIPI_Init(MIPIx, MIPI_InitStruct);

	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "MIPI Switch video mode!\n");
	MIPI_DSI_Mode_Switch(MIPI, ENABLE);
}

void LcdcInitConfig(void)
{
	LCDC_StructInit(&LCDC_InitStruct);
	LCDC_InitStruct.LCDC_ImageWidth = LCDC_TEST_IMG_BUF_X;
	LCDC_InitStruct.LCDC_ImageHeight = LCDC_TEST_IMG_BUF_Y;

	rgb888_t bg_color;
	*(u32 *)&bg_color = RGB_TOMATO;
	LCDC_InitStruct.LCDC_BgColorRed = bg_color.red;
	LCDC_InitStruct.LCDC_BgColorGreen = bg_color.green;
	LCDC_InitStruct.LCDC_BgColorBlue = bg_color.blue;

	for (u8 idx = 0; idx < LCDC_LAYER_MAX_NUM; idx++) {
		LCDC_InitStruct.layerx[idx].LCDC_LayerEn = ENABLE;
		LCDC_InitStruct.layerx[idx].LCDC_LayerImgFormat = LCDC_LAYER_IMG_FORMAT_ARGB8888;
		LCDC_InitStruct.layerx[idx].LCDC_LayerImgBaseAddr = (u32)LcdImgBuffer1;
		LCDC_InitStruct.layerx[idx].LCDC_LayerHorizontalStart = 1;/*1-based*/
		LCDC_InitStruct.layerx[idx].LCDC_LayerHorizontalStop = LCDC_TEST_IMG_BUF_X;
		LCDC_InitStruct.layerx[idx].LCDC_LayerVerticalStart = 1;/*1-based*/
		LCDC_InitStruct.layerx[idx].LCDC_LayerVerticalStop = LCDC_TEST_IMG_BUF_Y;
		LCDC_InitStruct.layerx[idx].LCDC_LayerColorKeyingEn = ENABLE;
	}

	LCDC_Init(LCDC, &LCDC_InitStruct);
}

void Lcdc_display_pure(u32 *LcdImgBuffer)
{
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888
	u32 idx_x, idx_y, idx, value32;
	value32 = RGB_CYAN;

	for (idx_x = 0; idx_x < LCDC_TEST_IMG_BUF_X; idx_x++) {
		for (idx_y = 0; idx_y < LCDC_TEST_IMG_BUF_Y; idx_y++) {
			idx = idx_y * LCDC_TEST_IMG_BUF_X + idx_x;
			pImgBuf[idx] = *(argb8888_t *)&value32;
		}
	}
	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}
void Lcdc_display_fps_word(u32 *LcdImgBuffer, u32 x, u32 y, u32 value32)
{
	
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888
	u32 idx_x, idx_y, idx; 
	int i = 0;
	while (i < 3){
		for (idx_x = 0; idx_x < FONT_WIDTH; idx_x++) {
			for (idx_y = 0; idx_y < FONT_HEIGHT; idx_y++) {
				idx = (y + idx_y) *  LCDC_TEST_IMG_BUF_X + (x + idx_x);
				if((fps[i][idx_y] >> (FONT_WIDTH - 1 - idx_x)) & 0x01) {
					
		            		pImgBuf[idx] = *(argb8888_t *)&value32;
		        	}
				
			}
		}
		y+= FONT_WIDTH;
		i++;
		
	}
	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}

void Lcdc_display_fps(u32 *LcdImgBuffer, u32 x, u32 y, u8 character, u32 value32, u8 fps_word)
{
	
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888
	u32 idx_x, idx_y, idx; 
	if (fps_word){
		Lcdc_display_fps_word(LcdImgBuffer, x, y - 4 * FONT_WIDTH, value32);
	}
	for (idx_x = 0; idx_x < FONT_WIDTH; idx_x++) {
		for (idx_y = 0; idx_y < FONT_HEIGHT; idx_y++) {
			idx = (y + idx_y) *  LCDC_TEST_IMG_BUF_X + (x + idx_x);
			if((font[character - '0'][idx_y] >> (FONT_WIDTH - 1 - idx_x)) & 0x01) {
				
                    		pImgBuf[idx] = *(argb8888_t *)&value32;
                	}
			
		}
	}
	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}

void Lcdc_display_column(u32 *LcdImgBuffer)
{
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888
	u32 idx_x, idx_y, idx, value32;
	for (idx_x = 0; idx_x < LCDC_TEST_IMG_BUF_X; idx_x++) {
		for (idx_y = 0; idx_y < LCDC_TEST_IMG_BUF_Y; idx_y++) {
			idx = idx_y * LCDC_TEST_IMG_BUF_X + idx_x;

			if (idx_x < LCDC_TEST_IMG_BUF_X / 4) {
				value32 = RGB_RED;
			} else if (idx_x < LCDC_TEST_IMG_BUF_X / 4 * 2) {
				value32 = RGB_GREEN;
			} else if (idx_x < LCDC_TEST_IMG_BUF_X / 4 * 3) {
				value32 = RGB_BLUE;
			} else {
				value32 = RGB_YELLOW;
			}
			pImgBuf[idx] = *(argb8888_t *)&value32;
		}
	}

	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}

void Lcdc_display_row(u32 *LcdImgBuffer)
{
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888
	u32 idx_x, idx_y, idx, value32;
	for (idx_x = 0; idx_x < LCDC_TEST_IMG_BUF_X; idx_x++) {
		for (idx_y = 0; idx_y < LCDC_TEST_IMG_BUF_Y; idx_y++) {
			idx = idx_y * LCDC_TEST_IMG_BUF_X + idx_x;

			if (idx_y < LCDC_TEST_IMG_BUF_Y / 4) {
				value32 = RGB_RED;
			} else if (idx_y < LCDC_TEST_IMG_BUF_Y / 4 * 2) {
				value32 = RGB_GREEN;
			} else if (idx_y < LCDC_TEST_IMG_BUF_Y / 4 * 3) {
				value32 = RGB_BLUE;
			} else {
				value32 = RGB_YELLOW;
			}
			pImgBuf[idx] = *(argb8888_t *)&value32;
		}
	}

	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}

void Lcdc_display_grid(u32 *LcdImgBuffer)
{
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888

	u32 idx_x, idx_y, idx, value32;
	for (idx_x = 0; idx_x < LCDC_TEST_IMG_BUF_X; idx_x++) {
		for (idx_y = 0; idx_y < LCDC_TEST_IMG_BUF_Y; idx_y++) {
			idx = idx_y * LCDC_TEST_IMG_BUF_X + idx_x;

			if (idx_x < LCDC_TEST_IMG_BUF_X / 2 && idx_y < LCDC_TEST_IMG_BUF_Y / 2) {
				value32 = RGB_RED;
			}
			if (idx_x >= LCDC_TEST_IMG_BUF_X / 2 && idx_y < LCDC_TEST_IMG_BUF_Y / 2) {
				value32 = RGB_GREEN;
			}
			if (idx_x < LCDC_TEST_IMG_BUF_X / 2 && idx_y >= LCDC_TEST_IMG_BUF_Y / 2) {
				value32 = RGB_BLUE;
			}
			if (idx_x >= LCDC_TEST_IMG_BUF_X / 2 && idx_y >= LCDC_TEST_IMG_BUF_Y / 2) {
				value32 = RGB_YELLOW;
			}
			
			pImgBuf[idx] = *(argb8888_t *)&value32;
		}
	}

	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}

void Lcdc_display_frame(u32 *LcdImgBuffer)
{
	argb8888_t *pImgBuf = (argb8888_t *)LcdImgBuffer;//LCDC_LAYER_IMG_FORMAT_ARGB8888
	u32 h_width = 100;
	u32	v_width = 100;

	u32 idx_x, idx_y, idx, value32;
	for (idx_x = 0; idx_x < LCDC_TEST_IMG_BUF_X; idx_x++) {
		for (idx_y = 0; idx_y < LCDC_TEST_IMG_BUF_Y; idx_y++) {
			idx = idx_y * LCDC_TEST_IMG_BUF_X + idx_x;

			if (idx_x >= h_width  && (idx_x <= LCDC_TEST_IMG_BUF_X - h_width) &&
				idx_y >= v_width && (idx_y <= LCDC_TEST_IMG_BUF_Y - v_width)) {
				value32 = RGB_GREEN;
			} else {
				value32 = RGB_RED;
			}
			pImgBuf[idx] = *(argb8888_t *)&value32;
		}
	}

	DCache_CleanInvalidate((u32)LcdImgBuffer, LCDC_IMG_BUF_SIZE);
}

void MipiDsi_UnderFlowReset(void)
{
	MIPI_TypeDef *MIPIx = MIPI;
	LCDC_TypeDef *pLCDC = LCDC;
	u32 reg_val2 = MIPI_DSI_INTS_ACPU_Get(MIPIx);

	if (reg_val2) {
		UnderFlowCnt = 0;
		MIPI_DSI_INT_Config(MIPI, DISABLE, DISABLE, DISABLE);
		MIPI_DSI_INTS_ACPU_Clr(MIPIx, reg_val2);

		/*Disable the LCDC*/
		LCDC_Cmd(pLCDC, DISABLE);
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "#LCDC_CTRL 0x%x\n", pLCDC->LCDC_CTRL);

		LCDC_Cmd(pLCDC, ENABLE);
		while (!LCDC_CheckLCDCReady(pLCDC));

		MIPI_DSI_Mode_Switch(MIPI, ENABLE);
	}
}

/**
  * @brief       LCDC irq handler for RGB I/F HV interrupt test
  * @param     Data:  a void type pointer, here is a LCDC_TypeDef pointer.
  *
  * @retval None
  */
u32 LCDCRgbHvIsr(void *Data)
{
	LCDC_TypeDef *LCDCx = (LCDC_TypeDef *)Data;
	/*get LCDC interrupt status*/
	volatile u32 IntId = LCDC_GetINTStatus(LCDCx);

	/*select operations according to interrupt ID*/
	if (IntId & LCDC_BIT_LCD_LIN_INTS) {
		LCDC_ClearINT(LCDCx, LCDC_BIT_LCD_LIN_INTS);
	}

	if (IntId & LCDC_BIT_DMA_UN_INTS) {
		LCDC_ClearINT(LCDCx, LCDC_BIT_DMA_UN_INTS);
		UnderFlowCnt++;
		if (UnderFlowCnt == 1) {
			DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "LCDC DMA Under-----\n");
			InterruptRegister((IRQ_FUN)MipiDsi_UnderFlowReset, MipiIrqInfo.IrqNum, (u32)MipiIrqInfo.IrqData, MipiIrqInfo.IrqPriority);
			InterruptEn(MipiIrqInfo.IrqNum, MipiIrqInfo.IrqPriority);
			MIPI_DSI_INTS_ACPU_Clr(MIPI, MIPI_DSI_INTS_ACPU_Get(MIPI));
			MIPI_DSI_Mode_Switch(MIPI, DISABLE);
			MIPI_DSI_INT_Config(MIPI, ENABLE, ENABLE, ENABLE);
		}
	}
	return 0;
}

void LcdcRgbDisplayTest(void)
{

	LcdcInitConfig();

	rgb888_t bg_color;
	*(u32 *)&bg_color = RGB_CYAN;
	LCDC_InitStruct.LCDC_BgColorRed = bg_color.red;
	LCDC_InitStruct.LCDC_BgColorGreen = bg_color.green;
	LCDC_InitStruct.LCDC_BgColorBlue = bg_color.blue;

	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER1].LCDC_LayerEn = ENABLE;
	Lcdc_display_pure((u32 *)LcdImgBuffer1);
	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER2].LCDC_LayerEn = DISABLE;
	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerEn = DISABLE;

	LCDC_Init(LCDC, &LCDC_InitStruct);
	LCDC_DMAModeConfig(LCDC, LCDC_LAYER_BURSTSIZE_4X64BYTES);
	LCDC_DMADebugConfig(LCDC, LCDC_DMA_OUT_DISABLE, 0);

	/*register LCDC ISR*/
	InterruptDis(LcdcIrqInfo.IrqNum);
	InterruptUnRegister(LcdcIrqInfo.IrqNum);
	InterruptRegister((IRQ_FUN)LCDCRgbHvIsr, LcdcIrqInfo.IrqNum, (u32)LcdcIrqInfo.IrqData, LcdcIrqInfo.IrqPriority);
	InterruptEn(LcdcIrqInfo.IrqNum, LcdcIrqInfo.IrqPriority);

	/*line number*/
	LCDC_LineINTPosConfig(LCDC, MIPI_VACT_g * 4 / 5);
	/*enbale LCDC interrupt*/
	LCDC_INTConfig(LCDC, LCDC_BIT_LCD_LIN_INTEN | LCDC_BIT_DMA_UN_INTEN, ENABLE);

	LcdcEnable();

	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER2].LCDC_LayerEn = ENABLE;
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER2].LCDC_LayerImgBaseAddr = (u32)LcdImgBuffer2;
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER2].LCDC_LayerBlendConfig = 1;
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER2].LCDC_LayerConstAlpha = 0x7F;
	//
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerEn = ENABLE;
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerImgBaseAddr = (u32)LcdImgBuffer3;
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerHorizontalStart = 1;/*1-based*/
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerHorizontalStop = LCDC_TEST_IMG_BUF_X;
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerVerticalStart = 1;/*1-based*/
	//	LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3].LCDC_LayerVerticalStop = LCDC_TEST_IMG_BUF_Y;
	LCDC_LayerConfig(LCDC, LCDC_LAYER_LAYER2, &LCDC_InitStruct.layerx[LCDC_LAYER_LAYER2]);
	LCDC_LayerConfig(LCDC, LCDC_LAYER_LAYER3, &LCDC_InitStruct.layerx[LCDC_LAYER_LAYER3]);
	LCDC_TrigerSHWReload(LCDC);

}

void LcdcRgbDisplayChk(void)
{

	u32 layer_idx = LCDC_LAYER_LAYER1;
	u32 *LcdShowBuffer;

	/*dump the current X/Y position*/
	while (1) {
		if (LCDC_InitStruct.layerx[layer_idx].LCDC_LayerImgBaseAddr == (u32)LcdImgBuffer1) {
			LcdShowBuffer = (u32 *)BackupLcdImgBuffer;
		} else {
			LcdShowBuffer = (u32 *)LcdImgBuffer1;
		}
		LCDC_InitStruct.layerx[layer_idx].LCDC_LayerImgBaseAddr = (u32)LcdShowBuffer;
		LCDC_LayerConfig(LCDC, layer_idx, &LCDC_InitStruct.layerx[layer_idx]);

		switch (LCDC_Show_Idx++) {
		case 0:
			Lcdc_display_column(LcdShowBuffer);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 3 * FONT_WIDTH, '1' , RGB_BLACK, 1);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 2 * FONT_WIDTH, '3' , RGB_BLACK, 0);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 1 * FONT_WIDTH, '7' , RGB_BLACK, 0);			
			break;
		case 1:
			Lcdc_display_row(LcdShowBuffer);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 3 * FONT_WIDTH, '1' , RGB_BLACK, 1);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 2 * FONT_WIDTH, '3' , RGB_BLACK, 0);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 1 * FONT_WIDTH, '7' , RGB_BLACK, 0);			
			break;
			
			break;
		case 2:
			Lcdc_display_grid(LcdShowBuffer);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 3 * FONT_WIDTH, '1' , RGB_BLACK, 1);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 2 * FONT_WIDTH, '3' , RGB_BLACK, 0);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 1 * FONT_WIDTH, '7' , RGB_BLACK, 0);			
			break;
			
			break;
		case 3:
			Lcdc_display_frame(LcdShowBuffer);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 3 * FONT_WIDTH, '1' , RGB_BLACK, 1);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 2 * FONT_WIDTH, '3' , RGB_BLACK, 0);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 1 * FONT_WIDTH, '7' , RGB_BLACK, 0);			
			break;	
			
			break;
		default:
			LCDC_Show_Idx = 0;
			Lcdc_display_pure(LcdShowBuffer);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 3 * FONT_WIDTH, '1' , 0, 1);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 2 * FONT_WIDTH, '3' , 0, 0);
			Lcdc_display_fps(LcdShowBuffer, LCDC_TEST_IMG_BUF_X - FONT_HEIGHT, LCDC_TEST_IMG_BUF_Y - 1 * FONT_WIDTH, '7' , 0, 0);			
			break;
			break;
		}

		LCDC_TrigerSHWReload(LCDC);
		//vTaskDelay(1000);
		rtw_mdelay_os(10000);
	}
DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "[%s]ln%d\n", __FUNCTION__, __LINE__);
}

void MIPIDemoShow_task(void)
{
	if (ChipInfo_MemoryType() == Memory_Type_PSRAM) {
		DBG_PRINTF(MODULE_BOOT, LEVEL_ERROR, "Remember Modify LCDC_IMG_BUF_OFFSET in PSRAM(8MB)\n");
		BackupLcdImgBuffer = (u8 *)(DDR_BASE + (5 << 20));	/*PSRAM use 5~8M*/
		LcdImgBuffer1 = (u8 *)(BackupLcdImgBuffer + LCDC_IMG_BUF_SIZE);
	}

	RCC_PeriphClockCmd(APBPeriph_NULL, APBPeriph_HPERI_CLOCK, ENABLE);
	RCC_PeriphClockCmd(APBPeriph_LCDC, APBPeriph_LCDCMIPI_CLOCK, ENABLE);

	MipiDsi_ST7701S_lcm_init();
	LcdcRgbDisplayTest();

	LcdcRgbDisplayChk();
}

void MIPIDemoShow(void)
{
	//MIPI(<=66.7M), LCDC(<=NPPLL/4), LCDC : MIPI >= 3 : 1
	u32 totaly = MIPI_DSI_VSA + MIPI_DSI_VBP + MIPI_DSI_VFP + LCDC_TEST_IMG_BUF_Y;
	u32 totalx = MIPI_DSI_HSA + MIPI_DSI_HBP + MIPI_DSI_HFP + LCDC_TEST_IMG_BUF_X;
	vo_freq = totaly * totalx * MIPI_FRAME_RATE / Mhz + 4;
	DBG_PRINTF(MODULE_BOOT, LEVEL_INFO, "vo_freq: %d\n", vo_freq);
	assert_param(vo_freq < 67);

	u32 PLLDiv = PLL_GET_NPLL_DIVN_SDM(PLL_BASE->PLL_NPPLL_CTRL1) + 2;
	u32 PLL_CLK = XTAL_ClkGet() * PLLDiv;
	u32 mipi_ckd = PLL_CLK / vo_freq - 1;
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0) & ~LSYS_MASK_CKD_MIPI) | LSYS_CKD_MIPI(mipi_ckd));
	HAL_WRITE32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0, (HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_CKD_GRP0) & ~LSYS_MASK_CKD_HPERI) | LSYS_CKD_HPERI(3));

	/*xTaskCreate((TaskFunction_t)MIPIDemoShow_task,
				(const char *)"MIPIDemoShow_task",
				(uint16_t)2048,
				(void *)NULL,
				(UBaseType_t)tskIDLE_PRIORITY + 1,
				(TaskHandle_t *)NULL);*/
				
	//pthread_t pid;
	//int ret = pthread_create(&pid, NULL, (pthread_startroutine_t)MIPIDemoShow_task, NULL);
	struct task_struct pid;
	rtw_create_task(&pid,
                (const char *)"MIPIDemoShow_task",
                (uint16_t)1024,
                1024 + 1,
                (thread_func_t)MIPIDemoShow_task,
                NULL);
	
	
	//if (ret < 0) {
	//	return -1;
	//}
}

