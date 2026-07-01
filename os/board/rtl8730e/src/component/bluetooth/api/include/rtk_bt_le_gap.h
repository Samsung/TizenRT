/**
 * @file      rtk_bt_le_gap.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth LE GAP part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_LE_GAP_H__
#define __RTK_BT_LE_GAP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <basic_types.h>
#include <bt_api_config.h>

/**
 * @typedef   rtk_bt_le_appearance_t
 * @brief     Bluetooth LE device appearance definition.
 */
typedef enum {
	RTK_BT_LE_GAP_APPEARANCE_UNKNOWN                             = 0x0000, /*!< 0x0000 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_PHONE                       = 0x0040, /*!< 0x0040 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_COMPUTER                    = 0x0080, /*!< 0x0080 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_WATCH                       = 0x00C0, /*!< 0x00C0 */
	RTK_BT_LE_GAP_APPEARANCE_SPORTS_WATCH                        = 0x00C1, /*!< 0x00C1 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_CLOCK                       = 0x0100, /*!< 0x0100 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_DISPLAY                     = 0x0140, /*!< 0x0140 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_REMOTE                      = 0x0180, /*!< 0x0180 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_EYEGLASSES                  = 0x01C0, /*!< 0x01C0 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_TAG                         = 0x0200, /*!< 0x0200 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_KEYRING                     = 0x0240, /*!< 0x0240 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_MEDIA_PLAYER                = 0x0280, /*!< 0x0280 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_BARCODE_SCANNER             = 0x02C0, /*!< 0x02C0 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_THERMOMETER                 = 0x0300, /*!< 0x0300 */
	RTK_BT_LE_GAP_APPEARANCE_THERMOMETER_EAR                     = 0x0301, /*!< 0x0301 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_HEART_RATE                  = 0x0340, /*!< 0x0340 */
	RTK_BT_LE_GAP_APPEARANCE_HEART_RATE_BELT                     = 0x0341, /*!< 0x0341 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_BLOOD_PRESSURE              = 0x0380, /*!< 0x0380 */
	RTK_BT_LE_GAP_APPEARANCE_BLOOD_PRESSURE_ARM                  = 0x0381, /*!< 0x0381 */
	RTK_BT_LE_GAP_APPEARANCE_BLOOD_PRESSURE_WRIST                = 0x0382, /*!< 0x0382 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_HID                         = 0x03C0, /*!< 0x03C0 */
	RTK_BT_LE_GAP_APPEARANCE_HID_KEYBOARD                        = 0x03C1, /*!< 0x03C1 */
	RTK_BT_LE_GAP_APPEARANCE_HID_MOUSE                           = 0x03C2, /*!< 0x03C2 */
	RTK_BT_LE_GAP_APPEARANCE_HID_JOYSTICK                        = 0x03C3, /*!< 0x03C3 */
	RTK_BT_LE_GAP_APPEARANCE_HID_GAMEPAD                         = 0x03C4, /*!< 0x03C4 */
	RTK_BT_LE_GAP_APPEARANCE_HID_DIGITIZER_TABLET                = 0x03C5, /*!< 0x03C5 */
	RTK_BT_LE_GAP_APPEARANCE_HID_CARD_READER                     = 0x03C6, /*!< 0x03C6 */
	RTK_BT_LE_GAP_APPEARANCE_HID_DIGITAL_PEN                     = 0x03C7, /*!< 0x03C7 */
	RTK_BT_LE_GAP_APPEARANCE_HID_BARCODE_SCANNER                 = 0x03C8, /*!< 0x03C8 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_GLUCOSE                     = 0x0400, /*!< 0x0400 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_WALKING                     = 0x0440, /*!< 0x0440 */
	RTK_BT_LE_GAP_APPEARANCE_WALKING_IN_SHOE                     = 0x0441, /*!< 0x0441 */
	RTK_BT_LE_GAP_APPEARANCE_WALKING_ON_SHOE                     = 0x0442, /*!< 0x0442 */
	RTK_BT_LE_GAP_APPEARANCE_WALKING_ON_HIP                      = 0x0443, /*!< 0x0443 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_CYCLING                     = 0x0480, /*!< 0x0480 */
	RTK_BT_LE_GAP_APPEARANCE_CYCLING_COMPUTER                    = 0x0481, /*!< 0x0481 */
	RTK_BT_LE_GAP_APPEARANCE_CYCLING_SPEED                       = 0x0482, /*!< 0x0482 */
	RTK_BT_LE_GAP_APPEARANCE_CYCLING_CADENCE                     = 0x0483, /*!< 0x0483 */
	RTK_BT_LE_GAP_APPEARANCE_CYCLING_POWER                       = 0x0484, /*!< 0x0484 */
	RTK_BT_LE_GAP_APPEARANCE_CYCLING_SPEED_CADENCE               = 0x0485, /*!< 0x0485 */
	RTK_BT_LE_GAP_APPEARANCE_EARBUD                              = 0x0941, /*!< 0x0941 */
	RTK_BT_LE_GAP_APPEARANCE_HEADSET                             = 0x0942, /*!< 0x0942 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_PULSE_OXIMETER              = 0x0C40, /*!< 0x0C40 */
	RTK_BT_LE_GAP_APPEARANCE_PULSE_OXIMETER_FINGERTIP            = 0x0C41, /*!< 0x0C41 */
	RTK_BT_LE_GAP_APPEARANCE_PULSE_OXIMETER_WRIST                = 0x0C42, /*!< 0x0C42 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_WEIGHT                      = 0x0C80, /*!< 0x0C80 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_PERSONAL_MOBILITY_DEVICE    = 0x0CC0, /*!< 0x0CC0 */
	RTK_BT_LE_GAP_APPEARANCE_POWERED_WHEELCHAIR                  = 0x0CC1, /*!< 0x0CC1 */
	RTK_BT_LE_GAP_APPEARANCE_MOBILITY_SCOOTER                    = 0x0CC2, /*!< 0x0CC2 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_CONTINUOUS_GLUCOSE_MONITOR  = 0x0D00, /*!< 0x0D00 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_INSULIN_PUMP                = 0x0D40, /*!< 0x0D40 */
	RTK_BT_LE_GAP_APPEARANCE_INSULIN_PUMP_DURABLE_PUMP           = 0x0D41, /*!< 0x0D41 */
	RTK_BT_LE_GAP_APPEARANCE_INSULIN_PUMP_PATCH_PUMP             = 0x0D44, /*!< 0x0D44 */
	RTK_BT_LE_GAP_APPEARANCE_INSULIN_PEN                         = 0x0D48, /*!< 0x0D48 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_MEDICATION_DELIVERY         = 0x0D80, /*!< 0x0D80 */
	RTK_BT_LE_GAP_APPEARANCE_GENERIC_OUTDOOR_SPORTS              = 0x1440, /*!< 0x1440 */
	RTK_BT_LE_GAP_APPEARANCE_OUTDOOR_SPORTS_LOCATION             = 0x1441, /*!< 0x1441 */
	RTK_BT_LE_GAP_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV     = 0x1442, /*!< 0x1442 */
	RTK_BT_LE_GAP_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD         = 0x1443, /*!< 0x1443 */
	RTK_BT_LE_GAP_APPEARANCE_OUTDOOR_SPORTS_LOCATION_POD_AND_NAV = 0x1444, /*!< 0x1444 */
} rtk_bt_le_appearance_t;


/**
 * @typedef   rtk_bt_le_gap_ad_type_t
 * @brief     Bluetooth ble gap advertising data type definition.
 */
typedef enum {
	RTK_BT_LE_GAP_ADTYPE_FLAGS                                   = 0x01, /*!< 0x01, The Flags data type contains one bit Boolean flags. */
	RTK_BT_LE_GAP_ADTYPE_16BIT_MORE                              = 0x02, /*!< 0x02, Service: More 16-bit UUIDs available */
	RTK_BT_LE_GAP_ADTYPE_16BIT_COMPLETE                          = 0x03, /*!< 0x03, Service: Complete list of 16-bit UUIDs */
	RTK_BT_LE_GAP_ADTYPE_32BIT_MORE                              = 0x04, /*!< 0x04, Service: More 32-bit UUIDs available */
	RTK_BT_LE_GAP_ADTYPE_32BIT_COMPLETE                          = 0x05, /*!< 0x05, Service: Complete list of 32-bit UUIDs */
	RTK_BT_LE_GAP_ADTYPE_128BIT_MORE                             = 0x06, /*!< 0x06, Service: More 128-bit UUIDs available */
	RTK_BT_LE_GAP_ADTYPE_128BIT_COMPLETE                         = 0x07, /*!< 0x07, Service: Complete list of 128-bit UUIDs */
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_SHORT                        = 0x08, /*!< 0x08, Shortened local name */
	RTK_BT_LE_GAP_ADTYPE_LOCAL_NAME_COMPLETE                     = 0x09, /*!< 0x09, Complete local name */
	RTK_BT_LE_GAP_ADTYPE_POWER_LEVEL                             = 0x0A, /*!< 0x0A, TX Power Level: = 0xXX: -127 to +127 dBm */
	RTK_BT_LE_GAP_ADTYPE_OOB_CLASS_OF_DEVICE                     = 0x0D, /*!< 0x0D, Simple Pairing OOB Tag: Class of device (3 octets) */
	RTK_BT_LE_GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC                = 0x0E, /*!< 0x0E, Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets) */
	RTK_BT_LE_GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR                = 0x0F, /*!< 0x0F, Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets) */
	RTK_BT_LE_GAP_ADTYPE_SM_TK                                   = 0x10, /*!< 0x10, Security Manager TK Value */
	RTK_BT_LE_GAP_ADTYPE_SM_OOB_FLAG                             = 0x11, /*!< 0x11, Secutiry Manager OOB Flags */
	RTK_BT_LE_GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE               = 0x12, /*!< 0x12, Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max) */
	RTK_BT_LE_GAP_ADTYPE_SIGNED_DATA                             = 0x13, /*!< 0x13, Signed Data field */
	RTK_BT_LE_GAP_ADTYPE_SERVICES_LIST_16BIT                     = 0x14, /*!< 0x14, Service Solicitation: list of 16-bit Service UUIDs */
	RTK_BT_LE_GAP_ADTYPE_SERVICES_LIST_128BIT                    = 0x15, /*!< 0x15, Service Solicitation: list of 128-bit Service UUIDs */
	RTK_BT_LE_GAP_ADTYPE_SERVICE_DATA                            = 0x16, /*!< 0x16, Service Data */
	RTK_BT_LE_GAP_ADTYPE_PUBLIC_TGT_ADDR                         = 0x17, /*!< 0x17, Public Target Address */
	RTK_BT_LE_GAP_ADTYPE_RANDOM_TGT_ADDR                         = 0x18, /*!< 0x18, Random Target Address */
	RTK_BT_LE_GAP_ADTYPE_APPEARANCE                              = 0x19, /*!< 0x19, Appearance */
	RTK_BT_LE_GAP_ADTYPE_ADV_INTERVAL                            = 0x1A, /*!< 0x1A, Advertising Interval */
	RTK_BT_LE_GAP_ADTYPE_LE_BT_ADDR                              = 0x1B, /*!< 0x1B, LE Bluetooth Device Address */
	RTK_BT_LE_GAP_ADTYPE_LE_ROLE                                 = 0x1C, /*!< 0x1C, LE Role */
	RTK_BT_LE_GAP_ADTYPE_SP_HASH_C256                            = 0x1D, /*!< 0x1D, Simple Pairing Hash C-256 */
	RTK_BT_LE_GAP_ADTYPE_SP_RAND_R256                            = 0x1E, /*!< 0x1E, Simple Pairing Randomizer R-256 */
	RTK_BT_LE_GAP_ADTYPE_LIST_32BIT_SILI                         = 0x1F, /*!< 0x1F, List of 32-bit Service Solicitation UUIDs */
	RTK_BT_LE_GAP_ADTYPE_SERVICE_DATA_32BIT                      = 0x20, /*!< 0x20, Service Data - 32-bit UUID */
	RTK_BT_LE_GAP_ADTYPE_SERVICE_DATA_128BIT                     = 0x21, /*!< 0x21, Service Data - 128-bit UUID */
	RTK_BT_LE_GAP_ADTYPE_SC_CONF_VALUE                           = 0x22, /*!< 0x22, LE Secure Connections Confirmation Value */
	RTK_BT_LE_GAP_ADTYPE_SC_RAND_VALUE                           = 0x23, /*!< 0x23, LE Secure Connections Random Value */
	RTK_BT_LE_GAP_ADTYPE_URI                                     = 0x24, /*!< 0x24, URI */
	RTK_BT_LE_GAP_ADTYPE_INDOOR_POSITION                         = 0x25, /*!< 0x25, Indoor Positioning */
	RTK_BT_LE_GAP_ADTYPE_TRANSPORT_DISCOVERY_DATA                = 0x26, /*!< 0x26, Transport Discovery Data */
	RTK_BT_LE_GAP_ADTYPE_LE_SUPPORTED_FEATURES                   = 0x27, /*!< 0x27, LE Supported Features */
	RTK_BT_LE_GAP_ADTYPE_CHAN_MAP_UPDATE_IND                     = 0x28, /*!< 0x28, Channel Map Update Indication */
	RTK_BT_LE_GAP_ADTYPE_MESH_PB_ADV                             = 0x29, /*!< 0x29, Mesh Pb-Adv */
	RTK_BT_LE_GAP_ADTYPE_MESH_PACKET                             = 0x2A, /*!< 0x2A, Mesh Packet */
	RTK_BT_LE_GAP_ADTYPE_MESH_BEACON                             = 0x2B, /*!< 0x2B, Mesh Beacon */
	RTK_BT_LE_GAP_ADTYPE_RSI                                     = 0x2E, /*!< 0x2E, RSI */
	RTK_BT_LE_GAP_ADTYPE_BROADCAST_NAME                          = 0x30, /*!< 0x30, Broadcast Name */
	RTK_BT_LE_GAP_ADTYPE_3D_INFO_DATA                            = 0x3D, /*!< 0x3D, 3D Information Data */
	RTK_BT_LE_GAP_ADTYPE_MANUFACTURER_SPECIFIC                   = 0xFF, /*!< 0xFF, Manufacturer Specific Data */
} rtk_bt_le_gap_ad_type_t;

/**
 * @typedef   rtk_bt_le_gap_ad_flags_data_t
 * @brief     Bluetooth ble gap advertising data type data definition.
 */
typedef enum {
	RTK_BT_LE_GAP_ADTYPE_FLAGS_LIMITED                           = 0x01, /*!< 0x01, Discovery Mode: LE Limited Discoverable Mode*/
	RTK_BT_LE_GAP_ADTYPE_FLAGS_GENERAL                           = 0x02, /*!< 0x02, Discovery Mode: LE General Discoverable Mode*/
	RTK_BT_LE_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED               = 0x04, /*!< 0x04, Discovery Mode: BR/EDR Not Supported*/
	RTK_BT_LE_GAP_ADTYPE_FLAGS_SIMULTANEOUS_LE_BREDR_CONTROLLER  = 0x08, /*!< 0x08, Discovery Mode: Simultaneous LE and BR/EDR Controller Supported*/
	RTK_BT_LE_GAP_ADTYPE_FLAGS_SIMULTANEOUS_LE_BREDR_HOST        = 0x10, /*!< 0x10, Discovery Mode: Simultaneous LE and BR/EDR Host Supported*/
} rtk_bt_le_gap_ad_flags_data_t;

/**
 * @typedef   rtk_bt_le_adv_type_t
 * @brief     Bluetooth LE GAP adv type definition.
 */
typedef enum {
	RTK_BT_LE_ADV_TYPE_IND                     = 0x00, /*!< 0x00, Connectable and scannable undirected advertising (ADV_IND) (default) */
	RTK_BT_LE_ADV_TYPE_DIRECT_IND_HIGH         = 0x01, /*!< 0x01, Connectable high duty cycle directed advertising (ADV_DIRECT_IND, high duty cycle) */
	RTK_BT_LE_ADV_TYPE_SCAN_IND                = 0x02, /*!< 0x02, Scannable undirected advertising (ADV_SCAN_IND) */
	RTK_BT_LE_ADV_TYPE_NONCONN_IND             = 0x03, /*!< 0x03, Non connectable undirected advertising (ADV_NONCONN_IND) */
	RTK_BT_LE_ADV_TYPE_DIRECT_IND_LOW          = 0x04, /*!< 0x04, Connectable low duty cycle directed advertising (ADV_DIRECT_IND, low duty cycle) */
	RTK_BT_LE_ADV_TYPE_RESERVED,                       /*!< reserved */
} rtk_bt_le_adv_type_t;

/**
 * @typedef   rtk_bt_le_version_info_t
 * @brief     Bluetooth LE GAP lower stack version information.
 */
typedef struct {
	uint16_t hci_revision;          /*!< hci revision */
	uint16_t lmp_subversion;        /*!< lmp subversion */
	uint16_t btgap_revision;        /*!< btgap stack version */
	uint16_t btgap_buildnum;        /*!< btgap stack build number */
} rtk_bt_le_version_info_t;

/**
 * @typedef   rtk_bt_le_get_conn_id_t
 * @brief     Bluetooth LE GAP get connection id from connection handle.
 */
typedef struct {
	uint16_t conn_handle;           /*!< connection handle */
	uint8_t *p_conn_id;             /*!< LE connection id */
} rtk_bt_le_get_conn_id_t;

#if defined(RTK_BLE_5_0_SET_PHYS_SUPPORT) && RTK_BLE_5_0_SET_PHYS_SUPPORT
/**
 * @typedef   rtk_bt_le_phy_type_t
 * @brief     Bluetooth LE PHY type.
 */
typedef enum {
	RTK_BT_LE_PHYS_1M       = 0x01,    /**< LE PHY 1M used. */
	RTK_BT_LE_PHYS_2M       = 0x02,    /**< LE PHY 2M used. */
	RTK_BT_LE_PHYS_CODED    = 0x03     /**< LE Coded PHY used. */
} rtk_bt_le_phy_type_t;

/**
 * @typedef   rtk_bt_le_phy_prim_adv_t
 * @brief     Bluetooth LE Primary Adv PHY.
 */
typedef enum {
	RTK_BT_LE_PHYS_PRIM_ADV_1M      = 1, /**<  Primary advertisement PHY is LE 1M */
	RTK_BT_LE_PHYS_PRIM_ADV_CODED   = 3  /**<  Primary advertisement PHY is LE Coded */
} rtk_bt_le_phy_prim_adv_t;

/**
 * @typedef   rtk_bt_le_all_phy_prefer_t
 * @brief     Bluetooth LE ALL_PHYs prefer bit field.
 */
typedef enum {
	RTK_BT_LE_PHYS_PREFER_ALL    = 0,
	RTK_BT_LE_PHYS_NO_PREFER_TX  = (1 << 0), /**< The Host has no preference among the transmitter PHYs supported by the Controller */
	RTK_BT_LE_PHYS_NO_PREFER_RX  = (1 << 1), /**< The Host has no preference among the receiver PHYs supported by the Controller */
} rtk_bt_le_all_phy_prefer_t;

/**
 * @typedef   rtk_bt_le_trx_phy_prefer_t
 * @brief     Bluetooth LE TX_PHYs or RX_PHYs prefer bit field.
 */
typedef enum {
	RTK_BT_LE_PHYS_PREFER_1M     = (1 << 0), /**< The Host prefers to use the LE 1M transmitter PHY (possibly among others) */
	RTK_BT_LE_PHYS_PREFER_2M     = (1 << 1), /**< The Host prefers to use the LE 2M transmitter PHY (possibly among others) */
	RTK_BT_LE_PHYS_PREFER_CODED  = (1 << 2), /**< The Host prefers to use the LE Coded transmitter PHY (possibly among others) */
} rtk_bt_le_trx_phy_prefer_t;

/**
 * @typedef   rtk_bt_le_phy_option_t
 * @brief     Bluetooth LE PHY options.
 */
typedef enum {
	RTK_BT_LE_PHYS_OPTIONS_CODED_PREFER_NO = 0x0, /**< Host has no preferred coding when transmitting on the LE Coded PHY */
	RTK_BT_LE_PHYS_OPTIONS_CODED_PREFER_S2 = 0x1, /**< Host prefers that S=2 coding be used when transmitting on the LE Coded PHY */
	RTK_BT_LE_PHYS_OPTIONS_CODED_PREFER_S8 = 0x2, /**< Host prefers that S=8 coding be used when transmitting on the LE Coded PHY */
} rtk_bt_le_phy_option_t;
#endif

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
/**
 * @typedef   rtk_bt_le_adv_event_prop_t
 * @brief     Bluetooth LE GAP adv event properties.
 */
