/*
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __MPA_H__
#define __MPA_H__

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include <stdint.h>
#include "gap.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * \defgroup    MPA       MPA
 *
 * \brief   Provide functions to control L2CAP layer in Bluetooth stack.
 * \details MPA(Multi protocol adaptor) is designed for user to implement protocols over L2CAP easily.
 *          Many functions are provided to connect, disconnect or send data over L2CAP channels.
 *          Upstream messages can be received in callback fucntions registered in MPA.\n
 * \note    The structure here must be sycn with internal btif layer.
 *
 */

/**
 * mpa.h
 *
 * \name  MPA_L2C_MODE
 * \brief L2CAP channel mode.
 * \anchor  MPA_L2C_MODE
 */
/**
 * \ingroup     MPA
 */
#define MPA_L2C_MODE_BASIC              (1 << 0x00)    /**< L2CAP channel mode: basic mode. */
#define MPA_L2C_MODE_ERTM               (1 << 0x03)    /**< L2CAP channel mode: enhanced retransmission mode. */

/**
 * mpa.h
 *
 * \name  MPA_SEC_SETTING
 * \brief Protocol security requirement settings bit mask.
 * \anchor  MPA_SEC_SETTING
 *
 */
/**
 * \ingroup     MPA
 */
/**@{*/
#define MPA_SEC_BIT_OUTGOING            0x01    /**< Request for outgoing connection. */
#define MPA_SEC_BIT_AUTHEN              0x02    /**< Authentication is required. */
#define MPA_SEC_BIT_MITM                0x04    /**< MITM authentication is required, only possible if authentication is set. */
#define MPA_SEC_BIT_AUTHOR              0x08    /**< User level authorization is required. */
#define MPA_SEC_BIT_ENCRYPT             0x10    /**< Encryption on link is required, only possible if authentication is set. */
/**@}*/

/**
 * mpa.h
 *
 * \name  MPA_ATT_PSM_DATA_PATH
 * \brief The ATT data path of the L2CAP channel using \ref PSM_ATT.
 * \anchor  MPA_ATT_PSM_DATA_PATH
 *
 */
/**
 * \ingroup     MPA
 */
/**@{*/
#define MPA_ATT_PSM_DATA_PATH_APP       0x00 /**< Application needs to handle the data. The ATT layer will not handle the data. */
#define MPA_ATT_PSM_DATA_PATH_GATT      0x01 /**< The ATT layer handles the data. */
/**@}*/

/**
 * mpa.h
 *
 * \brief    Response of registering protocol over L2CAP. It will be received in
 *           the callback function registered by \ref mpa_reg_l2c_proto with message
 *           type as \ref L2C_PROTO_REG_RSP.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t  psm;      /**< PSM of the protocol. */
	uint16_t  proto_id; /**< Protocol ID assigned by MPA layer. */
	uint16_t  cause;    /**< Result of registering protocol to L2CAP layer. */
} T_MPA_L2C_PROTO_REG_RSP;

/**
 * mpa.h
 *
 * \brief    Indication of remote L2CAP connection request. It will be received in
 *           the callback function registered by \ref mpa_reg_l2c_proto with message
 *           type as \ref L2C_CONN_IND.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cid;        /**< Local CID assigned by Bluetooth stack. */
	uint16_t    proto_id;   /**< Protocol ID which is assigned by MPA. */
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_MPA_L2C_CONN_IND;

/**
 * mpa.h
 *
 * \brief    Response of starting L2CAP connection request. It will be received in
 *           the callback function registered by \ref mpa_reg_l2c_proto with message
 *           type as \ref L2C_CONN_RSP.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cid;        /**< Local CID assigned by Bluetooth stack. */
	uint16_t    proto_id;   /**< Protocol ID assigned by MPA. */
	uint16_t    cause;      /**< Result of executing L2CAP connection request. */
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_MPA_L2C_CONN_RSP;

