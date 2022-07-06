/**
*****************************************************************************************
*     Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_ecfc.h
  * @brief   This file contains all the constants and functions prototypes for L2CAP enhanced credit based flow control mode.
  * @details
  * @author  jane
  * @date    2021-04-06
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __GAP_ECFC_H__
#define __GAP_ECFC_H__

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>
#include "gap.h"
#include "upperstack_config.h"

#ifdef  __cplusplus
extern "C" {
#endif
#if F_BT_5_2_L2C_ECFC_SUPPORT
/** @defgroup GAP_ECFC_MODULE GAP ECFC Module
  * @brief GAP enhanced credit based flow control mode module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_ECFC_Exported_Macros GAP ECFC Exported Macros
  * @{
  */

#define GAP_ECFC_CREATE_CHANN_MAX_NUM        5  //!< The maximum number of channels created at one time.
#define GAP_ECFC_MIN_MTU                     64 //!< Minimum length of MTU.
#define GAP_ECFC_MIN_MPS                     64 //!< Minimum length of MPS.


/** @defgroup GAP_ECFC_SEC_SETTING GAP ECFC Security Setting
 * @{
 */
#define GAP_ECFC_SEC_BIT_AUTHEN              0x02 //!< Authentication is required.
#define GAP_ECFC_SEC_BIT_MITM                0x04 //!< MITM authentication is required.
#define GAP_ECFC_SEC_BIT_AUTHOR              0x08 //!< User level authorization is required.
#define GAP_ECFC_SEC_BIT_ENCRYPT             0x10 //!< Encryption on link is required.
#define GAP_ECFC_SEC_BIT_SC                  0x20 //!< Secure connection on link is required.
/**
  * @}
  */

/** @defgroup GAP_ECFC_DATA_PATH GAP ECFC Data Path
 * @{
 */
#define GAP_ECFC_DATA_PATH_APP               0x00 //!< Application needs to handle the data. The ATT layer will not handle the data.
#define GAP_ECFC_DATA_PATH_GATT              0x01 //!< The ATT layer handles the data.
/**
  * @}
  */

/** End of GAP_ECFC_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_ECFC_Exported_Types GAP ECFC Exported Types
  * @{
  */

/** @brief L2CAP ECFC channel connection state*/
typedef enum {
	GAP_ECFC_STATE_DISCONNECTED  = 0, //!< Disconnected.
	GAP_ECFC_STATE_CONNECTED     = 2, //!< Connected.
	GAP_ECFC_STATE_DISCONNECTING = 3  //!< Disconnecting.
} T_GAP_ECFC_STATE;

/** @brief GAP ECFC Parameter List */
typedef enum {
	GAP_ECFC_PARAM_INIT_CREDITS      = 0x430, //!< Local initial credits. Write Only.  Default is 4.
	GAP_ECFC_PARAM_LOCAL_MPS         = 0x431, //!< Local MPS. Write Only. Default is 254.
} T_GAP_ECFC_PARAM_TYPE;

/**
 * @brief  Response of registering protocol over L2CAP. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_PROTO_REG_RSP.
 */
typedef struct {
	bool      is_le;    //!< Whether using LE transport.
	uint16_t  proto_id; //!< Protocol ID assigned by GAP ECFC Module
	uint16_t  psm;      //!< PSM of the protocol.
	uint16_t  cause;    //!< Result of registering protocol to L2CAP layer.
} T_GAP_ECFC_PROTO_REG_RSP;

/**
 * @brief  Indication of remote L2CAP ECFC connection request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_CONN_IND.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    conn_handle; //!< Connection handle.
	uint16_t    remote_mtu;  //!< Remote MTU value.
	uint8_t     cid_num;     //!< The number of the L2CAP ECFC channels request to establish.
	uint16_t    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local CID array assigned by Bluetooth stack.
	uint8_t     bd_addr[6];  //!< Bluetooth address of remote device.
	uint8_t     bd_type;     //!< Bluetooth address type of remote device. @ref T_GAP_REMOTE_ADDR_TYPE
	uint16_t    identity_id; //!< Identity id for gap_ecfc_send_conn_cfm.
} T_GAP_ECFC_CONN_IND;

/**
 * @brief  Response of starting L2CAP ECFC connection request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_CONN_RSP.
 */
