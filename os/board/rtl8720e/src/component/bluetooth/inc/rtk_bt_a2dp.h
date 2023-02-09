/**
 * @file      rtk_bt_a2dp.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth LE GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_A2DP_H__
#define __RTK_BT_A2DP_H__

#include <rtk_bt_common.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_BT_A2DP_MAX_CODEC_PARAM_LENGTH 12
#define RTK_BT_A2DP_MAX_STREAM_DATA_LENGTH 1024

/* sbc */
#define RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_16KHZ				(1<<7)
#define RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_32KHZ				(1<<6)
#define RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_44_1KHZ				(1<<5)
#define RTK_BT_A2DP_SBC_SAMPLING_FREQUENCY_48KHZ				(1<<4)
#define RTK_BT_A2DP_SBC_CHANNEL_MODE_MONO						(1<<3)
#define RTK_BT_A2DP_SBC_CHANNEL_MODE_DUAL_CHANNEL				(1<<2)
#define RTK_BT_A2DP_SBC_CHANNEL_MODE_STEREO						(1<<1)
#define RTK_BT_A2DP_SBC_CHANNEL_MODE_JOINT_STEREO				(1<<0)
#define RTK_BT_A2DP_SBC_BLOCK_LENGTH_4							(1<<7)
#define RTK_BT_A2DP_SBC_BLOCK_LENGTH_8							(1<<6)
#define RTK_BT_A2DP_SBC_BLOCK_LENGTH_12							(1<<5)
#define RTK_BT_A2DP_SBC_BLOCK_LENGTH_16							(1<<4)
#define RTK_BT_A2DP_SBC_SUBBANDS_4								(1<<3)
#define RTK_BT_A2DP_SBC_SUBBANDS_8								(1<<2)
#define RTK_BT_A2DP_SBC_ALLOCATION_METHOD_SNR					(1<<1)
#define RTK_BT_A2DP_SBC_ALLOCATION_METHOD_LOUDNESS				(1<<0)

/* aac */
#define RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_2_AAC_LC				(1<<7)
#define RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_LC				(1<<6)
#define RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_LTP				(1<<5)
#define RTK_BT_A2DP_AAC_OBJECT_TYPE_MPEG_4_AAC_SCALABLE			(1<<4)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_8KHZ					(1<<15)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_11_025KHZ			(1<<14)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_12KHZ				(1<<13)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_16KHZ				(1<<12)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_22_05KHZ				(1<<11)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_24KHZ				(1<<10)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_32KHZ				(1<<9)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_44_1KHZ				(1<<8)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_48KHZ				(1<<7)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_64KHZ				(1<<6)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_88_2KHZ				(1<<5)
#define RTK_BT_A2DP_AAC_SAMPLING_FREQUENCY_96KHZ				(1<<4)
#define RTK_BT_A2DP_AAC_CHANNEL_NUMBER_1						(1<<3)
#define RTK_BT_A2DP_AAC_CHANNEL_NUMBER_2						(1<<2)

/* ------------------------------- Data Types ------------------------------- */

/**
 * @typedef   rtk_bt_a2dp_role_t
 * @brief     a2dp role 
 */
typedef enum {
	RTK_BT_A2DP_ROLE_SNK = 0x00,					/*!< BT A2DP SINK */
	RTK_BT_A2DP_ROLE_SRC = 0x01,					/*!< BT A2DP SOURCE */
} rtk_bt_a2dp_role_t;

/**
 * @struct    rtk_bt_a2dp_media_codec_sbc_t
 * @brief     a2dp codec sbc configuration data structure.
 */
typedef struct
{
	uint8_t sampling_frequency_mask;				/*!< sample frequency */
	uint8_t channel_mode_mask;						/*!< channel mode */
	uint8_t block_length_mask;						/*!< block length */
	uint8_t subbands_mask;							/*!< subbands number */
	uint8_t allocation_method_mask;					/*!< allocation method */
	uint8_t min_bitpool;							/*!< min bitpool */
	uint8_t max_bitpool;							/*!< max bitpool */
} rtk_bt_a2dp_media_codec_sbc_t;

/**
 * @struct    rtk_bt_a2dp_media_codec_sbc_t
 * @brief     a2dp codec aac configuration data structure.
 */
typedef struct
{
	uint8_t  object_type_mask;						/*!< object type */
	uint16_t sampling_frequency_mask;				/*!< sample frequency */
	uint8_t  channel_number_mask;					/*!< channel number */
	bool     vbr_supported;							/*!< vbr */
	uint32_t bit_rate;								/*!< bit rate */
} rtk_bt_a2dp_media_codec_aac_t;

/**
 * @struct    rtk_bt_a2dp_conn_ind_t
 * @brief     a2dp connection connection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_a2dp_conn_ind_t;

/**
 * @struct    rtk_bt_a2dp_conn_ind_t
 * @brief     a2dp connection connection indication.
 */