/**
 * mpa.h
 *
 * \brief    Information of L2CAP channel establish result. It will be received
 *           in the callback function registered by \ref mpa_reg_l2c_proto with
 *           message type as \ref L2C_CONN_CMPL.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cause;          /**< Result of establishing a L2CAP channel with remote device. */
	uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
	uint16_t    remote_mtu;     /**< Remote MTU value. */
	uint16_t    proto_id;       /**< Protocol ID assigned by MPA. */
	uint16_t    ds_data_offset; /**< Offset used to fill user data when sending L2CAP data to remote. */
	uint8_t     bd_addr[6];     /**< Bluetooth address of remote device. */
	uint16_t    local_mtu;      /**< Local MTU value. */
	uint16_t    conn_handle;    /**< Connection handle. */
} T_MPA_L2C_CONN_CMPL_INFO;

/**
 * mpa.h
 *
 * \brief    Indication of L2CAP data received from remote side. It will be received
 *           in the callback function registered by \ref mpa_reg_l2c_proto with
 *           message type as \ref L2C_DATA_IND.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint32_t    bt_clock; /**< Bluetooth native clock. */
	uint16_t    proto_id; /**< Protocol ID assigned by upper layer. */
	uint16_t    cid;      /**< Local CID assigned by Bluetooth stack. */
	uint16_t    length;   /**< Length of L2CAP data. */
	uint16_t    gap;      /**< Offset from data parameter to the real L2CAP data. */
	uint8_t     data[1];  /**< The rest of message which contains real L2CAP data at offset of gap. */
} T_MPA_L2C_DATA_IND;

/**
 * mpa.h
 *
 * \brief    Indication of L2CAP data response with ack flag set. It will be received
 *           in the callback function registered by \ref mpa_reg_l2c_proto with
 *           message type as \ref L2C_DATA_RSP.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
	uint8_t     proto_id;       /**< Protocol ID assigned by MPA. */
	uint8_t     dlci;          /**< RFCOMM DLCI assigned by RFCOMM. */
} T_MPA_L2C_DATA_RSP;

/**
 * mpa.h
 *
 * \brief    Indication of receiving L2CAP disconnection request from remote device. It will
 *           be received in the callback function registered by \ref mpa_reg_l2c_proto
 *           with message type as \ref L2C_DISCONN_IND.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
	uint16_t    proto_id;       /**< Protocol ID assigned by MPA. */
	uint16_t    cause;          /**< L2CAP channel disconnect reason received. */
} T_MPA_L2C_DISCONN_IND;

/**
 * mpa.h
 *
 * \brief    Response of sending L2CAP disconnection request to remote device. It will be
 *           received in the callback function registered by \ref mpa_reg_l2c_proto
 *           with message type as \ref L2C_DISCONN_RSP.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cid;            /**< Local CID assigned by Bluetooth stack. */
	uint16_t    proto_id;       /**< Protocol ID assigned by MPA. */
	uint16_t    cause;          /**< Result of disconnecting L2CAP channel with remote device. */
} T_MPA_L2C_DISCONN_RSP;

/**
 * mpa.h
 *
 * \brief    Response of registering protocol security into Bluetooth stack. It will be
 *           received in the callback function registered by \ref mpa_reg_l2c_proto
 *           with message type as \ref L2C_SEC_REG_RSP.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    psm;            /**< PSM of the protocol. */
	uint16_t    server_chann;   /**< Server channel when PSM is RFCOMM, otherwise is 0. */
	uint8_t     active;         /**< Active/Deactive the security entry. */
	uint16_t    uuid;           /**< UUID of the protocol the security entry registered for. */
	uint16_t    cause;          /**< Result of registering protocol security. */
} T_MPA_L2C_SEC_REG_RSP;

