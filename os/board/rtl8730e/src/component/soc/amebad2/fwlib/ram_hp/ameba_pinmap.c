/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ameba_soc.h"

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup PINMAP
* @brief PINMAP driver modules
* @{
*/

/** @defgroup PINMAP_Exported_Functions PINMAP Exported Functions
  * @{
  */

/**
  *  @brief Config all pins to the right function and pull state based on pmap_func table.
  *  @note should set pmap_func. Function & FuncPuPd corectly based on your board.
  *  @retval None
  */
void pinmap_init(void)
{
	int i = 0;

	/* share audio pin */
	//HAL_WRITE32(PINMUX_REG_BASE, REG_PAD_AUD_PAD_CTRL, 0x1FFFFF);

	for (;;) {
		/*  Check if search to end */
		if (pmap_func[i].PinName == _PNC) {
			break;
		}

		/* HW PMC enable all the PAD by default, shutdown if do not need.*/
		if (pmap_func[i].FuncPuPd == GPIO_PuPd_SHUTDOWN) {
			PAD_CMD(pmap_func[i].PinName, DISABLE);
		}

		/* keep active level when GPIO_PuPd_KEEP */
		if (pmap_func[i].FuncPuPd != GPIO_PuPd_KEEP) {
			PAD_PullCtrl(pmap_func[i].PinName, pmap_func[i].FuncPuPd);
		}

		/* keep sleep setting level when GPIO_PuPd_KEEP */
		if (pmap_func[i].SleepPuPd != GPIO_PuPd_KEEP) {
			PAD_SleepPullCtrl(pmap_func[i].PinName, pmap_func[i].SleepPuPd);
		}

		i++;
	}
}
/**@}*/
/**@}*/
/**@}*/
