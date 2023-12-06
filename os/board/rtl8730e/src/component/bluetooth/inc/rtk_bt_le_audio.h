/*
 *******************************************************************************
 * Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
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

#if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT

/* Define ASCS max ase num.*/
#define RTK_BT_LE_ASCS_AES_CHAR_MAX_NUM    7

/* Define metadata max len.*/
#define RTK_BT_LE_AUDIO_METADATA_MAX_LEN  (32)

/* Define PA advertiser max num.*/
#define RTK_BT_LE_AUDIO_PA_ADV_SET_MAX_NUM  4

/* Define PA Sync max num.*/
#define RTK_BT_LE_AUDIO_SYNC_HANDLE_MAX_NUM 4

/* Define ISOC CIG maximum number.*/
#define RTK_BT_LE_AUDIO_CIG_MAX_NUM         2

/* Define ISOC CIS maximum number.*/
#define RTK_BT_LE_AUDIO_CIS_MAX_NUM         4

/* Define broadcaster group maximum number.*/
#define RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM  2

/* Define broadcaster subgroup maximum number.*/
#define RTK_BT_LE_AUDIO_BROADCASTER_SUBGROUP_MAX_NUM  2

/* Define broadcaster bis maximum number.*/
#define RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM  4

/* Define receiver big maximum number.*/
#define RTK_BT_LE_AUDIO_RECEIVER_BIG_MAX_NUM  2

/* Define receiver bis maximum number.*/
#define RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM  4

/* Define scan delegator subgroup maximum number.*/
#define RTK_BT_LE_AUDIO_SCAN_DELEGATOR_SUBGROUP_MAX_NUM  2

/* Define bis max number.*/
#define RTK_BT_LE_AUDIO_BIS_MAX_NUM  ((RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM > RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM) ? RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM : RTK_BT_LE_AUDIO_RECEIVER_BIS_MAX_NUM)

/* Define iso max number.*/
#define RTK_BT_LE_AUDIO_ISO_MAX_NUM  ((RTK_BT_LE_AUDIO_CIS_MAX_NUM > RTK_BT_LE_AUDIO_BIS_MAX_NUM) ? RTK_BT_LE_AUDIO_CIS_MAX_NUM : RTK_BT_LE_AUDIO_BIS_MAX_NUM)

/* Define Audio Streaming Maximum CIS Number.*/
#define RTK_BT_LE_AUDIO_STREAM_SRV_CIS_MAX   (2)

/* Define BAP Unicast Server Maximum Number.*/
#define RTK_BT_LE_AUDIO_UNICAST_SERVERS_MAX_NUM (2) 

/* Define BAP Unicast Server Source ASE Maximum Number.*/
#define RTK_BT_LE_AUDIO_STREAM_SRV_SRC_ASE_MAX (2)

/* Define BAP Unicast Server Sink ASE Maximum Number.*/
#define RTK_BT_LE_AUDIO_STREAM_SRV_SNK_ASE_MAX (2)

/* Define BAP Unicast Server ASE Maximum Number.*/
#define RTK_BT_LE_AUDIO_STREAM_SRV_ASE_MAX (RTK_BT_LE_AUDIO_STREAM_SRV_SRC_ASE_MAX + RTK_BT_LE_AUDIO_STREAM_SRV_SNK_ASE_MAX)

/* Define CSIS Service RSI Length.*/
#define RTK_BT_LE_CSI_RSI_LEN           6

/* Define BD Address len.*/
#define RTK_BD_ADDR_LEN 	6

/*  Max length of device name, if device name length exceeds it, it will be truncated.*/
#define RTK_BT_GAP_DEVICE_NAME_LEN      (39+1)

/* Define ISO mode.*/
#define RTK_BT_LE_AUDIO_CIS_MODE        0x01
#define RTK_BT_LE_AUDIO_BIS_MODE        0x02

/* Define Codec ID and Codec ID Length.*/
#define RTK_BT_LE_LC3_CODEC_ID                        0x06
#define RTK_BT_LE_AUDIO_CODEC_ID_LEN                  (5)

/* Define Broadcast ID and Code Length.*/
#define RTK_BT_LE_AUDIO_BROADCAST_ID_LEN               (3)
#define RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN             (16)

/* Define Set Identity Resolving Key Length.*/
#define RTK_BT_LE_CSIS_SIRK_LEN        (16)

/* Define Service UUID */
#define RTK_BT_LE_GATT_UUID_AICS                           0x1843   /**< Audio Input Control Service */
#define RTK_BT_LE_GATT_UUID_VCS                            0x1844   /**< Volume Control Service */
#define RTK_BT_LE_GATT_UUID_VOCS                           0x1845   /**< Volume Offset Control Service */
#define RTK_BT_LE_GATT_UUID_CSIS                           0x1846   /**< Coordinated Set Identification Service */
#define RTK_BT_LE_GATT_UUID_MICS                           0x184D   /**< Microphone Input Control Service */
#define RTK_BT_LE_GATT_UUID_ASCS                           0x184E   /**< Audio Stream Control Service */
#define RTK_BT_LE_GATT_UUID_BASS                           0x184F   /**< Broadcast Audio Scan Service */
#define RTK_BT_LE_BASIC_AUDIO_ANNOUNCEMENT_SRV_UUID        0x1851   /**< Basic Audio Announcement Service */
#define RTK_BT_LE_BROADCAST_AUDIO_ANNOUNCEMENT_SRV_UUID    0x1852   /**< Broadcast Audio Announcement Service */
#define RTK_BT_LE_GATT_UUID_CAS             			   0x1853   /**< Common Audio Service */
#define RTK_BT_LE_GATT_UUID_TMAS                           0x1855   /**< Telephony and Media Audio Service */
#define RTK_BT_LE_PUBIC_BROADCAST_ANNOUNCEMENT_SRV_UUID    0x1856   /**< Public Broadcast Announcement Service */

//Public Broadcast Announcement features
#define RTK_BT_LE_AUDIO_PUBIC_BROADCAST_BIT_ENCRYPTED                 0x01
#define RTK_BT_LE_AUDIO_PUBIC_BROADCAST_BIT_STANDARD_QUALITY_PRESENT  0x02
#define RTK_BT_LE_AUDIO_PUBIC_BROADCAST_BIT_HIGH_QUALITY_PRESENT      0x04
#define RTK_BT_LE_AUDIO_PUBIC_BROADCAST_BIT_MASK                      0x07

/* Define Audio Context Type.*/
#define RTK_BT_LE_AUDIO_CONTEXT_PROHIBITED           0x0000
#define RTK_BT_LE_AUDIO_CONTEXT_UNSPECIFIED          0x0001     /**< Unspecified. */
#define RTK_BT_LE_AUDIO_CONTEXT_CONVERSATIONAL       0x0002     /**< Conversational. Conversation between humans. */
#define RTK_BT_LE_AUDIO_CONTEXT_MEDIA                0x0004     /**< Media. */
#define RTK_BT_LE_AUDIO_CONTEXT_GAME                 0x0008     /**< Game. Audio associated with video gaming. */
#define RTK_BT_LE_AUDIO_CONTEXT_INSTRUCTIONAL        0x0010     /**< Instructional. Instructional audio. */
#define RTK_BT_LE_AUDIO_CONTEXT_VOICE_ASSISTANTS     0x0020     /**< Voice assistants. Man-machine communication. */
#define RTK_BT_LE_AUDIO_CONTEXT_LIVE                 0x0040     /**< Live. Live audio */
#define RTK_BT_LE_AUDIO_CONTEXT_SOUND_EFFECTS        0x0080     /**< Sound effects. Sound effects including keyboard and touch feedback; menu and user interface sounds; and other system sounds. */
#define RTK_BT_LE_AUDIO_CONTEXT_NOTIFICATIONS        0x0100     /**< Notifications. Notification and reminder sounds; attention-seeking audio. */
#define RTK_BT_LE_AUDIO_CONTEXT_RINGTONE             0x0200     /**< Ringtone. Alerts the user to an incoming call. */
#define RTK_BT_LE_AUDIO_CONTEXT_ALERTS               0x0400     /**< Alerts. Alarms and timers; immediate alerts. */
#define RTK_BT_LE_AUDIO_CONTEXT_EMERGENCY_ALERT      0x0800     /**< Emergency alarm. Emergency alarm Emergency sounds. */
#define RTK_BT_LE_AUDIO_CONTEXT_MASK                 0x0FFF

/* Define Advertising Announcements Type.*/
#define RTK_BT_LE_ADV_TARGETED_ANNOUNCEMENT           0x01
#define RTK_BT_LE_ADV_GENERAL_ANNOUNCEMENT            0x00

/*The Preferred_Audio_Contexts LTV structure is typically included
in the Metadata field of PAC records exposed by Unicast Servers and Broadcast Sinks. */
#define RTK_BT_LE_METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS 0x01

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

/* Define Supported Sampling Frequencies.*/
#define RTK_BT_LE_SAMPLING_FREQUENCY_8K              0x0001
#define RTK_BT_LE_SAMPLING_FREQUENCY_16K             0x0004
#define RTK_BT_LE_SAMPLING_FREQUENCY_24K             0x0010
#define RTK_BT_LE_SAMPLING_FREQUENCY_32K             0x0020
#define RTK_BT_LE_SAMPLING_FREQUENCY_44_1K           0x0040
#define RTK_BT_LE_SAMPLING_FREQUENCY_48K             0x0080

/* Define Sampling Frequency Configurations.*/
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_8K              0x01
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_16K             0x03
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_24K             0x05
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_32K             0x06
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_44_1K           0x07
#define RTK_BT_LE_SAMPLING_FREQUENCY_CFG_48K             0x08

/* Define Supported Frame Durations.*/
#define RTK_BT_LE_FRAME_DURATION_7_5_MS_BIT           0x01
#define RTK_BT_LE_FRAME_DURATION_10_MS_BIT            0x02
#define RTK_BT_LE_FRAME_DURATION_PREFER_7_5_MS_BIT    0x10
#define RTK_BT_LE_FRAME_DURATION_PREFER_10_MS_BIT     0x20

/* Define Frame Duration Configurations.*/
#define RTK_BT_LE_FRAME_DURATION_CFG_7_5_MS              0x00
#define RTK_BT_LE_FRAME_DURATION_CFG_10_MS               0x01

/* Define CAP Codec Specific Capabilities Exist Flags.*/
#define RTK_BT_LE_CODEC_CAP_SUPPORTED_SAMPLING_FREQUENCIES_EXIST   0x0001
#define RTK_BT_LE_CODEC_CAP_SUPPORTED_FRAME_DURATIONS_EXIST        0x0002
#define RTK_BT_LE_CODEC_CAP_AUDIO_CHANNEL_COUNTS_EXIST             0x0004
#define RTK_BT_LE_CODEC_CAP_SUPPORTED_OCTETS_PER_CODEC_FRAME_EXIST 0x0008
#define RTK_BT_LE_CODEC_CAP_MAX_SUPPORTED_FRAMES_PER_SDU_EXIST     0x0010

/* Define Coordinated Set Identification Service Features Exist flags*/
#define RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_LOCK_EXIST          0x01
#define RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_SIZE_EXIST          0x02
#define RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_RANK_EXIST          0x04
#define RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_SIRK_NOTIFY_SUPPORT 0x10
#define RTK_BT_LE_AUDIO_CSIS_SET_MEMBER_SIZE_NOTIFY_SUPPORT 0x20

/* Define Codec Configuration Exist Flags.*/
#define RTK_BT_LE_CODEC_CFG_SAMPLING_FREQUENCY_EXIST       0x0001
#define RTK_BT_LE_CODEC_CFG_FRAME_DURATION_EXIST           0x0002
#define RTK_BT_LE_CODEC_CFG_AUDIO_CHANNEL_ALLOCATION_EXIST 0x0004
#define RTK_BT_LE_CODEC_CFG_OCTET_PER_CODEC_FRAME_EXIST    0x0008
#define RTK_BT_LE_CODEC_CFG_TYPE_BLOCKS_PER_SDU_EXIST      0x0010

/* Define Supported Audio Channel Counts.*/
#define RTK_BT_LE_AUDIO_CHANNEL_COUNTS_1             0x01
#define RTK_BT_LE_AUDIO_CHANNEL_COUNTS_2             0x02

/**
 * @typedef    rtk_bt_le_audio_pacs_char_value_exist_t
 * @brief     Define Audio Locations.
 */
typedef enum {
    RTK_BT_LE_AUDIO_LOCATION_MONO                =0x00000000,
    RTK_BT_LE_AUDIO_LOCATION_FL                  =0x00000001,
    RTK_BT_LE_AUDIO_LOCATION_FR                  =0x00000002,
    RTK_BT_LE_AUDIO_LOCATION_FC                  =0x00000004,
    RTK_BT_LE_AUDIO_LOCATION_LFE1                =0x00000008,
    RTK_BT_LE_AUDIO_LOCATION_BL                  =0x00000010,
    RTK_BT_LE_AUDIO_LOCATION_BR                  =0x00000020,
    RTK_BT_LE_AUDIO_LOCATION_FLC                 =0x00000040,
    RTK_BT_LE_AUDIO_LOCATION_FRC                 =0x00000080,
    RTK_BT_LE_AUDIO_LOCATION_BC                  =0x00000100,
    RTK_BT_LE_AUDIO_LOCATION_LFE2                =0x00000200,
    RTK_BT_LE_AUDIO_LOCATION_SIL                 =0x00000400,
    RTK_BT_LE_AUDIO_LOCATION_SIR                 =0x00000800,
    RTK_BT_LE_AUDIO_LOCATION_TPFL                =0x00001000,
    RTK_BT_LE_AUDIO_LOCATION_TPFR                =0x00002000,
    RTK_BT_LE_AUDIO_LOCATION_TPFC                =0x00004000,
    RTK_BT_LE_AUDIO_LOCATION_TPC                 =0x00008000,
    RTK_BT_LE_AUDIO_LOCATION_TPBL                =0x00010000,
    RTK_BT_LE_AUDIO_LOCATION_TPBR                =0x00020000,
    RTK_BT_LE_AUDIO_LOCATION_TPSIL               =0x00040000,
    RTK_BT_LE_AUDIO_LOCATION_TPSIR               =0x00080000,
    RTK_BT_LE_AUDIO_LOCATION_TPBC                =0x00100000,
    RTK_BT_LE_AUDIO_LOCATION_BTFC                =0x00200000,
    RTK_BT_LE_AUDIO_LOCATION_BTFL                =0x00400000,
    RTK_BT_LE_AUDIO_LOCATION_BTFR                =0x00800000,
    RTK_BT_LE_AUDIO_LOCATION_FLW                 =0x01000000,
    RTK_BT_LE_AUDIO_LOCATION_FRW                 =0x02000000,
    RTK_BT_LE_AUDIO_LOCATION_LS                  =0x04000000,
    RTK_BT_LE_AUDIO_LOCATION_RS                  =0x08000000,
    RTK_BT_LE_AUDIO_LOCATION_RFU                 =0xF0000000,
    RTK_BT_LE_AUDIO_LOCATION_MASK                =0x0FFFFFFF
} rtk_bt_le_audio_location_t;

//No current track or the duration of the current track is unknown in Generic Media Control Service (GMCS) or Media Control Service (MCS)
#define RTK_BT_LE_AUDIO_MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN    0xFFFFFFFF
//No current track or the start of the track is not well defined in Generic Media Control Service (GMCS) or Media Control Service (MCS)
#define RTK_BT_LE_AUDIO_MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE 0xFFFFFFFF 

/** Little Endian uint16 to array */
#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

/**
 * @typedef    rtk_bt_le_audio_pacs_char_value_exist_t
 * @brief     Bluetooth LE AUDIO PACS Characteristic Value Exist Mask.
 */
 typedef enum {
    RTK_BT_LE_AUDIO_PACS_AUDIO_AVAILABLE_CONTEXTS_EXIST = 0x01,
    RTK_BT_LE_AUDIO_PACS_AUDIO_SUPPORTED_CONTEXTS_EXIST = 0x02,	
    RTK_BT_LE_AUDIO_PACS_SINK_AUDIO_LOC_EXIST = 0x04,				      
    RTK_BT_LE_AUDIO_PACS_SINK_PAC_EXIST = 0x08,				    
    RTK_BT_LE_AUDIO_PACS_SOURCE_AUDIO_LOC_EXIST = 0x10,				    
    RTK_BT_LE_AUDIO_PACS_SOURCE_PAC_EXIST = 0x20										
} rtk_bt_le_audio_pacs_char_value_exist_t;

/**
 * @typedef    rtk_bt_le_audio_pacs_ba_check_filter_type_t
 * @brief     Bluetooth LE AUDIO PACS BA check filter type.
 */
typedef enum {
	RTK_BT_LE_AUDIO_PACS_BA_CHECK_FILTER_CODEC = 0x00,
	RTK_BT_LE_AUDIO_PACS_BA_CHECK_FILTER_ALLOCATION = 0x01
} rtk_bt_le_audio_pacs_ba_check_filter_type_t;

/**
 * @typedef    rtk_bt_le_audio_group_handle_t
 * @brief     Bluetooth LE AUDIO group handle.
 */ 
typedef void * rtk_bt_le_audio_group_handle_t;

/**
 * @typedef    rtk_bt_le_audio_device_handle_t
 * @brief     Bluetooth LE AUDIO device handle.
 */
typedef void * rtk_bt_le_audio_device_handle_t;

/**
 * @typedef    rtk_bt_le_audio_stream_session_handle_t
 * @brief     Bluetooth LE AUDIO stream session handle.
 */
typedef void * rtk_bt_le_audio_stream_session_handle_t;

/**
 * @typedef    rtk_bt_le_audio_bap_role_t
 * @brief     Bluetooth LE AUDIO BAP Role.
 */
typedef enum {
    RTK_BT_LE_AUDIO_BAP_ROLE_UNKNOWN = 0,
	RTK_BT_LE_AUDIO_BAP_ROLE_BRO_SOUR = 0x01,				    /**< BAP Broadcast Source role. */
    RTK_BT_LE_AUDIO_BAP_ROLE_BRO_SINK = 0x02,				    /**< BAP Broadcast Sink role. */    
    RTK_BT_LE_AUDIO_BAP_ROLE_BRO_ASSI = 0x04,				    /**< BAP Broadcast Assistant role. */
    RTK_BT_LE_AUDIO_BAP_ROLE_SCAN_DELE = 0x08,				    /**< BAP Scan Delegate role. */
    RTK_BT_LE_AUDIO_BAP_ROLE_UNI_SER = 0x10,					/**< BAP Unicast Server role. */
	RTK_BT_LE_AUDIO_BAP_ROLE_UNI_CLI = 0x20,					/**< BAP Unicast Client role. */
}rtk_bt_le_audio_bap_role_t;

/**
 * @typedef    rtk_bt_le_audio_cap_role_t
 * @brief     Bluetooth LE AUDIO CAP Role.
 */
typedef enum {
    RTK_BT_LE_AUDIO_CAP_ROLE_UNKNOWN = 0,
    RTK_BT_LE_AUDIO_CAP_ROLE_INITIATOR = 0x01,					/**< CAP Initiator. */
	RTK_BT_LE_AUDIO_CAP_ROLE_ACCEPTOR = 0x02,					/**< CAP Acceptor. */
    RTK_BT_LE_AUDIO_CAP_ROLE_COMMANDER = 0x04,				    /**< CAP Commander. */
}rtk_bt_le_audio_cap_role_t;

/**
 * @typedef    rtk_bt_le_audio_tmap_role_t
 * @brief     Bluetooth LE AUDIO TMAP Role.
 */
typedef enum {
    RTK_BT_LE_AUDIO_TMAP_ROLE_UNKNOWN = 0,
    RTK_BT_LE_AUDIO_TMAP_ROLE_CG = 0x01,					/**< TMAP Call Gateway. */
	RTK_BT_LE_AUDIO_TMAP_ROLE_CT = 0x02,					/**< TMAP Call Terminal. */
    RTK_BT_LE_AUDIO_TMAP_ROLE_UMS = 0x04,					/**< TMAP Unicast Media Sender. */
    RTK_BT_LE_AUDIO_TMAP_ROLE_UMR = 0x08,					/**< TMAP Unicast Media Receiver. */
	RTK_BT_LE_AUDIO_TMAP_ROLE_BMS = 0x10,					/**< TMAP Broadcast Media Sender. */
    RTK_BT_LE_AUDIO_TMAP_ROLE_BMR = 0x20,					/**< TMAP Broadcast Media Receiver. */
}rtk_bt_le_audio_tmap_role_t;

#if defined(RTK_BLE_AUDIO_TMAP_SUPPORT) && RTK_BLE_AUDIO_TMAP_SUPPORT
/**
 * @typedef   rtk_bt_le_audio_tmas_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio TMAS client discovery done indication
 */
typedef struct
{
    uint16_t conn_handle;                                      /**< connect handle. */
    bool    is_found;                                          /**< Whether to find service. */
    bool    load_from_ftl;                                     /**< Whether the service table is loaded from FTL. */
} rtk_bt_le_audio_tmas_client_discovery_done_ind_t;

/**
 * @typedef   rtk_bt_le_audio_tmas_read_role_result_t
 * @brief     Bluetooth LE audio TMAS client read TMAP role result
 */
typedef struct
{
    uint16_t conn_handle;
    uint16_t role;                                              /**< @ref rtk_bt_le_audio_tmap_role_t  */
    uint16_t cause;
} rtk_bt_le_audio_tmas_read_role_result_t;
#endif

#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
/**
 * @typedef   rtk_bt_le_audio_csis_init_param_t
 * @brief     Bluetooth LE audio CSIS init param
 */
typedef struct {
    uint8_t csis_size;
    uint8_t csis_rank;
    uint8_t csis_feature;   
    uint8_t csis_sirk_type;                                     /**< @ref rtk_bt_le_audio_csis_sirk_type_t  */
	uint8_t csis_sirk[RTK_BT_LE_CSIS_SIRK_LEN];
} rtk_bt_le_audio_csis_init_param_t;
#endif 

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT

/* Define AICS Audio Input Status Type.*/
#define RTK_BT_LE_AUDIO_AICS_INPUT_STATUS_INACTIVE               0
#define RTK_BT_LE_AUDIO_AICS_INPUT_STATUS_ACTIVE                 1

/**
 * @typedef   rtk_bt_le_audio_input_type_t
 * @brief     Bluetooth LE audio input type
 */
typedef enum {
    RTK_BT_LE_AUDIO_INPUT_UNSPECIFIED         = 0x00,     /**< Unspecified Input */
    RTK_BT_LE_AUDIO_INPUT_BLUETOOTH           = 0x01,     /**< Bluetooth Audio Stream */
    RTK_BT_LE_AUDIO_INPUT_MICROPHONE          = 0x02,     /**< Microphone */
    RTK_BT_LE_AUDIO_INPUT_ANALOG              = 0x03,     /**< Analog Interface */
    RTK_BT_LE_AUDIO_INPUT_DIGITAL             = 0x04,     /**< Digital Interface */
    RTK_BT_LE_AUDIO_INPUT_RADIO               = 0x05,     /**< AM/FM/XM/etc. */
    RTK_BT_LE_AUDIO_INPUT_STREAMING           = 0x06,     /**< Streaming Audio Source */
} rtk_bt_le_audio_input_type_t;

/**
 * @typedef   rtk_bt_le_audio_aics_init_param_t
 * @brief     Bluetooth LE audio AICS init param
 */
typedef struct { 
    uint8_t aics_vcs_num;
    uint8_t *p_aics_vcs_tbl;
    uint8_t aics_mics_num;
    uint8_t *p_aics_mics_tbl;
} rtk_bt_le_audio_aics_init_param_t;    
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
/**
 * @typedef   rtk_bt_le_audio_vocs_init_param_t
 * @brief     Bluetooth LE audio VOCS init param
 */
typedef struct { 
    uint8_t vocs_num;
    uint8_t *p_vocs_feature_tbl;            /*!< @ref rtk_bt_le_audio_vocs_feature_mask_t */
} rtk_bt_le_audio_vocs_init_param_t; 
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
/**
 * @typedef   rtk_bt_le_audio_mcs_init_param_t
 * @brief     Bluetooth LE audio MCS init param
 */
typedef struct {
    uint32_t general_mcs:1;
    uint32_t char_media_control_point_support:1;
    uint32_t char_media_control_point_opcodes_supported_optional_property_notify:1;
    uint32_t char_media_player_name_optional_property_notify:1;
    uint32_t char_track_title_optional_property_notify:1;
    uint32_t char_track_duration_optional_property_notify:1;
    uint32_t char_track_position_optional_property_notify:1;
    uint32_t char_playing_order_support:1;
    uint32_t char_playing_order_optional_property_notify:1;
    uint32_t char_playing_orders_supported_support:1;
    uint32_t char_playback_speed_support:1;
    uint32_t char_playback_speed_optional_property_notify:1;
    uint32_t char_seeking_speed_support:1;
    uint32_t char_seeking_speed_optional_property_notify:1;
    uint32_t char_media_player_icon_url_support:1;
    uint8_t  media_state;                                           /*!< @ref rtk_bt_le_audio_mcs_media_state_t */
    uint16_t playing_orders_supported;                              /*!< @ref rtk_bt_le_audio_mcs_playing_orders_supported_t */
    uint32_t media_control_point_opcodes_supported;                 /*!< @ref rtk_bt_le_audio_mcs_media_control_point_opcodes_supported_t */
    uint8_t *p_mcs_service_id;
} rtk_bt_le_audio_mcs_init_param_t;
#endif 

/**
 * @typedef   rtk_bt_le_audio_cap_init_param_t
 * @brief     Bluetooth LE audio CAP init param
 */
typedef struct {
    rtk_bt_le_audio_cap_role_t      cap_role;                       /*!< Default le audio cap role */
#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
    rtk_bt_le_audio_csis_init_param_t    csis_param;                     /*!< Default csis param */
#endif
#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
    rtk_bt_le_audio_aics_init_param_t aics_param;                         /*!< Default aics client param */
#endif
#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
    rtk_bt_le_audio_vocs_init_param_t vocs_param;                        /*!< Default vocs client param */
#endif
#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
    rtk_bt_le_audio_mcs_init_param_t mcs_param;                          /*!< Default mcs client param */
#endif
} rtk_bt_le_audio_cap_init_param_t;

/**
 * @typedef   rtk_bt_le_audio_pacs_init_param_t
 * @brief     Bluetooth LE audio PACS init param
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
} rtk_bt_le_audio_pacs_init_param_t;

/**
 * @typedef   rtk_bt_le_audio_ascs_init_param_t
 * @brief     Bluetooth LE audio ASCS init param
 */
typedef struct {
 	uint8_t sink_ase_num;
    uint8_t source_ase_num;
} rtk_bt_le_audio_ascs_init_param_t;

/**
 * @typedef rtk_bt_le_audio_app_conf_t
 * @brief   structure of default LE audio configuration
 */
typedef struct {
	rtk_bt_le_audio_bap_role_t   	bap_role;                		/*!< Default le audio bap role */
	rtk_bt_le_audio_pacs_init_param_t 	pacs_param;						/*!< Default pacs param */
	rtk_bt_le_audio_ascs_init_param_t	ascs_param;						/*!< Default ascs param */
	rtk_bt_le_audio_cap_init_param_t     cap_param;						/*!< Default cap param */ 
	rtk_bt_le_audio_tmap_role_t			tmap_role;						/*!< Default tmap param */ 
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
} rtk_bt_le_audio_cfg_codec_t;

