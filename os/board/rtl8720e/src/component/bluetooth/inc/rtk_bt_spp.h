/**
 * @file      rtk_bt_spp.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth SPP part type and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_SPP_H__
#define __RTK_BT_SPP_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* ------------------------------- Macros ------------------------------- */
/** @brief  Config SPP RTK_VENDOR_SPP server channel
 * @note  If not defined, disable RTK_VENDOR_SPP.If defined, enable RTK_VENDOR_SPP
*/
//#define									RTK_VENDOR_SPP 

/**
* @brief Define SPP maximum link numbers.
* @note  only one SPP server channel exists in the unique link based on RFCOMM.
*/
#define SPP_MAX_LINK_NUM						0x2

/**
* @brief Define SPP maximum service numbers.
* @note  SPP service num includes different server channels
*/
#define SPP_RFC_SPP_MAX_SERVICE_NUM				0x3 

/**
* @brief Define SPP default l2cap mtu size
* @note  SPP application may send tx data lower than SPP mtu size at a time
*/
#define SPP_DEMO_SPP_DEFAULT_MTU_SIZE			1012

/**
* @brief Define SPP default local tx window size
* @note  SPP application should send tx data when SPP link credit is not zero
*
*/
#define SPP_DEMO_SPP_DEFAULT_CREDITS			7

/**
* @brief Define SPP local server channel
* @note  SPP service num may be different in different SPP applications
*
*/
#define SPP_RFC_CHANN_NUM						0x3

/**
* @brief Define vendor SPP local server channel
* @note  SPP service num may be different in different SPP applications
*
*/
#define SPP_RFC_VENDOR_CHANN_NUM				0x4

/**
* @brief Define rtk_bt_spp_data_ind_t struct data buffer maximum length.
*/
#define RTK_BT_SPP_MAX_DATA_IND_LENGTH			512

/**
* @brief Define SPP max service class uuid length
*
*/
#define RTK_BT_SPP_MAX_SRV_CLASS_UUID_LENGTH	16


/* ------------------------------- Data Types ------------------------------- */
/**
 * @typedef   rtk_bt_spp_role_t
 * @brief     spp role 
 */
typedef enum {
	RTK_BT_SPP_ROLE_CLIENT = 0x00,			/*!< BT SPP client */
	RTK_BT_SPP_ROLE_SERVER = 0x01,			/*!< BT SPP server */
} rtk_bt_spp_role_t;

/**
 * @typedef   rtk_bt_spp_uuid_type_t
 * @brief     spp uuid type 
 */
typedef enum
{
	SPP_UUID16,								/*!< UUID type 16 bits */
	SPP_UUID32,								/*!< UUID type 32 bits */
	SPP_UUID128								/*!< UUID type 128 bits */
} rtk_bt_spp_uuid_type_t;

/**
 * @typedef   rtk_bt_spp_uuid_data_t
 * @brief     spp uuid data 
 */
typedef union
{
	uint16_t	uuid_16;					/*!< UUID data in 16 bits */
	uint32_t	uuid_32;					/*!< UUID data in 32 bits */
	uint8_t		uuid_128[16];				/*!< UUID data in 128 bits */
} rtk_bt_spp_uuid_data_t;

/**
 * @struct    rtk_bt_spp_sdp_attr_info_t
 * @brief     Bluetooth SPP SDP attribute information struct
 */
typedef struct 
{
	uint8_t						bd_addr[6];					/**< Address */
	rtk_bt_spp_uuid_type_t		srv_class_uuid_type;		/**< Service class UUID type */
	rtk_bt_spp_uuid_data_t		srv_class_uuid_data;		/**< Service class UUID data */
	uint16_t					profile_version; 			/**< Remote profile version */
	uint16_t					protocol_version;			/**< Remote protocol version */
	uint8_t						server_channel;				/**< Server channel number */
	uint16_t					supported_feat;				/**< Supported features */
	uint16_t					l2c_psm;					/**< L2CAP PSM value */
	uint8_t						supported_repos;			/**< Supported repositories */
	uint32_t					pbap_supported_feat;		/**< PBAP supported features */
} rtk_bt_spp_attr_info_t;

/**
 * @struct   rtk_bt_spp_srv_cfg_t
 * @brief    SPP service register configuration
 */
typedef struct 
{
	uint8_t local_server_chann;												/**< Server channel number */
	uint32_t length;														/**< Service class UUID data length*/
	uint8_t srv_class_uuid128[RTK_BT_SPP_MAX_SRV_CLASS_UUID_LENGTH];		/**< Service class UUID data */
} rtk_bt_spp_srv_cfg_t;

/**
 * @struct   rtk_bt_spp_sdp_discov_cmpl_t
 * @brief    SPP SDP discovery completely
 */
typedef struct 
{
	uint8_t bd_addr[6];					/**< Address */
	uint16_t cause;						/**< cause */
} rtk_bt_spp_sdp_discov_cmpl_t;

/**
 * @struct   rtk_bt_spp_conn_ind_t
 * @brief    SPP connection connect indication
 */
typedef struct 
{
	uint8_t bd_addr[6];					/**< Address */
	uint8_t local_server_chann;			/**< local server channel */
	uint16_t frame_size;				/**< SPP tx/rx mtu size */
} rtk_bt_spp_conn_ind_t;

/**
 * @struct   rtk_bt_spp_disconn_ind_t
 * @brief    SPP connection disconnect indication
 */
