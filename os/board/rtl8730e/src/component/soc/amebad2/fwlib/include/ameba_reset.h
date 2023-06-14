#ifndef _AMEBAD_CHIPEN_H_
#define _AMEBAD_CHIPEN_H_

/* AUTO_GEN_START */


/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here
/**************************************************************************//**
 * @defgroup CHIPEN_ACK_TIMEOUT
 * @brief: If the interrupt did't clear within ack time,the system will reboot
 * @{
 *****************************************************************************/
#define CHIPEN_ACK_50MS 0x00UL
#define CHIPEN_ACK_100MS 0x01UL
#define CHIPEN_ACK_200MS 0x02UL
#define CHIPEN_ACK_400MS 0x03UL
/** @} */

/**************************************************************************//**
 * @defgroup CHIPEN_Long_Press_Interrupt_trigger_threshold
 * @brief: If CHIPEN Pressed longer than threshold, LP interrupt will be triggered
 * @{
 *****************************************************************************/
#define CHIPEN_LP_1S 0x00UL
#define CHIPEN_LP_1P5S 0x01UL
#define CHIPEN_LP_2S 0x02UL
#define CHIPEN_LP_2P5S 0x03UL
#define CHIPEN_LP_3S 0x04UL
#define CHIPEN_LP_3P5S 0x05UL
#define CHIPEN_LP_4S 0x06UL
#define CHIPEN_LP_4P5S 0x07UL
/** @} */

/**************************************************************************//**
 * @defgroup CHIPEN_Short_Press_Interrupt_trigger_threshold
 * @brief: If CHIPEN Pressed longer than threshold, SP interrupt will be triggered
 * @{
 *****************************************************************************/
#define CHIPEN_SP_0MS 0x00UL
#define CHIPEN_SP_50MS 0x01UL
#define CHIPEN_SP_100MS 0x02UL
#define CHIPEN_SP_150MS 0x03UL
#define CHIPEN_SP_200MS 0x04UL
#define CHIPEN_SP_250MS 0x05UL
#define CHIPEN_SP_300MS 0x06UL
#define CHIPEN_SP_350MS 0x07UL
/** @} */

/**************************************************************************//**
 * @defgroup CHIPEN_Debounce Time
 * @brief: CHIPEN HW debounce time
 * @{
 *****************************************************************************/
#define CHIPEN_DBC_0US 0x00UL
#define CHIPEN_DBC_100US 0x01UL
#define CHIPEN_DBC_500US 0x02UL
#define CHIPEN_DBC_1MS 0x03UL
#define CHIPEN_DBC_2MS 0x04UL
#define CHIPEN_DBC_4MS 0x05UL
#define CHIPEN_DBC_8MS 0x06UL
#define CHIPEN_DBC_16MS 0x07UL
/** @} */

enum CHIPEN_WORK_MODE {
	CHIPEN_HW_RESET_MODE = 0,
	CHIPEN_INT_RESET_MODE = 1,
	CHIPEN_PULSE_RESET_MODE = 2,
};


enum CPU_ID {
	LP_CPU_ID = 0,
	NP_CPU_ID = 1,
	AP_CPU_ID = 2,
};


#define SYS_RESET_KEY 0x96969696
#define SYS_RESET_TRIG 0x69696969


extern void CHIPEN_WorkMode(enum CHIPEN_WORK_MODE mode);
extern void CHIPEN_DebounceSet(u32 Debounce);
extern BOOL CHIPEN_IsPress(void);
extern void CHIPEN_ThresHoldSet(u32 Thres_LP, u32 Thres_SP);
extern void CHIPEN_AckTimeSet(u32 Tack);
extern void CHIPEN_ClearINT(u32 INTrBit);
extern u32 CHIPEN_GetINT(void);
extern u32 BOOT_Reason(void);
extern void System_Reset(void);


/**
  * @brief  Get Current CPU dynamiclly
  * @param  None
  * @retval CPU ID.
  *		 0: CPU_LP
  *		 1: CPU_NP
  *		 2: CPU_AP
  */
__STATIC_INLINE u32 SYS_CPUID(void)
{
	u32 temp = HAL_READ32(SYSTEM_CTRL_BASE_LP, REG_LSYS_SYSTEM_CFG0);

	return LSYS_GET_HW_CPU_ID(temp);
}

/* MANUAL_GEN_END */

#endif
