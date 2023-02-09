/*
*******************************************************************************
* Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/

#ifndef __RTK_BT_HFP_H__
#define __RTK_BT_HFP_H__

#include <rtk_bt_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_BT_RFC_HFP_CHANN_NUM               1
#define RTK_BT_RFC_HSP_CHANN_NUM               2
#define RTK_BT_RFC_HSP_AG_CHANN_NUM            22
#define RTK_BT_RFC_HFP_AG_CHANN_NUM            23
#define RTK_BT_HFP_MAX_SCO_DATA_LENGTH         1024
/* BT HFP AG Supported Local Feature */
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_3WAY                   (1 << 0)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_EC_NR                  (1 << 1)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_VOICE_RECOGNITION      (1 << 2)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_INBAND_RINGING         (1 << 3)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_VOICE_TAG              (1 << 4)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_REJECT_CALLS           (1 << 5)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_ENHANCED_CALL_STATUS   (1 << 6)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_ENHANCED_CALL_CONTROL  (1 << 7)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_EXTENED_ERROR_RESULT   (1 << 8)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_CODEC_NEGOTIATION      (1 << 9)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_HF_INDICATORS          (1 << 10)
#define RTK_BT_HFP_AG_LOCAL_CAPABILITY_ESCO_S4_T2_SUPPORTED   (1 << 11)
/* BT HFP HF Supported Local Feature */
#define RTK_BT_HFP_HF_LOCAL_EC_NR_FUNCTION               (1 << 0)
#define RTK_BT_HFP_HF_LOCAL_THREE_WAY_CALLING            (1 << 1)
#define RTK_BT_HFP_HF_LOCAL_CLI_PRESENTATION_CAPABILITY  (1 << 2)
#define RTK_BT_HFP_HF_LOCAL_VOICE_RECOGNITION_ACTIVATION (1 << 3)
#define RTK_BT_HFP_HF_LOCAL_REMOTE_VOLUME_CONTROL        (1 << 4)
#define RTK_BT_HFP_HF_LOCAL_ENHANCED_CALL_STATUS         (1 << 5)
#define RTK_BT_HFP_HF_LOCAL_ENHANCED_CALL_CONTROL        (1 << 6)
#define RTK_BT_HFP_HF_LOCAL_CODEC_NEGOTIATION            (1 << 7)
#define RTK_BT_HFP_HF_LOCAL_HF_INDICATORS                (1 << 8)
#define RTK_BT_HFP_HF_LOCAL_ESCO_S4_SETTINGS             (1 << 9)
/* ------------------------------- Data Types ------------------------------- */

/**
 * @typedef   rtk_bt_hfp_role_t
 * @brief     hfp role 
 */
typedef enum {
	RTK_BT_AUDIO_HFP_ROLE_AG = 0x00,					/*!< HFP Role Audio Gate */
	RTK_BT_AUDIO_HFP_ROLE_HF = 0x01,					/*!< HFP Role Hand Free */
} rtk_bt_hfp_role_t;

/**
 * @typedef   rtk_bt_hfp_call_status_t
 * @brief     hfp call status 
 */
typedef enum
{
	RTK_BT_HFP_CALL_IDLE							= 0x00,
	RTK_BT_HFP_CALL_INCOMING						= 0x01,
	RTK_BT_HFP_CALL_OUTGOING						= 0x02,
	RTK_BT_HFP_CALL_ACTIVE							= 0x03,
	RTK_BT_HFP_CALL_HELD							= 0x04,
	RTK_BT_HFP_CALL_ACTIVE_WITH_CALL_WAITING		= 0x05,
	RTK_BT_HFP_CALL_ACTIVE_WITH_CALL_HELD			= 0x06,
} rtk_bt_hfp_call_status_t;

/**
 * @typedef   rtk_bt_hfp_ag_call_status_t
 * @brief     hfp ag call status 
 */
typedef enum
{
	RTK_BT_HFP_AG_CALL_IDLE								= 0x00,
	RTK_BT_HFP_AG_CALL_INCOMING							= 0x01,
	RTK_BT_HFP_AG_CALL_OUTGOING							= 0x02,
	RTK_BT_HFP_AG_CALL_ACTIVE							= 0x03,
	RTK_BT_HFP_AG_CALL_ACTIVE_WITH_CALL_WAITING			= 0x04,
	RTK_BT_HFP_AG_CALL_ACTIVE_WITH_CALL_HOLD			= 0x05,
} rtk_bt_hfp_ag_call_status_t;

