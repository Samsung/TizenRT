/*
 *******************************************************************************
 * Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
 *******************************************************************************
 */

#ifndef __RTK_BT_LE_AUDIO_H__
#define __RTK_BT_LE_AUDIO_H__

#include <rtk_bt_def.h>
#include <basic_types.h>
#include <rtk_bt_le_gap.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)
#define RTK_BLE_AUDIO_DEFAULT_PHY_1M                  (1) /**< bit 0:LE PHY 1M used. */
#define RTK_BLE_AUDIO_DEFAULT_PHY_2M                  (2) /**< bit 1:LE PHY 2M used. */
#define RTK_BLE_AUDIO_DEFAULT_PHY_CODED               (4) /**< bit 2:LE Coded PHY used. */  

#define RTK_BT_LE_AUDIO_CODEC_ID_LEN                       (5)
#define RTK_BT_LE_AUDIO_BROADCAST_ID_LEN                   (3)
#define RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN                 (16)

#define RTK_BT_LE_BROADCAST_AUDIO_ANNOUNCEMENT_SRV_UUID    0x1852
#define RTK_BT_LE_BASIC_AUDIO_ANNOUNCEMENT_SRV_UUID        0x1851

#define RTK_BT_LE_GATT_UUID_AICS                           0x1843
#define RTK_BT_LE_GATT_UUID_VCS                            0x1844
#define RTK_BT_LE_GATT_UUID_VOCS                           0x1845
#define RTK_BT_LE_GATT_UUID_CSIS                           0x1846
#define RTK_BT_LE_GATT_UUID_MICS                           0x184D
#define RTK_BT_LE_GATT_UUID_ASCS                           0x184E
#define RTK_BT_LE_GATT_UUID_BASS                           0x184F

#define RTK_BT_LE_GATT_UUID_CAS             			   0x1853
#define RTK_BT_LE_GATT_UUID_TMAS                           0x1855

#define RTK_BT_LE_AUDIO_MEDIA_PLAYER_NAME_CHAR_UUID                 0x2B93
#define RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_OBJID_CHAR_UUID           0x2B94
#define RTK_BT_LE_AUDIO_MEDIA_PLAYER_ICON_URL_CHAR_UUID             0x2B95
#define RTK_BT_LE_AUDIO_TRACK_CHANGE_CHAR_UUID                      0x2B96
#define RTK_BT_LE_AUDIO_TRACK_TITLE_CHAR_UUID                       0x2B97
#define RTK_BT_LE_AUDIO_TRACK_DURATION_CHAR_UUID                    0x2B98
#define RTK_BT_LE_AUDIO_TRACK_POSITION_CHAR_UUID                    0x2B99
#define RTK_BT_LE_AUDIO_PLAYBACK_SPEED_CHAR_UUID                    0x2B9A
#define RTK_BT_LE_AUDIO_SEEKING_SPEED_CHAR_UUID                     0x2B9B
#define RTK_BT_LE_AUDIO_CUR_TRACK_SEG_OBJID_CHAR_UUID               0x2B9C
#define RTK_BT_LE_AUDIO_CUR_TRACK_OBJID_CHAR_UUID                   0x2B9D
#define RTK_BT_LE_AUDIO_NEXT_TRACK_OBJID_CHAR_UUID                  0x2B9E
#define RTK_BT_LE_AUDIO_PARENT_GRP_OBJID_CHAR_UUID                  0x2B9F
#define RTK_BT_LE_AUDIO_CUR_GRP_OBJID_CHAR_UUID                     0x2BA0
#define RTK_BT_LE_AUDIO_PLAYING_ORDER_CHAR_UUID                     0x2BA1
#define RTK_BT_LE_AUDIO_PLAYING_ORDER_SUP_CHAR_UUID                 0x2BA2
#define RTK_BT_LE_AUDIO_MEDIA_STATE_CHAR_UUID                       0x2BA3
#define RTK_BT_LE_AUDIO_MEDIA_CONTROL_POINT_CHAR_UUID               0x2BA4
#define RTK_BT_LE_AUDIO_MEDIA_CTPOINT_OPCODE_SUP_CHAR_UUID          0x2BA5
#define RTK_BT_LE_AUDIO_SEARCH_CONTROL_POINT_CHAR_UUID              0x2BA7
#define RTK_BT_LE_AUDIO_SEARCH_RESULT_OBJID_CHAR_UUID               0x2BA6
#define RTK_BT_LE_AUDIO_CONTENT_CONTROL_ID_CHAR_UUID                0x2BBA

#define RTK_BT_LE_AUDIO_CONTEXT_CONVERSATIONAL       0x0002
#define RTK_BT_LE_AUDIO_CONTEXT_MEDIA                0x0004

#define RTK_BT_LE_ADV_TARGETED_ANNOUNCEMENT           0x01
#define RTK_BT_LE_ADV_GENERAL_ANNOUNCEMENT            0x00

/*The Preferred_Audio_Contexts LTV structure is typically included
in the Metadata field of PAC records exposed by Unicast Servers and Broadcast Sinks. */
#define RTK_BT_LE_METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS 0x01

#define RTK_BT_LE_LC3_CODEC_ID                       0x06
/* Codec_Specific_Capabilities parameters*/
//Codec Specific Capability Types
#define RTK_BT_LE_CODEC_CAP_TYPE_SUPPORTED_SAMPLING_FREQUENCIES   0x01
#define RTK_BT_LE_CODEC_CAP_TYPE_SUPPORTED_FRAME_DURATIONS        0x02
#define RTK_BT_LE_CODEC_CAP_TYPE_AUDIO_CHANNEL_COUNTS             0x03
/*4 octets.
Octet 0-1: Minimum number of octets supported per codec frame
Octet 2-3: Maximum number of octets supported per codec frame*/
#define RTK_BT_LE_CODEC_CAP_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME 0x04
#define RTK_BT_LE_CODEC_CAP_TYPE_MAX_SUPPORTED_FRAMES_PER_SDU     0x05

//Config Codec operation
#define RTK_BT_LE_CODEC_CFG_TYPE_SAMPLING_FREQUENCY       0x01
#define RTK_BT_LE_CODEC_CFG_TYPE_FRAME_DURATION           0x02
#define RTK_BT_LE_CODEC_CFG_TYPE_AUDIO_CHANNEL_ALLOCATION 0x03
#define RTK_BT_LE_CODEC_CFG_TYPE_OCTET_PER_CODEC_FRAME    0x04
#define RTK_BT_LE_CODEC_CFG_TYPE_BLOCKS_PER_SDU           0x05

/*The Preferred_Audio_Contexts LTV structure is typically included
in the Metadata field of PAC records exposed by Unicast Servers and Broadcast Sinks. */
#define RTK_BT_LE_METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS 0x01

/*The Streaming_Audio_Contexts LTV structure is typically included in a Metadata parameter value
when initiating the Enable or Update Metadata ASE Control operations for unicast Audio Streams,
or in the Metadata parameter value included in a BASE structure for broadcast Audio Streams.*/
#define RTK_BT_LE_METADATA_TYPE_STREAMING_AUDIO_CONTEXTS 0x02
/*Title and/or summary of Audio Stream content: UTF-8 format. */
#define RTK_BT_LE_METADATA_TYPE_PROGRAM_INFO             0x03
/*3-byte, lower case language code as defined in ISO 639-3. */
#define RTK_BT_LE_METADATA_TYPE_LANGUAGE                 0x04
/*Array of CCID values. */
#define RTK_BT_LE_METADATA_TYPE_CCCD_LIST                0x05
/*Parental_Rating. */
#define RTK_BT_LE_METADATA_TYPE_PARENTAL_RATING          0x06
/*A UTF-8 formatted URL link used to present more information about Program_Info. */
#define RTK_BT_LE_METADATA_TYPE_PROGRAM_INFO_URI         0x07
/*Extended Metadata. */
#define RTK_BT_LE_METADATA_TYPE_EXTENDED                 0xFE
/*Vendor_Specific. */
#define RTK_BT_LE_METADATA_TYPE_VENDOR_SPECIFIC          0xFF

#define RTK_BT_LE_SAMPLING_FREQUENCY_8K              0x0001
#define RTK_BT_LE_SAMPLING_FREQUENCY_16K             0x0004
#define RTK_BT_LE_SAMPLING_FREQUENCY_24K             0x0010
#define RTK_BT_LE_SAMPLING_FREQUENCY_32K             0x0020
#define RTK_BT_LE_SAMPLING_FREQUENCY_44_1K           0x0040
#define RTK_BT_LE_SAMPLING_FREQUENCY_48K             0x0080

#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_8K              0x01
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_16K             0x03
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_24K             0x05
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_32K             0x06
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_44_1K           0x07
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_48K             0x08

#define RTK_BT_LE_FRAME_DURATION_7_5_MS_BIT           0x01
#define RTK_BT_LE_FRAME_DURATION_10_MS_BIT            0x02
#define RTK_BT_LE_FRAME_DURATION_PREFER_7_5_MS_BIT    0x10
#define RTK_BT_LE_FRAME_DURATION_PREFER_10_MS_BIT     0x20

#define RTK_BT_LE_FRAME_DURATION_CFG_7_5_MS              0x00
#define RTK_BT_LE_FRAME_DURATION_CFG_10_MS               0x01

#define RTK_BT_LE_CODEC_CFG_SAMPLING_FREQUENCY_EXIST       0x0001
#define RTK_BT_LE_CODEC_CFG_FRAME_DURATION_EXIST           0x0002
#define RTK_BT_LE_CODEC_CFG_AUDIO_CHANNEL_ALLOCATION_EXIST 0x0004
#define RTK_BT_LE_CODEC_CFG_OCTET_PER_CODEC_FRAME_EXIST    0x0008
#define RTK_BT_LE_CODEC_CFG_TYPE_BLOCKS_PER_SDU_EXIST      0x0010


#define RTK_BT_LE_AUDIO_CHANNEL_COUNTS_1             0x01
#define RTK_BT_LE_AUDIO_CHANNEL_COUNTS_2             0x02