/**
 * @typedef rtk_bt_le_audio_ascs_cp_codec_cfg_ind_t
 * @brief   structure of LE audio ascs codec config control point indication
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     ase_id;
    rtk_bt_le_audio_cfg_codec_t codec_cfg;          /**< @ref rtk_bt_le_audio_cfg_codec_t*/       
} rtk_bt_le_audio_ascs_cp_codec_cfg_ind_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_cp_qos_config_t
 * @brief     structure of LE audio ascs qos config control point indication
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
} rtk_bt_le_audio_ascs_cp_qos_cfg_ind_t;

/**
 * @typedef   rtk_bt_le_audio_iso_data_path_direction_t
 * @brief     Specify which directions have been configured.Or use the combination of macro definitions to specify which directions are to have the data path removed
*/
typedef enum {
    RTK_BLE_AUDIO_ISO_DATA_PATH_TX  = 0x01,    /**< Input (Host to Controller). Source or Tx*/
    RTK_BLE_AUDIO_ISO_DATA_PATH_RX,           /**< Output (Controller to Host). Sink or Rx*/
}rtk_bt_le_audio_iso_data_path_direction_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_ase_state_t
 * @brief     enum of LE audio ascs ase state
 */
typedef enum {
    RTK_BLE_AUDIO_ASCS_ASE_STATE_IDLE             = 0x00,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_CODEC_CONFIGURED = 0x01,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_QOS_CONFIGURED   = 0x02,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_ENABLING         = 0x03,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_STREAMING        = 0x04,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_DISABLING        = 0x05,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_RELEASING        = 0x06,
    RTK_BLE_AUDIO_ASCS_ASE_STATE_UNKOWN           = 0xff,
} rtk_bt_le_audio_ascs_ase_state_t;

/**
 * @typedef    rtk_bt_le_audio_unframed_supported_t
 * @brief     enum of LE audio support unframed iso pdu
 */
typedef enum
{
    RTK_BLE_AUDIO_UNFRAMED_SUPPORTED      = 0x00,
    RTK_BLE_AUDIO_UNFRAMED_NOT_SUPPORTED  = 0x01,
} rtk_bt_le_audio_unframed_supported_t;

/**
 * @typedef    rtk_bt_le_audio_framed_t
 * @brief     enum of LE audio framing parameter value for an ASE written by the client in the Config QoS operation
 */
typedef enum
{
    RTK_BLE_AUDIO_UNFRAMED = 0x00,
    RTK_BLE_AUDIO_FRAMED   = 0x01,
} rtk_bt_le_audio_framed_t;

/**
 * @typedef    rtk_bt_le_audio_role_t
 * @brief     enum of LE audio role
 */
typedef enum
{
    RTK_BLE_AUDIO_SINK = 0x01,
    RTK_BLE_AUDIO_SOURCE = 0x02,
} rtk_bt_le_audio_role_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_ase_target_latency_t
 * @brief     enum of LE audio ascs ase target latency
 */
typedef enum
{
    RTK_BLE_AUDIO_ASCS_ASE_TARGET_LOWER_LATENCY      = 0x01,    /**< Target low latency. */
    RTK_BLE_AUDIO_ASCS_ASE_TARGET_BALANCED           = 0x02,    /**< Target balanced latency and reliability. */
    RTK_BLE_AUDIO_ASCS_ASE_TARGET_HIGHER_RELIABILITY = 0x03,    /**< Target high reliability. */
} rtk_bt_le_audio_ascs_ase_target_latency_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_ase_target_phy_t
 * @brief     enum of LE audio ascs ase target phy
 */
typedef enum
{
    RTK_BLE_AUDIO_ASCS_ASE_TARGET_PHY_1M    = 0x01,      /**< LE 1M PHY. */
    RTK_BLE_AUDIO_ASCS_ASE_TARGET_PHY_2M    = 0x02,      /**< LE 2M PHY. */
    RTK_BLE_AUDIO_ASCS_ASE_TARGET_PHY_CODED = 0x03,      /**< LE Coded PHY. */
} rtk_bt_le_audio_ascs_ase_target_phy_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_cp_enable_ind_t
 * @brief     structure of LE audio ascs enable control point indication
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     ase_num;
    uint8_t     ase_id[RTK_BT_LE_ASCS_AES_CHAR_MAX_NUM];
} rtk_bt_le_audio_ascs_cp_enable_ind_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_cp_disable_ind_t
 * @brief     structure of LE audio ascs disable control point indication
 */
typedef rtk_bt_le_audio_ascs_cp_enable_ind_t rtk_bt_le_audio_ascs_cp_disable_ind_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_prefer_qos_data_t
 * @brief     structure of LE audio ascs prefer qos data
 */
typedef struct {
    uint8_t  supported_framing;
    uint8_t  preferred_phy;
    uint8_t  preferred_retrans_number;
    uint16_t max_transport_latency;
    uint32_t presentation_delay_min;
    uint32_t presentation_delay_max;
    uint32_t preferred_presentation_delay_min;
    uint32_t preferred_presentation_delay_max;
} rtk_bt_le_audio_ascs_prefer_qos_data_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_get_prefer_qos_ind_t
 * @brief     structure of LE audio ascs get prefer qos indication
 */
typedef struct {
    uint16_t             conn_handle;
    uint8_t              ase_id;
    uint8_t              direction;
    rtk_bt_le_audio_cfg_codec_t codec_cfg;
    rtk_bt_le_audio_ascs_ase_target_latency_t target_latency;
    rtk_bt_le_audio_ascs_ase_target_phy_t target_phy;    
    rtk_bt_le_audio_ascs_prefer_qos_data_t *p_prefer_qos_data;
} rtk_bt_le_audio_ascs_get_prefer_qos_ind_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_asc_state_ind_t
 * @brief     structure of LE audio ascs state
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     ase_id;
    rtk_bt_le_audio_role_t          audio_role;            /**< @ref rtk_bt_le_audio_role_t.*/
    rtk_bt_le_audio_ascs_ase_state_t     ase_state;        /**< @ref rtk_bt_le_audio_ascs_ase_state_t*/
} rtk_bt_le_audio_ascs_ase_state_ind_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_setup_data_path_ind_t
 * @brief     structure of LE audio ascs setup data path indication
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t  ase_id;
    rtk_bt_le_audio_iso_data_path_direction_t path_direction;/**< @ref rtk_bt_le_audio_iso_data_path_direction_t.*/
    uint16_t cis_conn_handle;                                 /**< connection handle of the CIS*/
    rtk_bt_le_audio_cfg_codec_t codec_cfg;                    /**< @ref rtk_bt_le_audio_cfg_codec_t*/
} rtk_bt_le_audio_ascs_setup_data_path_ind_t;

/**
 * @typedef    rtk_bt_le_audio_ascs_remove_data_path_ind_t
 * @brief     structure of LE audio ascs remove data path indication
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t  ase_id;
    rtk_bt_le_audio_iso_data_path_direction_t path_direction; /**< @ref rtk_bt_le_audio_iso_data_path_direction_t.*/
    uint16_t cis_conn_handle;                                 /**< connection handle of the CIS*/
} rtk_bt_le_audio_ascs_remove_data_path_ind_t;

/**
 * @typedef    rtk_bt_le_audio_iso_data_send_info_t
 * @brief     Bluetooth LE audio mgr data send info definition .
 */
typedef struct
{
    uint16_t iso_conn_handle;       /*!< connection handle of the ISO*/
    bool ts_flag;                   /*!< indicates whether contains time_stamp, true: contain time_stamp; false: not contain time_stamp */
    uint32_t time_stamp;            /*!< a time in microseconds. time_stamp is used when @ref ts_flag is True */
    uint16_t pkt_seq_num;           /*!< sequence number of the SDU */
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
 * @typedef    rtk_bt_le_iso_direct_iso_data_ind_t
 * @brief     Indication of ISO Datat packet with cb_type @ref RTK_BT_LE_ISO_EVT_DATA_RECEIVE_IND .
 */
typedef struct
{
    uint16_t iso_conn_handle;                                   /**< Connection handle of the ISO. */
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
 * @typedef    rtk_bt_le_audio_csis_lock_t
 * @brief     enum of LE audio csis lock
 */
typedef enum
{
    RTK_BT_LE_CSIS_NONE_LOCK  = 0,
    RTK_BT_LE_CSIS_UNLOCKED   = 0x01,
    RTK_BT_LE_CSIS_LOCKED     = 0x02,
} rtk_bt_le_audio_csis_lock_t;

/**
 * @typedef    rtk_bt_le_audio_csis_sirk_type_t
 * @brief     enum of LE audio csis sirk type
 */
typedef enum
{
    RTK_BT_LE_CSIS_SIRK_ENC = 0x00,             /**< Encrypted SIRK. */
    RTK_BT_LE_CSIS_SIRK_PLN = 0x01,             /**< Plain text SIRK. */
}rtk_bt_le_audio_csis_sirk_type_t;

/**
 * @typedef    rtk_bt_le_audio_csis_set_mem_info_t
 * @brief     Bluetooth LE audio csis set mem info definition .
 */
typedef struct
{
    rtk_bt_le_addr_t                    bd_addr;
    uint16_t                            srv_uuid;
    uint8_t                             srv_instance_id;
    uint8_t                             char_exit;
    uint8_t                             rank;
    rtk_bt_le_audio_csis_lock_t 	    lock;
    uint8_t                 		    set_mem_size;
    rtk_bt_le_audio_csis_sirk_type_t    sirk_type;
    uint8_t                 			sirk[RTK_BT_LE_CSIS_SIRK_LEN];
}rtk_bt_le_audio_csis_set_mem_info_t;

/**
 * @typedef    rtk_bt_le_audio_csis_client_discovery_done_ind_t
 * @brief     Indication of csis client discovery done event with cb_type @ref RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_DISCOVERY_DONE_IND .
 */
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    bool    load_from_ftl;
    uint8_t srv_num;
}rtk_bt_le_audio_csis_client_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_csis_client_read_result_ind_t
 * @brief     Indication of csis client read result event with cb_type @ref RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_READ_RESULT_IND .
 */
typedef struct
{
    uint16_t                            cause;
    uint16_t                            conn_handle;
    rtk_bt_le_audio_group_handle_t      group_handle;           /**< @ref rtk_bt_le_audio_group_handle_t. */ 
    rtk_bt_le_audio_device_handle_t     dev_handle;             /**< @ref rtk_bt_le_audio_device_handle_t. */ 
    rtk_bt_le_audio_csis_set_mem_info_t mem_info;               /**< @ref rtk_bt_le_audio_csis_set_mem_info_t. */ 
}rtk_bt_le_audio_csis_client_read_result_ind_t;

/**
 * @typedef    rtk_bt_le_audio_csis_client_search_done_ind_t
 * @brief     Indication of csis client search done or timeout event with cb_type @ref RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_SEARCH_DONE_IND.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t      group_handle;           /**< @ref rtk_bt_le_audio_group_handle_t. */ 
    uint8_t         set_mem_size;
    bool            search_done;
    bool            search_timeout;
}rtk_bt_le_audio_csis_client_search_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_csis_client_set_mem_found_ind_t
 * @brief     Indication of csis client set mem found event with cb_type @ref RTK_BT_LE_AUDIO_EVT_CSIS_CLIENT_SET_MEM_FOUND_IND.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t      group_handle;           /**< @ref rtk_bt_le_audio_group_handle_t. */ 
    rtk_bt_le_audio_device_handle_t     dev_handle;             /**< @ref rtk_bt_le_audio_device_handle_t. */ 
    rtk_bt_le_addr_t                    bd_addr;
    uint16_t                            srv_uuid;
    uint8_t                             rank;
    uint8_t                 		    set_mem_size;
    uint8_t                 			sirk[RTK_BT_LE_CSIS_SIRK_LEN];
}rtk_bt_le_audio_csis_client_set_mem_found_ind_t;


/**
 * @typedef    rtk_bt_le_audio_bap_discovery_done_ind_t
 * @brief     Bluetooth LE audio info for bap discovery all done indication.
 */
typedef struct
{
    uint16_t conn_handle;
    bool    pacs_is_found;
    uint8_t sink_pac_num;
    uint8_t source_pac_num;
    bool    ascs_is_found;
    uint8_t sink_ase_num;
    uint8_t source_ase_num;
    bool    bass_is_found;
    uint8_t brs_char_num;
} rtk_bt_le_audio_bap_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bap_pacs_info_t
 * @brief     Bluetooth LE audio bap pacs info.
 */
typedef struct
{
    uint16_t            value_exist;          /**< PACS characteristic value exist flag: @ref rtk_bt_le_audio_pacs_char_value_exist_t. */
    uint8_t             sink_pac_num;         /**< Sink PAC number. */
    uint8_t             source_pac_num;       /**< Source PAC number. */
    uint32_t            snk_audio_loc;        /**< Sink Audio Locations. */
    uint32_t            src_audio_loc;        /**< Source Audio Locations. */
    uint16_t            snk_sup_context;      /**< Sink Audio Supported Audio Contexts. */
    uint16_t            src_sup_context;      /**< Source Audio Supported Audio Contexts. */
    uint16_t            snk_avail_context;    /**< Sink Audio Available Audio Contexts. */
    uint16_t            src_avail_context;    /**< Source Audio Available Audio Contexts. */
} rtk_bt_le_audio_bap_pacs_info_t;

/**
 * @typedef    rtk_bt_le_audio_cap_discovery_done_ind_t
 * @brief     Bluetooth LE audio info for cap discovery done indication. Common Audio Profile Client Discover Service Result
 */
typedef struct
{
    uint16_t conn_handle;
    bool     load_from_ftl; /**< Whether the service table is loaded from FTL. */
    bool     cas_is_found;  /**< Whether to find CAS. */
    bool     cas_inc_csis;  /**< Whether the CAS instance include the CSIS instance. */
    bool     vcs_is_found;  /**< Whether to find VCS. */
    bool     mics_is_found; /**< Whether to find MICS. */
} rtk_bt_le_audio_cap_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_codec_cap_t
 * @brief     Bluetooth LE audio codec cap.
 */
typedef struct
{
    uint16_t type_exist;
    uint16_t supported_sampling_frequencies;
    uint8_t  supported_frame_durations;
    uint8_t  audio_channel_counts;
    uint8_t  max_supported_codec_frames_per_sdu;
    uint16_t min_octets_per_codec_frame;
    uint16_t max_octets_per_codec_frame;
} rtk_bt_le_audio_codec_cap_t; 

/**
 * @typedef    rtk_bt_le_audio_bap_pacs_pac_record_t
 * @brief     Bluetooth LE audio bap pacs pac record.
 */
typedef struct
{
    uint16_t                    pac_handle;                                 /**< pac handle */
    uint8_t                     codec_id[RTK_BT_LE_AUDIO_CODEC_ID_LEN];
    rtk_bt_le_audio_codec_cap_t codec_cap;
    uint32_t                    lc3_sup_cfg_bits;
    uint16_t                    pref_audio_contexts;
    uint8_t                     metadata_length;
    uint8_t                     metadata[RTK_BT_LE_AUDIO_METADATA_MAX_LEN];
} rtk_bt_le_audio_bap_pacs_pac_record_t;

/**
 * @typedef    rtk_bt_le_audio_bis_data_path_param_t
 * @brief     Bluetooth LE audio bis setup data path parameter.
 */
typedef struct
{  
    uint8_t codec_id[RTK_BT_LE_AUDIO_CODEC_ID_LEN];         /*!< Specify the coding format used over the air. 
                                                                Octet 0: See Assigned Numbers for Coding Format.
                                                                Octets 1 to 2: Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if octet 0 is not 0xFF.
                                                                Octets 3 to 4: Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF. */
    uint32_t controller_delay;                              /*!< Controller delay in microseconds.
                                                                Range: 0x000000 to 0x3D0900 
                                                                Time range: 0 s to 4 s */
    uint8_t codec_config_len;                               /*!< Length of codec configuration */
    uint8_t *p_codec_config;                                /*!< Specify codec-specific configuration information for the specified direction*/
} rtk_bt_le_audio_bis_data_path_param_t;


/**
 * @typedef    rtk_bt_le_audio_broadcast_source_handle_t
 * @brief     Bluetooth LE audio broadcast source handle.
 */
typedef void * rtk_bt_le_audio_broadcast_source_handle_t; 

/**
 * @typedef    rtk_bt_le_audio_sync_handle_t
 * @brief     Bluetooth LE audio sync handle.
 */
typedef void * rtk_bt_le_audio_sync_handle_t; 

/**
 * @typedef    rtk_bt_le_audio_base_data_mapping_t
 * @brief     Bluetooth LE audio base data mapping.
 */
typedef void * rtk_bt_le_audio_base_data_mapping_t; 




/**
 * @typedef    rtk_bt_le_audio_pa_sync_establish_t
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
 * @typedef    rtk_bt_le_audio_pa_action_t
 * @brief     Bluetooth LE audio pa action.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_PA_IDLE         = 0x00,
    RTK_BT_LE_AUDIO_PA_SYNC         = 0x01,
    RTK_BT_LE_AUDIO_PA_TERMINATE    = 0x02,
    RTK_BT_LE_AUDIO_PA_LOST         = 0x03,
} rtk_bt_le_audio_pa_action_t;

/**
 * @typedef    rtk_bt_le_audio_pa_sync_state_t
 * @brief     Bluetooth LE audio pa sync state.
 */
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
 * @typedef    rtk_bt_le_audio_pa_sync_state_ind_t
 * @brief     Bluetooth LE audio pa sync indication infomation.
 */
typedef struct
{    
    uint8_t cause;                                                  /**< process result */     
    rtk_bt_le_audio_sync_handle_t sync_handle;  /**< Synchronization handle*/
    uint8_t sync_state;                                             /**< PA Synchronization state. @ref rtk_bt_le_audio_pa_sync_state_t*/
    uint16_t action;                                                /**< PA Synchronization action. @ref rtk_bt_le_audio_pa_action_t. */             
} rtk_bt_le_audio_pa_sync_state_ind_t;


/**
 * @typedef    rtk_bt_le_audio_bis_channel_location_t
 * @brief     Bluetooth LE audio bis and channel_location map.
 */
typedef struct
{
    uint8_t subgroup_idx;                 /**< Subgroup idx*/
    uint8_t bis_idx;                      /**< BIS index*/
    uint32_t audio_channel_allocation;      
}rtk_bt_le_audio_bis_channel_location_t;

/**
 * @typedef    rtk_bt_le_audio_pa_adv_report_ind_t
 * @brief     Bluetooth LE audio pa adv report indication.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;                          /**< Synchronization handle*/
	uint8_t sync_id;                                                    /**< Identify the synchronization with a periodic advertising train. */
	uint16_t pa_sync_handle;                                            /**< Identifying the periodic advertising train. */
	int8_t tx_power;                                                    /**< Range: -127 to +20, Units: dBm.
                                                                            0x7F: Tx Power information not available. */
	int8_t rssi;                                                        /**< Range: -127 to +20, Units: dBm.
                                                                            0x7F: RSSI is not available. */
	uint8_t cte_type;                                                   /**< @ref rtk_bt_le_pa_sync_report_cte_t */
	uint8_t data_status;                                                /**< @ref rtk_bt_le_pa_sync_report_data_status_t */
	uint8_t data_len;                                                   /**< Length of the Data field. Range: 0 to 247. */
	uint8_t *p_data;                                                    /**< Data received from a Periodic Advertising packet. Must be the last member. */
}rtk_bt_le_audio_pa_adv_report_ind_t;

/**
 * @typedef    rtk_bt_le_audio_big_sync_state_t
 * @brief     Bluetooth LE audio paramsters for big sync.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_TERMINATED = 0x00,     /**< Terminated. */
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_SYNCHRONIZING = 0x01,  /**< Start synchronizing to a BIG. A temporary state, haven't received the result.*/
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_SYNCHRONIZED = 0x02,   /**< Synchronized to a BIG. */
    RTK_BT_LE_AUDIO_BIG_SYNC_STATE_TERMINATING = 0x03,    /**< Terminate synchronization to the BIG. A temporary state, haven't received the result.*/
} rtk_bt_le_audio_big_sync_state_t;

/**
 * @typedef    rtk_bt_le_audio_big_action_t
 * @brief     Bluetooth LE audio big action.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_BIG_IDLE         = 0x00,
    RTK_BT_LE_AUDIO_BIG_SYNC         = 0x01,
    RTK_BT_LE_AUDIO_BIG_TERMINATE    = 0x02,
    RTK_BT_LE_AUDIO_BIG_LOST         = 0x03,
} rtk_bt_le_audio_big_action_t;

/**
 * @typedef    rtk_bt_le_audio_big_sync_state_ind_t
 * @brief     Bluetooth LE audio pa sync indication infomation.
 */
typedef struct
{  
    uint8_t cause;                                                      /**< process result */     
    rtk_bt_le_audio_sync_handle_t sync_handle;      /**< Synchronization handle*/
    uint8_t sync_state;                                                 /**< BIG Synchronization state. @ref rtk_bt_le_audio_big_sync_state_t*/
    uint8_t encryption;                                                 /**< Identify the encryption mode of the BIG. 0x00: Unencrypted 0x01: Encrypted */
    uint16_t action;                                                    /**< BIG Synchronization action. @ref rtk_bt_le_audio_big_action_t. */           
} rtk_bt_le_audio_big_sync_state_ind_t;

/**
 * @typedef    rtk_bt_le_audio_biginfo_t
 * @brief     Bluetooth LE audio big infomation.
 */
typedef struct
{
    uint8_t sync_id;         /**< Identify the periodic advertising train. */
    uint16_t pa_sync_handle; /**< Sync_Handle identifying the periodic advertising train. */
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
 * @typedef    rtk_bt_le_audio_pa_biginfo_ind_t
 * @brief     Bluetooth LE audio biginfo adv report indication.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;              /**< Synchronization handle*/
    rtk_bt_le_audio_biginfo_t biginfo;                                          /**< @ref rtk_bt_le_audio_biginfo_t*/
}rtk_bt_le_audio_pa_biginfo_ind_t;

/**
 * @typedef rtk_bt_le_audio_base_data_bis_info_t
 * @brief   structure of LE audio BASE data BIS info [Level3]
 */
typedef struct
{
    uint8_t         subgroup_idx;
    uint8_t         bis_index;
    uint8_t         codec_id[RTK_BT_LE_AUDIO_CODEC_ID_LEN];
    rtk_bt_le_audio_cfg_codec_t bis_codec_cfg;
} rtk_bt_le_audio_base_data_bis_info_t;

/**
 * @typedef rtk_bt_le_audio_base_data_subgroup_info_t
 * @brief   structure of LE audio BASE data subgroup info [Level2]
 */
typedef struct
{
    uint8_t                 subgroup_idx;
    uint8_t                 num_bis;  
    uint8_t                 metadata_len;
    uint32_t                bis_array;    
    uint8_t                 *p_metadata;  
    rtk_bt_le_audio_base_data_bis_info_t   *p_bis_info;        /*!< Level3 param */
} rtk_bt_le_audio_base_data_subgroup_info_t; 

/**
 * @typedef rtk_bt_le_audio_base_data_mapping_info_t
 * @brief   structure of LE audio BASE data mapping info[Level1]
 */
typedef struct
{   
    uint8_t num_bis;
    uint8_t num_subgroups;
    uint32_t presentation_delay;    
    rtk_bt_le_audio_base_data_subgroup_info_t *p_subgroup;       /*!< Level2 param */
} rtk_bt_le_audio_base_data_mapping_info_t; 

/**
 * @typedef    rtk_bt_le_audio_base_data_mapping_modify_ind_t
 * @brief     Bluetooth LE audio BASE data mapping modify indication.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;                  /**< Synchronization handle*/
    rtk_bt_le_audio_base_data_mapping_t base_data_mapping;      /**< @ref rtk_bt_le_audio_base_data_mapping_t*/
}rtk_bt_le_audio_base_data_mapping_modify_ind_t;

/**
 * @typedef    rtk_bt_le_audio_sync_info_t
 * @brief     Bluetooth LE audio sync info.
 */
typedef struct
{
    rtk_bt_le_addr_t advertiser_address;                    /**< @ref rtk_bt_le_addr_t*/
    uint8_t adv_sid;
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];
    uint8_t sync_id;
    rtk_bt_le_audio_pa_sync_state_t pa_state;               /**< @ref rtk_bt_le_audio_pa_sync_state_t*/
    uint16_t pa_interval;
    rtk_bt_le_audio_base_data_mapping_t base_data_mapping;  /**< @ref rtk_bt_le_audio_base_data_mapping_t*/
    rtk_bt_le_addr_t pa_sync_addr;                          /**< @ref rtk_bt_le_addr_t*/
    bool big_info_received;
    rtk_bt_le_audio_biginfo_t big_info;                     /**< @ref rtk_bt_le_audio_biginfo_t*/
    rtk_bt_le_audio_big_sync_state_t big_state;             /**< @ref rtk_bt_le_audio_big_sync_state_t*/
} rtk_bt_le_audio_sync_info_t;

/**
 * @typedef    rtk_bt_le_audio_big_sync_establish_t
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
 * @typedef    rtk_bt_le_audio_bis_conn_handle_info_t
 * @brief     Bluetooth LE audio bis connetion handle information.
 */
typedef struct
{
    uint8_t bis_idx;                  /**< bis index */
    uint16_t bis_conn_handle;         /**< Connection handle of a BIS. */
} rtk_bt_le_audio_bis_conn_handle_info_t; 

/**
 * @typedef    rtk_bt_le_audio_bis_info_t
 * @brief     Bluetooth LE audio bis information.
 */
typedef struct
{
    uint8_t  num_bis;                                                                     /**< bis number*/ 
    rtk_bt_le_audio_bis_conn_handle_info_t bis_conn_info[RTK_BT_LE_AUDIO_BIS_MAX_NUM];    /**< @ref rtk_bt_le_audio_bis_conn_handle_info_t*/
} rtk_bt_le_audio_bis_info_t;


/**
 * @typedef    rtk_bt_le_audio_bass_cp_op_t
 * @brief     Bluetooth LE audio Broadcast Audio Scan Control Point Opcodes.
 */
typedef enum {
    RTK_BT_LE_AUDIO_BASS_CP_REMOTE_SCAN_STOP     = 0x00,
    RTK_BT_LE_AUDIO_BASS_CP_REMOTE_SCAN_START     = 0x01,
    RTK_BT_LE_AUDIO_BASS_CP_ADD_SOURCE              = 0x02,
    RTK_BT_LE_AUDIO_BASS_CP_MODIFY_SOURCE           = 0x03,
    RTK_BT_LE_AUDIO_BASS_CP_SET_BROADCAST_CODE      = 0x04,
    RTK_BT_LE_AUDIO_BASS_CP_REMOVE_SOURCE           = 0x05,
    RTK_BT_LE_AUDIO_BASS_CP_MAX
} rtk_bt_le_audio_bass_cp_op_t;

/**
 * @typedef    rtk_bt_le_audio_bass_brs_pa_sync_state_t
 * @brief     Bluetooth LE audio broadcast receive state PA_Sync_State field.
 */
typedef enum {
    RTK_BT_LE_AUDIO_BASS_BRS_PA_SYNC_STATE_NOT_SYNC     = 0x00,      /**< Not synchronized to PA */
    RTK_BT_LE_AUDIO_BASS_BRS_PA_SYNC_STATE_SYNC_INFO_REQ= 0x01,      /**< SyncInfo Request */
    RTK_BT_LE_AUDIO_BASS_BRS_PA_SYNC_STATE_SYNC         = 0x02,      /**< Synchronized to PA */
    RTK_BT_LE_AUDIO_BASS_BRS_PA_SYNC_STATE_FAILED       = 0x03,      /**< Failed to synchronize to PA */
    RTK_BT_LE_AUDIO_BASS_BRS_PA_SYNC_STATE_NO_PAST      = 0x04       /**< No PAST */
} rtk_bt_le_audio_bass_brs_pa_sync_state_t;


