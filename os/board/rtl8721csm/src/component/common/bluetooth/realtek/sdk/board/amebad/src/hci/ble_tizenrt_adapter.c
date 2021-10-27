/**
 * Copyright (c) 2017, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <tinyara/net/if/ble.h>
#include "stddef.h"
#include "gap_customer.h"

trble_result_e rtw_ble_get_version(uint8_t *version)
{
    if (version == NULL)
        return TRBLE_FAIL;

    int ret;
    T_BT_VERSION_INFO ble_version;
    ret = gap_get_bt_version_info(&ble_version);

    if(ret)
    {
        version[0] = (uint8_t) (ble_version.lmp_subversion >> 8);
        version[1] = (uint8_t) ble_version.lmp_subversion;
        version[2] = (uint8_t) ble_version.btgap_buildnum;
        return TRBLE_SUCCESS;
    }
    return TRBLE_FAIL;
}
