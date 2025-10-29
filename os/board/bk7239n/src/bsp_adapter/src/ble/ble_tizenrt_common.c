#include <tinyara/net/if/ble.h>
#include "ble_tizenrt_impl.h"

#include "bk_ble_tizenrt.h"
#include "ble_tizenrt_common.h"

#include "components/bluetooth/bk_ble_types.h"
#include "components/bluetooth/bk_ble.h"
#include "components/bluetooth/bk_dm_bluetooth.h"

#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef LOGE
    #undef LOGE
#endif

#ifdef LOGW
    #undef LOGW
#endif

#ifdef LOGI
    #undef LOGI
#endif

#ifdef LOGD
    #undef LOGD
#endif

#ifdef LOGV
    #undef LOGV
#endif

#ifdef LOG_TAG
    #undef LOG_TAG
#endif

#ifdef LOG_LEVEL
    #undef LOG_LEVEL
#endif

enum
{
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE,
};

#define LOG_TAG "bkble_common"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOGE(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_ERROR)   BK_BLE_LOGE(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGW(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_WARNING) BK_BLE_LOGW(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGI(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_INFO)    BK_BLE_LOGI(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGD(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_DEBUG)   BK_BLE_LOGD(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)
#define LOGV(format, ...) do{if(LOG_LEVEL >= LOG_LEVEL_VERBOSE) BK_BLE_LOGV(LOG_TAG, "%s:" format "\n", __func__, ##__VA_ARGS__);} while(0)