/**
 * mpa.h
 *
 * \brief    Indication to request user's authorization for service connection
 *           establish. It will be received in the callback function registered by
 *           \ref mpa_reg_l2c_proto with message type as \ref L2C_PROTO_AUTHOR_IND.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    psm;          /**< PSM of the protocol. */
	uint16_t    server_chann; /**< Server channel number when PSM is RFCOMM, otherwise is 0. */
	uint16_t    uuid;         /**< UUID of the service. */
	uint8_t     bd_addr[6];   /**< Bluetooth address of remote device. */
	uint8_t     outgoing;     /**< Direction of the service establishment, 1: outgoing, 0: incoming. */
} T_MPA_AUTHOR_REQ_IND;

/**
 * mpa.h
 *
 * \brief    Reponse of authenticaion on profile over RFCOMM channel. It will be
 *           received in the callback function registered by \ref mpa_reg_rfc_authen_cb.
 *
 * \ingroup  MPA
 */
typedef struct {
	uint16_t    cid;        /**< Local CID assigned by Bluetooth stack. */
	uint16_t    dlci;       /**< DCLI value of the RFCOMM channel. */
	uint16_t    cause;      /**< Result of authentication on the profile. */
	uint8_t     outgoing;   /**< Direction of the service establishment, 1: outgoing, 0: incoming. */
	uint8_t     bd_addr[6]; /**< Bluetooth address of remote device. */
} T_MPA_RFC_AUTHEN_RSP;

/**
 * mpa.h
 *
 * \brief    L2CAP confirm cause used in \ref mpa_send_l2c_conn_cfm API.
 *
 * \ingroup  MPA
 */
typedef enum {
	MPA_L2C_CONN_ACCEPT               = L2C_CONN_ACCEPT,                        /**< L2CAP connection accept */
	MPA_L2C_CONN_PENDING              = (L2C_ERR | L2C_ERR_PENDING),            /**< L2CAP connection pending */
	MPA_L2C_CONN_INVALID_PSM          = (L2C_ERR | L2C_ERR_INVALID_PSM),        /**< L2CAP connection reject because of PSM not support */
	MPA_L2C_CONN_SECURITY_BLOCK       = (L2C_ERR | L2C_ERR_SECURITY_BLOCK),     /**< L2CAP connection reject because of security block */
	MPA_L2C_CONN_NO_RESOURCE          = (L2C_ERR | L2C_ERR_NO_RESOURCE),        /**< L2CAP connection reject because of no resources available */
	MPA_L2C_CONN_INVALID_PARAM        = (L2C_ERR | L2C_ERR_INVALID_PARAM),      /**< L2CAP connection reject because of invalid parameter */
	MPA_L2C_CONN_INVALID_SOURCE_CID   = (L2C_ERR | L2C_ERR_INVALID_SOURCE_CID), /**< L2CAP connection reject because of invalid source CID */
	MPA_L2C_CONN_SOURCE_CID_ALLOCATED = (L2C_ERR | L2C_ERR_SOURCE_CID_ALLOCATED)/**< L2CAP connection reject because of source CID already allocated */
} T_MPA_L2C_CONN_CFM_CAUSE;

/**
 * mpa.h
 *
 * \brief   Messages types that protocols will get through callback function registed by \ref mpa_reg_l2c_proto.
 *
 * \ingroup     MPA
 */
