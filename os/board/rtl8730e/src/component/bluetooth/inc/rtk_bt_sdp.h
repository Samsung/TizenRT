/**
 * @file      rtk_bt_sdp.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth LE GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_SDP_H__
#define __RTK_BT_SDP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define RTK_BT_SDK_MAX_RECORD_LENGTH    600

/* ------------------------------- Data Types ------------------------------- */

/* BT_SDP_HEADER */
#define RTK_BT_SDP_UUID16_HDR                      0x19  /**< 00011 001 type:3 UUID; size: 2 bytes */
#define RTK_BT_SDP_UUID32_HDR                      0x1a  /**< 00011 010 type:3 UUID; size: 4 bytes */
#define RTK_BT_SDP_UUID128_HDR                     0x1c  /**< 00011 010 type:3 UUID; size: 16 bytes */
#define RTK_BT_SDP_DATA_ELEM_SEQ_HDR               0x35  /**< 00110 101 type:6 size:5(next byte) */
#define RTK_BT_SDP_DATA_ELEM_SEQ_HDR_2BYTE         0x36  /**< 00110 110 type:6 size:6(next two bytes) */
#define RTK_BT_SDP_STRING_HDR                      0x25

#define RTK_BT_SDP_UNSIGNED_ONE_BYTE               0x08
#define RTK_BT_SDP_UNSIGNED_TWO_BYTE               0x09
#define RTK_BT_SDP_UNSIGNED_FOUR_BYTE              0x0A
#define RTK_BT_SDP_BOOL_ONE_BYTE                   0x28

/* sdk attr */
#define RTK_BT_SDP_ATTR_SRV_RECORD_HANDLE          0x0000
#define RTK_BT_SDP_ATTR_SRV_CLASS_ID_LIST          0x0001
#define RTK_BT_SDP_ATTR_SRV_RECORD_STATE           0x0002
#define RTK_BT_SDP_ATTR_SRV_ID                     0x0003
#define RTK_BT_SDP_ATTR_PROTO_DESC_LIST            0x0004
#define RTK_BT_SDP_ATTR_BROWSE_GROUP_LIST          0x0005
#define RTK_BT_SDP_ATTR_LANG_BASE_ATTR_ID_LIST     0x0006
#define RTK_BT_SDP_ATTR_SRV_INFO_TIME_TO_LIVE      0x0007
#define RTK_BT_SDP_ATTR_SRV_AVAILABILITY           0x0008
#define RTK_BT_SDP_ATTR_PROFILE_DESC_LIST          0x0009
#define RTK_BT_SDP_ATTR_DOC_URL                    0x000A
#define RTK_BT_SDP_ATTR_CLIENT_EXEC_URL            0x000B
#define RTK_BT_SDP_ATTR_ICON_URL                   0x000C
#define RTK_BT_SDP_ATTR_ADD_PROTO_DESC_LIST        0x000D

#define RTK_BT_SDP_ATTR_SRV_NAME                   0x0000  /**< these attrib values are base values */
#define RTK_BT_SDP_ATTR_SRV_DESC                   0x0001  /**< these attrib values are base values */
#define RTK_BT_SDP_ATTR_PROVIDER_NAME              0x0002  /**< these attrib values are base values */

#define RTK_BT_SDP_ATTR_VER_NUMBER_LIST            0x0200
#define RTK_BT_SDP_ATTR_GROUP_ID                   0x0200
#define RTK_BT_SDP_ATTR_IP_SUBNET                  0x0200
#define RTK_BT_SDP_ATTR_SRV_DB_STATE               0x0201

#define RTK_BT_SDP_BASE_LANG_OFFSET                0x0100  /**< offset to base language */
#define RTK_BT_SDP_LANG_ENGLISH                    0x656e  /**< this is "en", according to ISO 639:1988 */
#define RTK_BT_SDP_LANG_GERMAN                     0x6465  /**< this is "de", according to ISO 639:1988 */
#define RTK_BT_SDP_CHARACTER_UTF8                  106     /**< UTF8 encoding, cf. http://www.isi.edu/in-notes/iana/assignments/character-sets */

#define RTK_BT_SDP_ATTR_L2C_PSM                    0x0200

