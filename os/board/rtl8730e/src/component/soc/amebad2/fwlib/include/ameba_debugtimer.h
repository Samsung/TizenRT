#ifndef _AMEBAD2_DEBUGTIMER_H_
#define _AMEBAD2_DEBUGTIMER_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup DEBUGTIMER
  * @brief DEBUGTIMER driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup DEBUGTIMER_Register_Definitions DEBUGTIMER Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup DBGT_CTRL
 * @brief debugtimer control register
 * @{
 **/
#define DBGT_MASK_DIV            ((u32)0x00000FFF << 4)          /*!<R/W 28h  The default value can used to divide 40M Xtal to 1M */
#define DBGT_DIV(x)              ((u32)(((x) & 0x00000FFF) << 4))
#define DBGT_GET_DIV(x)          ((u32)(((x >> 4) & 0x00000FFF)))
#define DBGT_MASK_CLKSEL         ((u32)0x00000007 << 1)          /*!<R/W 0h  Clock source select 2~7: Reserved 1: Select 32K as clock source. 0: Select Xtal as clock source. */
#define DBGT_CLKSEL(x)           ((u32)(((x) & 0x00000007) << 1))
#define DBGT_GET_CLKSEL(x)       ((u32)(((x >> 1) & 0x00000007)))
#define DBGT_BIT_ENABLE          ((u32)0x00000001 << 0)          /*!<R/W 1h  Debug timer enable. 1: The counter is enabled. 0: The counter is disabled. */
/** @} */

/** @defgroup DBGT_CRV
 * @brief debugtimer count register
 * @{
 **/
#define DBGT_MASK_CRV            ((u32)0xFFFFFFFF << 0)          /*!<WOP 0h  Continuing to count from 0 once it's reached 32'hFFFFFFFF */
#define DBGT_CRV(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DBGT_GET_CRV(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DBGT_ATOM_INC
 * @brief debugtimer atom read increase
 * @{
 **/
#define DBGT_MASK_ATOM_INC       ((u32)0xFFFFFFFF << 0)          /*!<RP 0h  Each read will increase 1 and return to the increased value to bus. */
#define DBGT_ATOM_INC(x)         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DBGT_GET_ATOM_INC(x)     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DBGT_ATOM
 * @brief debugtimer atom
 * @{
 **/
#define DBGT_MASK_ATOM           ((u32)0xFFFFFFFF << 0)          /*!<WOP 0h  Return the last increased value of atom counter */
#define DBGT_ATOM(x)             ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DBGT_GET_ATOM(x)         ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DBGT_SCRATCH
 * @brief debugtimer scratch
 * @{
 **/
#define DBGT_MASK_SCRATCH        ((u32)0xFFFFFFFF << 0)          /*!<R/W 0h  Reserved for user use */
#define DBGT_SCRATCH(x)          ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DBGT_GET_SCRATCH(x)      ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup DEBUGTIMER_Exported_Types DEBUGTIMER Exported Types
  * @{
*****************************************************************************/


/**
 * @brief DEBUGTIMER Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t DBGT_CTRL;                              /*!< DEBUGTIMER CONTROL REGISTER,  Address offset: 0x000 */
	__O  uint32_t DBGT_CRV;                               /*!< DEBUGTIMER COUNT REGISTER,  Address offset: 0x004 */
	__IO uint32_t DBGT_ATOM_INC;                          /*!< DEBUGTIMER ATOM READ INCREASE Register,  Address offset: 0x008 */
	__O  uint32_t DBGT_ATOM;                              /*!< DEBUGTIMER ATOM Register,  Address offset: 0x00C */
	__IO uint32_t DBGT_SCRATCH;                           /*!< DEBUGTIMER SCRATCH Register,  Address offset: 0x010 */
} DEBUGTIMER_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported constants --------------------------------------------------------*/
/** @defgroup DEBUGTIMER_Exported_Constants DEBUGTIMER Exported Constants
  * @{
  */

/** @defgroup DEBUGTIMER_Clock_Source
  * @{
  */
#define  DTIM_CLK_XTAL  0
#define  DTIM_CLK_32K    1
/**
  * @}
  */

/**
  * @}
  */


/* group: DEBUGTIMER_Exported_Functions */
/* Since all the members in the group are __STATIC_INLINE which will not be extracted to the doxygen doc,
  original comment has been deleted to avoid there is nothing displayed under the group. */

/**
  * @brief  Enables or disables the DebugTimer peripheral
  *   The debug timer is a free run timer works at 1MHZ.
  * @param  NewState: new state of the Debug timer peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup DEBUGTIMER_Exported_Functions DEBUGTIMER Exported Functions
  * @{
  */

__STATIC_INLINE void DTimer_Cmd(BOOL Newstatus)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	if (Newstatus == ENABLE) {
		DTimer->DBGT_CTRL |= DBGT_BIT_ENABLE;
	} else {
		DTimer->DBGT_CTRL &= ~DBGT_BIT_ENABLE;
	}

}

/**
  * @brief  Select clock for timestamp, which works at 1MHz by default.
  * @param  DTim_speed: Speed of debugtimer counter.
  *   @arg 0: OSC32KHz.
  *   @arg 1: 1MHz, which is divided form XTAL40MHz.
  * @retval None
  */
__STATIC_INLINE void DTimer_CLK(u8 DTim_speed)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;
	u8 xtal = 40;
	u32 Temp;

	//assert_param(DTim_speed <= 40);

	if (DTim_speed == 0) {
		/*32K clock,the counter increase 1every 32us*/
		Temp = DTimer->DBGT_CTRL;
		Temp &= ~DBGT_MASK_CLKSEL;
		Temp |= DBGT_CLKSEL(DTIM_CLK_32K);

		DTimer->DBGT_CTRL = Temp;
	} else if (DTim_speed == 1)  {

		//if ((xtal % DTim_speed) != 0) {
		//	DBG_8195A("inaccurate speed selected\n");
		//}

		Temp = DTimer->DBGT_CTRL;
		Temp &= ~DBGT_MASK_CLKSEL;
		Temp |= DBGT_CLKSEL(DTIM_CLK_XTAL);
		Temp |= DBGT_DIV(xtal / DTim_speed);

		DTimer->DBGT_CTRL = Temp;
	}

}


/**
  * @brief  Get the Debug timer current Counter.
  * @param  None
  * @retval current counter
  */
__STATIC_INLINE u32 DTimestamp_Get(void)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	return DTimer->DBGT_CRV;
}

/**
  * @brief  Set the Debug timer current Counter.
  * @param  NewVaule: new debug counter.
  * @retval current counter
  */
__STATIC_INLINE void DTimestamp_Set(u32 NewVaule)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	DTimer->DBGT_CRV = NewVaule;
}