/**
 * @typedef    rtk_bt_le_audio_bass_brs_big_encrption_t
 * @brief     Bluetooth LE audio broadcast receive state BIG_Encryption field.
 */
typedef enum {
    RTK_BT_LE_AUDIO_BASS_BRS_BIG_NOT_ENCRYPTED           = 0x00,      /**< Not encrypted */
    RTK_BT_LE_AUDIO_BASS_BRS_BIG_BROADCAST_CODE_REQUIRED = 0x01,      /**< Broadcast_Code required */
    RTK_BT_LE_AUDIO_BASS_BRS_BIG_DECRYPTING              = 0x02,      /**< Decrypting */
    RTK_BT_LE_AUDIO_BASS_BRS_BIG_BAD_CODE                = 0x03       /**< Bad_Code (incorrect encryption key) */
} rtk_bt_le_audio_bass_brs_big_encrption_t;

/**
 * @typedef    rtk_bt_le_audio_bass_cp_pa_sync_t
 * @brief     Bluetooth LE audio broadcast audio scan control point PA_Sync parameter.
 */ 
typedef enum
{
    RTK_BT_LE_AUDIO_BASS_PA_NOT_SYNC     = 0x00,      /**< Do not synchronize to PA */
    RTK_BT_LE_AUDIO_BASS_PA_SYNC_PAST    = 0x01,      /**< Synchronize to PA: PAST available */
    RTK_BT_LE_AUDIO_BASS_PA_SYNC_NO_PAST = 0x02      /**< Synchronize to PA: PAST not available */
} rtk_bt_le_audio_bass_cp_pa_sync_t; 

/**
 * @typedef    rtk_bt_le_audio_bass_past_srv_data_t
 * @brief     Bluetooth LE audio BASS PAST Service Data.
 */ 
typedef struct
{
    uint8_t adv_a_match_ext_adv; /**< AdvA in PAST matches AdvA in ADB_EXT_IND, 0b0=Yes, 0b1=No/Do't know */
    uint8_t adv_a_match_src;     /**< AdvA in PAST matches Source_Address, 0b0=Yes, 0b1=No/Do't know */
    uint8_t source_id;
} rtk_bt_le_audio_bass_past_srv_data_t;

/**
 * @typedef    rtk_bt_le_audio_bass_cp_bis_info_t
 * @brief     Bluetooth LE audio bass broadcast receive state characteristic subgroup info.
 */
typedef struct {
    uint32_t bis_sync_state;                    /**< BIS_Sync_State for the subgroup */
    uint8_t metadata_len;
    uint8_t *p_metadata;
} rtk_bt_le_audio_bass_cp_bis_info_t; 

/**
 * @typedef    rtk_bt_le_audio_bass_cp_add_source_param_t
 * @brief     Bluetooth LE audio param for Broadcast Audio Scan Control Point add source.
 */
typedef struct {
    rtk_bt_le_addr_t adv_addr;
    uint8_t adv_sid;
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];
    uint8_t pa_sync;                                                                /**< PA_Sync parameter @ref rtk_bt_le_audio_bass_cp_pa_sync_t */
    uint16_t pa_interval;
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    rtk_bt_le_audio_bass_cp_bis_info_t *p_cp_bis_info;                              /**< @ref rtk_bt_le_audio_bass_cp_bis_info_t */
} rtk_bt_le_audio_bass_cp_add_source_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_cp_modify_source_param_t
 * @brief     Bluetooth LE audio param for Broadcast Audio Scan Control Point modify source.
 */
typedef struct {
    uint8_t source_id;
    uint8_t pa_sync;                                                                /**< PA_Sync parameter @ref rtk_bt_le_audio_bass_cp_pa_sync_t */
    uint16_t pa_interval;
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    rtk_bt_le_audio_bass_cp_bis_info_t *p_cp_bis_info;                              /**< @ref rtk_bt_le_audio_bass_cp_bis_info_t */
} rtk_bt_le_audio_bass_cp_modify_source_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_cp_remove_source_param_t
 * @brief     Bluetooth LE audio param for Broadcast Audio Scan Control Point remove source.
 */
typedef struct {
    uint8_t source_id;
} rtk_bt_le_audio_bass_cp_remove_source_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_cp_set_broadcast_code_param_t
 * @brief     Bluetooth LE audio param for Broadcast Audio Scan Control Point set broadcast code.
 */
typedef struct {
    uint8_t source_id;
    uint8_t broadcast_code[RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN];
} rtk_bt_le_audio_bass_cp_set_broadcast_code_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_client_write_cp_op_param_t
 * @brief     Bluetooth LE audio bass client write cp op param.
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t  is_req;
    uint8_t  opcode;                                                                /**< @ref rtk_bt_le_audio_bass_cp_op_t */
    union {
        rtk_bt_le_audio_bass_cp_add_source_param_t add_source_param;          /**< @ref rtk_bt_le_audio_bass_cp_add_source_param_t */
        rtk_bt_le_audio_bass_cp_modify_source_param_t modify_source_param;    /**< @ref rtk_bt_le_audio_bass_cp_modify_source_param_t */
        rtk_bt_le_audio_bass_cp_remove_source_param_t remove_source_param;    /**< @ref rtk_bt_le_audio_bass_cp_remove_source_param_t */
        rtk_bt_le_audio_bass_cp_set_broadcast_code_param_t set_broadcast_code_param;/**< @ref rtk_bt_le_audio_bass_cp_set_broadcast_code_param_t */
    } op_param;
} rtk_bt_le_audio_bass_client_write_cp_op_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_past_type_t
 * @brief     Bluetooth LE audio BASS PAST TYPE.
 */ 
typedef enum
{
    RTK_BT_LE_AUDIO_BASS_PAST_BY_LOCAL_SRC     = 0x00,     /**< Initiating PAST for local PA */
    RTK_BT_LE_AUDIO_BASS_PAST_BY_REMOTE_SRC    = 0x01      /**< Initiating PAST for remote PA */
} rtk_bt_le_audio_bass_past_type_t; 

/**
 * @typedef    rtk_bt_le_audio_bass_client_past_param_t
 * @brief     Bluetooth LE audio bass client past param.
 */
typedef struct {
    uint8_t past_type;                                                  /**< @ref rtk_bt_le_audio_bass_past_type_t */
    void * handle;                                                      /**< @ref rtk_bt_le_audio_broadcast_source_handle_t when past_type is RTK_BT_LE_AUDIO_BASS_PAST_BY_LOCAL_SRC; 
                                                                             @ref rtk_bt_le_audio_sync_handle_t when past_type is RTK_BT_LE_AUDIO_BASS_PAST_BY_REMOTE_SRC;  */
    uint16_t conn_handle;                                                            
    rtk_bt_le_audio_bass_past_srv_data_t srv_data;                      /**< @ref rtk_bt_le_audio_bass_past_srv_data_t */
} rtk_bt_le_audio_bass_client_past_param_t;


/**
 * @typedef    rtk_bt_le_audio_bass_brs_data_t
 * @brief     Bluetooth LE audio bass broadcast receive state characteristic data.
 */
typedef struct {
    bool brs_is_used;   
    uint8_t source_id;                                          /**< Broadcast Receive State Field */
    rtk_bt_le_addr_t source_address;
    uint8_t source_adv_sid;
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];
    uint8_t pa_sync_state;                                      /**< @ref rtk_bt_le_audio_pa_sync_state_t */
    uint8_t big_encryption;                                     /**< @ref rtk_bt_le_audio_bass_brs_big_encrption_t */
    uint8_t bad_code[RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN];       
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    rtk_bt_le_audio_bass_cp_bis_info_t *p_cp_bis_info; /**< @ref rtk_bt_le_audio_bass_cp_bis_info_t */
} rtk_bt_le_audio_bass_brs_data_t;

/**
 * @typedef    rtk_bt_le_audio_bass_client_sync_info_req_ind_t
 * @brief     Bluetooth LE audio bass client sync info indication parameter.
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t char_instance_id;
    rtk_bt_le_audio_bass_brs_data_t *p_brs_data; /**< @ref rtk_bt_le_audio_bass_brs_data_t */
} rtk_bt_le_audio_bass_client_sync_info_req_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_client_brs_data_ind_t
 * @brief     Bluetooth LE audio bass client brs data indication parameter.
 */
typedef struct {
    uint16_t conn_handle;
    bool notify;
    uint16_t read_cause;
    uint8_t char_instance_id;
    rtk_bt_le_audio_bass_brs_data_t *p_brs_data; /**< @ref rtk_bt_le_audio_bass_brs_data_t */
} rtk_bt_le_audio_bass_client_brs_data_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_pa_sync_param_t
 * @brief     Bluetooth LE audio bass pa sync param.
 */
typedef struct
{                       
    uint8_t pa_sync_options;                   /**< @ref rtk_bt_le_pa_sync_create_opt_t */
    uint16_t pa_sync_skip;                     /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                                     Range: 0x0000 to 0x01F3 */
    uint16_t pa_sync_timeout;                  /**< Synchronization timeout for the periodic advertising train.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
    uint16_t past_timeout;                     /**< Synchronization timeout for PAST.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
}rtk_bt_le_audio_bass_pa_sync_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_get_pa_sync_param_ind_t
 * @brief     Bluetooth LE audio bass set pa sync parameter.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;              /**< Synchronization handle*/ 
    uint8_t source_id;     
    bool    is_past; 
    uint16_t pa_interval;                          
    rtk_bt_le_audio_bass_pa_sync_param_t *p_pa_sync_param;  /**< @ref rtk_bt_le_audio_bass_pa_sync_param_t*/
}rtk_bt_le_audio_bass_get_pa_sync_param_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_big_sync_param_t
 * @brief     Bluetooth LE audio bass big sync param.
 */
typedef struct
{                       
    uint8_t big_mse;                       /**< The maximum number of subevents that a Controller should use to receive data
                                                payloads in each interval for a BIS.
                                                0x00: The Controller can schedule reception of any number of subevents up
                                                      to NSE.
                                                0x01 to 0x1F: Maximum number of subevents that should be used to receive data
                                                              payloads in each BIS event      */
    uint16_t big_sync_timeout;             /**< Specify the maximum permitted time between successful receptions of BIS PDUs.
                                                Range: 0x000A to 0x4000
                                                Time = N*10 ms
                                                Time Range: 100 ms to 163.84 s */
}rtk_bt_le_audio_bass_big_sync_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_get_big_sync_param_ind_t
 * @brief     Bluetooth LE audio bass set big sync parameter.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;                  /**< Synchronization handle*/
    uint8_t source_id;                              
    rtk_bt_le_audio_bass_big_sync_param_t *p_big_sync_param;    /**< @ref rtk_bt_le_audio_bass_big_sync_param_t*/
}rtk_bt_le_audio_bass_get_big_sync_param_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_get_broadcast_code_ind_t
 * @brief     Bluetooth LE audio bass set broadcast code.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;  /**< Synchronization handle*/ 
    uint8_t source_id;                              
    uint8_t *p_broadcast_code;                  /**< Used to generate the session key to decrypt payloads of an encrypted BIS. length is RTK_BT_LE_AUDIO_BROADCAST_CODE_LEN */
}rtk_bt_le_audio_bass_get_broadcast_code_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t
 * @brief     Bluetooth LE audio bass get refer bis sync.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;   /**< Synchronization handle*/
    uint8_t source_id;   
    uint32_t *p_support_bis_array;
}rtk_bt_le_audio_bass_get_prefer_bis_sync_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_brs_modify_ind_t
 * @brief     Bluetooth LE audio bass brs data indication parameter
 */
typedef struct {
    rtk_bt_le_audio_sync_handle_t sync_handle;   /**< Synchronization handle*/
    uint8_t source_id;   
    rtk_bt_le_audio_bass_brs_data_t *p_brs_data; /**< @ref rtk_bt_le_audio_bass_brs_data_t */
} rtk_bt_le_audio_bass_brs_modify_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_cp_ind_t
 * @brief     Bluetooth LE audio bass control point characteristic data indication parameter
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t  opcode;                                                                /**< @ref rtk_bt_le_audio_bass_cp_op_t */
    union {
        rtk_bt_le_audio_bass_cp_add_source_param_t add_source_param;          /**< @ref rtk_bt_le_audio_bass_cp_add_source_param_t */
        rtk_bt_le_audio_bass_cp_modify_source_param_t modify_source_param;    /**< @ref rtk_bt_le_audio_bass_cp_modify_source_param_t */
        rtk_bt_le_audio_bass_cp_remove_source_param_t remove_source_param;    /**< @ref rtk_bt_le_audio_bass_cp_remove_source_param_t */
        rtk_bt_le_audio_bass_cp_set_broadcast_code_param_t set_broadcast_code_param;/**< @ref rtk_bt_le_audio_bass_cp_set_broadcast_code_param_t */
    } op_param;
} rtk_bt_le_audio_bass_cp_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bass_ba_add_source_ind_t
 * @brief     Bluetooth LE audio bass add source indication parameter
 */
typedef struct {
    rtk_bt_le_audio_sync_handle_t sync_handle;   /**< Synchronization handle*/
    uint8_t source_id;  
} rtk_bt_le_audio_bass_ba_add_source_ind_t;

//#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_bis_param_t
 * @brief     Bluetooth LE audio broadcast source bis param(BASE Level3).
 */
typedef struct
{
    uint8_t  codec_cfg_l3_len;
    uint8_t  *p_codec_cfg_l3;
    uint8_t  *bis_idx;          /**< returned bis index */
} rtk_bt_le_audio_broadcast_source_bis_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_subgroup_param_t
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
 * @typedef    rtk_bt_le_audio_broadcast_source_group_param_t
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
 * @typedef    rtk_bt_le_audio_broadcast_source_bis_ipc_param_t
 * @brief     Bluetooth LE audio broadcast source bis ipc param(BASE Level3).
 */
typedef struct
{
    uint8_t  bis_idx;          /**< returned bis index */
} rtk_bt_le_audio_broadcast_source_bis_ipc_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t
 * @brief     Bluetooth LE audio broadcast source subgroup ipc param(BASE Level2).
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_bis_ipc_param_t bis[RTK_BT_LE_AUDIO_BROADCASTER_BIS_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_bis_ipc_param_t*/
    uint8_t subgroup_idx;
} rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_group_ipc_param_t
 * @brief     Bluetooth LE audio broadcast source group ipc parameter (BASE Level1).
 */
typedef struct
{       
    rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t subgroup[RTK_BT_LE_AUDIO_BROADCASTER_SUBGROUP_MAX_NUM]; /**<@ ref rtk_bt_le_audio_broadcast_source_subgroup_ipc_param_t*/  
    uint8_t group_idx;                                /**< Returned broadcast source group index*/ 
} rtk_bt_le_audio_broadcast_source_group_ipc_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_create_param_t
 * @brief     Bluetooth LE audio broadcast source create parameter.
 */
typedef struct
{ 
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;                                      /**< Broadcast source handle*/
    rtk_bt_le_audio_broadcast_source_group_ipc_param_t group[RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM];    /**<@ ref rtk_bt_le_audio_broadcast_source_group_ipc_param_t*/             
} rtk_bt_le_audio_broadcast_source_create_ipc_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_state_t
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
 * @typedef    rtk_bt_le_audio_broadcast_source_info_t
 * @brief     Bluetooth LE audio broadcast source infomation.
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_state_t state;         /**< broadcast source state. @ref rtk_bt_le_audio_broadcast_source_state_t */
    uint8_t adv_sid;
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];
    uint8_t ext_adv_state;              /**< ext adv state. @ref rtk_bt_le_adv_state */
    uint8_t pa_state;                   /**< pa state. @ref rtk_bt_le_pa_state_t */
    uint8_t big_state;                  /**< big state. @ref rtk_bt_le_iso_big_broadcaster_sync_state_t*/
    uint8_t adv_handle;
    uint8_t big_handle;
} rtk_bt_le_audio_broadcast_source_info_t;
                            
/**
 * @typedef    rtk_bt_le_audio_broadcast_source_config_t
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
 * @typedef    rtk_bt_le_audio_broadcast_source_create_big_t
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
 * @typedef    rtk_bt_le_audio_broadcast_source_state_ind_t
 * @brief     Bluetooth LE audio broadcast source state indication.
 */
typedef struct
{   
    uint8_t cause;                                                      /**< process result */  
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< Broadcast source handle*/
    uint8_t broadcast_source_state;                                     /**< Broadcast source state. @ref rtk_bt_le_audio_broadcast_source_state_t*/          
} rtk_bt_le_audio_broadcast_source_state_ind_t;

//#endif

/**
 * @typedef    rtk_bt_le_audio_play_mode_t
 * @brief     Bluetooth LE AUDIO play mode.
 */
typedef enum {
    RTK_BT_LE_AUDIO_PLAY_MODE_UNKNOWN = 0,
    RTK_BT_LE_AUDIO_PLAY_MODE_MEDIA,				
	RTK_BT_LE_AUDIO_PLAY_MODE_CONVERSATION,			
}rtk_bt_le_audio_play_mode_t;

/**
 * @typedef    rtk_bt_le_audio_gap_conn_state_t
 * @brief     Bluetooth LE audio group device connection states.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_GAP_CONN_STATE_DISCONNECTED, //!< Disconnected.
    RTK_BT_LE_AUDIO_GAP_CONN_STATE_CONNECTING,   //!< Connecting.
    RTK_BT_LE_AUDIO_GAP_CONN_STATE_CONNECTED,    //!< Connected.
    RTK_BT_LE_AUDIO_GAP_CONN_STATE_DISCONNECTING //!< Disconnecting.
} rtk_bt_le_audio_gap_conn_state_t;

/**
 * @typedef    rtk_bt_le_audio_stream_state_t
 * @brief     Bluetooth LE audio stream state.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_STREAM_STATE_IDLE               = 0x00,
    RTK_BT_LE_AUDIO_STREAM_STATE_IDLE_CONFIGURED    = 0x01,
    RTK_BT_LE_AUDIO_STREAM_STATE_CONFIGURED         = 0x02,
    RTK_BT_LE_AUDIO_STREAM_STATE_STARTING           = 0x03, 
    RTK_BT_LE_AUDIO_STREAM_STATE_STREAMING          = 0x04, 
    RTK_BT_LE_AUDIO_STREAM_STATE_PARTIAL_STREAMING  = 0x05, 
    RTK_BT_LE_AUDIO_STREAM_STATE_STOPPING           = 0x06,
    RTK_BT_LE_AUDIO_STREAM_STATE_RELEASING          = 0x07,
} rtk_bt_le_audio_stream_state_t;

/**
 * @typedef    rtk_bt_le_audio_group_dev_msg_type_t
 * @brief     Bluetooth LE audio group device message type.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_GROUP_MSG_DEV_CONN                = 0x20,
    RTK_BT_LE_AUDIO_GROUP_MSG_DEV_DISCONN             = 0x21,
    RTK_BT_LE_AUDIO_GROUP_MSG_DEV_BOND_REMOVE         = 0x22,
    RTK_BT_LE_AUDIO_GROUP_MSG_DEV_BOND_CLEAR          = 0x23,
    RTK_BT_LE_AUDIO_GROUP_MSG_DEV_EMPTY               = 0x24, 
} rtk_bt_le_audio_group_dev_msg_type_t;

/**
 * @typedef    rtk_bt_le_audio_group_device_info_t
 * @brief     Bluetooth LE audio group device info.
 */
typedef struct
{
    bool                   is_used;
    uint8_t                conn_state;                      /**< conn state. @ref rtk_bt_le_audio_gap_conn_state_t */
    uint16_t               conn_handle;                     /**< connection handle */
    rtk_bt_le_addr_t       addr;                            /**< device addr. @ref rtk_bt_le_addr_t */
    rtk_bt_le_audio_device_handle_t device_handle;          /**< device handle. @ref rtk_bt_le_audio_device_handle_t */ 
} rtk_bt_le_audio_group_device_info_t;

/**
 * @typedef    rtk_bt_le_audio_bap_state_ind_t
 * @brief     Bluetooth LE audio bap state indication.
 */
typedef struct
{
    uint16_t cause;                                                 /**< process result */ 
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;  /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    uint8_t curr_action;                                            /**< current bap action. @ref rtk_bt_le_audio_bap_unicast_action_t  */
    uint8_t stream_state;                                           /**< stream state. @ref rtk_bt_le_audio_stream_state_t */
    uint8_t result;                                                 /**< result. @ref rtk_bt_le_audio_bap_unicast_result_t */    
} rtk_bt_le_audio_bap_state_ind_t;

/**
 * @typedef    rtk_bt_le_audio_group_dev_msg_ind_t
 * @brief     Bluetooth LE audio group msg device indication.
 */
typedef struct
{
    uint16_t cause;       
    rtk_bt_le_audio_group_dev_msg_type_t msg_type;                  /**< msg type. @ref rtk_bt_le_audio_group_dev_msg_type_t  */                                          /**< process result */ 
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                  /**< device handle. @ref rtk_bt_le_audio_device_handle_t */  
} rtk_bt_le_audio_group_dev_msg_ind_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_session_qos_t
 * @brief     Bluetooth LE audio unicast session qos.
 */
typedef struct
{
    uint8_t  sca;
    uint8_t  packing;
    uint8_t  framing;
    uint32_t sdu_interval_m_s;/*3bytes*/
    uint32_t sdu_interval_s_m;/*3bytes*/
    uint16_t latency_m_s;
    uint16_t latency_s_m;
    uint32_t sink_presentation_delay;
    uint32_t source_presentation_delay;
} rtk_bt_le_audio_unicast_session_qos_t;

/**
 * @typedef    rtk_bt_le_audio_bap_start_qos_cfg_ind_t
 * @brief     Bluetooth LE audio bap start qos config indication.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                     /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    uint32_t sink_preferred_presentation_delay_min;
    uint32_t sink_preferred_presentation_delay_max;    
    uint32_t sink_presentation_delay_min;                            /**< sink presentation delay min */
    uint32_t sink_presentation_delay_max;                            /**< sink presentation delay max */
    uint32_t source_preferred_presentation_delay_min;
    uint32_t source_preferred_presentation_delay_max;    
    uint32_t source_presentation_delay_min;                          /**< source presentation delay min */ 
    uint32_t source_presentation_delay_max;                          /**< source presentation delay max */   
    uint16_t sink_transport_latency_max;                             /**< sink transport latency max */
    uint16_t source_transport_latency_max;                           /**< source transport latency max */
    rtk_bt_le_audio_unicast_session_qos_t *p_session_qos;            /**< stream session qos config. @ref rtk_bt_le_audio_unicast_session_qos_t */
} rtk_bt_le_audio_bap_start_qos_cfg_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bap_start_metadata_cfg_ind_t
 * @brief     Bluetooth LE audio bap start metadata config indication.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                     /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                   /**< device handle. @ref rtk_bt_le_audio_device_handle_t */
    uint8_t ase_id;
    uint8_t *p_metadata_len;
    uint8_t *p_metadata;
} rtk_bt_le_audio_bap_start_metadata_cfg_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bap_setup_data_path_ind_t
 * @brief     Bluetooth LE audio bap setup data path indication.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                     /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                   /**< device handle. @ref rtk_bt_le_audio_device_handle_t */
    uint8_t ase_id;
    uint8_t path_direction;                                          /**< path direction. @ref rtk_bt_le_audio_iso_data_path_direction_t */
    uint16_t cis_conn_handle;                                        /**< connection handle of the CIS*/
    rtk_bt_le_audio_cfg_codec_t codec_parsed_data;                   /**< @ref rtk_bt_le_audio_cfg_codec_t */
} rtk_bt_le_audio_bap_setup_data_path_ind_t;

/**
 * @typedef    rtk_bt_le_audio_bap_remove_data_path_ind_t
 * @brief     Bluetooth LE audio bap remove data path indication.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                     /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                   /**< device handle. @ref rtk_bt_le_audio_device_handle_t */
    uint8_t ase_id;
    uint8_t path_direction;                                          /**< path direction. @ref rtk_bt_le_audio_iso_data_path_direction_t */
    uint16_t cis_conn_handle;                                        /**< connection handle of the CIS*/
    uint16_t cause;
} rtk_bt_le_audio_bap_remove_data_path_ind_t;

//#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_bap_unicast_action_t
 * @brief     Bluetooth LE audio bap unicast action.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_IDLE             = 0x00,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_START            = 0x01,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_STOP             = 0x02,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_RELEASE          = 0x03,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_UPDATE           = 0x04,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_REMOVE_CFG       = 0x05,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_REMOVE_SESSION   = 0x06,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_SERVER_STOP      = 0x07,
    RTK_BT_LE_AUDIO_BAP_UNICAST_ACTION_SERVER_RELEASE   = 0x08,
} rtk_bt_le_audio_bap_unicast_action_t;

/**
 * @typedef    rtk_bt_le_audio_bap_unicast_result_t
 * @brief     Bluetooth LE audio bap unicast result.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_SUCCESS,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_FAILED,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_CIG_SET_ERR,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_CIS_ERR,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_ASE_CP_ERR,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_CIS_DISCONN,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_ASE_INVALID_STATE,
    RTK_BT_LE_AUDIO_BAP_UNICAST_RESULT_ACL_DISCONN,
} rtk_bt_le_audio_bap_unicast_result_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_cfg_type_t
 * @brief     Bluetooth LE audio unicast cfg type. see in BAP P50.
 */
typedef enum
{
    RTK_BT_LE_UNICAST_AUDIO_CFG_UNKNOWN,
    RTK_BT_LE_UNICAST_AUDIO_CFG_1,                      /* ---->  */
    RTK_BT_LE_UNICAST_AUDIO_CFG_2,                      /* <----  */
    RTK_BT_LE_UNICAST_AUDIO_CFG_3,                      /* <---->  */
    RTK_BT_LE_UNICAST_AUDIO_CFG_4,                      /* ---->>  */
    RTK_BT_LE_UNICAST_AUDIO_CFG_5,                      /* <---->>  */
    RTK_BT_LE_UNICAST_AUDIO_CFG_6_I,                    
    RTK_BT_LE_UNICAST_AUDIO_CFG_6_II,                   
    RTK_BT_LE_UNICAST_AUDIO_CFG_7_I,
    RTK_BT_LE_UNICAST_AUDIO_CFG_7_II,
    RTK_BT_LE_UNICAST_AUDIO_CFG_8_I,
    RTK_BT_LE_UNICAST_AUDIO_CFG_8_II,
    RTK_BT_LE_UNICAST_AUDIO_CFG_9_I,
    RTK_BT_LE_UNICAST_AUDIO_CFG_9_II,
    RTK_BT_LE_UNICAST_AUDIO_CFG_10,
    RTK_BT_LE_UNICAST_AUDIO_CFG_11_I,
    RTK_BT_LE_UNICAST_AUDIO_CFG_11_II,
    RTK_BT_LE_UNICAST_AUDIO_CFG_MAX
} rtk_bt_le_audio_unicast_cfg_type_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_cfg_type_bit_mask_t
 * @brief     Bluetooth LE audio codec cfg type bit mask.
 */