#define RTK_BT_LE_AUDIO_LOCATION_MONO                0x00000000
#define RTK_BT_LE_AUDIO_LOCATION_FL                  0x00000001
#define RTK_BT_LE_AUDIO_LOCATION_FR                  0x00000002
#define RTK_BT_LE_AUDIO_LOCATION_FC                  0x00000004
#define RTK_BT_LE_AUDIO_LOCATION_LFE1                0x00000008
#define RTK_BT_LE_AUDIO_LOCATION_BL                  0x00000010
#define RTK_BT_LE_AUDIO_LOCATION_BR                  0x00000020
#define RTK_BT_LE_AUDIO_LOCATION_FLC                 0x00000040
#define RTK_BT_LE_AUDIO_LOCATION_FRC                 0x00000080
#define RTK_BT_LE_AUDIO_LOCATION_BC                  0x00000100
#define RTK_BT_LE_AUDIO_LOCATION_LFE2                0x00000200
#define RTK_BT_LE_AUDIO_LOCATION_SIL                 0x00000400
#define RTK_BT_LE_AUDIO_LOCATION_SIR                 0x00000800
#define RTK_BT_LE_AUDIO_LOCATION_TPFL                0x00001000
#define RTK_BT_LE_AUDIO_LOCATION_TPFR                0x00002000
#define RTK_BT_LE_AUDIO_LOCATION_TPFC                0x00004000
#define RTK_BT_LE_AUDIO_LOCATION_TPC                 0x00008000
#define RTK_BT_LE_AUDIO_LOCATION_TPBL                0x00010000
#define RTK_BT_LE_AUDIO_LOCATION_TPBR                0x00020000
#define RTK_BT_LE_AUDIO_LOCATION_TPSIL               0x00040000
#define RTK_BT_LE_AUDIO_LOCATION_TPSIR               0x00080000
#define RTK_BT_LE_AUDIO_LOCATION_TPBC                0x00100000
#define RTK_BT_LE_AUDIO_LOCATION_BTFC                0x00200000
#define RTK_BT_LE_AUDIO_LOCATION_BTFL                0x00400000
#define RTK_BT_LE_AUDIO_LOCATION_BTFR                0x00800000
#define RTK_BT_LE_AUDIO_LOCATION_FLW                 0x01000000
#define RTK_BT_LE_AUDIO_LOCATION_FRW                 0x02000000
#define RTK_BT_LE_AUDIO_LOCATION_LS                  0x04000000
#define RTK_BT_LE_AUDIO_LOCATION_RS                  0x08000000
#define RTK_BT_LE_AUDIO_LOCATION_RFU                 0xF0000000
#define RTK_BT_LE_AUDIO_LOCATION_MASK                0x0FFFFFFF

#define RTK_BT_LE_TMAS_CG_ROLE_SUP                0x01
#define RTK_BT_LE_TMAS_CT_ROLE_SUP                0x02
#define RTK_BT_LE_TMAS_UMS_ROLE_SUP               0x04
#define RTK_BT_LE_TMAS_UMR_ROLE_SUP               0x08
#define RTK_BT_LE_TMAS_BMS_ROLE_SUP               0x10
#define RTK_BT_LE_TMAS_BMR_ROLE_SUP               0x20
#define RTK_BT_LE_TMAS_ROLE_RFU                   0xC0

#define RTK_BT_LE_CSI_SIRK_LEN        (16)

/** Little Endian uint16 to array */
#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

#define RTK_BT_LE_AUDIO_MONO            0
#define RTK_BT_LE_AUDIO_LEFT            1
#define RTK_BT_LE_AUDIO_RIGHT           2
#define RTK_BT_LE_AUDIO_STEREO          3

#define RTK_BT_LE_CSI_RSI_LEN           6
#define RTK_BD_ADDR_LEN 	6
#define RTK_BT_GAP_DEVICE_NAME_LEN      (39+1)//!< Max length of device name, if device name length exceeds it, it will be truncated.

#define RTK_BT_LE_AUDIO_PA_ADV_SET_MAX_NUM 4
#define RTK_BT_LE_AUDIO_SYNC_HANDLE_MAX_NUM 4

#define RTK_BT_LE_AUDIO_CIG_MAX_NUM                      2
#define RTK_BT_LE_AUDIO_CIS_MAX_NUM                      4

#define RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM  2
#define RTK_BT_LE_AUDIO_BROADCASTER_SUBGROUP_MAX_NUM  2
#define RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM  4


#define RTK_BT_LE_AUDIO_RECEIVER_BIG_MAX_NUM  2
#define RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM  4

/**
 * @struct    rtk_bt_le_audio_role_t
 * @brief     Bluetooth LE AUDIO role.
 */
typedef enum {
    RTK_BT_LE_AUDIO_ROLE_UNKNOWN = 0,
    RTK_BT_LE_AUDIO_ROLE_UNI_SER,					/**< Unicast - Server role. */
	RTK_BT_LE_AUDIO_ROLE_UNI_CLI,					/**< Unicast - Client role. */
    RTK_BT_LE_AUDIO_ROLE_BRO_SINK,				    /**< Broadcast - Sink role. */
	RTK_BT_LE_AUDIO_ROLE_BRO_SOUR,				    /**< Broadcast - Source role. */
	RTK_BT_LE_AUDIO_ROLE_MAX,
}rtk_bt_le_audio_role_t;

/**
 * @typedef   rtk_bt_le_audio_profile_t
 * @brief     Bluetooth LE audio profile type bit mask
 */
typedef enum {
	RTK_BT_LE_AUDIO_PROFILE_ASCS    =  BIT0,
	RTK_BT_LE_AUDIO_PROFILE_BASS    =  BIT1,
	RTK_BT_LE_AUDIO_PROFILE_CSIS    =  BIT2,
	RTK_BT_LE_AUDIO_PROFILE_MCS     =  BIT3,
    RTK_BT_LE_AUDIO_PROFILE_OTS     =  BIT4,
	RTK_BT_LE_AUDIO_PROFILE_CCP     =  BIT5,
	RTK_BT_LE_AUDIO_PROFILE_VCS     =  BIT6,
	RTK_BT_LE_AUDIO_PROFILE_AICS    =  BIT7,
	RTK_BT_LE_AUDIO_PROFILE_VOCS    =  BIT8,
	RTK_BT_LE_AUDIO_PROFILE_MICS    =  BIT9,
	RTK_BT_LE_AUDIO_PROFILE_TMAS    =  BIT10
} rtk_bt_le_audio_profile_t;

#define RTK_BT_LE_AUDIO_PROFILE_ALL   0xffffffffUL

/**
 * @typedef   rtk_bt_le_audio_cap_param_t
 * @brief     Bluetooth LE audio CAP param
 */
typedef struct {
    uint8_t csis_size;
    uint8_t csis_rank;
	uint8_t csis_sirk[RTK_BT_LE_CSI_SIRK_LEN];
} rtk_bt_le_audio_cap_param_t;

/**
 * @typedef   rtk_bt_le_audio_pacs_param_t
 * @brief     Bluetooth LE audio PACS param
 */
typedef struct {
 	uint32_t sink_audio_location;
    uint32_t source_audio_location;
    uint16_t sink_available_contexts;
    uint16_t source_available_contexts;
	uint8_t *p_sink_pac_id;
	uint16_t pac_sink_codec_len;
	uint8_t *p_pac_sink_codec;
	uint8_t *p_source_pac_id;
	uint16_t pac_source_codec_len;
	uint8_t *p_pac_source_codec;
} rtk_bt_le_audio_pacs_param_t;

/**
 * @typedef   rtk_bt_le_audio_ascs_param_t
 * @brief     Bluetooth LE audio ASCS param
 */
typedef struct {
 	uint8_t sink_ase_num;
    uint8_t source_ase_num;
} rtk_bt_le_audio_ascs_param_t;

/**
 * @typedef   rtk_bt_le_audio_mcs_server_param_t
 * @brief     Bluetooth LE audio MCS client param
 */
typedef struct {
 	uint16_t mcs_char_msk;
    uint16_t mcs_attr_feature;
 	bool general_mcs;
 	uint16_t ots_char_msk;
    uint16_t ots_attr_feature;
    uint8_t *p_mcs_id;
} rtk_bt_le_audio_mcs_server_param_t;

/**
 * @typedef   rtk_bt_le_audio_vcp_micp_param_t
 * @brief     Bluetooth LE audio VCP and MICP param
 */
typedef struct {
    uint8_t vcp_aics_cfg;
    uint8_t vcp_vocs_cfg;
} rtk_bt_le_audio_vcp_micp_param_t;

/**
 * @typedef rtk_bt_le_audio_app_conf_t
 * @brief   structure of default LE audio configuration
 */
typedef struct {
	rtk_bt_le_audio_role_t   		role;                			/*!< Default le audio profile role */
	rtk_bt_le_audio_profile_t   	profile_support;      			/*!< app profile support @ref rtk_bt_le_audio_profile_t */
	rtk_bt_le_audio_pacs_param_t 	pacs_param;						/*!< Default pacs param */
	rtk_bt_le_audio_cap_param_t     cap_param;						/*!< Default cap param */
	rtk_bt_le_audio_ascs_param_t	ascs_param;						/*!< Default ascs param */
    rtk_bt_le_audio_mcs_server_param_t mcs_param;                   /*!< Default mcs client param */
    rtk_bt_le_audio_vcp_micp_param_t vcp_micp_param;                /*!< Default vcp and micp param */
} rtk_bt_le_audio_app_conf_t;


/**
 * @typedef rtk_bt_le_audio_codec_config_t
 * @brief   structure of LE audio codec config
 */
typedef struct {
    uint16_t    type_exist;
    uint8_t     frame_duration;
    uint8_t     sample_frequency;
    uint8_t     codec_frame_blocks_per_sdu;
    uint16_t    octets_per_codec_frame;
    uint32_t    audio_channel_allocation;
    uint32_t    presentation_delay;
} rtk_bt_le_audio_cfg_codec_t;

/**
 * @typedef rtk_bt_le_audio_codec_cfg_ind_t
 * @brief   structure of LE audio codec config indication
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     ase_id;
    rtk_bt_le_audio_cfg_codec_t codec_cfg;          /**< @ref rtk_bt_le_audio_cfg_codec_t*/       
} rtk_bt_le_audio_codec_cfg_ind_t;


/**
 * @typedef rtk_bt_le_audio_cfg_codec_for_lc3_t
 * @brief   structure of LE audio codec config for LC3 codec
 */
typedef struct {
	uint8_t  frame_duration;
	uint8_t  sample_frequency;
	uint8_t  codec_frame_blocks_per_sdu;
	uint16_t octets_per_codec_frame;
	uint32_t audio_channel_allocation;
	int      compress_bytes;
} rtk_bt_le_audio_cfg_codec_for_lc3_t;

/**
 * @typedef rtk_bt_le_audio_cfg_codec_param_t
 * @brief   structure of LE audio codec param
 */
typedef struct {
    uint16_t conn_handle;
	rtk_bt_le_audio_cfg_codec_for_lc3_t codec_cfg_for_lc3; /**< @ref rtk_bt_le_audio_cfg_codec_for_lc3_t*/ 
} rtk_bt_le_audio_cfg_codec_param_t;

/**
 * @struct    rtk_bt_le_audio_qos_config_t
 * @brief     structure of LE audio qos config
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     ase_id;
    uint8_t     cig_id;
    uint8_t     cis_id;    
    uint8_t     framing;
    uint8_t     phy;
    uint8_t     retransmission_number;
    uint16_t    max_sdu;    
    uint16_t    max_transport_latency;
    uint32_t    sdu_interval;
    uint32_t    presentation_delay;
} rtk_bt_le_audio_qos_cfg_ind_t;

/**
 * @struct    rtk_bt_le_audio_enable_ind_t
 * @brief     structure of LE audio enable
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     ase_id;
} rtk_bt_le_audio_enable_ind_t;

typedef rtk_bt_le_audio_enable_ind_t rtk_bt_le_audio_disable_ind_t;

typedef rtk_bt_le_audio_enable_ind_t rtk_bt_le_audio_release_ind_t;

/**
 * @struct    rtk_bt_le_audio_iso_data_send_info_t
 * @brief     Bluetooth LE audio mgr data send info definition .
 */