typedef enum {
	RTK_BT_LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED = 0x00, /**<  Non-connectable and non-scannable undirected. */
	RTK_BT_LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED   = 0x04, /**<  Non-connectable and non-scannable directed. */
	RTK_BT_LE_EXT_ADV_EXTENDED_ADV_CONN_UNDIRECTED              = 0x01, /**<  Connectable undirected. Advertising data shall not exceed 245 bytes. */
	RTK_BT_LE_EXT_ADV_EXTENDED_ADV_CONN_DIRECTED                = 0x05, /**<  Connectable directed. Advertising data shall not exceed 239 bytes. */
	RTK_BT_LE_EXT_ADV_EXTENDED_ADV_SCAN_UNDIRECTED              = 0x02, /**<  Scannable undirected. */
	RTK_BT_LE_EXT_ADV_EXTENDED_ADV_SCAN_DIRECTED                = 0x06, /**<  Scannable directed. */

	RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_SCAN_UNDIRECTED           = 0x13, /**<  Connectable and scannable undirected. Advertising data or scan response data shall not exceed 31 bytes. */
	RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_LOW_DUTY_DIRECTED         = 0x15, /**<  Connectable directed (low duty cycle). */
	RTK_BT_LE_EXT_ADV_LEGACY_ADV_CONN_HIGH_DUTY_DIRECTED        = 0x1D, /**<  Connectable directed (high duty cycle). */
	RTK_BT_LE_EXT_ADV_LEGACY_ADV_SCAN_UNDIRECTED                = 0x12, /**<  Scannable undirected. Advertising data or scan response data shall not exceed 31 bytes. */
	RTK_BT_LE_EXT_ADV_LEGACY_ADV_NON_SCAN_NON_CONN_UNDIRECTED   = 0x10, /**<  Non-connectable and non-scannable undirected. Advertising data shall not exceed 31 bytes. */
} rtk_bt_le_adv_event_prop_t;
#endif

/**
 * @typedef   rtk_bt_le_adv_ch_map_t
 * @brief     Bluetooth LE GAP adv channel map definition.
 */
typedef enum {
	RTK_BT_LE_ADV_CHNL_37               = 0x01, /*!< 0x01, Channel 37 shall be used */
	RTK_BT_LE_ADV_CHNL_38               = 0x02, /*!< 0x02, Channel 38 shall be used */
	RTK_BT_LE_ADV_CHNL_39               = 0x04, /*!< 0x04, Channel 39 shall be used */
	RTK_BT_LE_ADV_CHNL_ALL              = 0x07, /*!< 0x07, Channel 37,38,39 shall be used */
	RTK_BT_LE_ADV_CHNL_RESERVED,                /*!< reserved */
} rtk_bt_le_adv_ch_map_t;

/**
 * @typedef   rtk_bt_le_adv_filter_t
 * @brief     Bluetooth LE GAP adv filter type definition.
 */
typedef enum {
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY              = 0, /*!< 0, Allow both scan and connection requests from anyone */
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY             = 1, /*!< 1, Allow both scan requests from White List devices only and connection requests from anyone */
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST             = 2, /*!< 2, Allow both scan requests from anyone and connection requests from White List devices only */
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST            = 3, /*!< 3, Allow scan and connection requests from White List devices only */
	RTK_BT_LE_ADV_FILTER_RESERVED,                                /*!< All bigger value are reserved */
} rtk_bt_le_adv_filter_t;

/**
 * @struct    rtk_bt_le_preferred_conn_param_t
 * @brief     Bluetooth LE preferred connection parameters definition.
 */
typedef struct {
	uint16_t conn_interval_min;     /*!<Defines minimum value for the connection interval in the following manner:
                                    connIntervalmin = Conn_Interval_Min * 1.25 ms
                                    Conn_Interval_Min range: 0x0006 to 0x0C80
                                    Value of 0xFFFF indicates no specific minimum.
                                    Values outside the range (except 0xFFFF) are reserved for future use.*/
	uint16_t conn_interval_max;     /*!<Defines maximum value for the connection interval in the following manner:
                                    connIntervalmax = Conn_Interval_Max * 1.25 ms
                                    Conn_Interval_Max range: 0x0006 to 0x0C80
                                    Shall be equal to or greater than the Conn_Interval_Min.
                                    Value of 0xFFFF indicates no specific maximum.
                                    Values outside the range (except 0xFFFF) are reserved for future use.*/
	uint16_t slave_latency;         /*!<Defines the slave latency for the connection in number of connection events.
                                    Slave latency range: 0x0000 to 0x01F3
                                    Values outside the range are reserved for future use.*/
	uint16_t supv_timeout;          /*!<Defines the connection supervison timeout multiplier as amultiple of 10ms.
                                    Range: 0xFFFF indicates no specific value requested.
                                    Range: 0x000A to 0x0C80
                                    Time = N * 10 ms
                                    Time Range: 100 ms to 32 seconds
                                    Values outside the range (except 0xFFFF) are reserved for future use.*/
} rtk_bt_le_preferred_conn_param_t;

/**
 * @typedef   rtk_bt_le_addr_type_t
 * @brief     Bluetooth device address type definition.
 */
typedef enum {
	RTK_BT_LE_ADDR_TYPE_PUBLIC =            0x00,      /*!< Public Device Address */
	RTK_BT_LE_ADDR_TYPE_RANDOM =            0x01,      /*!< Random Device Address */
	RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC =        0x02,      /*!< Public Identity Address (RPA) */
	RTK_BT_LE_ADDR_TYPE_RPA_RANDOM =        0x03,      /*!< Random (static) Identity Address (RPA)*/

	RTK_BT_LE_ADDR_TYPE_PEER_ANONYMOUS =    0xFF,      /*!< Anonymous Device Address */
} rtk_bt_le_addr_type_t;

/**
 * @enum    rtk_bt_le_ident_addr_type_t
 * @brief   Bluetooth LE indentity address type definition.
 */
typedef enum {
	RTK_BT_LE_IDENT_ADDR_PUBLIC      = 0x00,    /*  Public identity address. */
	RTK_BT_LE_IDENT_ADDR_RAND        = 0x01,    /*  Random identity address. */
} rtk_bt_le_ident_addr_type_t;

/**
 * @typedef   rtk_bt_le_rand_addr_type_t
 * @brief     Bluetooth device address type definition.
 */
typedef enum {
	RTK_BT_LE_RAND_ADDR_STATIC =            0x00,      /*!< Static random device address */
	RTK_BT_LE_RAND_ADDR_NON_RESOLVABLE =    0x01,      /*!< Non resolvable random device address */
	RTK_BT_LE_RAND_ADDR_RESOLVABLE =        0x02,      /*!< Resolvable random device address */
} rtk_bt_le_rand_addr_type_t;

/**
 * @def       RTK_BD_ADDR_LEN
 * @brief     Bluetooth device address length macro.
 */
#define RTK_BD_ADDR_LEN             6

/**
 * @struct    rtk_bt_le_addr_t
 * @brief     Bluetooth LE device address definition.
 */
typedef struct {
	rtk_bt_le_addr_type_t type;                 /*!< address type */
	uint8_t addr_val[RTK_BD_ADDR_LEN];          /*!< address */
} rtk_bt_le_addr_t;

/**
 * @struct    rtk_bt_le_ident_addr_t
 * @brief     Bluetooth LE indentity address definition.
 */
typedef struct {
	rtk_bt_le_ident_addr_type_t type;       /*!< Identity address type */
	uint8_t addr_val[RTK_BD_ADDR_LEN];      /*!< Identity address value */
} rtk_bt_le_ident_addr_t;

/**
 * @struct    rtk_bt_le_set_rand_addr_t
 * @brief     Bluetooth LE set random address parameter definition.
 */
typedef struct {
	bool auto_generate;                         /*!< Auto generate
                                                 * If true, set device random address to auto generated value according to @ref type, and @ref addr_val will return the generated random address.
                                                 * If false, set device random address to @ref addr_val, and @ref type will be ignored.
                                                 */
	rtk_bt_le_rand_addr_type_t type;            /*!< Random address type */
	uint8_t *addr_val;                          /*!< Random address */
} rtk_bt_le_set_rand_addr_t;

/**
 * @typedef   rtk_bt_le_scan_type_t
 * @brief     Bluetooth LE GAP scan type definition.
 */
typedef enum {
	RTK_BT_LE_SCAN_TYPE_PASSIVE = 0,          /*!< 0, Passive scan */
	RTK_BT_LE_SCAN_TYPE_ACTIVE,               /*!< 1, Active scan */
	RTK_BT_LE_SCAN_TYPE_RESERVED,             /*!< All bigger value are reserved */
} rtk_bt_le_scan_type_t;

/**
 * @typedef   rtk_bt_le_conn_filter_t
 * @brief     Bluetooth LE GAP connection filter type definition.
 */
typedef enum {
	RTK_BT_LE_CONN_FILTER_WITHOUT_WHITELIST,  /*!< 0, filter without whitelist */
	RTK_BT_LE_CONN_FILTER_WITH_WHITELIST,     /*!< 1, filter with whitelist */
	RTK_BT_LE_CONN_FILTER_RESERVED,           /*!< All bigger value are reserved */
} rtk_bt_le_conn_filter_t;

/**
 * @typedef   rtk_bt_le_scan_filter_t
 * @brief     Bluetooth LE GAP scan filter type definition.
 */
typedef enum {
	RTK_BT_LE_SCAN_FILTER_ALLOW_ALL             = 0x0, /*!< Accept all :
                                                        * 1. advertisement packets except directed advertising packets not addressed to this device (default).
                                                        */
	RTK_BT_LE_SCAN_FILTER_ALLOW_ONLY_WLST       = 0x1, /*!< Accept only :
                                                        * 1. advertisement packets from devices where the advertiser's address is in the White list.
                                                        * 2. Directed advertising packets which are not addressed for this device shall be ignored.
                                                        */
	RTK_BT_LE_SCAN_FILTER_ALLOW_UND_RPA_DIR     = 0x2, /*!< Accept all :
                                                        * 1. undirected advertisement packets, and
                                                        * 2. directed advertising packets where the initiator address is a resolvable private address, and
                                                        * 3. directed advertising packets addressed to this device.
                                                        */
	RTK_BT_LE_SCAN_FILTER_ALLOW_WLIST_RPA_DIR   = 0x3, /*!< Accept all :
                                                        * 1. advertisement packets from devices where the advertiser's address is in the White list, and
                                                        * 2. directed advertising packets where the initiator address is a resolvable private address, and
                                                        * 3. directed advertising packets addressed to this device.
                                                        */
} rtk_bt_le_scan_filter_t;


/**
 * @typedef   rtk_bt_le_scan_duplicate_t
 * @brief     Bluetooth LE GAP scan duplicate filter type definition.
 */
typedef enum {
	RTK_BT_LE_SCAN_DUPLICATE_DISABLE = 0,     /*!< 0, Link Layer should generate advertising reports to the host for each packet received */
	RTK_BT_LE_SCAN_DUPLICATE_ENABLE  = 1,     /*!< 1, Link Layer should filter out duplicate advertising reports to the Host */
	RTK_BT_LE_SCAN_DUPLICATE_ENABLED_RESET_FOR_EACH_PERIOD = 2,     /**<  Duplicate filtering enabled, reset for each scan period. */
	RTK_BT_LE_SCAN_DUPLICATE_RESERVED,        /*!< reserved */
} rtk_bt_le_scan_duplicate_t;

/**
 * @typedef   rtk_bt_le_link_role_t
 * @brief     Bluetooth LE GAP link role definition.
 */
typedef enum {
	RTK_BT_LE_ROLE_MASTER           = 0x00, /*!< 0x00, means master */
	RTK_BT_LE_ROLE_SLAVE            = 0x01, /*!< 0x01, means slave */
	RTK_BT_LE_ROLE_UNKNOWN,                 /*!< unknown */
} rtk_bt_le_link_role_t;

/**
 * @typedef   rtk_bt_le_adv_stop_reason_t
 * @brief     Bluetooth LE ADV type Definition
 */
typedef enum {
	RTK_BT_LE_ADV_STOP_BY_HOST      = 0,        /*!< 0, stopped by host */
	RTK_BT_LE_ADV_STOP_BY_CONN      = 1,        /*!< 1, stopped due to connection established */
	RTK_BT_LE_ADV_STOP_BY_DURATION  = 2,        /*!< 2, stopped due to duration expired or number of extended adv events exceeded */
	RTK_BT_LE_ADV_STOP_UNKNOWN,                 /*!< unknown */
} rtk_bt_le_adv_stop_reason_t;

/**
 * @struct    rtk_bt_le_adv_param_t
 * @brief     Bluetooth LE GAP set adv data paramter definition.
 */
typedef struct {
	/**
	 * Minimum advertising interval for undirected and low duty cycle\n
	 * directed advertising.\n
	 * Range: 0x0020 to 0x4000\n
	 * Default: 0x0800 (1.28 s)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 20 ms to 10.24 s
	 */
	uint16_t interval_min;
	/**
	 * Maximum advertising interval for undirected and low duty cycle\n
	 * directed advertising.\n
	 * Range: 0x0020 to 0x4000\n
	 * Default: 0x0800 (1.28 s)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 20 ms to 10.24 s
	 */
	uint16_t interval_max;
	/** Advertising_Type */
	rtk_bt_le_adv_type_t type;
	/** Own_Address_Type */
	rtk_bt_le_addr_type_t own_addr_type;
	/** Peer_Address */
	rtk_bt_le_addr_t peer_addr;
	/** Advertising_Channel_Map */
	rtk_bt_le_adv_ch_map_t channel_map;
	/** Advertising_Filter_Policy */
	rtk_bt_le_adv_filter_t filter_policy;
} rtk_bt_le_adv_param_t;

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
/**
 * @struct    rtk_bt_le_ext_adv_param_t
 * @brief     Bluetooth LE GAP ext adv paramter definition.
 */
typedef struct {
	/** Advertising_Event_Properties */
	rtk_bt_le_adv_event_prop_t adv_event_prop;
	/**
	 * Minimum advertising interval for undirected and low duty cycle\n
	 * directed advertising.\n
	 * Range: 0x000020 to 0xFFFFFF\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 20 ms to 10,485.759375 s
	 */
	uint32_t primary_adv_interval_min;
	/**
	 * Maximum advertising interval for undirected and low duty cycle\n
	 * directed advertising.\n
	 * Range: 0x000020 to 0xFFFFFF\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 20 ms to 10,485.759375 s
	 */
	uint32_t primary_adv_interval_max;
	/** Advertising_Primary_Channel_Map */
	rtk_bt_le_adv_ch_map_t primary_adv_channel_map;
	/** Own_Address_Type_and_Address */
	rtk_bt_le_addr_t own_addr;
	/** Peer_Address_Type_and_Address */
	rtk_bt_le_addr_t peer_addr;
	/** Advertising_Filter_Policy */
	rtk_bt_le_adv_filter_t filter_policy;
	/**
	 * Advertising_TX_Power: indicates max Tx power.\n
	 * Range: -127 to +20\n
	 * Units: dBm\n
	 * 0x7F: Host has no preference.
	 */
	int8_t tx_power;
	/** Primary_Advertising_PHY */
	rtk_bt_le_phy_prim_adv_t primary_adv_phy;
	/** Secondary_Advertising_Max_Skip */
	uint8_t secondary_adv_max_skip;
	/** Secondary_Advertising_PHY */
	rtk_bt_le_phy_type_t secondary_adv_phy;
	/**
	 * Value of the Advertising SID subfield in the ADI field of the PDU.\n
	 * Range: 0x00 to 0x0F\n
	 */
	uint8_t adv_sid;
} rtk_bt_le_ext_adv_param_t;

/**
 * @struct    rtk_bt_le_ext_adv_create_t
 * @brief     Bluetooth LE GAP create ext adv definition.
 */
typedef struct {
	/** Advertising paramter */
	rtk_bt_le_ext_adv_param_t *p_adv_param;
	/** Advertising set handle created by stack */
	uint8_t *p_adv_handle;
} rtk_bt_le_ext_adv_create_t;

/**
 * @struct    rtk_bt_le_ext_adv_start_t
 * @brief     Bluetooth LE GAP start ext adv definition.
 */
typedef struct {
	/** Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	uint8_t adv_handle;
	/** If non-zero: Advertising duration, in units of 10ms */
	uint16_t duration;
	/**
	 * If non-zero, indicates the maximum extended advertising events that shall attempt to
	 * send prior to terminating the extended advertising.
	 */
	uint8_t num_events;
} rtk_bt_le_ext_adv_start_t;

/**
 * @typedef   rtk_bt_le_ext_adv_data_t
 * @brief     Bluetooth LE GAP set ext adv data definition.
 */
typedef struct {
	/** Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	uint8_t adv_handle;
	/** Advertising Data */
	uint8_t *pdata;
	/** Advertising Data Len */
	uint16_t len;
} rtk_bt_le_ext_adv_data_t;

/**
 * @struct    rtk_bt_le_ext_create_conn_param_t
 * @brief     Bluetooth LE GAP create ext connection paramter definition.
 */
typedef struct {
	/** Initiator_Filter_Policy */
	rtk_bt_le_conn_filter_t filter_policy;
	/** Own_Address_Type */
	rtk_bt_le_addr_type_t own_addr_type;
	/** Peer_Address */
	rtk_bt_le_addr_t peer_addr;
	/** Initiating_PHYs\n
	 * init_phys[0] for 1M PHY, init_phys[1] for 2M PHY, init_phys[2] for Coded PHY\n
	 */
	bool init_phys[3];
	/**
	 * Time interval from when the Controller started its last scan until it\n
	 * begins the subsequent scan on the primary advertising channel.
	 * Range: 0x0004 to 0xFFFF\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 40.959375 s\n
	 * scan_interval[0] for 1M PHY, scan_interval[1] for 2M PHY, scan_interval[2] for Coded PHY\n
	 */
	uint16_t scan_interval[3];
	/**
	 * Duration of the scan on the primary advertising channel.\n
	 * shall be less than or equal to LE_Scan_Interval\n
	 * Range: 0x0004 to 0xFFFF\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 40.959375 s\n
	 * scan_window[0] for 1M PHY, scan_window[1] for 2M PHY, scan_window[2] for Coded PHY\n
	 */
	uint16_t scan_window[3];
	/**
	 * Minimum value for the connection interval. This shall be less\n
	 * than or equal to Connection_Interval_Max[i].\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4 s\n
	 * conn_interval_min[0] for 1M PHY, conn_interval_min[1] for 2M PHY, conn_interval_min[2] for Coded PHY\n
	 */
	uint16_t conn_interval_min[3];
	/**
	 * Maximum value for the connection interval. This shall be\n
	 * greater than or equal to Connection_Interval_Min[i].\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4 s\n
	 * conn_interval_max[0] for 1M PHY, conn_interval_max[1] for 2M PHY, conn_interval_max[2] for Coded PHY\n
	 */
	uint16_t conn_interval_max[3];
	/**
	 * Slave latency for the connection in number of connection events.\n
	 * Range: 0x0000 to 0x01F3\n
	 * conn_latency[0] for 1M PHY, conn_latency[1] for 2M PHY, conn_latency[2] for Coded PHY\n
	 */
	uint16_t conn_latency[3];
	/**
	 * Supervision timeout for the LE Link.\n
	 * Range: 0x000A to 0x0C80\n
	 * Time = N * 10 ms\n
	 * Time Range: 100 ms to 32 s\n
	 * supv_timeout[0] for 1M PHY, supv_timeout[1] for 2M PHY, supv_timeout[2] for Coded PHY\n
	 */
	uint16_t supv_timeout[3];
	/**
	 * scan timeout value
	 */
	uint16_t scan_timeout;
} rtk_bt_le_ext_create_conn_param_t;
#endif

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
/**
 * @typedef   rtk_bt_le_pa_prop_t
 * @brief     Periodic Advertising Properties flag. Use the combination of enum definitions to indicate which fields should be included in the advertising packet.
 */
typedef enum {
	RTK_BT_LE_PA_ADV_PROP_INCLUDE_TX_POWER = 0x0040   /**< Include TxPower in the periodic header of the advertising PDU. */
} rtk_bt_le_pa_prop_t;

/**
 * @struct    rtk_bt_le_pa_param_t
 * @brief     Bluetooth LE GAP set periodic adv paramter definition.
 */