/**
 * @typedef   rtk_bt_cvsd_frame_duration
 * @brief     hfp call status 
 */
typedef enum
{
	RTK_BT_AUDIO_CVSD_FRAME_DURATION_3_75_MS		= 0x00,
	RTK_BT_AUDIO_CVSD_FRAME_DURATION_7_5_MS			= 0x01,
} rtk_bt_cvsd_frame_duration;

/**
 * @struct    rtk_bt_hfp_ag_conf_t
 * @brief     hfp ag configuration data structure.
 */
typedef struct
{
	uint8_t link_num				;				/*!< HFP HF/HSP AG maximum connected link number */
	uint8_t rfc_hfp_ag_chann_num;					/*!< RFCOMM channel num used for hfp */
	uint8_t rfc_hsp_ag_chann_num;					/*!< RFCOMM channel num used for hsp */
	int ag_supported_features;					/*!< HFP AG hf supported features \ref BT_HFP_AG_LOCAL_SUPPORTED_FEATURE */
} rtk_bt_hfp_ag_conf_t;

/**
 * @struct    rtk_bt_hfp_hf_conf_t
 * @brief     hfp hf configuration data structure.
 */
typedef struct
{
	uint8_t link_num				;				/*!< HFP/HSP maximum connected link number */
	uint8_t rfc_hfp_chann_num;						/*!< RFCOMM channel num used for hfp */
	uint8_t rfc_hsp_chann_num;						/*!< RFCOMM channel num used for hsp */
	int hf_supported_features;					/*!< HFP hf supported features \ref BT_HFP_HF_LOCAL_SUPPORTED_FEATURE */
} rtk_bt_hfp_hf_conf_t;

/**
 * @struct    rtk_bt_hfp_call_incoming_t
 * @brief     hfp calling data structure.
 */
typedef struct
{
	uint8_t bd_addr[6]				;				/*!< Remote BT address */
	const char call_num[20];						/*!< HFP AG incoming call number */
	uint8_t call_num_len;							/*!< HFP AG incoming call number length with the maximum of 20 including '\0' */
	uint8_t call_num_type;							/*!< HFP AG incoming call number type */
} rtk_bt_hfp_call_incoming_t;

/**
 * @struct    rtk_bt_hfp_sdp_attr_info_t
 * @brief     Bluetooth hfp sdp attr information struct.
 */
typedef struct {
	uint8_t					bd_addr[6];				/*!< address */
	br_gap_uuid_type_t		srv_class_uuid_type;	/*!< Service class UUID type */
	br_gap_uuid_data_t		srv_class_uuid_data;	/*!< Service class UUID data */
	uint16_t				profile_version;		/*!< Remote profile version */
	uint16_t				protocol_version;		/*!< Remote protocol version */
	uint8_t					server_channel;			/*!< Server channel number */
	uint16_t				supported_feat;			/*!< Supported features */
	uint16_t				l2c_psm;				/*!< L2CAP PSM value */
	uint8_t					supported_repos;		/*!< Supported repositories */
	uint32_t				pbap_supported_feat;	/*!< PBAP supported features */
} rtk_bt_hfp_sdp_attr_info_t;

/**
 * @struct    rtk_bt_hfp_conn_ind_t
 * @brief     hfp connection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_conn_ind_t;

/**
 * @struct    rtk_bt_hfp_disconn_ind_t
 * @brief     hfp disconnection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_disconn_ind_t;

/**
 * @struct    rtk_bt_hfp_call_status_ind_t
 * @brief     hfp call status indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
	uint8_t prev_status;								/*!< previous status */
	uint8_t curr_status;								/*!< curr status */
} rtk_bt_hfp_call_status_ind_t;

/**
 * @struct    rtk_bt_hfp_caller_id_ind_t
 * @brief     hfp caller number indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
	char    number[20];									/*!< number */
	uint8_t type;										/*!< number type */
} rtk_bt_hfp_caller_id_ind_t;

/**
 * @struct    rtk_bt_hfp_codec_t
 * @brief     Bluetooth HFP codec status.
 */