typedef struct {
	uint8_t bd_addr[6];									/*!< address */
} rtk_bt_a2dp_disconn_ind_t;
/**
 * @struct    rtk_bt_a2dp_codec_param_t
 * @brief     Bluetooth A2DP stream data indication.
 */
typedef struct {
	uint8_t  codec_type;								/*!< codec type */
	uint32_t length;									/*!< codec param data length */
	uint8_t  data[RTK_BT_A2DP_MAX_CODEC_PARAM_LENGTH];	/*!< codec data */
} rtk_bt_a2dp_codec_param_t;

/**
 * @struct    rtk_bt_a2dp_codec_t
 * @brief     Bluetooth A2DP codec status.
 */
typedef struct {
	uint8_t codec_type;									/*!< indicate codec */
	union {
		struct {
			uint8_t 	sampling_frequency;				/*!< indicate sample frequence */
			uint8_t 	channel_mode;					/*!< indicate channel mode */
			uint8_t 	block_length;					/*!< indicate block length */
			uint8_t 	subbands;						/*!< indicate subbands */
			uint8_t 	allocation_method;				/*!< indicate allocation method */
			uint8_t 	min_bitpool;					/*!< indicate min bitpool */
			uint8_t 	max_bitpool;					/*!< indicate max bitpool */
		} sbc;
		struct {
			uint8_t 	object_type;					/*!< indicate object type */
			uint16_t 	sampling_frequency;				/*!< indicate sample frequence */
			uint8_t 	channel_number;					/*!< indicate channel number */
			bool 		vbr_supported;					/*!< indicate vbr_supported */
			uint32_t 	bit_rate;						/*!< indicate bit rate */
		} aac;
		struct {
			uint8_t 	info[12];						/*!< vendor codec data */
		} vendor;
	};
} rtk_bt_a2dp_codec_t;

/**
 * @struct    rtk_bt_a2dp_stream_start_t
 * @brief     Bluetooth A2DP stream start indication.
 */
typedef struct {
	uint8_t active_a2dp_link_index;						/*!< stream index */
	uint8_t stream_cfg;									/*!< stream cfg */
} rtk_bt_a2dp_stream_start_t;

/**
 * @struct    rtk_bt_a2dp_stream_data_send_t
 * @brief     Bluetooth A2DP stream start indication.
 */
typedef struct {
	uint8_t  bd_addr[6];								/*!< address */
	uint8_t  frame_num;									/*!< frame number */
	uint16_t len;										/*!< data length */
	uint8_t  *frame_buf;								/*!< frame buffer */
} rtk_bt_a2dp_stream_data_send_t;

/**
 * @struct    rtk_bt_a2dp_stream_data_ind_t
 * @brief     Bluetooth A2DP stream start indication.
 */
typedef struct {
	uint32_t length;									/*!< stream data length */
	uint8_t  data[RTK_BT_A2DP_MAX_STREAM_DATA_LENGTH];	/*!< stream data */
} rtk_bt_a2dp_stream_data_ind_t;

/**
 * @struct    rtk_bt_a2dp_stream_data_rsp_t
 * @brief     Bluetooth A2DP stream data rsp indication.
 */
typedef struct {
	uint8_t  bd_addr[6];								/*!< address */
} rtk_bt_a2dp_stream_data_rsp_t;

/**
 * @struct    rtk_bt_a2dp_sdp_attr_info_t
 * @brief     Bluetooth A2DP sdp attr information struct.
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
} rtk_bt_a2dp_sdp_attr_info_t;


/* ------------------------------ Functions Declaration ------------------------------ */
/**
 * @defgroup  bt_a2dp BT A2DP APIs
 * @brief     BT A2DP related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_a2dp_codec_cfg(uint8_t codec_type, void *codec_t, uint32_t length)
 * @brief     Set a2dp codec configuration.
 * @param[in] codec_type: codec index
 * @param[in] codec_t: codec struct pointer
 * @param[in] length: data length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_a2dp_codec_cfg(uint8_t codec_type, void *codec_t, uint32_t length);

/**
 * @fn        uint16_t rtk_bt_a2dp_connect(uint8_t *bd_addr)
 * @brief     send a2dp connect request.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_a2dp_connect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_a2dp_disconnect(uint8_t *bd_addr)
 * @brief     disconnect a2dp.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_a2dp_disconnect(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_a2dp_start(uint8_t *bd_addr)
 * @brief     start a2dp stream sending.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_a2dp_start(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_a2dp_suspend(uint8_t *bd_addr)
 * @brief     a2dp suspend.
 * @param[in] bd_addr: bt address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_a2dp_suspend(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_a2dp_data_send(rtk_bt_a2dp_stream_data_send_t *p_data_send_t)
 * @brief     a2dp data send.
 * @param[in] p_data_send_t: data send data struct pointer
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_a2dp_data_send(rtk_bt_a2dp_stream_data_send_t *p_data_send_t);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_A2DP_H__ */
