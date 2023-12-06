#ifndef __BLE_MGR_H
#define __BLE_MGR_H

#include <stdint.h>
#include <stdbool.h>
#include "gap_callback_le.h"
#include "gap_msg.h"

typedef struct
{
    struct
    {
        bool enable;
        uint32_t adv_num;
    } ble_ext_adv;

    struct
    {
        bool enable;
        uint32_t link_num;
    } ble_conn;

    struct
    {
        bool enable;
        bool update_scan_data;
        T_GAP_LOCAL_ADDR_TYPE own_address_type;
        T_GAP_REMOTE_ADDR_TYPE peer_address_type;
        uint32_t adv_interval;
        uint32_t scan_rsp_len;
        uint8_t  *scan_rsp_data;
    } ble_adv_data;

    struct
    {
        bool enable;
    } ble_scan;

} BLE_MGR_PARAMS;

typedef void (*BLE_MGR_MSG_CB)(uint8_t subtype, T_LE_GAP_MSG *gap_msg);

/**
 * @brief ble_mgr_init
 * initialize ble manager module
 * @param p_params
 * \code{.c}
    void app_ble_gap_ble_mgr_init(void)
    {
        BLE_MGR_PARAMS param = {0};
        param.ble_ext_adv.enable = true;
        param.ble_ext_adv.adv_num = app_ble_gap_get_ext_adv_num();
        param.ble_conn.enable = true;
        uint8_t supported_max_le_link_num = le_get_max_link_num();
        param.ble_conn.link_num = ((MAX_BLE_LINK_NUM <= supported_max_le_link_num) ?
                                    MAX_BLE_LINK_NUM : supported_max_le_link_num);
        if (extend_app_cfg_const.ama_support || extend_app_cfg_const.xiaowei_support ||
            extend_app_cfg_const.bisto_support || extend_app_cfg_const.xiaoai_support)
        {
            param.ble_adv_data.enable = true;
            param.ble_adv_data.update_scan_data = true;
            param.ble_adv_data.adv_interval = (extend_app_cfg_const.multi_adv_interval * 8) / 5;
            app_ble_gap_gen_scan_rsp_data(&scan_rsp_data_len, scan_rsp_data);
            param.ble_adv_data.scan_rsp_len = scan_rsp_data_len;
            param.ble_adv_data.scan_rsp_data = scan_rsp_data;
        }
        param.ble_scan.enable = true;
        ble_mgr_init(&param);
    }
 * \endcode
 */
void ble_mgr_init(BLE_MGR_PARAMS *p_params);

/**
 * @brief ble_mgr_handle_gap_cb
 * used to handle GAP Callback Message in gap_callback_le.h
 * @param cb_type
 * @param cb_data
 * \code{.c}
 *  static T_APP_RESULT app_ble_gap_cb(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        ......
        ble_mgr_handle_gap_cb(cb_type, p_cb_data);
        ......
    }
 * \endcode
 */
void ble_mgr_handle_gap_cb(uint8_t cb_type, T_LE_CB_DATA *cb_data);

/**
 * @brief ble_mgr_handle_gap_msg
 * used to handle GAP Message
 * @param subtype
 * @param gap_msg
 * \code{.c}
 *  void app_ble_gap_handle_gap_msg(T_IO_MSG *p_io_msg)
    {
        T_LE_GAP_MSG stack_msg;
        memcpy(&stack_msg, &p_io_msg->u.param, sizeof(p_io_msg->u.param));
        ......
        ble_mgr_handle_gap_msg(p_io_msg->subtype, &stack_msg);
        ......
    }
 * \endcode
 */
void ble_mgr_handle_gap_msg(uint8_t subtype, T_LE_GAP_MSG *gap_msg);

/**
 * @brief ble_mgr_msg_cback_register
 * other module can register callback function to handle GAP Message
 * @param cb
 * @return true
 * @return false
    void app_device_init(void)
    {
        ble_mgr_msg_cback_register(app_device_ble_cback);
        ......
    }
 * \endcode
 */
bool ble_mgr_msg_cback_register(BLE_MGR_MSG_CB cb);


void ble_mgr_deinit(void);
#endif