typedef struct {
	uint8_t codec_type;									/*!< indicate codec */
	union {
		struct {
			uint8_t 	channel_mode;					/*!< indicate channel mode */
			int 		sample_rate;					/*!< indicate sample frequence */
			uint8_t 	bitpool;						/*!< indicate min bitpool */
			uint8_t 	allocation_method;				/*!< indicate allocation method */
			uint8_t 	subbands;						/*!< indicate subbands */
			uint8_t 	block_length;					/*!< indicate block length */
		} msbc;
		struct {
			uint8_t 	channel_num;					/*!< indicate channel number */
			int 		sample_rate;					/*!< indicate sample frequence */
			uint8_t 	frame_duration;					/*!< indicate frame duration */
		} cvsd;
	};
} rtk_bt_hfp_codec_t;

/**
 * @struct    rtk_bt_hfp_sco_data_ind_t
 * @brief     Bluetooth hfp sco data receive indication.
 */
typedef struct {
	uint32_t length;									/*!< stream data length */
	uint8_t  data[RTK_BT_HFP_MAX_SCO_DATA_LENGTH];		/*!< stream data */
} rtk_bt_hfp_sco_data_ind_t;

/**
 * @struct    rtk_bt_hfp_sco_data_send_t
 * @brief     Bluetooth HFP sco data send.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< Remote BT address */
	uint8_t seq_num;									/*!< SCO data seq number */
	uint8_t len;										/*!< SCO buffer length */
	uint8_t *buf;										/*!< SCO buffer to send */
} rtk_bt_hfp_sco_data_send_t;

/**
 * @struct    rtk_bt_hfp_ag_indicators_status_req_t
 * @brief     Bluetooth hfp ag indicators status request.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_ag_indicators_status_req_t;

/**
 * @struct    rtk_bt_hfp_ag_curr_calls_list_query_t
 * @brief     Bluetooth hfp ag query current calls list.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_ag_curr_calls_list_query_t;

/**
 * @struct    rtk_bt_hfp_ag_dial_last_number_ind_t
 * @brief     Bluetooth hfp ag dial last number indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_ag_dial_last_number_ind_t;

/**
 * @struct    rtk_bt_hfp_ag_call_answer_req_ind_t
 * @brief     Bluetooth hfp ag call answer requset indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_ag_call_answer_req_ind_t;

/**
 * @struct    rtk_bt_hfp_ag_call_terminate_req_ind_t
 * @brief     Bluetooth hfp ag call terminate requset indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_hfp_ag_call_terminate_req_ind_t;

/* ------------------------------ Functions Declaration ------------------------------ */
/**
 * @defgroup  bt_a2dp BT A2DP APIs
 * @brief     BT A2DP related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_hfp_cfg(uint8_t role, void *p_conf_t)
 * @brief     hfp configuration.
 * @param[in] role: hfp role (RTK_BT_AUDIO_HFP_ROLE_AG/RTK_BT_AUDIO_HFP_ROLE_HF)
 * @param[in] p_conf_t: pointer of configuration data (rtk_bt_hfp_ag_conf_t/rtk_bt_hfp_hf_conf_t)
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_cfg(uint8_t role, void *p_conf_t);

/**
 * @fn        uint16_t rtk_bt_hfp_connect(uint8_t *bd_addr)
 * @brief     send hfp connect request.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_connect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hfp_disconnect(uint8_t *bd_addr)
 * @brief     disconnect hfp.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_disconnect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hfp_sco_connect(uint8_t *bd_addr)
 * @brief     start hfp sco connect.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_sco_connect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hfp_sco_disconnect(uint8_t *bd_addr)
 * @brief     disconnect hfp sco.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_sco_disconnect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hfp_call_incoming(uint8_t *bd_addr, const char *call_num, uint8_t call_num_len, uint8_t call_num_type)
 * @brief     incoming call.
 * @param[in] bd_addr: bt address
 * @param[in] call_num: call number
 * @param[in] call_num_len: call number length(MAX 20)
 * @param[in] call_num_type: call number type
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_call_incoming(uint8_t *bd_addr, const char *call_num, uint8_t call_num_len, uint8_t call_num_type);

/**
 * @fn        uint16_t rtk_bt_hfp_call_answer(uint8_t *bd_addr)
 * @brief     answer call.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_call_answer(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hfp_call_ternimate(uint8_t *bd_addr)
 * @brief     terminate call.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_call_ternimate(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_hfp_data_send(rtk_bt_hfp_sco_data_send_t *p_data_send_t)
 * @brief     hfp sco data send.
 * @param[in] p_data_send_t: send data struct pointer
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_hfp_data_send(rtk_bt_hfp_sco_data_send_t *p_data_send_t);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_HFP_H__ */