typedef struct
{
    uint16_t conn_handle;           /*!< connection handle of the CIS or BIS*/
    bool ts_flag;                   /*!< indicates whether contains time_stamp, true: contain time_stamp; false: not contain time_stamp */
    uint32_t time_stamp;            /*!< a time in microseconds. time_stamp is used when @ref ts_flag is True */
    uint16_t pkt_seq_num;           /*!< sequence number of the SDU */
    uint32_t sdu_interval;          /*!< interval of the SDU */
    uint16_t data_len;              /*!< length of the SDU to be sent */
    uint8_t *p_data;                /*!< point to data to be sent */
} rtk_bt_le_audio_iso_data_send_info_t;

/**
 * @typedef   rtk_bt_le_iso_isoch_data_pkt_status_t
 * @brief     Packet Status Flag in Bluetooth LE HCI ISO Datat packet sent by the Controller
*/
typedef enum
{
    RTK_BT_LE_ISO_ISOCH_DATA_PKT_STATUS_VALID_DATA = 0,             /**< Valid data. The complete SDU was received correctly. */
    RTK_BT_LE_ISO_ISOCH_DATA_PKT_STATUS_POSSIBLE_ERROR_DATA = 1,    /**< Possibly invalid data. The contents of the SDU may
                                                                        contain errors or part of the SDU may be missing.
                                                                        This is reported as "data with possible errors". */
    RTK_BT_LE_ISO_ISOCH_DATA_PKT_STATUS_LOST_DATA = 2,              /**< Part(s) of the SDU were not received correctly.
                                                                        This is reported as "lost data". */
} rtk_bt_le_audio_isoch_data_pkt_status_t;

/**
 * @struct    rtk_bt_le_iso_direct_iso_data_ind_t
 * @brief     Indication of ISO Datat packet with cb_type @ref RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND .
 */
typedef struct
{
    uint16_t conn_handle;                                       /**< Connection handle of the unicast or boardcast. */
    rtk_bt_le_audio_isoch_data_pkt_status_t pkt_status_flag;    /**< @ref rtk_bt_le_audio_isoch_data_pkt_status_t. */
    uint16_t  offset;                                           /**< Offset from start of the SDU to @ref p_buf.
                                                                    e.g. p_buf + offset indicates the start of the SDU. */
    uint16_t  iso_sdu_len;                                      /**< Length of the SDU. */
    uint16_t  pkt_seq_num;                                      /**< The sequence number of the SDU. */
    bool      ts_flag;                                          /**< Indicates whether contains time_stamp.
                                                                    True: contain time_stamp.
                                                                    False: not contain time_stamp. */
    uint32_t  time_stamp;                                       /**< A time in microseconds. time_stamp is valid when @ref ts_flag is True. */
    uint16_t  buf_len;                                          /**< indicate buffer length. */
    uint8_t   *p_buf;                                           /**< Point the buffer that needs to release. */    
} rtk_bt_le_audio_direct_iso_data_ind_t;

/**
 * @struct    rtk_bt_le_audio_csis_gen_rsi_info_t
 * @brief     Bluetooth LE audio info for csis gen for rsi.
 */
typedef struct
{
    uint8_t sirk[RTK_BT_LE_CSI_RSI_LEN];        /**< new csis sirk. */ 
	uint8_t *p_rsik;                            /**< returned csis rsik. */ 
} rtk_bt_le_audio_csis_gen_rsi_info_t;


typedef enum
{
    RTK_BT_LE_CSIS_SIRK_ENC = 0x00,
    RTK_BT_LE_CSIS_SIRK_PLN = 0x01,
}rtk_bt_le_audio_csis_sirk_type_t;

/**
 * @struct    rtk_bt_le_audio_csis_change_sirk_info_t
 * @brief     Bluetooth LE audio info for csis change sirk.
 */
typedef struct
{
    rtk_bt_le_audio_csis_sirk_type_t sirk_type;        /**< csis sirk type. @ref rtk_bt_le_audio_csis_sirk_type_t. */ 
	uint8_t sirk[RTK_BT_LE_CSI_RSI_LEN];               /**< new csis sirk. */ 
} rtk_bt_le_audio_csis_change_sirk_info_t;

#define RTK_BT_LE_AUDIO_MCS_MPN_CCCD_FLAG                       0x0001
#define RTK_BT_LE_AUDIO_MCS_TK_CHG_CCCD_FLAG                    0x0002
#define RTK_BT_LE_AUDIO_MCS_TK_TITLE_CCCD_FLAG                  0x0004
#define RTK_BT_LE_AUDIO_MCS_TK_DUR_CCCD_FLAG                    0x0008
#define RTK_BT_LE_AUDIO_MCS_TK_POS_CCCD_FLAG                    0x0010
#define RTK_BT_LE_AUDIO_MCS_PY_SPEED_CCCD_FLAG                  0x0020
#define RTK_BT_LE_AUDIO_MCS_SK_SPEED_CCCD_FLAG                  0x0040
#define RTK_BT_LE_AUDIO_MCS_CUR_TK_OBJID_CCCD_FLAG              0x0080
#define RTK_BT_LE_AUDIO_MCS_NEXT_TK_OBJID_CCCD_FLAG             0x0100
#define RTK_BT_LE_AUDIO_MCS_PARENT_GRP_OBJID_CCCD_FLAG          0x0200
#define RTK_BT_LE_AUDIO_MCS_CUR_GRP_OBJID_CCCD_FLAG             0x0400
#define RTK_BT_LE_AUDIO_MCS_PY_ORDER_CCCD_FLAG                  0x0800
#define RTK_BT_LE_AUDIO_MCS_MEDIA_ST_CCCD_FLAG                  0x1000
#define RTK_BT_LE_AUDIO_MCS_MEDIA_CTL_POINT_CCCD_FLAG           0x2000
#define RTK_BT_LE_AUDIO_MCS_MEDIA_CTLPOINT_OPCODE_SUP_CCCD_FLAG 0x4000
#define RTK_BT_LE_AUDIO_MCS_SEARCH_CTL_POINT_CCCD_FLAG          0x8000
#define RTK_BT_LE_AUDIO_MCS_SEARCH_RES_OBJID_CCCD_FLAG          0x10000

#define RTK_BT_LE_AUDIO_MCS_PLAY_CONTROL_OPCODE                   0x01
#define RTK_BT_LE_AUDIO_MCS_PAUSE_CONTROL_OPCODE                  0x02
#define RTK_BT_LE_AUDIO_MCS_FAST_REWIND_CONTROL_OPCODE            0x03
#define RTK_BT_LE_AUDIO_MCS_FAST_FORWARD_CONTROL_OPCODE           0x04
#define RTK_BT_LE_AUDIO_MCS_STOP_CONTROL_OPCODE                   0x05
#define RTK_BT_LE_AUDIO_MCS_MOVE_RELATIVE_CONTROL_OPCODE          0x10
#define RTK_BT_LE_AUDIO_MCS_PREVIOUS_SEGM_CONTROL_OPCODE          0x20
#define RTK_BT_LE_AUDIO_MCS_NEXT_SEGM_CONTROL_OPCODE              0x21
#define RTK_BT_LE_AUDIO_MCS_FIRST_SEGM_CONTROL_OPCODE             0x22
#define RTK_BT_LE_AUDIO_MCS_LAST_SEGM_CONTROL_OPCODE              0x23
#define RTK_BT_LE_AUDIO_MCS_GOTO_SEGM_CONTROL_OPCODE              0x24
#define RTK_BT_LE_AUDIO_MCS_PREVOUS_TRACK_CONTROL_OPCODE          0x30
#define RTK_BT_LE_AUDIO_MCS_NEXT_TRACK_CONTROL_OPCODE             0x31
#define RTK_BT_LE_AUDIO_MCS_FIRST_TRACK_CONTROL_OPCODE            0x32
#define RTK_BT_LE_AUDIO_MCS_LAST_TRACK_CONTROL_OPCODE             0x33
#define RTK_BT_LE_AUDIO_MCS_GOTO_TRACK_CONTROL_OPCODE             0x34
#define RTK_BT_LE_AUDIO_MCS_PREVIOUS_GRP_CONTROL_OPCODE           0x40
#define RTK_BT_LE_AUDIO_MCS_NEXT_GRP_CONTROL_OPCODE               0x41
#define RTK_BT_LE_AUDIO_MCS_FIRST_GRP_CONTROL_OPCODE              0x42
#define RTK_BT_LE_AUDIO_MCS_LAST_GRP_CONTROL_OPCODE               0x43
#define RTK_BT_LE_AUDIO_MCS_GOTO_GRP_CONTROL_OPCODE               0x44

/**
 * @struct    rtk_bt_le_audio_mcs_media_state_t
 * @brief     Bluetooth LE audio mcs media state.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_MCS_MEDIA_INACTIVE_STATE = 0x00,
    RTK_BT_LE_AUDIO_MCS_MEDIA_PLAYING_STATE,
    RTK_BT_LE_AUDIO_MCS_MEDIA_PAUSED_STATE,
    RTK_BT_LE_AUDIO_MCS_MEDIA_SEEKING_STATE
} rtk_bt_le_audio_mcs_media_state_t;

/**
 * @struct    rtk_bt_le_audio_mcs_client_send_mcp_op_info_t
 * @brief     Bluetooth LE audio info for mcs client send mcp op.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< connection handle. */                         
	uint8_t     srv_idx;                                /**< Service Index. */
    bool        general_mcs;                            /**< is generic media control service (GMCS)*/
    uint8_t     opcode;  
    int32_t     op_param;           
} rtk_bt_le_audio_mcs_client_send_mcp_op_info_t;

/**
 * @struct    rtk_bt_le_audio_mcs_client_read_char_value_info_t
 * @brief     Bluetooth LE audio info for mcs client read char value.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< Connection handle. */ 
	uint8_t     srv_idx;                                /**< Service Index. */
    bool        general_mcs;                            /**< is generic media control service (GMCS)*/
    uint16_t    uuid;                                   /**< uuid. */
} rtk_bt_le_audio_mcs_client_read_char_value_info_t;

/**
 * @struct    rtk_bt_le_audio_mcs_client_cfg_cccd_info_t
 * @brief     Bluetooth LE audio info for mcs client config cccd.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< Connection handle. */  
	uint8_t     srv_idx;                                /**< Service Index. */
    bool        general_mcs;                            /**< is generic media control service (GMCS)*//**< is generic media control service (GMCS)*/
    uint32_t    cfg_flags;
    bool        enable;            
} rtk_bt_le_audio_mcs_client_cfg_cccd_info_t;

