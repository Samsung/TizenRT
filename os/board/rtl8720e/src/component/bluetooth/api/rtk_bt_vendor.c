/*
 *******************************************************************************
 * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */
#include <stdio.h>
#include <string.h>
#include <osif.h>

/* ---------------------------------- define -------------------------------*/

/* ------------------------------ Global Variables -------------------------*/
uint8_t bt_manual_tx_power_gain_enable = 0;
uint8_t bt_manual_gain_index = 0xFE;

/* -------------------------------- Functions ------------------------------*/

/**
 * @brief     set bt tx power gain index.
 * @param[in] index: tx power gain index.
 * @return    None
 */
void rtk_bt_set_bt_tx_power_gain_index(uint8_t index)
{
	bt_manual_tx_power_gain_enable = 1;
	bt_manual_gain_index = index;
}