typedef enum {
    RTK_BT_LE_UNICAST_AUDIO_CFG_1_BIT  =  (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_1),
    RTK_BT_LE_UNICAST_AUDIO_CFG_2_BIT  =  (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_2),
    RTK_BT_LE_UNICAST_AUDIO_CFG_3_BIT  =   (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_3),
    RTK_BT_LE_UNICAST_AUDIO_CFG_4_BIT  =   (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_4),
    RTK_BT_LE_UNICAST_AUDIO_CFG_5_BIT  =   (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_5),
    RTK_BT_LE_UNICAST_AUDIO_CFG_6_I_BIT =  (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_6_I),
    RTK_BT_LE_UNICAST_AUDIO_CFG_6_II_BIT =  (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_6_II),
    RTK_BT_LE_UNICAST_AUDIO_CFG_7_I_BIT  = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_7_I),
    RTK_BT_LE_UNICAST_AUDIO_CFG_7_II_BIT = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_7_II),
    RTK_BT_LE_UNICAST_AUDIO_CFG_8_I_BIT  = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_8_I),
    RTK_BT_LE_UNICAST_AUDIO_CFG_8_II_BIT = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_8_II),
    RTK_BT_LE_UNICAST_AUDIO_CFG_9_I_BIT  = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_9_I),
    RTK_BT_LE_UNICAST_AUDIO_CFG_9_II_BIT = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_9_II),
    RTK_BT_LE_UNICAST_AUDIO_CFG_10_BIT   = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_10),
    RTK_BT_LE_UNICAST_AUDIO_CFG_11_I_BIT = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_11_I),
    RTK_BT_LE_UNICAST_AUDIO_CFG_11_II_BIT = (1 << RTK_BT_LE_UNICAST_AUDIO_CFG_11_II),
    RTK_BT_LE_UNICAST_AUDIO_CFG_MASK  =   0x1FFFE
} rtk_bt_le_audio_unicast_cfg_type_bit_mask_t;   

/**
 * @typedef    rtk_bt_le_audio_codec_cfg_item_t
 * @brief     Bluetooth LE audio codec cfg item. see in BAP P86 and P111.
 */
typedef enum {
    RTK_BT_LE_CODEC_CFG_ITEM_8_1 = 0,
    RTK_BT_LE_CODEC_CFG_ITEM_8_2 = 1,
    RTK_BT_LE_CODEC_CFG_ITEM_16_1 = 2,
    RTK_BT_LE_CODEC_CFG_ITEM_16_2 = 3,
    RTK_BT_LE_CODEC_CFG_ITEM_24_1 = 4,
    RTK_BT_LE_CODEC_CFG_ITEM_24_2 = 5,
    RTK_BT_LE_CODEC_CFG_ITEM_32_1 = 6,
    RTK_BT_LE_CODEC_CFG_ITEM_32_2 = 7,
    RTK_BT_LE_CODEC_CFG_ITEM_441_1 = 8,
    RTK_BT_LE_CODEC_CFG_ITEM_441_2 = 9,
    RTK_BT_LE_CODEC_CFG_ITEM_48_1 = 10,
    RTK_BT_LE_CODEC_CFG_ITEM_48_2 = 11,
    RTK_BT_LE_CODEC_CFG_ITEM_48_3 = 12,
    RTK_BT_LE_CODEC_CFG_ITEM_48_4 = 13,
    RTK_BT_LE_CODEC_CFG_ITEM_48_5 = 14,
    RTK_BT_LE_CODEC_CFG_ITEM_48_6 = 15,
    RTK_BT_LE_CODEC_CFG_ITEM_LC3_MAX,
} rtk_bt_le_audio_codec_cfg_item_t;

/**
 * @typedef    rtk_bt_le_audio_codec_cfg_item_bit_mask_t
 * @brief     Bluetooth LE audio codec cfg item bit mask.
 */
typedef enum {
    RTK_BT_LE_CODEC_CFG_ITEM_8_1_BIT  = (1 << RTK_BT_LE_CODEC_CFG_ITEM_8_1),
    RTK_BT_LE_CODEC_CFG_ITEM_8_2_BIT  = (1 << RTK_BT_LE_CODEC_CFG_ITEM_8_2),
    RTK_BT_LE_CODEC_CFG_ITEM_16_1_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_16_1),
    RTK_BT_LE_CODEC_CFG_ITEM_16_2_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_16_2),
    RTK_BT_LE_CODEC_CFG_ITEM_24_1_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_24_1),
    RTK_BT_LE_CODEC_CFG_ITEM_24_2_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_24_2),
    RTK_BT_LE_CODEC_CFG_ITEM_32_1_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_32_1),
    RTK_BT_LE_CODEC_CFG_ITEM_32_2_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_32_2),
    RTK_BT_LE_CODEC_CFG_ITEM_441_1_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_441_1),
    RTK_BT_LE_CODEC_CFG_ITEM_441_2_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_441_2),
    RTK_BT_LE_CODEC_CFG_ITEM_48_1_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_48_1),
    RTK_BT_LE_CODEC_CFG_ITEM_48_2_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_48_2),
    RTK_BT_LE_CODEC_CFG_ITEM_48_3_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_48_3),
    RTK_BT_LE_CODEC_CFG_ITEM_48_4_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_48_4),
    RTK_BT_LE_CODEC_CFG_ITEM_48_5_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_48_5),
    RTK_BT_LE_CODEC_CFG_ITEM_48_6_BIT = (1 << RTK_BT_LE_CODEC_CFG_ITEM_48_6),
} rtk_bt_le_audio_codec_cfg_item_bit_mask_t;    

/**
 * @typedef    rtk_bt_le_audio_qos_cfg_type_t
 * @brief     Bluetooth LE audio qos cfg type item. see in BAP P86 and P111.
 */
typedef enum {
    RTK_BT_LE_QOS_CFG_CIS_LOW_LATENCY = 0,
    RTK_BT_LE_QOS_CFG_CIS_HIG_RELIABILITY = 1,
    RTK_BT_LE_QOS_CFG_BIS_LOW_LATENCY = 2,
    RTK_BT_LE_QOS_CFG_BIS_HIG_RELIABILITY = 3,
    RTK_BT_LE_QOS_CFG_MAX,
} rtk_bt_le_audio_qos_cfg_type_t;

/**
 * @typedef    rtk_bt_le_audio_qos_cfg_preferred_t
 * @brief     Bluetooth LE audio qos cfg preferred item. see in BAP P86 and P111.
 */
typedef struct
{
    uint32_t sdu_interval;
    uint8_t  framing;
    uint16_t max_sdu;
    uint8_t  retransmission_number;
    uint16_t max_transport_latency;
    uint32_t presentation_delay;
} rtk_bt_le_audio_qos_cfg_preferred_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_ase_info_t
 * @brief     Bluetooth LE audio unicast ase info.
 */
typedef struct
{
    uint8_t                             ase_id;
    uint8_t                             cfg_idx;
    rtk_bt_le_audio_ascs_ase_state_t    ase_state;
    uint8_t                             direction;
    uint8_t                             channel_num;
} rtk_bt_le_audio_unicast_ase_info_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_device_info_t
 * @brief     Bluetooth LE audio unicast device info.
 */
typedef struct
{
    rtk_bt_le_audio_device_handle_t     dev_handle;
    rtk_bt_le_audio_gap_conn_state_t    conn_state;
    uint8_t                             ase_num;
    rtk_bt_le_audio_unicast_ase_info_t  ase_info[RTK_BT_LE_AUDIO_STREAM_SRV_ASE_MAX];
} rtk_bt_le_audio_unicast_device_info_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_session_info_t
 * @brief     Bluetooth LE audio unicast session info.
 */
typedef struct
{
    uint8_t                                state;               /**< stream session state. @ref rtk_bt_le_audio_stream_state_t */ 
    rtk_bt_le_audio_unicast_cfg_type_t     cfg_type;
    uint8_t                                conn_dev_num;
    uint8_t                                dev_num;
    rtk_bt_le_audio_unicast_device_info_t  dev_info[RTK_BT_LE_AUDIO_UNICAST_SERVERS_MAX_NUM];
} rtk_bt_le_audio_unicast_session_info_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_cis_info_t
 * @brief     Bluetooth LE audio unicast cis info.
 */
typedef struct
{
    uint8_t     cis_id;
    uint16_t    cis_conn_handle;
    uint8_t     data_path_flags;
    uint8_t     sink_ase_id;
    rtk_bt_le_audio_ascs_ase_state_t sink_ase_state;
    uint8_t     source_ase_id;
    rtk_bt_le_audio_ascs_ase_state_t source_ase_state;
} rtk_bt_le_audio_unicast_cis_info_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_cis_infos_t
 * @brief     Bluetooth LE audio unicast cis infos.
 */
typedef struct
{
    uint8_t cis_num;
    rtk_bt_le_audio_unicast_cis_info_t  cis_info[RTK_BT_LE_AUDIO_STREAM_SRV_CIS_MAX];   /**< cis info table. @ref rtk_bt_le_audio_unicast_cis_info_t */
} rtk_bt_le_audio_unicast_cis_infos_t;


/**
 * @typedef    rtk_bt_le_audio_unicast_aes_codec_t
 * @brief     Bluetooth LE audio unicast aes codec.
 */
typedef struct
{
    uint8_t                                     codec_id[RTK_BT_LE_AUDIO_CODEC_ID_LEN];
    rtk_bt_le_audio_ascs_ase_target_latency_t   target_latency;          /**< target latency. @ref rtk_bt_le_audio_ascs_ase_target_latency_t */
    rtk_bt_le_phy_type_t                        target_phy;              /**< target phy. @ref rtk_bt_le_phy_type_t */
    rtk_bt_le_audio_cfg_codec_t                 codec_cfg;               /**< codec cfg. @ref rtk_bt_le_audio_cfg_codec_t */
} rtk_bt_le_audio_unicast_ase_codec_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_ase_qos_t
 * @brief     Bluetooth LE audio unicast aes qos.
 */
typedef struct
{
    uint8_t  phy;
    uint8_t  retransmission_number;
    uint16_t max_sdu;
} rtk_bt_le_audio_unicast_ase_qos_t;

//#endif

#if (defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) || \
	(defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT)
/**
 * @typedef    rtk_bt_le_audio_vcs_mute_state_t
 * @brief     Bluetooth LE audio vcs mute state.
 */
typedef enum 
{
    RTK_BT_LE_AUDIO_VCS_NOT_MUTED =  0,
    RTK_BT_LE_AUDIO_VCS_MUTED  = 1,   
} rtk_bt_le_audio_vcs_mute_state_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_param_t
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
 * @typedef    rtk_bt_le_audio_vcs_cp_op_t
 * @brief     Bluetooth LE audio vcs cp op.
 */
typedef enum {
    RTK_BT_LE_AUDIO_VCS_CP_RELATIVE_VOLUME_DOWN        = 0x00,     /**< Relative Volume Down */
    RTK_BT_LE_AUDIO_VCS_CP_RELATIVE_VOLUME_UP          = 0x01,     /**< Relative Volume Up */
    RTK_BT_LE_AUDIO_VCS_CP_UNMUTE_RELATIVE_VOLUME_DOWN = 0x02,     /**< Unmute/Relative Volume Down */
    RTK_BT_LE_AUDIO_VCS_CP_UNMUTE_RELATIVE_VOLUME_UP   = 0x03,     /**< Unmute/Relative Volume Up */
    RTK_BT_LE_AUDIO_VCS_CP_SET_ABSOLUTE_VOLUME         = 0x04,     /**< Set Absolute Volume */
    RTK_BT_LE_AUDIO_VCS_CP_UNMUTE                      = 0x05,     /**< Unmute */
    RTK_BT_LE_AUDIO_VCS_CP_MUTE                        = 0x06,     /**< Mute */
    RTK_BT_LE_AUDIO_VCS_CP_MAX
} rtk_bt_le_audio_vcs_cp_op_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_cp_param_t
 * @brief     Bluetooth LE audio vcs cp param.
 */
typedef struct
{
    uint8_t volume_setting;             /**< volume_setting, used only for RTK_BT_LE_AUDIO_VCS_CP_SET_ABSOLUTE_VOLUME */
} rtk_bt_le_audio_vcs_cp_param_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_volume_state_t
 * @brief     Bluetooth LE audio vcs volume state.
 */
typedef struct
{
    uint8_t volume_setting;
    uint8_t mute;
    uint8_t change_counter;
} rtk_bt_le_audio_vcs_volume_state_t;

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_vcs_volume_cp_ind_t
 * @brief     Bluetooth LE audio vcs volume control point indicate.
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t volume_setting;
    uint8_t mute;
    rtk_bt_le_audio_vcs_cp_op_t cp_op;
} rtk_bt_le_audio_vcs_volume_cp_ind_t;
#endif

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_vcs_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio vcs client discovery done indication.
 */
typedef struct
{
    uint16_t conn_handle;
    bool     is_found;      /**< Whether to find service. */
    bool     load_from_ftl; /**< Whether the service table is loaded from FTL. */
    uint8_t  type_exist;    /**< vcs char exist flag. 0x01: Volume State Exist. 0x02:Volume Flags Exist. */   
} rtk_bt_le_audio_vcs_client_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_client_volume_state_ind_t
 * @brief     Bluetooth LE audio vcs client volume state indication.
 */
typedef struct
{
    uint16_t        conn_handle;
    bool            is_notify;
    rtk_bt_le_audio_vcs_volume_state_t volume_state;              /**< @ref rtk_bt_le_audio_vcs_volume_state_t */
} rtk_bt_le_audio_vcs_client_volume_state_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_client_volume_flag_ind_t
 * @brief     Bluetooth LE audio vcs client volume flag indication.
 */
typedef struct
{
    uint16_t       conn_handle;
    bool           is_notify;        
    uint8_t        volume_flags;
} rtk_bt_le_audio_vcs_client_volume_flag_ind_t;
#endif
#endif

#if (defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) || \
	(defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) 
/**
 * @typedef    rtk_bt_le_audio_mics_mute_state_t
 * @brief     Bluetooth LE audio mics mute state.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_MICS_NOT_MUTE   = 0,
    RTK_BT_LE_AUDIO_MICS_MUTED      = 1,
    RTK_BT_LE_AUDIO_MICS_DISABLED   = 2,
    RTK_BT_LE_AUDIO_MICS_RFU,
}  rtk_bt_le_audio_mics_mute_state_t;

#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT

/**
 * @typedef    rtk_bt_le_audio_mics_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio mics client discovery done indication.
 */
typedef struct
{
    uint16_t conn_handle;
    bool     is_found;      /**< Whether to find service. */
    bool     load_from_ftl; /**< Whether the service table is loaded from FTL. */
    uint8_t  mic_mute;      /**< mic mute state. @ref rtk_bt_le_audio_mics_mute_state_t */     
} rtk_bt_le_audio_mics_client_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_mics_client_notify_ind_t
 * @brief     Bluetooth LE audio mics client notify indication.
 */
typedef struct
{
    uint16_t conn_handle;
    uint8_t  mic_mute;       /**< mic mute state. @ref rtk_bt_le_audio_mics_mute_state_t */   
} rtk_bt_le_audio_mics_client_notify_ind_t;
#endif

#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_mics_write_mute_ind_t
 * @brief     Bluetooth LE audio mics write mute state indicate.
 */
typedef struct {
    uint8_t mic_mute;		 /**< mic mute state. @ref rtk_bt_le_audio_mics_mute_state_t */
} rtk_bt_le_audio_mics_write_mute_ind_t;

/**
 * @typedef    rtk_bt_le_audio_mics_param_t
 * @brief     Bluetooth LE audio mics write mute state indicate.
 */
typedef struct {
    uint8_t mic_mute;       /**< mic mute state. @ref rtk_bt_le_audio_mics_mute_state_t */
} rtk_bt_le_audio_mics_param_t;
#endif
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_vocs_feature_mask_t
 * @brief     Bluetooth LE AUDIO Volume Offset Control Service Feature Mask.
 */
 typedef enum {
    RTK_BT_LE_AUDIO_VOCS_AUDIO_LOCATION_WRITE_WITHOUT_RSP_SUPPORT = 0x01,
    RTK_BT_LE_AUDIO_VOCS_AUDIO_OUTPUT_DES_WRITE_WITHOUT_RSP_SUPPORT = 0x02,		
    RTK_BT_LE_AUDIO_VOCS_AUDIO_LOCATION_NOTIFY_SUPPORT = 0x04,
    RTK_BT_LE_AUDIO_VOCS_AUDIO_OUTPUT_DES_NOTIFY_SUPPORT = 0x08		    										
}rtk_bt_le_audio_vocs_feature_mask_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_cccd_flags_t
 * @brief     Bluetooth LE AUDIO Volume Offset Control Service CCCD Flags.
 */
 typedef enum {
    RTK_BT_LE_AUDIO_VOCS_VOLUME_OFFSET_STATE_FLAG = 0x01,
    RTK_BT_LE_AUDIO_VOCS_AUDIO_LOCATION_FLAG = 0x02,	
    RTK_BT_LE_AUDIO_VOCS_AUDIO_OUTPUT_DES_FLAG = 0x04				    										
}rtk_bt_le_audio_vocs_cccd_flags_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_output_des_t
 * @brief     Bluetooth LE audio vocs audio output description value.
 */
typedef struct {
    uint16_t    output_des_len;
    uint8_t    *p_output_des;    
} rtk_bt_le_audio_vocs_output_des_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_param_t
 * @brief     Bluetooth LE audio vocs param.
 */
typedef struct {
    int16_t                             volume_offset;
    uint8_t                             change_counter;
    uint32_t                            audio_location;  /**<  Audio location: @ref rtk_bt_le_audio_location_t. */
    rtk_bt_le_audio_vocs_output_des_t   output_des;
} rtk_bt_le_audio_vocs_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_offset_state_ind_t
 * @brief     Bluetooth LE audio vocs write offset state indicate.
 */
typedef struct {
    uint16_t conn_handle;
    uint8_t  srv_instance_id; 
    int16_t  volume_offset;
} rtk_bt_le_audio_vocs_write_offset_state_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_audio_location_ind_t
 * @brief     Bluetooth LE audio vocs write audio location indicate.
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     srv_instance_id; 
    uint32_t    audio_location;  /**<  Audio location: @ref rtk_bt_le_audio_location_t. */
} rtk_bt_le_audio_vocs_write_audio_location_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_output_des_ind_t
 * @brief     Bluetooth LE audio vocs write output des indicate.
 */
typedef struct {
    uint16_t    conn_handle;
    uint8_t     srv_instance_id; 
    rtk_bt_le_audio_vocs_output_des_t output_des;
} rtk_bt_le_audio_vocs_write_output_des_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_char_type_t
 * @brief     Bluetooth LE audio vocs characteristics type.
 */
typedef enum {
    RTK_BT_LE_AUDIO_VOCS_CHAR_OFFSET_STATE,
    RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_LOCATION,
    RTK_BT_LE_AUDIO_VOCS_CHAR_AUDIO_OUTPUT_DESC,
} rtk_bt_le_audio_vocs_char_type_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_volume_offset_state_t
 * @brief     Bluetooth LE audio vocs volume offset state value.
 */
typedef struct {
    int16_t volume_offset;
    uint8_t change_counter;
} rtk_bt_le_audio_vocs_volume_offset_state_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_cp_op_t
 * @brief     Bluetooth LE audio vocs volume offset control point opcode.
 */
typedef enum {
    RTK_BT_LE_AUDIO_VOCS_CP_SET_VOLUME_OFFSET = 0x01,   /**< Set Volume Offset */
    RTK_BT_LE_AUDIO_VOCS_CP_MAX
} rtk_bt_le_audio_vocs_cp_op_t;

 /**
 * @typedef    rtk_bt_le_audio_vocs_cp_param_t
 * @brief     Bluetooth LE audio vocs cp param.
 */
typedef struct {
    bool    counter_used;
    uint8_t change_counter;
    int16_t volume_offset;
}  rtk_bt_le_audio_vocs_cp_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_char_data_t
 * @brief     Bluetooth LE audio vocs characteristics data.
 */
typedef union {
    rtk_bt_le_audio_vocs_volume_offset_state_t  volume_offset;
    uint32_t                                    audio_location;  /**<  Audio location: @ref rtk_bt_le_audio_location_t. */
    rtk_bt_le_audio_vocs_output_des_t           output_des;
} rtk_bt_le_audio_vocs_char_data_t;


/**
 * @typedef    rtk_bt_le_audio_vocs_srv_data_t
 * @brief     Bluetooth LE audio vocs service data.
 */
typedef struct {
    uint8_t                     						srv_instance_id;
    uint8_t                     						type_exist;
    rtk_bt_le_audio_vocs_volume_offset_state_t  	    volume_offset;
    uint32_t                    						audio_location;  /**<  Audio location: @ref rtk_bt_le_audio_location_t. */
    rtk_bt_le_audio_vocs_output_des_t           		output_des;
} rtk_bt_le_audio_vocs_srv_data_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio vocs client discovery done indicate.
 */
typedef struct {
    uint16_t conn_handle;
    bool    is_found;      
    bool    load_from_ftl; 
    uint8_t srv_num;       /**< Service instance number. */
} rtk_bt_le_audio_vocs_client_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_client_read_result_ind_t
 * @brief     Bluetooth LE audio vocs client read result indicate.
 */
typedef struct {
    uint16_t                            conn_handle;
    uint8_t                             srv_instance_id;  
    rtk_bt_le_audio_vocs_char_type_t    type;
    rtk_bt_le_audio_vocs_char_data_t    data;
    uint16_t                            cause;
} rtk_bt_le_audio_vocs_client_read_result_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_client_notify_ind_t
 * @brief     Bluetooth LE audio vocs client notification indicate.
 */
typedef struct {
    uint16_t                            conn_handle;
    uint8_t                             srv_instance_id;  
    rtk_bt_le_audio_vocs_char_type_t    type;
    rtk_bt_le_audio_vocs_char_data_t    data;
} rtk_bt_le_audio_vocs_client_notify_ind_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_client_cp_result_ind_t
 * @brief     Bluetooth LE audio vocs write volume offset control point result indicate.
 */
typedef struct {
    uint16_t                        conn_handle;
    uint8_t                         srv_instance_id;   
    uint16_t                        cause;
    rtk_bt_le_audio_vocs_cp_op_t    cp_op;
} rtk_bt_le_audio_vocs_client_cp_result_ind_t;

#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_aics_cccd_flags_t
 * @brief     Bluetooth LE audio AICS Characteristic Flags.
 */
typedef enum {
    RTK_BT_LE_AUDIO_AICS_INPUT_STATE_FLAG       = 0x01,   /**< Audio Input State Flag for read and CCCD configuration. */  
    RTK_BT_LE_AUDIO_AICS_INPUT_STATUS_FLAG      = 0x02,   /**< Audio Input Status Flag for read and CCCD configuration. */
    RTK_BT_LE_AUDIO_AICS_INPUT_DES_FLAG			= 0x04,   /**< Audio Input Description Flag for read and CCCD configuration. */
    RTK_BT_LE_AUDIO_AICS_INPUT_TYPE_FLAG		= 0x08,   /**< Audio Input Type Flag for read. */
    RTK_BT_LE_AUDIO_AICS_GAIN_SETTING_PROP_FLAG = 0x10,   /**< Gain Setting Properties Flag for read. */
} rtk_bt_le_audio_aics_cccd_flags_t;

/**
 * @typedef    rtk_bt_le_audio_aics_gain_mode_t
 * @brief     Bluetooth LE audio AICS Gain Modes.
 */
typedef enum {
    RTK_BT_LE_AUDIO_AICS_GAIN_MODE_MANUAL_ONLY       = 0,  
    RTK_BT_LE_AUDIO_AICS_GAIN_MODE_AUTOMATIC_ONLY    = 1,  
    RTK_BT_LE_AUDIO_AICS_GAIN_MODE_MANUAL			 = 2,
    RTK_BT_LE_AUDIO_AICS_GAIN_MODE_AUTOMATIC		 = 3
} rtk_bt_le_audio_aics_gain_mode_t;

/**
 * @typedef    rtk_bt_le_audio_aics_mute_state_t
 * @brief     Bluetooth LE audio AICS Audio Mute State .
 */
typedef enum {
    RTK_BT_LE_AUDIO_AICS_NOT_MUTED       			= 0,  
    RTK_BT_LE_AUDIO_AICS_MUTED  					= 1,  
    RTK_BT_LE_AUDIO_AICS_MUTE_DISABLED			    = 2
} rtk_bt_le_audio_aics_mute_state_t;

/**
 * @typedef    rtk_bt_le_audio_aics_input_state_t
 * @brief     Bluetooth LE audio AICS Service Audio Input State Characteristic Value.
 */
typedef struct {
    int8_t  gain_setting;
    uint8_t mute;
    uint8_t gain_mode;
    uint8_t change_counter;
} rtk_bt_le_audio_aics_input_state_t;

/**
 * @typedef    rtk_bt_le_audio_aics_gain_setting_prop_t
 * @brief     Bluetooth LE audio AICS Service Gain Setting Properties Characteristic Value.
 */
typedef struct {
    uint8_t gain_setting_units;
    int8_t  gain_setting_min;
    int8_t  gain_setting_max;
} rtk_bt_le_audio_aics_gain_setting_prop_t;

/**
 * @typedef    rtk_bt_le_audio_aics_input_des_t
 * @brief     Bluetooth LE audio AICS service audio input description characteristic values.
 */
typedef struct {
    uint16_t input_des_len;
    uint8_t  *p_input_des;
} rtk_bt_le_audio_aics_input_des_t;

/**
 * @typedef    rtk_bt_le_audio_aics_param_type_t
 * @brief     Bluetooth LE audio AICS parameter type.
 */
typedef enum {
    RTK_BT_LE_AUDIO_AICS_PARAM_INPUT_STATE       = 0x01,   /**< Parameter Data @ref rtk_bt_le_audio_aics_input_state_t */
    RTK_BT_LE_AUDIO_AICS_PARAM_GAIN_SETTING_PROP = 0x02,   /**< Parameter Data @ref rtk_bt_le_audio_aics_gain_setting_prop_t */
    RTK_BT_LE_AUDIO_AICS_PARAM_INPUT_TYPE        = 0x03,   /**< Parameter Data length is 1 byte */
    RTK_BT_LE_AUDIO_AICS_PARAM_INPUT_STATUS      = 0x04,   /**< Parameter Data length is 1 byte */
    RTK_BT_LE_AUDIO_AICS_PARAM_INPUT_DES         = 0x05,   /**< Parameter Data @ref rtk_bt_le_audio_aics_input_des_t */
} rtk_bt_le_audio_aics_param_type_t;

/**
 * @typedef    rtk_bt_le_audio_aics_cp_op_t
 * @brief     Bluetooth LE audio AICS Audio Input Control Point Opcodes.
 */
typedef enum {
    RTK_BT_LE_AUDIO_AICS_CP_SET_GAIN_SETTING        = 0x01,
    RTK_BT_LE_AUDIO_AICS_CP_UNMUTE                  = 0x02,
    RTK_BT_LE_AUDIO_AICS_CP_MUTE                    = 0x03,
    RTK_BT_LE_AUDIO_AICS_CP_SET_MANUAL_GAIN_MODE    = 0x04,
    RTK_BT_LE_AUDIO_AICS_CP_SET_AUTOMATIC_GAIN_MODE = 0x05,
    RTK_BT_LE_AUDIO_AICS_CP_MAX
} rtk_bt_le_audio_aics_cp_op_t;

 /**
 * @typedef    rtk_bt_le_audio_aics_cp_param_t
 * @brief     Bluetooth LE audio aics cp param.
 */
typedef struct {
    int8_t  gaining_setting;
}  rtk_bt_le_audio_aics_cp_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_srv_data_t
 * @brief     Bluetooth LE audio vocs service data.
 */