#define RTK_BT_SDP_ATTR_SRV_VER                    0x0300
#define RTK_BT_SDP_ATTR_EXT_NETWORK                0x0301
#define RTK_BT_SDP_ATTR_FAX_CLASS1_SUPPORT         0x0302
#define RTK_BT_SDP_ATTR_RMT_AUDIO_VOL_CTRL         0x0302
#define RTK_BT_SDP_ATTR_FAX_CLASS20_SUPPORT        0x0303
#define RTK_BT_SDP_ATTR_SUPPORTED_FORMATS_LIST     0x0303
#define RTK_BT_SDP_ATTR_FAX_CLASS2_SUPPORT         0x0304
#define RTK_BT_SDP_ATTR_AUDIO_FEEDBACK_SUPPORT     0x0305
#define RTK_BT_SDP_ATTR_NETWORK_ADDR               0x0306
#define RTK_BT_SDP_ATTR_WAP_GATEWAY                0x0307
#define RTK_BT_SDP_ATTR_HOME_PAGE_URL              0x0308
#define RTK_BT_SDP_ATTR_WAP_STACK_TYPE             0x0309
#define RTK_BT_SDP_ATTR_SECURITY_DESC              0x030A
#define RTK_BT_SDP_ATTR_NET_ACCESS_TYPE            0x030B
#define RTK_BT_SDP_ATTR_MAX_NET_ACCESS_RATE        0x030C
#define RTK_BT_SDP_ATTR_SUPPORTED_CAPABILITIES     0x0310
#define RTK_BT_SDP_ATTR_SUPPORTED_FEATURES         0x0311
#define RTK_BT_SDP_ATTR_SUPPORTED_FUNCTIONS        0x0312
#define RTK_BT_SDP_ATTR_TOTAL_IMAGING_CAPACITY     0x0313
#define RTK_BT_SDP_ATTR_SUPPORTED_REPOSITORIES     0x0314

#define RTK_BT_SDP_ATTR_MAS_INSTANCE_ID            0x0315
#define RTK_BT_SDP_ATTR_SUPPORTED_MSG_TYPES        0x0316
#define RTK_BT_SDP_ATTR_MAP_SUPPERTED_FEATS        0x0317

#define RTK_BT_SDP_ATTR_HID_DEV_RELEASE_NUM        0x0200
#define RTK_BT_SDP_ATTR_HID_PARSER_VER             0x0201
#define RTK_BT_SDP_ATTR_HID_DEV_SUB_CLASS          0x0202
#define RTK_BT_SDP_ATTR_HID_COUNTRY_CODE           0x0203
#define RTK_BT_SDP_ATTR_HID_VIRTUAL_CABLE          0x0204
#define RTK_BT_SDP_ATTR_HID_RECONN_INIT            0x0205
#define RTK_BT_SDP_ATTR_HID_DESC_LIST              0x0206
#define RTK_BT_SDP_ATTR_HID_LANG_ID_BASE_LIST      0x0207
#define RTK_BT_SDP_ATTR_HID_SDP_DISABLE            0x0208
#define RTK_BT_SDP_ATTR_HID_BATTERY_POWER          0x0209
#define RTK_BT_SDP_ATTR_HID_REMOTE_WAKE            0x020A
#define RTK_BT_SDP_ATTR_HID_PROFILE_VER            0x020B
#define RTK_BT_SDP_ATTR_HID_SUPERVISION_TIMEOUT    0x020C
#define RTK_BT_SDP_ATTR_HID_NORMALLY_CONNECTABLE   0x020D
#define RTK_BT_SDP_ATTR_HID_BOOT_DEV               0x020E

#define RTK_BT_SDP_ATTR_PBAP_SUPPORTED_FEAT        0x0317

#define RTK_BT_SDP_ATTR_DIP_SPECIFICATION_ID       0x0200
#define RTK_BT_SDP_ATTR_DIP_VENDOR_ID              0x0201
#define RTK_BT_SDP_ATTR_DIP_PRODUCT_ID             0x0202
#define RTK_BT_SDP_ATTR_DIP_PRODUCT_VERSION        0x0203
#define RTK_BT_SDP_ATTR_DIP_PRIMARY_RECORD         0x0204
#define RTK_BT_SDP_ATTR_DIP_VENDOR_ID_SOURCE       0x0205

/**
 * @struct    rtk_sdp_record_t
 * @brief     Bluetooth sdp srecord struct.
 */
typedef struct {
	struct list_head	list;									//!< list of .rtk_sdp_record_t.
	uint8_t				record[RTK_BT_SDK_MAX_RECORD_LENGTH];	//!< record data
	uint32_t			length;									//!< indicate length of record data
} rtk_sdp_record_t;



/**
 * @fn        uint16_t rtk_sdp_record_add(void *record, uint32_t length)
 * @brief     set profile record.
 * @param[in] record: record data pointer
 * @param[in] length: data length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_sdp_record_add(void *record, uint32_t length);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_SDP_H__ */