/**
  * @brief  Get the Debug timer atom value.
  *   The read action won't trigger Atom increase
  * @param  None
  * @retval atom counter
  */
__STATIC_INLINE u32 DTimer_AtomGet(void)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	return DTimer->DBGT_ATOM;
}

/**
  * @brief  Get the Debug timer atom value + 1.
  *   The read action will trigger Atom increase
  * @param  None
  * @retval atom counter
  */
__STATIC_INLINE u32 DTimer_AtomIncGet(void)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	return DTimer->DBGT_ATOM_INC;
}

/**
  * @brief  Set the Debug timer atom value .
  * @param  New value
  * @retval None
  */
__STATIC_INLINE void DTimer_AtomSet(u32 NewVaule)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	DTimer->DBGT_ATOM = NewVaule;
}

/**
  * @brief  Get the Debug timer Scratch value.
  * @param  None
  * @retval atom scratch
  */
__STATIC_INLINE u32 DTimer_ScratchGet(void)
{
	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	return DTimer->DBGT_SCRATCH;
}

/**
  * @brief  Set the Debug timer Scratch value .
  * @param  New value
  * @retval None
  */
__STATIC_INLINE void DTimer_ScratchSet(u32 NewVaule)
{

	DEBUGTIMER_TypeDef *DTimer = DTIMER_DEV;

	DTimer->DBGT_SCRATCH = NewVaule;
}
/* end of the function group */
/** @} */
/** @} */

/** @} */

/* MANUAL_GEN_END */

#endif