typedef struct {
	/** Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	uint8_t adv_handle;
	/**
	 * Minimum advertising interval for periodic advertising.\n
	 * Must be different from periodic_adv_interval_max.\n
	 * Range: 0x0006 to 0xFFFF\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 81.91875 s\n
	 */
	uint16_t pa_interval_min;
	/**
	 * Maximum advertising interval for periodic advertising.\n
	 * Must be different from periodic_adv_interval_min.\n
	 * Range: 0x0006 to 0xFFFF\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 81.91875 s\n
	 */
	uint16_t pa_interval_max;
	/** Periodic_Advertising_Properties, @ref rtk_bt_le_pa_prop_t */
	uint16_t pa_prop;
	/** Advertising Data */
	uint8_t *pa_data;
	/** Advertising Data Len */
	uint16_t pa_len;
#if defined(RTK_BLE_5_4_PA_RSP_SUPPORT) && RTK_BLE_5_4_PA_RSP_SUPPORT
	/**
	 * Number of subevents
	 * If zero, the periodic advertiser will be a broadcaster, without responses.
	 */
	uint8_t num_subevents;
	/** Interval between subevents (N * 1.25 ms), shall be between 7.5ms and 318.75 ms. */
	uint8_t subevent_interval;
	/** Time between the advertising packet in a subevent and the first response slot (N * 1.25 ms) */
	uint8_t response_slot_delay;
	/** Time between response slots (N * 0.125 ms), shall be between 0.25 and 31.875 ms. */
	uint8_t response_slot_spacing;
	/**
	 * Number of subevent response slots
	 * If zero, response_slot_delay and response_slot_spacing are ignored.
	 */
	uint8_t num_response_slots;
#endif
} rtk_bt_le_pa_param_t;

/**
 * @struct    rtk_bt_le_pa_update_t
 * @brief     Bluetooth LE GAP update periodic adv data definition.
 */
typedef struct {
	/** Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	uint8_t adv_handle;
	/** Just update advertising DID, keep advertising data unchanged */
	bool update_did_only;
	/** Advertising Data */
	uint8_t *pa_data;
	/** Advertising Data Len */
	uint16_t pa_len;
} rtk_bt_le_pa_update_t;
#endif

/**
 * @struct    rtk_bt_le_scan_info_filter_param_t
 * @brief     Bluetooth LE GAP scan info filter paramters definition.
 */
typedef struct {
	bool enable;        /*!< Wether to enable the scan info comparison function */
	uint8_t offset;     /*!< The start offset of the scan info to compare */
	uint8_t len;        /*!< Length of data to compare */
	uint8_t *p_filter;  /*!< Point the data to compare with the scan info */
} rtk_bt_le_scan_info_filter_param_t;

/**
 * @struct    rtk_bt_le_scan_param_t
 * @brief     Bluetooth LE GAP scan paramters definition.
 */
typedef struct {
	/** LE_Scan_Type */
	rtk_bt_le_scan_type_t type;
	/**
	 * This is defined as the time interval from when the Controller\n
	 * started its last LE scan until it begins the subsequent LE scan.\n
	 * Range: 0x0004 to 0x4000\n
	 * Default: 0x0010 (10 ms)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 10.24 s\n
	 */
	uint16_t interval;
	/**
	 * The duration of the LE scan. LE_Scan_Window shall be less\n
	 * than or equal to LE_Scan_Interval\n
	 * Range: 0x0004 to 0x4000\n
	 * Default: 0x0010 (10 ms)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 10.24 s\n
	 */
	uint16_t window;
	/** Own_Address_Type */
	rtk_bt_le_addr_type_t own_addr_type;
	/** Scanning_Filter_Policy */
	rtk_bt_le_scan_filter_t filter_policy;
	/** Scanning_Filter_Duplicated_Option */
	uint8_t duplicate_opt;
} rtk_bt_le_scan_param_t;

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
/**
 * @struct    rtk_bt_le_ext_scan_param_t
 * @brief     Bluetooth LE GAP ext scan paramters definition.
 */
typedef struct {
	/** Own_Address_Type */
	rtk_bt_le_addr_type_t own_addr_type;

	/**
	 * Scanning_PHYs\n
	 * phys[0] for 1M PHY, phys[1] for Coded PHY\n
	 * */
	bool phys[2];

	/** LE_Scan_Type: type[0] for 1M PHY, type[1] for Coded PHY*/
	rtk_bt_le_scan_type_t type[2];
	/**
	 * This is defined as the time interval from when the Controller\n
	 * started its last LE scan until it begins the subsequent LE scan.\n
	 * Range: 0x0004 to 0xFFFF\n
	 * Default: 0x0040 (40 ms)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 40.959375 s\n
	 * interval[0] for 1M PHY, interval[1] for Coded PHY.\n
	 */
	uint16_t interval[2];
	/**
	 * The duration of the LE scan. LE_Scan_Window shall be less\n
	 * than or equal to LE_Scan_Interval\n
	 * Range: 0x0004 to 0xFFFF\n
	 * Default: 0x0020 (20 ms)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 40.959375 s\n
	 * window[0] for 1M PHY, window[1] for Coded PHY.\n
	 */
	uint16_t window[2];

	/**
	 * 0x0000 indicates scan continuously until explicitly disable.\n
	 * Other values indicate scan duration\n
	 * Range: 0x0001 to 0xFFFF\n
	 * Time = N * 10 ms\n
	 * Time Range: 10 ms to 655.35 s\n
	 */
	uint16_t duration;

	/**
	 * 0x0000 indicates scan continuously.\n
	 * Other values indicate time interval from when the Controller started\n
	 * its last Scan_Duration until it begins the subsequent Scan_Duration.\n
	 * Range: 0x0001 to 0xFFFF\n
	 * Time = N * 1.28 sec\n
	 * Time Range: 1.28 s to 83,884.8 s\n
	 */
	uint16_t period;

	/** Scanning_Filter_Policy */
	rtk_bt_le_scan_filter_t filter_policy;
	/** Scanning_Duplicated_Option */
	rtk_bt_le_scan_duplicate_t duplicate_opt;
} rtk_bt_le_ext_scan_param_t;
#endif

/**
 * @struct    rtk_bt_le_create_conn_param_t
 * @brief     Bluetooth LE GAP create connect parameters definition.
 */
typedef struct {
	/**
	 * This is defined as the time interval from when the Controller started\n
	 * its last LE scan until it begins the subsequent LE scan.\n
	 * Range: 0x0004 to 0x4000\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 10.24 s\n
	 */
	uint16_t scan_interval;
	/**
	 * Amount of time for the duration of the LE scan. LE_Scan_Window\n
	 * shall be less than or equal to LE_Scan_Interval\n
	 * Range: 0x0004 to 0x4000\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 10.24 s\n
	 */
	uint16_t scan_window;
	/** Initiator_Filter_Policy */
	rtk_bt_le_conn_filter_t filter_policy;
	/** Peer_Address */
	rtk_bt_le_addr_t peer_addr;
	/** Own_Address_Type */
	rtk_bt_le_addr_type_t own_addr_type;
	/**
	 * Minimum value for the connection interval. This shall be less\n
	 * than or equal to Connection_Interval_Max.\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4 s.\n
	 */
	uint16_t conn_interval_min;
	/**
	 * Maximum value for the connection interval. This shall be\n
	 * greater than or equal to Connection_Interval_Min.\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4 s.\n
	 */
	uint16_t conn_interval_max;
	/**
	 * Slave latency for the connection in number of connection events.\n
	 * Range: 0x0000 to 0x01F3\n
	 */
	uint16_t conn_latency;
	/**
	 * Supervision timeout for the LE Link.\n
	 * Range: 0x000A to 0x0C80\n
	 * Time = N * 10 ms\n
	 * Time Range: 100 ms to 32 s\n
	 */

	uint16_t supv_timeout;
	/**
	 * scan timeout value
	 */
	uint16_t scan_timeout;
} rtk_bt_le_create_conn_param_t;

/**
 * @struct    rtk_bt_le_update_conn_param_t
 * @brief     Bluetooth LE GAP update connection parameters definition.
 */
typedef struct {
	/** Connection handle */
	uint16_t conn_handle;
	/**
	 * Minimum value for the connection interval. This shall be less\n
	 * than or equal to Connection_Interval_Max.\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4 s.\n
	 */
	uint16_t conn_interval_min;
	/**
	 * Maximum value for the connection interval. This shall be\n
	 * greater than or equal to Connection_Interval_Min.\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4 s.\n
	 */
	uint16_t conn_interval_max;
	/**
	 * Slave latency for the connection in number of connection events.\n
	 * Range: 0x0000 to 0x01F3\n
	 */
	uint16_t conn_latency;
	/**
	 * Supervision timeout for the LE Link.\n
	 * Range: 0x000A to 0x0C80\n
	 * Time = N * 10 ms\n
	 * Time Range: 100 ms to 32 s\n
	 */
	uint16_t supv_timeout;
} rtk_bt_le_update_conn_param_t;

/**
 * @enum      rtk_bt_le_wl_op_t
 * @brief     Bluetooth LE GAP whitelist operation type definition.
 */
typedef enum {
	RTK_BT_LE_WHITELIST_CLEAR,                /*!< Clear whitelist */
	RTK_BT_LE_WHITELIST_ADD,                  /*!< Add device to whitelist */
	RTK_BT_LE_WHITELIST_REMOVE,               /*!< Remove device from whitelist */
	RTK_BT_LE_WHITELIST_RESERVED,             /*!< All bigger value are reserved  */
} rtk_bt_le_wl_op_t;

/**
 * @struct    rtk_bt_le_modify_wl_param_t
 * @brief     Bluetooth LE GAP whitelist operation paramters definition.
 */
typedef struct {
	rtk_bt_le_wl_op_t op;                  /*!< whitelist operation type */
	rtk_bt_le_addr_t addr;                 /*!< device address */
} rtk_bt_le_modify_wl_param_t;

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
/**
 * @enum    rtk_bt_le_resolv_list_op_t
 * @brief   Bluetooth LE resolving list operation definition.
 */
typedef enum {
	RTK_BT_LE_RESOLV_LIST_OP_CLEAR  = 0x00,    /**< Clear resolving list operation. */
	RTK_BT_LE_RESOLV_LIST_OP_ADD    = 0x01,    /**< Add a device to the resolving list operation. */
	RTK_BT_LE_RESOLV_LIST_OP_REMOVE = 0x02     /**< Remove a device from the resolving list operation. */
} rtk_bt_le_resolv_list_op_t;

/**
 * @struct  rtk_bt_le_resolv_list_entry_t
 * @brief   Bluetooth LE modify resolving list parameter.
 */
typedef struct {
	rtk_bt_le_ident_addr_type_t addr_type;      /*!< Peer identity address type */
	uint8_t addr[RTK_BD_ADDR_LEN];              /*!< Peer identity address */
	bool device_mode;                           /*!< peer enable device privacy mode  */
} rtk_bt_le_resolv_list_entry_t;

/**
 * @struct  rtk_bt_le_modify_resolv_list_t
 * @brief   Bluetooth LE modify resolving list parameter.
 */
typedef struct {
	rtk_bt_le_resolv_list_op_t op;              /*!< Resolving list modify operation */
	rtk_bt_le_resolv_list_entry_t entry;        /*!< Resolving list entry */
} rtk_bt_le_modify_resolv_list_t;

/**
 * @struct  rtk_bt_le_modify_resolv_list_t
 * @brief   Bluetooth LE modify resolving list parameter.
 */
typedef struct {
	rtk_bt_le_resolv_list_entry_t *p_entry;     /*!< Resolving list entry */
	uint8_t *p_size;                            /*!< Resolving list entry size */
} rtk_bt_le_get_resolv_list_t;
#endif

/**
 * @def       RTK_BT_LE_CHAN_MAP_LEN
 * @brief     Bluetooth LE GAP channel map size macro.
 */
#define RTK_BT_LE_CHAN_MAP_LEN                5

/**
 * @struct    rtk_bt_le_conn_info_t
 * @brief     Bluetooth LE connection info definition.
 */
typedef struct {
	/** Controller role */
	rtk_bt_le_link_role_t role;
	/** Remote device address used during connection setup. */
	rtk_bt_le_addr_t remote;
	/**
	 * Connection interval used on this connection.\n
	 * Range: 0x0006 to 0x0C80\n
	 * Time = N * 1.25 ms\n
	 * Time Range: 7.5 ms to 4000 ms.\n
	 */
	uint16_t interval;
	/**
	 * Slave latency for the connection in number of connection events.\n
	 * Range: 0x0000 to 0x01F3\n
	 */
	uint16_t latency;
	/**
	 * Connection supervision timeout.
	 * Range: 0x000A to 0x0C80
	 * Time = N * 10 ms
	 * Time Range: 100 ms to 32 s
	 */
	uint16_t supv_timeout;
	/** connection tx phy */
	uint8_t tx_phy;
	/** connection rx phy */
	uint8_t rx_phy;
} rtk_bt_le_conn_info_t;

/**
 * @enum      rtk_bt_le_gap_init_state
 * @brief     Bluetooth LE GAP init state.
 */
enum rtk_bt_le_gap_init_state {
	RTK_BT_LE_GAP_INIT_STATE_INIT           = 0,    /*!< GAP is in initiating status */
	RTK_BT_LE_GAP_INIT_STATE_STACK_READY    = 1,    /*!< GAP stack is ready */
};

/**
 * @enum      rtk_bt_le_adv_state
 * @brief     Bluetooth LE GAP adv state.
 */
enum rtk_bt_le_adv_state {
	RTK_BT_LE_ADV_STATE_IDLE                = 0,    /*!< Idle, no advertising */
	RTK_BT_LE_ADV_STATE_START               = 1,    /*!< Start Advertising. A temporary state, haven't received the result */
	RTK_BT_LE_ADV_STATE_ADVERTISING         = 2,    /*!< Advertising */
	RTK_BT_LE_ADV_STATE_STOP                = 3,    /*!< Stop Advertising. A temporary state, haven't received the result */
};

/**
 * @enum    rtk_bt_le_scan_state
 * @brief     Bluetooth LE GAP scan state.
 */
enum rtk_bt_le_scan_state {
	RTK_BT_LE_SCAN_STATE_IDLE               = 0,    /*!< Idle, no scanning */
	RTK_BT_LE_SCAN_STATE_START              = 1,    /*!< Start scanning. A temporary state, haven't received the result */
	RTK_BT_LE_SCAN_STATE_SCANNING           = 2,    /*!< Scanning */
	RTK_BT_LE_SCAN_STATE_STOP               = 3,    /*!< Stop scanning, A temporary state, haven't received the result */
};

/**
 * @enum      rtk_bt_le_conn_state
 * @brief     Bluetooth LE GAP connection state.
 */
enum rtk_bt_le_conn_state {
	RTK_BT_LE_CONN_STATE_IDLE               = 0,    /*!< Idle */
	RTK_BT_LE_CONN_STATE_INITIATING         = 1,    /*!< Initiating Connection */
};

/**
 * @struct    rtk_bt_le_gap_dev_state_t
 * @brief     Bluetooth LE GAP dev state.
 */
typedef struct {
	uint8_t gap_init_state;         /*!< @ref rtk_bt_le_gap_init_state */
	uint8_t gap_adv_state;          /*!< @ref rtk_bt_le_adv_state */
	uint8_t gap_scan_state;         /*!< @ref rtk_bt_le_scan_state */
	uint8_t gap_conn_state;         /*!< @ref rtk_bt_le_conn_state */
} rtk_bt_le_gap_dev_state_t;

/**
 * @struct    rtk_bt_le_get_active_conn_t
 * @brief     Bluetooth LE GAP get active connection num and their handles.
 */
typedef struct {
	uint16_t conn_handle[RTK_BLE_GAP_MAX_LINKS];
	uint16_t conn_num;
} rtk_bt_le_get_active_conn_t;

/**
 * @struct    rtk_bt_le_set_datalen_param_t
 * @brief     Bluetooth LE GAP set connection data length parameters definition.
 */
typedef struct {
	uint16_t conn_handle;           /*!< connction handle */
	uint16_t max_tx_octets;         /*!< Maximum tx octects */
	uint16_t max_tx_time;           /*!< Maximum tx time */
} rtk_bt_le_set_datalen_param_t;

/**
 * @struct    rtk_bt_le_set_phy_param_t
 * @brief     Bluetooth LE GAP set connection phy parameters definition.
 */
typedef struct {
	uint16_t conn_handle;       /*!< connection handle */
	uint8_t all_phys;           /*!< preferred all phys, @ref rtk_bt_le_all_phy_prefer_t */
	uint8_t tx_phys;            /*!< preferred tx phys, @ref rtk_bt_le_trx_phy_prefer_t */
	uint8_t rx_phys;            /*!< preferred rx phys, @ref rtk_bt_le_trx_phy_prefer_t */
	uint16_t phy_options;       /*!< preferred phy options, @ref rtk_bt_le_phy_option_t */
} rtk_bt_le_set_phy_param_t;

/**
 * @typedef   rtk_bt_le_pairing_mode_t
 * @brief     Bluetooth BLE SM pairing mode definition.
 */
typedef enum {
	RTK_PAIRING_MODE_NO_PAIRING  = 0x00,
	RTK_PAIRING_MODE_PAIRABLE    = 0x01,
} rtk_bt_le_pairing_mode_t;

/**
 * @typedef   rtk_bt_le_io_cap_t
 * @brief     Bluetooth BLE SM IO capability type definition.
 */
typedef enum {
	RTK_IO_CAP_DISPALY_ONLY     = 0x00,     /*!< 0x00 DisplayOnly */
	RTK_IO_CAP_DISPLAY_YES_NO   = 0x01,     /*!< 0x01 DisplayYesNo */
	RTK_IO_CAP_KEYBOARD_ONLY    = 0x02,     /*!< 0x02 KeyboardOnly */
	RTK_IO_CAP_NO_IN_NO_OUT     = 0x03,     /*!< 0x03 NoInputNoOutput */
	RTK_IO_CAP_KEYBOARD_DISPALY = 0x04,     /*!< 0x04 KeyboardDisplay */
	RTK_IO_CAP_MAX              = 0x05,     /*!< 0x05-0xFF Reserved for future use */
} rtk_bt_le_io_cap_t;

/**
 * @def       RTK_PAIRING_PASSKEY_MAX
 * @brief     Bluetooth LE SM max passkey value macro.
 */
#define RTK_PAIRING_PASSKEY_MAX     999999

/**
 * @struct    rtk_bt_le_security_param_t
 * @brief     Bluetooth BLE SM security parameter type definition.
 */
typedef struct {
	rtk_bt_le_io_cap_t io_cap;              /*!< IO capabilities */
	uint8_t oob_data_flag;                  /*!< OOB data flag */
	uint8_t bond_flag;                      /*!< Bonding flags */
	uint8_t mitm_flag;                      /*!< MITM flag */
	uint8_t sec_pair_flag;                  /*!< Secure connection pairing support flag */
	uint8_t sec_pair_only_flag;             /*!< Only accept secure connection pairing when local sec_pair_flag is set */
	uint8_t use_fixed_key;                  /*!< Pairing use fixed passkey */
	uint32_t fixed_key;                     /*!< Fixed passkey value */
	uint8_t auto_sec_req;                   /*!< Auto send security request when connected */
	uint8_t sign_key_flag;                  /*!< Sign key flag, this field can't be get by @ref rtk_bt_le_sm_get_security_param */
} rtk_bt_le_security_param_t;

/**
 * @struct    rtk_bt_le_pair_cfm_t
 * @brief     Bluetooth BLE SM pairing confirm parameter definition.
 */
typedef struct {
	uint16_t conn_handle;       /*!< Connection handle */
	uint8_t confirm;            /*!< Confirm value, 1: continue pairing, 0: stop pairing */
} rtk_bt_le_pair_cfm_t;

/**
 * @struct    rtk_bt_le_auth_key_confirm_t
 * @brief     Bluetooth BLE SM key confirm definition.
 */
typedef struct {
	uint16_t conn_handle;       /*!< Connection handle */
	uint8_t confirm;            /*!< Confirm value, 1: passkey correct, 0: passkey incorrect */
} rtk_bt_le_auth_key_confirm_t;

/**
 * @struct    rtk_bt_le_auth_key_input_t
 * @brief     Bluetooth BLE SM input auth key definition.
 */
typedef struct {
	uint16_t conn_handle;       /*!< Connection handle */
	uint32_t passkey;           /*!< Input passkey value */
} rtk_bt_le_auth_key_input_t;

/**
 * @def       RTK_BT_LE_SM_OOB_KEY_LEN
 * @brief     Bluetooth LE SM max oob key length macro.
 */
#define RTK_BT_LE_SM_OOB_KEY_LEN              16

