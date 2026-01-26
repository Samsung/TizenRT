#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef struct
{
    uint8_t _is_gatt_discovery_auto;                //only valid when use single ble host.
    uint8_t _ignore_smp_key_distr_all_zero;         //only valid when use single ble host.
    uint8_t _strict_smp_key_distr_check_except_all_zero;
    uint8_t _ignore_smp_already_pair;               //only valid when use single ble host.
    uint8_t _send_peripheral_feature_req_auto;
    uint8_t _stop_smp_when_pair_err;
    uint8_t _check_kernel_msg_queue_repeat;
    uint8_t _auto_rsp_att_indicate_when_no_all_discover;
    uint8_t _support_ble_adv_txpwr_set;
	uint8_t _auto_rsp_write_req;
    uint8_t _support_reconnection_strategy;
    uint8_t _support_reporting_coex_ble_state;
    uint16_t _ble_max_latency;
    uint8_t _support_lowpower_sleep;
    uint8_t _ble_coc_local_cid_monotonous_increase;
}bt_feature_struct_t;
