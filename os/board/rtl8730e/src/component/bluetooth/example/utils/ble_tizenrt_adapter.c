/**
 * Copyright (c) 2017, Realsil Semiconductor Corporation. All rights reserved.
 *
 */

#include <tinyara/net/if/ble.h>
#include "stddef.h"
#include "gap_customer.h"
#include "rtk_bt_le_gap.h"
#include "rtk_bt_def.h"

trble_result_e rtw_ble_get_version(uint8_t *version)
{
    if (version == NULL)
        return TRBLE_FAIL;

    int ret;
    rtk_bt_le_version_info_t ble_version;
    ret = rtk_bt_le_gap_get_version(&ble_version);

    if(RTK_BT_OK == ret)
    {
        version[0] = (uint8_t) (ble_version.lmp_subversion >> 8);
        version[1] = (uint8_t) ble_version.lmp_subversion;
        version[2] = (uint8_t) ble_version.btgap_buildnum;
        return TRBLE_SUCCESS;
    }
    return TRBLE_FAIL;
}