typedef struct {
	uint16_t    proto_id;   //!< Protocol ID assigned by GAP ECFC Module.
	uint8_t     cid_num;    //!< The number of the L2CAP ECFC channels request to establish.
	uint16_t    cause;      //!< Result of executing L2CAP ECFC connection request.
	uint8_t     bd_addr[6]; //!< Bluetooth address of remote device.
	uint8_t     bd_type;    //!< Bluetooth address type of remote device. @ref T_GAP_REMOTE_ADDR_TYPE
} T_GAP_ECFC_CONN_RSP;

/**
 * @brief  Information of L2CAP ECFC channel establish result. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_CONN_CMPL.
 */
typedef struct {
	uint16_t    proto_id;       //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    cause;          //!< Result of establishing a L2CAP ECFC channels with remote device.
	uint16_t    conn_handle;    //!< Connection handle.
	uint8_t     cid_num;        //!< The number of the L2CAP ECFC channels successfully established.
	uint16_t    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local CID array assigned by Bluetooth stack.
	uint16_t    remote_mtu;     //!< Remote MTU value.
	uint16_t    local_mtu;      //!< Local MTU value.
	uint16_t    local_mps;      //!< Local MPS value.
	uint16_t    ds_data_offset; //!< Offset used to fill user data when sending L2CAP data to remote.
	uint8_t     bd_addr[6];     //!< Bluetooth address of remote device.
	uint8_t
	bd_type;        //!< Bluetooth address type of remote device. @ref T_GAP_REMOTE_ADDR_TYPE
} T_GAP_ECFC_CONN_CMPL_INFO;

/**
 * @brief  Indication of L2CAP data received from remote side. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DATA_IND.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    conn_handle; //!< Connection handle.
	uint16_t    cid;         //!< Local CID assigned by Bluetooth stack.
	uint16_t    length;      //!< Length of L2CAP data.
	uint16_t    gap;         //!< Offset from data parameter to the real L2CAP data.
	uint8_t     data[1];     //!< The rest of message which contains real L2CAP data at offset of gap.
} T_GAP_ECFC_DATA_IND;

/**
 * @brief  Response of local L2CAP data transmission. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DATA_RSP.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    conn_handle; //!< Connection handle.
	uint16_t    cid;         //!< Local CID assigned by Bluetooth stack.
	uint16_t    cause;       //!< Result of sending data.
} T_GAP_ECFC_DATA_RSP;

/**
 * @brief  Indication of receiving L2CAP disconnection request from remote device. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DISCONN_IND.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    conn_handle; //!< Connection handle.
	uint16_t    cid;         //!< Local CID assigned by Bluetooth stack.
	uint16_t    cause;       //!< L2CAP channel disconnect reason received.
} T_GAP_ECFC_DISCONN_IND;

/**
 * @brief  Response of sending L2CAP disconnection request to remote device. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_DISCONN_RSP.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    conn_handle; //!< Connection handle.
	uint16_t    cid;         //!< Local CID assigned by Bluetooth stack.
	uint16_t    cause;       //!< Result of disconnecting L2CAP channel with remote device.
} T_GAP_ECFC_DISCONN_RSP;

/**
 * @brief  Response of registering protocol security into Bluetooth stack. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_SEC_REG_RSP.
 */
typedef struct {
	bool        is_le;        //!< Whether using LE transport.
	uint16_t    psm;          //!< PSM of the protocol.
	uint8_t     active;       //!< Active/Deactive the security entry.
	uint16_t    uuid;         //!< UUID of the service.
	uint16_t    cause;        //!< Result of registering protocol security.
} T_GAP_ECFC_SEC_REG_RSP;

