#ifndef _AMEBAD2_TRUSTZONE_H_
#define _AMEBAD2_TRUSTZONE_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup PPC_REG_0
 * @brief
 * @{
 *****************************************************************************/
#define BIT_SECFG_SPORT2_I2S    ((u32)0x00000001 << 31)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SPORT1_I2S    ((u32)0x00000001 << 30)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SPORT0_I2S    ((u32)0x00000001 << 29)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_VAD           ((u32)0x00000001 << 28)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_AUDIO_CDEDC   ((u32)0x00000001 << 27)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_TIMER         ((u32)0x00000001 << 26)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_TRNG          ((u32)0x00000001 << 25)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_LEDC          ((u32)0x00000001 << 24)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_UART3_BT      ((u32)0x00000001 << 23)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_UART2         ((u32)0x00000001 << 22)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_UART1         ((u32)0x00000001 << 21)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_UART0         ((u32)0x00000001 << 20)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_DDRC          ((u32)0x00000001 << 19)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_PSRAM_CTRL    ((u32)0x00000001 << 18)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_RXI300_HS     ((u32)0x00000001 << 17)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_HS_SYSON      ((u32)0x00000001 << 16)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_I2C2          ((u32)0x00000001 << 15)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_I2C1          ((u32)0x00000001 << 14)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_IR            ((u32)0x00000001 << 13)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_EDCSA         ((u32)0x00000001 << 12)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_ED25519       ((u32)0x00000001 << 11)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_RSA           ((u32)0x00000001 << 10)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_MIPI_DSI      ((u32)0x00000001 << 9)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SPI1          ((u32)0x00000001 << 8)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SPI0          ((u32)0x00000001 << 7)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_GDMA0_CFG     ((u32)0x00000001 << 6)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_LCDC_CFG      ((u32)0x00000001 << 5)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SDEMMC        ((u32)0x00000001 << 4)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SHA           ((u32)0x00000001 << 3)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_AES           ((u32)0x00000001 << 2)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_USBOTG_CFG    ((u32)0x00000001 << 1)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_WIFI_CFG      ((u32)0x00000001 << 0)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
/** @} */

/**************************************************************************//**
 * @defgroup PPC_REG_1
 * @brief
 * @{
 *****************************************************************************/
#define BIT_SECFG_ZIGBEE        ((u32)0x00000001 << 4)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_DDR_BSTC      ((u32)0x00000001 << 3)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_DDR_PHY       ((u32)0x00000001 << 2)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_BT_CFG        ((u32)0x00000001 << 1)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
#define BIT_SECFG_SPORT3_I2S    ((u32)0x00000001 << 0)          /*!<R/W 0  1: non-secure attribution 0: secure attribution */
/** @} */

/* AUTO_GEN_END */

/* MANUAL_GEN_START */

typedef struct {
	__IO u32 IDAU_BARx;		/*!< ,	Address offset: 0x00 */
	__IO u32 IDAU_LARx;		/*!< ,	Address offset: 0x04 */
} MPC_EntryTypeDef;

typedef struct {
	MPC_EntryTypeDef MPC_Entry[8]; /*!< ,	Address offset: 0x00 ~ 0x3C*/
	__IO u32 IDAU_CTRL;		/*!< ,	Address offset: 0x40 */
	__IO u32 IDAU_LOCK;		/*!< ,	Address offset: 0x44 */
} MPC_TypeDef;

typedef struct {
	__IO u32 PPC_REG0;		/*!< ,	Address offset: 0x00 */
	__IO u32 PPC_REG1;		/*!< ,	Address offset: 0x04 */
} PPC_TypeDef;

typedef struct {
	u32 Start;
	u32 End;
	u32 NSC;
} TZ_CFG_TypeDef;

typedef enum {
	SECFG_WIFI_CFG = 0,
	SECFG_USBOTG_CFG,
	SECFG_AES,
	SECFG_SHA,
	SECFG_SDEMMC,
	SECFG_LCDC_CFG,
	SECFG_GDMA0_CFG,
	SECFG_SPI0,
	SECFG_SPI1,
	SECFG_MIPI_DSI,
	SECFG_RSA,
	SECFG_ED25519,
	SECFG_EDCSA,
	SECFG_IR,
	SECFG_I2C1,
	SECFG_I2C2,
	SECFG_HS_SYSON,
	SECFG_RXI300_HS,
	SECFG_PSRAM_CTRL,
	SECFG_DDRC,
	SECFG_UART0,
	SECFG_UART1,
	SECFG_UART2,
	SECFG_UART3_BT,
	SECFG_LEDC,
	SECFG_TRNG,
	SECFG_TIMER,
	SECFG_AUDIO_CDEDC,
	SECFG_VAD,
	SECFG_SPORT0_I2S,
	SECFG_SPORT1_I2S,
	SECFG_SPORT2_I2S,

	SECFG_SPORT3_I2S,
	SECFG_BT_CFG,
	SECFG_DDR_PHY,
	SECFG_DDR_BSTC,
	SECFG_ZIGBEE,

	SECFG_MAXNUM,

} PPC_PeripheralId;

#define IS_PPC_PERIID(x)			((x >= SECFG_WIFI_CFG) && (x < SECFG_MAXNUM))

#define IS_PPC_REG0(x)			(x <= SECFG_SPORT2_I2S)
#define IS_PPC_REG1(x)			(x == SECFG_ZIGBEE) || \
								(x == SECFG_DDR_BSTC) || \
								(x == SECFG_DDR_PHY) || \
								(x == SECFG_BT_CFG) || \
								(x == SECFG_SPORT3_I2S)

#define SECURE				0
#define NON_SECURE			1

/*
  *   <q> Enable SAU
  *   <i> Value for SAU->CTRL register bit ENABLE
  */
#define SAU_INIT_CTRL_ENABLE   1

/*
  *   <o> When SAU is disabled
  *     <0=> All Memory is Secure
  *     <1=> All Memory is Non-Secure
  *   <i> Value for SAU->CTRL register bit ALLNS
  *   <i> When all Memory is Non-Secure (ALLNS is 1), IDAU can override memory map configuration.
  */
#define SAU_INIT_CTRL_ALLNS  0

#define SAU_ENTRYS_NUM			8
#define MPC_ENTRYS_NUM			8

#define MPC_CNT					2

void BOOT_RAM_TZCfg(void);
void TZ_ConfigSlaveSecurity(PPC_PeripheralId Perip, u32 Status);


/* 	The TT instruction takes a memory address and returns the configuration of the Memory Protection Unit (MPU) at that address.
	The cmse_address_info_t is declared as following:
		typedef union {
			struct cmse_address_info {
				unsigned mpu_region:8;
				unsigned :8;
				unsigned mpu_region_valid:1;
				unsigned :1;
				unsigned read_ok:1;
				unsigned readwrite_ok:1;
				unsigned :12;
			} flags;
			unsigned value;
		} cmse_address_info_t;

	When executed in the secure state the result of TT instruction is extended to return the SAU and IDAU configurations at the specific address.
	The extended cmse_address_info_t is declared as following:
		typedef union {
			struct cmse_address_info {
				unsigned mpu_region:8;
				unsigned sau_region:8;
				unsigned mpu_region_valid:1;
				unsigned sau_region_valid:1;
				unsigned read_ok:1;
				unsigned readwrite_ok:1;
				unsigned nonsecure_read_ok:1;
				unsigned nonsecure_readwrite_ok:1;
				unsigned secure:1;
				unsigned idau_region_valid:1;
				unsigned idau_region:8;
			} flags;
			unsigned value;
		} cmse_address_info_t;

	As a result, these extended bits are only valid when executing in Secure state, and are UNDEFINED if used from Non-secure state.
*/
__STATIC_INLINE u32 TrustZone_IsSecure(void)
{
#if defined (ARM_CORE_CM4)
	cmse_address_info_t cmse_address_info = cmse_TT((void *)DiagPrintf);
	return cmse_address_info.flags.secure;
#else
	return 0;
#endif
}


/* MANUAL_GEN_END */

#endif

/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
