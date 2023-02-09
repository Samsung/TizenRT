/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef _BT_IPC_DEV_API_H_
#define _BT_IPC_DEV_API_H_

/* -------------------------------- Includes -------------------------------- */
#include <stdint.h>
#include "ameba.h"
#include "bt_ipc_profile_config.h"

#define IPC_DEV_API_DATA_MAX    (1024 + 120)   // for bt audio scenario
#define IPC_DEV_API_RET_MAX     40

/* ------------------------------- Data Types ------------------------------- */

/** @brief bt ipc dev api task private struct */
struct bt_ipc_dev_api_task_struct {
	void		*task;            /* I: workqueue thread */
	void		*wakeup_sema;     /* for internal use only */
	void		*terminate_sema;  /* for internal use only */
	uint32_t	blocked;          /* for internal use only */
	uint8_t		task_is_running;  /* for indicate task running */
};

typedef struct bt_ipc_dev_request_message {
	uint32_t	PROFILE_ID;
	uint32_t	EVENT_ID;
	uint32_t	buf_size;
	uint8_t		param_buf[IPC_DEV_API_DATA_MAX];
	int32_t		ret[15];
} bt_ipc_dev_request_message;

/**
 * @brief  to send a ipc message to device and wait result.
 * @param  profile_id[in]: mesh/peripheral/central ...
 * @param  event_id[in]: BT_EVENT_ID.
 * @param  param_buf[in]: pointer to API parameter.
 * @param  buf_len[in]: buffer size.
 * @return pointer to result of API.
 */
int *bt_ipc_api_dev_message_send(uint32_t profile_id, uint32_t event_id, uint8_t *param_buf, uint32_t buf_len);

/**
 * @brief  to initialize the ipc device for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_init_dev(void);

/**
 * @brief  to deinit the ipc device for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_deinit_dev(void);

#endif /* _BT_IPC_DEV_API_H_ */