/**
 * @brief  Response of starting L2CAP ECFC reconfigure request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_RECONFIGURE_IND.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    cause;       //!< Result of reconfiguration.
	uint16_t    conn_handle; //!< Connection handle.
	uint8_t     cid_num;     //!< The number of the L2CAP ECFC channels successfully reconfigured.
	uint16_t    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local CID array assigned by Bluetooth stack.
	uint16_t    local_mtu;   //!< Local MTU value.
	uint16_t    local_mps;   //!< Local MPS value.
} T_GAP_ECFC_RECONFIGURE_RSP;

/**
 * @brief  Indication of remote L2CAP ECFC reconfigure request. It will be received in
 *         the callback function registered by @ref gap_ecfc_reg_proto with message
 *         type as @ref GAP_ECFC_RECONFIGURE_RSP.
 */
typedef struct {
	uint16_t    proto_id;    //!< Protocol ID assigned by GAP ECFC Module.
	uint16_t    conn_handle; //!< Connection handle.
	uint8_t     cid_num;     //!< The number of the L2CAP ECFC channels need to reconfigure.
	uint16_t    cid[GAP_ECFC_CREATE_CHANN_MAX_NUM]; //!< Local CID array assigned by Bluetooth stack.
	uint16_t    remote_mtu;  //!< Remote MTU.
	uint16_t    remote_mps;  //!< REmote MPS.
} T_GAP_ECFC_RECONFIGURE_IND;

/** @brief L2CAP ECFC connection confirm cause used in \ref gap_ecfc_send_conn_cfm API. */
typedef enum {
	GAP_ECFC_CONN_ACCEPT               = L2C_ECFC_ALL_CONN_SUCCESS,                                  //!< All connections accept
	GAP_ECFC_CONN_NO_RESOURCES         = (L2C_ERR | L2C_ECFC_CONN_RSP_SOME_CONN_NO_RESOURCES),       //!< Some connections refused:insufficient resources available
	GAP_ECFC_CONN_UNACCEPTABLE_PARAMS  = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_UNACCEPTABLE_PARAMS), //!< All connections refused:unacceptable parameters
	GAP_ECFC_CONN_INVALID_PARAMS       = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_INVALID_PARAMS),      //!< All connections refused:invalid parameters
	GAP_ECFC_CONN_PENDING_NO_INFO      = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_NO_INFO),     //!< All connections pending:no further information available
	GAP_ECFC_CONN_PENDING_AUTHOR       = (L2C_ERR | L2C_ECFC_CONN_RSP_ALL_CONN_PENDING_AUTHOR),      //!< All connections pending:authorization pending
} T_GAP_ECFC_CONN_CFM_CAUSE;

/** @brief L2CAP reconfigure confirm cause used in \ref GAP_ECFC_RECONFIGURE_IND callback.*/
typedef enum {
	GAP_ECFC_RCFG_ACCEPT               = L2C_ECFC_ALL_CONN_SUCCESS,                         //!< All connections accept
	GAP_ECFC_RCFG_UNACCEPTABLE_PARAMS  = (L2C_ERR | L2C_ECFC_RCFG_RSP_UNACCEPTABLE_PARAMS), //!< Reconfiguration failed - other unacceptable parameters
} T_GAP_ECFC_RCFG_CFM_CAUSE;

