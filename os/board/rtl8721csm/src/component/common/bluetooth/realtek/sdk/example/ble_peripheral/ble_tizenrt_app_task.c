/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      ble_tizenrt_app_task.c
   * @brief     Routines to create App task and handle events & messages
   * @author    jane
   * @date      2017-06-02
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <os_msg.h>
#include <os_task.h>
#include <gap.h>
#include <gap_le.h>
#include "ble_tizenrt_app_task.h"
#include <app_msg.h>
#include "ble_tizenrt_app.h"
#include <stdio.h>
#include <basic_types.h>
#include <gap_msg.h>
#include "ble_tizenrt_app.h"

/** @defgroup  PERIPH_APP_TASK Peripheral App Task
    * @brief This file handles the implementation of application task related functions.
    *
    * Create App task and handle events & messages
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define BLE_TIZENRT_APP_TASK_PRIORITY             5         /* Task priorities */
#define BLE_TIZENRT_APP_TASK_STACK_SIZE           256 * 4   /* Task stack size */
#define BLE_TIZENRT_CALLBACK_TASK_PRIORITY        BLE_TIZENRT_APP_TASK_PRIORITY         /* Task priorities */
#define BLE_TIZENRT_CALLBACK_TASK_STACK_SIZE      256 * 8   /* Task stack size */
#define BLE_TIZENRT_MAX_NUMBER_OF_GAP_MESSAGE     0x20      /* GAP message queue size */
#define BLE_TIZENRT_MAX_NUMBER_OF_IO_MESSAGE      0x20      /* IO message queue size */
#define BLE_TIZENRT_MAX_NUMBER_OF_EVENT_MESSAGE   (BLE_TIZENRT_MAX_NUMBER_OF_GAP_MESSAGE + BLE_TIZENRT_MAX_NUMBER_OF_IO_MESSAGE)    /* Event message queue size */
#define BLE_TIZENRT_MAX_NUMBER_OF_CALLBACK_MESSAGE      0x20      /* IO message queue size */

/*============================================================================*
 *                              Variables
 *============================================================================*/
void *ble_tizenrt_app_task_handle = NULL;   /* APP Task handle */
void *ble_tizenrt_evt_queue_handle = NULL;  /* Event queue handle */
void *ble_tizenrt_io_queue_handle = NULL;   /* IO queue handle */

void *ble_tizenrt_callback_task_handle = NULL;   /* Callback Task handle */
void *ble_tizenrt_callback_queue_handle = NULL;  /* Callback Queue handle */

static void *start_bt_stack_sem_handle = NULL;
extern T_GAP_DEV_STATE ble_tizenrt_gap_dev_state;

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief        App task to handle events & messages
 * @param[in]    p_param    Parameters sending to the task
 * @return       void
 */
void ble_tizenrt_app_main_task(void *p_param)
{
    (void)p_param;
    uint8_t event;

    os_sem_give(start_bt_stack_sem_handle);

    while (true)
    {
        if (os_msg_recv(ble_tizenrt_evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(ble_tizenrt_io_queue_handle, &io_msg, 0) == true)
                {
                    ble_tizenrt_app_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
    debug_print("Init Success \n");
}

void ble_tizenrt_callback_main_task(void *p_param)
{
    (void)p_param;
    T_TIZENRT_APP_CALLBACK_MSG callback_msg;

    while (true)
    {
        if (os_msg_recv(ble_tizenrt_callback_queue_handle, &callback_msg, 0xFFFFFFFF) == true)
        {
            debug_print("Recieve msg type %d \n", callback_msg.type);
            ble_tizenrt_handle_callback_msg(callback_msg);
        }
    }
}
/**
 * @brief  Initialize App task
 * @return void
 */
void ble_tizenrt_app_task_init(void)
{
    os_msg_queue_create(&ble_tizenrt_io_queue_handle, BLE_TIZENRT_MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&ble_tizenrt_evt_queue_handle, BLE_TIZENRT_MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    os_msg_queue_create(&ble_tizenrt_callback_queue_handle, BLE_TIZENRT_MAX_NUMBER_OF_CALLBACK_MESSAGE, sizeof(T_TIZENRT_APP_CALLBACK_MSG));

    os_sem_create(&start_bt_stack_sem_handle, 0, 1);

    os_task_create(&ble_tizenrt_app_task_handle, "app", ble_tizenrt_app_main_task, 0, BLE_TIZENRT_APP_TASK_STACK_SIZE,
                   BLE_TIZENRT_APP_TASK_PRIORITY);
    os_task_create(&ble_tizenrt_callback_task_handle, "tizenrt_callback", ble_tizenrt_callback_main_task, 0, BLE_TIZENRT_CALLBACK_TASK_STACK_SIZE,
                    BLE_TIZENRT_CALLBACK_TASK_PRIORITY);

    if (os_sem_take(start_bt_stack_sem_handle, 0xFFFFFFFF) == true) {
        if(false == gap_start_bt_stack(ble_tizenrt_evt_queue_handle, ble_tizenrt_io_queue_handle, BLE_TIZENRT_MAX_NUMBER_OF_GAP_MESSAGE))
            debug_print("gap_start_bt_stack return false \n");
        os_sem_delete(start_bt_stack_sem_handle);
        start_bt_stack_sem_handle = NULL;
    } else
        debug_print("take start_bt_stack_sem_handle fail \n");
    debug_print("Init Done \n");
}

extern T_ATTRIB_APPL *tizenrt_ble_service_tbl;
#if defined(CONFIG_BLE_INDICATION)
extern void *ble_tizenrt_indicate_sem;
#endif
void ble_tizenrt_app_task_deinit(void)
{
    if (ble_tizenrt_io_queue_handle) {
        os_msg_queue_delete(ble_tizenrt_io_queue_handle);
    }
    if (ble_tizenrt_evt_queue_handle) {
        os_msg_queue_delete(ble_tizenrt_evt_queue_handle);
    }
    if (ble_tizenrt_app_task_handle) {
        os_task_delete(ble_tizenrt_app_task_handle);
    }

    if (ble_tizenrt_callback_queue_handle) {
        os_msg_queue_delete(ble_tizenrt_callback_queue_handle);
    }
    if (ble_tizenrt_callback_task_handle) {
        os_task_delete(ble_tizenrt_callback_task_handle);
    }

#if defined(CONFIG_BLE_INDICATION)
    if (ble_tizenrt_indicate_sem) {
        os_mutex_delete(ble_tizenrt_indicate_sem);
        ble_tizenrt_indicate_sem = NULL;
    }
#endif

    ble_tizenrt_io_queue_handle = NULL;
    ble_tizenrt_evt_queue_handle = NULL;
    ble_tizenrt_app_task_handle = NULL;

    ble_tizenrt_callback_task_handle = NULL;
    ble_tizenrt_callback_queue_handle = NULL;

    os_mem_free(tizenrt_ble_service_tbl);

    ble_tizenrt_gap_dev_state.gap_init_state = 0;
    ble_tizenrt_gap_dev_state.gap_adv_sub_state = 0;
    ble_tizenrt_gap_dev_state.gap_adv_state = 0;
    ble_tizenrt_gap_dev_state.gap_scan_state = 0;
    ble_tizenrt_gap_dev_state.gap_conn_state = 0;
}

/** @} */ /* End of group PERIPH_APP_TASK */