/**
 * @struct    rtk_bt_le_audio_mcs_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio mcs client discovery done indication.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< Connection handle. */ 
    bool        general_mcs;                            /**< Is generic media control service (GMCS)*/
    bool        is_found;
    uint8_t     srv_num;           
} rtk_bt_le_audio_mcs_client_discovery_done_ind_t;

/**
 * @struct    rtk_bt_le_audio_mcs_client_read_result_ind_t
 * @brief     Bluetooth LE audio mcs client read result indication.
 */
typedef struct {
    uint16_t    conn_handle;                            /**< Connection handle. */ 
    bool        general_mcs;                            /**< Is generic media control service (GMCS)*/					
    uint8_t     srv_idx;                                /**< Service Index. */
    uint16_t    uuid;
    uint16_t    cause;
    union {
		struct media_player_name_read_param {
            uint8_t  *player_name;     //UTF-8 string
            uint16_t  name_len;                 
		} media_player_name;

		struct media_player_icon_url_read_param {
            uint8_t  *p_icon_url;     //UTF-8 string
            uint16_t  url_len;               
		} media_player_icon_url;

		struct track_title_read_param {
            uint8_t  *title_name;     //UTF-8 string
            uint16_t  title_len;               
		} track_title_url;	

        int32_t           track_duration;
        int               track_position;
        int8_t            playback_speed;
        int8_t            seeking_speed;
        uint8_t           playing_order;
        uint8_t           media_state;
        uint64_t          read_obj_id;
        uint64_t          search_res_obj_id;
    }data;
} rtk_bt_le_audio_mcs_client_read_result_ind_t;

/**
 * @struct    rtk_bt_le_audio_mcs_client_notify_result_ind_t
 * @brief     Bluetooth LE audio mcs client notify result indication.
 */
typedef struct {
    uint16_t    conn_handle;                            /**< Connection handle. */ 
    bool        general_mcs;                            /**< Is generic media control service (GMCS)*/					
    uint8_t     srv_idx;                                /**< Service Index. */
    uint16_t    uuid;
    union {
		struct media_player_name_notify_param {
            uint8_t  *player_name;     //UTF-8 string
            uint16_t  name_len;                 
		} media_player_name;

		struct media_player_icon_url_notify_param {
            uint8_t  *p_icon_url;     //UTF-8 string
            uint16_t  url_len;               
		} media_player_icon_url;

		struct track_title_notify_param {
            uint8_t  *title_name;     //UTF-8 string
            uint16_t  title_len;               
		} track_title_url;	

        int32_t           track_duration;
        int               track_position;
        int8_t            playback_speed;
        int8_t            seeking_speed;
        uint8_t           playing_order;
        uint8_t           media_state;

        uint64_t          cur_tk_obj_id;
        uint64_t          next_tk_obj_id;
        uint64_t          cur_gp_obj_id;
        uint64_t          paraent_gp_obj_id;
        uint64_t          search_res_obj_id;
    }data;
} rtk_bt_le_audio_mcs_client_notify_result_ind_t;

/**
 * @struct    rtk_bt_le_audio_mcs_media_player_info_t
 * @brief     Bluetooth LE audio mcs server media player info.
 */
typedef struct
{
    uint8_t   *p_player_name;
    uint16_t  player_name_len;
    uint8_t   *p_icon_fmt;
    uint16_t  icon_fmt_len;     
    uint8_t   *p_url;
    uint16_t  url_len;
    uint16_t  play_order_sup;
} rtk_bt_le_audio_mcs_media_player_info_t;

/**
 * @struct    rtk_bt_le_audio_mcs_media_player_info_set_t
 * @brief     Bluetooth LE audio info for mcs server media player info set.
 */
typedef struct
{
    uint8_t service_id;       
	rtk_bt_le_audio_mcs_media_player_info_t media_player_info;      /**< media player info. @ref rtk_bt_le_audio_mcs_media_player_info_t. */  
} rtk_bt_le_audio_mcs_media_player_info_set_t;

/**
 * @struct    rtk_bt_le_audio_mcs_media_track_info_t
 * @brief     Bluetooth LE audio mcs server media track info.
 */
typedef struct
{
    uint8_t       *p_title;
    uint16_t      title_len;

    int           tk_duration;
    int           tk_pos;
    uint64_t      tk_obj_id;
    uint64_t      next_tk_obj_id;
} rtk_bt_le_audio_mcs_media_track_info_t;

/**
 * @struct    rtk_bt_le_audio_mcs_media_track_info_set_t
 * @brief     Bluetooth LE audio info for mcs server media track info set.
 */
typedef struct
{
    uint8_t service_id;       
	rtk_bt_le_audio_mcs_media_track_info_t media_track_info;      /**< media track info. @ref rtk_bt_le_audio_mcs_media_track_info_t. */  
} rtk_bt_le_audio_mcs_media_track_info_set_t;

/**
 * @struct    rtk_bt_le_audio_mcs_media_state_update_t
 * @brief     Bluetooth LE audio info for mcs media state update.
 */
typedef struct
{
    uint8_t service_id;       
	rtk_bt_le_audio_mcs_media_state_t media_state;                        /**< media state. @ref rtk_bt_le_audio_mcs_media_state_t. */  
} rtk_bt_le_audio_mcs_media_state_update_t;

/**
 * @struct    rtk_bt_le_audio_mcs_set_mcp_sup_opcode_t
 * @brief     Bluetooth LE audio info for mcs set mcp sup opcode.
 */
typedef struct
{
    uint8_t service_id;       
	uint32_t mcp_sup_opcode;                        /**< mcp sup opcode. */  
} rtk_bt_le_audio_mcs_set_mcp_sup_opcode_t;

/**
 * @struct    rtk_bt_le_audio_bis_setup_data_path_t
 * @brief     Bluetooth LE audio bis setup data path parameter.
 */
typedef struct
{  
    uint8_t bis_idx;                                        /**< bis index */
    uint8_t codec_id[RTK_BT_LE_AUDIO_CODEC_ID_LEN];         /*!< Specify the coding format used over the air. 
                                                                Octet 0: See Assigned Numbers for Coding Format.
                                                                Octets 1 to 2: Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if octet 0 is not 0xFF.
                                                                Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF. */
    uint32_t controller_delay;                              /*!< Controller delay in microseconds.
                                                                Range: 0x000000 to 0x3D0900 
                                                                Time range: 0 s to 4 s */
    uint8_t codec_config_len;                               /*!< Length of codec configuration */
    uint8_t *p_codec_config;                                  /*!< Specify codec-specific configuration information for the specified direction*/
} rtk_bt_le_audio_bis_setup_data_path_t;


#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)

/**
 * @struct    rtk_bt_le_audio_sync_create_param_t
 * @brief     Bluetooth LE audio paramsters for sync create.
 */
typedef struct
{
    uint8_t adv_sid;                            /**< If Periodic Advertiser List is not used (@ref rtk_bt_le_pa_sync_create_opt_t),
                                                    Advertising SID subfield in the ADI field used to identify the Periodic Advertising. */
    rtk_bt_le_addr_t adv_addr;					/**< If Periodic Advertiser List is not used
                                                     (@ref rtk_bt_le_pa_sync_create_opt_t),
                                                     only @ref RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC and
                                                     @ref RTK_BT_LE_ADDR_TYPE_RPA_RANDOM could be
                                                     used for creating sync. */
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];                                                        
	uint8_t * sync_handle_id;					/**< to store created sync handle id */
} rtk_bt_le_audio_sync_create_param_t;


/**
 * @struct    rtk_bt_le_audio_pa_sync_establish_t
 * @brief     Bluetooth LE audio paramsters for pa sync establish.
 */
typedef struct
{
    uint8_t options;                            /**< @ref rtk_bt_le_pa_sync_create_opt_t */
    uint8_t sync_cte_type;                      /**< @ref rtk_bt_le_pa_sync_cte_t */
    uint16_t skip;                              /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                                     Range: 0x0000 to 0x01F3 */
    uint16_t sync_timeout;                      /**< Synchronization timeout for the periodic advertising train.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
} rtk_bt_le_audio_pa_sync_establish_t;

/**
 * @struct    rtk_bt_le_audio_pa_sync_establish_param_t
 * @brief     Bluetooth LE audio paramsters for pa sync establish.
 */
typedef struct
{
    uint8_t sync_handle_id;                                 /**< Synchronization handle id*/
    rtk_bt_le_audio_pa_sync_establish_t establish_param;    /**< @ref rtk_bt_le_audio_pa_sync_establish_t*/
} rtk_bt_le_audio_pa_sync_establish_param_t;

/** @brief rtk_bt_le_audio_pa_action_t. */
typedef enum
{
    RTK_BT_LE_AUDIO_PA_IDLE         = 0x00,
    RTK_BT_LE_AUDIO_PA_SYNC         = 0x01,
    RTK_BT_LE_AUDIO_PA_TERMINATE    = 0x02,
    RTK_BT_LE_AUDIO_PA_LOST         = 0x03,
} rtk_bt_le_audio_pa_action_t;

/** @brief rtk_bt_le_audio_pa_sync_state_t. */
typedef enum
{
    RTK_BT_LE_AUDIO_PA_SYNC_STATE_TERMINATED = 0x00,                   /**< Terminated. */
    RTK_BT_LE_AUDIO_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE = 0x01,      /**< Start synchronizing when extended scanning is disabled. A temporary state, haven't received the result. */
    RTK_BT_LE_AUDIO_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING = 0x02,  /**< No attempt to synchronize will take place while extended scanning is disabled. */
    RTK_BT_LE_AUDIO_PA_SYNC_STATE_SYNCHRONIZING = 0x03,                /**< Start synchronizing when extended scanning is enabled. */
    RTK_BT_LE_AUDIO_PA_SYNC_STATE_SYNCHRONIZED = 0x04,                 /**< Synchronized. */
    RTK_BT_LE_AUDIO_PA_SYNC_STATE_TERMINATING = 0x05,                  /**< Terminate synchronization. A temporary state, haven't received the result. */
} rtk_bt_le_audio_pa_sync_state_t;

/**
 * @struct    rtk_bt_le_audio_pa_sync_state_ind_t
 * @brief     Bluetooth LE audio pa sync indication infomation.
 */
typedef struct
{    
    uint8_t cause;                   /**< process result */     
    uint8_t sync_handle_id;         /**< Synchronization handle id*/
    uint8_t sync_state;              /**< PA Synchronization state. @ref rtk_bt_le_audio_pa_sync_state_t*/
    uint16_t action;                 /**< PA Synchronization action. @ref rtk_bt_le_audio_pa_action_t. */             
} rtk_bt_le_audio_pa_sync_state_ind_t;


/**
 * @struct    rtk_bt_le_audio_bis_channel_location_t
 * @brief     Bluetooth LE audio bis and channel_location map.
 */
typedef struct
{
    uint8_t subgroup_idx;                 /**< Subgroup idx*/
    uint8_t bis_idx;                      /**< BIS index*/
    uint32_t audio_channel_allocation;      
}rtk_bt_le_audio_bis_channel_location_t;

/**
 * @struct    rtk_bt_le_audio_pa_adv_report_ind_t
 * @brief     Bluetooth LE audio pa adv report indication.
 */
typedef struct
{
    uint8_t sync_handle_id;                       /**< Synchronization handle id*/
    uint8_t num_subgroups;                        /**< The number of subgroups in the BIG. */
    uint8_t num_bis;                              /**< The number of BISes in the BIG. */        
    rtk_bt_le_audio_bis_channel_location_t  bis_channel_location[RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM];  /**< @ref rtk_bt_le_audio_bis_channel_location_t. */
	uint8_t sync_id;                              /**< Identify the synchronization with a periodic advertising train. */
	uint16_t sync_handle;                         /**< Identifying the periodic advertising train. */
	int8_t tx_power;                              /**< Range: -127 to +20, Units: dBm.
                                                       0x7F: Tx Power information not available. */
	int8_t rssi;                                  /**< Range: -127 to +20, Units: dBm.
                                                       0x7F: RSSI is not available. */
	uint8_t cte_type;                             /**< @ref rtk_bt_le_pa_sync_report_cte_t */
	uint8_t data_status;                          /**< @ref rtk_bt_le_pa_sync_report_data_status_t */
	uint8_t data_len;                             /**< Length of the Data field. Range: 0 to 247. */
	uint8_t *p_data;                              /**< Data received from a Periodic Advertising packet. Must be the last member. */
}rtk_bt_le_audio_pa_adv_report_ind_t;

/**
 * @struct    rtk_bt_le_audio_big_sync_state_t
 * @brief     Bluetooth LE audio paramsters for big sync.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_TERMINATED = 0x00,     /**< Terminated. */
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_SYNCHRONIZING = 0x01,  /**< Start synchronizing to a BIG. A temporary state, haven't received the result.*/
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_SYNCHRONIZED = 0x02,   /**< Synchronized to a BIG. */
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_TERMINATING = 0x03,    /**< Terminate synchronization to the BIG. A temporary state, haven't received the result.*/
} rtk_bt_le_audio_big_sync_state_t;