/** @brief  */
typedef enum {
	GAP_ECFC_PROTO_REG_RSP,              /**< Response of register protocol into L2CAP layer. Message data is \ref T_GAP_ECFC_PROTO_REG_RSP. */
	GAP_ECFC_SEC_REG_RSP,                /**< Response of register protocol security requirement into Bluetooth stack. Message data is \ref T_GAP_ECFC_SEC_REG_RSP. */
	GAP_ECFC_CONN_IND,                   /**< Indication of L2CAP ECFC connection request from remote device. Message data is \ref T_GAP_ECFC_CONN_IND. */
	GAP_ECFC_CONN_RSP,                   /**< Response of send L2CAP ECFC connection request to remote device. Message data is \ref T_GAP_ECFC_CONN_RSP. */
	GAP_ECFC_CONN_CMPL,                  /**< Information of L2CAP ECFC connection establish result. Message data is \ref T_GAP_ECFC_CONN_CMPL_INFO. */
	GAP_ECFC_DATA_IND,                   /**< Indication of L2CAP ECFC data received from remote device. Message data is \ref T_GAP_ECFC_DATA_IND. */
	GAP_ECFC_DATA_RSP,                   /**< Response of send L2CAP ECFC data to remote device. Message data is \ref T_GAP_ECFC_DATA_RSP. */
	GAP_ECFC_DISCONN_IND,                /**< Indication of L2CAP disconnection request from remote device. Message data is \ref T_GAP_ECFC_DISCONN_IND. */
	GAP_ECFC_DISCONN_RSP,                /**< Response of send L2CAP disconnection request ro remote device. Message data is \ref T_GAP_ECFC_DISCONN_RSP. */
	GAP_ECFC_RECONFIGURE_IND,            /**< Indication of L2CAP ECFC reconfigure request from remote device. Message data is \ref T_GAP_ECFC_RECONFIGURE_IND. */
	GAP_ECFC_RECONFIGURE_RSP             /**< Response of send L2CAP ECFC reconfigure request to remote device. Message data is \ref T_GAP_ECFC_RECONFIGURE_RSP. */
} T_GAP_ECFC_MSG;

/** End of GAP_ECFC_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_ECFC_EXPORT_Functions GAP ECFC Exported Functions
 *
 * @{
 */
/**
 * @brief Prototype of callback function to handle L2CAP ECFC related messages
 * @param[in] p_buf     Message data and the content of data is dependent on message type.
 * @param[in] msg       callback msy type @ref T_GAP_ECFC_MSG.
 * @return Result
 * @retval 0 Success
 * @retval other Failed
  */
typedef uint16_t (* P_GAP_ECFC_CB)(void *p_buf, T_GAP_ECFC_MSG msg);

/**
 * @brief  Initialize parameters of GAP ECFC Module.
 * @param[in] proto_num Initialize protocal number.
 * @return Operation result.
 * @retval true Operation success
 * @retval false Operation failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        gap_ecfc_init(2);
    }
 * \endcode
 */
bool gap_ecfc_init(uint8_t proto_num);

/**
 * @brief  Set a GAP ECFC parameter.
 * @param[in] param     Profile parameter ID: @ref T_GAP_ECFC_PARAM_TYPE
 * @param[in] len       Length of data to write
 * @param[in] p_value   Pointer to data to write.  This is dependent on
 *                      the parameter ID and WILL be cast to the appropriate
 *                      data type (For example: if data type param is uint16, p_value will be cast to
 *                      pointer of uint16_t).
 * @return Operation result.
 * @retval true Operation success
 * @retval false Operation failed.
 *
 * <b>Example usage</b>
 * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_ecfccfg(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t type = p_parse_value->dw_param[0];
        if (type == 0)
        {
            uint16_t initial_credits = p_parse_value->dw_param[1];
            if (gap_ecfc_set_param(GAP_ECFC_PARAM_INIT_CREDITS, sizeof(uint16_t), &initial_credits))
            {
                return RESULT_SUCESS;
            }
        }
        else if (type == 1)
        {
            uint16_t local_mps = p_parse_value->dw_param[1];
            if (gap_ecfc_set_param(GAP_ECFC_PARAM_LOCAL_MPS, sizeof(uint16_t), &local_mps))
            {
                return RESULT_SUCESS;
            }
        }
        return RESULT_ERR;
    }
 * \endcode
 */