/**
 * @struct    rtk_bt_le_set_oob_key_t
 * @brief     Bluetooth BLE SM set OOB key parameter definition.
 */
typedef struct {
	uint16_t conn_handle;                           /*!< Connection handle */
	uint8_t oob_key[RTK_BT_LE_SM_OOB_KEY_LEN];      /*!< OOB KEY to input */
} rtk_bt_le_set_oob_key_t;

/**
 * @struct    rtk_bt_le_sc_local_oob_data_t
 * @brief     Bluetooth BLE SM get secure connection pairing local OOB data definition.
 */
typedef struct {
	uint8_t rand[16];
	uint8_t confirm[16];
} rtk_bt_le_sc_local_oob_data_t;

/**
 * @struct    rtk_bt_le_sc_peer_oob_data_t
 * @brief     Bluetooth BLE SM input secure connection pairing peer OOB data definition.
 */
typedef struct {
	uint8_t addr[RTK_BD_ADDR_LEN];
	uint8_t rand[16];
	uint8_t confirm[16];
} rtk_bt_le_sc_peer_oob_data_t;

/**
 * @struct    rtk_bt_le_bond_info_t
 * @brief     Bluetooth BLE SM bond information type definition.
 */
typedef struct {
	rtk_bt_le_addr_t remote_addr;           /*!< Bonded device address */
	rtk_bt_le_ident_addr_t ident_addr;      /*!< Identity address */
} rtk_bt_le_bond_info_t;

/**
 * @struct    rtk_bt_le_adv_start_ind_t
 * @brief     Bluetooth LE adv start indication msg.
 */
typedef struct {
	uint16_t err;                           /*!< Error code */
	rtk_bt_le_adv_type_t  adv_type;         /*!< Adv type */
} rtk_bt_le_adv_start_ind_t;

/**
 * @struct    rtk_bt_le_adv_stop_ind_t
 * @brief     Bluetooth LE adv stop indication msg.
 */
typedef struct {
	uint16_t err;                                   /*!< Error code */
	rtk_bt_le_adv_stop_reason_t  stop_reason;       /*!< Adv stop reason */
} rtk_bt_le_adv_stop_ind_t;

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
/**
 * @struct    rtk_bt_le_ext_adv_ind_t
 * @brief     Bluetooth LE ext adv state indication msg.
 */
typedef struct {
	uint8_t adv_handle;      /*!< Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	bool is_start;           /*!< Start or Stop */
	uint16_t err;            /*!< Error code */
	rtk_bt_le_adv_stop_reason_t stop_reason; /*!< Extended adv stop reason, valid when member is_start is false. */
} rtk_bt_le_ext_adv_ind_t;
#endif

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
/**
 * @typedef    rtk_bt_le_pa_state_t
 * @brief     Bluetooth LE periodic adv state.
 */
typedef enum {
	RTK_BT_LE_PA_STATE_IDLE,                        /**< Idle, no periodic advertising. */
	RTK_BT_LE_PA_STATE_ADVERTISING,                 /**< Periodic advertising. */
	RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING,    /**< Periodic advertising is not started until the extended advertising of the specified advertising set is enabled. */
} rtk_bt_le_pa_state_t;

/**
 * @struct    rtk_bt_le_pa_ind_t
 * @brief     Bluetooth LE periodic adv state indication msg.
 */
typedef struct {
	uint8_t adv_handle;             /*!< Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	rtk_bt_le_pa_state_t state;     /*!< State */
	uint16_t cause;                 /*!< reason */
} rtk_bt_le_pa_ind_t;
#endif

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
/**
 * @typedef   rtk_bt_le_pa_synced_info_t
 * @brief     Bluetooth LE GAP PA Sync Common Sync parameter.
 */
typedef struct {
	uint16_t sync_handle;                   /*!< Sync_Handle identifying the periodic advertising train. */
	uint8_t adv_sid;                        /*!< Value of the Advertising SID subfield in the ADI field of the PDU. */
	rtk_bt_le_addr_t addr;                  /*!< Advertiser address */
	rtk_bt_le_phy_type_t adv_phy;           /*!< Advertiser_PHY specifies the PHY used for the periodic advertising. */
	uint8_t adv_clock_accuracy;             /**< Advertiser_Clock_Accuracy specifies the accuracy of the periodic advertiser's clock.
                                                 0x00: 500 ppm
                                                 0x01: 250 ppm
                                                 0x02: 150 ppm
                                                 0x03: 100 ppm
                                                 0x04: 75  ppm
                                                 0x05: 50  ppm
                                                 0x06: 30  ppm
                                                 0x07: 20  ppm */
	uint16_t periodic_adv_interval;         /**< Periodic advertising interval.
                                                 Range: 0x0006 to 0xFFFF
                                                 Time = N * 1.25 ms
                                                 Time Range: 7.5 ms to 81.91875 s */
#if defined(RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT) && RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT
	uint8_t num_subevents;                  /*!< Number of subevents */
	uint8_t subevent_interval;              /*!< Subevent interval (N * 1.25ms) */
	uint8_t response_slot_delay;            /*!< Response slot delay (N * 1.25ms) */
	uint8_t response_slot_spacing;          /*!< Response slot spacing (N * 1.25ms) */
#endif
	bool past_received;                     /*!< Synchronization is received by params or established by periodic adv */
	uint16_t conn_handle;                   /*!< Handle of connection from which the synchronization params received. Only for past_received that is True. */
	uint16_t service_data;                  /*!< Application service data provided by the synchronization params sender. Only for past_received that is True. */
} rtk_bt_le_pa_synced_info_t;

/**
 * @typedef   rtk_bt_le_pa_sync_state_t
 * @brief     Bluetooth LE GAP PA synchronization state.
 */
typedef enum {
	RTK_BT_LE_PA_SYNC_STATE_TERMINATED                  = 0x00, /**< Terminated. */
	RTK_BT_LE_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING = 0x02, /**< No attempt to synchronize will take place while extended scanning is disabled. */
	RTK_BT_LE_PA_SYNC_STATE_SYNCHRONIZING               = 0x03, /**< Start synchronizing when extended scanning is enabled. */
	RTK_BT_LE_PA_SYNC_STATE_SYNCHRONIZED                = 0x04, /**< Synchronized. */
} rtk_bt_le_pa_sync_state_t;

/**
 * @struct    rtk_bt_le_pa_sync_ind_t
 * @brief     Bluetooth LE periodic adv synchronization state indication msg.
 */
typedef struct {
	uint8_t sync_id;                    /*!< Identify the synchronization with a periodic advertising train. */
	rtk_bt_le_pa_sync_state_t state;    /*!< State */
	uint16_t cause;                     /*!< reason */
	rtk_bt_le_pa_synced_info_t info;    /*!< Synchronization info. Only valid when state is Synchronized */
} rtk_bt_le_pa_sync_ind_t;

/**
 * @struct    rtk_bt_le_gap_pawr_data_req_ind_t
 * @brief     Bluetooth LE periodic adv subevent data request indication msg.
 */
typedef struct {
	uint8_t adv_handle;                 /*!< Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	uint8_t start;                      /*!< The first subevent data can be set for */
	uint8_t count;                      /*!< The number of subevents data can be set for */
} rtk_bt_le_gap_pawr_data_req_ind_t;

/**
 * @struct    rtk_bt_le_gap_pawr_rsp_ind_t
 * @brief     Bluetooth LE periodic adv subevent data request indication msg.
 */
typedef struct {
	uint8_t adv_handle;                 /*!< Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv. */
	uint8_t subevent;                   /*!< The subevent the response was received in */
	uint8_t tx_status;                  /**< Status of the subevent indication.
                                             0 if subevent indication was transmitted.
                                             1 if subevent indication was not transmitted.
                                             All other values RFU. */
	int8_t tx_power;                    /*!< The TX power of the response in dBm */
	int8_t rssi;                        /*!< The RSSI of the response in dBm */
	uint8_t cte_type;                   /*!< The Constant Tone Extension (CTE) of the advertisement (@ref rtk_bt_le_gap_cte_type_t) */
	uint8_t response_slot;              /*!< The slot the response was received in */
	uint16_t len;                       /*!< The received data length */
	uint8_t *data;                      /*!< The received data */
} rtk_bt_le_gap_pawr_rsp_ind_t;

/**
  * @typedef  rtk_bt_le_pa_sync_report_data_status_t
  * @brief    Definition of data status in LE Periodic Advertising Report.
 */
typedef enum {
	RTK_BT_LE_PA_REPORT_DATA_STATUS_COMPLETE  = 0x00, /**< Data complete. */
	RTK_BT_LE_PA_REPORT_DATA_STATUS_MORE      = 0x01, /**< Data incomplete, more data to come. */
	RTK_BT_LE_PA_REPORT_DATA_STATUS_TRUNCATED = 0x02, /**< Data incomplete, data truncated, no more to come. */
	RTK_BT_LE_PA_REPORT_DATA_STATUS_RX_FAILED = 0xFF, /**< Data received fail, no data is received. */
} rtk_bt_le_pa_sync_report_data_status_t;

/**
 * @struct    rtk_bt_le_pa_adv_report_ind_t
 * @brief     Bluetooth LE periodic adv synchronization adv report indication msg.
 */
typedef struct {
	uint8_t sync_id;                    /**< Identify the synchronization with a periodic advertising train. */
	uint16_t sync_handle;               /**< Identifying the periodic advertising train. */
	int8_t tx_power;                    /**< Range: -127 to +20, Units: dBm.
                                             0x7F: Tx Power information not available. */
	int8_t rssi;                        /**< Range: -127 to +20, Units: dBm.
                                             0x7F: RSSI is not available. */
	uint8_t cte_type;                   /**< @ref rtk_bt_le_gap_cte_type_t */
	uint8_t data_status;                /**< @ref rtk_bt_le_pa_sync_report_data_status_t */
#if defined(RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT) && RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT
	uint16_t periodic_event_counter;    /** The value of the event counter where the subevent indication was received. */
	uint8_t subevent;                   /** The subevent where the subevend indication was received. */
#endif
	uint8_t data_len;                   /**< Length of the Data field. Range: 0 to 247. */
	uint8_t *p_data;                    /**< Data received from a Periodic Advertising packet. */
} rtk_bt_le_pa_adv_report_ind_t;

/**
  * @typedef  rtk_bt_le_pa_sync_create_opt_t
  * @brief    Synchronization State of Periodic Advertising Create Sync Options.
  *           Use the combination of macro definitions to determine whether the Periodic Advertiser List is used,
  *           whether periodic advertising report for this periodic advertising train are initially enabled or disabled,
  *           and whether duplicate reports are filtered or not.
 */
typedef enum {
	RTK_BT_LE_PA_SYNC_CREATE_OPTIONS_USE_PERIODIC_ADV_LIST              = 0x01,   /**< Use the Periodic Advertiser List to determine which advertiser to listen to.
                                                                                       Otherwise, use the Advertising_SID, Advertiser_Address_Type, and
                                                                                       Advertiser_Address parameters to determine which advertiser to listen to.*/
	RTK_BT_LE_PA_SYNC_CREATE_OPTIONS_REPORT_INITIALLY_DISABLED          = 0x02,   /**< Reporting initially disabled.
                                                                                       Otherwise, Reporting initially enabled.*/
	RTK_BT_LE_PA_SYNC_CREATE_OPTIONS_DUPLICATE_FILTER_INITIALLY_ENABLED = 0x04,   /**< Duplicate filtering initially enabled.
                                                                                       Otherwise, Duplicate filtering initially disabled.*/
} rtk_bt_le_pa_sync_create_opt_t;

/**
  * @typedef  rtk_bt_le_pa_sync_cte_t
  * @brief    Synchronization State of Periodic Advertising Sync CTE Type.
  *           Use the combination of macro definitions to specify whether to only synchronize to
  *           periodic advertising with certain types of Constant Tone Extension (a value of 0
  *           indicates that the presence or absence of a Constant Tone Extension is irrelevant).
 */
typedef enum {
	RTK_BT_LE_PA_SYNC_CTE_TYPE_CTE_IRRELEVANT            = 0x00,   /**< A value of 0 (i.e. all bits clear) indicates that the presence or absence of
                                                                        a Constant Tone Extension is irrelevant. */

	RTK_BT_LE_PA_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOA_CTE     = 0x01,   /**< Do not sync to packets with an AoA Constant Tone Extension. */
	RTK_BT_LE_PA_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_1US = 0x02,   /**< Do not sync to packets with an AoD Constant Tone Extension with 1 us slots. */
	RTK_BT_LE_PA_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_2US = 0x04,   /**< Do not sync to packets with an AoD Constant Tone Extension with 2 us slots. */
	RTK_BT_LE_PA_SYNC_CTE_TYPE_NOT_SYNC_WITH_TYPE_3_CTE  = 0x08,   /**< Do not sync to packets with a type 3 Constant Tone Extension (currently reserved for future use). */
	RTK_BT_LE_PA_SYNC_CTE_TYPE_NOT_SYNC_WITHOUT_CTE      = 0x10,   /**< Do not sync to packets without a Constant Tone Extension*/
} rtk_bt_le_pa_sync_cte_t;

/**
 * @struct    rtk_bt_le_pa_sync_create_t
 * @brief     Bluetooth LE periodic adv synchronization create sync parameter.
 */
typedef struct {
	uint8_t options;                            /**< @ref rtk_bt_le_pa_sync_create_opt_t */
	uint8_t sync_cte_type;                      /**< Bit combination of @ref rtk_bt_le_pa_sync_cte_t */
	uint8_t adv_sid;                            /**< If Periodic Advertiser List is not used (@ref rtk_bt_le_pa_sync_create_opt_t),
                                                    Advertising SID subfield in the ADI field used to identify the Periodic Advertising. */
	rtk_bt_le_addr_t adv_addr;                  /**< If Periodic Advertiser List is not used
                                                     (@ref rtk_bt_le_pa_sync_create_opt_t),
                                                     only @ref RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC and
                                                     @ref RTK_BT_LE_ADDR_TYPE_RPA_RANDOM could be
                                                     used for creating sync. */
	uint16_t skip;                              /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                                     Range: 0x0000 to 0x01F3 */
	uint16_t sync_timeout;                      /**< Synchronization timeout for the periodic advertising train.
                                                     Range: 0x000A to 0x4000
                                                     Time = N*10 ms
                                                     Time Range: 100 ms to 163.84 s */
	uint8_t *p_sync_id;                         /**< to store created sync id */
} rtk_bt_le_pa_sync_create_t;

/**
  * @typedef  rtk_bt_le_pa_sync_advlist_op_t
  * @brief    Definition of LE Periodic Advertiser list operation.
 */
typedef enum {
	RTK_BT_LE_PA_SYNC_ADV_LIST_OP_CLEAR  = 0x00,    /**< Clear all entries from Periodic Advertiser list. */
	RTK_BT_LE_PA_SYNC_ADV_LIST_OP_ADD    = 0x01,    /**< Add an entry to Periodic Advertiser list. */
	RTK_BT_LE_PA_SYNC_ADV_LIST_OP_REMOVE = 0x02     /**< Remove one entry from Periodic Advertiser list. */
} rtk_bt_le_pa_sync_advlist_op_t;

/**
 * @struct    rtk_bt_le_pa_sync_modify_advertiser_t
 * @brief     Bluetooth LE periodic adv synchronization modify advertiser.
 */
typedef struct {
	rtk_bt_le_pa_sync_advlist_op_t operation;   /**< @ref rtk_bt_le_pa_sync_create_opt_t */
	rtk_bt_le_addr_t adv_addr;                  /**< Advertiser Address */
	uint8_t adv_sid;                            /**< Advertising SID subfield in the ADI field used to identify the Periodic Advertising. */
} rtk_bt_le_pa_sync_modify_advlist_t;
#endif

#if defined(RTK_BLE_5_1_PAST_SENDER_SUPPORT) && RTK_BLE_5_1_PAST_SENDER_SUPPORT
/**
 * @struct    rtk_bt_le_past_send_t
 * @brief     Bluetooth LE periodic adv sync transfer paramter.
 */
typedef struct {
	uint16_t conn_handle;       /**< Connection handle */
	uint16_t service_data;      /**< A value provided by the Host to identify the periodic advertising train to the peer device. */
	bool use_sync_id;           /**< Indicate following member 'idx' is sync id or adv handle.
                                     If sync info to transter is obtained from remote device by sync procedure, set use_sync_id to true.
                                     If sync info to transter is from own periodic adv, set use_sync_id to false. */
	uint8_t idx;                /**< If use_sync_id is true, idx is sync id, if use_sync_id is false, idx is adv handle. */
} rtk_bt_le_past_send_t;
#endif

#if defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
/**
  * @typedef  rtk_bt_le_past_recv_mode_t
  * @brief    GAP PAST recipient mode.
 */
typedef enum {
	RTK_BT_LE_PAST_RECV_MODE_NO_ATTEMPT_TO_SYNCHRONIZE = 0x00,                     /**< No attempt to synchronize to the periodic advertising (default). */
	RTK_BT_LE_PAST_RECV_MODE_PA_ADV_REPORT_DISABLED = 0x01,                        /**< RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND will be disabled. */
	RTK_BT_LE_PAST_RECV_MODE_PA_ADV_REPORT_ENABLED = 0x02,                         /**< RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND will be enabled with duplicate filtering disabled. */
	RTK_BT_LE_PAST_RECV_MODE_PA_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER = 0x03,   /**< RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND will be enabled with duplicate filtering enabled. */
} rtk_bt_le_past_recv_mode_t;

/** @brief  Definition of GAP PAST recipient periodic advertising sync transfer parameter. */
/**
 * @struct    rtk_bt_le_past_recv_param_t
 * @brief     Bluetooth LE periodic adv sync transfer receive paramter.
 */
typedef struct {
	uint16_t conn_handle;
	rtk_bt_le_past_recv_mode_t mode;    /**< @ref rtk_bt_le_past_recv_mode_t */
	uint8_t sync_cte_type;              /**< Bits combination of @ref rtk_bt_le_pa_sync_cte_t */
	uint16_t skip;                      /**< The maximum number of periodic advertising events that can be skipped after a successful receive.
                                             Range: 0x0000 to 0x01F3 */
	uint16_t sync_timeout;              /**< Synchronization timeout for the periodic advertising train.
                                             Range: 0x000A to 0x4000
                                             Time = N*10 ms
                                             Time Range: 100 ms to 163.84 s */
} rtk_bt_le_past_recv_param_t;
#endif

/**
 * @struct    rtk_bt_le_scan_start_ind_t
 * @brief     Bluetooth LE scan start indication msg.
 */
typedef struct {
	uint16_t err;                       /*!< Error code */
	rtk_bt_le_scan_type_t scan_type;    /*!< Scan type */
} rtk_bt_le_scan_start_ind_t;

/**
 * @enum      rtk_bt_le_adv_report_type_t
 * @brief     Bluetooth LE adv report event type.
 */
typedef enum {
	RTK_BT_LE_EVT_CONN_ADV       = 0x00,      /*!< ADV_IND         */
	RTK_BT_LE_EVT_CONN_DIR_ADV   = 0x01,      /*!< ADV_DIRECT_IND  */
	RTK_BT_LE_EVT_SCAN_UNDIR_ADV = 0x02,      /*!< ADV_SCAN_IND    */
	RTK_BT_LE_EVT_NON_CONN_ADV   = 0x03,      /*!< ADV_NONCONN_IND */
	RTK_BT_LE_EVT_SCAN_RSP       = 0x04,      /*!< SCAN_RSP        */
} rtk_bt_le_adv_report_type_t;

/**
 * @def       RTK_BT_LE_MAX_ADV_DATA_LEN
 * @brief     Bluetooth LE max adv data len.
 */
#define RTK_BT_LE_MAX_ADV_DATA_LEN  31
#define RTK_BT_LE_MAX_EXT_ADV_DATA_LEN  255

/**
 * @struct    rtk_bt_le_adv_report_t
 * @brief     Bluetooth LE adv report data.
 */
typedef struct {
	rtk_bt_le_adv_report_type_t evt_type;
	rtk_bt_le_addr_t addr;
	uint8_t len;
	uint8_t data[RTK_BT_LE_MAX_ADV_DATA_LEN];
	int8_t rssi;
} rtk_bt_le_adv_report_t;

/**
 * @struct    rtk_bt_le_scan_res_ind_t
 * @brief     Bluetooth LE scan result indication msg.
 */
typedef struct {
	uint8_t num_report;                 /*!< num of responses in event, here will be always 1 */
	rtk_bt_le_adv_report_t adv_report;  /*!< adv data to be reported from controller */
} rtk_bt_le_scan_res_ind_t;

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV

