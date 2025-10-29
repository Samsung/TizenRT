#include <components/system.h>
#include <os/os.h>
#include <os/mem.h>
#include <os/str.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mbedtls/aes.h"

#include "ble_tizenrt_impl.h"

#define BK_BLE_ADAPTER_TAG "bkble_storage"
#define LOGI(...) BK_BLE_LOGI(BK_BLE_ADAPTER_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_BLE_LOGW(BK_BLE_ADAPTER_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_BLE_LOGE(BK_BLE_ADAPTER_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_BLE_LOGI(BK_BLE_ADAPTER_TAG, ##__VA_ARGS__)

#define DEBUG_BT_STORAGE_SHOW 0

extern hal_ble_scan_env_t hal_ble_scan_env;
extern hal_ble_con_env_t hal_ble_con_env;

bt_bond_info_storage_t *bt_bond_info_storage = NULL;
bt_whitelist_info_storage_t *bt_whitelist_info_storage = NULL;

const uint8_t s_bt_empty_addr[6] = {0,0,0,0,0,0};
const uint8_t s_bt_invaild_addr[6] = {0xff,0xff,0xff,0xff,0xff,0xff};
const uint8_t s_bt_empty_irk[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


__attribute__((weak)) int32_t ef_set_env_blob(const char *key, const void *value_buf, size_t buf_len)
{
    LOGE("unimplement function!!");
    return -1;
}

__attribute__((weak)) int32_t ef_save_env(void)
{
    LOGE("unimplement function!!");
    return -1;
}

__attribute__((weak)) size_t ef_get_env_blob(const char *key, void *value_buf, size_t buf_len, size_t *saved_value_len)
{
    LOGE("unimplement function!!");
    return -1;
}

ble_list_t *ble_list_new(void)
{
    ble_list_t *list = (ble_list_t *) os_zalloc(sizeof(ble_list_t));
    if (!list) {
        return NULL;
    }

    list->head = list->tail = NULL;
    list->length = 0;
    return list;
}

bool ble_list_is_empty(const ble_list_t *list)
{
    return (list->length == 0);
}

ble_list_node_t *ble_list_free_node(ble_list_t *list, ble_list_node_t *node)
{
    ble_list_node_t *next = node->next;

    os_free(node->data);

    os_free(node);
    --list->length;

    return next;
}

void ble_list_clear(ble_list_t *list)
{
    for (ble_list_node_t *node = list->head; node; ) {
        node = ble_list_free_node(list, node);
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

void ble_list_free(ble_list_t *list)
{
    if (!list) {
        return;
    }

    ble_list_clear(list);
    os_free(list);
}

bool ble_list_append(ble_list_t *list, void *data)
{
    ble_list_node_t *node = (ble_list_node_t *)os_zalloc(sizeof(ble_list_node_t));
    if (!node) {
        LOGE("%s os_zalloc failed.\n", __func__ );
        return false;
    }
    node->next = NULL;
    node->data = data;
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    ++list->length;
    return true;
}

void *ble_list_front(const ble_list_t *list)
{
    return list->head->data;
}

bool ble_list_remove(ble_list_t *list, void *data)
{
    if (ble_list_is_empty(list)) {
        return false;
    }

    if (list->head->data == data) {
        ble_list_node_t *next = ble_list_free_node(list, list->head);
        if (list->tail == list->head) {
            list->tail = next;
        }
        list->head = next;
        return true;
    }

    for (ble_list_node_t *prev = list->head, *node = list->head->next; node; prev = node, node = node->next)
    {
        if (node->data == data) {
            prev->next = ble_list_free_node(list, node);
            if (list->tail == node) {
                list->tail = prev;
            }
            return true;
        }
    }
    return false;
}


bool bdaddr_compare(uint8_t *addr1, uint8_t *addr2)
{
    for (uint8_t idx = 0; idx < 6; idx++)
    {
        if (addr1[idx] != addr2[idx])
        {
            return false;
        }
    }

    return true;
}


uint8_t bk_adapter_ble_convert_addr_type_2_bk(uint8_t addr_type)
{
    uint8_t peer_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;

    if (TRBLE_ADDR_TYPE_PUBLIC == addr_type)
    {
        peer_addr_type = OWN_ADDR_TYPE_PUBLIC_ADDR;
    }
    else if (TRBLE_ADDR_TYPE_RANDOM_STATIC == addr_type
        || TRBLE_ADDR_TYPE_RANDOM_RESOLVABLE == addr_type
        || TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE == addr_type
    )
    {
        peer_addr_type = OWN_ADDR_TYPE_RANDOM_ADDR;
    }
    else
    {
        LOGE("%s , the addr type is not supported!!! %d\r\n",__func__,addr_type);
    }

    return peer_addr_type;
}

uint8_t bk_adapter_ble_convert_addr_type_2_tr(uint8_t addr_type, uint8_t *addr)
{
    if(addr_type == GAP_REMOTE_ADDR_LE_PUBLIC)
    {
        return TRBLE_ADDR_TYPE_PUBLIC;
    }else if(addr_type == GAP_REMOTE_ADDR_LE_RANDOM)
    {
        if(BLE_IS_RPA(addr_type, addr[5]))
        {
            return TRBLE_ADDR_TYPE_RANDOM_RESOLVABLE;
        }
        else if(BLE_IS_NON_PRA(addr_type, addr[5]))
        {
            return TRBLE_ADDR_TYPE_RANDOM_NON_RESOLVABLE;
        }else if(BLE_IS_STATIC_ADDR(addr_type, addr[5]))
        {
            return TRBLE_ADDR_TYPE_RANDOM_STATIC;
        }else
        {
            return TRBLE_ADDR_TYPE_UNKNOWN;
        }
    }else
    {
        return TRBLE_ADDR_TYPE_UNKNOWN;
    }
}

bool hal_ble_adv_reports_list_check(uint8_t *addr, uint8_t addr_type, uint8_t adv_evt_type)
{
    bool found = false;

    for (uint16_t i = 0; i < hal_ble_scan_env.nb_adv; i++)
    {
        if (bdaddr_compare(addr, hal_ble_scan_env.table[i].adv_addr)
            && (addr_type == hal_ble_scan_env.table[i].addr_type)
            && (adv_evt_type == hal_ble_scan_env.table[i].adv_evt_type))
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        struct adv_dup_filter_entry *dev =  hal_ble_scan_env.table + hal_ble_scan_env.curr_pos;
        os_memcpy(dev->adv_addr, addr, 6);
        dev->adv_evt_type = adv_evt_type;
        dev->addr_type = addr_type;

        hal_ble_scan_env.curr_pos++;
        if (hal_ble_scan_env.curr_pos == BLE_HAL_FILTER_DUPLICATES_MAX)
        {
            hal_ble_scan_env.curr_pos = 0;
        }

        if (hal_ble_scan_env.nb_adv < BLE_HAL_FILTER_DUPLICATES_MAX)
        {
            hal_ble_scan_env.nb_adv++;
        }
    }

    return found;
}


int ble_push_read_info(uint8_t conn_id, uint8_t client_id)
{
    int ret = 0;
    rd_info_t *rd_info = (rd_info_t *)os_malloc(sizeof(rd_info_t));
    if (rd_info)
    {
        rd_info->client_id = client_id;
        ble_list_append(hal_ble_con_env.con_dev[conn_id].rd_list, rd_info);
    }
    else
    {
        LOGE("%s, os_malloc failed!!!\r\n",__func__);
        ret = -1;
    }
    return ret;
}

int ble_pop_read_info(uint8_t conn_id, uint8_t *client_id)
{
    if (ble_list_is_empty(hal_ble_con_env.con_dev[conn_id].rd_list))
    {
        LOGW("%s, rd_list is empty\n", __func__);
        return -1;
    }

    rd_info_t *p_buf = ble_list_front(hal_ble_con_env.con_dev[conn_id].rd_list);

    *client_id = p_buf->client_id;

    ble_list_remove(hal_ble_con_env.con_dev[conn_id].rd_list, p_buf);

    return 0;
}

int ble_push_write_info(uint8_t conn_id, uint8_t client_id, uint16_t handle)
{
    int ret = 0;
    wr_info_t *wr_info = (wr_info_t *)os_malloc(sizeof(wr_info_t));
    if (wr_info)
    {
        wr_info->client_id = client_id;
        wr_info->att_handle = handle;
        ble_list_append(hal_ble_con_env.con_dev[conn_id].wr_list, wr_info);
    }
    else
    {
        LOGE("%s, os_malloc failed!!!\r\n",__func__);
        ret = -1;
    }
    return ret;
}

int ble_pop_write_info(uint8_t conn_id, uint8_t *client_id, uint16_t *handle)
{
    if (ble_list_is_empty(hal_ble_con_env.con_dev[conn_id].wr_list))
    {
        LOGW("%s, wr_list is empty\n", __func__);
        return -1;
    }

    wr_info_t *p_buf = ble_list_front(hal_ble_con_env.con_dev[conn_id].wr_list);

    *client_id = p_buf->client_id;
    *handle = p_buf->att_handle;

    ble_list_remove(hal_ble_con_env.con_dev[conn_id].wr_list, p_buf);

    return 0;
}

int32_t bk_adapter_bt_storage_alloc_linkkey_info(uint8_t *addr, uint8_t addr_type)
{
    (void) addr;
    (void) addr_type;
    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    for (int i = 0; i < BT_LINKKEY_MAX_SAVE_COUNT; ++i)
    {
        if (!os_memcmp(s_bt_empty_addr, bt_bond_info_storage->linkkey[i].peer_addr, 6)
            || !os_memcmp(s_bt_invaild_addr, bt_bond_info_storage->linkkey[i].peer_addr, 6))
        {
            os_memset(&(bt_bond_info_storage->linkkey[i]), 0, sizeof(bt_bond_info_storage->linkkey[i]));
            return i;
        }
    }

    return -1;
}

int32_t bk_adapter_bt_storage_find_linkkey_info_index(uint8_t *addr, uint8_t addr_type)
{
    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    for (int i = 0; i < BT_LINKKEY_MAX_SAVE_COUNT; ++i)
    {
        if ((!os_memcmp(addr, bt_bond_info_storage->linkkey[i].peer_addr, 6))
            && (addr_type == bt_bond_info_storage->linkkey[i].peer_addr_type))
        {
            return i;
        }
    }

    return -1;
}

int32_t bk_adapter_bt_storage_save_linkkey_info(bk_ble_key_t *linkkey)
{
    int32_t index = 0;

    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    index = bk_adapter_bt_storage_find_linkkey_info_index(linkkey->peer_addr, linkkey->peer_addr_type);

    if (index >= 0)
    {
        LOGD("%s find existed linkkey info index %d\n", __func__, index);
    }
    else
    {
        index = bk_adapter_bt_storage_alloc_linkkey_info(linkkey->peer_addr, linkkey->peer_addr_type);

        if (index >= 0)
        {

        }
        else
        {
            LOGE("%s overwrite 0 linkkey info, %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                      bt_bond_info_storage->linkkey[0].peer_addr[5],
                      bt_bond_info_storage->linkkey[0].peer_addr[4],
                      bt_bond_info_storage->linkkey[0].peer_addr[3],
                      bt_bond_info_storage->linkkey[0].peer_addr[2],
                      bt_bond_info_storage->linkkey[0].peer_addr[1],
                      bt_bond_info_storage->linkkey[0].peer_addr[0]);
            index = 0;
        }
    }

    bt_bond_info_storage->linkkey[index]= *linkkey;
    LOGD("save linkkey info, index %d, %02X:%02X:%02X:%02X:%02X:%02X\n", index,
                      linkkey->peer_addr[5],
                      linkkey->peer_addr[4],
                      linkkey->peer_addr[3],
                      linkkey->peer_addr[2],
                      linkkey->peer_addr[1],
                      linkkey->peer_addr[0]);
    return index;
}


int32_t bk_adapter_bt_storage_del_linkkey_info(uint8_t *addr, uint8_t addr_type)
{
    int32_t index = 0;

    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    index = bk_adapter_bt_storage_find_linkkey_info_index(addr, addr_type);

    if (index >= 0)
    {
        os_memset(&bt_bond_info_storage->linkkey[index], 0, sizeof(bt_bond_info_storage->linkkey[0]));
    }

    return index;
}

int32_t bk_adapter_bt_storage_clean_linkkey_info(void)
{
    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    memset(bt_bond_info_storage->linkkey, 0, sizeof(bt_bond_info_storage->linkkey));

    return 0;
}

int32_t bk_adapter_bt_storage_sync_bond_info_to_flash(void)
{
    int32_t ret = 0;

    ret = ef_set_env_blob(BT_BONDED_DEVICES_KEY, bt_bond_info_storage, sizeof(*bt_bond_info_storage));

    if (ret)
    {
        LOGE("%s ef_set_env_blob err %d\n", __func__, ret);
    }

    ret = ef_save_env();

    if (ret)
    {
        LOGE("%s ef_save_env err %d\n", __func__, ret);
    }

    return ret;
}


static uint8_t bluetooth_storage_is_addr_valid(uint8_t *addr)
{
    uint8_t sum_ff = 0xff, sum_zero = 0;

    for (int i = 0; i < 6; ++i)
    {
        sum_ff &= addr[i];
        sum_zero |= addr[i];
    }

    return sum_ff != 0xff && sum_zero != 0;
}

uint8_t bk_adapter_bt_storage_get_bond_device_num(void)
{
    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return 0;
    }

    uint8_t count = 0;
    for (int i = 0; i < sizeof(bt_bond_info_storage->linkkey) / sizeof(bt_bond_info_storage->linkkey[0]); ++i)
    {
        if (bluetooth_storage_is_addr_valid(bt_bond_info_storage->linkkey[i].peer_addr))
        {
            count++;
        }
    }

    return count;
}
uint8_t bk_adapter_bt_storage_get_bond_device(bt_bond_info_t *info, uint8_t size)
{
    if (!bt_bond_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return 0;
    }

    if (!info)
    {
        LOGE("%s info pointer is NULL\n", __func__);
        return 0;
    }

    uint8_t count = 0;
    int max_count = sizeof(bt_bond_info_storage->linkkey) / sizeof(bt_bond_info_storage->linkkey[0]);
    for (int i = 0; i < max_count; ++i)
    {
        if (bluetooth_storage_is_addr_valid(bt_bond_info_storage->linkkey[i].peer_addr))
        {
            if(count < size)
            {
                info[count].device.peer_addr_type = bt_bond_info_storage->linkkey[i].peer_addr_type;
                os_memcpy(info[count].device.peer_addr, bt_bond_info_storage->linkkey[i].peer_addr, 6);
            }
            count++;
        }
    }
    if(count != size)
    {
        LOGW("%s count not equal size, count %d, size %d\n", __func__, count, size);
    }
    return count;
}


int32_t bk_adapter_bt_storage_alloc_whitelist_info(uint8_t *addr, uint8_t addr_type)
{
    (void) addr;
    (void) addr_type;
    if (!bt_whitelist_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    for (int i = 0; i < BT_WHITELIST_MAX_SAVE_COUNT; ++i)
    {
        if (!os_memcmp(s_bt_empty_addr, bt_whitelist_info_storage->device[i].peer_addr, 6)
            || !os_memcmp(s_bt_invaild_addr, bt_whitelist_info_storage->device[i].peer_addr, 6))
        {
            os_memset(&(bt_whitelist_info_storage->device[i]), 0, sizeof(bt_whitelist_info_storage->device[i]));
            return i;
        }
    }

    return -1;
}

int32_t bk_adapter_bt_storage_find_whitelist_info_index(uint8_t *addr, uint8_t addr_type)
{
    if (!bt_whitelist_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    for (int i = 0; i < BT_WHITELIST_MAX_SAVE_COUNT; ++i)
    {
        if ((!os_memcmp(addr, bt_whitelist_info_storage->device[i].peer_addr, 6))
            && (addr_type == bt_whitelist_info_storage->device[i].peer_addr_type))
        {
            return i;
        }
    }

    return -1;
}

int32_t bk_adapter_bt_storage_save_whitelist_info(bt_storage_elem_whitelist_t *device)
{
    int32_t index = 0;
    int32_t save_flag = 0;

    if (!bt_whitelist_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    index = bk_adapter_bt_storage_find_whitelist_info_index(device->peer_addr, device->peer_addr_type);

    if (index >= 0)
    {

    }
    else
    {
        index = bk_adapter_bt_storage_alloc_whitelist_info(device->peer_addr, device->peer_addr_type);

        if (index >= 0)
        {

        }
        else
        {
            LOGE("%s overwrite 0 linkkey info, %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
                      bt_whitelist_info_storage->device[0].peer_addr[5],
                      bt_whitelist_info_storage->device[0].peer_addr[4],
                      bt_whitelist_info_storage->device[0].peer_addr[3],
                      bt_whitelist_info_storage->device[0].peer_addr[2],
                      bt_whitelist_info_storage->device[0].peer_addr[1],
                      bt_whitelist_info_storage->device[0].peer_addr[0]);
            index = 0;
        }

        save_flag = 1;
    }

    bt_whitelist_info_storage->device[index]= *device;

    return save_flag;
}


int32_t bk_adapter_bt_storage_del_whitelist_info(uint8_t *addr, uint8_t addr_type)
{
    int32_t index = 0;

    if (!bt_whitelist_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    index = bk_adapter_bt_storage_find_whitelist_info_index(addr, addr_type);

    if (index >= 0)
    {
        os_memset(&bt_whitelist_info_storage->device[index], 0, sizeof(bt_whitelist_info_storage->device[0]));
    }

    return index;
}

int32_t bk_adapter_bt_storage_clean_whitelist_info(void)
{
    if (!bt_whitelist_info_storage)
    {
        LOGE("%s not init\n", __func__);
        return -1;
    }

    memset(bt_whitelist_info_storage->device, 0, sizeof(bt_whitelist_info_storage->device));

    return 0;
}

int32_t bk_adapter_bt_storage_sync_whitelist_info_to_flash(void)
{
    int32_t ret = 0;

    ret = ef_set_env_blob(BT_WHITELIST_KEY, bt_whitelist_info_storage, sizeof(*bt_whitelist_info_storage));

    if (ret)
    {
        LOGE("%s ef_set_env_blob err %d\n", __func__, ret);
    }

    ret = ef_save_env();

    if (ret)
    {
        LOGE("%s ef_save_env err %d\n", __func__, ret);
    }

    return ret;
}
void bk_adapter_bt_storage_info_show(void)
{
    LOGD("%s\r\n", __func__);
    LOGD("--------------bonding list--------------\r\n");
    for (int i = 0; i < BT_LINKKEY_MAX_SAVE_COUNT; ++i)
    {
        LOGD("0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n", bt_bond_info_storage->linkkey[i].peer_addr[0], bt_bond_info_storage->linkkey[i].peer_addr[1],
            bt_bond_info_storage->linkkey[i].peer_addr[2], bt_bond_info_storage->linkkey[i].peer_addr[3], bt_bond_info_storage->linkkey[i].peer_addr[4],
            bt_bond_info_storage->linkkey[i].peer_addr[5], bt_bond_info_storage->linkkey[i].peer_addr_type);
    }

    LOGD("---------------whitelist----------------\r\n");
    for (int i = 0; i < BT_WHITELIST_MAX_SAVE_COUNT; ++i)
    {
        LOGD("0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n", bt_whitelist_info_storage->device[i].peer_addr[0], bt_whitelist_info_storage->device[i].peer_addr[1],
            bt_whitelist_info_storage->device[i].peer_addr[2], bt_whitelist_info_storage->device[i].peer_addr[3], bt_whitelist_info_storage->device[i].peer_addr[4],
            bt_whitelist_info_storage->device[i].peer_addr[5], bt_whitelist_info_storage->device[i].peer_addr_type);
    }
}

void bk_adapter_ble_display_white_list_data(void)
{
    for (int i = 0; i < BT_WHITELIST_MAX_SAVE_COUNT; ++i)
    {
        if (os_memcmp(s_bt_empty_addr, bt_whitelist_info_storage->device[i].peer_addr, 6)
         && os_memcmp(s_bt_invaild_addr, bt_whitelist_info_storage->device[i].peer_addr, 6))
        {
            LOGD("0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n", bt_whitelist_info_storage->device[i].peer_addr[0], bt_whitelist_info_storage->device[i].peer_addr[1],
                bt_whitelist_info_storage->device[i].peer_addr[2], bt_whitelist_info_storage->device[i].peer_addr[3], bt_whitelist_info_storage->device[i].peer_addr[4],
                bt_whitelist_info_storage->device[i].peer_addr[5], bt_whitelist_info_storage->device[i].peer_addr_type);
        }
    }
}

uint8_t bk_adapter_ble_check_white_list_addr(uint8_t *addr, uint8_t addr_type)
{
    uint8_t ret = 0;

    if (bk_adapter_bt_storage_find_whitelist_info_index(addr , addr_type) >= 0)
    {
        ret = 1;
    }

    return ret;
}

void bk_adapter_ble_clear_white_list_data(void)
{
    LOGD("%s \r\n",__func__);

    if (bk_adapter_bt_storage_clean_whitelist_info() >=0)
    {
        bk_adapter_bt_storage_sync_whitelist_info_to_flash();
    }
}

void bk_adapter_ble_add_white_list_data(uint8_t *addr, uint8_t addr_type)
{
    LOGD("%s, 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n",__func__, addr[0], addr[1], addr[2],
        addr[3], addr[4], addr[5], addr_type);

    bt_storage_elem_whitelist_t device;
    device.peer_addr_type = addr_type;
    os_memcpy(device.peer_addr, addr, 6);

    if (bk_adapter_bt_storage_save_whitelist_info(&device) == 1)
    {
        bk_adapter_bt_storage_sync_whitelist_info_to_flash();
    }
}

void bk_adapter_ble_remove_white_list_data(uint8_t *addr, uint8_t addr_type)
{
    LOGD("%s, 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n",__func__, addr[0], addr[1], addr[2],
        addr[3], addr[4], addr[5], addr_type);

    if (bk_adapter_bt_storage_del_whitelist_info(addr, addr_type) >=0)
    {
        bk_adapter_bt_storage_sync_whitelist_info_to_flash();
    }
}

int32_t bk_adapter_bt_storage_init(void)
{
    int32_t ret = 0;
    size_t saved_len = 0;

    LOGD("%s \n", __func__);

    if (!bt_bond_info_storage)
    {
        bt_bond_info_storage = os_malloc(sizeof(*bt_bond_info_storage));

        if (!bt_bond_info_storage)
        {
            LOGE("%s bt_bond_info_storage alloc fail\n", __func__);
            return -1;
        }

        os_memset(bt_bond_info_storage, 0, sizeof(*bt_bond_info_storage));

        ret = ef_get_env_blob(BT_BONDED_DEVICES_KEY, bt_bond_info_storage, sizeof(*bt_bond_info_storage), &saved_len);

        if (ret != sizeof(*bt_bond_info_storage) || saved_len != sizeof(*bt_bond_info_storage))
        {
            LOGE("%s ef_get_env_blob err1 %d %d\n", __func__, ret, saved_len);

            if (saved_len < sizeof(*bt_bond_info_storage))
            {
                os_memset(((uint8_t *)bt_bond_info_storage) + saved_len, 0, sizeof(*bt_bond_info_storage) - saved_len);
            }
        }
    }

    if (!bt_whitelist_info_storage)
    {
        bt_whitelist_info_storage = os_malloc(sizeof(*bt_whitelist_info_storage));

        if (!bt_whitelist_info_storage)
        {
            LOGE("%s bt_whitelist_info_storage alloc fail\n", __func__);
            return -1;
        }

        os_memset(bt_whitelist_info_storage, 0, sizeof(*bt_whitelist_info_storage));

        ret = ef_get_env_blob(BT_WHITELIST_KEY, bt_whitelist_info_storage, sizeof(*bt_whitelist_info_storage), &saved_len);

        if (ret != sizeof(*bt_whitelist_info_storage) || saved_len != sizeof(*bt_whitelist_info_storage))
        {
            LOGE("%s ef_get_env_blob err2 %d %d\n", __func__, ret, saved_len);

            if (saved_len < sizeof(*bt_whitelist_info_storage))
            {
                os_memset(((uint8_t *)bt_whitelist_info_storage) + saved_len, 0, sizeof(*bt_whitelist_info_storage) - saved_len);
            }
        }
    }

#if DEBUG_BT_STORAGE_SHOW
    bk_adapter_bt_storage_info_show();
#endif

    (void)saved_len;
    (void)ret;
    return 0;
}

int32_t bk_adapter_bt_storage_deinit(void)
{
    if (bt_bond_info_storage)
    {
        os_free(bt_bond_info_storage);
        bt_bond_info_storage = NULL;
    }

    if (bt_whitelist_info_storage)
    {
        os_free(bt_whitelist_info_storage);
        bt_whitelist_info_storage = NULL;
    }

    return 0;
}

void bk_adapter_ble_swap_buf(uint8_t *dst, const uint8_t *src, uint16_t len)
{
    int i;
    if (dst == NULL || src == NULL)
    {
        return;
    }

    for (i = 0; i < len; i++)
    {
        dst[len - 1 - i] = src[i];
    }
}

void bk_adapter_ble_find_initial_pseudo_addr(uint8_t *addr)
{
    mbedtls_aes_context aes;
    uint8_t key[16];
    uint8_t plain_text[16];
    uint8_t encrypted_data[16];

    mbedtls_aes_init(&aes);

    for (int i = 0; i < BT_LINKKEY_MAX_SAVE_COUNT; ++i)
    {
        if (os_memcmp(s_bt_empty_addr, bt_bond_info_storage->linkkey[i].peer_addr, 6)
            && os_memcmp(s_bt_invaild_addr, bt_bond_info_storage->linkkey[i].peer_addr, 6)
            && os_memcmp(s_bt_empty_irk, bt_bond_info_storage->linkkey[i].pirk.irk, 16))
        {
            bk_adapter_ble_swap_buf(key, bt_bond_info_storage->linkkey[i].pirk.irk, 16);
            mbedtls_aes_setkey_enc(&aes, key, 128);

            os_memset(plain_text, 0, sizeof(plain_text));
            plain_text[13] = addr[5];
            plain_text[14] = addr[4];
            plain_text[15] = addr[3];

            mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, plain_text, encrypted_data);

            if((encrypted_data[13] == addr[2]) && (encrypted_data[14] == addr[1]) && (encrypted_data[15] == addr[0]))
            {
                os_memcpy(addr, bt_bond_info_storage->linkkey[i].peer_addr, 6);

                LOGD("find initial pseudo addr, 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\r\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
                break;
            }
        }
    }

    mbedtls_aes_free(&aes);
}

void bk_adapter_reset_client_info(uint8 conidx)
{
    // uint8_t i = 0;  // Unused variable

    if (hal_ble_con_env.con_dev[conidx].wr_list)
    {
        ble_list_clear(hal_ble_con_env.con_dev[conidx].wr_list);
    }

    if (hal_ble_con_env.con_dev[conidx].rd_list)
    {
        ble_list_clear(hal_ble_con_env.con_dev[conidx].rd_list);
    }

    if (hal_ble_con_env.con_dev[conidx].dis_list)
    {
        ble_list_clear(hal_ble_con_env.con_dev[conidx].dis_list);
    }
}

void le_bond_clear_all_keys(void)
{
    LOGD("%s \r\n",__func__);

    if (bk_adapter_bt_storage_clean_linkkey_info() >=0)
    {
        bk_adapter_bt_storage_sync_bond_info_to_flash();
    }
}

void le_bond_delete_by_bd(uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE bd_type)
{
    LOGD("%s, 0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n",__func__, bd_addr[0], bd_addr[1], bd_addr[2],
        bd_addr[3], bd_addr[4], bd_addr[5], bd_type);

    if (bk_adapter_bt_storage_del_linkkey_info(bd_addr, bd_type) >=0)
    {
        bk_adapter_bt_storage_sync_bond_info_to_flash();
    }
}

bool le_get_conn_info(uint8_t conn_id, T_GAP_CONN_INFO *p_conn_info)
{
    if (conn_id >= HAL_BLE_CON_NUM)
    {
        LOGE("%s, the conn_id(%d) exceeds the link limit(%d)\r\n",__func__,conn_id, HAL_BLE_CON_NUM);
        return 0;
    }

    os_memcpy(p_conn_info->remote_bd, hal_ble_con_env.con_dev[conn_id].peer_addr, 6);
    p_conn_info->remote_bd_type = hal_ble_con_env.con_dev[conn_id].peer_addr_type;
    p_conn_info->role = hal_ble_con_env.con_dev[conn_id].role;
    p_conn_info->conn_state = hal_ble_con_env.con_dev[conn_id].con_status;

    LOGD("%s, conn_id %d ,0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x(%d)\r\n",__func__, conn_id, p_conn_info->remote_bd[0], p_conn_info->remote_bd[1],
         p_conn_info->remote_bd[2], p_conn_info->remote_bd[3], p_conn_info->remote_bd[4], p_conn_info->remote_bd[5], p_conn_info->remote_bd_type);

    if (GAP_CONN_STATE_CONNECTED != p_conn_info->conn_state)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

hal_ble_conn_t * le_get_conn_info_ext(uint8_t conn_id)
{
    if (conn_id >= HAL_BLE_CON_NUM)
    {
        LOGE("%s, the conn_id(%d) exceeds the link limit(%d)\r\n", __func__, conn_id, HAL_BLE_CON_NUM);
        return 0;
    }

    return GAP_CONN_STATE_CONNECTED != hal_ble_con_env.con_dev[conn_id].con_status ?
                    NULL:
                    &hal_ble_con_env.con_dev[conn_id];
}

T_GAP_CONN_INFO *le_get_conn_info_by_mac(uint8_t *mac, uint32_t *index)
{
   for (uint32_t i = 0; i < sizeof(hal_ble_con_env.con_dev) / sizeof(hal_ble_con_env.con_dev[0]); ++i)
   {
       if(hal_ble_con_env.con_dev[i].con_status == GAP_CONN_STATE_CONNECTED &&
                       !os_memcpy(mac, hal_ble_con_env.con_dev[i].peer_addr, sizeof(hal_ble_con_env.con_dev[i].peer_addr)))
       {
           if(index)
           {
               *index = i;
           }

           return &hal_ble_con_env.con_dev[i];
       }
   }

   return NULL;
}

uint8_t le_get_active_link_num(void)
{
    uint8_t link_num = 0;
    for (uint8_t i = 0; i < HAL_BLE_CON_NUM; i++)
    {
        if (HAL_CONN_STATE_CONNECTED == hal_ble_con_env.con_dev[i].con_status)
        {
            link_num++;
        }
    }
    return link_num;
}

uint8_t le_get_idle_link_num(void)
{
    uint8_t link_num = 0;
    for (uint8_t i = 0; i < HAL_BLE_CON_NUM; i++)
    {
        if (HAL_CONN_STATE_DISCONNECTED == hal_ble_con_env.con_dev[i].con_status)
        {
            link_num++;
        }
    }
    return link_num;
}