typedef struct {
    uint8_t                     						srv_instance_id;
    uint8_t                     						type_exist;		 /**< @ref rtk_bt_le_audio_aics_cccd_flags_t. */
    rtk_bt_le_audio_aics_input_state_t  			    input_state;	 /**< @ref rtk_bt_le_audio_aics_input_state_t. */
    rtk_bt_le_audio_aics_gain_setting_prop_t            setting_prop;	 /**< @ref rtk_bt_le_audio_aics_gain_setting_prop_t. */
    uint8_t                 							input_type;
    uint8_t                 							input_status;
    rtk_bt_le_audio_aics_input_des_t           		    input_des;       /**< @ref rtk_bt_le_audio_aics_input_des_t. */
} rtk_bt_le_audio_aics_srv_data_t;

/**
 * @typedef    rtk_bt_le_audio_aics_cp_ind_t
 * @brief     Bluetooth LE audio aics control point indicate.
 */
typedef struct {
    uint16_t                        conn_handle;
    uint8_t                         srv_instance_id;     
    rtk_bt_le_audio_aics_cp_op_t    cp_op;              /**< @ref rtk_bt_le_audio_aics_cp_op_t */
    int8_t                          gain_setting;
} rtk_bt_le_audio_aics_cp_ind_t;

/**
 * @typedef    rtk_bt_le_audio_aics_write_input_des_ind_t
 * @brief     Bluetooth LE audio aics audio input description write indicate.
 */
typedef struct {
    uint16_t         conn_handle;
    uint8_t          srv_instance_id;     
    rtk_bt_le_audio_aics_input_des_t input_des;         /**< @ref rtk_bt_le_audio_aics_input_des_t */
} rtk_bt_le_audio_aics_write_input_des_ind_t;

/**
 * @typedef    rtk_bt_le_audio_aics_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio aics client discovery done indicate.
 */
typedef struct {
    uint16_t conn_handle;
    bool     is_found;          
    bool     load_from_ftl;     
    uint8_t  srv_num;           
} rtk_bt_le_audio_aics_client_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_aics_char_type_t
 * @brief     Bluetooth LE audio aics characteristic type.
 */
typedef enum
{
    RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_STATE,          /**< Audio Input State. */
    RTK_BT_LE_AUDIO_AICS_CHAR_GAIN_SETTING_PROP,    /**< Gain Setting Properties. */
    RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_TYPE,           /**< Audio Input Type. */
    RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_STATUS,         /**< Audio Input Status. */
    RTK_BT_LE_AUDIO_AICS_CHAR_INPUT_DES,            /**< Audio Input Description. */
} rtk_bt_le_audio_aics_char_type_t;

/**
 * @typedef    rtk_bt_le_audio_aics_char_type_t
 * @brief     Bluetooth LE audio aics characteristic type.
 */
typedef union {
    rtk_bt_le_audio_aics_input_state_t          input_state;    /**< Audio Input State Data. */
    rtk_bt_le_audio_aics_gain_setting_prop_t    setting_prop;   /**< Gain Setting Properties Data. */
    uint8_t                                     input_type;     /**< Audio Input Type Data. */
    uint8_t                                     input_status;   /**< Audio Input Status Data. */
    rtk_bt_le_audio_aics_input_des_t            input_des;      /**< Audio Input Description Data. */
} rtk_bt_le_audio_aics_char_data_t;

/**
 * @typedef    rtk_bt_le_audio_aics_client_read_result_ind_t
 * @brief     Bluetooth LE audio aics client read characteristic result indicate.
 */
typedef struct {
    uint16_t                         conn_handle;
    uint8_t                          srv_instance_id;  
    rtk_bt_le_audio_aics_char_type_t type;
    rtk_bt_le_audio_aics_char_data_t data;
    uint16_t                         cause;
} rtk_bt_le_audio_aics_client_read_result_ind_t;

/**
 * @typedef    rtk_bt_le_audio_aics_client_notify_ind_t
 * @brief     Bluetooth LE audio aics client receive notification indicate.
 */
typedef struct {
    uint16_t                            conn_handle;
    uint8_t                             srv_instance_id;   
    rtk_bt_le_audio_aics_char_type_t    type;
    rtk_bt_le_audio_aics_char_data_t    data;
} rtk_bt_le_audio_aics_client_notify_ind_t;

/**
 * @typedef    rtk_bt_le_audio_aics_client_cp_result_ind_t
 * @brief     Bluetooth LE audio aics client write control point result indicate.
 */
typedef struct {
    uint16_t                        conn_handle;
    uint8_t                         srv_instance_id; 
    rtk_bt_le_audio_aics_cp_op_t    cp_op;
    uint16_t                        cause;    
} rtk_bt_le_audio_aics_client_cp_result_ind_t;

#endif
/****************** Data structure for API func param pack and unpack ******************/
/**
 * @typedef    rtk_bt_le_audio_get_prefer_codec_cfg_param_t
 * @brief     Bluetooth LE audio get prefer codex config param.
 */
typedef struct {
    uint8_t item;
    rtk_bt_le_audio_cfg_codec_t *p_cfg_codec;
} rtk_bt_le_audio_get_prefer_codec_cfg_param_t;

/**
 * @typedef    rtk_bt_le_audio_get_prefer_qos_cfg_param_t
 * @brief     Bluetooth LE audio get prefer qos config param.
 */
typedef struct {
    rtk_bt_le_audio_codec_cfg_item_t codec_cfg_item;
    rtk_bt_le_audio_qos_cfg_type_t qos_cfg_type;
    rtk_bt_le_audio_qos_cfg_preferred_t *p_qos_cfg;
} rtk_bt_le_audio_get_prefer_qos_cfg_param_t;

/**
 * @typedef    rtk_bt_le_audio_pacs_get_info_param_t
 * @brief     Bluetooth LE audio pacs get info param.
 */
typedef struct
{
    uint16_t conn_handle;                           /**< connection handle */
    rtk_bt_le_audio_bap_pacs_info_t *p_pacs_info;   /**< @ref rtk_bt_le_audio_bap_pacs_info_t*/
} rtk_bt_le_audio_pacs_get_info_param_t;

/**
 * @typedef    rtk_bt_le_audio_pacs_get_pac_record_param_t
 * @brief     Bluetooth LE audio pacs query pac record param.
 */
typedef struct
{
    uint16_t conn_handle;                                       /**< connection handle */
    rtk_bt_le_audio_role_t audio_role;                          /**< @ref rtk_bt_le_audio_role_t */       
    uint8_t *p_pac_num;                                         /**< pac num */
    rtk_bt_le_audio_bap_pacs_pac_record_t * p_pac_tbl;          /**< @ref rtk_bt_le_audio_bap_pacs_pac_record_t */                       
} rtk_bt_le_audio_pacs_get_pac_record_param_t;

/**
 * @typedef    rtk_bt_le_audio_pacs_get_lc3_table_mask_param_t
 * @brief     Bluetooth LE audio pacs get lc3 table mask param.
 */  
typedef struct
{
    uint16_t conn_handle;                       /**< connection handle */
    rtk_bt_le_audio_role_t audio_role;          /**< @ref rtk_bt_le_audio_role_t */ 
    uint16_t pref_audio_contexts;
    uint8_t channel_count;
    uint32_t *p_lc3_table_mask;                 /**< returned lc3 suppport table mask . @ref rtk_bt_le_audio_codec_cfg_item_bit_mask_t */ 
}rtk_bt_le_audio_pacs_get_lc3_table_mask_param_t;  

/**
 * @typedef    rtk_bt_le_audio_pacs_get_bis_array_by_sync_info_param_t
 * @brief     Bluetooth LE audio pacs get bis array by sync info param.
 */  
typedef struct
{
	uint16_t conn_handle;                       /**< connection handle */
	rtk_bt_le_audio_sync_handle_t sync_handle;  /**< @ref rtk_bt_le_audio_sync_handle_t */
	uint8_t filter;                             /**< @ref rtk_bt_le_audio_pacs_ba_check_filter_type_t*/ 
    uint32_t *p_bis_array;                      /**< returned bis array*/ 
} rtk_bt_le_audio_pacs_get_bis_array_by_sync_info_param_t;

//#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || (defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)
/**
 * @typedef    rtk_bt_le_audio_sync_allocate_param_t
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
	rtk_bt_le_audio_sync_handle_t *p_broadcast_sync_handle;	/**< to store created sync handle. @ref rtk_bt_le_audio_sync_handle_t */
} rtk_bt_le_audio_sync_allocate_param_t;

/**
 * @typedef    rtk_bt_le_audio_pa_sync_establish_param_t
 * @brief     Bluetooth LE audio paramsters for pa sync establish.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;              /**< Synchronization handle*/
    rtk_bt_le_audio_pa_sync_establish_t establish_param;                        /**< @ref rtk_bt_le_audio_pa_sync_establish_t*/
} rtk_bt_le_audio_pa_sync_establish_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_find_param_t
 * @brief     Bluetooth LE audio paramsters for sync find.
 */
typedef struct
{
    uint8_t adv_sid;                            /**< If Periodic Advertiser List is not used (@ref rtk_bt_le_pa_sync_create_opt_t),
                                                    Advertising SID subfield in the ADI field used to identify the Periodic Advertising. */
    uint8_t broadcast_id[RTK_BT_LE_AUDIO_BROADCAST_ID_LEN];                                                        
	rtk_bt_le_audio_sync_handle_t *p_broadcast_sync_handle;	/**< to store found sync handle. @ref rtk_bt_le_audio_sync_handle_t */
} rtk_bt_le_audio_sync_find_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_get_sync_info_param_t
 * @brief     Bluetooth LE audio paramsters for pa sync get info.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;                 /**< Synchronization handle*/
    rtk_bt_le_audio_sync_info_t *p_sync_info;                  /**< @ref rtk_bt_le_audio_sync_info_t*/
}rtk_bt_le_audio_sync_get_sync_info_param_t;

/**
 * @typedef rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t
 * @brief   structure of LE audio  get bis codec config parameter
 */
typedef struct {
    rtk_bt_le_audio_sync_handle_t sync_handle;                          /**< Synchronization handle*/
    uint8_t bis_idx;                                                    /**< bis index */
    rtk_bt_le_audio_cfg_codec_t *p_codec_cfg;                           /**< @ref rtk_bt_le_audio_cfg_codec_t*/       
} rtk_bt_le_audio_sync_get_bis_codec_cfg_param_t;

//#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_sync_big_establish_param_t
 * @brief     Bluetooth LE audio paramsters for big sync establish.
 */
typedef struct
{                                                 
	rtk_bt_le_audio_sync_handle_t sync_handle;              /**< Synchronization handle*/
    rtk_bt_le_audio_big_sync_establish_t  establish_param;                      /**< @ref rtk_bt_le_audio_big_sync_establish_t*/ 
} rtk_bt_le_audio_big_sync_establish_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_get_bis_info_param_t
 * @brief     Bluetooth LE audio bis information get param.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;    /**< Synchronization handle*/ 
    rtk_bt_le_audio_bis_info_t *p_bis_info;                      /**< @ref rtk_bt_le_audio_bis_info_t*/
} rtk_bt_le_audio_sync_get_bis_info_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_get_bis_conn_handle_param_t
 * @brief     Bluetooth LE audio bis conn handle get param.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;    /**< Synchronization handle*/ 
    uint8_t bis_idx;                  /**< bis index */
    uint16_t *p_bis_conn_handle;      /**< Connection handle of a BIS. */
} rtk_bt_le_audio_sync_get_bis_conn_handle_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_get_support_bis_array_param_t
 * @brief     Bluetooth LE audio support bis array get param.
 */
typedef struct
{
    rtk_bt_le_audio_sync_handle_t sync_handle;    /**< Synchronization handle*/ 
    uint32_t sink_audio_location;                 /**< sink audio channel allocation */
    uint32_t *p_support_bis_array;                /**< support bis array. */
} rtk_bt_le_audio_sync_get_support_bis_array_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_setup_data_path_param_t
 * @brief     Bluetooth LE audio sync setup data path parameter.
 */
typedef struct
{  
    rtk_bt_le_audio_sync_handle_t sync_handle;                /**< Synchronization handle*/   
    uint8_t bis_idx;                                          /**< bis index */
    rtk_bt_le_audio_bis_data_path_param_t setup_param;        /**< @ref rtk_bt_le_audio_bis_data_path_param_t*/
} rtk_bt_le_audio_sync_setup_data_path_param_t;

/**
 * @typedef    rtk_bt_le_audio_sync_remove_data_path_param_t
 * @brief     Bluetooth LE audio sync setup data path parameter.
 */
typedef struct
{  
    rtk_bt_le_audio_sync_handle_t sync_handle;                  /**< Synchronization handle*/   
    uint8_t bis_idx;                                            /**< bis index */
} rtk_bt_le_audio_sync_remove_data_path_param_t;

//#endif
//#endif
//#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_broadcast_source_create_param_t
 * @brief     Bluetooth LE audio broadcast source create parameter.
 */
typedef struct
{   
    uint8_t num_groups;   
    rtk_bt_le_audio_broadcast_source_group_param_t group[RTK_BT_LE_AUDIO_BROADCASTER_GROUP_MAX_NUM];    /**<@ ref rtk_bt_le_audio_broadcast_source_group_param_t*/             
    rtk_bt_le_audio_broadcast_source_handle_t *p_broadcast_source_handle;                               /**<@ ref rtk_bt_le_audio_broadcast_source_handle_t*/
} rtk_bt_le_audio_broadcast_source_create_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_info_get_param_t
 * @brief     Bluetooth LE audio param for get broadcast source infomation.
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< Broadcast source handle */
    rtk_bt_le_audio_broadcast_source_info_t *p_info;                    /**< broadcast source infomation. @ref rtk_bt_le_audio_broadcast_source_info_t */ 
} rtk_bt_le_audio_broadcast_source_info_get_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_config_param_t
 * @brief     Bluetooth LE audio param for config broadcast source.
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< broadcast source handle */
    uint8_t adv_sid;                                                    /**< Value of the Advertising SID subfield in the ADI field of the PDU, Range: 0x00 to 0x0F */
    uint8_t group_idx;                                                  /**< broadcast source group index */ 
    rtk_bt_le_audio_broadcast_source_config_t config;                   /**< @ref rtk_bt_le_audio_broadcast_source_config_t */
    uint16_t p_announcements_len;                                       /**< Length of broadcast audio announcements */ 
    uint8_t *p_broadcast_audio_announcements;                           /**< broadcast audio announcements */ 
} rtk_bt_le_audio_broadcast_source_config_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_enable_param_t
 * @brief     Bluetooth LE audio param for broadcast source create big.
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< Broadcast source handle*/
    rtk_bt_le_audio_broadcast_source_create_big_t create_big;           /**< @ref rtk_bt_le_audio_broadcast_source_create_big_t*/
} rtk_bt_le_audio_broadcast_source_enable_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_setup_data_path_param_t
 * @brief     Bluetooth LE audio bis setup data path parameter.
 */
typedef struct
{  
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< Broadcast source handle*/
    uint8_t bis_idx;                                                    /**< bis index */
    rtk_bt_le_audio_bis_data_path_param_t setup_param;                  /**< @ref rtk_bt_le_audio_bis_data_path_param_t*/
    uint16_t *p_bis_conn_handle;                                        /**< Connection handle of a BIS. */
} rtk_bt_le_audio_broadcast_source_setup_data_path_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_setup_data_path_param_t
 * @brief     Bluetooth LE audio bis setup data path parameter.
 */
typedef struct
{  
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< Broadcast source handle*/    
    uint8_t bis_idx;                                                    /**< bis index */
} rtk_bt_le_audio_broadcast_source_remove_data_path_param_t;

/**
 * @typedef    rtk_bt_le_audio_broadcast_source_reconfig_param_t
 * @brief     Bluetooth LE audio param for broadcast source reconfig.
 */
typedef struct
{
    rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle;  /**< Broadcast source handle*/
    uint8_t group_idx;                                                  /**< Broadcast source group id*/
    uint8_t subgroup_idx;                                               /**< Broadcast source subgroup id*/
    uint8_t metadata_len;                                               /**< metadata len*/
    uint8_t *p_metadata;                                                /**< metadata*/
} rtk_bt_le_audio_broadcast_source_reconfig_param_t;
//#endif

//#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_bass_client_get_brs_data_param_t
 * @brief     Bluetooth LE audio param for get broadcast receive state data.
 */
typedef struct
{
    uint16_t conn_handle;                              /**< Connection handle */
    uint8_t char_instance_id;                          /**< Broadcast sync handle @ref rtk_bt_le_audio_sync_handle_t */
    bool is_get_bis_info;                              /**< Whether get cp_bis_info in brs_data, the returned p_cp_bis_info is NULL when it is false */
    rtk_bt_le_audio_bass_brs_data_t *p_brs_data;       /**< @ref rtk_bt_le_audio_bass_brs_data_t */
} rtk_bt_le_audio_bass_client_get_brs_data_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_client_cp_add_source_by_sync_info_param_t
 * @brief     Bluetooth LE audio param for write control point add source operation by sync information.
 */
typedef struct
{
    uint16_t conn_handle;                                               /**< Connection handle */
    rtk_bt_le_audio_sync_handle_t sync_handle;                          /**< Broadcast sync handle @ref rtk_bt_le_audio_sync_handle_t */
    uint8_t pa_sync;                                                    /**< PA_Sync parameter */
    uint32_t bis_array;                                                 /**< BIS array */
    bool is_req;                                                        /**< Whether to send write request */
} rtk_bt_le_audio_bass_client_cp_add_source_by_sync_info_param_t;

/**
 * @typedef    rtk_bt_le_audio_bass_client_cp_modify_source_by_sync_info_param_t
 * @brief     Bluetooth LE audio param for write control point modify source operation by sync information.
 */
typedef struct
{
    uint16_t conn_handle;                                               /**< Connection handle */
    rtk_bt_le_audio_sync_handle_t sync_handle;                          /**< Broadcast sync handle @ref rtk_bt_le_audio_sync_handle_t */
    uint8_t source_id;                                                  /**< Broadcast source id */
    uint8_t pa_sync;                                                    /**< PA_Sync parameter */
    uint32_t bis_array;                                                 /**< BIS array */
    bool is_req;                                                        /**< Whether to send write request */
} rtk_bt_le_audio_bass_client_cp_modify_source_by_sync_info_param_t;
//#endif
/**
 * @typedef    rtk_bt_le_audio_group_add_device_param_t
 * @brief     Bluetooth LE audio group add device param.
 */
typedef struct
{    
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    uint16_t conn_handle;                                           /**< connection handle */
    rtk_bt_le_audio_device_handle_t *p_device_handle;               /**< return device handle. @ref rtk_bt_le_audio_device_handle_t */           
} rtk_bt_le_audio_group_add_device_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_delete_device_param_t
 * @brief     Bluetooth LE audio group remove device param.
 */
typedef struct
{    
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    rtk_bt_le_audio_device_handle_t device_handle;                  /**< device handle. @ref rtk_bt_le_audio_device_handle_t */           
} rtk_bt_le_audio_group_delete_device_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_find_device_param_t
 * @brief     Bluetooth LE audio group find device handle param.
 */
typedef struct
{    
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    rtk_bt_le_addr_t dev_addr;                                      /**< device address. @ref rtk_bt_le_addr_t */ 
    rtk_bt_le_audio_device_handle_t *p_device_handle;               /**< device handle. @ref rtk_bt_le_audio_device_handle_t */           
} rtk_bt_le_audio_group_find_device_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_find_device_by_conn_handle_param_t
 * @brief     Bluetooth LE audio group find device handle by conn handle param.
 */
typedef struct
{    
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    uint16_t conn_handle;                                           /**< connection handle */ 
    rtk_bt_le_audio_device_handle_t *p_device_handle;               /**< device handle. @ref rtk_bt_le_audio_device_handle_t */           
} rtk_bt_le_audio_group_find_device_by_conn_handle_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_get_device_num_param_t
 * @brief     Bluetooth LE audio group find device num param.
 */
typedef struct
{    
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    uint8_t *p_device_num;                                          /**< returned device num */           
} rtk_bt_le_audio_group_get_device_num_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_get_used_device_num_param_t
 * @brief     Bluetooth LE audio group find used_device num param.
 */
typedef struct
{    
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    bool check_conn;                                                /**< whether check connection */ 
    uint8_t *p_used_device_num;                                     /**< returned used device num */          
} rtk_bt_le_audio_group_get_used_device_num_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_get_device_info_param_t
 * @brief     Bluetooth LE audio group get device info param.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;              /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    rtk_bt_le_audio_device_handle_t device_handle;            /**< device handle. @ref rtk_bt_le_audio_device_handle_t */ 
    rtk_bt_le_audio_group_device_info_t *p_device_info;       /**< device info. @ref rtk_bt_le_audio_group_device_info_t */ 
} rtk_bt_le_audio_group_get_device_info_param_t;

/**
 * @typedef    rtk_bt_le_audio_group_get_all_device_info_param_t
 * @brief     Bluetooth LE audio group get all device info param.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;              /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    uint8_t *p_device_num;                                    /**< returned device num */ 
    rtk_bt_le_audio_group_device_info_t *p_device_info_tbl;   /**< returned device num table*/ 
} rtk_bt_le_audio_group_get_all_device_info_param_t;

//#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT

/**
 * @typedef    rtk_bt_le_audio_stream_session_allocate_param_t
 * @brief     Bluetooth LE audio stream session handle allocate param.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                      /**< group handle. @ref rtk_bt_le_audio_group_handle_t */ 
    rtk_bt_le_audio_stream_session_handle_t *p_stream_session_handle; /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */ 
} rtk_bt_le_audio_stream_session_allocate_param_t;

/**
 * @typedef    rtk_bt_le_audio_stream_session_get_group_handle_param_t
 * @brief     Bluetooth LE audio stream session get group handle param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_group_handle_t *p_group_handle;                  /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
} rtk_bt_le_audio_stream_session_get_group_handle_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_get_session_info_param_t
 * @brief     Bluetooth LE audio unicast get session info param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;  /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */  
    rtk_bt_le_audio_unicast_session_info_t  *p_session_info;        /**< session info. @ref rtk_bt_le_audio_unicast_session_info_t */  
} rtk_bt_le_audio_unicast_get_session_info_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_get_cfg_mask_param_t
 * @brief     Bluetooth LE audio unicast cfg mask get param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle; /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    uint32_t prefer_cfg;                   
    uint8_t device_num;
    rtk_bt_le_audio_device_handle_t *p_device_handle_tbl;          /**< device handle table. @ref rtk_bt_le_audio_device_handle_t */
    uint32_t *p_cfg_mask;    
} rtk_bt_le_audio_unicast_get_cfg_mask_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_get_cis_info_param_t
 * @brief     Bluetooth LE audio unicast cis info get param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle; /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                 /**< device handle. @ref rtk_bt_le_audio_device_handle_t */
    rtk_bt_le_audio_unicast_cis_infos_t * p_cis_infos;             /**< cis infos. @ref rtk_bt_le_audio_unicast_cis_infos_t */    
} rtk_bt_le_audio_unicast_get_cis_info_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_config_param_t
 * @brief     Bluetooth LE audio unicast config param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle; /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_unicast_cfg_type_t cfg_type;                   /**< cfg type. @ref rtk_bt_le_audio_unicast_cfg_type_t */
    uint8_t device_num;
    rtk_bt_le_audio_device_handle_t *p_device_handle_tbl;          /**< device handle table. @ref rtk_bt_le_audio_device_handle_t */
} rtk_bt_le_audio_unicast_config_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_cfg_ase_codec_param_t
 * @brief     Bluetooth LE audio unicast ase codec config param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t     stream_session_handle;  /**< @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t             device_handle;          /**< @ref rtk_bt_le_audio_device_handle_t */
    uint8_t                                     cfg_idx;
    rtk_bt_le_audio_unicast_ase_codec_t         ase_codec;              /**< @ref rtk_bt_le_audio_unicast_ase_codec_t */
} rtk_bt_le_audio_unicast_cfg_ase_codec_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_get_session_qos_param_t
 * @brief     Bluetooth LE audio unicast get session qos parameter.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t  stream_session_handle;   /**< @ref rtk_bt_le_audio_stream_session_handle_t*/
    rtk_bt_le_audio_unicast_session_qos_t *p_session_qos;             /**< @ref rtk_bt_le_audio_unicast_session_qos_t*/
} rtk_bt_le_audio_unicast_get_session_qos_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_cfg_ase_qos_param_t
 * @brief     Bluetooth LE audio unicast config aes qos param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                   /**< device handle. @ref rtk_bt_le_audio_device_handle_t */
    uint8_t ase_id;
    rtk_bt_le_audio_unicast_ase_qos_t ase_qos;                       /**< ase qos. @ref rtk_bt_le_audio_unicast_ase_qos_t */
} rtk_bt_le_audio_unicast_cfg_ase_qos_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_get_ase_qos_param_t
 * @brief     Bluetooth LE audio unicast get aes qos param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle;   /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    rtk_bt_le_audio_device_handle_t device_handle;                   /**< device handle. @ref rtk_bt_le_audio_device_handle_t */
    uint8_t ase_id;
    rtk_bt_le_audio_unicast_ase_qos_t *p_ase_qos;                    /**< ase qos. @ref rtk_bt_le_audio_unicast_ase_qos_t */
} rtk_bt_le_audio_unicast_get_ase_qos_param_t;

/**
 * @typedef    rtk_bt_le_audio_unicast_stop_param_t
 * @brief     Bluetooth LE audio unicast stop param.
 */
typedef struct
{
    rtk_bt_le_audio_stream_session_handle_t stream_session_handle; /**< stream session handle. @ref rtk_bt_le_audio_stream_session_handle_t */
    uint32_t cis_timeout_ms;                                       /**< cis timeout for stop, unit: ms  */
} rtk_bt_le_audio_unicast_stop_param_t;

//#endif

#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_csis_gen_rsi_info_t
 * @brief     Bluetooth LE audio info for csis gen for rsi.
 */
typedef struct
{
    uint8_t sirk[RTK_BT_LE_CSIS_SIRK_LEN];      /**< csis sirk (set identity resolving key)*/ 
	uint8_t *p_rsi;                             /**< returned resolvable set identifier. */ 
} rtk_bt_le_audio_csis_gen_rsi_info_t;

/**
 * @typedef    rtk_bt_le_audio_csis_update_sirk_param_t
 * @brief     Bluetooth LE audio info for csis change sirk.
 */
typedef struct
{
    rtk_bt_le_audio_csis_sirk_type_t sirk_type;        /**< csis sirk type. @ref rtk_bt_le_audio_csis_sirk_type_t. */ 
	uint8_t sirk[RTK_BT_LE_CSIS_SIRK_LEN];             /**< new csis sirk. */ 
} rtk_bt_le_audio_csis_update_sirk_param_t;

/**
 * @typedef    rtk_bt_le_audio_csis_update_size_param_t
 * @brief     Bluetooth LE audio info for csis change size.
 */
typedef struct
{
	uint8_t csis_size;                                 /**< Coordinated Set Size characteristic value. Range:0x01 to 0xFF. */ 
} rtk_bt_le_audio_csis_update_size_param_t;

#endif 

#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t
 * @brief     Bluetooth LE audio info for csis set coordinator check adv rsi.
 */
typedef struct
{
    uint8_t report_data_len;
	uint8_t *p_report_data;
	rtk_bt_le_addr_t adv_addr;				/**< @ref rtk_bt_le_addr_t. */ 
} rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi_info_t;