typedef enum
{
    RTK_BT_LE_AUDIO_BIG_IDLE         = 0x00,
    RTK_BT_LE_AUDIO_BIG_SYNC         = 0x01,
    RTK_BT_LE_AUDIO_BIG_TERMINATE    = 0x02,
    RTK_BT_LE_AUDIO_BIG_LOST         = 0x03,
} rtk_bt_le_audio_big_action_t;

/**
 * @struct    rtk_bt_le_audio_big_sync_state_ind_t
 * @brief     Bluetooth LE audio pa sync indication infomation.
 */
typedef struct
{  
    uint8_t cause;                   /**< process result */     
    uint8_t sync_handle_id;          /**< Synchronization handle id*/
    uint8_t sync_state;              /**< BIG Synchronization state. @ref rtk_bt_le_audio_big_sync_state_t*/
    uint8_t encryption;              /**< Identify the encryption mode of the BIG. 0x00: Unencrypted 0x01: Encrypted */
    uint16_t action;                 /**< BIG Synchronization action. @ref rtk_bt_le_audio_big_action_t. */           
} rtk_bt_le_audio_big_sync_state_ind_t;

/**
 * @struct    rtk_bt_le_audio_biginfo_t
 * @brief     Bluetooth LE audio big infomation.
 */
typedef struct
{
    uint8_t sync_id;         /**< Identify the periodic advertising train. */
    uint16_t  sync_handle;   /**< Sync_Handle identifying the periodic advertising train. */
    uint8_t num_bis;         /**< The number of BISes in the BIG. */
    uint8_t nse;             /**< The number of subevents per BIS in each BIG event. */
    uint16_t iso_interval;   /**< Value of the ISO_Interval subfield of the BIGInfo field.
                                  Time between two adjacent BIG anchor points, in units of 1.25 ms.
                                  The value shall be between 4 and 3200 (i.e. 5 ms to 4 s).*/
    uint8_t bn;              /**< Value of the BN subfield of the BIGInfo field.
                                  Burst Number contains the number of new payloads for each BIS in a BIS event. */
    uint8_t pto;             /**< Value of the PTO subfield of the BIGInfo field.
                                  Pre_Transmission_Offset contains the offset in number of ISO_Intervals for
                                  pre transmissions of data packets. */
    uint8_t irc;             /**< Value of the IRC subfield of the BIGInfo field.
                                  Immediate Repetition Count contains the number of times the scheduled
                                  data packet is transmitted. */
    uint16_t max_pdu;        /**< The maximum number of data octets (excluding the MIC, if any) that can be
                                  carried in each BIS Data PDU in the BIG. */
    uint32_t sdu_interval;   /**< Value of the SDU_Interval subfield of the BIGInfo field.
                                  The time interval, in microseconds, of the periodic SDUs. */
    uint16_t max_sdu;        /**< The maximum size, in octets, of an SDU. */
    rtk_bt_le_phy_type_t phy;     /**< Indicate the PHY used by the BIG. @ref rtk_bt_le_phy_type_t. */
    uint8_t framing;         /**< Indicate whether the BIG carries framed or unframed data.
                                  0x00: Unframed
                                  0x01: Framed */
    uint8_t encryption;      /**< Indicate whether the BIG is encrypted or not.
                                  0x00: BIG carries unencrypted data
                                  0x01: BIG carries encrypted data */
}rtk_bt_le_audio_biginfo_t;

/**
 * @struct    rtk_bt_le_audio_pa_biginfo_ind_t
 * @brief     Bluetooth LE audio biginfo adv report indication.
 */
typedef struct
{
    uint8_t sync_handle_id;                                 /**< Synchronization handle id*/
    rtk_bt_le_audio_biginfo_t biginfo;                      /**< @ref rtk_bt_le_audio_biginfo_t*/
}rtk_bt_le_audio_pa_biginfo_ind_t;


typedef struct
{
    uint8_t sync_handle_id;                                    /**< Synchronization handle id*/
    rtk_bt_le_audio_biginfo_t *p_big_info;                     /**< @ref rtk_bt_le_audio_biginfo_t*/
}rtk_bt_le_audio_sync_get_big_info_param_t;

/**
 * @typedef rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t
 * @brief   structure of LE audio  get bis codec config parameter
 */
typedef struct {
    uint8_t sync_handle_id;                         /**< Synchronization handle id*/
    uint8_t bis_idx;                                /**< bis index */
    rtk_bt_le_audio_cfg_codec_t *p_codec_cfg;       /**< @ref rtk_bt_le_audio_cfg_codec_t*/       
} rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t;

/**
 * @struct    rtk_bt_le_audio_big_sync_establish_t
 * @brief     Bluetooth LE audio paramsters for big sync establish.
 */
typedef struct
{                                                 
    uint8_t encryption;                    /**< Identify the encryption mode of the BIG.
                                                0x00: Unencrypted
                                                0x01: Encrypted */
    uint8_t broadcast_code[RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN]; /**< Used to generate the session key to decrypt payloads of an encrypted BIS. */
    uint8_t mse;                           /**< The maximum number of subevents that a Controller should use to receive data
                                                payloads in each interval for a BIS.
                                                0x00: The Controller can schedule reception of any number of subevents up
                                                      to NSE.
                                                0x01 to 0x1F: Maximum number of subevents that should be used to receive data
                                                              payloads in each BIS event      */
    uint16_t big_sync_timeout;             /**< Specify the maximum permitted time between successful receptions of BIS PDUs.
                                                Range: 0x000A to 0x4000
                                                Time = N*10 ms
                                                Time Range: 100 ms to 163.84 s */
    uint8_t  num_bis;                      /**< Total number of BISes to synchronize. */
    uint8_t  bis[RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM]; /**< A list of indices corresponding to BIS(es). */    
} rtk_bt_le_audio_big_sync_establish_t;

/**
 * @struct    rtk_bt_le_audio_sync_big_establish_param_t
 * @brief     Bluetooth LE audio paramsters for big sync establish.
 */
typedef struct
{                                                 
	uint8_t sync_handle_id;                                 /**< Synchronization handle id*/
    rtk_bt_le_audio_big_sync_establish_t  establish_param;  /**< @ref rtk_bt_le_audio_big_sync_establish_t*/ 
} rtk_bt_le_audio_big_sync_establish_param_t;

/**
 * @struct    rtk_bt_le_audio_sync_setup_data_path_param_t
 * @brief     Bluetooth LE audio sync setup data path parameter.
 */
typedef struct
{  
    uint8_t sync_handle_id;                                       /**< Synchronization handle id*/   
    rtk_bt_le_audio_bis_setup_data_path_t setup_param;            /**< @ref rtk_bt_le_audio_bis_setup_data_path_t*/
} rtk_bt_le_audio_sync_setup_data_path_param_t;

/**
 * @struct    rtk_bt_le_audio_sync_remove_data_path_param_t
 * @brief     Bluetooth LE audio sync setup data path parameter.
 */
typedef struct
{  
    uint8_t sync_handle_id;                                 /**< Synchronization handle id*/    
    uint8_t bis_idx;                                        /**< bis index */
} rtk_bt_le_audio_sync_remove_data_path_param_t;

/**
 * @struct    rtk_bt_le_audio_bis_conn_handle_info_t
 * @brief     Bluetooth LE audio bis connetion handle information.
 */
typedef struct
{
    uint8_t bis_idx;                  /**< bis index */
    uint16_t bis_conn_handle;         /**< Connection handle of a BIS. */
} rtk_bt_le_audio_bis_conn_handle_info_t; 

/**
 * @struct    rtk_bt_le_audio_sync_bis_info_t
 * @brief     Bluetooth LE audio bis information.
 */
typedef struct
{
    uint8_t  bis_num;                                                                       /**< bis number*/ 
    rtk_bt_le_audio_bis_conn_handle_info_t bis_conn_info[RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM];    /**< @ref rtk_bt_le_audio_bis_conn_handle_info_t*/
} rtk_bt_le_audio_sync_bis_info_t;


/**
 * @struct    rtk_bt_le_audio_sync_get_bis_info_param_t
 * @brief     Bluetooth LE audio bis information get param.
 */
typedef struct
{
    uint8_t sync_handle_id;                                      /**< Synchronization handle id*/ 
    rtk_bt_le_audio_sync_bis_info_t *p_bis_info;                 /**< @ref rtk_bt_le_audio_sync_bis_info_t*/
} rtk_bt_le_audio_sync_get_bis_info_param_t;
                            
/**
 * @struct    rtk_bt_le_audio_bass_get_pa_sync_param_ind_t
 * @brief     Bluetooth LE audio bass set pa sync parameter.
 */
