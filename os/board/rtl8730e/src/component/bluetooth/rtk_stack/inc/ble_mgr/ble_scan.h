/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */
#ifndef __BLE_SCAN_H
#define __BLE_SCAN_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BLE_SCAN Ble Scan magager
  * @brief Ble scan manager module
  * @note Each of other module can independently manage scan policies and actions by scan handle,
  *       and has independent callback to receive and filter reported advertising packets.
  * @{
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <stdint.h>
#include "gap_ext_scan.h"

/*============================================================================*
 *                              Constants
 *============================================================================*/

/** @defgroup BLE_SCAN_FILTER BLE SCAN FILTER
  * @brief Ble scan filter flags to filter reported advertising packets.
  * @{
  */
#define BLE_SCAN_FILTER_NONE            0x00 /**< Report all advertising packets */
#define BLE_SCAN_FILTER_EVT_TYPE_BIT    0x01 /**< Flag used to filter reported advertising packets by Advertising Event Properties*/
#define BLE_SCAN_FILTER_ADV_DATA_BIT    0x02 /**< Flag used to filter reported advertising packets by Advertising Data*/
#define BLE_SCAN_FILTER_REMOTE_ADDR_BIT 0x04 /**< Flag used to filter reported advertising packets by remote address type and remote address*/
/** @} End BLE_SCAN_FILTER */


/*============================================================================*
 *                              Types
 *============================================================================*/
/**
 * @brief Ble scan handle, which is used to manage a set of scan policy and scan actions.
 */
typedef void *BLE_SCAN_HDL;

/**
 * @brief Define the ble scan events would be reported to and handled in ble scan callback.
 */
typedef enum
{
    BLE_SCAN_REPORT          = 0,
    BLE_SCAN_PARAM_CHANGES   = 1,
    BLE_SCAN_INVALID_EVT,
} BLE_SCAN_EVT;

/**
 * @brief Used to filter reported adv by remote address type and remote address when @ref BLE_SCAN_FILTER_REMOTE_ADDR_BIT is set.
 */
typedef struct
{
    uint8_t bd_type; /**< Refer to @ref T_GAP_REMOTE_ADDR_TYPE*/
    uint8_t bd_addr[6]; /**< Remote address*/
} BLE_SCAN_ADDR;

/**
 * @brief Used to set filter parameters.
 */
typedef struct
{
    uint16_t filter_flags; /**< Filter falgs @ref BLE_SCAN_FILTER*/
    uint16_t evt_type; /**< Advertising event properties @ref EXT_ADV_EVT_PROP intended to filter: used when @ref BLE_SCAN_FILTER_EVT_TYPE_BIT
                           is set in filter_flags*/
    uint16_t ad_len; /**< The length of data pointed by @ref ad_struct intended to filter: used when @ref BLE_SCAN_FILTER_ADV_DATA_BIT
                         is set in filter_flags*/
    uint8_t addr_num; /**< Number of address(es) intended to filter, it should be corresponding with address(es) @ref p_scan_addr pointed to: used
                          when @ref BLE_SCAN_FILTER_REMOTE_ADDR_BIT is set in filter_flags*/
    uint8_t *ad_struct; /**< Advertising data intended to filter, and the first byte should be @ref ADV_DATA_TYPE : used when
                            @ref BLE_SCAN_FILTER_ADV_DATA_BIT is set in filter_flags*/
    BLE_SCAN_ADDR *p_scan_addr; /**< @ref BLE_SCAN_ADDR, Pointer to remote address(es) intended to
                                     filter: used when @ref BLE_SCAN_FILTER_REMOTE_ADDR_BIT is set in filter_flags*/
} BLE_SCAN_FILTER;

/**
 * @brief  Used to report scan event data in @ref BLE_SCAN_CB .
 */
typedef union
{
    T_LE_EXT_ADV_REPORT_INFO *report; /**< data of @ref BLE_SCAN_REPORT*/
} BLE_SCAN_EVT_DATA;

/**
 * @brief  Used to set scan param.
 */
typedef struct
{
    T_GAP_LOCAL_ADDR_TYPE own_addr_type; /**< Local address type @ref T_GAP_LOCAL_ADDR_TYPE*/
    uint8_t phys; /**< Scan advertisements on the specified PHY: @ref EXT_SCAN_PHY */
    T_GAP_SCAN_FILTER_POLICY    ext_filter_policy; /**< Used to set scan filter policy */
    T_GAP_SCAN_FILTER_DUPLICATE ext_filter_duplicate; /**< Used to set scan duplicate filter type */
    T_GAP_LE_EXT_SCAN_PARAM scan_param_1m; /**< Used to set scan param for LE 1M PHY when
                                                @ref GAP_EXT_SCAN_PHYS_1M_BIT is set in phys */
    T_GAP_LE_EXT_SCAN_PARAM scan_param_coded; /**< Used to set scan param for LE Coded PHY when
                                                   @ref GAP_EXT_SCAN_PHYS_CODED_BIT is set in phys*/
} BLE_SCAN_PARAM;

/**
 * @brief  Definition of scan callback
 */
typedef void (*BLE_SCAN_CB)(BLE_SCAN_EVT evt, BLE_SCAN_EVT_DATA *p_data);

/*============================================================================*
 *                              Functisons
 *============================================================================*/
/**
 * @brief Used to get current scan state.
 * @note It is generated according to all of the scan handles' scan states.
 * @return @ref GAP_SCAN_STATE
 *
 * <b>Example usage</b>
 * \code{.c}
    void get_cur_ble_scan_state_test(void)
    {
        uint8_t cur_scan_state;
        cur_scan_state = le_scan_get_cur_state();
    }
 * \endcode
 */
uint8_t ble_scan_get_cur_state(void);

/**
 * @brief Used to get current scan param.
 * @note It is generated according to all of the scan handles' scan param.
 * @param[out] p_scan_param  the current scan param would be filled in p_scan_param.
 * @return    bool
 * @retval    true  succeeded to get current scan param.
 * @retval    false  failed to get current scan param.
 *
 * <b>Example usage</b>
 * \code{.c}
    void get_cur_ble_scan_param_test(void)
    {
        BLE_SCAN_PARAM param;
        bool result = false;
        result = ble_scan_get_cur_param(&param);
    }
 * \endcode
 */
bool ble_scan_get_cur_param(BLE_SCAN_PARAM *p_scan_param);

/**
 * @brief Used to get current scan param used by scan handle.
 * @param[in] hdl @ref  BLE_SCAN_HDL
 * @param[out] p_scan_param  if the scan handle is existed, the current scan param used by hdl would be filled in p_scan_param.
 * @return    bool
 * @retval    true  succeeded to get current scan param.
 * @retval    false  failed to get current scan param.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool print_ble_scan_param_test(BLE_SCAN_HDL scan_hdl)
    {
        BLE_SCAN_PARAM scan_param;
        if(scan_hdl == NULL)
        {
            return false;
        }

        if (ble_scan_get_param_by_hdl(scan_hdl, &scan_param))
        {
            APP_PRINT_INFO4("print_ble_scan_param_test: own_addr_type %d, scan_phy %d, filter_policy %d, ext_filter_duplicate %d",
                            scan_param.own_addr_type,
                            scan_param.scan_phy,
                            scan_param.filter_policy,
                            scan_param.ext_filter_duplicate);

            if(scan_param.scan_phy & GAP_EXT_SCAN_PHYS_1M_BIT)
            {
                APP_PRINT_INFO3("print_ble_scan_param_test: SCAN_PHYS_1M, scan_type %d, scan_interval 0x%04x, scan_window 0x%04x",
                                scan_param.scan_param_1m.scan_type,
                                scan_param.scan_param_1m.scan_interval,
                                scan_param.scan_param_1m.scan_window);
            }

            if(scan_param.scan_phy & GAP_EXT_SCAN_PHYS_CODED_BIT)
            {
                APP_PRINT_INFO3("print_ble_scan_param_test: SCAN_PHYS_CODED, scan_type %d, scan_interval 0x%04x, scan_window 0x%04x",
                                scan_param.scan_param_1m.scan_type,
                                scan_param.scan_param_1m.scan_interval,
                                scan_param.scan_param_1m.scan_window);
            }
            return true;
        }
        return false;
    }
 * \endcode
 */
bool ble_scan_get_param_by_hdl(BLE_SCAN_HDL hdl, BLE_SCAN_PARAM *p_scan_param);

/**
 * @brief Used to set current scan param used by scan handle.
 * @param[in] hdl  @ref BLE_SCAN_HDL
 * @param[in] p_scan_param  used to set the hdl's current scan param.
 * @return    bool
 * @retval    true  succeeded to set the hdl's scan param.
 * @retval    false  failed to set the hdl's scan param.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool set_ble_scan_param_test(BLE_SCAN_HDL scan_hdl)
    {
        BLE_SCAN_PARAM scan_param;
        if(scan_hdl == NULL)
        {
            return false;
        }

        if (ble_scan_get_param_by_hdl(scan_hdl, &scan_param))
        {
            scan_param.scan_param_1m.scan_type = GAP_SCAN_MODE_ACTIVE;
            scan_param.scan_param_1m.scan_interval = 0x100;
            scan_param.scan_param_1m.scan_window = 0x50;
            if (ble_scan_set_param_by_hdl(scan_hdl, &scan_param))
            {
                return true;
            }
        }
        return false;
    }
 * \endcode
 */
bool ble_scan_set_param_by_hdl(BLE_SCAN_HDL hdl, BLE_SCAN_PARAM *p_scan_param);

/**
 * @brief Used to request to start scan by scan handle.
 * @param[out] hdl  @ref BLE_SCAN_HDL, pointer to the created scan handle.
 * @param[in] cb  @ref BLE_SCAN_CB, used to receive and handle @ref BLE_SCAN_EVT
 * @param[in] p_scan_param  used to set the hdl's scan param.
 * @param[in] p_filter  used to set the hdl's filter policy.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool start_ble_scan_test(void)
    {
        BLE_SCAN_HDL ble_scan_hdl = NULL;
        BLE_SCAN_PARAM param;
        BLE_SCAN_FILTER scan_filter;
        BLE_SCAN_ADDR scan_addr[2];

        memset(&param, 0, sizeof(param));
        memset(&scan_filter, 0, sizeof(scan_filter));

        param.scan_param_1m.scan_type = GAP_SCAN_MODE_PASSIVE;
        param.scan_param_1m.scan_interval = scan_interval;
        param.scan_param_1m.scan_window = scan_window;
        param.ext_filter_duplicate = GAP_SCAN_FILTER_DUPLICATE_DISABLE;
        param.ext_filter_policy = GAP_SCAN_FILTER_WHITE_LIST;
        param.own_addr_type = GAP_LOCAL_ADDR_LE_PUBLIC;
        param.phys = GAP_EXT_SCAN_PHYS_1M_BIT;

        scan_filter.filter_flags = (BLE_SCAN_FILTER_EVT_TYPE_BIT | BLE_SCAN_FILTER_ADV_DATA_BIT |
                                    BLE_SCAN_FILTER_REMOTE_ADDR_BIT);

        //filter advertising data
        ad_struct[0] = GAP_ADTYPE_16BIT_COMPLETE; //the first byte of ad_struct should be @ref ADV_DATA_TYPE
        ad_struct[1] = LO_WORD(GATT_UUID_BATTERY);
        ad_struct[2] = HI_WORD(GATT_UUID_BATTERY);
        scan_filter.ad_len = 3;
        scan_filter.ad_struct = ad_struct;

        //filter remote address(es)
        scan_addr[0].bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        scan_addr[0].bd_addr[0] = 0x66;
        scan_addr[0].bd_addr[1] = 0x66;
        scan_addr[0].bd_addr[2] = 0x66;
        scan_addr[0].bd_addr[3] = 0x56;
        scan_addr[0].bd_addr[4] = 0x22;
        scan_addr[0].bd_addr[5] = 0x03;
        scan_addr[1].bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        scan_addr[1].bd_addr[0] = 0x77;
        scan_addr[1].bd_addr[1] = 0x77;
        scan_addr[1].bd_addr[2] = 0x77;
        scan_addr[1].bd_addr[3] = 0x56;
        scan_addr[1].bd_addr[4] = 0x22;
        scan_addr[1].bd_addr[5] = 0x03;

        scan_filter.addr_num = 2;
        scan_filter.p_scan_addr = scan_addr;

        //filter adv event property
        scan_filter.evt_type = LE_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED;

        bool rtn = ble_scan_start(&ble_scan_hdl, bud_scan_cb, &param, &scan_filter);
        return rtn;
    }
 * \endcode
 */

bool ble_scan_start(BLE_SCAN_HDL *p_hdl, BLE_SCAN_CB cb,
                    BLE_SCAN_PARAM *p_scan_param, BLE_SCAN_FILTER *p_filter);

/**
 * @brief Used to request to stop scan by scan handle.
 * @param[in] hdl  @ref BLE_SCAN_HDL
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    bool stop_ble_scan_test(void)
    {
        return ble_scan_stop(&scan_hdl);
    }
 * \endcode
 */
bool ble_scan_stop(BLE_SCAN_HDL *p_hdl);

/**
 * @brief Used to request to pause all the handles' scan.
 * @note When scan filter policy( @ref T_GAP_SCAN_FILTER_POLICY) is @ref GAP_SCAN_FILTER_WHITE_LIST or
 *       @ref GAP_SCAN_FILTER_WHITE_LIST_RPA, it could be used to pause scan before modifying whitelist.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_GAP_CAUSE app_ble_gap_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                                          T_GAP_REMOTE_ADDR_TYPE bd_type)
    {
        T_GAP_CAUSE ret;
        if (white_list_op_busy == false)
        {
            white_list_op_busy = true;
            ble_scan_pause();
            ret = le_modify_white_list(operation, bd_addr, bd_type);
            ble_scan_resume();
        }
        return ret;
    }
 * \endcode
 */
bool ble_scan_pause(void);

/**
 * @brief Used to request to resume all the handles' scan.
 * @return Operation result.
 * @note When scan filter policy( @ref T_GAP_SCAN_FILTER_POLICY) is @ref GAP_SCAN_FILTER_WHITE_LIST or
 *       @ref GAP_SCAN_FILTER_WHITE_LIST_RPA, it could be used to resume scan after modifying whitelist.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_GAP_CAUSE app_ble_gap_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                                          T_GAP_REMOTE_ADDR_TYPE bd_type)
    {
        T_GAP_CAUSE ret;
        if (white_list_op_busy == false)
        {
            white_list_op_busy = true;
            ble_scan_pause();
            ret = le_modify_white_list(operation, bd_addr, bd_type);
            ble_scan_resume();
        }
        return ret;
    }
 * \endcode
 */
bool ble_scan_resume(void);

/** End of BLE_SCAN
* @}
*/

#ifdef __cplusplus
}
#endif

#endif