bool gap_ecfc_set_param(T_GAP_ECFC_PARAM_TYPE param, uint8_t len, void *p_value);

/**
 * @brief  Register a callback function to handle L2CAP ECFC related messages for a specific PSM.
 * @param[in] psm       Protocol service multiplexer that the callback function is related.
 * @param[in] callback  Callback function to handle L2CAP ECFC messages. The function
 *                      must have the prototype defined as @ref P_GAP_ECFC_CB.
 * @param[in] is_le     Whether is LE transport.
 * @param[out] p_proto_id Protocol ID assigned by GAP ECFC module which will be used in @ref gap_ecfc_send_conn_req.
 * @param[in] data_path Data path to use. Should be use @ref GAP_ECFC_DATA_PATH.
 * @return The result of callback function register.
 * @retval true  The function was successfully registered.
 * @retval false The function was failed to register.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint8_t ecfc_proto_id;

   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_PROTO_REG_RSP:
            {
                T_GAP_ECFC_PROTO_REG_RSP *p_rsp = (T_GAP_ECFC_PROTO_REG_RSP *)p_buf;
                APP_PRINT_INFO3("GAP_ECFC_PROTO_REG_RSP: proto_id %d, psm 0x%x, cause 0x%x",
                                p_rsp->proto_id,
                                p_rsp->psm,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_reg_proto(uint16_t psm, P_GAP_ECFC_CB callback, bool is_le, uint8_t *p_proto_id,
						uint8_t data_path);

/**
 * @brief    Send a request to create L2CAP ECFC connections. If the request was successfully sent,
 *           @ref GAP_ECFC_CONN_RSP will be received in the callback function registed by
 *           @ref gap_ecfc_reg_proto to indicate whether the procedure was started successfully.
 *           If the procedure was started, @ref GAP_ECFC_CONN_CMPL will be received later to tell the
 *           result of L2CAP channel establishment.
 *
 * @param[in] psm       PSM of the L2CAP ECFC channel that will be established.
 * @param[in] proto_id  Protocol ID.
 * @param[in] local_mtu Preferred MTU size value of the L2CAP ECFC channel.
 * @param[in] bd_addr   Bluetooth address of remote device.
 * @param[in] bd_type   Bluetooth address type of remote device.
 * @param[in] cid_num   The number of the L2CAP ECFC channels request to establish. Range: (1- GAP_ECFC_CREATE_CHANN_MAX_NUM)
 * @return Operation result.
 * @retval true Send request success
 * @retval false Send request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_CONN_RSP:
            {
                T_GAP_ECFC_CONN_RSP *p_rsp = (T_GAP_ECFC_CONN_RSP *)p_buf;
                APP_PRINT_INFO5("GAP_ECFC_CONN_RSP: proto_id %d, cid_num %d, cause 0x%x, bd_addr %s, bd_type 0x%x",
                                p_rsp->proto_id,
                                p_rsp->cid_num,
                                p_rsp->cause,
                                TRACE_BDADDR(p_rsp->bd_addr),
                                p_rsp->bd_type);
            }
            break;

        case GAP_ECFC_CONN_CMPL:
            {
                T_GAP_ECFC_CONN_CMPL_INFO *p_info = (T_GAP_ECFC_CONN_CMPL_INFO *)p_buf;
                APP_PRINT_INFO6("GAP_ECFC_CONN_CMPL: proto_id %d, cause 0x%x, conn_handle 0x%x, ds_data_offset %d, bd_addr %s, bd_type 0x%x",
                                p_info->proto_id,
                                p_info->cause,
                                p_info->conn_handle,
                                p_info->ds_data_offset,
                                TRACE_BDADDR(p_info->bd_addr),
                                p_info->bd_type);
                APP_PRINT_INFO3("GAP_ECFC_CONN_CMPL: remote_mtu %d, local_mtu %d, local_mps %d",
                                p_info->remote_mtu,
                                p_info->local_mtu,
                                p_info->local_mps);
                for (uint8_t i = 0; i < p_info->cid_num; i++)
                {
                    APP_PRINT_INFO2("GAP_ECFC_CONN_CMPL: cis[%d] 0x%x", i, p_info->cid[i]);
                }
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
    static T_USER_CMD_PARSE_RESULT cmd_ecfccon(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t addr[6] = {0};
        uint8_t addr_len;
        for (addr_len = 0; addr_len < GAP_BD_ADDR_LEN; addr_len++)
        {
            addr[addr_len] = p_parse_value->dw_param[GAP_BD_ADDR_LEN - addr_len - 1];
        }
        if (gap_ecfc_send_conn_req(PSM_EATT, ecfc_proto_id,
                                ecfc_local_mtu, addr, GAP_REMOTE_ADDR_LE_PUBLIC, p_parse_value->dw_param[6]))
        {
            return RESULT_SUCESS;
        }
        return RESULT_ERR;
    }
 * \endcode
 */