typedef struct
{
    uint8_t sync_handle_id;                          /**< Synchronization handle id*/
    uint8_t source_id;     
    bool    is_past; 
    uint16_t pa_interval;                          
    uint8_t *pa_sync_options;                        /**< @ref rtk_bt_le_pa_sync_create_opt_t */
    uint16_t *pa_sync_skip;                          /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                                     Range: 0x0000 to 0x01F3 */
    uint16_t *pa_sync_timeout;                  /**< Synchronization timeout for the periodic advertising train.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
    uint16_t *past_timeout;                     /**< Synchronization timeout for PAST.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
}rtk_bt_le_audio_bass_get_pa_sync_param_ind_t;

/**
 * @struct    rtk_bt_le_audio_bass_get_big_sync_param_ind_t
 * @brief     Bluetooth LE audio bass set big sync parameter.
 */
typedef struct
{
    uint8_t sync_handle_id;                 /**< Synchronization handle id*/
    uint8_t source_id;                              
    uint8_t *big_mse;                       /**< The maximum number of subevents that a Controller should use to receive data
                                                payloads in each interval for a BIS.
                                                0x00: The Controller can schedule reception of any number of subevents up
                                                      to NSE.
                                                0x01 to 0x1F: Maximum number of subevents that should be used to receive data
                                                              payloads in each BIS event      */
    uint16_t *big_sync_timeout;             /**< Specify the maximum permitted time between successful receptions of BIS PDUs.
                                                Range: 0x000A to 0x4000
                                                Time = N*10 ms
                                                Time Range: 100 ms to 163.84 s */

}rtk_bt_le_audio_bass_get_big_sync_param_ind_t;

/**
 * @struct    rtk_bt_le_audio_bass_get_broadcast_code_ind_t
 * @brief     Bluetooth LE audio bass set broadcast code.
 */
typedef struct
{
    uint8_t sync_handle_id;                 /**< Synchronization handle id*/
    uint8_t source_id;                              
    uint8_t broadcast_code[RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN]; /**< Used to generate the session key to decrypt payloads of an encrypted BIS. */
}rtk_bt_le_audio_bass_get_broadcast_code_ind_t;

/**
 * @struct    rtk_bt_le_audio_bass_set_prefer_bis_sync_ind_t
 * @brief     Bluetooth LE audio bass set refer bis sync.
 */
typedef struct
{
    uint8_t sync_handle_id;                 /**< Synchronization handle id*/
    uint8_t source_id;   
    uint8_t  num_subgroups;                           
    uint8_t  subgroups_idx;
    uint32_t *bis_sync;
}rtk_bt_le_audio_bass_set_prefer_bis_sync_ind_t;
#endif

#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
/**
 * @struct    rtk_bt_le_audio_broadcast_source_bis_param_t
 * @brief     Bluetooth LE audio broadcast source bis param(BASE Level3).
 */
typedef struct
{
    uint8_t  codec_cfg_l3_len;
    uint8_t  *p_codec_cfg_l3;
    uint8_t  *bis_idx;          /**< returned bis index */
} rtk_bt_le_audio_broadcast_source_bis_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_subgroup_param_t
 * @brief     Bluetooth LE audio broadcast source subgroup param(BASE Level2).
 */
typedef struct
{
    uint8_t num_bis;
    rtk_bt_le_audio_broadcast_source_bis_param_t bis[RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_bis_param_t*/
    uint8_t codec_id[RTK_BT_LE_AUDIO_CODEC_ID_LEN];         /*!< Specify the coding format used over the air. 
                                                                Octet 0: See Assigned Numbers for Coding Format.
                                                                Octets 1 to 2: Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if octet 0 is not 0xFF.
                                                                Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF. */
    uint8_t codec_cfg_l2_len;
    uint8_t *p_codec_cfg_l2;
    uint8_t metadata_len;
    uint8_t *p_metadata;
    uint8_t *subgroup_idx;
} rtk_bt_le_audio_broadcast_source_subgroup_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_group_param_t
 * @brief     Bluetooth LE audio broadcast source group parameter (BASE Level1).
 */
typedef struct
{
    uint32_t presentation_delay;                       
    uint8_t num_subgroups;        
    rtk_bt_le_audio_broadcast_source_subgroup_param_t subgroup[RTK_BT_LE_AUDIO_BROADCASTER_SUBGROUP_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_subgroup_param_t*/  
    uint8_t *group_idx;                                /**< Returned broadcast source group index*/ 
} rtk_bt_le_audio_broadcast_source_group_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_setup_data_path_param_t
 * @brief     Bluetooth LE audio bis setup data path parameter.
 */
typedef struct
{  
    uint8_t group_idx;                                 /**< Synchronization handle id*/   
    rtk_bt_le_audio_bis_setup_data_path_t setup_param;       /**< @ref rtk_bt_le_audio_bis_setup_data_path_t*/
} rtk_bt_le_audio_broadcast_source_setup_data_path_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_setup_data_path_param_t
 * @brief     Bluetooth LE audio bis setup data path parameter.
 */
typedef struct
{  
    uint8_t group_idx;                                 /**< Synchronization handle id*/   
    uint8_t bis_idx;                                   /**< bis index */
} rtk_bt_le_audio_broadcast_source_remove_data_path_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_create_param_t
 * @brief     Bluetooth LE audio broadcast source create parameter.
 */
typedef struct
{   
    uint8_t num_groups;   
    rtk_bt_le_audio_broadcast_source_group_param_t group[RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_group_param_t*/             
} rtk_bt_le_audio_broadcast_source_create_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_bis_ipc_param_t
 * @brief     Bluetooth LE audio broadcast source bis ipc param(BASE Level3).
 */
typedef struct
{
    uint8_t  bis_idx;          /**< returned bis index */
} rtk_bt_le_audio_broadcast_source_bis_ipc_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t
 * @brief     Bluetooth LE audio broadcast source subgroup ipc param(BASE Level2).
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_bis_ipc_param_t bis[RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_bis_ipc_param_t*/
    uint8_t subgroup_idx;
} rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_group_ipc_param_t
 * @brief     Bluetooth LE audio broadcast source group ipc parameter (BASE Level1).
 */
typedef struct
{       
    rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t subgroup[RTK_BT_LE_AUDIO_BROADCASTER_SUBGROUP_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t*/  
    uint8_t group_idx;                                /**< Returned broadcast source group index*/ 
} rtk_bt_le_audio_broadcast_source_group_ipc_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_create_param_t
 * @brief     Bluetooth LE audio broadcast source create parameter.
 */
typedef struct
{ 
    rtk_bt_le_audio_broadcast_source_group_ipc_param_t group[RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_group_ipc_param_t*/             
} rtk_bt_le_audio_broadcast_source_create_ipc_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_state_t
 * @brief     Bluetooth LE audio state for broadcast source.
 */
typedef enum
{  
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_IDLE                = 0x00,
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_CONFIGURED_STARTING = 0x01,
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_CONFIGURED          = 0x02,
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_CONFIGURED_STOPPING = 0x03,
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_STREAMING_STARTING  = 0x04,
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_STREAMING           = 0x05,
    RTK_BT_LE_AUDIO_BROADCAST_SOURCE_STATE_STREAMING_STOPPING  = 0x06 
} rtk_bt_le_audio_broadcast_source_state_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_info_t
 * @brief     Bluetooth LE audio broadcast source infomation.
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_state_t state;         /**< broadcast source state. @ref rtk_bt_le_audio_broadcast_source_state_t */
    uint8_t adv_sid;
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];
    uint8_t ext_adv_state;              /**< ext adv state. @ref rtk_bt_le_adv_state */
    uint8_t pa_state;                   /**< pa state. @rtk_bt_le_pa_state_t */
    uint8_t big_state;                  /**< big state. @rtk_bt_le_iso_big_broadcaster_sync_state_t*/
    uint8_t adv_handle;
    uint8_t big_handle;
} rtk_bt_le_audio_broadcast_source_info_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_info_get_param_t
 * @brief     Bluetooth LE audio param for get broadcast source infomation.
 */
typedef struct
{
    uint8_t group_idx;                      /**< Broadcast source group id*/
    rtk_bt_le_audio_broadcast_source_info_t *p_info;   /**< broadcast source infomation. @ref rtk_bt_le_audio_broadcast_source_info_t */ 
} rtk_bt_le_audio_broadcast_source_info_get_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_conn_handle_get_param_t
 * @brief     Bluetooth LE audio param for get broadcast source bis conn handle.
 */
typedef struct
{
    uint8_t group_idx;                      /**< Broadcast source group id. */
    uint8_t bis_idx;                        /**< BIS index. */
    uint16_t *p_bis_conn_handle;            /**< Connection handle of a BIS. */
} rtk_bt_le_audio_broadcast_source_conn_handle_get_param_t;
                            
/**
 * @struct    rtk_bt_le_audio_broadcast_source_config_t
 * @brief     Bluetooth LE audio param for config broadcast source.
 */
typedef struct
{
    uint32_t primary_adv_interval_min;              	/** Minimum advertising interval for undirected and low duty cycle\n
                                                        * directed advertising.\n
                                                        * Range: 0x000020 to 0xFFFFFF\n
                                                        * Time = N * 0.625 ms\n
                                                        * Time Range: 20 ms to 10,485.759375 s */        
    uint32_t primary_adv_interval_max;              	/** Maximum advertising interval for undirected and low duty cycle\n
                                                        * directed advertising.\n
                                                        * Range: 0x000020 to 0xFFFFFF\n
                                                        * Time = N * 0.625 ms\n
                                                        * Time Range: 20 ms to 10,485.759375 s */
    uint8_t primary_adv_channel_map;                    /** Advertising_Primary_Channel_Map */
    rtk_bt_le_addr_t own_addr;	                        /** Own_Address_Type_and_Address */
    rtk_bt_le_adv_filter_t filter_policy;               /** Advertising_Filter_Policy */
    uint8_t tx_power;                           	    /** Advertising_TX_Power: indicates max Tx power.\n
                                                        * Range: -127 to +20\n
                                                        * Units: dBm\n
                                                        * 0x7F: Host has no preference. */
    rtk_bt_le_phy_prim_adv_t primary_adv_phy;           /** Primary_Advertising_PHY */
    uint8_t secondary_adv_max_skip;                     /** Secondary_Advertising_Max_Skip */
    rtk_bt_le_phy_type_t secondary_adv_phy;             /** Secondary_Advertising_PHY */
    uint16_t periodic_adv_interval_min;                 /** Minimum advertising interval for periodic advertising.\n
                                                        * Must be different from periodic_adv_interval_max.\n
                                                        * Range: 0x0006 to 0xFFFF\n
                                                        * Time = N * 1.25 ms\n
                                                        * Time Range: 7.5 ms to 81.91875 s\n */
    uint16_t periodic_adv_interval_max;                 /** Maximum advertising interval for periodic advertising.\n
                                                        * Must be different from periodic_adv_interval_min.\n
                                                        * Range: 0x0006 to 0xFFFF\n
                                                        * Time = N * 1.25 ms\n
                                                        * Time Range: 7.5 ms to 81.91875 s\n */
    uint16_t periodic_adv_prop;                          /** Periodic_Advertising_Properties, @ref rtk_bt_le_pa_prop_t */
} rtk_bt_le_audio_broadcast_source_config_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_config_param_t
 * @brief     Bluetooth LE audio param for config broadcast source.
 */
typedef struct
{
    uint8_t group_idx;                      /**< Broadcast source group id*/
    rtk_bt_le_audio_broadcast_source_config_t config;   /**< @ref rtk_bt_le_audio_broadcast_source_config_t */
} rtk_bt_le_audio_broadcast_source_config_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_create_big_t
 * @brief     Bluetooth LE audio param for broadcast source create big.
 */
typedef struct
{
    uint8_t num_bis;                 /**< Total number of BISes in the BIG. */
    uint32_t sdu_interval;           /**< The interval, in microseconds, of periodic SDUs.
                                          Range: 0x0000FF to 0x0FFFFF */
    uint16_t max_sdu;                /**< Maximum size of an SDU, in octets. */
    uint16_t max_transport_latency;  /**< Maximum transport latency, in milliseconds.
                                          Range: 0x0005 to 0x0FA0 */
    uint8_t rtn;                     /**< The number of times that every BIS Data PDU should be retransmitted.
                                          Range: 0x00 to 0x1E */
    uint8_t phy;                     /**< A bit field that indicates the PHY used for transmission of PDUs of BISes in the BIG.
                                          bit 0: The transmitter PHY of packets is LE 1M.
                                          bit 1: The transmitter PHY of packets is LE 2M.
                                          bit 2: The transmitter PHY of packets is LE Coded. */
    uint8_t packing;                /**< Indicate the preferred method of arranging subevents of multiple BISes.
                                          0x00: Sequential
                                          0x01: Interleaved */
    uint8_t framing;                /**< Indicate the format for sending BIS Data PDUs.
                                          0x00: Unframed
                                          0x01: Framed */
    uint8_t encryption;              /**< Identify the encryption mode of the BISes.
                                          0x00: Unencrypted
                                          0x01: Encrypted */
    uint8_t broadcast_code[RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN];      /**< Used to generate the encryption key for encrypting payloads of all BISes. */
} rtk_bt_le_audio_broadcast_source_create_big_t;


/**
 * @struct    rtk_bt_le_audio_broadcast_source_enable_param_t
 * @brief     Bluetooth LE audio param for broadcast source create big.
 */
typedef struct
{
    uint8_t group_idx;                      /**< Broadcast source group id*/
    rtk_bt_le_audio_broadcast_source_create_big_t create_big;   /**< @ref rtk_bt_le_audio_broadcast_source_create_big_param_t*/
} rtk_bt_le_audio_broadcast_source_enable_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_reconfig_param_t
 * @brief     Bluetooth LE audio param for broadcast source reconfig.
 */
typedef struct
{
    uint8_t group_idx;                      /**< Broadcast source group id*/
    uint8_t subgroup_idx;                   /**< Broadcast source subgroup id*/
    uint8_t metadata_len;                   /**< metadata len*/
    uint8_t *p_metadata;                    /**< metadata*/
} rtk_bt_le_audio_broadcast_source_reconfig_param_t;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_metadata_update_param_t
 * @brief     Bluetooth LE audio param for broadcast source metadata update.
 */
typedef rtk_bt_le_audio_broadcast_source_reconfig_param_t rtk_bt_le_audio_broadcast_source_metadata_update_param_t ;

/**
 * @struct    rtk_bt_le_audio_broadcast_source_state_ind_t
 * @brief     Bluetooth LE audio broadcast source state indication.
 */
typedef struct
{    
    uint8_t group_idx;                      /**< Broadcast source group id*/
    uint8_t broadcast_source_state;         /**< Broadcast source state. @ref rtk_bt_le_audio_broadcast_source_state_t*/
    uint8_t cause;                          /**< process result */           
} rtk_bt_le_audio_broadcast_source_state_ind_t;
#endif
/**
 * @struct    rtk_bt_le_audio_vcs_param_t
 * @brief     Bluetooth LE audio vcs param.
 */
typedef struct
{
    uint8_t volume_setting;
    uint8_t mute;
    uint8_t change_counter;
    uint8_t volume_flags;
    uint8_t step_size;
} rtk_bt_le_audio_vcs_param_t;

/**
 * @struct    rtk_bt_le_audio_vcs_cp_op_t
 * @brief     Bluetooth LE audio vcs cp op.
 */
typedef enum
{
    RTK_LE_VCS_CP_RELATIVE_VOLUME_DOMN        = 0x00,
    RTK_LE_VCS_CP_RELATIVE_VOLUME_UP          = 0x01,
    RTK_LE_VCS_CP_UNMUTE_RELATIVE_VOLUME_DOMN = 0x02,
    RTK_LE_VCS_CP_UNMUTE_RELATIVE_VOLUME_UP   = 0x03,
    RTK_LE_VCS_CP_SET_ABSOLUTE_VOLUME         = 0x04,
    RTK_LE_VCS_CP_UNMUTE                      = 0x05,
    RTK_LE_VCS_CP_MUTE                        = 0x06,
    RTK_LE_VCS_CP_MAX
} rtk_bt_le_audio_vcs_cp_op_t;

/**
 * @struct    rtk_bt_le_audio_vcs_cp_param_t
 * @brief     Bluetooth LE audio vcs cp param.
 */
typedef struct
{
    bool counter_used;
    uint8_t change_counter;
    uint8_t volume_setting;//used only for VCS_CP_SET_ABSOLUTE_VOLUME
} rtk_bt_le_audio_vcs_cp_param_t;

/**
 * @struct    rtk_bt_le_audio_vcs_send_volume_cp_param_t
 * @brief     Bluetooth LE audio vcs cp param.
 */
typedef struct
{
    uint16_t conn_handle;                     /**< connection handle */
    rtk_bt_le_audio_vcs_cp_op_t cp_op;        /**<cp op. @ref rtk_bt_le_audio_vcs_cp_op_t */
    rtk_bt_le_audio_vcs_cp_param_t cp_param;  /**< cp param. @ref rtk_bt_le_audio_vcs_cp_param_t*/
} rtk_bt_le_audio_vcs_send_volume_cp_param_t;


/**
 * @struct    rtk_bt_le_audio_vcs_volume_state_t
 * @brief     Bluetooth LE audio vcs volume state.
 */
typedef struct
{
    uint8_t volume_setting;
    uint8_t mute;
    uint8_t change_counter;
} rtk_bt_le_audio_vcs_volume_state_t;

/**
 * @struct    rtk_bt_le_audio_vcs_get_volume_state_param_t
 * @brief     Bluetooth LE audio vcs volume state get param.
 */
typedef struct
{
    uint16_t conn_handle;                                     /**< connection handle */
    rtk_bt_le_audio_vcs_volume_state_t *p_vcs_volume_state ;  /**< vcs volume state. @ref rtk_bt_le_audio_vcs_volume_state_t*/
} rtk_bt_le_audio_vcs_get_volume_state_param_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_le_iso BT LE AUDIO APIs
 * @brief     BT LE AUDIO related function APIs
 * @ingroup   BT_APIs
 * @{
 */


/**
 * @fn        uint16_t rtk_bt_le_audio_data_send(rtk_bt_le_audio_iso_data_send_info_t *info)
 * @brief     Send ISO data [SYNC].
 * @param[in] info: The info of iso send data
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_iso_data_send(rtk_bt_le_audio_iso_data_send_info_t *info);

/**
 * @fn        uint16_t rtk_bt_le_audio_csis_gen_rsi(uint8_t *p_sirk, uint8_t *p_rsik)
 * @brief     BLE AUDIO generate RSI for CSIS. 
 * @param[in] p_sirk: sirk
 * @param[out] p_rsik: rsik
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_gen_rsi(uint8_t *p_sirk, uint8_t *p_rsik);

/**
 * @fn        uint16_t rtk_bt_le_audio_csis_change_sirk(rtk_bt_le_audio_csis_sirk_type_t sirk_type,uint8_t *p_sirk)
 * @brief     BLE AUDIO change SIRK for CSIS. 
 * @param[in] sirk_type: sirk type
 * @param[out] p_sirk: new sirk
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_change_sirk(rtk_bt_le_audio_csis_sirk_type_t sirk_type,uint8_t *p_sirk);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_client_send_mcp_op(uint16_t conn_handle,uint8_t srv_idx,bool general_mcs,uint8_t opcode,int32_t op_param)
 * @brief     BLE AUDIO MCS CLIENT SEND MCP OP. 
 * @param[in] conn_handle: connection handle
 * @param[in] srv_idx: Service Index
 * @param[in] general_mcs: is generic media control service (GMCS)
 * @param[in] opcode: op code
 * @param[in] op_param: op code param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_client_send_mcp_op(uint16_t conn_handle,uint8_t srv_idx,bool general_mcs,uint8_t opcode,int32_t op_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_client_read_char_value(uint16_t conn_handle,uint8_t srv_idx,bool general_mcs,uint16_t uuid)
 * @brief     BLE AUDIO MCS CLIENT READ CHAR VALUE. 
 * @param[in] conn_handle: connection handle
 * @param[in] srv_idx: Service Index
 * @param[in] general_mcs: is generic media control service (GMCS)
 * @param[in] uuid: uuid
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_client_read_char_value(uint16_t conn_handle,uint8_t srv_idx,bool general_mcs,uint16_t uuid);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_client_cfg_cccd(uint16_t conn_handle,uint8_t srv_idx,bool general_mcs,uint32_t cfg_flags,bool enable)
 * @brief     BLE AUDIO MCS CLIENT CONFIG CCCD. 
 * @param[in] conn_handle: connection handle
 * @param[in] srv_idx: Service Index
 * @param[in] general_mcs: is generic media control service (GMCS)
 * @param[in] cfg_flags: cfg flags
 * @param[in] enable: enable
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_client_cfg_cccd(uint16_t conn_handle,uint8_t srv_idx,bool general_mcs,uint32_t cfg_flags,bool enable);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_media_player_info_set(uint8_t service_id,rtk_bt_le_audio_mcs_media_player_info_t *p_media_player_info)
 * @brief     BLE AUDIO mcs media player info set.  
 * @param[in] service_id: mcs service id
 * @param[in] p_media_player_info: media player info
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_media_player_info_set(uint8_t service_id,rtk_bt_le_audio_mcs_media_player_info_t *p_media_player_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_media_track_info_set(uint8_t service_id,rtk_bt_le_audio_mcs_media_track_info_t *p_media_track_info)
 * @brief     BLE AUDIO mcs media track info set.  
 * @param[in] service_id: mcs service id
 * @param[in] p_media_track_info: media track info
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_media_track_info_set(uint8_t service_id,rtk_bt_le_audio_mcs_media_track_info_t *p_media_track_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_media_state_update(uint8_t service_id,rtk_bt_le_audio_mcs_media_state_t media_state)
 * @brief     BLE AUDIO mcs media state update.  
 * @param[in] service_id: mcs service id
 * @param[in] media_state: media state
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_media_state_update(uint8_t service_id,rtk_bt_le_audio_mcs_media_state_t media_state);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_set_mcp_sup_opcode(uint8_t service_id,uint32_t mcp_sup_opcode)
 * @brief     BLE AUDIO mcs media state update.  
 * @param[in] service_id: mcs service id
 * @param[in] mcp_sup_opcode: mcp sup opcode
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_set_mcp_sup_opcode(uint8_t service_id,uint32_t mcp_sup_opcode);

#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_create(uint8_t adv_sid,rtk_bt_le_addr_t adv_addr,uint8_t *broadcast_id,uint8_t * sync_handle_id)
 * @brief     BLE AUDIO SYNC CREATE. 
 * @param[in]  adv_sid: paramsters for le audio sync create
 * @param[in]  adv_addr: address of broadcast source 
 * @param[in]  broadcast_id: broadcast id of broadcast source 
 * @param[in]  sync_handle_id: returned synchronization handle id
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_create(uint8_t adv_sid,rtk_bt_le_addr_t adv_addr,uint8_t *broadcast_id,uint8_t * sync_handle_id);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_release(uint8_t sync_handle_id)
 * @brief     BLE AUDIO SYNC RELEASE. 
 * @param[in]  sync_handle_id: Synchronization Handle id
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_release(uint8_t sync_handle_id); 


/**
 * @fn        uint16_t rtk_bt_le_audio_pa_sync_establish(uint8_t sync_handle_id, rtk_bt_le_audio_pa_sync_establish_t * p_establish)
 * @brief     BLE AUDIO PA SYNC ESTABLISH. 
 * @param[in]  sync_handle_id: Synchronization Handle id
 * @param[in]  p_establish_param: paramsters for le audio pa sync establish
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pa_sync_establish(uint8_t sync_handle_id, rtk_bt_le_audio_pa_sync_establish_t * p_establish);

/**
 * @fn        uint16_t rtk_bt_le_audio_pa_sync_terminate(uint8_t sync_handle_id)
 * @brief     BLE AUDIO PA SYNC TERMINATE. 
 * @param[in]  sync_handle_id: Synchronization Handle id
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pa_sync_terminate(uint8_t sync_handle_id);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_big_info(uint8_t sync_handle_id, rtk_bt_le_audio_biginfo_t *p_big_info)
 * @brief     BLE AUDIO BIG SYNC INFO GET. 
 * @param[in]  sync_handle_id: Synchronization Handle id
 * @param[out] p_big_info: the return big information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_big_info(uint8_t sync_handle_id, rtk_bt_le_audio_biginfo_t *p_big_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_bis_sync_codec_cfg(uint8_t sync_handle_id, uint8_t bis_idx, rtk_bt_le_audio_cfg_codec_t *p_codec_cfg)
 * @brief     BLE AUDIO SYNC BIS CODEC CFG GET. 
 * @param[in]  sync_handle_id: Synchronization Handle id
 * @param[in]  bis_idx: bis index
 * @param[out] p_codec_cfg: the return codec config
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_bis_sync_codec_cfg(uint8_t sync_handle_id, uint8_t bis_idx, rtk_bt_le_audio_cfg_codec_t *p_codec_cfg);

/**
 * @fn        uint16_t rtk_bt_le_audio_big_sync_establish(uint8_t sync_handle_id, rtk_bt_le_audio_big_sync_establish_t * p_establish)
 * @brief     BLE AUDIO BIG SYNC ESTABLISH. 
 * @param[in]  sync_handle_id: Synchronization Handle id
 * @param[in]  p_establish_param: paramsters for le audio pa sync establish
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_big_sync_establish(uint8_t sync_handle_id, rtk_bt_le_audio_big_sync_establish_t * p_establish);

/**
 * @fn        uint16_t rtk_bt_le_audio_big_sync_terminate(uint8_t sync_handle_id)
 * @brief     BLE AUDIO BIG SYNC TERMINATE. 
 * @param[in] sync_handle_id: Synchronization handle id
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_big_sync_terminate(uint8_t sync_handle_id);

/**
 * @fn        rtk_bt_le_audio_sync_get_bis_info(uint8_t sync_handle_id, rtk_bt_le_audio_sync_bis_info_t *p_bis_info)
 * @brief     BLE AUDIO BIS SYNC INFO GET. 
 * @param[in] sync_handle_id: Synchronization handle id
 * @param[out] p_bis_info: the return bis sync information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_bis_info(uint8_t sync_handle_id, rtk_bt_le_audio_sync_bis_info_t *p_bis_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_setup_data_path(uint8_t sync_handle_id, rtk_bt_le_audio_bis_setup_data_path_t *p_setup)
 * @brief     BLE AUDIO SETUP BIS DATA PATH. 
 * @param[in] sync_handle_id: Synchronization handle id
 * @param[in] p_setup_param: paramsters for le audio bis data path setup
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_setup_data_path(uint8_t sync_handle_id, rtk_bt_le_audio_bis_setup_data_path_t *p_setup);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_remove_data_path(uint8_t sync_handle_id, uint8_t bis_idx)
 * @brief     BLE AUDIO REMOVE BIS DATA PATH. 
 * @param[in]  sync_handle_id: Synchronization handle id
 * @param[in]  bis_idx: bis index
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_remove_data_path(uint8_t sync_handle_id, uint8_t bis_idx);
#endif //#if (defined RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT)

#if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)
/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_create(uint8_t num_groups, rtk_bt_le_audio_broadcast_source_group_param_t *group)
 * @brief     BLE AUDIO BROADCAST SOURCE CREATE
 * @param[in]  num_groups: num of broadcast source groups
 * @param[in]  group: broadcast source groups parameter
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_create(uint8_t num_groups, rtk_bt_le_audio_broadcast_source_group_param_t *group);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_get_info(uint8_t group_idx, rtk_bt_le_audio_broadcast_source_info_t *p_info)
 * @brief     BLE AUDIO BROADCAST SOURCE GET INFO 
 * @param[in]  group_idx: broadcast source group idx
 * @param[out] p_info: the returned broadcast source information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_get_info(uint8_t group_idx, rtk_bt_le_audio_broadcast_source_info_t *p_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_get_conn_handle(uint8_t group_idx, uint8_t bis_idx, uint16_t *p_bis_conn_handle)
 * @brief     BLE AUDIO BROADCAST SOURCE GET BIS CONN HANDLE 
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  bis_idx: bis index
 * @param[out] p_bis_conn_handle: the returned broadcast source conn handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_get_conn_handle(uint8_t group_idx, uint8_t bis_idx, uint16_t *p_bis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_config(uint8_t group_idx, rtk_bt_le_audio_broadcast_source_config_t *p_config)
 * @brief     BLE AUDIO BROADCAST SOURCE CONFIG 
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  p_config: broadcast source config information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_config(uint8_t group_idx, rtk_bt_le_audio_broadcast_source_config_t *p_config);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_enable(uint8_t group_idx, rtk_bt_le_audio_broadcast_source_create_big_t *p_create_big)
 * @brief     BLE AUDIO BROADCAST SOURCE ENABLE 
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  p_create_big: broadcast source enable information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_enable(uint8_t group_idx, rtk_bt_le_audio_broadcast_source_create_big_t *p_create_big);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_disable(uint8_t group_idx)
 * @brief     BLE AUDIO BROADCAST SOURCE DISABLE 
 * @param[in]  group_idx: broadcast source group idx
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_disable(uint8_t group_idx);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_release(uint8_t group_idx)
 * @brief     BLE AUDIO BROADCAST SOURCE RELEASE 
 * @param[in]  group_idx: broadcast source group idx
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_release(uint8_t group_idx);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_setup_data_path(uint8_t group_idx, rtk_bt_le_audio_bis_setup_data_path_t *p_setup)
 * @brief     BLE AUDIO BROADCAST SORUCE SETUP BIS DATA PATH. 
 * @param[in] group_idx: broadcast source group idx
 * @param[in] p_setup_param: paramsters for le audio bis data path setup
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_setup_data_path(uint8_t group_idx, rtk_bt_le_audio_bis_setup_data_path_t *p_setup);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_remove_data_path(uint8_t group_idx, uint8_t bis_idx)
 * @brief     BLE AUDIO BROADCAST SORUCE REMOVE BIS DATA PATH. 
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  bis_idx: bis index
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_remove_data_path(uint8_t group_idx, uint8_t bis_idx);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_reconfig(uint8_t group_idx, uint8_t subgroup_idx, uint8_t metadata_len, uint8_t *p_metadata)
 * @brief     BLE AUDIO BROADCAST SORUCE RECONFIG. 
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  group_idx: broadcast source subgroup idx
 * @param[in]  metadata_len: metadata len
 * @param[in]  p_metadata: p_metadata
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_reconfig(uint8_t group_idx, uint8_t subgroup_idx, uint8_t metadata_len, uint8_t *p_metadata);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_metadata_update(uint8_t group_idx, uint8_t subgroup_idx, uint8_t metadata_len, uint8_t *p_metadata)
 * @brief     BLE AUDIO BROADCAST SORUCE METADATA UPDATE. 
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  group_idx: broadcast source subgroup idx
 * @param[in]  metadata_len: metadata len
 * @param[in]  p_metadata: p_metadata
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_metadata_update(uint8_t group_idx, uint8_t subgroup_idx, uint8_t metadata_len, uint8_t *p_metadata);

#endif //end of #if (defined RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && (RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT)

/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_param_set(rtk_bt_le_audio_vcs_param_t *param)
 * @brief     BLE AUDIO VCS PARAM SET. 
 * @param[in]  param: vcs param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_param_set(rtk_bt_le_audio_vcs_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_param_get(rtk_bt_le_audio_vcs_param_t *param)
 * @brief     BLE AUDIO VCS PARAM GET. 
 * @param[out]  param: vcs param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_param_get(rtk_bt_le_audio_vcs_param_t *param);

/**
 * @fn        uint16_t rtk_bt_audio_vcs_send_volume_cp(uint16_t conn_handle,rtk_bt_le_audio_vcs_cp_op_t cp_op, rtk_bt_le_audio_vcs_cp_param_t *cp_param)
 * @brief     BLE AUDIO VCS SEND VOLUME CP. 
 * @param[in]  conn_handle: connection handle
 * @param[in]  cp_op:  cp op code 
 * @param[in]  cp_param: cp param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_audio_vcs_send_volume_cp(uint16_t conn_handle,rtk_bt_le_audio_vcs_cp_op_t cp_op, rtk_bt_le_audio_vcs_cp_param_t *cp_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_get_volume_state(uint16_t conn_handle,rtk_bt_le_audio_vcs_volume_state_t *p_vcs_volume_state)
 * @brief     BLE AUDIO VCS GET VOLUME STATE. 
 * @param[in]  conn_handle: connection handle
 * @param[out] p_vcs_volume_state:  the returned vcs volume state
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_get_volume_state(uint16_t conn_handle,rtk_bt_le_audio_vcs_volume_state_t *p_vcs_volume_state);

/**
 * @}
 */

#endif //end of #if (defined RTK_BLE_AUDIO_SUPPORT) && (RTK_BLE_AUDIO_SUPPORT)

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_BT_LE_ISO_H__ */