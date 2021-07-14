/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_scatternet_app_task.h
   * @brief     Routines to create App task and handle events & messages
   * @author    jane
   * @date      2017-06-02
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _BLE_TIZENRT_SCATTERNET_APP_TASK_H_
#define _BLE_TIZENRT_SCATTERNET_APP_TASK_H_

/**
 * @brief  Initialize App task
 * @return void
 */
void ble_tizenrt_scatternet_app_task_init(void);
void ble_tizenrt_scatternet_app_main_task(void *p_param);
void ble_tizenrt_scatternet_app_task_deinit(void);

#endif