typedef enum {
	L2C_PROTO_REG_RSP,              /**< Response of register protocol into L2CAP layer. Message data is \ref T_MPA_L2C_PROTO_REG_RSP. */
	L2C_CONN_IND,                   /**< Indication of L2CAP connection request from remote device. Message data is \ref T_MPA_L2C_CONN_IND. */
	L2C_CONN_RSP,                   /**< Response of send L2CAP connection request to remote device. Message data is \ref T_MPA_L2C_CONN_RSP. */
	L2C_CONN_CMPL,                  /**< Information of L2CAP connection establish result. Message data is \ref T_MPA_L2C_CONN_CMPL_INFO. */
	L2C_DATA_IND,                   /**< Indication of L2CAP data received from remote device. Message data is \ref T_MPA_L2C_DATA_IND. */
	L2C_DATA_RSP,                   /**< Indication of L2CAP data response with ack flag set. Message data is \ref T_MPA_L2C_DATA_RSP. */
	L2C_DISCONN_IND,                /**< Indication of L2CAP disconnection request from remote device. Message data is \ref T_MPA_L2C_DISCONN_IND. */
	L2C_DISCONN_RSP,                /**< Response of send L2CAP disconnection request ro remote device. Message data is \ref T_MPA_L2C_DISCONN_RSP. */
	L2C_SEC_REG_RSP,                /**< Response of register protocol security requirement into Bluetooth stack. Message data is \ref T_MPA_L2C_SEC_REG_RSP. */
	L2C_PROTO_AUTHOR_IND,           /**< Indication of request authorization for service connection establish. Message data is \ref T_MPA_AUTHOR_REQ_IND. */
} T_PROTO_MSG;

/**
 * mpa.h
 *
 * \brief   Prototype of callback function to handle L2CAP related messages from MPA.
 * \details Message types are defined in \ref T_PROTO_MSG. Message data is located
 *          at \c p_buf and the content of data is dependent on message type.\n
 *
 * \ingroup  MPA
 */
typedef void (* P_PROTO_CB)(void *p_buf, T_PROTO_MSG msg);

/**
 * mpa.h
 *
 * \brief   Prototype of callback function to handle response of authenticaion on profile over RFCOMM channel.
 *
 * \ingroup  MPA
 */
typedef void (* P_RFC_AUTHEN_CB)(T_MPA_RFC_AUTHEN_RSP *p_rsp);