#define RTK_BT_LE_EXT_ADV_EVT_BIT_CONNECTABLE_ADV   (1 << 0)
#define RTK_BT_LE_EXT_ADV_EVT_BIT_SCANNABLE_ADV     (1 << 1)
#define RTK_BT_LE_EXT_ADV_EVT_BIT_DIRECTED_ADV      (1 << 2)
#define RTK_BT_LE_EXT_ADV_EVT_BIT_SCAN_RESPONSE     (1 << 3)

/**
 * @typedef    rtk_bt_le_ext_adv_report_type_t
 * @brief     Bluetooth LE ext adv report event type.
 */
typedef enum {
	/*!< Other value that less than 0x10, reference RTK_BT_LE_EXT_ADV_EVT_BIT_XXX bit value. */
	RTK_BT_LE_EXT_EVT_LEGACY_ADV_IND =                  0x13,
	RTK_BT_LE_EXT_EVT_LEGACY_ADV_DIRECT_IND =           0x15,
	RTK_BT_LE_EXT_EVT_LEGACY_ADV_SCAN_IND =             0x12,
	RTK_BT_LE_EXT_EVT_LEGACY_ADV_NONCONN_IND =          0x10,
	RTK_BT_LE_EXT_EVT_LEGACY_SCAN_RSP_TO_ADV_IND =      0x1B,
	RTK_BT_LE_EXT_EVT_LEGACY_SCAN_RSP_TO_ADV_SCAN_IND = 0x1A,
} rtk_bt_le_ext_adv_report_type_t;

/**
  * @typedef  rtk_bt_le_ext_adv_data_status_t
  * @brief    Definition of data status in LE Extended Advertising Report.
 */
typedef enum {
	RTK_BT_LE_EXT_ADV_DATA_STATUS_COMPLETE  = 0x00, /**< Data status: Complete */
	RTK_BT_LE_EXT_ADV_DATA_STATUS_MORE      = 0x01, /**< Data status: Incomplete, more data to come */
	RTK_BT_LE_EXT_ADV_DATA_STATUS_TRUNCATED = 0x02, /**< Data status: Incomplete, data truncated, no more to come */
	RTK_BT_LE_EXT_ADV_DATA_STATUS_RFU       = 0x03  /**< Data status: Reserved for future use */
} rtk_bt_le_ext_adv_data_status_t;

/**
 * @struct    rtk_bt_le_ext_scan_res_ind_t
 * @brief     Bluetooth LE ext scan result indication msg.
 */
typedef struct {
	uint16_t evt_type;      /*!< ref @ref rtk_bt_le_ext_adv_report_type_t, more bit field information, please ref bt spec "LE Extended Advertising Report Event". */
	rtk_bt_le_addr_t addr;
	rtk_bt_le_addr_t direct_addr;
	int8_t rssi;
	uint8_t primary_phy;
	uint8_t secondary_phy;
	uint8_t adv_sid;
	int8_t tx_power;
	uint16_t peri_adv_interval;
	uint8_t data_status;  /*!< ref @ref rtk_bt_le_ext_adv_data_status_t */
	uint16_t len;
	uint8_t *data; /*!< Must be the last member */
} rtk_bt_le_ext_scan_res_ind_t;
#endif

/**
 * @enum      rtk_bt_le_scan_stop_reason_t
 * @brief     Bluetooth LE scan stop reason.
 */
typedef enum {
	RTK_BT_LE_SCAN_STOP_BY_HOST      = 0,        /*!< 0, stopped by host */
	RTK_BT_LE_SCAN_STOP_BY_DURATION  = 1,        /*!< 1, stopped due to scan duration timeout */
	RTK_BT_LE_SCAN_STOP_UNKNOWN,                 /*!< unknown */
} rtk_bt_le_scan_stop_reason_t;

/**
 * @struct    rtk_bt_le_scan_stop_ind_t
 * @brief     Bluetooth LE scan stop indication msg.
 */
typedef struct {
	uint16_t err;                               /*!< Error code */
	rtk_bt_le_scan_stop_reason_t stop_reason;   /*!< scan stop reason */
} rtk_bt_le_scan_stop_ind_t;

/**
 * @struct    rtk_bt_le_conn_ind_t
 * @brief     Bluetooth LE connection complete event msg.
 */
typedef struct {
	uint16_t err;                               /*!< Error code */
	rtk_bt_le_link_role_t role;                 /*!< Local device's role in this link */
	uint16_t conn_handle;                       /*!< Connection handle */
	rtk_bt_le_addr_t peer_addr;                 /*!< Peer address */
	uint16_t conn_interval;                     /*!< Connection interval */
	uint16_t conn_latency;                      /*!< Connection latency */
	uint16_t supv_timeout;                      /*!< Connection supervision timeout */
} rtk_bt_le_conn_ind_t;

/**
 * @struct    rtk_bt_le_disconn_ind_t
 * @brief     Bluetooth LE disconnect complete event msg.
 */
typedef struct {
	uint16_t reason;                            /*!< Disconnect reason */
	rtk_bt_le_link_role_t role;                 /*!< Local device's role in this link */
	uint16_t conn_handle;                       /*!< Connection handle */
	rtk_bt_le_addr_t peer_addr;                 /*!< Peer address */
} rtk_bt_le_disconn_ind_t;

/**
 * @struct    rtk_bt_le_conn_update_ind_t
 * @brief     Bluetooth LE connection parameters updated event msg.
 */
typedef struct {
	uint16_t err;                           /*!< Error code */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t conn_interval;                 /*!< Connection interval */
	uint16_t conn_latency;                  /*!< Connection latency */
	uint16_t supv_timeout;                  /*!< Connection supervision timeout */
} rtk_bt_le_conn_update_ind_t;

/**
 * @struct    rtk_bt_le_remote_conn_update_req_ind_t
 * @brief     Bluetooth LE remote device connection parameters change request event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t conn_interval_min;             /*!< Mininum connection interval */
	uint16_t conn_interval_max;             /*!< Maximum connection interval */
	uint16_t conn_latency;                  /*!< Connection latency */
	uint16_t supv_timeout;                  /*!< Connection supervision timeout */
} rtk_bt_le_remote_conn_update_req_ind_t;

/**
 * @struct    rtk_bt_le_auth_pair_cfm_ind_t
 * @brief     Bluetooth LE just work pairing need to confirm event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
} rtk_bt_le_auth_pair_cfm_ind_t;

/**
 * @struct    rtk_bt_le_auth_key_display_ind_t
 * @brief     Bluetooth LE pairing key need to display event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	uint32_t passkey;                       /*!< Passkey value need to display */
} rtk_bt_le_auth_key_display_ind_t;

/**
 * @struct    rtk_bt_le_auth_key_input_ind_t
 * @brief     Bluetooth LE pairing key need to input event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
} rtk_bt_le_auth_key_input_ind_t;

/**
 * @struct    rtk_bt_le_auth_key_cfm_ind_t
 * @brief     Bluetooth LE pairing key need to compare and confirm event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	uint32_t passkey;                       /*!< Passkey value need to compare and confirm */
} rtk_bt_le_auth_key_cfm_ind_t;

/**
 * @struct    rtk_bt_le_auth_oob_input_ind_t
 * @brief     Bluetooth LE pairing oob key need to input event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
} rtk_bt_le_auth_oob_input_ind_t;

/**
 * @struct    rtk_bt_le_sec_level_t
 * @brief     Bluetooth LE security level.
 */
typedef enum {
	RTK_BT_LE_SEC_LEVEL_NONE          = 0x01,   /*!< No security (No authentication and no encryption). */
	RTK_BT_LE_SEC_LEVEL_UNAUTHEN      = 0x02,   /*!< Unauthenticated pairing with encryption. */
	RTK_BT_LE_SEC_LEVEL_AUTHEN        = 0x03,   /*!< Authenticated pairing with encryption. */
	RTK_BT_LE_SEC_LEVEL_SC_AUTHEN_128 = 0x04,   /*!< Authenticated LE Secure Connections pairing with encryption using a 128-bit strength encryption key. */
} rtk_bt_le_sec_level_t;

/**
 * @struct    rtk_bt_le_auth_complete_ind_t
 * @brief     Bluetooth LE auth compeleted event msg.
 */
typedef struct {
	uint16_t err;                           /*!< Error code */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint8_t dev_ltk_length;                 /*!< Device long term key length*/
	uint8_t dev_ltk[32];                    /*!< Device long term key */
	rtk_bt_le_sec_level_t sec_level;        /*!< Security level */
} rtk_bt_le_auth_complete_ind_t;

/**
 * @struct    rtk_bt_le_data_len_change_ind_t
 * @brief     Bluetooth LE data length changed event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t max_tx_octets;                 /*!< Maximum tx octets */
	uint16_t max_tx_time;                   /*!< Maximum tx time */
	uint16_t max_rx_octets;                 /*!< Maximum rx octets */
	uint16_t max_rx_time;                   /*!< Maximum rx time */
} rtk_bt_le_data_len_change_ind_t;

/**
 * @struct    rtk_bt_le_phy_update_ind_t
 * @brief     Bluetooth LE PHY updated event msg.
 */
typedef struct {
	uint16_t err;                           /*!< Error code */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint8_t tx_phy;                         /*!< TX PHY */
	uint8_t rx_phy;                         /*!< RX PHY */
} rtk_bt_le_phy_update_ind_t;

/**
 * @struct    rtk_bt_le_read_remote_version_ind_t
 * @brief     Bluetooth LE read remote version event msg.
 */
typedef struct {
	uint16_t err;                           /*!< Error code */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint8_t version;                        /*!< Version information of remote controller */
	uint16_t company_id;                    /*!< Manufacturer of the remote controller */
	uint16_t subversion;                    /*!< Subversion of remote controller, it's vendor-specific. */
} rtk_bt_le_read_remote_version_ind_t;

/**
 * @struct    rtk_bt_le_wl_modify_ind_t
 * @brief     Bluetooth LE whitelist modified event msg.
 */
typedef struct {
	rtk_bt_le_wl_op_t op;                   /*!< Whitelist modify operation */
	uint16_t err;                           /*!< Error code */
} rtk_bt_le_wl_modify_ind_t;

/**
 * @enum      rtk_bt_le_bond_op_t
 * @brief     Bluetooth LE bond operation definition.
 */
typedef enum {
	RTK_BT_LE_BOND_DELETE,          /*!< Delete bond device by address */
	RTK_BT_LE_BOND_ADD,             /*!< Add bond device to bond list */
	RTK_BT_LE_BOND_CLEAR,           /*!< Clear all bond device in bond list */
	RTK_BT_LE_BOND_FULL,
	RTK_BT_LE_BOND_KEY_MISSING,
	RTK_BT_LE_BOND_RESERVED,        /*!< Reserved */
} rtk_bt_le_bond_op_t;

/**
 * @struct    rtk_bt_le_bond_modify_ind_t
 * @brief     Bluetooth LE bond modified event msg.
 */
typedef struct {
	rtk_bt_le_bond_op_t op;             /*!< Bond modify operation */
	rtk_bt_le_addr_t remote_addr;       /*!< Bonded device address */
	rtk_bt_le_ident_addr_t ident_addr;  /*!< Identity address */
} rtk_bt_le_bond_modify_ind_t;

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
/**
 * @struct    rtk_bt_le_modify_resolv_list_ind_t
 * @brief     Bluetooth LE resolving list modified indication.
 */
typedef struct {
	rtk_bt_le_resolv_list_op_t op;              /*!< Resolving list modify operation */
	rtk_bt_le_resolv_list_entry_t entry;        /*!< resolving list entry */
	uint16_t err;                               /*!< Error code */
} rtk_bt_le_modify_resolv_list_ind_t;

/**
 * @enum      rtk_bt_le_resolv_list_pending_reason_t
 * @brief     Bluetooth LE resolving list modification pending reason definition.
 */
typedef enum {
	RTK_BT_LE_RESOLV_LIST_PEND_BY_ADV = 0x01,    /**< Resolving list modification is pended because device is advertising. */
	RTK_BT_LE_RESOLV_LIST_PEND_BY_SCAN = 0x02,   /**< Resolving list modification is pended because device is scanning. */
	RTK_BT_LE_RESOLV_LIST_PEND_BY_CONNECT = 0x04 /**< Resolving list modification is pended because device is connecting. */
} rtk_bt_le_resolv_list_pending_reason_t;

/**
 * @struct    rtk_bt_le_resolv_list_pending_ind_t
 * @brief     Bluetooth LE resolving list modification pending indication.
 */
typedef struct {
	uint8_t reason;                             /*!< Bits combination of @ref rtk_bt_le_resolv_list_pending_reason_t */
} rtk_bt_le_resolv_list_pending_ind_t;

/**
 * @enum      rtk_bt_le_privacy_mode_t
 * @brief     Bluetooth LE privacy mode type.
 */
typedef enum {
	RTK_BT_LE_NETWORK_PRIVACY_MODE = 0x00,
	RTK_BT_LE_DEVICE_PRIVACY_MODE = 0x01,
} rtk_bt_le_privacy_mode_t;
#endif

#if defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT
/**
 * @def       RTK_BT_LE_TXPOWER_LEVEL
 * @brief     Bluetooth LE tx power level.
 */
#define RTK_BLE_LE_TXPOWER_UNAVAILABLE      0x7F
#define RTK_BLE_LE_TXPOWER_NOT_MANAGING     0x7E

/**
 * @enum      rtk_bt_le_txpower_phy_t
 * @brief     Bluetooth LE phy definition in tx power report.
 */
typedef enum {
	RTK_BT_LE_TXPOWER_PHYS_1M          = 0x01, /*!< LE 1M PHY */
	RTK_BT_LE_TXPOWER_PHYS_2M          = 0x02, /*!< LE 2M PHY */
	RTK_BT_LE_TXPOWER_PHYS_CODED_S8    = 0x03, /*!< LE Coded PHY with S=8 data coding */
	RTK_BT_LE_TXPOWER_PHYS_CODED_S2    = 0x04, /*!< LE Coded PHY with S=2 data coding */
} rtk_bt_le_txpower_phy_t;

/**
 * @enum      rtk_bt_le_txpower_flag_t
 * @brief     Bluetooth LE TX power level flag Bits.
 */
typedef enum {
	RTK_BT_LE_TXPOWER_FLAG_MIN_BIT         = 0x01, /*!< Transmit power level is at minimum level. */
	RTK_BT_LE_TXPOWER_FLAG_MAX_BIT         = 0x02, /*!< Transmit power level is at maximum level. */
} rtk_bt_le_txpower_flag_t;

/**
 * @struct    rtk_bt_le_txpower_read_local_t
 * @brief     Bluetooth LE read remote TX power parameter definition.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	rtk_bt_le_txpower_phy_t phy;
	int8_t *cur_txpower;                    /*!< Current transmit power level Range: -127 to 20 Units: dBm
                                             *   Note: 0x7F meands current transmit power level is unavailable
                                             */
	int8_t *max_txpower;                    /*!< Maximum transmit power level Range: -127 to 20 Units: dBm */
} rtk_bt_le_txpower_read_local_t;

/**
 * @struct    rtk_bt_le_txpower_read_remote_t
 * @brief     Bluetooth LE read remote TX power parameter definition.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	rtk_bt_le_txpower_phy_t phy;
} rtk_bt_le_txpower_read_remote_t;

/**
 * @struct    rtk_bt_le_txpower_report_set_t
 * @brief     Bluetooth LE read TX power parameter indication.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	bool local_enable;                      /*!< Enable local transmit power level report. */
	bool remote_enable;                     /*!< Enable remote transmit power level report. */
} rtk_bt_le_txpower_report_set_t;

/**
 * @enum      rtk_bt_le_txpower_report_type_t
 * @brief     Bluetooth LE TX power report type definition.
 */
typedef enum {
	RTK_BT_LE_TXPOWER_REPORT_LOCAL_CHANGED         = 0x00, /*!< Local transmit power changed. */
	RTK_BT_LE_TXPOWER_REPORT_REMOTE_CHANGED        = 0x01, /*!< Remote transmit power changed. */
	RTK_BT_LE_TXPOWER_REPORT_REMOTE_READ_RESPONSE  = 0x02, /*!< Response of read remote transmit power. */
} rtk_bt_le_txpower_report_type_t;

/**
 * @struct    rtk_bt_le_txpower_ind_t
 * @brief     Bluetooth LE TX power report indication.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	rtk_bt_le_txpower_report_type_t type;
	rtk_bt_le_txpower_phy_t phy;
	int8_t txpower;                         /*!< Transmit power level Range: -127 to 20 Units: dBm
                                             *   Note: 0x7E means remote device is not managing power levels on this PHY. @RTK_BLE_LE_TXPOWER_UNAVAILABLE
                                             *         0x7F means transmit power level is not available. @RTK_BLE_LE_TXPOWER_NOT_MANAGING
                                             */
	uint8_t flag;                           /*!< Bits combination of @ref rtk_bt_le_txpower_flag_t */
	int8_t delta;                           /*!< Change in transmit power level Units: dB
                                             *   positive indicates increased power,
                                             *   negative indicates decreased power,
                                             *   zero indicates unchanged.
                                             */
} rtk_bt_le_txpower_ind_t;
#endif

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
typedef enum {
	RTK_BT_LE_COC_SEC_NONE,                 /**< Security None */
	RTK_BT_LE_COC_SEC_UNAUTHEN_ENCRYPT,     /**< Security unauthenticated encryption */
	RTK_BT_LE_COC_SEC_AUTHEN_ENCRYPT,       /**< Security authenticated encryption */
	RTK_BT_LE_COC_SEC_UNAUTHEN_DATA_SIGN,   /**< Security unauthenticated data signed */
	RTK_BT_LE_COC_SEC_AUTHEN_DATA_SIGN,     /**< Security authenticated data signed */
	RTK_BT_LE_COC_SEC_AUTHOR,               /**< Security authorized */
	RTK_BT_LE_COC_SEC_SECURE_CONN_UNAUTHEN, /**< Security unauthenticated LE secure connection */
	RTK_BT_LE_COC_SEC_SECURE_CONN_AUTHEN,   /**< Security LE secure connection */
} rtk_bt_le_coc_security_mode_t;

typedef enum {
	RTK_BT_LE_COC_PARAM_LOCAL_MTU          = 0x1,
} rtk_bt_le_coc_param_type_t;

typedef enum {
	RTK_BT_LE_COC_CHAN_PARAM_CUR_CREDITS   = 0x0,
	RTK_BT_LE_COC_CHAN_PARAM_MAX_CREDITS   = 0x1,
	RTK_BT_LE_COC_CHAN_PARAM_MTU           = 0x2,
} rtk_bt_le_coc_chan_param_type_t;

#ifdef CONFIG_PLATFORM_TIZENRT_OS
typedef struct {
	uint16_t le_psm;                          /*!< LE PSM value */
	uint16_t err;                             /*!< Error code of registraion of LE PSM value */
} rtk_bt_le_coc_reg_psm_ind_t;

typedef struct {
	uint16_t err;                             /*!< Error code of setting LE COC security */
} rtk_bt_le_coc_set_sec_ind_t;
#endif //#ifdef CONFIG_PLATFORM_TIZENRT_OS

typedef struct {
	uint16_t conn_handle;                     /*!< Connection handle */
	uint16_t cid;                             /*!< Local l2cap channel ID assgined by bt stack */
	uint16_t err;                             /*!< Error code of l2cap channel connect or disconnect */
} rtk_bt_le_coc_conn_state_ind_t;

typedef struct {
	uint16_t conn_handle;                     /*!< Connection handle */
	uint16_t cid;                             /*!< Local l2cap channel ID */
	uint16_t err;                             /*!< Error code of l2cap channel send data */
	uint8_t credit;                           /*!< Remain credits number after send data */
} rtk_bt_le_coc_send_data_res_ind_t;

typedef struct {
	uint16_t conn_handle;                     /*!< Connection handle */
	uint16_t cid;                             /*!< Local l2cap channel ID */
	uint16_t len;                             /*!< Length of received data */
	uint8_t *data;                            /*!< Received data from remote */
} rtk_bt_le_coc_receive_data_ind_t;
#endif /* RTK_BLE_COC_SUPPORT */

/****************** Data structure for API func param pack and unpack ******************/
typedef struct {
	rtk_bt_le_rand_addr_type_t type;
	uint8_t *p_addr;
} rtk_bt_le_gen_rand_addr_t;

typedef struct {
	uint16_t conn_handle;
	int8_t *p_rssi;
} rtk_bt_le_read_rssi_param_t;

typedef struct {
	uint16_t conn_handle;
	uint8_t reason;
} rtk_bt_le_disconn_with_reason_param_t;