/**
 * @typedef    rtk_bt_le_audio_csis_set_coordinator_cfg_discover_info_t
 * @brief     Bluetooth LE audio info for csis set coordinator config discover.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
	bool discover;
	uint32_t timeout_ms;				
} rtk_bt_le_audio_csis_set_coordinator_cfg_discover_info_t;

/**
 * @typedef    rtk_bt_le_audio_csis_set_coordinator_add_group_info_t
 * @brief     Bluetooth LE audio info for csis set coordinator add group.
 */
typedef struct
{ 	
	rtk_bt_le_audio_csis_set_mem_info_t mem_info;
	rtk_bt_le_audio_group_handle_t *p_group_handle;                    /**< returned group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_device_handle_t *p_device_handle;
} rtk_bt_le_audio_csis_set_coordinator_add_group_info_t;

/**
 * @typedef    rtk_bt_le_audio_csis_set_coordinator_add_device_info_t
 * @brief     Bluetooth LE audio info for csis set coordinator add device in group.
 */
typedef struct
{ 
	rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_csis_set_mem_info_t mem_info;
    rtk_bt_le_audio_device_handle_t *p_device_handle;               /**< returned device handle. @ref rtk_bt_le_audio_group_handle_t */
} rtk_bt_le_audio_csis_set_coordinator_add_device_info_t;

/**
 * @typedef    rtk_bt_le_audio_csis_set_coordinator_write_lock_info_t
 * @brief     Bluetooth LE audio info for csis set coordinator write lock or unlock in group.
 */
typedef struct
{ 
	rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    bool lock;
} rtk_bt_le_audio_csis_set_coordinator_write_lock_info_t;

#endif 

#if (defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) || \
	(defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT)
/**
 * @typedef    rtk_bt_le_audio_mcs_char_uuid_t
 * @brief     Bluetooth LE audio mcs characteristic UUID in MCS or GMCS.
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME             =0x2B93,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_ICON_URL         =0x2B95,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_CHANGED                 =0x2B96,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE                   =0x2B97,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_DURATION                =0x2B98,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_POSITION                =0x2B99,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_PLAYBACK_SPEED                =0x2B9A,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_SEEKING_SPEED                 =0x2B9B,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_PLAYING_ORDER                 =0x2BA1,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_PLAYING_ORDERS_SUPPORTED      =0x2BA2,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_STATE                   =0x2BA3,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_CONTROL_POINT           =0x2BA4,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_CONTROL_POINT_OPCODE_SUPPORTED=0x2BA5,
    RTK_BT_LE_AUDIO_MCS_UUID_CHAR_CONTENT_CONTROL_ID            =0x2BBA
} rtk_bt_le_audio_mcs_char_uuid_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_cccd_flag_t
 * @brief     Bluetooth LE audio mcs cccd flag.
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_PLAYER_NAME                       =0x00000001,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_TRACK_CHANGED                           =0x00000002,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_TRACK_TITLE                             =0x00000004,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_TRACK_DURATION                          =0x00000008,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_TRACK_POSITION                          =0x00000010,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_STATE                             =0x00000020,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_CONTROL_POINT                     =0x00000040,
    RTK_BT_LE_AUDIO_MCP_CLIENT_CFG_CCCD_FLAG_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED   =0x00000080
} rtk_bt_le_audio_mcs_cccd_flag_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_media_cp_opcode_t
 * @brief     Bluetooth LE audio mcs media control point characteristic opcodes in MCS or GMCS.
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_PLAY                  =0x01,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_PAUSE                 =0x02,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_FAST_REWIND           =0x03,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_FAST_FORWARD          =0x04,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_STOP                  =0x05,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_MOVE_RELATIVE         =0x10,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_GOTO_SEGMENT          =0x24,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_PREVIOUS_TRACK        =0x30,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_NEXT_TRACK            =0x31,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_FIRST_TRACK           =0x32,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_LAST_TRACK            =0x33,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_GOTO_TRACK            =0x34,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_CHAR_OPCODE_GOTO_GROUP            =0x44,
} rtk_bt_le_audio_mcs_media_cp_opcode_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_media_state_t
 * @brief     Bluetooth LE audio mcs media state.
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCS_MEDIA_STATE_INACTIVE = 0x00,
    RTK_BT_LE_AUDIO_MCS_MEDIA_STATE_PLAYING,
    RTK_BT_LE_AUDIO_MCS_MEDIA_STATE_PAUSED,
    RTK_BT_LE_AUDIO_MCS_MEDIA_STATE_SEEKING
} rtk_bt_le_audio_mcs_media_state_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_playing_orders_supported_t
 * @brief     Bluetooth LE audio mcs playing orders supported characteristic bit values .
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SINGLE_ONCE = 0x0001,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SINGLE_REPEAT = 0x0002,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_IN_ORDER_ONCE = 0x0004,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_IN_ORDER_REPEAT = 0x0008,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_OLDEST_ONCE = 0x0010,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_OLDEST_REPEAT  = 0x0020,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_NEWEST_ONCE = 0x0040,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_NEWEST_REPEAT = 0x0080,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SHUFFLE_ONCE = 0x0100,
    RTK_BT_LE_AUDIO_MCS_PLAYING_ORDERS_SUPPORTED_CHAR_BIT_VALUE_SHUFFLE_REPEAT = 0x0200
} rtk_bt_le_audio_mcs_playing_orders_supported_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_media_control_point_opcodes_supported_t
 * @brief     Bluetooth LE audio mcs media control point opcodes supported characteristic bit values.
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_PLAY = 0x00000001,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_PAUSE = 0x00000002,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_FAST_REWIND = 0x00000004,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_FAST_FORWARD = 0x00000008,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_STOP = 0x00000010,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_MOVE_RELATIVE = 0x00000020,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_PREVIOUS_TRACK = 0x00000800,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_NEXT_TRACK = 0x00001000,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_FIRST_TRACK = 0x00002000,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_OPCODES_SUPPORTED_CHAR_BIT_VALUE_LAST_TRACK = 0x00004000,
} rtk_bt_le_audio_mcs_media_control_point_opcodes_supported_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_media_control_point_notification_result_codes_t
 * @brief     Bluetooth LE audio mcs media control point notification result codes in MCS or GMCS.
 */
typedef enum {
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_SUCCESS                    =0x01,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_OPCODE_NOT_SUPPORTED       =0x02,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_MEDIA_PLAYER_INACTIVE      =0x03,
    RTK_BT_LE_AUDIO_MCS_MEDIA_CONTROL_POINT_NOTIFICATION_RESULT_CODES_COMMAND_CANNOT_BE_COMPLETED=0x04
} rtk_bt_le_audio_mcs_media_control_point_notification_result_codes_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_media_play_name_t
 * @brief     Bluetooth LE audio mcp server media play name.
 */
typedef struct {
    uint8_t       *p_media_player_name;     /* UTF-8 string */
    uint16_t      media_player_name_len;
} rtk_bt_le_audio_mcp_server_media_play_name_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_track_title_t
 * @brief     Bluetooth LE audio mcp server track title.
 */
typedef struct
{
    uint8_t       *p_track_title;           /* UTF-8 string */
    uint16_t      track_title_len;
} rtk_bt_le_audio_mcp_server_track_title_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_track_title_t
 * @brief     Bluetooth LE audio mcp server media player icon url.
 */
typedef struct
{
    uint8_t       *p_media_player_icon_url;
    uint16_t      media_player_icon_url_len;
} rtk_bt_le_audio_mcp_server_media_player_icon_url_t;

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_mcp_client_write_media_cp_info_t
 * @brief     Bluetooth LE audio info for mcs client send mcp op.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< connection handle. */                         
	uint8_t     srv_instance_id;                        /**< Service Instance Id. */
    bool        general_mcs;                            /**< is generic media control service (GMCS) */
    uint8_t     opcode;                                 /**< @ref rtk_bt_le_audio_mcs_media_cp_opcode_t */
    union {
        int32_t   move_relative_opcode_offset;
        int32_t   goto_segment_opcode_n;
        int32_t   goto_track_opcode_n;
        int32_t   goto_group_opcode_n;
    } op_param; 
    bool        is_req;                                 /**<true: Write request, false: Write command */
} rtk_bt_le_audio_mcp_client_write_media_cp_info_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_client_read_char_value_info_t
 * @brief     Bluetooth LE audio info for mcs client read char value.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< Connection handle. */ 
	uint8_t     srv_instance_id;                        /**< Service Instance Id. */
    bool        general_mcs;                            /**< is generic media control service (GMCS) */
    uint16_t    char_uuid;                              /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
} rtk_bt_le_audio_mcp_client_read_char_value_info_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_client_cfg_cccd_info_t
 * @brief     Bluetooth LE audio info for mcs client config cccd.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< Connection handle. */  
	uint8_t     srv_instance_id;                        /**< Service Instance Id. */
    bool        general_mcs;                            /**< is generic media control service (GMCS)*/
    uint32_t    cfg_flags;                              /**< @ref rtk_bt_le_audio_mcs_cccd_flag_t */    
    bool        enable;            
} rtk_bt_le_audio_mcp_client_cfg_cccd_info_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_client_discovery_done_ind_t
 * @brief     Bluetooth LE audio mcp client discovery done indication.
 */
typedef struct
{
    uint16_t    conn_handle;                            /**< Connection handle. */ 
    bool        is_found;
    bool        load_from_ftl;                          /**< Whether the service table is loaded from FTL. */
    bool        general_mcs;                            /**< Is generic media control service (GMCS)*/    
    uint8_t     srv_num;           
} rtk_bt_le_audio_mcp_client_discovery_done_ind_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_client_read_result_ind_t
 * @brief     Bluetooth LE audio mcp client read result indication.
 */
typedef struct {
    uint16_t    cause;
    uint16_t    conn_handle;                            /**< Connection handle. */ 
    bool        general_mcs;                            /**< Is generic media control service (GMCS)*/					
    uint8_t     srv_instance_id;                        /**< Service Instance id. */
    uint16_t    char_uuid;                              /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
    union {
	    rtk_bt_le_audio_mcp_server_media_play_name_t media_player_name;    /*@ref rtk_bt_le_audio_mcp_server_media_play_name_t */
	    rtk_bt_le_audio_mcp_server_track_title_t track_title;			   /*@ref rtk_bt_le_audio_mcp_server_track_title_t */	
        int32_t track_duration;  /* Length of the current track in 0.01-second resolution,zero or greater. 
                                            * \arg    @ref RTK_BT_LE_AUDIO_MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN : no current track or the duration of the current track is unknown */
        int32_t track_position;  /* Current track position of the current track in 0.01-second resolution, Offset from the start of the track to the current playing position.  
                                            * \arg    0: starting position or start of the track is not well defined
                                            * \arg    @ref RTK_BT_LE_AUDIO_MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE : no current track or the start of the track is not well defined
                                            * \arg    other values */
        int8_t  playback_speed;
        int8_t  seeking_speed;
        uint8_t playing_order;      /*@ref rtk_bt_le_audio_mcs_playing_orders_supported_t */
        uint8_t media_state;        /*@ref rtk_bt_le_audio_mcs_media_state_t */
        uint8_t content_control_id; /*Represent a unique instance of a service that either controls or provides status information on an audio-related feature */
        uint16_t playing_orders_supported; /*Combination of @ref rtk_bt_le_audio_mcs_playing_orders_supported_t */
        uint32_t media_control_point_opcodes_supported; /*Combination of @ref rtk_bt_le_audio_mcs_media_control_point_opcodes_supported_t */
    } data;
} rtk_bt_le_audio_mcp_client_read_result_ind_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_client_notify_ind_t
 * @brief     Bluetooth LE audio mcs client notify result indication.
 */
typedef struct {
    uint16_t    conn_handle;                            /**< Connection handle. */ 
    bool        general_mcs;                            /**< Is generic media control service (GMCS)*/					
    uint8_t     srv_instance_id;                        /**< Service Instance Id. */
    uint16_t    char_uuid;                              /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
    union {
	    rtk_bt_le_audio_mcp_server_media_play_name_t media_player_name;    /*@ref rtk_bt_le_audio_mcp_server_media_play_name_t */
	    rtk_bt_le_audio_mcp_server_track_title_t track_title;			    /*@ref rtk_bt_le_audio_mcp_server_track_title_t */	
        int32_t track_duration;  /* Length of the current track in 0.01-second resolution,zero or greater. @ref RTK_BT_LE_AUDIO_MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN : no current track or the duration of the current track is unknown */
        int32_t track_position;  /* Current track position of the current track in 0.01-second resolution, Offset from the start of the track to the current playing position.                                                                         * \arg    0: starting position or start of the track is not well defined
                                    * \arg    @ref RTK_BT_LE_AUDIO_MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE : no current track or the start of the track is not well defined
                                    * \arg    other values */
        int8_t  playback_speed;
        int8_t  seeking_speed;
        uint8_t playing_order;      /*@ref rtk_bt_le_audio_mcs_playing_orders_supported_t */    
        uint8_t media_state;        /*@ref rtk_bt_le_audio_mcs_media_state_t */
    } data;
} rtk_bt_le_audio_mcp_client_notify_ind_t;

/**
 * @typedef    rtk_bt_le_audio_mcs_media_cp_nodify_ind_t
 * @brief     Bluetooth LE audio mcs client media cp notify indication.
 */
typedef struct {
    uint16_t    conn_handle;       /**< Connection handle. */ 
    bool        general_mcs;       /**< Is generic media control service (GMCS)*/					
    uint8_t     srv_instance_id;   /**< Service Instance Id. */
    uint8_t     requested_opcode;  /* @ref rtk_bt_le_audio_mcs_media_cp_opcode_t */
    uint8_t     result_code;       /* @ref rtk_bt_le_audio_mcs_media_control_point_notification_result_codes_t */
} rtk_bt_le_audio_mcs_media_cp_nodify_ind_t;

#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_mcp_server_set_param_t
 * @brief     Bluetooth LE audio mcp server set parameter.
 */
typedef struct {
    uint8_t     service_id;                             /**< mcs service id */
    uint16_t    char_uuid;                              /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
    union {
        uint8_t  media_state;  				            /*@ref rtk_bt_le_audio_mcs_media_state_t */
        uint16_t playing_orders_supported;	            /*@ref rtk_bt_le_audio_mcs_playing_orders_supported_t */
        uint32_t media_control_point_opcodes_supported; /*@ref rtk_bt_le_audio_mcs_media_control_point_opcodes_supported_t */
    } param;
} rtk_bt_le_audio_mcp_server_set_param_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_write_media_cp_ind_t
 * @brief     Bluetooth LE audio mcp server send data parameter.
 */
typedef struct {
    uint8_t     service_id;                                     /**< mcs service id */    
    uint16_t    char_uuid;                                      /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
    union {
	    rtk_bt_le_audio_mcp_server_media_play_name_t media_player_name;    /*@ref rtk_bt_le_audio_mcp_server_media_play_name_t */
	    rtk_bt_le_audio_mcp_server_track_title_t track_title;				/*@ref rtk_bt_le_audio_mcp_server_track_title_t */
        int32_t track_duration;  /* Length of the current track in 0.01-second resolution,zero or greater. @ref RTK_BT_LE_AUDIO_MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN : no current track or the duration of the current track is unknown */
        int32_t track_position;  /* Current track position of the current track in 0.01-second resolution, Offset from the start of the track to the current playing position.                                                                         * \arg    0: starting position or start of the track is not well defined
                                    * \arg    @ref RTK_BT_LE_AUDIO_MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE : no current track or the start of the track is not well defined
                                    * \arg    other values */
        int8_t  playback_speed;
        int8_t  seeking_speed;
        uint8_t playing_order;  /*@ref rtk_bt_le_audio_mcs_playing_orders_supported_t */
    } param;
} rtk_bt_le_audio_mcp_server_send_data_param_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_read_confirm_param_t
 * @brief     Bluetooth LE audio mcp server read confirm parameter.
 */
typedef struct {
    uint16_t  conn_handle;
    uint16_t  cid;
    uint8_t   service_id;                                     /**< mcs service id */ 
    uint16_t  char_uuid;                                      /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
    uint16_t  offset;
    union {
	    rtk_bt_le_audio_mcp_server_media_play_name_t media_player_name;             /*@ref rtk_bt_le_audio_mcp_server_media_play_name_t */
        rtk_bt_le_audio_mcp_server_media_player_icon_url_t  media_player_icon_url;  /*@ref  rtk_bt_le_audio_mcp_server_media_player_icon_url_t */
	    rtk_bt_le_audio_mcp_server_track_title_t track_title;			            /*@ref rtk_bt_le_audio_mcp_server_track_title_t */        
        int32_t track_duration;  /* Length of the current track in 0.01-second resolution,zero or greater. @ref RTK_BT_LE_AUDIO_MCS_TRACK_DURATION_CHAR_VALUE_UNKNOWN : no current track or the duration of the current track is unknown */
        int32_t track_position;  /* Current track position of the current track in 0.01-second resolution, Offset from the start of the track to the current playing position.                                                                         * \arg    0: starting position or start of the track is not well defined
                                            * \arg    @ref RTK_BT_LE_AUDIO_MCS_TRACK_POSITION_CHAR_VALUE_UNAVAILABLE : no current track or the start of the track is not well defined
                                            * \arg    other values */
        int8_t  playback_speed;
        int8_t  seeking_speed;
        uint8_t playing_order;          /*@ref rtk_bt_le_audio_mcs_playing_orders_supported_t */
        uint8_t content_control_id;
    } param;
} rtk_bt_le_audio_mcp_server_read_confirm_param_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_write_media_cp_ind_t
 * @brief     Bluetooth LE audio mcp server write media control point indication.
 */
typedef struct {
    uint16_t        conn_handle;
    uint16_t        cid;
    uint8_t         service_id;                     /**< mcs service id */ 
    uint8_t         opcode;                         /**< @ref rtk_bt_le_audio_mcs_media_cp_opcode_t */
    union {
        int32_t   move_relative_opcode_offset;
        int32_t   goto_segment_opcode_n;
        int32_t   goto_track_opcode_n;
        int32_t   goto_group_opcode_n;
    } param;         
} rtk_bt_le_audio_mcp_server_write_media_cp_ind_t;

/**
 * @typedef    rtk_bt_le_audio_mcp_server_read_ind_t
 * @brief     Bluetooth LE audio mcp read indication.
 */
typedef struct {
    uint16_t  conn_handle;
    uint16_t  cid;
    uint8_t   service_id;                             /**< mcs service id */ 
    uint16_t  char_uuid;                              /**< char_uuid. @ref rtk_bt_le_audio_mcs_char_uuid_t */
    uint16_t  offset;         
} rtk_bt_le_audio_mcp_server_read_ind_t;
#endif
#endif

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT

/**
 * @typedef    rtk_bt_le_audio_vcs_write_cp_param_t
 * @brief     Bluetooth LE audio vcs cp param.
 */
typedef struct
{
    uint16_t conn_handle;                     /**< connection handle */
    rtk_bt_le_audio_vcs_cp_op_t cp_op;        /**<cp op. @ref rtk_bt_le_audio_vcs_cp_op_t */
    rtk_bt_le_audio_vcs_cp_param_t cp_param;  /**< cp param. @ref rtk_bt_le_audio_vcs_cp_param_t*/
} rtk_bt_le_audio_vcs_write_cp_param_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_get_volume_state_param_t
 * @brief     Bluetooth LE audio vcs volume state get param.
 */
typedef struct
{
    uint16_t conn_handle;                                     /**< connection handle */
    rtk_bt_le_audio_vcs_volume_state_t *p_vcs_volume_state ;  /**< vcs volume state. @ref rtk_bt_le_audio_vcs_volume_state_t*/
} rtk_bt_le_audio_vcs_get_volume_state_param_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_change_mute_param_t
 * @brief     Bluetooth LE audio vcs change mute param.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    uint8_t mute;  									      /**< vcs mute. @ref rtk_bt_le_audio_vcs_volume_state_t*/
} rtk_bt_le_audio_vcs_change_mute_param_t;

/**
 * @typedef    rtk_bt_le_audio_vcs_change_volume_param_t
 * @brief     Bluetooth LE audio vcs change volume param.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    uint8_t volume_setting;  							      /**< volume setting. */
} rtk_bt_le_audio_vcs_change_volume_param_t;
#endif

#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_mics_change_mute_param_t
 * @brief     Bluetooth LE audio mics change mute param.
 */
typedef struct
{
    rtk_bt_le_audio_group_handle_t group_handle;                    /**< group handle. @ref rtk_bt_le_audio_group_handle_t */
    rtk_bt_le_audio_mics_mute_state_t mic_mute;  			      /**< mic mute.  @ref rtk_bt_le_audio_mics_mute_state_t*/
} rtk_bt_le_audio_mics_change_mute_param_t;

/**
 * @typedef    rtk_bt_le_audio_mics_set_mute_value_param_t
 * @brief     Bluetooth LE audio mics set mute value param.
 */
typedef struct
{
    uint16_t conn_handle;                                     	  /**< connection handle */
    rtk_bt_le_audio_mics_mute_state_t mic_mute;  			      /**< mic mute.  @ref rtk_bt_le_audio_mics_mute_state_t*/
} rtk_bt_le_audio_mics_set_mute_value_param_t;

/**
 * @typedef    rtk_bt_le_audio_mics_get_mute_value_param_t
 * @brief     Bluetooth LE audio mics get mute value param.
 */
typedef struct
{
    uint16_t conn_handle;                                     	/**< connection handle */
    rtk_bt_le_audio_mics_mute_state_t *p_mic_mute;  			/**< mic mute.  @ref rtk_bt_le_audio_mics_mute_state_t*/
} rtk_bt_le_audio_mics_get_mute_value_param_t;
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_vocs_set_param_t
 * @brief     Bluetooth LE audio vocs set parameter.
 */
typedef struct {
    uint8_t                             srv_instance_id;
    uint8_t                             set_mask;
    rtk_bt_le_audio_vocs_param_t        vocs_param;
} rtk_bt_le_audio_vocs_set_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_get_param_t
 * @brief     Bluetooth LE audio vocs get parameter.
 */
typedef struct {
    uint8_t                             srv_instance_id;
    rtk_bt_le_audio_vocs_param_t        *p_vocs_param;
} rtk_bt_le_audio_vocs_get_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_cfg_cccd_param_t
 * @brief     Bluetooth LE audio vocs config cccd parameter.
 */
typedef struct {	
    uint16_t conn_handle;			/**< Connection handle */
    uint8_t srv_instance_id;		/**< VOCS service index */
    uint8_t cfg_flags;				/**< VOCS CCCD Flag, @ref rtk_bt_le_audio_vocs_cccd_flags_t*/
    bool enable;					/**< Whether to enable CCCD */
} rtk_bt_le_audio_vocs_cfg_cccd_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_read_char_value_param_t
 * @brief     Bluetooth LE audio vocs read char value parameter.
 */
typedef struct {
    uint16_t conn_handle;					/**<  Connection handle */
    uint8_t srv_instance_id;				/**<  VOCS service index */
    rtk_bt_le_audio_vocs_char_type_t type;     /**<  VOCS characteristic type, @ref rtk_bt_le_audio_vocs_char_type_t */
} rtk_bt_le_audio_vocs_read_char_value_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_cp_param_t
 * @brief     Bluetooth LE audio vocs write control point parameter.
 */
typedef struct {
    uint16_t conn_handle;					/**<  Connection handle */
    uint8_t srv_instance_id;				/**<  VOCS service index */
    rtk_bt_le_audio_vocs_cp_op_t cp_op;     /**<  Volume offset control point opcode, @ref rtk_bt_le_audio_vocs_cp_op_t */
    rtk_bt_le_audio_vocs_cp_param_t cp_param;   /**<  Volume offset control point parameter, @ref rtk_bt_le_audio_vocs_cp_param_t */
} rtk_bt_le_audio_vocs_write_cp_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_cp_by_group_param_t
 * @brief     Bluetooth LE audio vocs write control point parameter.
 */
typedef struct {
    rtk_bt_le_audio_group_handle_t group_handle; 	/**< @ref rtk_bt_le_audio_group_handle_t. */ 
    uint8_t srv_instance_id;						/**<  VOCS service index */
    rtk_bt_le_audio_vocs_cp_op_t cp_op;       		/**<  Volume offset control point opcode, @ref rtk_bt_le_audio_vocs_cp_op_t */
    rtk_bt_le_audio_vocs_cp_param_t cp_param;   	/**<  Volume offset control point parameter, @ref rtk_bt_le_audio_vocs_cp_param_t */
} rtk_bt_le_audio_vocs_write_cp_by_group_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_audio_location_param_t
 * @brief     Bluetooth LE audio vocs write audio location parameter.
 */
typedef struct {
    uint16_t conn_handle;							/**<  Connection handle */
    uint8_t srv_instance_id;						/**<  VOCS service index */
    uint32_t audio_location;       		            /**<  Audio location: @ref rtk_bt_le_audio_location_t. */
} rtk_bt_le_audio_vocs_write_audio_location_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_write_output_des_param_t
 * @brief     Bluetooth LE audio vocs write output descriptor parameter.
 */
typedef struct {
    uint16_t conn_handle;							/**<  Connection handle */
    uint8_t srv_instance_id;						/**<  VOCS service index */
    uint16_t output_des_len;                        /**<  The length of the output descriptor value */
    uint8_t *p_output_des;                          /**<  Point to the output descriptor value. */
} rtk_bt_le_audio_vocs_write_output_des_param_t;

/**
 * @typedef    rtk_bt_le_audio_vocs_get_srv_data_param_t
 * @brief     Bluetooth LE audio vocs get service data parameter.
 */
typedef struct {
    uint16_t conn_handle;							/**<  Connection handle */
    uint8_t srv_instance_id;						/**<  VOCS service index */
    rtk_bt_le_audio_vocs_srv_data_t *p_srv_data;   /**<  returned service data, @ref rtk_bt_le_audio_vocs_srv_data_t */
} rtk_bt_le_audio_vocs_get_srv_data_param_t;

#endif 

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
/**
 * @typedef    rtk_bt_le_audio_aics_set_param_t
 * @brief     Bluetooth LE audio aics set paramter.
 */
typedef struct {
    uint8_t                             srv_instance_id;
    rtk_bt_le_audio_aics_param_type_t   aics_param_type;    /**< @ref rtk_bt_le_audio_aics_param_type_t */
    uint8_t                             value_len;          /**< The value length of p_value. */
    uint8_t                             *p_value;           /**< Pointer to the AICS parameter data. */
    bool                                set_change_counter; /**< Whether to set change counter. */
} rtk_bt_le_audio_aics_set_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_get_param_t
 * @brief     Bluetooth LE audio aics get paramter.
 */
typedef struct {
    uint8_t                             srv_instance_id;
    rtk_bt_le_audio_aics_param_type_t   aics_param_type;    /**< @ref rtk_bt_le_audio_aics_param_type_t, except RTK_BT_LE_AUDIO_AICS_PARAM_INPUT_DES*/
    uint8_t                             value_len;          /**< The value length of p_value. */
    uint8_t                             *p_value;           /**< Pointer to the AICS parameter data. */
} rtk_bt_le_audio_aics_get_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_cfg_cccd_param_t
 * @brief     Bluetooth LE audio aics config cccd parameter.
 */
typedef struct {	
    uint16_t conn_handle;			/**< Connection handle */
    uint8_t srv_instance_id;		/**< AICS service index */
    uint8_t cfg_flags;				/**< AICS CCCD Flag, @ref rtk_bt_le_audio_aics_cccd_flags_t*/
    bool enable;					/**< Whether to enable CCCD */
} rtk_bt_le_audio_aics_cfg_cccd_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_read_char_value_param_t
 * @brief     Bluetooth LE audio aics read char value parameter.
 */