trble_result_e bktr_ble_common_get_mac_address(uint8_t mac[TRBLE_BD_ADDR_MAX_LEN])
{
    uint8_t bd_mac[BK_BT_ADDR_LEN] = {0};
    // int ret = bk_bluetooth_get_address(bd_mac);
    int ret = bk_get_mac(bd_mac, MAC_TYPE_BLUETOOTH);
    if (ret)
    {
        LOGE("get_address fail");
        return TRBLE_FAIL;
    }
    memcpy(mac, bd_mac, BK_BT_ADDR_LEN);
    LOGD("mac: %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_common_get_bonded_device(trble_bonded_device_list_s *bonded_device_list, uint16_t *device_count)
{
    if (bonded_device_list == NULL || device_count == NULL)
    {
        LOGE("bonded_device_list or device_count is NULL");
        return TRBLE_INVALID_ARGS;
    }

    uint8_t device_num = bk_adapter_bt_storage_get_bond_device_num();
    LOGI("bonded device num: %d, bond_list len:%d", device_num, *device_count);
    *device_count = device_num;
    if(*device_count == 0)
    {
        LOGW("no bonded device");
        return TRBLE_SUCCESS;
    }
    bt_bond_info_t *bond_info = os_zalloc(sizeof(bt_bond_info_t) * (*device_count));

    if (bond_info == NULL)
    {
        LOGE("os_zalloc fail");
        return TRBLE_FAIL;
    }

    bk_adapter_bt_storage_get_bond_device(bond_info, *device_count);

    for (uint8_t i = 0; i < *device_count; i++)
    {
        bonded_device_list[i].bd_addr.type = bk_adapter_ble_convert_addr_type_2_tr(bond_info[i].device.peer_addr_type, bond_info[i].device.peer_addr);
        memcpy(bonded_device_list[i].bd_addr.mac, bond_info[i].device.peer_addr, TRBLE_BD_ADDR_MAX_LEN);
        LOGI("bonded device[%d]: mac: %02x:%02x:%02x:%02x:%02x:%02x, addr_type %d", i,
             bonded_device_list[i].bd_addr.mac[0], bonded_device_list[i].bd_addr.mac[1], bonded_device_list[i].bd_addr.mac[2],
             bonded_device_list[i].bd_addr.mac[3], bonded_device_list[i].bd_addr.mac[4], bonded_device_list[i].bd_addr.mac[5],
             bonded_device_list[i].bd_addr.type);
    }

    if (bond_info)
    {
        os_free(bond_info);
    }

    return TRBLE_SUCCESS;
}


bool bktr_ble_common_conn_is_active(trble_conn_handle con_handle)
{
    T_GAP_CONN_INFO conn_info;
    le_get_conn_info(con_handle, &conn_info);

    if (conn_info.conn_state == GAP_CONN_STATE_CONNECTED && conn_info.role == GAP_LINK_ROLE_SLAVE)
    {
        return true;
    }

    return false;
}

bool bktr_ble_common_conn_is_any_active(void)
{
    uint8_t active_link_num = le_get_active_link_num();
    return (active_link_num > 0);
}

trble_result_e bktr_ble_common_conn_param_update(trble_conn_handle *conn_handle, trble_conn_param *conn_param)
{
    if (le_update_conn_param(*conn_handle, conn_param->min_conn_interval, conn_param->max_conn_interval, conn_param->slave_latency, conn_param->supervision_timeout, 0, 0))
    {
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_common_delete_bond(trble_addr *addr)
{
    if (addr == NULL)
    {
        LOGE("addr is NULL");
        return TRBLE_FAIL;
    }
    le_bond_delete_by_bd(addr->mac, addr->type);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_common_delete_bond_all(void)
{
    le_bond_clear_all_keys();
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_common_sm_set_security_param(trble_sec_param *sec_param)
{
    LOGD("io_cap: %d, oob_data_flag: %d, bond_flag: %d, mitm_flag: %d, sec_pair_flag: %d, sec_pair_only_flag: %d, use_fixed_key: %d, fixed_key: %d",
         sec_param->io_cap, sec_param->oob_data_flag, sec_param->bond_flag, sec_param->mitm_flag, sec_param->sec_pair_flag,
         sec_param->sec_pair_only_flag, sec_param->use_fixed_key, sec_param->fixed_key);
    hal_ble_env.auth_io_cap = sec_param->io_cap;
    hal_ble_env.auth_oob = sec_param->oob_data_flag;
    hal_ble_env.bond_flag = sec_param->bond_flag;
    hal_ble_env.mitm_flag = sec_param->mitm_flag;
    hal_ble_env.sec_pair_flag = sec_param->sec_pair_flag;
    hal_ble_env.sec_pair_only_flag = sec_param->sec_pair_only_flag; //todo: add only sec_pair function, not support now.
    hal_ble_env.use_fixed_key = sec_param->use_fixed_key;
    hal_ble_env.fixed_key = sec_param->fixed_key;
    hal_ble_env.auth_flags = 0;
    hal_ble_env.sec_req = GAP_SEC1_NOAUTH_PAIR_ENC;
    if (hal_ble_env.bond_flag)
    {
        hal_ble_env.auth_flags |= GAP_AUTH_BOND;
    }

    if (hal_ble_env.mitm_flag)
    {
        hal_ble_env.auth_flags |= GAP_AUTH_MITM;
        hal_ble_env.sec_req = GAP_SEC1_AUTH_PAIR_ENC;
    }

    if (hal_ble_env.sec_pair_flag)
    {
        hal_ble_env.auth_flags |= GAP_AUTH_SEC_CON;
    }

    if (hal_ble_env.sec_pair_only_flag)
    {
        hal_ble_env.auth_flags |= GAP_AUTH_SEC_CON;
        hal_ble_env.sec_req = GAP_SEC1_SEC_CON_PAIR_ENC;
    }
    LOGD("set secparam success, auth_flags: %d, sec_req: %d", hal_ble_env.auth_flags, hal_ble_env.sec_req);
    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_common_pairing_passkey_confirm(uint8_t *conn_handle, uint8_t *confirm)
{
    if (bk_ble_number_compare_send(*conn_handle, *confirm))
    {
        LOGE("pairing passkey confirm fail");
        return TRBLE_FAIL;
    }

    return TRBLE_SUCCESS;
}

trble_result_e bktr_ble_common_get_version(uint8_t *version)
{
#define BLE_LMP_SUBVERSION_H 0x00
#define BLE_LMP_SUBVERSION_L 0x06
#define BLE_GAP_BUILD_VERSION 0x01

    if (version == NULL)
    {
        return TRBLE_INVALID_ARGS;
    }

    version[0] = BLE_LMP_SUBVERSION_H;
    version[1] = BLE_LMP_SUBVERSION_L;
    version[2] = BLE_GAP_BUILD_VERSION;
    LOGD("ble version: 0x%02x%02x%02x", version[0], version[1], version[2]);
    return TRBLE_SUCCESS;
}

int32_t bk_tr_ble_common_init(void)
{
    return 0;
}

int32_t bk_tr_ble_common_deinit(void)
{
    return 0;
}