/**
 * mpa.h
 *
 * \brief    Register a callback function to handle L2CAP related messages for a specific PSM.
 *
 * \param[in]  psm          Protocol service multiplexer that the callback function is related.
 * \param[in]  callback     Callback function to handle L2CAP messages. The function
 *                          must have the prototype defined as \ref P_PROTO_CB.
 * \param[out] p_proto_id   Protocol ID assigned by MPA which will be used in \ref mpa_send_l2c_conn_req.
 *
 * \return    The result of callback function register.
 * \retval    true     The function was successfully registered.
 * \retval    false    The function was failed to register.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t proto_id;

   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_PROTO_REG_RSP:
           {
               T_MPA_L2C_PROTO_REG_RSP *p_rsp = (T_MPA_L2C_PROTO_REG_RSP *)p_buf;
               APP_PRINT_TRACE2("protocol register response: psm 0x%04x, cause 0x%04x\r\n", p_rsp->psm, p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
bool mpa_reg_l2c_proto(uint16_t psm, P_PROTO_CB callback, uint8_t *p_proto_id);

#if F_BT_ATT_OVER_BREDR
/**
 * mpa.h
 *
 * \brief    Setting the data path of the L2CAP channel using \ref PSM_ATT.
 *
 * \param[in]  data_path Data processing method. Should be use \ref MPA_ATT_PSM_DATA_PATH.
 *
 * \return    The result of callback function register.
 * \retval    true     The function was successfully configuration.
 * \retval    false    The function was failed to configure.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       mpa_set_att_psm_data_path(MPA_ATT_PSM_DATA_PATH_GATT);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
bool mpa_set_att_psm_data_path(uint8_t data_path);
#endif

/**
 * mpa.h
 *
 * \brief    Send a request to create a L2CAP connection. If the request was successfully sent,
 *           \ref L2C_CONN_RSP will be received in the callback function registed by
 *           \ref mpa_reg_l2c_proto to indicate whether the procedure was started successfully.
 *           If the procedure was started, \ref L2C_CONN_CMPL will be received later to tell the
 *           result of L2CAP channel establishment.
 *
 * \param[in]  psm              PSM of the L2CAP channel that will be established.
 * \param[in]  uuid             UUID of profile that is established over the L2CAP channel.
 * \param[in]  proto_id         Protocol ID assigned by MPA.
 * \param[in]  mtu_size         Prefered MTU size value of the L2CAP channel.
 * \param[in]  bd_addr          Bluetooth address of remote device.
 * \param[in]  mode             Channel mode to use. Should be the combination of \ref MPA_L2C_MODE.
 * \param[in]  flush_timeout    Flush timeout of flushable data on this channel, 0xFFFF for not flush.
 *
 * \return    void
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t proto_id;

   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_PROTO_REG_RSP:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               T_MPA_L2C_PROTO_REG_RSP *p_rsp = (T_MPA_L2C_PROTO_REG_RSP *)p_buf;

               APP_PRINT_TRACE2("protocol register response: psm 0x%04x, cause 0x%04x\r\n", p_rsp->psm, p_rsp->cause);

               if (!p_rsp->cause)
               {
                   mpa_send_l2c_conn_req(PSM_AVDTP, UUID_AVDTP, proto_id, 672, bd_addr, MPA_L2C_MODE_BASIC, 0xFFFF);
               }
           }
           break;

       case L2C_CONN_RSP:
           {
               T_MPA_L2C_CONN_RSP *p_rsp = (T_MPA_L2C_CONN_RSP *)p_buf;

               if (p_rsp->cause)
               {
                   APP_PRINT_ERROR1("send l2cap connection request fail, cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       case L2C_CONN_CMPL:
           {
               T_MPA_L2C_CONN_CMPL *p_info = (T_MPA_L2C_CONN_CMPL *)p_buf;

               if (!p_info->cause)
               {
                   APP_PRINT_TRACE1("l2cap channel connect successfully, cid 0x%04x", p_info->cid);
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_l2c_conn_req(uint16_t psm, uint16_t uuid, uint16_t proto_id,
						   uint16_t mtu_size, uint8_t *bd_addr, uint8_t mode, uint16_t flush_timeout);

/**
 * mpa.h
 *
 * \brief    Send a confirmation for a L2CAP connection request from remote device. If the
 *           confirmation was successfully sent with cause as \ref MPA_L2C_CONN_ACCEPT,
 *           \ref L2C_CONN_CMPL will also be received in the callback function registered by
 *           \ref mpa_reg_l2c_proto to tell the result of L2CAP channel establishment.
 *
 * \param[in]  cause            Confirmation cause for the connection request from remote device.
 * \param[in]  cid              Local channel ID assigned by Bluetooth stack.
 * \param[in]  mtu_size         Prefered MTU size value of the L2CAP channel.
 * \param[in]  mode             Channel mode to use. Should be the combination of \ref MPA_L2C_MODE.
 * \param[in]  flush_timeout    Flush timeout of flushable data on this channel, 0xFFFF for not flush.
 *
 * \return    void
 *
 * <b>Example usage</b>
   \code{.c}
   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_CONN_IND:
           {
               T_MPA_L2C_CONN_IND *p_ind = (T_MPA_L2C_CONN_IND *)p_buf;

               mpa_send_l2c_conn_cfm(MPA_L2C_CONN_ACCEPT, p_ind->cid, 672, MPA_L2C_MODE_BASIC, 0xFFFF);
           }
           break;

       case L2C_CONN_CMPL:
           {
               T_MPA_L2C_CONN_CMPL *p_info = (T_MPA_L2C_CONN_CMPL *)p_buf;

               if (!p_info->cause)
               {
                   APP_PRINT_TRACE1("l2cap channel connect successfully, cid 0x%04x", p_info->cid);
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_l2c_conn_cfm(T_MPA_L2C_CONN_CFM_CAUSE cause, uint16_t cid, uint16_t mtu_size,
						   uint8_t mode, uint16_t flush_timeout);

/**
 * mpa.h
 *
 * \brief    Send a request to disconnect a L2CAP connection. If the request was successfully sent,
 *           \ref L2C_DISCONN_RSP will be received in the callback function registed by
 *           \ref mpa_reg_l2c_proto to show the result of disconnection.
 *
 * \param[in]  cid              Local channel ID of the L2CAP channel to disconnect.
 *
 * \return    void
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t proto_id;

   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_PROTO_REG_RSP:
           {
               uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
               T_MPA_L2C_PROTO_REG_RSP *p_rsp = (T_MPA_L2C_PROTO_REG_RSP *)p_buf;

               APP_PRINT_TRACE2("protocol register response: psm 0x%04x, cause 0x%04x\r\n", p_rsp->psm, p_rsp->cause);

               if (!p_rsp->cause)
               {
                   mpa_send_l2c_conn_req(PSM_AVDTP, UUID_AVDTP, proto_id, 672, bd_addr, MPA_L2C_MODE_BASIC, 0xFFFF);
               }
           }
           break;

       case L2C_CONN_RSP:
           {
               T_MPA_L2C_CONN_RSP *p_rsp = (T_MPA_L2C_CONN_RSP *)p_buf;

               if (p_rsp->cause)
               {
                   APP_PRINT_ERROR1("send l2cap connection request fail, cause 0x%04x", p_rsp->cause);
               }
           }
           break;

       case L2C_CONN_CMPL:
           {
               T_MPA_L2C_CONN_CMPL *p_info = (T_MPA_L2C_CONN_CMPL *)p_buf;

               if (!p_info->cause)
               {
                   mpa_send_l2c_disconn_req(p_info->cid);
               }
           }
           break;

       case L2C_DISCONN_RSP:
           {
               T_MPA_L2C_DISCONN_RSP *p_rsp = (T_MPA_L2C_DISCONN_RSP *)p_buf;

               if (p_rsp->cause)
               {
                   APP_PRINT_TRACE1("l2cap disconnect response, cause 0x%04x", p_rsp->cause);
               }
           }

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_l2c_disconn_req(uint16_t cid);

/**
 * mpa.h
 *
 * \brief    Send a confirmation for a L2CAP disconnection request from remote device.
 *
 * \param[in]  cid              Local channel ID of the L2CAP channel.
 *
 * \return    void
 *
 * <b>Example usage</b>
   \code{.c}
   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_DISCONN_IND:
           {
               T_MPA_L2C_DISCONN_IND *p_ind = (T_MPA_L2C_DISCONN_IND *)p_buf;

               mpa_send_l2c_disconn_cfm(p_ind->cid);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_l2c_disconn_cfm(uint16_t cid);

/**
 * mpa.h
 *
 * \brief    Get buffer from Bluetooth stack to put in L2CAP data which will be sent to remote device.
 *
 * \param[in]  size         Length of L2CAP data.
 * \param[in]  offset       Offset of real L2CAP data in the buffer. Offset value can be get from \ref L2C_CONN_CMPL message.
 * \param[in]  ack          Whether need an ack when the buffer was released. If set to true, \ref L2C_DATA_RSP message will be
 *                          received in the callback function registered by \ref legacy_register_cb.
 *
 * \return    The address of the allocated buffer. If the address in NULL, the buffer was get failed.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t proto_id;
   uint8_t proto_data[30] = {0};

   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_CONN_CMPL:
           {
               T_MPA_L2C_CONN_CMPL *p_info = (T_MPA_L2C_CONN_CMPL *)p_buf;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = mpa_get_l2c_buf(proto_id, p_info->cid, 0, 30, p_info->ds_data_offset, false, 0);

                   if (p_buffer)
                   {
                       memcpy(p_buffer + p_info->ds_data_offset, proto_data, 30);
                       mpa_send_l2c_data_req(p_buffer, p_info->ds_data_offset, p_info->cid, 30, false);
                   }
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */

