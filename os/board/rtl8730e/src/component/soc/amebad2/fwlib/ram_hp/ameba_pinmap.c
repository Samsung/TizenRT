/**
  ******************************************************************************
  * @file    rtl8721dlp_pinmap.c
  * @author
  * @version V1.0.0
  * @date    2016-05-17
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of pin control:
  *           - pinmux
  *           - active pad pull up & pull down
  *           - sleep pad pull up & pull down
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#include "ameba_soc.h"

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


/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
