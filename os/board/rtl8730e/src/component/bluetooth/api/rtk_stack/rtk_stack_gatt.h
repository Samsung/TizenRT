/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef __RTK_STACK_GATT_H__
#define __RTK_STACK_GATT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <dlist.h>
#include <rtk_bt_gattc.h>
#include <rtk_stack_config.h>

#define BT_QUEUE_PENDING_ELEMENT_MAX		40

#define REQ_FLAG_READ_BY_UUID	(1 << 0)
#define REQ_FLAG_ALREADY_SENT	(1 << 1)

typedef struct{
	struct list_head pending_list;		/* the pending_list to store pending gatt operation */
	uint16_t pending_ele_num;			/* number of elements in pending_list */
}rtk_bt_gatt_queue_t;

/***************************** GATT Server related ***************************/
typedef struct {
	struct list_head service_list;
#if !RTK_BLE_MGR_LIB
	uint8_t srv_registering;
#endif
	rtk_bt_gatt_queue_t notify_queue[RTK_BLE_GAP_MAX_LINKS];
	rtk_bt_gatt_queue_t indicate_queue[RTK_BLE_GAP_MAX_LINKS];
	uint32_t last_seq;
}rtk_bt_gatts_app_priv_t;

typedef enum {
    SRV_UNUSED = 0,
    SRV_ALLOC_FOR_INCLUDE,
    SRV_WAIT_FOR_REG,
    SRV_REGISTERING,
	SRV_REGISTERED,
} rtk_bt_gatts_srv_reg_status_t;

typedef struct{
	struct list_head list;
	uint16_t conn_handle;
	uint8_t app_id;
	uint32_t seq;
	uint16_t index;
	uint16_t len;
	uint8_t *data;
	uint16_t cid;
	uint8_t flag;
} rtk_bt_gatts_req_t;

/***************************** GATT Client related ***************************/
#if !RTK_BLE_MGR_LIB
typedef enum {
	BT_STACK_GATTC_DISC_REQ  = 1,
	BT_STACK_GATTC_READ_REQ,
	BT_STACK_GATTC_WRITE_REQ,
	BT_STACK_GATTC_WRITE_CMD,
	BT_STACK_GATTC_CCCD_ENABLE_REQ,
	BT_STACK_GATTC_CCCD_DISABLE_REQ,
	BT_STACK_GATTC_UNKNOWN = 0xFF,
} rtk_bt_gattc_req_type_t;

typedef struct{
	rtk_bt_gatt_queue_t request_queue[RTK_BLE_GAP_MAX_LINKS];
	rtk_bt_gatt_queue_t cmd_queue[RTK_BLE_GAP_MAX_LINKS];
	struct list_head cccd_record_list[RTK_BLE_GAP_MAX_LINKS];
	uint32_t last_seq;
} rtk_bt_gattc_app_priv_t;

typedef struct{
	struct list_head list;
	rtk_bt_gattc_req_type_t req_type;
	uint8_t conn_id;
	uint32_t seq;
	union {
		rtk_bt_gattc_discover_param_t disc_param;
		rtk_bt_gattc_read_param_t read_param;
		rtk_bt_gattc_write_param_t write_param;
		rtk_bt_gattc_update_cccd_param_t update_cccd_param;
	};
	void *user_data;			/* mark the 2nd level ptr in user param, convenient for free */
	uint8_t flag;				/* flag for internal use */
}rtk_bt_gattc_req_t;

typedef struct {
	struct list_head list;
	uint16_t profile_id;
	uint16_t conn_handle;
	uint16_t val_handle;
	uint16_t cccd_handle;
	uint16_t cccd_value;
} rtk_bt_gattc_cccd_record_t;
#endif

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_STACK_GATT_H__ */