typedef struct {
    uint16_t conn_handle;					/**<  Connection handle */
    uint8_t srv_instance_id;				/**<  AICS service index */
    rtk_bt_le_audio_aics_char_type_t type;  /**<  AICS characteristic type, @ref rtk_bt_le_audio_aics_char_type_t */
} rtk_bt_le_audio_aics_read_char_value_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_write_cp_param_t
 * @brief     Bluetooth LE audio aics write control point parameter.
 */
typedef struct {
    uint16_t conn_handle;					  /**<  Connection handle */
    uint8_t srv_instance_id;				  /**<  AICS service index */
    rtk_bt_le_audio_aics_cp_op_t cp_op;       /**<  AICS control point opcode, @ref rtk_bt_le_audio_aics_cp_op_t */
    rtk_bt_le_audio_aics_cp_param_t cp_param; /**<  AICS control point parameter, @ref rtk_bt_le_audio_aics_cp_param_t */
} rtk_bt_le_audio_aics_write_cp_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_write_cp_by_group_param_t
 * @brief     Bluetooth LE audio aics write control point parameter.
 */
typedef struct {
    rtk_bt_le_audio_group_handle_t group_handle; 	/**< @ref rtk_bt_le_audio_group_handle_t. */ 
    uint8_t srv_instance_id;						/**<  AICS service index */
    rtk_bt_le_audio_aics_cp_op_t cp_op;       		/**<  AICS control point opcode, @ref rtk_bt_le_audio_aics_cp_op_t */
    rtk_bt_le_audio_aics_cp_param_t cp_param;   	/**<  AICS control point parameter, @ref rtk_bt_le_audio_aics_cp_param_t */
} rtk_bt_le_audio_aics_write_cp_by_group_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_write_input_des_param_t
 * @brief     Bluetooth LE audio aics write input descriptor parameter.
 */
typedef struct {
    uint16_t conn_handle;							/**<  Connection handle */
    uint8_t srv_instance_id;						/**<  AICS service index */
    uint16_t input_des_len;                         /**<  The length of the input descriptor value */
    uint8_t *p_input_des;                           /**<  Point to the input descriptor value. */
} rtk_bt_le_audio_aics_write_input_des_param_t;

/**
 * @typedef    rtk_bt_le_audio_aics_get_srv_data_param_t
 * @brief     Bluetooth LE audio aics get service data parameter.
 */
typedef struct {
    uint16_t conn_handle;							/**<  Connection handle */
    uint8_t srv_instance_id;						/**<  AICS service index */
    rtk_bt_le_audio_aics_srv_data_t *p_srv_data;   /**<   returned service data, @ref rtk_bt_le_audio_aics_srv_data_t */
} rtk_bt_le_audio_aics_get_srv_data_param_t;
#endif 
/****************** Data structure for API func param pack and unpack ******************/

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_le_audio BT LE AUDIO APIs
 * @brief     BT LE AUDIO related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_le_audio_iso_data_send(rtk_bt_le_audio_iso_data_send_info_t *info)
 * @brief     Send ISO data .
 * @param[in] info: The info of iso send data
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_iso_data_send(rtk_bt_le_audio_iso_data_send_info_t *info);