void *mpa_get_l2c_buf(uint8_t proto_id, uint16_t cid, uint8_t dlci, uint16_t size,
					  uint16_t offset, bool ack);

/**
 * mpa.h
 *
 * \brief    Send a request to send L2CAP data to remote device.
 *
 * \param[in]  p_buf        Address of L2CAP data buffer which is allocated by \ref mpa_get_l2c_buf.
 * \param[in]  offset       Offset of real L2CAP data in the buffer. Offset value can be get from \ref L2C_CONN_CMPL message.
 * \param[in]  cid          Local channel ID. The value can be get from \ref L2C_CONN_CMPL message.
 * \param[in]  length       Length of real L2CAP data.
 * \param[in]  auto_flush   Whether the L2CAP data can be flushed or not.
 *
 * \return    void.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t proto_id;
   uint8_t proto_data[30] = {0};

   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_CONN_CMPL:
           {
               T_MPA_L2C_CONN_CMPL *p_info = (T_MPA_L2C_CONN_CMPL *)p_buf;

               if (!p_info->cause)
               {
                   uint8_t *p_buffer = mpa_get_l2c_buf(proto_id, p_info->cid, 0, 30, p_info->ds_data_offset, false, 0);

                   if (p_buffer)
                   {
                       memcpy(p_buffer + p_info->ds_data_offset, proto_data, 30);
                       mpa_send_l2c_data_req(p_buffer, p_info->ds_data_offset, p_info->cid, 30, false);
                   }
               }
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_l2c_data_req(void *p_buf, uint16_t offset, uint16_t cid,
						   uint16_t length, bool auto_flush);

/**
 * mpa.h
 *
 * \brief    Send a request to register/deregister a protocol security entry over L2CAP.
 *           The security will be used when establishing a L2CAP channel. If the
 *           request was successfully sent, a message whose type is \ref L2C_SEC_REG_RSP
 *           and data is \ref T_MPA_L2C_SEC_REG_RSP will be received in the callback
 *           function registered by \ref mpa_reg_l2c_proto.
 *
 * \param  active             Register/Deregister the security entry.
 * \param  psm                PSM value for protocol.
 * \param  server_chann       Local server channel number for RFCOMM, for other protocols set to 0.
 * \param  uuid               UUID for profiles over rfcomm or UUID for other protocols except RFCOMM.
 * \param  requirement        Security requirement of the entry. Valid values are combinations of \ref MPA_SEC_SETTING.
 *
 * \return    None.
 *
 * <b>Example usage</b>
   \code{.c}
   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_PROTO_REG_RSP:
           {
               T_MPA_L2C_PROTO_REG_RSP *p_rsp = (T_MPA_L2C_PROTO_REG_RSP *)p_buf;
               APP_PRINT_TRACE2("protocol register response: psm 0x%04x, cause 0x%04x\r\n", p_rsp->psm, p_rsp->cause);
           }
           break;

       case L2C_SEC_REG_RSP:
           {
               T_MPA_L2C_SEC_REG_RSP *p_rsp = (T_MPA_L2C_SEC_REG_RSP *)p_buf;
               APP_PRINT_TRACE1("protocol security requirement register response: cause 0x%04x\r\n", p_rsp->cause);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);
       mpa_send_l2c_sec_reg_req(1, PSM_AVDTP, 0, UUID_AVDTP, MPA_SEC_BIT_AUTHEN | MPA_SEC_BIT_MITM);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_l2c_sec_reg_req(uint8_t active, uint16_t psm, uint16_t server_chann,
							  uint16_t uuid, uint8_t requirement);


/**
 * mpa.h
 *
 * \brief    Send a confirmation for authorization request indication. This function
 *           is used when receiving a message whose type is \ref L2C_PROTO_AUTHOR_IND
 *           and data is \ref T_MPA_AUTHOR_REQ_IND in the callback function registered
 *           by \ref mpa_reg_l2c_proto.
 *
 * \param[in]  bd_addr    Bluetooth address of remote device.
 * \param[in]  cause      Confirmation cause of authorization.
 *
 * \return    None.
 *
 * <b>Example usage</b>
   \code{.c}
   uint8_t proto_id;

   void test_l2c_cb(void *p_buf, T_PROTO_MSG msg)
   {
       switch (msg)
       {
       case L2C_PROTO_REG_RSP:
           {
               T_MPA_L2C_PROTO_REG_RSP *p_rsp = (T_MPA_L2C_PROTO_REG_RSP *)p_buf;
               APP_PRINT_TRACE2("protocol register response: psm 0x%04x, cause 0x%04x\r\n", p_rsp->psm, p_rsp->cause);
           }
           break;

       case L2C_PROTO_AUTHOR_IND:
           {
               T_MPA_AUTHOR_REQ_IND *p_ind = (T_MPA_AUTHOR_REQ_IND *)p_buf;
               mpa_send_author_cfm(p_ind->bd_addr, GAP_CFM_CAUSE_ACCEPT);
           }
           break;

       default:
           break;
       }
   }

   int test(void)
   {
       mpa_reg_l2c_proto(PSM_AVDTP, test_l2c_cb, &proto_id);

       return 0;
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_author_cfm(uint8_t *bd_addr, T_GAP_CFM_CAUSE result);

/**
 * mpa.h
 *
 * \brief    Register a callback function to handle authentication responses over RFCOMM.
 *           <b>This API can only be used when realize RFCOMM protocol.</b>
 *
 * \param  p_func            Callback function to handle authentication response. The function
 *                           must have the prototype as \ref P_RFC_AUTHEN_CB.
 *
 * \return    None.
 *
 * <b>Example usage</b>
   \code{.c}
   void rfc_handle_authen_rsp(T_MPA_RFC_AUTHEN_RSP *p_rsp)
   {
       APP_PRINT_TRACE1("RFCOMM authentication response: cause 0x%04x", p_rsp->cause);
   }

   void test(void)
   {
       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

       mpa_reg_rfc_authen_cb(rfc_handle_authen_rsp);

       // ...
       // HFP connected
       // ...

       mpa_send_rfc_authen_req(bd_addr, 0x40, 0x12, UUID_HANDSFREE, 0);
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_reg_rfc_authen_cb(P_RFC_AUTHEN_CB p_func);

/**
 * mpa.h
 *
 * \brief    Send a request to start authentication for a profile on RFCOMM channel.
 *           If the request was successfully sent, a response message will
 *           be received in the callback function registered by \ref mpa_reg_rfc_authen_cb.
 *           <b>This API can only be used when realize RFCOMM protocol.</b>
 *
 * \param  bd_addr            Bluetooth address of remote device.
 * \param  cid                L2CAP local channel ID.
 * \param  dlci               DLCI value for the RFCOMM channel.
 * \param  uuid               UUID for profiles over rfcomm.
 * \param  outgoing           Direction of the profile over rfcomm channel.
 *
 * \return    None.
 *
 * <b>Example usage</b>
   \code{.c}
   void rfc_handle_authen_rsp(T_MPA_RFC_AUTHEN_RSP *p_rsp)
   {
       APP_PRINT_TRACE1("RFCOMM authentication response: cause 0x%04x", p_rsp->cause);
   }

   void test(void)
   {
       uint8_t bd_addr[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

       mpa_reg_rfc_authen_cb(rfc_handle_authen_rsp);

       // ...
       // HFP connected
       // ...

       mpa_send_rfc_authen_req(bd_addr, 0x40, 0x12, UUID_HANDSFREE, 0);
   }
   \endcode
 *
 * \ingroup  MPA
 */
void mpa_send_rfc_authen_req(uint8_t *bd_addr, uint16_t cid, uint16_t dlci,
							 uint16_t uuid, uint8_t outgoing);

#if F_BT_DEINIT
void mpa_deinit(void);
#endif

#ifdef  __cplusplus
}
#endif

#endif

#endif  /*  __MPA_H__ */