typedef struct {
	uint16_t conn_handle;
	rtk_bt_le_conn_info_t *p_conn_info;
} rtk_bt_le_get_conn_info_param_t;

typedef struct {
	rtk_bt_le_addr_t addr;
	uint16_t *p_conn_handle;
} rtk_bt_le_get_conn_handle_by_addr_param_t;

typedef struct {
	uint16_t conn_handle;
	uint16_t *p_mtu_size;
} rtk_bt_le_get_mtu_size_param_t;

typedef struct {
	rtk_bt_le_bond_info_t *p_bond_info;
	uint8_t *p_size;
	uint8_t size;
} rtk_bt_le_get_bond_info_param_t;

typedef struct {
	uint16_t conn_handle;
	uint16_t *p_tx_pending_num;
} rtk_bt_le_get_tx_pending_num_param_t;

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
typedef struct {
	uint16_t conn_handle;
	uint8_t *adv_handle;
} rtk_bt_le_get_eadv_by_conn_handle_param_t;
#endif

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
typedef struct {
	rtk_bt_le_ident_addr_type_t peer_ident_addr_type;
	uint8_t *peer_addr;
	uint8_t *local_rpa;
} rtk_bt_le_read_local_rpa_param_t;

typedef struct {
	rtk_bt_le_ident_addr_type_t peer_ident_addr_type;
	uint8_t *peer_addr;
	uint8_t *peer_rpa;
} rtk_bt_le_read_peer_rpa_param_t;

typedef struct {
	rtk_bt_le_ident_addr_type_t peer_ident_addr_type;
	uint8_t *peer_addr;
	rtk_bt_le_privacy_mode_t privacy_mode;
} rtk_bt_le_set_privacy_mode_param_t;
#endif

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
typedef struct {
	uint8_t is_register;
	uint16_t le_psm;
} rtk_bt_le_coc_psm_reg_param_t;

typedef struct {
	uint16_t le_psm;
	uint8_t active;
	rtk_bt_le_coc_security_mode_t sec_mode;
	uint8_t key_size;
} rtk_bt_le_coc_set_psm_sec_param_t;

typedef struct {
	rtk_bt_le_coc_param_type_t param_type;
	uint16_t value;
} rtk_bt_le_coc_param_set_t;

typedef struct {
	rtk_bt_le_coc_chan_param_type_t param_type;
	uint16_t cid;
	uint16_t *value;
} rtk_bt_le_coc_chan_param_get_t;

typedef struct {
	uint16_t conn_handle;
	uint16_t le_psm;
} rtk_bt_le_coc_connect_param_t;

typedef struct {
	uint16_t cid;
	uint16_t len;
	uint8_t *data;
} rtk_bt_le_coc_send_data_param_t;
#endif /* RTK_BLE_COC_SUPPORT */
/***************************************************************************************/

typedef enum {
	/* Convenience value for purposes where non of CTE types is allowed. */
	RTK_BT_LE_GAP_CTE_TYPE_NONE = 0,
	/* AoA Constant Tone Extension */
	RTK_BT_LE_GAP_CTE_TYPE_AOA = 0x01,
	/* AoD Constant Tone Extension with 1us slots */
	RTK_BT_LE_GAP_CTE_TYPE_AOD_1US = 0x02,
	/* AoD Constant Tone Extension with 2us slots */
	RTK_BT_LE_GAP_CTE_TYPE_AOD_2US = 0x04,
} rtk_bt_le_gap_cte_type_t;


#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT

/* BLE connectionless CTE transmit parameters */
#define RTK_BLE_GAP_CTE_LEN_MIN                         (0x02u)
#define RTK_BLE_GAP_CTE_LEN_MAX                         (0x14u)
#define RTK_BLE_GAP_CTE_LEN_VALUE_IN_RANGE(value)       \
    (((value) >= RTK_BLE_GAP_CTE_LEN_MIN) && ((value) <= RTK_BLE_GAP_CTE_LEN_MAX))

#define RTK_BLE_GAP_CTE_TYPE_VALUE_IN_RANGE(value)      \
    ((value) == RTK_BT_LE_GAP_CTE_TYPE_AOA ||           \
     (value) == RTK_BT_LE_GAP_CTE_TYPE_AOD_1US ||       \
     (value) == RTK_BT_LE_GAP_CTE_TYPE_AOD_2US)

#define RTK_BLE_GAP_CTE_CNT_MIN                         (0x01u)
#define RTK_BLE_GAP_CTE_CNT_MAX                         (0x10u)
#define RTK_BLE_GAP_CTE_CNT_VALUE_IN_RANGE(value)       \
    (((value) >= RTK_BLE_GAP_CTE_CNT_MIN) && ((value) <= RTK_BLE_GAP_CTE_CNT_MAX))

#define RTK_BLE_GAP_CTE_NUM_ANT_IDS_MIN                 (0x02u)
#define RTK_BLE_GAP_CTE_NUM_ANT_IDS_MAX                 (0x0Cu)
#define RTK_BLE_GAP_CTE_NUM_ANT_IDS_VALUE_IN_RANGE(value)   \
    (((value) >= RTK_BLE_GAP_CTE_NUM_ANT_IDS_MIN) && ((value) <= RTK_BLE_GAP_CTE_NUM_ANT_IDS_MAX))

#define RTK_BLE_GAP_CTE_TX_ENABLE_VALUE_IN_RANGE(value) ((value) <= 0x01u)

/* BLE connectionless IQ sampling parameter */
#define RTK_BLE_GAP_CTE_RX_EANBLE_VALUE_IN_RANGE(value) ((value) <= 0x01u)

#define RTK_BLE_GAP_CTE_SLOT_DURATION_MIN               (0x01u)
#define RTK_BLE_GAP_CTE_SLOT_DURATION_MAX               (0x02u)
#define RTK_BLE_GAP_CTE_SLOT_DURATION_VALUE_IN_RANGE(value)     \
    (((value) >= RTK_BLE_GAP_CTE_SLOT_DURATION_MIN) && ((value) <= RTK_BLE_GAP_CTE_SLOT_DURATION_MAX))

#define RTK_BLE_GAP_CTE_MAX_SAMPLED_CTES_MIN            (0x00u)
#define RTK_BLE_GAP_CTE_MAX_SAMPLED_CTES_MAX            (0x10u)
#define RTK_BLE_GAP_CTE_MAX_SAMPLED_CTES_VALUE_IN_RANGE(value)      \
    ((value) <= RTK_BLE_GAP_CTE_MAX_SAMPLED_CTES_MAX)

#define RTK_BLE_GAP_CTE_TYPES_VALUE_IN_RANGE(value)         \
    ((value) <= (RTK_BT_LE_GAP_CTE_TYPE_AOA | RTK_BT_LE_GAP_CTE_TYPE_AOD_1US | RTK_BT_LE_GAP_CTE_TYPE_AOD_2US))

#define RTK_BLE_GAP_CTE_REQ_INTERVAL_MIN                (0x0000u)
#define RTK_BLE_GAP_CTE_REQ_INTERVAL_MAX                (0xFFFFu)
#define RTK_BLE_GAP_CTE_REQ_INTERVAL_VALUE_IN_RANGE(value)  \
    (((value) >= RTK_BLE_GAP_CTE_REQ_INTERVAL_MIN) && ((value) <= RTK_BLE_GAP_CTE_REQ_INTERVAL_MAX))

typedef struct {
	/* CTE duration, in units of 10ms*/
	uint16_t duration;

	/* Constant Tone Extension length in 8us units
	 * Range: 0x02 to 0x14
	 */
	uint8_t  cte_len;

	/* Constant Tone Extension type @ref rtk_bt_le_gap_cte_type_t */
	uint8_t  cte_type;

	/* The number of Constant Tone Extensions to transmit in each periodic advertising interval
	 * Range:  0x01 to 0x10
	 */
	uint8_t  cte_count;

	/* The number of Antenna IDs in the pattern
	 * Range: for AoD: 0x02 to RTK_BLE_GAP_CTE_NUM_ANT_IDS_MAX
	 *        for AoA: ignore the parameter and it's default value is 0
	 */
	uint8_t  num_ant_ids;

	/* Antenna ID in the pattern
	 * For AoA, ignore the parameter and it's default value is NULL
	 */
	uint8_t  *ant_ids;
} rtk_bt_le_gap_connless_cte_tx_param_t;

typedef struct {
	/* Advertising handle */
	uint8_t adv_handle;

	/* Connectionless CTE transmit parameters */
	rtk_bt_le_gap_connless_cte_tx_param_t   *param;
} rtk_bt_le_gap_connless_cte_tx_start_t;

typedef enum {
	/* Switching and sampling slots are 1us each */
	RTK_BT_LE_GAP_SLOT_DURATIONS_1US = 1,
	/* Switching and sampling slots are 2us each */
	RTK_BT_LE_GAP_SLOT_DURATIONS_2US = 2,
} rtk_bt_le_gap_slot_durations_e;

typedef struct {
	/* Switching and sampling slots @ref rtk_bt_le_gap_slot_durations_e */
	uint8_t  slot_durations;

	/* The maximum number of Constant Tone Extensions to sample and report in each periodic advertising interval
	 * Range:
	 *      0x00: Sample and report all available Constant Tone Extensions
	 *      0x01 to 0x10 : the maximum number of Constant Tone Extensions to sample and report
	 */
	uint8_t  max_sampled_ctes;

	/* The number of Antenna IDs in the switching pattern
	 * Range: for AoA: 0x02 to RTK_BLE_GAP_CTE_NUM_ANT_IDS_MAX
	 *        for AoD: ignore the parameter and it's default value is 0
	 */
	uint8_t  num_ant_ids;

	/* Antenna IDs in the switching pattern
	 * For AoD, ignore the parameter and it's default value is NULL
	 */
	uint8_t  *ant_ids;
} rtk_bt_le_gap_connless_cte_rx_param_t;

typedef struct {
	/* Periodic advertising sync id */
	uint8_t sync_id;

	/* Connectionless CTE receive parameters */
	rtk_bt_le_gap_connless_cte_rx_param_t *param;
} rtk_bt_le_gap_connless_cte_rx_start_t;

typedef struct {
	/* Constant Tone Extension types: bit mask @ref rtk_bt_le_gap_cte_type_t */
	uint8_t  cte_types;

	/* The number of Antenna IDs in the pattern
	 * Range: for AoD: 0x02 to RTK_BLE_GAP_CTE_NUM_ANT_IDS_MAX
	 *        for AoA: ignore the parameter and it's default value is 0
	 */
	uint8_t  num_ant_ids;

	/* Antenna IDs in the pattern
	 * For AoA, ignore the parameter and it's default value is NULL
	 */
	uint8_t  *ant_ids;
} rtk_bt_le_gap_conn_cte_tx_param_t;

typedef struct {
	/* connection handle */
	uint16_t conn_handle;

	/* Connection CTE transmit parameter */
	rtk_bt_le_gap_conn_cte_tx_param_t *param;
} rtk_bt_le_gap_conn_cte_tx_start_t;

typedef struct {
	/* CTE request interval, indicating whether CTE Request procedure is initiated only once or periodically.
	 * Range:
	 *      0x0000: Initiate the CTE Request procedure once, at the earliest practical opportunity
	 *      0x0001 to 0xFFFF: Requested interval for initiating the CTE Request procedure in number of underlying connection events.
	 */
	uint16_t cte_req_interval;

	/* Minimum length of the Constant Tone Extension being requested in 8μs units
	 * Range:  0x02 to 0x14
	 */
	uint8_t  req_cte_len;

	/* Requested CTE type @ref rtk_bt_le_gap_cte_type_t */
	rtk_bt_le_gap_cte_type_t  req_cte_type;

	/* Switching and sampling slots @ref rtk_bt_le_gap_slot_durations_e */
	uint8_t  slot_durations;

	/* The number of Antenna IDs in the pattern
	 * Range: for AoA: 0x02 to RTK_BLE_GAP_CTE_NUM_ANT_IDS_MAX
	 *        for AoD: ignore the parameter and it's default value is 0
	 */
	uint8_t  num_ant_ids;

	/* Antenna IDs in the pattern
	 * For AoD, ignore the parameter and it's default value is NULL
	 */
	uint8_t  *ant_ids;
} rtk_bt_le_gap_conn_cte_rx_param_t;

typedef struct {
	/* Connection handle */
	uint16_t conn_handle;
	/* enable=1 to start connection cte rx; enable=0 to stop connection cte rx */
	bool enable;
	/* Connection CTE receive parameters */
	rtk_bt_le_gap_conn_cte_rx_param_t  *p_rx_param;
} rtk_bt_le_gap_conn_cte_rx_t;

typedef enum {
	RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_IDLE = 0,
	RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_EXT_ADV_IDLE,
	RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_EXT_ADV,
	RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_WAIT_PA_ADV,
	RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_ENABLING,
	RTK_BT_LE_GAP_CONNLESS_CTE_TX_STATE_TRANSMITTING,
} rtk_bt_le_gap_connless_cte_tx_state_e;

typedef struct  {
	/* Extended advertising handle */
	uint8_t  adv_handle;
	uint16_t cause;
	/* Connectionless CTE transmit state */
	rtk_bt_le_gap_connless_cte_tx_state_e state;
} rtk_bt_le_gap_connless_cte_tx_ind_t;

typedef enum {
	/* CRC was correct. */
	RTK_BT_LE_GAP_CTE_PACKET_CRC_CORRECT = 0x00,
	/* CRC was incorrect, and the Length and CTETime fields  of the packet used to determine sampling points. */
	RTK_BT_LE_GAP_CTE_PACKET_CRC_INCORRECT_DETERMINE = 0x01,
	/* CRC was incorrect but the Controller has determined the position and length of the Constant Tone Extension in some other way. */
	RTK_BT_LE_GAP_CTE_PACKET_CRC_INCORRECT_DETERMINE_OTHERWAY = 0x02,
	/* Insufficient resources to sample (Data_Channel_Index, CTE_Type, and Slot_Durations invalid). */
	RTK_BT_LE_GAP_CTE_PACKET_INSUFFI_RESOURCES = 0xFF,
} rtk_bt_le_gap_cte_packet_status_type_e;

/**
 * @def    RTK_BT_LE_MAX_ADV_DATA_LEN
 * @brief     Bluetooth LE max adv data len.
 */
#define RTK_BT_LE_MAX_IQ_REPORT_CNT  (82)

typedef struct {
	/* Identify the periodic advertising train. */
	uint8_t                                 sync_id;
	/* Sync_Handle identifying the periodic advertising train. */
	uint16_t                                sync_handle;
	/* The index of the channel on which the packet was received.
	 * Range: 0x00 to 0x27. Note: 0x25 to 0x27 can be used only for packets generated during test modes.
	 */
	uint8_t                                 channel_index;
	/* RSSI of the packet.
	 * Range: -1270 to +200, Units: 0.1 dBm.
	 */
	int16_t                                 rssi;
	/* Antenna ID. */
	uint8_t                                 rssi_antenna_id;
	/* CTE type */
	rtk_bt_le_gap_cte_type_t                cte_type;
	/* Specify the sampling rate used by the Controller.*/
	rtk_bt_le_gap_slot_durations_e          slot_durations;
	/* CTE packet status */
	rtk_bt_le_gap_cte_packet_status_type_e  packet_status;
	/* The value of paEventCounter for the reported AUX_SYNC_IND PDU. */
	uint16_t                                periodic_event_counter;
	/* Sample count value:
	 *      0x00: No samples provided (only permitted if Packet_Status is 0xFF)
	 *      0x09 to 0x52: Total number of sample pairs (there shall be the same number of I samples and Q samples).
	 */
	uint8_t                                 sample_count;
	/* Sample length is sample_cout * 2. I_Sample[0],Q_Sample[0], ... , I_Sample[i],Q_Sample[i]
	 * Value 0x80 indicates No valid sample available. */
	int8_t                                  iq_sample[RTK_BT_LE_MAX_IQ_REPORT_CNT * 2];
} rtk_bt_le_gap_connless_iq_report_ind_t;

typedef struct {
	uint16_t                                conn_handle;
	/* RX PHY value:
	 *   0x01: The receiver PHY for the connection is LE 1M.
	 *   0x02: The receiver PHY for the connection is LE 2M.
	 */
	uint8_t                                 rx_phy;
	/* The index of the data channel on which the Data Physical Channel PDU was received.
	 * Range: 0x00 to 0x24.
	 */
	uint8_t                                 data_chan_index;
	/* RSSI of the packet.
	 * Range: -1270 to +200, Units: 0.1 dBm.
	 */
	int16_t                                 rssi;
	/* ID of the antenna on which the RSSI is measured. */
	uint8_t                                 rssi_antenna_id;
	/* CTE type */
	rtk_bt_le_gap_cte_type_t                cte_type;
	/* Switching and sampling slot durations */
	rtk_bt_le_gap_slot_durations_e          slot_durations;
	/* CTE packet status */
	rtk_bt_le_gap_cte_packet_status_type_e  packet_status;
	/* The value of connEventCounter for the reported PDU. */
	uint16_t                                connection_event_counter;
	/* Sample count value:
	 *  0x00: No samples provided (only permitted if Packet_Status is 0xFF)
	 *  0x09 to 0x52: Total number of sample pairs (there shall be the same number of I samples and Q samples).
	 */
	uint8_t                                 sample_count;
	/* Length is sample_cout * 2. I_Sample[0],Q_Sample[0], ... , I_Sample[i],Q_Sample[i]
	 * Value 0x80 indicates No valid sample available.
	 */
	int8_t                                  iq_sample[RTK_BT_LE_MAX_IQ_REPORT_CNT * 2];
} rtk_bt_le_gap_conn_iq_report_ind_t;

typedef struct {
	/* Connection handle */
	uint16_t     conn_handle;
	/* CTE request fail cause */
	uint16_t    cause;
} rtk_bt_le_gap_cte_req_fail_ind_t;

typedef struct {
	/* A bit field that indicates the type of switching sampling rates supported by the Controller.*/
	uint8_t supported_switching_sampling_rates;
	/* The number of antennae supported by the Controller. */
	uint8_t num_antennae;
	/* Maximum length of antenna switching pattern supported by the Controller. */
	uint8_t max_switching_pattern_length;
	/* Maximum length of a transmitted Constant Tone Extension supported in 8us units. */
	uint8_t max_cte_length;
} rtk_bt_le_gap_antenna_info_t;

#endif /* RTK_BLE_5_1_CTE_SUPPORT */


#if defined(RTK_BLE_5_4_PA_RSP_SUPPORT) && RTK_BLE_5_4_PA_RSP_SUPPORT
typedef struct {
	uint16_t adv_handle;            /*!< Advertising handle */
	uint8_t subevent;               /*!< The subevent to set data for */
	uint8_t response_slot_start;    /*!< The first response slot to listen to */
	uint8_t response_slot_count;    /*!< The number of response slots to listen to */
	uint16_t len;                   /*!< The length of data to send */
	uint8_t *data;                  /*!< The data to send */
} rtk_bt_le_gap_pawr_subevent_data_t;
#endif /* RTK_BLE_5_4_PA_RSP_SUPPORT */

#if defined(RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT) && RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT
typedef struct {
	uint8_t sync_id;                /*!< Periodic advertising sync id */
	uint16_t request_event;         /*!< The periodic event counter of the request the response is sent to */
	uint8_t request_subevent;       /*!< The subevent counter of the request the response is sent to */
	uint8_t response_subevent;      /*!< The subevent the response shall be sent in */
	uint8_t response_slot;          /*!< The response slot the response shall be sent in */
	uint16_t len;                   /*!< The length of response to send */
	uint8_t *rsp;                   /*!< The response to send */
} rtk_bt_le_gap_pawr_set_response_t;

typedef struct {
	uint8_t sync_id;                /*!< Periodic advertising sync id */
	uint16_t properties;            /*!< Periodic Advertising Properties */
	uint8_t num_subevents;          /*!< Number of subevents to sync to */
	uint8_t *subevents;             /*!< The subevent(s) to synchronize with. The array must have @ref num_subevents elements */
} rtk_bt_le_gap_pawr_sync_subevent_t;
#endif /* RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT */

/**
 * @typedef   rtk_bt_le_dtm_rx_version_t
 * @brief     Receiver test version.
 */
typedef enum {
	RTK_BT_LE_DTM_RX_VERSION_V1 = 0x01,    /**< Version 1. */
	RTK_BT_LE_DTM_RX_VERSION_V2 = 0x02,    /**< Version 2. */
	RTK_BT_LE_DTM_RX_VERSION_V3 = 0x03,    /**< Version 3. */
} rtk_bt_le_dtm_rx_version_t;