/**
 * @fn        uint16_t rtk_bt_le_audio_get_prefer_codec_cfg(uint8_t item, rtk_bt_le_audio_cfg_codec_t *p_cfg_codec)
 * @brief     BLE AUDIO GET PREFER CODEC CONFIG.
 * @param[in] item: codec item
 * @param[out] p_cfg_codec: returned cfg codec
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_get_prefer_codec_cfg(uint8_t item, rtk_bt_le_audio_cfg_codec_t *p_cfg_codec);

/**
 * @fn        uint16_t rtk_bt_le_audio_qos_preferred_cfg_get(uint8_t codec_cfg_item, uint8_t qos_cfg_type, rtk_bt_le_audio_qos_cfg_preferred_t *p_qos_cfg)
 * @brief     BLE AUDIO GET PREFER CODEC CONFIG.
 * @param[in] codec_cfg_item: codec item
 * @param[in] qos_cfg_type: qos cfg type
 * @param[out] p_qos_cfg: returned prefer qos cfg
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_qos_preferred_cfg_get(rtk_bt_le_audio_codec_cfg_item_t codec_cfg_item, rtk_bt_le_audio_qos_cfg_type_t qos_cfg_type, rtk_bt_le_audio_qos_cfg_preferred_t *p_qos_cfg);

/**
 * @fn        uint16_t rtk_bt_le_audio_pacs_get_info(uint16_t conn_handle, rtk_bt_le_audio_bap_pacs_info_t *p_pacs_info)
 * @brief     BLE AUDIO GET PACS INFO.
 * @param[in] conn_handle: conn handle
 * @param[out] p_pacs_info: returned pacs info
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pacs_get_info(uint16_t conn_handle, rtk_bt_le_audio_bap_pacs_info_t *p_pacs_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_pacs_get_pac_record(uint16_t conn_handle, rtk_bt_le_audio_role_t audio_role, uint8_t *p_pac_num,rtk_bt_le_audio_bap_pacs_pac_record_t * p_pac_tbl)
 * @brief     BLE AUDIO QUERY PAC RECORD[ASYNC].
 *            If sending query operation is success, the result will be returned by RTK_BT_LE_AUDIO_EVT_PAC_RECORD_IND. 
 * @param[in] conn_handle: conn handle
 * @param[in] audio_role: audio role
 * @param[in] p_pac_num: pac num
 * @param[in] p_pac_tbl: pac table
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pacs_get_pac_record(uint16_t conn_handle, rtk_bt_le_audio_role_t audio_role, uint8_t *p_pac_num, rtk_bt_le_audio_bap_pacs_pac_record_t * p_pac_tbl);

/**
 * @fn        uint16_t rtk_bt_le_audio_pacs_get_lc3_table_mask(uint16_t conn_handle, rtk_bt_le_audio_role_t audio_role, uint16_t pref_audio_contexts, uint8_t channel_count, uint32_t *p_lc3_table_mask)
 * @brief     BLE AUDIO QUERY PACS AUDIO SUPPORT TABLE MASK.
 * @param[in] conn_handle: conn handle
 * @param[in] audio_role: audio role
 * @param[in] pref_audio_contexts: prefer audio context
 * @param[in] channel_count: channel count
 * @param[out] p_lc3_table_mask: returned lc3 suppport table mask . @ref rtk_bt_le_audio_codec_cfg_item_bit_mask_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pacs_get_lc3_table_mask(uint16_t conn_handle, rtk_bt_le_audio_role_t audio_role, uint16_t pref_audio_contexts, uint8_t channel_count, uint32_t *p_lc3_table_mask);

/**
 * @fn        uint16_t rtk_bt_le_audio_pacs_get_bis_array_by_sync_info(uint16_t conn_handle, rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t filter, uint32_t *p_bis_array)
 * @brief     BLE AUDIO PACS GET BIS ARRAY BY SYNC INFO.
 * @param[in] conn_handle: Connection handle.
 * @param[in] sync_handle: BLE audio sync handle: @ref rtk_bt_le_audio_sync_handle_t.
 * @param[in] filter: Check filter type: @ref rtk_bt_le_audio_pacs_ba_check_filter_type_t.
 * @param[out] p_bis_array: bis array
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pacs_get_bis_array_by_sync_info(uint16_t conn_handle, rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t filter, uint32_t *p_bis_array);

//#if (defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) || (defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT)

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_allocate(uint8_t adv_sid,rtk_bt_le_addr_t adv_addr,uint8_t *broadcast_id,rtk_bt_le_audio_sync_handle_t * p_broadcast_sync_handle)
 * @brief     BLE AUDIO SYNC CREATE. 
 * @param[in]  adv_sid: paramsters for le audio sync create
 * @param[in]  adv_addr: address of broadcast source 
 * @param[in]  broadcast_id: broadcast id of broadcast source 
 * @param[out]  p_broadcast_sync_handle: returned synchronization handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_allocate(uint8_t adv_sid,rtk_bt_le_addr_t adv_addr,uint8_t *broadcast_id,rtk_bt_le_audio_sync_handle_t * p_broadcast_sync_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_release(rtk_bt_le_audio_sync_handle_t sync_handle)
 * @brief     BLE AUDIO SYNC RELEASE. 
 * @param[in]  sync_handle: Synchronization Handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_release(rtk_bt_le_audio_sync_handle_t sync_handle); 

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_find(uint8_t adv_sid, uint8_t *broadcast_id,rtk_bt_le_audio_sync_handle_t * p_broadcast_sync_handle)
 * @brief     BLE AUDIO SYNC FIND. 
 * @param[in]  adv_sid: paramsters for le audio sync create
 * @param[in]  broadcast_id: broadcast id of broadcast source 
 * @param[out]  p_broadcast_sync_handle: returned synchronization handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_find(uint8_t adv_sid, uint8_t *broadcast_id,rtk_bt_le_audio_sync_handle_t * p_broadcast_sync_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_pa_sync_establish(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_pa_sync_establish_t * p_establish)
 * @brief     BLE AUDIO PA SYNC ESTABLISH. 
 * @param[in]  sync_handle: Synchronization Handle
 * @param[in]  p_establish: paramsters for le audio pa sync establish
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pa_sync_establish(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_pa_sync_establish_t * p_establish);

/**
 * @fn        uint16_t rtk_bt_le_audio_pa_sync_terminate(rtk_bt_le_audio_sync_handle_t sync_handle)
 * @brief     BLE AUDIO PA SYNC TERMINATE. 
 * @param[in]  sync_handle: Synchronization Handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_pa_sync_terminate(rtk_bt_le_audio_sync_handle_t sync_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_sync_info(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_sync_info_t *p_sync_info)
 * @brief     BLE AUDIO BIG SYNC INFO GET. 
 * @param[in]  sync_handle: Synchronization Handle
 * @param[out] p_sync_info: the return sync information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_sync_info(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_sync_info_t *p_sync_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_bis_sync_codec_cfg(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx, rtk_bt_le_audio_cfg_codec_t *p_codec_cfg)
 * @brief     BLE AUDIO SYNC BIS CODEC CFG GET. 
 * @param[in]  sync_handle: Synchronization Handle
 * @param[in]  bis_idx: bis index
 * @param[out] p_codec_cfg: the return codec config
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_bis_sync_codec_cfg(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx, rtk_bt_le_audio_cfg_codec_t *p_codec_cfg);

//#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_big_sync_establish(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_big_sync_establish_t * p_establish)
 * @brief     BLE AUDIO BIG SYNC ESTABLISH. 
 * @param[in]  sync_handle: Synchronization Handle
 * @param[in]  p_establish: paramsters for le audio pa sync establish
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_big_sync_establish(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_big_sync_establish_t * p_establish);

/**
 * @fn        uint16_t rtk_bt_le_audio_big_sync_terminate(rtk_bt_le_audio_sync_handle_t sync_handle)
 * @brief     BLE AUDIO BIG SYNC TERMINATE. 
 * @param[in] sync_handle: Synchronization Handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_big_sync_terminate(rtk_bt_le_audio_sync_handle_t sync_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_bis_info(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_bis_info_t *p_bis_info)
 * @brief     BLE AUDIO BIS SYNC INFO GET. 
 * @param[in] sync_handle: Synchronization Handle
 * @param[out] p_bis_info: the return bis sync information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_bis_info(rtk_bt_le_audio_sync_handle_t sync_handle, rtk_bt_le_audio_bis_info_t *p_bis_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_bis_conn_handle(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx, uint16_t *p_bis_conn_handle)
 * @brief     BLE AUDIO BIS SYNC CONN HANDLE GET. 
 * @param[in] sync_handle: Synchronization Handle
 * @param[in] bis_idx: bis idx
 * @param[out] p_bis_conn_handle: the return bis conn handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_bis_conn_handle(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx, uint16_t *p_bis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_get_support_bis_array(rtk_bt_le_audio_sync_handle_t sync_handle, uint32_t sink_audio_location, uint32_t *p_support_bis_array)
 * @brief     BLE AUDIO BIS SYNC SUPPORT BIS ARRAY GET. 
 * @param[in] sync_handle: Synchronization Handle
 * @param[in] sink_audio_location: sink audio channel allocation
 * @param[out] p_support_bis_array: the return support bis array
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_get_support_bis_array(rtk_bt_le_audio_sync_handle_t sync_handle, uint32_t sink_audio_location, uint32_t *p_support_bis_array);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_setup_data_path(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx, rtk_bt_le_audio_bis_data_path_param_t *p_setup_param)
 * @brief     BLE AUDIO SETUP BIS DATA PATH. 
 * @param[in] sync_handle: Synchronization Handle
 * @param[in] bis_idx: bis index
 * @param[in] p_setup_param: paramsters for le audio bis data path setup
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_setup_data_path(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx, rtk_bt_le_audio_bis_data_path_param_t *p_setup_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_sync_remove_data_path(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx)
 * @brief     BLE AUDIO REMOVE BIS DATA PATH. 
 * @param[in]  sync_handle: Synchronization Handle
 * @param[in]  bis_idx: bis index
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_sync_remove_data_path(rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t bis_idx);
//#endif 
//#endif //#if defined(RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SINK_SUPPORT

//#if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_create(uint8_t num_groups, rtk_bt_le_audio_broadcast_source_group_param_t *group, rtk_bt_le_audio_broadcast_source_handle_t *p_broadcast_source_handle)
 * @brief     BLE AUDIO BROADCAST SOURCE CREATE
 * @param[in]  num_groups: num of broadcast source groups
 * @param[inout]  group: broadcast source groups parameter
 * @param[out]  p_broadcast_source_handle: broadcast source handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_create(uint8_t num_groups, rtk_bt_le_audio_broadcast_source_group_param_t *group, rtk_bt_le_audio_broadcast_source_handle_t *p_broadcast_source_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_get_info(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, rtk_bt_le_audio_broadcast_source_info_t *p_info)
 * @brief     BLE AUDIO BROADCAST SOURCE GET INFO 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @param[out] p_info: the returned broadcast source information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_get_info(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, rtk_bt_le_audio_broadcast_source_info_t *p_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_config(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t adv_sid, uint8_t group_idx, uint8_t *p_broadcast_audio_announcements, uint16_t p_announcements_len, rtk_bt_le_audio_broadcast_source_config_t *p_config);
 * @brief     BLE AUDIO BROADCAST SOURCE CONFIG 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @param[in]  adv_sid: Value of the Advertising SID subfield in the ADI field of the PDU, Range: 0x00 to 0x0F
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  p_broadcast_audio_announcements: broadcast audio announcements
 * @param[in]  p_announcements_len: Length of broadcast audio announcements
 * @param[in]  p_config: broadcast source config information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_config(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t adv_sid, uint8_t group_idx, uint8_t *p_broadcast_audio_announcements, uint16_t p_announcements_len, rtk_bt_le_audio_broadcast_source_config_t *p_config);
/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_enable(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, rtk_bt_le_audio_broadcast_source_create_big_t *p_create_big)
 * @brief     BLE AUDIO BROADCAST SOURCE ENABLE 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @param[in]  p_create_big: broadcast source enable information
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_enable(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, rtk_bt_le_audio_broadcast_source_create_big_t *p_create_big);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_disable(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle)
 * @brief     BLE AUDIO BROADCAST SOURCE DISABLE 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_disable(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_release(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle)
 * @brief     BLE AUDIO BROADCAST SOURCE RELEASE 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_release(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_setup_data_path(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle,uint8_t bis_idx, rtk_bt_le_audio_bis_data_path_param_t *p_setup, uint16_t * p_bis_conn_handle)
 * @brief     BLE AUDIO BROADCAST SORUCE SETUP BIS DATA PATH. 
 * @param[in] broadcast_source_handle: broadcast source handle
 * @param[in] bis_idx: bis index
 * @param[in] p_setup: paramsters for le audio bis data path setup
 * @param[out] p_bis_conn_handle: Return Connection handle of a BIS
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_setup_data_path(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t bis_idx,rtk_bt_le_audio_bis_data_path_param_t *p_setup, uint16_t * p_bis_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_remove_data_path(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t bis_idx)
 * @brief     BLE AUDIO BROADCAST SORUCE REMOVE BIS DATA PATH. 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @param[in]  bis_idx: bis index
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_remove_data_path(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t bis_idx);

/**
 * @fn        uint16_t rtk_bt_le_audio_broadcast_source_reconfig(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t group_idx, uint8_t subgroup_idx, uint8_t metadata_len, uint8_t *p_metadata)
 * @brief     BLE AUDIO BROADCAST SOURCE RECONFIG. 
 * @param[in]  broadcast_source_handle: broadcast source handle
 * @param[in]  group_idx: broadcast source group idx
 * @param[in]  subgroup_idx: broadcast source subgroup idx
 * @param[in]  metadata_len: metadata len
 * @param[in]  p_metadata: p_metadata
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_broadcast_source_reconfig(rtk_bt_le_audio_broadcast_source_handle_t broadcast_source_handle, uint8_t group_idx, uint8_t subgroup_idx, uint8_t metadata_len, uint8_t *p_metadata);

//#endif //end of #if defined(RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_SOURCE_SUPPORT

//#if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_bass_get_brs_data(uint16_t conn_handle,uint8_t char_instance_id, bool is_get_bis_info, rtk_bt_le_audio_bass_brs_data_t *p_brs_data)
 * @brief     BLE AUDIO BASS GET BROADCAST RECEIVE STATE CHAR DATA. 
 * @param[in]  conn_handle: Connection handle
 * @param[in]  char_instance_id: char instance id;
 * @param[in]  is_get_bis_info: whether get cp_bis_info in brs_data, the returned p_cp_bis_info is NULL when it is false 
 * @param[out]  p_brs_data: broadcast receive state data @ref rtk_bt_le_audio_bass_brs_data_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_bass_get_brs_data(uint16_t conn_handle,uint8_t char_instance_id,bool is_get_bis_info,rtk_bt_le_audio_bass_brs_data_t *p_brs_data);

/**
 * @fn        uint16_t rtk_bt_le_audio_bass_client_cp_add_source_by_sync_info(uint16_t conn_handle, rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t pa_sync,uint32_t bis_array,bool is_req)
 * @brief     BLE AUDIO BASS WRITE CONCTOL POINT ADD SOURCE BY SYNC INFO. 
 * @param[in]  conn_handle: Connection handle
 * @param[in]  sync_handle: BLE audio sync handle
 * @param[in]  pa_sync: PA_Sync parameter @ref rtk_bt_le_audio_bass_cp_pa_sync_t
 * @param[in]  bis_array: BIS array
 * @param[in]  is_req: Whether to send write request
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_bass_client_cp_add_source_by_sync_info(uint16_t conn_handle, rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t pa_sync,uint32_t bis_array,bool is_req);

/**
 * @fn        uint16_t rtk_bt_le_audio_bass_client_cp_modify_source_by_sync_info(uint16_t conn_handle, rtk_bt_le_audio_sync_handle_t sync_handle, uint8_t source_id,uint8_t pa_sync,uint32_t bis_array,bool is_req)
 * @brief     BLE AUDIO BASS CLIENT WRITE CONCTOL POINT MODIFY SOURCE BY SYNC INFO. 
 * @param[in]  conn_handle: Connection handle
 * @param[in]  sync_handle: BLE audio sync handle
 * @param[in]  source_id: Broadcast source id.
 * @param[in]  pa_sync: PA_Sync parameter @ref rtk_bt_le_audio_bass_cp_pa_sync_t
 * @param[in]  bis_array: BIS array
 * @param[in]  is_req: Whether to send write request
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_bass_client_cp_modify_source_by_sync_info(uint16_t conn_handle, rtk_bt_le_audio_sync_handle_t sync_handle,uint8_t source_id, uint8_t pa_sync,uint32_t bis_array,bool is_req);

/**
 * @fn        uint16_t rtk_bt_le_audio_bass_client_write_cp_op(uint16_t conn_handle, uint8_t opcode, void * op_param, bool is_req)
 * @brief     BLE AUDIO BASS CLIENT WRITE CONCTOL POINT OPCODE. 
 * @param[in]  conn_handle: Connection handle
 * @param[in]  opcode: op code @ref rtk_bt_le_audio_bass_cp_op_t
 * @param[in]  op_param: op param
 * @param[in]  is_req: Whether to send write request
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_bass_client_write_cp_op(uint16_t conn_handle, uint8_t opcode, void * op_param, bool is_req);

/**
 * @fn        uint16_t rtk_bt_le_audio_bass_client_past(uint8_t past_type, void * handle, uint16_t conn_handle, rtk_bt_le_audio_bass_past_srv_data_t srv_data)
 * @brief     BLE AUDIO BASS PAST Initiating. 
 * @param[in]  past_type: past type  @ref rtk_bt_le_audio_bass_past_type_t
 * @param[in]  handle: handle: broadcast_source_handle or sync_handle
 * @param[in]  conn_handle: Connection handle
 * @param[in]  srv_data: PAST Service Data @ref rtk_bt_le_audio_bass_past_srv_data_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_bass_client_past(uint8_t past_type, void * handle, uint16_t conn_handle, rtk_bt_le_audio_bass_past_srv_data_t srv_data);

//#endif //end of #if defined(RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT) && RTK_BLE_AUDIO_BROADCAST_ASSISTANT_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_group_allocate(rtk_bt_le_audio_group_handle_t *p_group_handle)
 * @brief     BLE AUDIO UNICAST CLIENT ALLOCATE GROUP. 
 * @param[out]  p_group_handle: unicast client group handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_allocate(rtk_bt_le_audio_group_handle_t *p_group_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_release(rtk_bt_le_audio_group_handle_t group_handle)
 * @brief     BLE AUDIO UNICAST CLIENT RELEASE GROUP. 
 * @param[in]  group_handle: unicast client group handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_release(rtk_bt_le_audio_group_handle_t group_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_add_device(rtk_bt_le_audio_group_handle_t group_handle, uint16_t conn_handle, rtk_bt_le_audio_device_handle_t *p_device_handle)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP ADD DEVICE. 
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  conn_handle: add device conn_handle
 * @param[out]  p_device_handle: unicast client device handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_add_device(rtk_bt_le_audio_group_handle_t group_handle, uint16_t conn_handle, rtk_bt_le_audio_device_handle_t *p_device_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_delete_device(rtk_bt_le_audio_group_handle_t group_handle, rtk_bt_le_audio_device_handle_t device_handle)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP REMOVE DEVICE.  
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  device_handle: unicast client device handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_delete_device(rtk_bt_le_audio_group_handle_t group_handle, rtk_bt_le_audio_device_handle_t device_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_find_device(rtk_bt_le_audio_group_handle_t group_handle, rtk_bt_le_addr_t dev_addr, rtk_bt_le_audio_device_handle_t *p_device_handle)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP FIND DEVICE. 
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  dev_addr: find device address
 * @param[out]  p_device_handle: unicast client device handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_find_device(rtk_bt_le_audio_group_handle_t group_handle, rtk_bt_le_addr_t dev_addr, rtk_bt_le_audio_device_handle_t *p_device_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_find_device_by_conn_handle(rtk_bt_le_audio_group_handle_t group_handle, uint16_t conn_handle, rtk_bt_le_audio_device_handle_t *p_device_handle)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP FIND DEVICE BY CONN HANDLE. 
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  conn_handle: find device conn_handle
 * @param[out]  p_device_handle: unicast client device handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_find_device_by_conn_handle(rtk_bt_le_audio_group_handle_t group_handle, uint16_t conn_handle, rtk_bt_le_audio_device_handle_t *p_device_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_get_device_num(rtk_bt_le_audio_group_handle_t group_handle, uint8_t *p_device_num)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP FIND DEVICE NUM. 
 * @param[in]  group_handle: unicast client group handle
 * @param[out]  p_device_num: total device num
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_get_device_num(rtk_bt_le_audio_group_handle_t group_handle, uint8_t *p_device_num);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_get_used_device_num(rtk_bt_le_audio_group_handle_t group_handle, bool check_conn, uint8_t *p_used_device_num)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP FIND USED DEVICE NUM. 
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  check_conn: whether check conn
 * @param[out]  p_used_device_num: used device num
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_get_used_device_num(rtk_bt_le_audio_group_handle_t group_handle, bool check_conn, uint8_t *p_used_device_num);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_get_device_info(rtk_bt_le_audio_group_handle_t group_handle, rtk_bt_le_audio_device_handle_t device_handle, rtk_bt_le_audio_group_device_info_t *p_device_info)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP GET DEVICE INFO. 
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  device_handle: unicast client device handle
 * @param[out] p_device_info: returned device info
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_get_device_info(rtk_bt_le_audio_group_handle_t group_handle, rtk_bt_le_audio_device_handle_t device_handle, rtk_bt_le_audio_group_device_info_t *p_device_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_group_get_all_device_info(rtk_bt_le_audio_group_handle_t group_handle, uint8_t *device_num, rtk_bt_le_audio_group_device_info_t *p_device_info_tbl)
 * @brief     BLE AUDIO UNICAST CLIENT GROUP GET ALL DEVICE INFO. 
 * @param[in]  group_handle: unicast client group handle
 * @param[in]  device_num: device num
 * @param[out] p_device_info_tbl: returned device info table
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_group_get_all_device_info(rtk_bt_le_audio_group_handle_t group_handle, uint8_t *device_num, rtk_bt_le_audio_group_device_info_t *p_device_info_tbl);

//#if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_stream_session_allocate(rtk_bt_le_audio_group_handle_t group_handle,rtk_bt_le_audio_stream_session_handle_t *p_stream_session_handle)
 * @brief     BLE AUDIO UNICAST CLIENT ALLOCATE STEAM SESSION. 
 * @param[in]  group_handle: unicast client group handle
 * @param[out] p_stream_session_handle: returned stream session handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_stream_session_allocate(rtk_bt_le_audio_group_handle_t group_handle,rtk_bt_le_audio_stream_session_handle_t *p_stream_session_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_stream_session_release(rtk_bt_le_audio_stream_session_handle_t stream_session_handle)
 * @brief     BLE AUDIO UNICAST CLIENT RELEASE STREAM SESSION.
 * @param[in] stream_session_handle: stream session handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_stream_session_release(rtk_bt_le_audio_stream_session_handle_t stream_session_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_stream_session_get_group_handle(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_group_handle_t *p_group_handle)
 * @brief     BLE AUDIO UNICAST CLIENT RELEASE STEAM SESSION. 
 * @param[in] stream_session_handle: stream session handle
 * @param[out] p_group_handle: returned group handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_stream_session_get_group_handle(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_group_handle_t *p_group_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_get_session_info(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_unicast_session_info_t *p_session_info)
 * @brief     BLE AUDIO UNICAST CLIENT GET SESSION INFO. 
 * @param[in] stream_session_handle: stream session handle
 * @param[out] p_session_info: returned session info
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_get_session_info(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_unicast_session_info_t *p_session_info);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_get_cfg_mask(rtk_bt_le_audio_stream_session_handle_t stream_session_handle,uint32_t prefer_cfg,uint8_t device_num,rtk_bt_le_audio_device_handle_t *p_device_handle_tbl,uint32_t *p_cfg_mask)
 * @brief     BLE AUDIO UNICAST CLIENT GET AUDIO CFG MASK. 
 * @param[in] stream_session_handle: stream session handle
 * @param[in] prefer_cfg: prefer cfg
 * @param[in] device_num: device num
 * @param[in] p_device_handle_tbl: device num
 * @param[out] p_cfg_mask: returned cfg mask
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_get_cfg_mask(rtk_bt_le_audio_stream_session_handle_t stream_session_handle,uint32_t prefer_cfg,uint8_t device_num,rtk_bt_le_audio_device_handle_t *p_device_handle_tbl,uint32_t *p_cfg_mask);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_get_cis_info(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,rtk_bt_le_audio_unicast_cis_infos_t *p_cis_infos)
 * @brief     BLE AUDIO UNICAST CLIENT RELEASE STEAM SESSION. 
 * @param[in] stream_session_handle: stream session handle
 * @param[in] device_handle: device handle
 * @param[out] p_cis_infos: returned cis info table
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_get_cis_info(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,rtk_bt_le_audio_unicast_cis_infos_t *p_cis_infos);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_config(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_unicast_cfg_type_t cfg_type,uint8_t device_num,rtk_bt_le_audio_device_handle_t *p_device_handle_tbl)
 * @brief     BLE AUDIO UNICAST CLIENT CONFIG. 
 * @param[in] stream_session_handle: stream session handle
 * @param[in] cfg_type: config type
 * @param[in] device_num: device num
 * @param[in] p_device_handle_tbl: device num
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_config(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_unicast_cfg_type_t cfg_type,uint8_t device_num,rtk_bt_le_audio_device_handle_t *p_device_handle_tbl);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_cfg_ase_codec(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,uint8_t cfg_idx,rtk_bt_le_audio_unicast_ase_codec_t ase_codec)
 * @brief     BLE AUDIO UNICAST CLIENT CONFIG ASE CODEC. 
 * @param[in] stream_session_handle: stream session handle
 * @param[in] device_handle: device handle
 * @param[in] cfg_idx: cfg idx
 * @param[in] ase_codec: ase codec
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_cfg_ase_codec(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,uint8_t cfg_idx,rtk_bt_le_audio_unicast_ase_codec_t ase_codec);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_remove_config(rtk_bt_le_audio_stream_session_handle_t stream_session_handle)
 * @brief     BLE AUDIO UNICAST CLIENT REMOVE CONFIG. 
 * @param[in] stream_session_handle: stream session handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_remove_config(rtk_bt_le_audio_stream_session_handle_t stream_session_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_get_session_qos(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_unicast_session_qos_t *p_session_qos)
 * @brief     BLE AUDIO UNICAST CLIENT GET SESSION QOS.
 * @param[in] stream_session_handle: stream session handle
 * @param[out] p_session_qos: session qos
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_get_session_qos(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_unicast_session_qos_t *p_session_qos);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_cfg_ase_qos(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,uint8_t ase_id,rtk_bt_le_audio_unicast_ase_qos_t ase_qos)
 * @brief     BLE AUDIO UNICAST CLIENT CONFIG ASE QOS. 
 * @param[in] stream_session_handle: stream session handle
 * @param[in] device_handle: device handle
 * @param[in] ase_id: ase id
 * @param[in] ase_qos: ase qos
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_cfg_ase_qos(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,uint8_t ase_id,rtk_bt_le_audio_unicast_ase_qos_t ase_qos);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_get_ase_qos(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,uint8_t ase_id,rtk_bt_le_audio_unicast_ase_qos_t *p_ase_qos)
 * @brief     BLE AUDIO UNICAST CLIENT GET ASE QOS.
 * @param[in] stream_session_handle: stream session handle
 * @param[in] device_handle: device handle
 * @param[in] ase_id: ase id
 * @param[out] p_ase_qos: ase qos
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_get_ase_qos(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, rtk_bt_le_audio_device_handle_t device_handle,uint8_t ase_id,rtk_bt_le_audio_unicast_ase_qos_t *p_ase_qos);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_start(rtk_bt_le_audio_stream_session_handle_t stream_session_handle)
 * @brief     BLE AUDIO UNICAST CLIENT START. 
 * @param[in] stream_session_handle: stream session handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_start(rtk_bt_le_audio_stream_session_handle_t stream_session_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_stop(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, uint32_t cis_timeout_ms)
 * @brief     BLE AUDIO UNICAST CLIENT STOP. 
 * @param[in] stream_session_handle: stream session handle
 * @param[in] cis_timeout_ms: cis timeout for stop
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_stop(rtk_bt_le_audio_stream_session_handle_t stream_session_handle, uint32_t cis_timeout_ms);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_release(rtk_bt_le_audio_stream_session_handle_t stream_session_handle)
 * @brief     BLE AUDIO UNICAST CLIENT RELEASE. 
 * @param[in] stream_session_handle: stream session handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_release(rtk_bt_le_audio_stream_session_handle_t stream_session_handle);

/**
 * @fn        uint16_t rtk_bt_le_audio_unicast_update(rtk_bt_le_audio_stream_session_handle_t stream_session_handle)
 * @brief     BLE AUDIO UNICAST CLIENT UPDATE. 
 * @param[in] stream_session_handle: stream session handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_unicast_update(rtk_bt_le_audio_stream_session_handle_t stream_session_handle);

//#endif //end of #if defined(RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT) && RTK_BLE_AUDIO_UNICAST_CLIENT_SUPPORT

#if defined(RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_MEMBER_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_gen_rsi(uint8_t *p_sirk, uint8_t *p_rsi)
 * @brief     BLE AUDIO generate RSI for CSIS. 
 * @param[in] p_sirk: sirk, set identity resolving key
 * @param[out] p_rsi: rsi, resolvable set identifier
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_gen_rsi(uint8_t *p_sirk, uint8_t *p_rsi);

/**
 * @fn        uint16_t rtk_bt_le_audio_csis_update_sirk(rtk_bt_le_audio_csis_sirk_type_t sirk_type,uint8_t *p_sirk)
 * @brief     BLE AUDIO change SIRK for CSIS. 
 * @param[in] sirk_type: sirk type
 * @param[out] p_sirk: new sirk, set identity resolving key
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_update_sirk(rtk_bt_le_audio_csis_sirk_type_t sirk_type,uint8_t *p_sirk);
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_update_size(uint8_t csis_size)
 * @brief     BLE AUDIO change SIZE for CSIS. 
 * @param[in] csis_size: csis size
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_update_size(uint8_t csis_size);
#endif

#if defined(RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT) && RTK_BLE_AUDIO_CSIP_SET_COORDINATOR_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi(rtk_bt_le_addr_t adv_addr,uint8_t report_data_len,uint8_t *p_report_data)
 * @brief     BLE AUDIO CHEK ADV RSI for CSIS SET COORDINATOR. 
 * @param[in] adv_addr: adv addr
 * @param[in] report_data_len: report data len
 * @param[in] p_report_data: report data
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_set_coordinator_check_adv_rsi(rtk_bt_le_addr_t adv_addr,uint8_t report_data_len,uint8_t *p_report_data);
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_set_coordinator_cfg_discover(rtk_bt_le_audio_group_handle_t group_handle,bool discover,uint32_t timeout_ms)
 * @brief     BLE AUDIO CONFIG DISCOVER for CSIS SET COORDINATOR. 
 * @param[in] group_handle: group handle
 * @param[in] discover: discover
 * @param[in] timeout_ms: timeout,unit:ms
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_set_coordinator_cfg_discover(rtk_bt_le_audio_group_handle_t group_handle,bool discover,uint32_t timeout_ms);
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_set_coordinator_add_group(rtk_bt_le_audio_csis_set_mem_info_t *p_mem_info,rtk_bt_le_audio_group_handle_t *p_group_handle,rtk_bt_le_audio_device_handle_t *p_device_handle)
 * @brief     BLE AUDIO CHEK ADD GROUP for CSIS SET COORDINATOR. 
 * @param[in] p_mem_info: mem info
 * @param[out] p_group_handle: returned group handle
 * @param[out] p_device_handle: returned device handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_set_coordinator_add_group(rtk_bt_le_audio_csis_set_mem_info_t *p_mem_info,rtk_bt_le_audio_group_handle_t *p_group_handle,rtk_bt_le_audio_device_handle_t *p_device_handle);
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_set_coordinator_add_device(rtk_bt_le_audio_group_handle_t group_handle,rtk_bt_le_audio_csis_set_mem_info_t *p_mem_info,rtk_bt_le_audio_device_handle_t *p_device_handle)
 * @brief     BLE AUDIO ADD GROUP for CSIS SET COORDINATOR. 
 * @param[in] group_handle: group handle
 * @param[in] p_mem_info: mem info
 * @param[out] p_device_handle: returned device handle
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_set_coordinator_add_device(rtk_bt_le_audio_group_handle_t group_handle,rtk_bt_le_audio_csis_set_mem_info_t *p_mem_info,rtk_bt_le_audio_device_handle_t *p_device_handle);
/**
 * @fn        uint16_t rtk_bt_le_audio_csis_set_coordinator_write_lock(rtk_bt_le_audio_group_handle_t group_handle,bool lock)
 * @brief     BLE AUDIO WRITE LOCK for CSIS SET COORDINATOR. 
 * @param[in] group_handle: group handle
 * @param[in] lock: lock or unlock
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_csis_set_coordinator_write_lock(rtk_bt_le_audio_group_handle_t group_handle,bool lock);
#endif

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_CLIENT_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_client_write_media_cp(uint16_t conn_handle,uint8_t srv_instance_id,bool general_mcs,bool is_req,uint8_t opcode,void * p_op_param)
 * @brief     BLE AUDIO MCS CLIENT WRITE MEDIA CP. If write operation is successful and Media Control Server sends Media Control Point Notification,
 *              Media Control Point Notification will be returned by callback registered by @ref rtk_bt_evt_register_callback with msg @ref RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_MEDIA_CP_NOTIFY_IND.
 * @param[in] conn_handle: connection handle
 * @param[in] srv_instance_id: Service Instance Id
 * @param[in] general_mcs: is generic media control service (GMCS)
 * @param[in] is_req: is request, true: Write request, false: Write command 
 * @param[in] opcode: op code. @ref rtk_bt_le_audio_mcs_media_cp_opcode_t
 * @param[in] p_op_param: op code param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_client_write_media_cp(uint16_t conn_handle,uint8_t srv_instance_id,bool general_mcs,bool is_req,uint8_t opcode,void * p_op_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_client_read_char_value(uint16_t conn_handle,uint8_t srv_instance_id,bool general_mcs,uint16_t char_uuid)
 * @brief     BLE AUDIO MCS CLIENT READ CHAR VALUE. If read operation is successful, the result of read characteristic value will be returned by
 *              callback registered by @ref rtk_bt_evt_register_callback with msg @ref RTK_BT_LE_AUDIO_EVT_MCP_CLIENT_READ_RESULT_IND.
 * @param[in] conn_handle: connection handle
 * @param[in] srv_instance_id: Service Instance Id
 * @param[in] general_mcs: is generic media control service (GMCS)
 * @param[in] char_uuid: char_uuid, only support RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME,RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE,RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_DURATION,
 *                      RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_POSITION, RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_STATE, RTK_BT_LE_AUDIO_MCS_UUID_CHAR_CONTENT_CONTROL_ID
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_client_read_char_value(uint16_t conn_handle,uint8_t srv_instance_id,bool general_mcs,uint16_t char_uuid);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_client_cfg_cccd(uint16_t conn_handle,uint8_t srv_instance_id,bool general_mcs,uint32_t cfg_flags,bool enable)
 * @brief     BLE AUDIO MCS CLIENT CONFIG CCCD. 
 * @param[in] conn_handle: connection handle
 * @param[in] srv_instance_id: Service Instance Id
 * @param[in] general_mcs: is generic media control service (GMCS)
 * @param[in] cfg_flags: cfg flags
 * @param[in] enable: enable
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_client_cfg_cccd(uint16_t conn_handle,uint8_t srv_instance_id,bool general_mcs,uint32_t cfg_flags,bool enable);
#endif 

#if defined(RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT) && RTK_BLE_AUDIO_MCP_MEDIA_CONTROL_SERVER_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_server_set_param(uint8_t service_id, uint16_t char_uuid, void * p_param)
 * @brief     BLE AUDIO MCS SERVER SET PARAM. 
 * @param[in] service_id: service id
 * @param[in] char_uuid: char uuid
 * @param[in] p_param: param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_server_set_param(uint8_t service_id, uint16_t char_uuid, void * p_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_server_send_data(uint8_t service_id,uint16_t char_uuid, void * p_param)
 * @brief     BLE AUDIO MCS SERVER SEND DATA. 
 * @param[in] service_id: service id
 * @param[in] char_uuid: char uuid, only support RTK_BT_LE_AUDIO_MCS_UUID_CHAR_MEDIA_PLAYER_NAME,RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_TITLE,RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_DURATION,
 *                      RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_POSITION, RTK_BT_LE_AUDIO_MCS_UUID_CHAR_TRACK_CHANGED
 * @param[in] p_param: param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_server_send_data(uint8_t service_id,uint16_t char_uuid, void * p_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_mcs_server_read_confirm(uint16_t conn_handle, uint16_t cid, uint8_t service_id,uint16_t char_uuid, uint16_t offset, void * p_param)
 * @brief     BLE AUDIO MCS SERVER SET PARAM. 
 * @param[in] conn_handle: conn handle
 * @param[in] cid: cid
 * @param[in] service_id: service id
 * @param[in] char_uuid: char uuid
 * @param[in] offset: offset
 * @param[in] p_param: param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mcs_server_read_confirm(uint16_t conn_handle, uint16_t cid, uint8_t service_id,uint16_t char_uuid, uint16_t offset, void * p_param);
#endif 

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_RENDERER_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_set_param(rtk_bt_le_audio_vcs_param_t *param)
 * @brief     BLE AUDIO VCS PARAM SET. 
 * @param[in]  param: vcs param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_set_param(rtk_bt_le_audio_vcs_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_get_param(rtk_bt_le_audio_vcs_param_t *param)
 * @brief     BLE AUDIO VCS PARAM GET. 
 * @param[out]  param: vcs param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_get_param(rtk_bt_le_audio_vcs_param_t *param);
#endif 

#if defined(RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_VCP_VOLUME_CONTROLLER_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_write_cp(uint16_t conn_handle,rtk_bt_le_audio_vcs_cp_op_t cp_op, rtk_bt_le_audio_vcs_cp_param_t *cp_param)
 * @brief     BLE AUDIO VCS SEND VOLUME CP. 
 * @param[in]  conn_handle: connection handle
 * @param[in]  cp_op:  cp op code 
 * @param[in]  cp_param: cp param 
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_write_cp(uint16_t conn_handle,rtk_bt_le_audio_vcs_cp_op_t cp_op, rtk_bt_le_audio_vcs_cp_param_t *cp_param);

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
 * @fn        uint16_t rtk_bt_le_audio_vcs_change_mute(rtk_bt_le_audio_group_handle_t group_handle,uint8_t mute)
 * @brief     BLE AUDIO VCS CHANGE MUTE STATE. 
 * @param[in]  group_handle: group handle
 * @param[in] mute:  vcs mute
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_change_mute(rtk_bt_le_audio_group_handle_t group_handle,uint8_t mute);

/**
 * @fn        uint16_t rtk_bt_le_audio_vcs_change_volume(rtk_bt_le_audio_group_handle_t group_handle,uint8_t volume_setting)
 * @brief     BLE AUDIO VCS CHANGE VOLUME SETTING. 
 * @param[in]  group_handle: group handle
 * @param[in] volume_setting:  volume setting
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vcs_change_volume(rtk_bt_le_audio_group_handle_t group_handle,uint8_t volume_setting);
#endif
#if defined(RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_CONTROLLER_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_mics_change_mute(rtk_bt_le_audio_group_handle_t group_handle,rtk_bt_le_audio_mics_mute_state_t mic_mute)
 * @brief     BLE AUDIO MICS CHANGE MUTE STATE. 
 * @param[in]  group_handle: group handle
 * @param[in] mic_mute:  mic mute
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mics_change_mute(rtk_bt_le_audio_group_handle_t group_handle,rtk_bt_le_audio_mics_mute_state_t mic_mute);

/**
 * @fn        uint16_t rtk_bt_le_audio_mics_set_mute_value(uint16_t conn_handle,rtk_bt_le_audio_mics_mute_state_t mic_mute)
 * @brief     BLE AUDIO MICS SET MUTE VALUE. 
 * @param[in]  conn_handle: connect handle
 * @param[in] mic_mute:  mic mute
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mics_set_mute_value(uint16_t conn_handle,rtk_bt_le_audio_mics_mute_state_t mic_mute);

/**
 * @fn        uint16_t rtk_bt_le_audio_mics_get_mute_value(uint16_t conn_handle,rtk_bt_le_audio_mics_mute_state_t *p_mic_mute)
 * @brief     BLE AUDIO MICS GET MUTE VALUE. 
 * @param[in]  conn_handle: connect handle
 * @param[in] p_mic_mute:  returned mic mute
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mics_get_mute_value(uint16_t conn_handle,rtk_bt_le_audio_mics_mute_state_t *p_mic_mute);
#endif

#if defined(RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT) && RTK_BLE_AUDIO_MICP_MIC_DEVICE_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_mics_set_param(rtk_bt_le_audio_mics_param_t *param)
 * @brief     BLE AUDIO MICS SET MUTE STATE. 
 * @param[in] param:  mics param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mics_set_param(rtk_bt_le_audio_mics_param_t *param);

/**
 * @fn        uint16_t rtk_bt_le_audio_mics_get_param(rtk_bt_le_audio_mics_param_t *param)
 * @brief     BLE AUDIO MICS SET MUTE STATE. 
 * @param[out] param:  returned mics param
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_mics_get_param(rtk_bt_le_audio_mics_param_t *param);
#endif

#if defined(RTK_BLE_AUDIO_VOCS_SUPPORT) && RTK_BLE_AUDIO_VOCS_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_set_param(uint8_t srv_instance_id,uint8_t set_mask,rtk_bt_le_audio_vocs_param_t *p_vocs_param)
 * @brief     BLE AUDIO VOCS SET PARAMETER. 
 * @param[in] srv_instance_id:  service instance id. Used when the number of services is greater than 1.
 * @param[in] set_mask:  set_mask
 * @param[in] p_vocs_param:  pointer to the location of volume offset control service parameter
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_set_param(uint8_t srv_instance_id,uint8_t set_mask,rtk_bt_le_audio_vocs_param_t *p_vocs_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_get_param(uint8_t srv_instance_id,rtk_bt_le_audio_vocs_param_t *p_vocs_param)
 * @brief     BLE AUDIO VOCS GET PARAMETER. 
 * @param[in] srv_instance_id:  service instance id. Used when the number of services is greater than 1.
 * @param[out] p_vocs_param:  returned the location of volume offset control service parameter
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_get_param(uint8_t srv_instance_id,rtk_bt_le_audio_vocs_param_t *p_vocs_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_cfg_cccd(uint16_t conn_handle, uint8_t srv_instance_id, uint8_t cfg_flags, bool enable)
 * @brief     BLE AUDIO VOCS CONFIG CCCD. 
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[in] cfg_flags:  VOCS CCCD Flag: @ref rtk_bt_le_audio_vocs_cccd_flags_t
 * @param[in] enable:  Whether to enable CCCD.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_cfg_cccd(uint16_t conn_handle, uint8_t srv_instance_id, uint8_t cfg_flags, bool enable);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_char_type_t type)
 * @brief     BLE AUDIO VOCS READ CHAR VALUE. If this API is called successfully, the Application will receive RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_READ_RESULT_IND
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[in] type:  @ref rtk_bt_le_audio_vocs_char_type_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_char_type_t type);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_write_cp(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_cp_op_t cp_op, rtk_bt_le_audio_vocs_cp_param_t cp_param)
 * @brief     BLE AUDIO VOCS WRITE CONTROL POINT. If this API is called successfully, the Application will receive RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_CP_RESULT_IND.
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[in] cp_op:  @ref rtk_bt_le_audio_vocs_cp_op_t
 * @param[in] cp_param:  @ref rtk_bt_le_audio_vocs_cp_param_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_write_cp(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_cp_op_t cp_op, rtk_bt_le_audio_vocs_cp_param_t cp_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_write_cp_by_group(rtk_bt_le_audio_group_handle_t group_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_cp_op_t cp_op, rtk_bt_le_audio_vocs_cp_param_t cp_param)
 * @brief     BLE AUDIO VOCS WRITE CONTROL POINT BY GROUP. If this API is called successfully, the Application will receive RTK_BT_LE_AUDIO_EVT_VOCS_CLIENT_CP_RESULT_IND.
 * @param[in] group_handle:  group handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[in] cp_op:  @ref rtk_bt_le_audio_vocs_cp_op_t
 * @param[in] cp_param:  @ref rtk_bt_le_audio_vocs_cp_param_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_write_cp_by_group(rtk_bt_le_audio_group_handle_t group_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_cp_op_t cp_op, rtk_bt_le_audio_vocs_cp_param_t cp_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_write_audio_location(uint16_t conn_handle, uint8_t srv_instance_id, uint32_t audio_location)
 * @brief     BLE AUDIO VOCS WRITE AUDIO LOCATION.
 * @param[in] group_handle:  group handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[in] audio_location:  @ref rtk_bt_le_audio_location_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_write_audio_location(uint16_t conn_handle, uint8_t srv_instance_id, uint32_t audio_location);

/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_write_output_des(uint16_t conn_handle, uint8_t srv_instance_id, uint16_t output_des_len, uint8_t *p_output_des)
 * @brief     BLE AUDIO VOCS WRITE OUTPUT DESCRIPTOR.
 * @param[in] conn_handle:  conn handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[in] output_des_len:  length of the output descriptor value 
 * @param[in] p_output_des:  output descriptor value.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_write_output_des(uint16_t conn_handle, uint8_t srv_instance_id, uint16_t output_des_len, uint8_t *p_output_des);
/**
 * @fn        uint16_t rtk_bt_le_audio_vocs_get_srv_data(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_srv_data_t *p_srv_data)
 * @brief     BLE AUDIO VOCS WRITE GET SERVICE DATA. 
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: VOCS service index.
 * @param[out] p_srv_data:  the returned service data @ref rtk_bt_le_audio_vocs_srv_data_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_vocs_get_srv_data(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_vocs_srv_data_t *p_srv_data);

#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_aics_set_param(uint8_t srv_instance_id,rtk_bt_le_audio_aics_param_type_t aics_param_type,uint8_t value_len,uint8_t *p_value,bool set_change_counter)
 * @brief     BLE AUDIO AICS SET PARAMETER. 
 * @param[in] srv_instance_id:  service instance id. Used when the number of services is greater than 1.
 * @param[in] aics_param_type:  AICS service parameter type
 * @param[in] value_len:  The value length of p_value
 * @param[in] p_value:  pointer to the AICS parameter data
 * @param[in] set_change_counter:  Whether to set change counter
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_set_param(uint8_t srv_instance_id,rtk_bt_le_audio_aics_param_type_t aics_param_type,uint8_t value_len,uint8_t *p_value,bool set_change_counter);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_get_param(uint8_t srv_instance_id,rtk_bt_le_audio_aics_param_type_t aics_param_type,uint8_t value_len,uint8_t *p_value)
 * @brief     BLE AUDIO AICS GET PARAMETER. 
 * @param[in] srv_instance_id:  service instance id. Used when the number of services is greater than 1.
 * @param[in] aics_param_type:  AICS service parameter type, except RTK_BT_LE_AUDIO_AICS_PARAM_INPUT_DES
 * @param[in] value_len:  The value length of p_value
 * @param[out] p_value:  returned to the AICS parameter data
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_get_param(uint8_t srv_instance_id,rtk_bt_le_audio_aics_param_type_t aics_param_type,uint8_t value_len,uint8_t *p_value);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_cfg_cccd(uint16_t conn_handle, uint8_t srv_instance_id, uint8_t cfg_flags, bool enable)
 * @brief     BLE AUDIO AICS CONFIG CCCD. 
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: AICS service index.
 * @param[in] cfg_flags:  AICS CCCD Flag: @ref rtk_bt_le_audio_aics_cccd_flags_t
 * @param[in] enable:  Whether to enable CCCD.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_cfg_cccd(uint16_t conn_handle, uint8_t srv_instance_id, uint8_t cfg_flags, bool enable);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_char_type_t type)
 * @brief     BLE AUDIO AICS READ CHAR VALUE. If this API is called successfully, the Application will receive RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_READ_RESULT_IND
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: AICS service index.
 * @param[in] type:  @ref rtk_bt_le_audio_aics_char_type_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_read_char_value(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_char_type_t type);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_write_cp(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_cp_op_t cp_op, rtk_bt_le_audio_aics_cp_param_t cp_param)
 * @brief     BLE AUDIO AICS WRITE CONTROL POINT. If this API is called successfully, the Application will receive RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_CP_RESULT_IND.
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: AICS service index.
 * @param[in] cp_op:  @ref rtk_bt_le_audio_aics_cp_op_t
 * @param[in] cp_param:  @ref rtk_bt_le_audio_aics_cp_param_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_write_cp(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_cp_op_t cp_op, rtk_bt_le_audio_aics_cp_param_t cp_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_write_cp_by_group(rtk_bt_le_audio_group_handle_t group_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_cp_op_t cp_op, rtk_bt_le_audio_aics_cp_param_t cp_param)
 * @brief     BLE AUDIO AICS WRITE CONTROL POINT BY GROUP. If this API is called successfully, the Application will receive RTK_BT_LE_AUDIO_EVT_AICS_CLIENT_CP_RESULT_IND.
 * @param[in] group_handle:  group handle.
 * @param[in] srv_instance_id: AICS service index.
 * @param[in] cp_op:  @ref rtk_bt_le_audio_aics_cp_op_t
 * @param[in] cp_param:  @ref rtk_bt_le_audio_aics_cp_param_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_write_cp_by_group(rtk_bt_le_audio_group_handle_t group_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_cp_op_t cp_op, rtk_bt_le_audio_aics_cp_param_t cp_param);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_write_input_des(uint16_t conn_handle, uint8_t srv_instance_id, uint16_t input_des_len, uint8_t *p_input_des)
 * @brief     BLE AUDIO AICS WRITE INPUT DESCRIPTOR.
 * @param[in] conn_handle:  conn handle.
 * @param[in] srv_instance_id: AICS service index.
 * @param[in] input_des_len:  length of the input descriptor value 
 * @param[in] p_input_des:  input descriptor value.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_write_input_des(uint16_t conn_handle, uint8_t srv_instance_id, uint16_t input_des_len, uint8_t *p_input_des);

/**
 * @fn        uint16_t rtk_bt_le_audio_aics_get_srv_data(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_srv_data_t *p_srv_data)
 * @brief     BLE AUDIO AICS WRITE GET SERVICE DATA. 
 * @param[in] conn_handle:  connect handle.
 * @param[in] srv_instance_id: AICS service index.
 * @param[out] p_srv_data:  the returned service data @ref rtk_bt_le_audio_aics_srv_data_t
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_aics_get_srv_data(uint16_t conn_handle, uint8_t srv_instance_id, rtk_bt_le_audio_aics_srv_data_t *p_srv_data);

#endif

#if defined(RTK_BLE_AUDIO_AICS_SUPPORT) && RTK_BLE_AUDIO_AICS_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_audio_tmas_client_read_role(uint16_t conn_handle)
 * @brief     BLE AUDIO TMAS CLIENT READ ROLE. 
 * @param[in] conn_handle:  connect handle.
 * @return
 *            - RTK_BT_OK  : Succeed
 *            - Others: Failed
 */
uint16_t rtk_bt_le_audio_tmas_client_read_role(uint16_t conn_handle);

#endif

/**
 * @}
 */

#endif //end of #if defined(RTK_BLE_AUDIO_SUPPORT) && RTK_BLE_AUDIO_SUPPORT

#ifdef __cplusplus
}
#endif

#endif  /* __RTK_BT_LE_AUDIO_H__ */
