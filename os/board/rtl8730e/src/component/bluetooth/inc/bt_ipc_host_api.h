/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef _BT_IPC_HOST_API_H_
#define _BT_IPC_HOST_API_H_

/* -------------------------------- Includes -------------------------------- */
#include <stdint.h>
#include "ameba.h"
#include "bt_ipc_profile_config.h"
#include "platform_opts_bt.h"
#include "rtk_bt_common.h"

#define IPC_HOST_API_DATA_MAX   972   // 243*4
#define IPC_HOST_API_RET_MAX    104    //26*4 = the ret size of bt_ipc_host_request_message

/* ------------------------------- Data Types ------------------------------- */

/** @brief bt ipc host api task private struct */
struct bt_ipc_host_api_task_struct {
	void		*task;	/* I: workqueue thread */
	void		*wakeup_sema;    /* for internal use only */
	void		*terminate_sema; /* for internal use only */
	uint32_t	blocked;          /* for internal use only */
};

typedef struct bt_ipc_host_request_message {
	uint32_t	PROFILE_ID;
	uint32_t	API_ID;
	uint32_t	buf_size;
	uint8_t		param_buf[IPC_HOST_API_DATA_MAX];
	int32_t		ret[IPC_HOST_API_RET_MAX>>2];
} bt_ipc_host_request_message;

/**
 * @brief  to initialize the ipc host for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_init_host(void);

/**
 * @brief  to deinit the ipc host for bt api.
 * @param  none.
 * @return none.
 */
void bt_ipc_api_deinit_host(void);

/**
 * @brief  to send a ipc message to device and wait result.
 * @param  profile_index[in]: mesh/peripheral/central ...
 * @param  api_id[in]: BT_API_ID.
 * @param  param_buf[in]: pointer to API parameter.
 * @return pointer to result of API.
 */
int *bt_ipc_api_host_message_send(uint32_t profile_id, uint32_t api_id, uint8_t *param_buf, uint32_t buf_len);

#endif /* _BT_IPC_HOST_API_H_ */