typedef struct 
{
	uint8_t bd_addr[6];					/*!< BT address */
	uint8_t local_server_chann;			/*!< local server channel */
	uint16_t cause;						/*!< cause */
} rtk_bt_spp_disconn_ind_t;

/**
 * @struct   rtk_bt_spp_conn_cmpl_t
 * @brief    SPP connection connect completed
 */
typedef struct 
{
	uint8_t bd_addr[6];					/*!< BT address */
	uint8_t local_server_chann;			/*!< local server channel */
	uint8_t  link_credit;				/*!< remained SPP tx link credit */
	uint16_t frame_size;				/*!< SPP tx/rx mtu size */
} rtk_bt_spp_conn_cmpl_t;

/**
 * @struct   rtk_bt_spp_disconn_cmpl_t
 * @brief    SPP connection disconnect completed
 */
typedef struct 
{
	uint8_t bd_addr[6];					/*!< BT address */
	uint8_t local_server_chann;			/*!< local server channel */
	uint16_t cause;						/*!< cause */
} rtk_bt_spp_disconn_cmpl_t;

/**
 * @struct   rtk_bt_spp_credit_rcvd_t
 * @brief    SPP link credits received
 */
typedef struct 
{
	uint8_t bd_addr[6];					/*!< BT address */
	uint8_t local_server_chann;			/*!< local server channel */
	uint8_t link_credit;				/*!< SPP link credit */
} rtk_bt_spp_credit_rcvd_t;

/**
 * @struct   rtk_bt_spp_data_ind_t
 * @brief    SPP data transfer indication
 */
typedef struct 
{
	uint8_t bd_addr[6];								/*!< BT address */
	uint8_t local_server_chann;						/*!< local server channel */
	uint8_t data[RTK_BT_SPP_MAX_DATA_IND_LENGTH];	/*!< data buffer */
	uint16_t len;									/*!< data buffer length */
} rtk_bt_spp_data_ind_t;

/**
 * @struct   rtk_bt_spp_data_rsp_t
 * @brief    SPP data response with ack flag set
 */
typedef struct 
{
	uint8_t bd_addr[6];					/*!< BT address */
	uint8_t local_server_chann;			/*!< local server channel */
} rtk_bt_spp_data_rsp_t;

/**
 * @struct    rtk_bt_spp_send_data_t
 * @brief     Bluetooth SPP send data on specific server channel
 */
typedef struct {
	uint8_t  bd_addr[6];				/*!< Remote BT address */
	uint8_t  local_server_chann;		/*!< local server channel */
	uint8_t  *data;						/*!< data buffer */
	uint16_t len;						/*!< data buffer length */
} rtk_bt_spp_send_data_t;

/**
 * @struct    rtk_bt_spp_disconn_req_t
 * @brief     Bluetooth SPP send disconnect request on specific server channel
 */
typedef struct {
	uint8_t  bd_addr[6];				/*!< Remote BT address */
	uint8_t  local_server_chann;		/*!< local server channel */
} rtk_bt_spp_disconn_req_t;

/**
 * @struct    rtk_bt_spp_credits_give_t
 * @brief     Bluetooth SPP give link credits to peer device
 */
typedef struct {
	uint8_t  bd_addr[6];				/*!< Remote BT address */
	uint8_t  local_server_chann;		/*!< local server channel */
	uint8_t  credits;					/*!< SPP tx credits that the local device gives to the remote device */
} rtk_bt_spp_credits_give_t;

/* ------------------------- Functions Declaration ------------------------- */
/**
 * @fn        uint16_t rtk_bt_service_register_cfg(void *spp_service_class_uuid, uint32_t length, uint8_t local_server_chann);
 * @brief     Config register spp uuid together with local server channel
 * @param[in] spp_service_class_uuid: the service uuid needs to be registered
 * @param[in] length: uuid length
 * @param[in] local_server_chann: local server channel
 *                    
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_service_register_cfg(void *spp_service_class_uuid, uint32_t length, uint8_t local_server_chann);

/**
 * @fn        uint16_t rtk_bt_spp_connect(uint8_t *bd_addr)
 * @brief     Send SPP connect request.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_spp_connect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_spp_disconnect(rtk_bt_spp_disconn_req_t *p_disconn_req_t)
 * @brief     Send an SPP disconnection request
 * @param[in] p_disconn_req_t: SPP disconnection request struct pointer
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_spp_disconnect(rtk_bt_spp_disconn_req_t *p_disconn_req_t);

/**
 * @fn        uint16_t rtk_bt_spp_disconnect_all(uint8_t *bd_addr);
 * @brief     Send a request to disconnect all SPP connection
 * @param[in] bd_addr: Remote bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_spp_disconnect_all(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_spp_data_send(rtk_bt_spp_send_data_t *p_send_data_t)
 * @brief     SPP send data.
 * @param[in] p_send_data_t: SPP send data struct pointer
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_spp_send_data(rtk_bt_spp_send_data_t *p_send_data_t);


/**
 * @fn        rtk_bt_spp_credits_give(rtk_bt_spp_credits_give_t *p_credits_give_t)
 * @brief     Give SPP credits to remote device
 * @param[in] p_credits_give_t: SPP link credits give struct pointer
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_spp_credits_give(rtk_bt_spp_credits_give_t *p_credits_give_t);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_SPP_H__ */