bool gap_ecfc_send_conn_req(uint16_t psm, uint16_t proto_id,
							uint16_t local_mtu, uint8_t *bd_addr, uint8_t bd_type, uint8_t cid_num);

/**
 * @brief    Send a confirmation for a L2CAP ECFC connection request from remote device.
 *           If the confirmation was successfully sent with cause as @ref GAP_ECFC_CONN_ACCEPT,
 *           @ref GAP_ECFC_CONN_CMPL will also be received in the callback function registered by
 *           @ref gap_ecfc_reg_proto to tell the result of L2CAP ECFC channels establishment.
 *           If there was no resources to establishment all cid_num(@ref T_GAP_ECFC_CONN_IND) channels,
 *           the confirmation can reduce cid_num and sent with cause as @ref GAP_ECFC_CONN_NO_RESOURCES,
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] identity_id Indentity id. The value shall be get from @ref T_GAP_ECFC_CONN_IND message.
 * @param[in] cause       Confirmation cause for the connection request from remote device.
 * @param[in] p_cid       Confirmation the cid array allow to establish.
 *                        The array shall be the subset of the p_cid get from @ref T_GAP_ECFC_CONN_IND message.
 * @param[in] cid_num     Confirmation the number of the L2CAP ECFC channels allow to establish.
 * @param[in] local_mtu   Preferred MTU size value of the L2CAP ECFC channel.
 * @return Operation result.
 * @retval true Send request success
 * @retval false Send request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_CONN_IND:
            {
                T_GAP_ECFC_CONN_IND *p_ind = (T_GAP_ECFC_CONN_IND *)p_buf;
                T_GAP_ECFC_CONN_CFM_CAUSE cause = GAP_ECFC_CONN_ACCEPT;
                APP_PRINT_INFO8("GAP_ECFC_CONN_IND: proto_id %d, conn_handle 0x%x, remote_mtu %d, cid_num %d, cis %b, bd_addr %s, bd_type 0x%x, identity_id 0x%x",
                                p_ind->proto_id,
                                p_ind->conn_handle,
                                p_ind->remote_mtu,
                                p_ind->cid_num,
                                TRACE_BINARY(GAP_ECFC_CREATE_CHANN_MAX_NUM * 2, p_ind->cid),
                                TRACE_BDADDR(p_ind->bd_addr),
                                p_ind->bd_type,
                                p_ind->identity_id);
                gap_ecfc_send_conn_cfm(p_ind->conn_handle, p_ind->identity_id,
                                    cause, p_ind->cid, p_ind->cid_num, ecfc_local_mtu);
            }
            break;

        case GAP_ECFC_CONN_CMPL:
            {
                T_GAP_ECFC_CONN_CMPL_INFO *p_info = (T_GAP_ECFC_CONN_CMPL_INFO *)p_buf;
                APP_PRINT_INFO6("GAP_ECFC_CONN_CMPL: proto_id %d, cause 0x%x, conn_handle 0x%x, ds_data_offset %d, bd_addr %s, bd_type 0x%x",
                                p_info->proto_id,
                                p_info->cause,
                                p_info->conn_handle,
                                p_info->ds_data_offset,
                                TRACE_BDADDR(p_info->bd_addr),
                                p_info->bd_type);
                APP_PRINT_INFO3("GAP_ECFC_CONN_CMPL: remote_mtu %d, local_mtu %d, local_mps %d",
                                p_info->remote_mtu,
                                p_info->local_mtu,
                                p_info->local_mps);
                for (uint8_t i = 0; i < p_info->cid_num; i++)
                {
                    APP_PRINT_INFO2("GAP_ECFC_CONN_CMPL: cis[%d] 0x%x", i, p_info->cid[i]);
                }
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_send_conn_cfm(uint16_t conn_handle, uint16_t identity_id,
							T_GAP_ECFC_CONN_CFM_CAUSE cause,
							uint16_t *p_cid, uint8_t cid_num, uint16_t local_mtu);

/**
 * @brief    Send a request to disconnect a L2CAP ECFC connection. If the request was successfully sent,
 *           @ref GAP_ECFC_DISCONN_RSP will be received in the callback function registed by
 *           @ref gap_ecfc_reg_proto to show the result of disconnection.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] cid         Local channel ID of the L2CAP ECFC channel to disconnect.
 * @return Operation result.
 * @retval true Send request success
 * @retval false Send request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_DISCONN_RSP:
            {
                T_GAP_ECFC_DISCONN_RSP *p_rsp = (T_GAP_ECFC_DISCONN_RSP *)p_buf;
                APP_PRINT_INFO4("GAP_ECFC_DISCONN_RSP: proto_id %d, conn_handle 0x%x, cid 0x%x, cause 0x%x",
                                p_rsp->proto_id,
                                p_rsp->conn_handle,
                                p_rsp->cid,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_send_disconn_req(uint16_t conn_handle, uint16_t cid);

/**
 * @brief  Send a request to send L2CAP ECFC channel data to remote device.
 *         Application can use this API when data_path is GAP_ECFC_DATA_PATH_APP
 *         configured by @ref gap_ecfc_reg_proto.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] cid         Local channel ID. The value can be get from \ref GAP_ECFC_CONN_CMPL message.
 * @param[in] p_data      Point to data to be sent.
 * @param[in] length      Length of value to be sent. Range: 0 ~ local_mtu.
 * @return Operation result.
 * @retval true Send request success
 * @retval false Send request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_DATA_RSP:
            {
                T_GAP_ECFC_DATA_RSP *p_rsp = (T_GAP_ECFC_DATA_RSP *)p_buf;
                APP_PRINT_INFO4("GAP_ECFC_DATA_RSP: proto_id %d, conn_handle 0x%d, cid 0x%x, cause 0x%x",
                                p_rsp->proto_id,
                                p_rsp->conn_handle,
                                p_rsp->cid,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_send_data_req(uint16_t conn_handle, uint16_t cid, uint8_t *p_data, uint16_t length);

/**
 * @brief    Send a request to register/deregister a protocol security entry over L2CAP.
 *           The security will be used when establishing a L2CAP ECFC channel. If the
 *           request was successfully sent, a message whose type is \ref GAP_ECFC_SEC_REG_RSP
 *           and data is \ref T_GAP_ECFC_SEC_REG_RSP will be received in the callback
 *           function registered by \ref gap_ecfc_reg_proto.
 *
 * @param[in] is_le       Whether is LE transport.
 * @param[in] active      Register/Deregister the security entry.
 * @param[in] psm         PSM value for protocol.
 * @param[in] uuid
 * @param[in] requirement Security requirement of the entry. Valid values are combinations of \ref GAP_ECFC_SEC_SETTING.
 * @param[in] key_size    Key size requirement of the entry.
 * @return Operation result.
 * @retval true Send request success
 * @retval false Send request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_SEC_REG_RSP:
            {
                T_GAP_ECFC_SEC_REG_RSP *p_rsp = (T_GAP_ECFC_SEC_REG_RSP *)p_buf;
                APP_PRINT_INFO5("GAP_ECFC_SEC_REG_RSP: is_le %d, psm 0x%x, active %d, uuid 0x%x, cause 0x%x",
                                p_rsp->is_le,
                                p_rsp->psm,
                                p_rsp->active,
                                p_rsp->uuid,
                                p_rsp->cause);
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
   static T_USER_CMD_PARSE_RESULT cmd_ecfcsec(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t requirements = p_parse_value->dw_param[0];
        if (gap_ecfc_send_sec_reg_req(true, 1, PSM_EATT, 0, requirements, 16))
        {
            return RESULT_SUCESS;
        }
        else
        {
            return RESULT_ERR;
        }
    }
 * \endcode
 */