/**
 * @typedef   rtk_bt_le_dtm_tx_version_t
 * @brief     Transmitter test version.
 */
typedef enum {
	RTK_BT_LE_DTM_TX_VERSION_V1 = 0x01,    /**< Version 1. */
	RTK_BT_LE_DTM_TX_VERSION_V2 = 0x02,    /**< Version 2. */
	RTK_BT_LE_DTM_TX_VERSION_V3 = 0x03,    /**< Version 3. */
	RTK_BT_LE_DTM_TX_VERSION_V4 = 0x04,    /**< Version 4. */
} rtk_bt_le_dtm_tx_version_t;

/**
 * @typedef   rtk_bt_le_dtm_phy_rx_t
 * @brief     Bluetooth LE PHY type for receiver.
 */
typedef enum {
	RTK_BT_LE_DTM_RX_PHYS_1M       = 0x01,    /**< LE PHY 1M used. */
	RTK_BT_LE_DTM_RX_PHYS_2M       = 0x02,    /**< LE PHY 2M used. */
	RTK_BT_LE_DTM_RX_PHYS_CODED    = 0x03,    /**< LE Coded PHY used. */
} rtk_bt_le_dtm_phy_rx_t;

/**
 * @typedef   rtk_bt_le_dtm_phy_tx_t
 * @brief     Bluetooth LE PHY type for transmitter.
 */
typedef enum {
	RTK_BT_LE_DTM_TX_PHYS_1M          = 0x01,    /**< LE PHY 1M used. */
	RTK_BT_LE_DTM_TX_PHYS_2M          = 0x02,    /**< LE PHY 2M used. */
	RTK_BT_LE_DTM_TX_PHYS_CODED_S8    = 0x03,    /**< LE Coded PHY with S=8 data coding used. */
	RTK_BT_LE_DTM_TX_PHYS_CODED_S2    = 0x04,    /**< LE Coded PHY with S=2 data coding used. */
} rtk_bt_le_dtm_phy_tx_t;

/**
 * @typedef   rtk_bt_le_dtm_mod_idx_t
 * @brief     Assume transmitter will have a certain type of modulation index.
 */
typedef enum {
	RTK_BT_LE_DTM_MODULATION_INDEX_STANDARD   = 0x00,    /**< Standard modulation index. */
	RTK_BT_LE_DTM_MODULATION_INDEX_STABLE     = 0x01,    /**< Stable modulation index. */
} rtk_bt_le_dtm_mod_idx_t;

/**
 * @typedef   rtk_bt_le_dtm_cte_type_t
 * @brief     Constant Tone Extension type.
 */
typedef enum {
	RTK_BT_LE_DTM_CTE_TYPE_AOA                = 0x00,    /**< AoA Constant Tone Extension. */
	RTK_BT_LE_DTM_CTE_TYPE_AOD_1US_SLOT       = 0x01,    /**< AoD Constant Tone Extension with 1 us slots. */
	RTK_BT_LE_DTM_CTE_TYPE_AOD_2US_SLOT       = 0x02,    /**< AoD Constant Tone Extension with 2 us slots. */
} rtk_bt_le_dtm_cte_type_t;

/**
 * @typedef   rtk_bt_le_dtm_slot_durations_t
 * @brief     Switching and sampling slot duration type.
 */
typedef enum {
	RTK_BT_LE_DTM_SLOT_DURATIONS_SWITCH_SAMPLE_1US    = 0x01,    /**< Switching and sampling slots are 1 us each. */
	RTK_BT_LE_DTM_SLOT_DURATIONS_SWITCH_SAMPLE_2US    = 0x02,    /**< Switching and sampling slots are 2 us each. */
} rtk_bt_le_dtm_slot_durations_t;

/**
 * @typedef   rtk_bt_le_dtm_packet_payload_t
 * @brief     Transmitted packet payload type.
 */
typedef enum {
	RTK_BT_LE_DTM_PACKET_PAYLOAD_PRBS9    = 0x00,    /**< PRBS9 sequence '11111111100000111101' (in transmission order). */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_1100     = 0x01,    /**< Repeated '11110000' (in transmission order) sequence. */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_10       = 0x02,    /**< Repeated '10101010' (in transmission order) sequence. */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_PRBS15   = 0x03,    /**< PRBS15 sequence. */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_ALL1     = 0x04,    /**< Repeated '11111111' (in transmission order) sequence. */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_ALL0     = 0x05,    /**< Repeated '00000000' (in transmission order) sequence. */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_0011     = 0x06,    /**< Repeated '00001111' (in transmission order) sequence. */
	RTK_BT_LE_DTM_PACKET_PAYLOAD_01       = 0x07,    /**< Repeated '01010101' (in transmission order) sequence. */
} rtk_bt_le_dtm_packet_payload_t;

/**
 * @struct    rtk_bt_le_dtm_rx_param_t
 * @brief     Bluetooth LE receiver test parameters.
 */

typedef struct {
	/**
	 * Channel to receive packets.
	 * Range: 0x00 - 0x27.
	 * Frequency Range: 2402 - 2480 MHz.
	 */
	uint8_t rx_channel;
	/** PHY used by the receiver: @ref rtk_bt_le_dtm_phy_rx_t. */
	rtk_bt_le_dtm_phy_rx_t phy;
	/** Modulation index to receive packets: @ref rtk_bt_le_dtm_mod_idx_t. */
	rtk_bt_le_dtm_mod_idx_t mod_idx;
	/**
	 * Expected length of the Constant Tone Extensions in received test reference packets.
	 * Range: 0x00 (No Constant Tone Extension expected (default)) or 0x02 to 0x14.
	 * Units: 8 us.
	 */
	uint8_t exp_cte_len;
	/**
	 * Expected type of the Constant Tone Extensions in received test reference
	 * packets: @ref rtk_bt_le_dtm_cte_type_t.
	 */
	rtk_bt_le_dtm_cte_type_t exp_cte_type;
	/**
	 * Slot durations to receive packets and shall be ignored when exp_cte_type is not set
	 * to RTK_BT_LE_DTM_CTE_TYPE_AOA: @ref rtk_bt_le_dtm_slot_durations_t.
	 */
	rtk_bt_le_dtm_slot_durations_t slot_durations;
	/**
	 * The number of Antenna IDs in the pattern and shall be ignored when exp_cte_type
	 * is not set to RTK_BT_LE_DTM_CTE_TYPE_AOA.
	 * Range: 0x02 to max_switching_pattern_length supported by controller,
	 * max_switching_pattern_length shall be less than or equal to 0x4B.
	 */
	uint8_t sw_pattern_len;
	/**
	 * Antenna ID in the pattern and shall be ignored when exp_cte_type is not set
	 * to RTK_BT_LE_DTM_CTE_TYPE_AOA.
	 */
	uint8_t *p_antenna_ids;
} rtk_bt_le_dtm_rx_param_t;

/**
 * @struct    rtk_bt_le_dtm_tx_param_t
 * @brief     Bluetooth LE transmitter test paramters.
 */

typedef struct  {
	/**
	 * Channel to transmit packets.
	 * Range: 0x00 - 0x27.
	 * Frequency Range: 2402 - 2480 MHz.
	 */
	uint8_t tx_channel;
	/** Length in bytes of payload data in each packet. */
	uint8_t data_len;
	/** Contents of the payload of the test reference packets: @ref rtk_bt_le_dtm_packet_payload_t. */
	rtk_bt_le_dtm_packet_payload_t pkt_pl;
	/** PHY used by the transmitter: @ref rtk_bt_le_dtm_phy_tx_t. */
	rtk_bt_le_dtm_phy_tx_t phy;
	/**
	 * Length of the Constant Tone Extensions in transmitted test reference packets.
	 * Range: 0x00 (No Constant Tone Extension expected (default)) or 0x02 to 0x14.
	 * Units: 8 us.
	 */
	uint8_t cte_len;
	/**
	 * Type of the Constant Tone Extensions in transmitted test reference
	 * packets: @ref rtk_bt_le_dtm_cte_type_t.
	 */
	rtk_bt_le_dtm_cte_type_t cte_type;
	/**
	 * The number of Antenna IDs in the pattern and shall be ignored when cte_type
	 * is set to RTK_BT_LE_DTM_CTE_TYPE_AOA.
	 * Range: 0x02 to max_switching_pattern_length supported by controller,
	 * max_switching_pattern_length shall be less than or equal to 0x4B.
	 */
	uint8_t sw_pattern_len;
	/**
	 * Antenna ID in the pattern and shall be ignored when cte_type is set
	 * to RTK_BT_LE_DTM_CTE_TYPE_AOA.
	 */
	uint8_t *p_antenna_ids;
	/**
	 * Set transmitter to the specified or the nearest transmit power level.
	 * Range: 0xXX Set transmitter to the specified or the nearest transmit power level from -127 to +20.
	 *             Units: dBm.
	 *        0x7E Set transmitter to minimum transmit power level.
	 *        0x7F Set transmitter to maximum transmit power level.
	 */
	int8_t tx_power_level;
} rtk_bt_le_dtm_tx_param_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_le_gap BT LE GAP APIs
 * @brief     BT LE GAP related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief     Get connection id by connection handle.
 * @param[in] conn_handle: Connection handle.
 * @param[out] p_conn_id: Connection id for BLE link. Range 0 ~ RTK_BLE_GAP_MAX_LINKS - 1.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_conn_id(uint16_t conn_handle, uint8_t *p_conn_id);

/**
 * @brief     Get local stack version information.
 * @param[out] version: version information
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_version(rtk_bt_le_version_info_t *version);

/**
 * @brief     Get local device's address.
 * @param[out] paddr: Device address under use
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_bd_addr(rtk_bt_le_addr_t *paddr);

/**
 * @brief     Set bluetooth Device name.
 * @param[in] name: Device name
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_device_name(const uint8_t *name);

/**
 * @brief     Set device appearance.
 * @param[in] appearance: Device apperance
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_appearance(uint16_t appearance);

/**
 * @brief     Set preferred connetion parameters.
 * @param[in] p_preferred_conn_param: preferred connetion parameters, @ref rtk_bt_le_preferred_conn_param_t
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_preferred_conn_param(rtk_bt_le_preferred_conn_param_t *p_preferred_conn_param);

/**
 * @brief     Set random address to controller. If the address is static random address, also set it as local identity address.
 *            NOTE: This API shall not be excuted when advertising, scanning and initiating are enabled.
 * @param[in] auto_generate: Trigger auto generate address.
 * @param[in] type: Type of auto generated random address, ignore if auto_generate is false.
 * @param[in] p_addr: Random address, if auto_generate is false, this random address will be set to controller.
 * @param[out] p_addr: Random address, if auto_generate is true, the auto generated random address will be returned here.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_rand_addr(bool auto_generate, rtk_bt_le_rand_addr_type_t type, uint8_t *p_addr);

/**
 * @brief     Generate random address.
 * @param[in] type: Type of random address to generate.
 * @param[out] p_addr: Random address generated.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_gen_rand_addr(rtk_bt_le_rand_addr_type_t type, uint8_t *p_addr);

/**
 * @brief     Configure local identity address.
 *            NOTE: Only when you want to use a static random address as the device's address,
 *            you'd better set it as local identity address which will be distributed during
 *            BLE pairing process. For other type of address, we don't suggest to set it.
 *            If not set, the default local identity address is your device's public address.
 * @param[in] type: Type of identity address.
 * @param[in] p_addr: Identity address value.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_cfg_local_ident_addr(rtk_bt_le_ident_addr_type_t type, uint8_t *p_addr);

/**
 * @brief     Set BLE GAP advertising data.
 * @param[in] padv_data: Advertising data
 * @param[in] adv_len: Length of padv_data
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_adv_data(uint8_t *padv_data, uint32_t adv_len);

/**
 * @brief     Set BLE GAP scan response data.
 * @param[in] pscan_rsp_data: Scan response data
 * @param[in] scan_rsp_len: Length of pscan_rsp_data
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_scan_rsp_data(uint8_t *pscan_rsp_data, uint32_t scan_rsp_len);

/**
 * @brief     Start BLE GAP advertising, will cause event @ref RTK_BT_LE_GAP_EVT_ADV_START_IND
 * @param[in] padv_param: Advertising parameters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_adv(rtk_bt_le_adv_param_t *padv_param);

/**
 * @brief     Stop BLE GAP advertising, will cause event @ref RTK_BT_LE_GAP_EVT_ADV_STOP_IND
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_adv(void);

/**
 * @brief     Get BLE advertising parameter
 * @param[out] padv_param: Advertising parameters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_adv_param(rtk_bt_le_adv_param_t *padv_param);

/**
 * @brief     Check if BLE GAP advertise is idle
 * @return
 *            - true  : Adv state is idle
 *            - false : Adv state is advertising
 */
bool rtk_bt_le_gap_adv_is_idle(void);

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
/**
 * @brief     Create an extended advertising set
 * @param[in] p_adv_param: Advertising parameters
 * @param[in,out] p_adv_handle: Pointer to advertising set handle
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_create_ext_adv(rtk_bt_le_ext_adv_param_t *p_adv_param, uint8_t *p_adv_handle);

/**
 * @brief     Set BLE GAP extended advertising data.
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @param[in] padv_data: Extended advertising data.
 *                       NOTE: This function just saves the pointer of the advertising data and will not copy the advertising data,
 *                       Actually, the data will be set to controller when @ref rtk_bt_le_gap_start_ext_adv is called.
 *                       So this pointer should remain valid and the data in it shall not be changed until @ref rtk_bt_le_gap_start_ext_adv executed.
 * @param[in] adv_len: Length of padv_data.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_ext_adv_data(uint8_t adv_handle, uint8_t *padv_data, uint16_t adv_len);

/**
 * @brief     Set BLE GAP extended scan response data.
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @param[in] pscan_rsp_data: Extended scan response data.
 *                       NOTE: This function just saves the pointer of the advertising data and will not copy the advertising data,
 *                       Actually, the data will be set to controller when @ref rtk_bt_le_gap_start_ext_adv is called.
 *                       So this pointer should remain valid and the data in it shall not be changed until @ref rtk_bt_le_gap_start_ext_adv executed.
 * @param[in] scan_rsp_len: Length of pscan_rsp_data.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_ext_scan_rsp_data(uint8_t adv_handle, uint8_t *pscan_rsp_data, uint16_t scan_rsp_len);

/**
 * @brief     Start BLE GAP extended advertising, will cause event @ref RTK_BT_LE_GAP_EVT_EXT_ADV_IND
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @param[in] duration: If non-zero: Advertising duration, in units of 10ms
 * @param[in] num_events: If non-zero, indicates the maximum number of extended advertising events the controller
 *                        shall attemp to send prior to terminating the extended advertising.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_ext_adv(uint8_t adv_handle, uint16_t duration, uint8_t num_events);

/**
 * @brief     Stop BLE GAP extended advertising, will cause event @ref RTK_BT_LE_GAP_EVT_EXT_ADV_IND
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_ext_adv(uint8_t adv_handle);

/**
 * @brief     Remove an advertising set.
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_remove_ext_adv(uint8_t adv_handle);

/**
 * @brief     When an extended advertising set is stopped due to connnection established, get the stopped
 *            advertising handle according to the connection handle.
 * @param[in] conn_handle: Handle of connection.
 * @param[out] adv_handle: Handle of advertising set that stopped due to this connection.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_ext_adv_handle_by_conn_handle(uint16_t conn_handle, uint8_t *adv_handle);

/**
 * @brief     Start extended connection, will cause event @ref RTK_BT_LE_GAP_EVT_CONNECT_IND
 * @param[in] p_ext_conn_param: Extended connection parameter.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_ext_connect(rtk_bt_le_ext_create_conn_param_t *p_ext_conn_param);
#endif

#if defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT
/**
 * @brief     Set and Start BLE GAP periodic advertising, will cause event @ref RTK_BT_LE_GAP_EVT_PA_IND
 *            NOTE: Only advertising event properties @ref RTK_BT_LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_UNDIRECTED and
 *            @ref RTK_BT_LE_EXT_ADV_EXTENDED_ADV_NON_SCAN_NON_CONN_DIRECTED support periodic advertising.
 * @param[in] param: Advertising paramter and data
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_pa(rtk_bt_le_pa_param_t *param);

/**
 * @brief     Stop BLE GAP periodic advertising, will cause event @ref RTK_BT_LE_GAP_EVT_PA_IND
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_pa(uint8_t adv_handle);

/**
 * @brief     Stop BLE GAP periodic advertising, will cause event @ref RTK_BT_LE_GAP_EVT_PA_IND
 * @param[in] adv_handle: Identify an advertising set, which is assigned by @ref rtk_bt_le_gap_create_ext_adv.
 * @param[in] update_did_only: If true, keep advertising data unchanged and just update advertising DID of periodic advertising, data and len will be ignored.
 * @param[in] data: New periodic advertising data.
 * @param[in] len: New periodic advertising data length.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_update_pa(uint8_t adv_handle, bool update_did_only, uint8_t *data, uint16_t len);
#endif

#if defined(RTK_BLE_5_0_PA_SYNC_SUPPORT) && RTK_BLE_5_0_PA_SYNC_SUPPORT
/**
 * @brief     Modify Periodic Advertiser list.
 * @param[in] operation:  Add entry to Periodic Advertiser list, remove entry from Periodic Advertiser list or clear all entries
 *                        from Periodic Advertiser list. @ref rtk_bt_le_pa_sync_advlist_op_t.
 *                        NOTE: If operation is @ref RTK_BT_LE_PA_SYNC_ADV_LIST_CLEAR, adv_addr, adv_addr_type and adv_sid are irrelevant.
 * @param[in] adv_addr:   Device Address.
 *                        NOTE: Only @ref RTK_BT_LE_ADDR_TYPE_PUBLIC and @ref RTK_BT_LE_ADDR_TYPE_RANDOM could be used for modifying Periodic Advertiser list.
 * @param[in] adv_sid:    Advertising SID subfield in the ADI field used to identify the Periodic Advertising.
 *
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_pa_sync_modify_adv_list(rtk_bt_le_pa_sync_advlist_op_t operation, rtk_bt_le_addr_t adv_addr, uint8_t adv_sid);

/**
 * @brief     Synchronize with a periodic advertising train from an advertiser.
 *            This will cause event @ref RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND.
 *            Event @ref RTK_BT_LE_GAP_EVT_PA_ADV_REPORT_IND will be sent when adv report is enabled by param->options.
 * @param[in] p_pa_sync_create_sync_param:   GAP PA Sync create sync parameter. @ref rtk_bt_le_pa_sync_create_t.
 *
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_pa_sync_create(rtk_bt_le_pa_sync_create_t *param);

/**
 * @brief     Stop reception of the periodic advertising train, or cancel the synchronization creation while it is pending.
 * @param[in] sync_id:   Identify the synchronization with a periodic advertising train.
 *
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_pa_sync_terminate(uint8_t sync_id);

// #if defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
// /**
//  * @brief     Enable or disable reports for specified periodic advertising train.
//  * @param[in] sync_id:   Identify the synchronization with a periodic advertising train.
//  * @param[in] report_enable:   Enable or disable reports for periodic advertising.
//  * @param[in] duplicate_filter_enable:   Enable or disable duplicate filter of reports.
//  *
//  * @return
//  *            - 0  : Succeed
//  *            - Others: Error code
//  */
// uint16_t rtk_bt_le_gap_pa_sync_report_set(uint8_t sync_id, bool report_enable, bool duplicate_filter_enable);
#endif

#if defined(RTK_BLE_5_1_PAST_SENDER_SUPPORT) && RTK_BLE_5_1_PAST_SENDER_SUPPORT
/**
 * @brief     Start PAST transfer, will cause event @ref RTK_BT_LE_GAP_ACT_PAST_TRANSFER.
 * @param[in] conn_handle:   Connection handle.
 * @param[in] service_data:  A value provided by the Host to identify the periodic advertising train to the peer device.
 * @param[in] use_sync_id:   Indicate following member 'idx' is sync id or adv handle.
 *                           NOTE: If sync info to transter is obtained from remote device by sync procedure, set use_sync_id to true.
 *                           NOTE: If sync info to transter is from own periodic adv, set use_sync_id to false.
 * @param[in] idx:   If use_sync_id is true, idx is sync id, if use_sync_id is false, idx is adv handle.
 *
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_past_send(uint16_t conn_handle, uint16_t service_data, bool use_sync_id, uint8_t idx);
#endif

#if defined(RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT) && RTK_BLE_5_1_PAST_RECIPIENT_SUPPORT
/**
 * @brief     Set PAST recipient paramter, will cause event @ref RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND when received past packet.
 * @param[in] param:   PAST recipient setting.
 *
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_past_recipient_set(rtk_bt_le_past_recv_param_t *param);

/**
 * @brief     Set default PAST recipient paramter, will cause event @ref RTK_BT_LE_GAP_EVT_PA_SYNC_STATE_IND when received past packet.
 * @param[in] param:   default PAST recipient setting.
 *
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_default_past_recipient_set(rtk_bt_le_past_recv_param_t *param);
#endif

/**
 * @brief     Set scan info filter.
 * @param[in] p_scan_info: Scan info filter parameters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_scan_info_filter(rtk_bt_le_scan_info_filter_param_t *p_scan_info);

/**
 * @brief     Set scan paramters.
 * @param[in] p_gap_scan_param: Scan paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_scan_param(rtk_bt_le_scan_param_t *p_gap_scan_param);

/**
 * @brief     Get scan paramters.
 * @param[out] pscan_param: Scan paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_scan_param(rtk_bt_le_scan_param_t *pscan_param);

/**
 * @brief     Start scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_START_IND.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_scan(void);

/**
 * @brief     Stop scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_STOP_IND
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_scan(void);

#if defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV
/**
 * @brief     Set ext scan paramters.
 * @param[in] p_param: Ext Scan paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_ext_scan_param(rtk_bt_le_ext_scan_param_t *p_param);

/**
 * @brief     Stop ext scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_START_IND
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_ext_scan(void);

/**
 * @brief     Stop ext scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_STOP_IND
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_ext_scan(void);
#endif

/**
 * @brief     Start connection, will cause event @ref RTK_BT_LE_GAP_EVT_CONNECT_IND
 * @param[in] p_conn_param: Connection parmaters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connect(rtk_bt_le_create_conn_param_t *p_conn_param);

/**
 * @brief     Cancel the creating connection, will cause event @ref RTK_BT_LE_GAP_EVT_DISCONN_IND
 * @param[in] cancel_addr: Address of remote device that need to cancel connection.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connect_cancel(rtk_bt_le_addr_t *cancel_addr);

/**
 * @brief     Start disconnection, will cause event @ref RTK_BT_LE_GAP_EVT_DISCONN_IND
 * @param[in] conn_handle: The connection handle need to disconnect.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_disconnect(uint16_t conn_handle);

/**
 * @brief     Start disconnection with specific reason, will cause event @ref RTK_BT_LE_GAP_EVT_DISCONN_IND
 * @param[in] conn_handle: The connection handle need to disconnect.
 * @param[in] reason: The reason to disconnect @ref rtk_bt_err_hci.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_disconnect_with_reason(uint16_t conn_handle, uint8_t reason);

/**
 * @brief     Update connection paramters. If controller update the connection parameters,
 *            will cause event @ref RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND
 * @param[in] p_update_conn_param: Updated connection parmaters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_update_conn_param(rtk_bt_le_update_conn_param_t *p_update_conn_param);

/**
 * @brief     Modify device white list.
 * @param[in] p_wl_op_param: Modification operation paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_modify_whitelist(rtk_bt_le_modify_wl_param_t *p_wl_op_param);

/**
 * @brief     Read specific connection RSSI.
 * @param[in] conn_handle: Handle of connection to Read RSSI
 * @param[out] p_rssi: RSSI value
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_rssi(uint16_t conn_handle, int8_t *p_rssi);

/**
 * @brief     Read remote device's version information. will cause event @ref RTK_BT_LE_GAP_EVT_READ_REMOTE_VERSION_IND
 * @param[in] conn_handle: Connection handle of remote device to read
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_remote_version(uint16_t conn_handle);

/**
 * @brief     Get gap dev state.
 * @param[out] p_dev_state: Deice state
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_dev_state(rtk_bt_le_gap_dev_state_t *p_dev_state);

/**
 * @brief     Get connection information.
 * @param[in] conn_handle: Connection handle
 * @param[out] p_conn_info: Connection information
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_conn_info(uint16_t conn_handle, rtk_bt_le_conn_info_t *p_conn_info);

/**
 * @brief     Get LE connection ATT_MTU size.
 * @param[in] conn_handle: Connection handle
 * @param[out] p_mtu_size: Exchanged MTU size
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_mtu_size(uint16_t conn_handle, uint16_t *p_mtu_size);

/**
 * @brief     Get active connection number and their handles.
 * @param[out] p_active_conn: Active connection number and handles
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_active_conn(rtk_bt_le_get_active_conn_t *p_active_conn);

/**
 * @brief     Get active connection handle by address.
 * @param[in]  p_addr: Remote device's address
 * @param[out] p_conn_handle: Connection handle
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_conn_handle_by_addr(rtk_bt_le_addr_t *p_addr, uint16_t *p_conn_handle);

/**
 * @brief     Set Max MTU size
 * @param[in] mtu_size: MTU size value
 * @note      This API shall be called before creating connection. When connection is etablishing or
 *            a connection has been establised, it will fail.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_max_mtu_size(uint16_t mtu_size);

/**
 * @brief     Set Bluetooth device channel map.
 * @param[in] p_chan_map: Channel map, size is @ref RTK_BT_LE_CHAN_MAP_LEN
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_channels(uint8_t *p_chan_map);

#if defined(RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT) && RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT
/**
 * @brief     Set Bluetooth device connection data length paramters. If controller change the data length,
 *            will cause event @ref RTK_BT_LE_GAP_EVT_DATA_LEN_CHANGE_IND
 * @param[in] p_datalen_param: Connection data length paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_data_len(rtk_bt_le_set_datalen_param_t *p_datalen_param);
#endif

#if defined(RTK_BLE_5_0_SET_PHYS_SUPPORT) && RTK_BLE_5_0_SET_PHYS_SUPPORT
/**
 * @brief     Set Bluetooth device PHY paramters. If controller updates the PHY,
 *            will cause event @ref RTK_BT_LE_GAP_EVT_PHY_UPDATE_IND
 * @param[in] p_phy_param: PHY paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_phy(rtk_bt_le_set_phy_param_t *p_phy_param);
#endif

#if defined(RTK_BLE_PRIVACY_SUPPORT) && RTK_BLE_PRIVACY_SUPPORT
/**
 * @brief     Enable and initialize privacy.
 * @param[in] whitelist: Enable white list or not
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_privacy_init(bool whitelist);

/**
 * @brief     Set BLE privacy mode.
 * @param[in] peer_ident_addr_type: Peer indentity address type
 * @param[in] peer_addr: Peer identity address
 * @param[in] privacy_mode: Privacy mode
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_privacy_mode(rtk_bt_le_ident_addr_type_t peer_ident_addr_type,
										uint8_t *peer_addr, rtk_bt_le_privacy_mode_t privacy_mode);

/**
 * @brief     Read local resolvable address.
 * @param[in] peer_ident_addr_type: Peer indentity address type
 * @param[in] peer_addr: Peer identity address
 * @param[out] local_rpa: Local resolvable address get by this API
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_local_resolv_addr(rtk_bt_le_ident_addr_type_t peer_ident_addr_type,
											  uint8_t *peer_addr, uint8_t *local_rpa);

/**
 * @brief     Read peer resolvable address.
 * @param[in] peer_ident_addr_type: Peer indentity address type
 * @param[in] peer_addr: Peer identity address
 * @param[out] peer_rpa: Peer resolvable address get by this API
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_peer_resolv_addr(rtk_bt_le_ident_addr_type_t peer_ident_addr_type,
											 uint8_t *peer_addr, uint8_t *peer_rpa);
#endif

/**
 * @brief     Set security pairing mode.
 * @param[in] pairing_mode: Security pairing mode, default is @ref RTK_PAIRING_MODE_PAIRABLE
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_set_pairing_mode(rtk_bt_le_pairing_mode_t pairing_mode);

/**
 * @brief     Set security paramters.
 * @param[in] p_sec_param: Security paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_set_security_param(rtk_bt_le_security_param_t *p_sec_param);

/**
 * @brief     Get security paramters.
 * @param[out] p_sec_param: Security paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_get_security_param(rtk_bt_le_security_param_t *p_sec_param);

/**
 * @brief     Start security. If security is completed, will cause event @ref RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND
 * @param[in] conn_handle: Handle of connection to start security
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_start_security(uint16_t conn_handle);

/**
 * @brief     Confirm to continue pairing when use just work.
 * @param[in] p_pair_cfm: Pairing confirm parameter
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_pairing_confirm(rtk_bt_le_pair_cfm_t *p_pair_cfm);

/**
 * @brief     Input the authentication key.
 * @param[in] p_key_input: Auth key input parameter
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_passkey_entry(rtk_bt_le_auth_key_input_t *p_key_input);

/**
 * @brief     Confirm the authentication key.
 * @param[in] p_key_cfm: Auth key confirmation parameter
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_passkey_confirm(rtk_bt_le_auth_key_confirm_t *p_key_cfm);

#if defined(RTK_BLE_SMP_OOB_SUPPORT) && RTK_BLE_SMP_OOB_SUPPORT
/**
 * @brief     Set LE legacy pairing OOB key data.
 * @param[in] p_set_oob_key: OOB key data
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_set_oob_tk(rtk_bt_le_set_oob_key_t *p_set_oob_key);

/**
 * @brief     Get LE secure connection paring OOB data generated by local. This local OOB
 *            data is used to transmit to peer device by out of band channel.
 * @param[out] local_oob: local generated OOB data
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_get_sc_local_oob(rtk_bt_le_sc_local_oob_data_t *local_oob);

/**
 * @brief     Input LE secure connection pairing OOB data which is transmitted from peer device
 *            by out of band channel. The peer device shall have been connected with local.
 * @param[in] peer_oob: peer OOB data to input
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_input_sc_peer_oob(rtk_bt_le_sc_peer_oob_data_t *peer_oob);
#endif

/**
 * @brief     Get the number of bonded device.
 * @param[out] bond_num: The number of bonded device
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_get_bond_num(uint8_t *bond_num);

/**
 * @brief     Judge if the device is bonded.
 * @param[in] paddr: Device's address
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
bool rtk_bt_le_sm_is_device_bonded(rtk_bt_le_addr_t *paddr);

/**
 * @brief     Get bonded device info.
 * @param[in] p_bond_info: The info of bonded device
 * @param[in] size: The num of bonded device we want get (buffer size of info)
 * @param[out] size: The num of actual bonded device in host stack.
 * @note      If the out param <size> is larger than the <size> in, it will just fill in the param <info> of size in.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_get_bond_info(rtk_bt_le_bond_info_t *p_bond_info, uint8_t *size);

/**
 * @brief     Delete bonded device by address, will cause event @ref RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND
 * @param[in] paddr: Bonded device's address
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_delete_bond_device(rtk_bt_le_addr_t *paddr);

/**
 * @brief     Clear bonded list, will cause event @ref RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_clear_bond_list(void);

/**
 * @brief     Get Tx pending num
 * @param[in]  conn_handle: Connection handle
 * @param[out] tx_pending_num: Tx pending num
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_tx_pending_num(uint16_t conn_handle, uint16_t *tx_pending_num);

#if defined(RTK_BLE_5_2_POWER_CONTROL_SUPPORT) && RTK_BLE_5_2_POWER_CONTROL_SUPPORT
/**
 * @brief     Read the current and maximum transmit power levels of the local Controller on the ACL connection and the PHY
 * @param[in] conn_handle: Connection handle
 * @param[in] phy: @ref rtk_bt_le_txpower_phy_t
 * @param[out] cur_txpower: Current transmit power level Range: -127 to 20 Units: dBm
 *                          Note: 0x7F meands current transmit power level is unavailable
 * @param[out] max_txpower: Maximum transmit power level Range: -127 to 20 Units: dBm
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_local_tx_power(uint16_t conn_handle, rtk_bt_le_txpower_phy_t phy,
										   int8_t *cur_txpower, int8_t *max_txpower);

/**
 * @brief     Read the transmit power level used by the remote Controller on the ACL connection and the PHY.
 *            Read result will be returned by callback @ref RTK_BT_LE_GAP_EVT_TXPOWER_REPORT_IND
 * @param[in] conn_handle: Connection handle
 * @param[in] phy: @ref rtk_bt_le_txpower_phy_t
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_remote_tx_power(uint16_t conn_handle, rtk_bt_le_txpower_phy_t phy);

/**
 * @brief     Enable or disable reporting transmit power level changes in the local and remote Controllers for the ACL connection.
 * @param[in] conn_handle: Connection handle
 * @param[in] local_enable: Enable or disable local transmit power change reports.
 * @param[in] remote_enable: Enable or disable remote transmit power change reports.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_tx_power_report_set(uint16_t conn_handle, bool local_enable, bool remote_enable);
#endif

#if defined(RTK_BLE_5_1_CTE_SUPPORT) && RTK_BLE_5_1_CTE_SUPPORT

/**
 * @brief     Get antenna information supprted by the controller
 * @param[out] antenna_info: Antenna information
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_antenna_info(rtk_bt_le_gap_antenna_info_t *antenna_info);

#if ((defined(RTK_BLE_5_0_USE_EXTENDED_ADV) && RTK_BLE_5_0_USE_EXTENDED_ADV) && \
    (defined(RTK_BLE_5_0_PA_ADV_SUPPORT) && RTK_BLE_5_0_PA_ADV_SUPPORT))
/**
 * @brief     Start connectionless CTE transmit.
 * @param[in] p_cte_param: connetionless CTE transmit parameters
 * @param[in] p_eadv_param: extended advertising parameters
 * @param[in] p_pa_param: periodic advertising parameters
 * @param[out] p_adv_handle: extended advertising handle
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connless_cte_tx_start(rtk_bt_le_gap_connless_cte_tx_param_t *p_cte_param,
											 rtk_bt_le_ext_adv_param_t *p_eadv_param,
											 rtk_bt_le_pa_param_t *p_pa_param,
											 uint8_t *p_adv_handle);

/**
 * @brief     Stop connectionless CTE transmit.
 * @param[in] adv_handle: advertising handle
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connless_cte_tx_stop(uint8_t adv_handle);

#endif /* RTK_BLE_5_0_USE_EXTENDED_ADV && RTK_BLE_5_0_PA_ADV_SUPPORT */

/**
 * @brief     Start connectionless CTE receive.
 * @param[in] sync_id: Identify the periodic advertising train.
 * @param[in] params: connetionless CTE receive paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connless_cte_rx_start(uint8_t sync_id, rtk_bt_le_gap_connless_cte_rx_param_t *params);

/**
 * @brief     Stop connectionless CTE receive.
 * @param[in] sync_id: Identify the periodic advertising train.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connless_cte_rx_stop(uint8_t sync_id);

/**
 * @brief     Start connection-oriented CTE transmit.
 * @param[in] conn_handle: connection handle
 * @param[in] params: connection-oriented CTE transmit paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_conn_cte_tx_start(uint16_t conn_handle, rtk_bt_le_gap_conn_cte_tx_param_t *params);

/**
 * @brief     Stop connection-oriented CTE transmit.
 * @param[in] conn_handle: connection handle
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_conn_cte_tx_stop(uint16_t conn_handle);

/**
 * @brief     Start connection-oriented CTE receive.
 * @param[in] conn_handle: connection handle
 * @param[in] rx_params:  connection-oriented CTE receive paramters
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_conn_cte_rx_start(uint16_t conn_handle,
										 rtk_bt_le_gap_conn_cte_rx_param_t *rx_params);

/**
 * @brief     Stop connection-oriented CTE receive.
 * @param[in] conn_handle: connection handle
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_conn_cte_rx_stop(uint16_t conn_handle);
#endif /* RTK_BLE_5_1_CTE_SUPPORT */

#if defined(RTK_BLE_5_4_PA_RSP_SUPPORT) && RTK_BLE_5_4_PA_RSP_SUPPORT
/**
 * @brief     Set the data for one subevent of a Periodic Advertising with Responses Advertiser in reply data request.
 * @param[in] param: parameter @rtk_bt_le_gap_pawr_subevent_data_t
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_pawr_set_subevent_data(rtk_bt_le_gap_pawr_subevent_data_t *param);
#endif

#if defined(RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT) && RTK_BLE_5_4_PA_SYNC_RSP_SUPPORT
/**
 * @brief     Set the data for a response slot in a specific subevent of the PAwR.
 * @param[in] param: parameter @rtk_bt_le_gap_pawr_set_response_t
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_pawr_set_response_data(rtk_bt_le_gap_pawr_set_response_t *param);

/**
 * @brief     Synchronize with a subset of subevents.
 * @param[in] param: parameter @rtk_bt_le_gap_pawr_sync_subevent_t
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_pawr_sync_subevent(rtk_bt_le_gap_pawr_sync_subevent_t *param);
#endif

#if defined(RTK_BLE_COC_SUPPORT) && RTK_BLE_COC_SUPPORT
/**
 * @brief     Register or unregister le psm for le credit based connection-oriented channel(coc).
 * @param[in] is_register: 1 for register, 0 for unregister
 * @param[in] le_psm: LE PSM value
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_register_psm(uint8_t is_register, uint16_t le_psm);

/**
 * @brief     Set security permission for le psm
 * @param[in] le_psm: LE PSM value
 * @param[in] active: 1 for set security, 0 for clear the set for this LE PSM
 * @param[in] sec_mode: COC security mode to set
 * @param[in] key_size: Security key size, usually set as 16(common encryption key size)
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_set_psm_security(uint16_t le_psm, uint8_t active,
											rtk_bt_le_coc_security_mode_t sec_mode,
											uint8_t key_size);

/**
 * @brief     Set le coc parameter
 * @param[in] param_type: Parameter type
 * @param[in] value: Parameter value
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_set_param(rtk_bt_le_coc_param_type_t param_type, uint16_t value);

/**
 * @brief     Get le coc channel parameter
 * @param[in] param_type: Parameter type
 * @param[in] cid: COC channel ID
 * @param[out] value: Pointer of parameter value to get
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_get_chan_param(rtk_bt_le_coc_chan_param_type_t param_type,
										  uint16_t cid, uint16_t *value);

/**
 * @brief     COC connection request, will cause event @ref RTK_BT_LE_GAP_EVT_COC_CONNECT_IND
 * @param[in] conn_handle: Connection handle
 * @param[in] le_psm: LE PSM value
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_connect(uint16_t conn_handle, uint16_t le_psm);

/**
 * @brief     COC disconnection request, will cause event @ref RTK_BT_LE_GAP_EVT_COC_DISCONNECT_IND
 * @param[in] cid: COC channel ID
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_disconnect(uint16_t cid);

/**
 * @brief     COC send data, will cause event @ref RTK_BT_LE_GAP_EVT_COC_SEND_DATA_RESULT_IND
 * @param[in] cid: COC channel ID
 * @param[in] len: Length of data
 * @param[in] data: Pointer of data
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_coc_send_data(uint16_t cid, uint16_t len, uint8_t *data);
#endif /* RTK_BLE_COC_SUPPORT */

/**
 * @brief     Set BLE receiver test parameters and start
 * @param[in] p_rx_test_param: BLE receiver test parameters
 * @param[in] rx_version:      BLE receiver test version
 * @note      This API shall be called after BT init, and DO NOT execute ADV / Scan / Connection related actions
 *            before calling this API. After calling this API, DO NOT execute ADV / Scan / Connection actions
 *            UNLESS BT deinit & BT init is done again before that.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_dtm_rx_test(rtk_bt_le_dtm_rx_param_t *p_rx_param, rtk_bt_le_dtm_rx_version_t rx_version);

/**
 * @brief     Set BLE transmitter test parameters and start
 * @param[in] p_tx_test_param: BLE transmitter test parameters
 * @param[in] tx_version:      BLE transmitter test version
 * @note      This API shall be called after BT init, and DO NOT execute ADV / Scan / Connection related actions
 *            before calling this API. After calling this API, DO NOT execute ADV / Scan / Connection actions
 *            UNLESS BT deinit & BT init is done again before that.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_dtm_tx_test(rtk_bt_le_dtm_tx_param_t *p_tx_param, rtk_bt_le_dtm_tx_version_t tx_version);

/**
 * @brief     Set BLE receiver or transmitter end and report number of test packets received
 * @param[out] p_num_pkts: Pointer of number of test packets received
 * @note      This API shall be called after BT init, and DO NOT execute ADV / Scan / Connection related actions
 *            before calling this API. After calling this API, DO NOT execute ADV / Scan / Connection actions
 *            UNLESS BT deinit & BT init is done again before that.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_dtm_end(uint16_t *p_num_pkts);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_LE_GAP_H__ */