bool gap_ecfc_send_sec_reg_req(bool is_le, uint8_t active, uint16_t psm,
							   uint16_t uuid, uint8_t requirement, uint8_t key_size);

/**
 * @brief    Send a request to reconfigure L2CAP ECFC channel. If the request was successfully sent,
 *           @ref GAP_ECFC_RECONFIGURE_RSP will be received in the callback function registed by
 *           @ref gap_ecfc_reg_proto to indicate whether the procedure was successful.
 *
 * @param[in] conn_handle Connection handle of the ACL link.
 * @param[in] p_cid       Local CID array need to reconfigure.
 * @param[in] cid_num     The cid number of p_cid array. Range: (1- GAP_ECFC_CREATE_CHANN_MAX_NUM)
 * @param[in] local_mtu   Preferred Local MTU size value of the L2CAP ECFC channel.
 * @param[in] local_mps   Preferred Local MPS value of the L2CAP ECFC channel.
 * @return Operation result.
 * @retval true Send request success
 * @retval false Send request failed.
 *
 * <b>Example usage</b>
 * \code{.c}
   uint16_t app_ecfc_callback(void *p_buf, T_GAP_ECFC_MSG msg)
   {
       switch (msg)
       {
        case GAP_ECFC_RECONFIGURE_RSP:
            {
                T_GAP_ECFC_RECONFIGURE_RSP *p_rsp = (T_GAP_ECFC_RECONFIGURE_RSP *)p_buf;
                APP_PRINT_INFO7("GAP_ECFC_RECONFIGURE_RSP: proto_id %d, cause 0x%x, conn_handle 0x%x, local_mtu %d, local_mps %d,cid_num %d, cis %b",
                                p_rsp->proto_id,
                                p_rsp->cause,
                                p_rsp->conn_handle,
                                p_rsp->local_mtu,
                                p_rsp->local_mps,
                                p_rsp->cid_num,
                                TRACE_BINARY(GAP_ECFC_CREATE_CHANN_MAX_NUM * 2, p_rsp->cid));
            }
            break;

       default:
           break;
       }
   }

   int test(void)
   {
       gap_ecfc_reg_proto(PSM_EATT, app_ecfc_callback, true, &ecfc_proto_id, GAP_ECFC_DATA_PATH_GATT);

       return 0;
   }
 * \endcode
 */
bool gap_ecfc_send_reconfigure_req(uint16_t conn_handle, uint16_t *p_cid, uint8_t cid_num,
								   uint16_t local_mtu, uint16_t local_mps);

/** @} */ /* End of group GAP_ECFC_EXPORT_Functions */
/** @} */ /* End of group GAP_ECFC_MODULE */
#endif

#ifdef  __cplusplus
}
#endif

#endif

#endif  /*  __GAP_ECFC_H__ */
