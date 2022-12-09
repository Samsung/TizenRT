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
typedef enum
{
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
typedef enum
{
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
	RTK_BT_LE_GAP_ADTYPE_3D_INFO_DATA                            = 0x3D, /*!< 0x3D, 3D Information Data */
	RTK_BT_LE_GAP_ADTYPE_MANUFACTURER_SPECIFIC                   = 0xFF, /*!< 0xFF, Manufacturer Specific Data */
} rtk_bt_le_gap_ad_type_t;

/**
 * @typedef   rtk_bt_le_gap_ad_flags_data_t
 * @brief     Bluetooth ble gap advertising data type data definition.
 */
typedef enum
{
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
typedef enum
{
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
	uint16_t hci_revision;			/*!< hci revision */
	uint16_t lmp_subversion;		/*!< lmp subversion */
	uint16_t btgap_revision;		/*!< btgap stack version */
	uint16_t btgap_buildnum;		/*!< btgap stack build number */
} rtk_bt_le_version_info_t;

#if RTK_BLE_5_0_SET_PHYS_SUPPORT
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
#endif

#if RTK_BLE_5_0_AE_ADV_SUPPORT
/**
 * @typedef   rtk_bt_le_adv_event_prop_t
 * @brief     Bluetooth LE GAP adv event properties.
 */
typedef enum
{
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
 * @brief     Bluetooth LE GAP adv type definition.
 */
typedef enum
{
	RTK_BT_LE_ADV_CHNL_37               = 0x01, /*!< 0x00, Channel 37 shall be used */
	RTK_BT_LE_ADV_CHNL_38               = 0x02, /*!< 0x02, Channel 38 shall be used */
	RTK_BT_LE_ADV_CHNL_39               = 0x04, /*!< 0x04, Channel 39 shall be used */
	RTK_BT_LE_ADV_CHNL_ALL              = 0x07, /*!< 0x07, Channel 37,38,39 shall be used */
	RTK_BT_LE_ADV_CHNL_RESERVED,                /*!< reserved */
} rtk_bt_le_adv_ch_map_t;

/**
 * @typedef   rtk_bt_le_adv_filter_t
 * @brief     Bluetooth LE GAP adv filter type definition.
 */
typedef enum
{
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY              = 0, /*!< 0, Allow both scan and connection requests from anyone */
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_ANY             = 1, /*!< 1, Allow both scan req from White List devices only and connection req from anyone */
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_ANY_CON_WLST             = 2, /*!< 2, Allow both scan req from anyone and connection req from White List devices only */
	RTK_BT_LE_ADV_FILTER_ALLOW_SCAN_WLST_CON_WLST            = 3, /*!< 3, Allow scan and connection requests from White List devices only */
	RTK_BT_LE_ADV_FILTER_RESERVED,                                /*!< All bigger value are reserved */
} rtk_bt_le_adv_filter_t;

/**
 * @struct    rtk_bt_le_preferred_conn_param_t
 * @brief     Bluetooth LE preferred connection parameters definition.
 */
typedef struct
{
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
	uint16_t supervision_timeout;   /*!<Defines the connection supervisor timeout multiplier as amultiple of 10ms.
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
typedef enum
{
	RTK_BT_LE_ADDR_TYPE_PUBLIC =            0x00,      /*!< Public Device Address */
	RTK_BT_LE_ADDR_TYPE_RANDOM =            0x01,      /*!< Random Device Address */
	RTK_BT_LE_ADDR_TYPE_RPA_PUBLIC =        0x02,      /*!< Public Identity Address (RPA) */
	RTK_BT_LE_ADDR_TYPE_RPA_RANDOM =        0x03,      /*!< Random (static) Identity Address (RPA)*/

	RTK_BT_LE_ADDR_TYPE_PEER_ANONYMOUS =    0xFF,      /*!< Anonymous Device Address */
} rtk_bt_le_addr_type_t;

/**
 * @def   RTK_BD_ADDR_LEN
 * @brief     Bluetooth device address length macro.
 */
#define RTK_BD_ADDR_LEN             6

/**
 * @struct    rtk_bt_le_addr_t
 * @brief     Bluetooth LE device address definition.
 */
typedef struct
{
	rtk_bt_le_addr_type_t type;           /*!< address type */
	uint8_t addr_val[RTK_BD_ADDR_LEN];              /*!< address */
} rtk_bt_le_addr_t;

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
														* 1. advertisement packets from devices where the advertiser’s address is in the White list.
														* 2. Directed advertising packets which are not addressed for this device shall be ignored. 
														*/
	RTK_BT_LE_SCAN_FILTER_ALLOW_UND_RPA_DIR     = 0x2, /*!< Accept all :
														* 1. undirected advertisement packets, and
														* 2. directed advertising packets where the initiator address is a resolvable private address, and
														* 3. directed advertising packets addressed to this device.
														*/
	RTK_BT_LE_SCAN_FILTER_ALLOW_WLIST_RPA_DIR   = 0x3, /*!< Accept all :
														* 1. advertisement packets from devices where the advertiser’s address is in the White list, and
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
	RTK_BT_LE_SCAN_DUPLICATE_ENABLED_RESET_FOR_EACH_PERIOD = 2,		/**<  Duplicate filtering enabled, reset for each scan period. */
	RTK_BT_LE_SCAN_DUPLICATE_RESERVED,        /*!< reserved */
} rtk_bt_le_scan_duplicate_t;

/**
 * @typedef   rtk_bt_le_link_role_t
 * @brief     Bluetooth LE GAP link role definition.
 */
typedef enum
{
	RTK_BT_LE_ROLE_MASTER           = 0x00, /*!< 0x00, means master */
	RTK_BT_LE_ROLE_SLAVE            = 0x01, /*!< 0x01, means slave */
	RTK_BT_LE_ROLE_UNKNOWN,                 /*!< unknown */
} rtk_bt_le_link_role_t;

/**
 * @typedef   rtk_bt_le_adv_stop_reason_t
 * @brief     Bluetooth LE ADV type Definition
 */
typedef enum
{
	RTK_BT_LE_ADV_STOP_BY_HOST      = 0,        /*!< 0, stopped by host */
	RTK_BT_LE_ADV_STOP_BY_CONN      = 1,        /*!< 1, stopped due to connection established */
	RTK_BT_LE_ADV_STOP_BY_DURATION  = 2,        /*!< 2, stopped due to high duty cycle directed advertising timeout */
	RTK_BT_LE_ADV_STOP_UNKNOWN,                 /*!< unkown */
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

#if RTK_BLE_5_0_AE_ADV_SUPPORT
/**
 * @struct    rtk_bt_le_ext_adv_param_t
 * @brief     Bluetooth LE GAP set ext adv paramter definition.
 */
typedef struct {
	/** Advertising Set Index */
	uint8_t idx;
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
	/** Scan_Request_Notification_Enable */
	bool scan_req_notification_enable;

	/** If non-zero: Advertising duration, in units of 10ms */
	uint16_t duration;
	/** If non-zero: Maximum number of extended advertising events to send */
    uint8_t  max_ext_adv_evt;
} rtk_bt_le_ext_adv_param_t;

/**
 * @typedef   rtk_bt_le_ext_adv_data_t
 * @brief     Bluetooth LE GAP set ext adv data definition.
 */
typedef struct {
	/** Advertising Set Index */
	uint8_t idx;
	/** Advertising Data */
	uint8_t *pdata;
	/** Advertising Data Len */
	uint16_t len;
} rtk_bt_le_ext_adv_data_t;
#endif

#if RTK_BLE_5_0_PA_SUPPORT
/**
 * @typedef   Periodic Advertising Properties flag
 * @brief Use the combination of enum definitions to indicate which fields should be included in the advertising packet.
 */
typedef enum
{
	RTK_BT_LE_PA_ADV_PROP_INCLUDE_TX_POWER = 0x0040   /**< Include TxPower in the periodic header of the advertising PDU. */
}rtk_bt_le_pa_prop_t;

/**
 * @struct    rtk_bt_le_pa_param_t
 * @brief     Bluetooth LE GAP set periodic adv paramter definition.
 */
typedef struct {
	/** Advertising Set Index */
	uint8_t idx;
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
} rtk_bt_le_pa_param_t;
#endif

/**
 * @struct    rtk_bt_le_scan_info_filter_param_t
 * @brief     Bluetooth LE GAP scan info filter paramters definition.
 */
typedef struct {
	bool enable;		/*!< Wether to enable the scan info comparison function */
	uint8_t offset;		/*!< The start offset of the scan info to compare */
	uint8_t len;		/*!< Length of data to compare */
	uint8_t *p_filter;	/*!< Point the data to compare with the scan info */
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

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
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
	 * Range: 0x0004 to 0x4000\n
	 * Default: 0x0010 (10 ms)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 10.24 s\n
	 * interval[0] for 1M PHY, interval[1] for Coded PHY.\n
	 */
	uint16_t interval[2];
	/** 
	 * The duration of the LE scan. LE_Scan_Window shall be less\n
	 * than or equal to LE_Scan_Interval\n
	 * Range: 0x0004 to 0x4000\n
	 * Default: 0x0010 (10 ms)\n
	 * Time = N * 0.625 ms\n
	 * Time Range: 2.5 ms to 10.24 s\n
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
	uint16_t timeout;
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
	uint16_t timeout;
} rtk_bt_le_update_conn_param_t;

/**
 * @enum    rtk_bt_le_wl_op_t
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

/**
 * @def    RTK_BT_LE_CHAN_MAP_LEN
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
	uint16_t timeout;
	/** connection tx phy */
	uint8_t tx_phy;
	/** connection rx phy */
	uint8_t rx_phy;
} rtk_bt_le_conn_info_t;

/**
 * @enum    rtk_bt_le_gap_init_state
 * @brief     Bluetooth LE GAP init state.
 */
enum rtk_bt_le_gap_init_state {
	RTK_BT_LE_GAP_INIT_STATE_INIT           = 0,    /*!< GAP is in initiating status */
	RTK_BT_LE_GAP_INIT_STATE_STACK_READY    = 1,    /*!< GAP stack is ready */
};

/**
 * @enum    rtk_bt_le_adv_state
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
	uint8_t all_phys;           /*!< preferred all phys */
	uint8_t tx_phys;            /*!< preferred tx phys */
	uint8_t rx_phys;            /*!< preferred rx phys */
	uint16_t phy_options;       /*!< preferred phy options*/
} rtk_bt_le_set_phy_param_t;

/**
 * @typedef   rtk_bt_le_privacy_mode_t
 * @brief     Bluetooth LE GAP privacy mode type definition.
 */
typedef enum {
	RTK_BT_LE_PRIVACY_MODE_NETWORK  = 0x0,      /*!< 0x00, Use Network Privacy Mode for this peer device (default) */
	RTK_BT_LE_PRIVACY_MODE_DEVICE   = 0x1,      /*!< 0x01, Use Device Privacy Mode for this peer device */
} rtk_bt_le_privacy_mode_t;

/**
 * @struct    rtk_bt_le_set_privacy_mode_param_t
 * @brief     Bluetooth LE GAP privacy mode parameter definition.
 */
typedef struct {
	rtk_bt_le_addr_t addr;                  /*!< Peer device address */
	rtk_bt_le_privacy_mode_t mode;          /*!< Privacy mode */
} rtk_bt_le_set_privacy_mode_param_t;

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
 * @def    RTK_PAIRING_PASSKEY_MAX
 * @brief     Bluetooth LE SM max passkey value macro.
 */
#define RTK_PAIRING_PASSKEY_MAX     999999

/**
 * @struct    rtk_bt_le_security_param_t
 * @brief     Bluetooth BLE SM security parameter type definition.
 */
typedef struct {
	rtk_bt_le_io_cap_t io_cap;      /*!< IO capabilities */
	uint8_t oob_data_flag;          /*!< OOB data flag */
	uint8_t bond_flag;              /*!< Bonding flags */
	uint8_t mitm_flag;              /*!< MITM flag */
	uint8_t sec_pair_flag;          /*!< Secure connection pairing support flag */
	uint8_t use_fixed_key;          /*!< Pairing use fixed passkey */
	uint32_t fixed_key;             /*!< Fixed passkey value */
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
 * @def    RTK_BT_LE_SM_OOB_KEY_LEN
 * @brief     Bluetooth LE SM max oob key length macro.
 */
#define RTK_BT_LE_SM_OOB_KEY_LEN              16

/**
 * @struct    rtk_bt_le_set_oob_key_t
 * @brief     Bluetooth BLE SM set OOB key parameter definition.
 */
typedef struct {
	uint16_t conn_handle;       /*!< Connection handle */
	uint8_t oob_key[RTK_BT_LE_SM_OOB_KEY_LEN];      /*!< OOB KEY to input */
} rtk_bt_le_set_oob_key_t;

/**
 * @struct    rtk_bt_le_bond_info_t
 * @brief     Bluetooth BLE SM bond information type definition.
 */
typedef struct {
	rtk_bt_le_addr_t addr;      /*!< Bonded device address */
	bool is_connected;       /*!< Connection status */
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
typedef struct{
	uint16_t err;                                   /*!< Error code */
	rtk_bt_le_adv_stop_reason_t  stop_reason;       /*!< Adv stop reason */
} rtk_bt_le_adv_stop_ind_t;

#if RTK_BLE_5_0_AE_ADV_SUPPORT
/**
 * @struct    rtk_bt_le_ext_adv_ind_t
 * @brief     Bluetooth LE ext adv state indication msg.
 */
typedef struct{
	uint8_t idx;			 /*!< Advertising Set Index */
	bool is_start;	     	 /*!< Start or Stop */
	uint16_t cause;          /*!< reason */
} rtk_bt_le_ext_adv_ind_t;
#endif

#if RTK_BLE_5_0_PA_SUPPORT
/**
 * @typedef    rtk_bt_le_pa_state_t
 * @brief     Bluetooth LE periodic adv state.
 */
typedef enum{
	RTK_BT_LE_PA_STATE_IDLE,			 			/**< Idle, no periodic advertising. */
	RTK_BT_LE_PA_STATE_ADVERTISING,	     	 		/**< Periodic advertising. */
	RTK_BT_LE_PA_STATE_WAIT_EXT_ADV_ADVERTISING,	/**< Periodic advertising is not started until the extended advertising of the specified advertising set is enabled. */
} rtk_bt_le_pa_state_t;

/**
 * @struct    rtk_bt_le_pa_ind_t
 * @brief     Bluetooth LE periodic adv state indication msg.
 */
typedef struct{
	uint8_t idx;			 		/*!< Advertising Set Index */
	rtk_bt_le_pa_state_t state;	/*!< State */
	uint16_t cause;          		/*!< reason */
} rtk_bt_le_pa_ind_t;
#endif

/**
 * @struct    rtk_bt_le_scan_start_ind_t
 * @brief     Bluetooth LE scan start indication msg.
 */
typedef struct {
	uint16_t err;						/*!< Error code */
	rtk_bt_le_scan_type_t scan_type;	/*!< Scan type */
} rtk_bt_le_scan_start_ind_t;

/**
 * @enum    rtk_bt_le_adv_report_type_t
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
 * @def    RTK_BT_LE_MAX_ADV_DATA_LEN
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
	uint8_t num_report;					/*!< num of responses in event, here will be always 1 */
	rtk_bt_le_adv_report_t adv_report;	/*!< adv data to be reported from controller */
} rtk_bt_le_scan_res_ind_t;

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
typedef struct {
	uint16_t evt_type;
	rtk_bt_le_addr_t addr;
	rtk_bt_le_addr_t direct_addr;
	int8_t rssi;
    uint8_t primary_phy;
    uint8_t secondary_phy;
    uint8_t adv_sid;
    int8_t tx_power;
    uint16_t peri_adv_interval;
	uint16_t len;
	uint8_t *data; /* Must be the last member */
} rtk_bt_le_ext_scan_res_ind_t;
#endif

/**
 * @enum    rtk_bt_le_scan_stop_reason_t
 * @brief     Bluetooth LE scan stop reason.
 */
typedef enum
{
	RTK_BT_LE_SCAN_STOP_BY_HOST      = 0,        /*!< 0, stopped by host */
	RTK_BT_LE_SCAN_STOP_BY_DURATION  = 1,        /*!< 1, stopped due to scan duration timeout */
	RTK_BT_LE_SCAN_STOP_UNKNOWN,                 /*!< unkown */
} rtk_bt_le_scan_stop_reason_t;

/**
 * @struct    rtk_bt_le_scan_stop_ind_t
 * @brief     Bluetooth LE scan stop indication msg.
 */
typedef struct {
	uint16_t err;								/*!< Error code */
	rtk_bt_le_scan_stop_reason_t stop_reason;	/*!< scan stop reason */
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
	uint16_t timeout;                           /*!< Connection supervision timeout */
} rtk_bt_le_conn_ind_t;

/**
 * @struct    rtk_bt_le_disconn_ind_t
 * @brief     Bluetooth LE disconnect complete event msg.
 */
typedef struct {
	uint16_t reason;               				/*!< Disconnect reason */
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
	uint16_t timeout;                       /*!< Connection supervision timeout */
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
	uint16_t timeout;                       /*!< Connection supervision timeout */
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
 * @struct    rtk_bt_le_auth_complete_ind_t
 * @brief     Bluetooth LE auth compeleted event msg.
 */
typedef struct {
    uint16_t err;                           /*!< Error code */
    uint16_t conn_handle;                   /*!< Connection handle */
    uint8_t dev_ltk_length;                 /*!< Device long term key length*/
    uint8_t dev_ltk[32];                    /*!< Device long term key */
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
 * @struct    rtk_bt_le_wl_modify_ind_t
 * @brief     Bluetooth LE whitelist modified event msg.
 */
typedef struct {
	rtk_bt_le_wl_op_t op;                   /*!< Whitelist modify operation */
	uint16_t err;                           /*!< Error code */
} rtk_bt_le_wl_modify_ind_t;

/**
 * @enum    rtk_bt_le_bond_op_t
 * @brief     Bluetooth LE bond operation definition.
 */
typedef enum {
	RTK_BT_LE_BOND_DELETE,          /*!< Delete bond device by address */
	RTK_BT_LE_BOND_CLEAR,           /*!< Clear all bond device in bond list */
	RTK_BT_LE_BOND_RESERVED,        /*!< Reserved */
} rtk_bt_le_bond_op_t;

/**
 * @struct    rtk_bt_le_bond_modify_ind_t
 * @brief     Bluetooth LE bond modified event msg.
 */
typedef struct {
	rtk_bt_le_bond_op_t op;         /*!< Bond modify operation */
} rtk_bt_le_bond_modify_ind_t;


/****************** Data structure for API func param pack and unpack ******************/

typedef struct {
	uint16_t conn_handle;
	int8_t *p_rssi;
} rtk_bt_le_read_rssi_param_t;

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


/**
 * @defgroup  bt_le_gap BT LE GAP APIs
 * @brief     BT LE GAP related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @fn        uint16_t rtk_bt_le_gap_get_version(rtk_bt_le_version_info_t *version)
 * @brief     Get local stack version information.
 * @param[out] version: version information
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_version(rtk_bt_le_version_info_t *version);

/**
 * @fn        uint16_t rtk_bt_le_gap_get_address(rtk_bt_le_addr_t *paddr)
 * @brief     Get local device's address.
 * @param[out] paddr: Device address under use
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_address(rtk_bt_le_addr_t *paddr);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_device_name(const uint8_t *name)
 * @brief     Set bluetooth Device name.
 * @param[in] name: Device name
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_device_name(const uint8_t *name);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_appearance(uint16_t appearance)
 * @brief     Set device appearance.
 * @param[in] appearance: Device apperance
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_appearance(uint16_t appearance);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_preferred_conn_param(rtk_bt_le_preferred_conn_param_t *p_preferred_conn_param)
 * @brief     Set preferred connetion parameters.
 * @param[in] p_preferred_conn_param: preferred connetion parameters, @ref rtk_bt_le_preferred_conn_param_t
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_preferred_conn_param(rtk_bt_le_preferred_conn_param_t *p_preferred_conn_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_rand_addr(rtk_bt_le_addr_t *bd_addr)
 * @brief     Set random address.
 * @param[in] bd_addr: Random address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_rand_addr(rtk_bt_le_addr_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_adv_data(uint8_t *padv_data, uint32_t adv_len)
 * @brief     Set BLE GAP advertising data.
 * @param[in] padv_data: Advertising data
 * @param[in] adv_len: Length of padv_data
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_adv_data(uint8_t *padv_data, uint32_t adv_len);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_scan_rsp_data(uint8_t *pscan_rsp_data, uint32_t scan_rsp_len)
 * @brief     Set BLE GAP scan response data.
 * @param[in] pscan_rsp_data: Scan response data
 * @param[in] scan_rsp_len: Length of pscan_rsp_data
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_scan_rsp_data(uint8_t *pscan_rsp_data, uint32_t scan_rsp_len);

/**
 * @fn        uint16_t rtk_bt_le_gap_start_adv(rtk_bt_le_adv_param_t *padv_param)
 * @brief     Start BLE GAP advertising, will cause event @ref RTK_BT_LE_GAP_EVT_ADV_START_IND
 * @param[in] padv_param: Advertising parameters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_adv(rtk_bt_le_adv_param_t *padv_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_stop_adv(void)
 * @brief     Stop BLE GAP advertising, will cause event @ref RTK_BT_LE_GAP_EVT_ADV_STOP_IND
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_adv(void);

#if RTK_BLE_5_0_AE_ADV_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_gap_set_ext_adv_data(rtk_bt_le_ext_adv_data_t *padv_data)
 * @brief     Set BLE GAP extended advertising data.
 * @param[in] padv_data: Extended advertising data
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_ext_adv_data(rtk_bt_le_ext_adv_data_t *padv_data);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_ext_scan_rsp_data(rtk_bt_le_ext_adv_data_t *pscan_rsp_data)
 * @brief     Set BLE GAP extended scan response data.
 * @param[in] pscan_rsp_data: Extended scan response data
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_ext_scan_rsp_data(rtk_bt_le_ext_adv_data_t *pscan_rsp_data);

/**
 * @fn        uint16_t rtk_bt_le_gap_start_ext_adv(rtk_bt_le_ext_adv_param_t *padv_param)
 * @brief     Start BLE GAP extended advertising, will cause event @ref RTK_BT_LE_GAP_EVT_EXT_ADV_IND
 * @param[in] padv_param: Advertising parameters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_ext_adv(rtk_bt_le_ext_adv_param_t *padv_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_stop_ext_adv(uint8_t adv_idx)
 * @brief     Stop BLE GAP extended advertising, will cause event @ref RTK_BT_LE_GAP_EVT_EXT_ADV_IND
 * @param[in] adv_idx: Advertising Set index
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_ext_adv(uint8_t adv_idx);

/**
 * @fn        uint16_t rtk_bt_le_gap_remove_ext_adv(uint8_t adv_idx)
 * @brief     Remove an advertising set.
 * @param[in] adv_idx: Advertising Set index
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_remove_ext_adv(uint8_t adv_idx);
#endif

#if RTK_BLE_5_0_PA_SUPPORT
/**
 * @fn        rtk_bt_le_gap_start_pa(rtk_bt_le_pa_param_t *ppa_param)
 * @brief     Set and Start BLE GAP periodic advertising, will cause event @ref RTK_BT_LE_GAP_EVT_PA_IND
 * @param[in] param: Advertising paramter and data
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_pa(rtk_bt_le_pa_param_t *param);
#endif

/**
 * @fn        uint16_t rtk_bt_le_gap_scan_info_filter(rtk_bt_le_scan_info_filter_param_t *p_scan_info)
 * @brief     Set scan info filter.
 * @param[in] p_scan_info: Scan info filter parameters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_scan_info_filter(rtk_bt_le_scan_info_filter_param_t *p_scan_info);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_scan_param(rtk_bt_le_scan_param_t *p_gap_scan_param)
 * @brief     Set scan paramters.
 * @param[in] p_gap_scan_param: Scan paramters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_scan_param(rtk_bt_le_scan_param_t *p_gap_scan_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_start_scan(void)
 * @brief     Start scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_START_IND.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_start_scan(void);

/**
 * @fn        uint16_t rtk_bt_le_gap_stop_scan(void)
 * @brief     Stop scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_STOP_IND
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_stop_scan(void);

#if RTK_BLE_5_0_AE_SCAN_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_gap_ext_scan_set_param(rtk_bt_le_ext_scan_param_t *p_param)
 * @brief     Set ext scan paramters.
 * @param[in] p_param: Ext Scan paramters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_ext_scan_set_param(rtk_bt_le_ext_scan_param_t *p_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_ext_scan_start(void)
 * @brief     Stop ext scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_START_IND
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_ext_scan_start(void);

/**
 * @fn        uint16_t rtk_bt_le_gap_ext_scan_stop(void)
 * @brief     Stop ext scan, will cause event @ref RTK_BT_LE_GAP_EVT_SCAN_STOP_IND
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_ext_scan_stop(void);
#endif

/**
 * @fn        uint16_t rtk_bt_le_gap_connect(rtk_bt_le_create_conn_param_t *p_conn_param)
 * @brief     Start connection, will cause event @ref RTK_BT_LE_GAP_EVT_CONNECT_IND
 * @param[in] p_conn_param: Connection parmaters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_connect(rtk_bt_le_create_conn_param_t *p_conn_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_disconnect(uint16_t conn_handle)
 * @brief     Start disconnection, will cause event @ref RTK_BT_LE_GAP_EVT_DISCONN_IND
 * @param[in] conn_handle: The connection handle need to disconnect.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_disconnect(uint16_t conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_gap_update_conn_param(rtk_bt_le_update_conn_param_t *p_update_conn_param)
 * @brief     Update connection paramters. If controller update the connection parameters,
 *            will cause event @ref RTK_BT_LE_GAP_EVT_CONN_UPDATE_IND
 * @param[in] p_update_conn_param: Updated connection parmaters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_update_conn_param(rtk_bt_le_update_conn_param_t *p_update_conn_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_modify_whitelist(rtk_bt_le_modify_wl_param_t *p_wl_op_param)
 * @brief     Modify device white list, will cause event @ref RTK_BT_LE_GAP_EVT_WHITELIST_MODIFY_IND
 * @param[in] p_wl_op_param: Modification operation paramters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_modify_whitelist(rtk_bt_le_modify_wl_param_t *p_wl_op_param);

/**
 * @fn        uint16_t rtk_bt_le_gap_read_rssi(uint16_t conn_handle, int8_t *p_rssi)
 * @brief     Read specific connection RSSI.
 * @param[in] conn_handle: Handle of connection to Read RSSI
 * @param[out] p_rssi: RSSI value
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_read_rssi(uint16_t conn_handle, int8_t *p_rssi);

/**
 * @fn        uint16_t rtk_bt_le_gap_get_dev_state(rtk_bt_le_gap_dev_state_t *p_dev_state)
 * @brief     Get gap dev state.
 * @param[out] p_dev_state: Deice state
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_dev_state(rtk_bt_le_gap_dev_state_t *p_dev_state);

/**
 * @fn        rtk_bt_le_gap_get_active_conn(rtk_bt_le_get_active_conn_t *p_active_conn)
 * @brief     Get active connections.
 * @param[out] p_active_conn: Connection handle
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_active_conn(rtk_bt_le_get_active_conn_t *p_active_conn);

/**
 * @fn        uint16_t rtk_bt_le_gap_get_conn_info(uint16_t conn_handle, rtk_bt_le_conn_info_t *p_conn_info)
 * @brief     Get connection information.
 * @param[in] conn_handle: Connection handle
 * @param[out] p_conn_info: Connection information
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_conn_info(uint16_t conn_handle, rtk_bt_le_conn_info_t *p_conn_info);

/**
 * @fn        uint16_t rtk_bt_le_gap_get_mtu_size(uint16_t conn_handle, uint16_t *p_mtu_size)
 * @brief     Get connection information.
 * @param[in] conn_handle: Connection handle
 * @param[out] p_mtu_size: Exchanged MTU size
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_mtu_size(uint16_t conn_handle, uint16_t *p_mtu_size);

/**
 * @fn        uint16_t rtk_bt_le_gap_get_active_conn(rtk_bt_le_get_active_conn_t *p_active_conn)
 * @brief     Get active connection number and their handles.
 * @param[out] p_active_conn: Active connection number and handles
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_active_conn(rtk_bt_le_get_active_conn_t *p_active_conn);

/**
 * @fn        uint16_t rtk_bt_le_gap_get_conn_handle_by_addr(rtk_bt_le_addr_t *p_addr, uint16_t *p_conn_handle)
 * @brief     Get active connection handle by address.
 * @param[in]  p_addr: Remote device's address
 * @param[out] p_conn_handle: Connection handle
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_get_conn_handle_by_addr(rtk_bt_le_addr_t *p_addr, uint16_t *p_conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_channels(uint8_t *p_chan_map)
 * @brief     Set Bluetooth device channel map.
 * @param[in] p_chan_map: Channel map, size is @ref RTK_BT_LE_CHAN_MAP_LEN
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_channels(uint8_t *p_chan_map);

#if defined(RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT) && RTK_BLE_4_2_DATA_LEN_EXT_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_gap_set_data_len(rtk_bt_le_set_datalen_param_t *p_datalen_param)
 * @brief     Set Bluetooth device connection data length paramters. If controller change the data len,
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
 * @fn        uint16_t rtk_bt_le_gap_set_phy(rtk_bt_le_set_phy_param_t *p_phy_param)
 * @brief     Set Bluetooth device PHY paramters. If controller updates the phy, 
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
 * @fn        uint16_t rtk_bt_le_gap_set_privacy(uint8_t enable)
 * @brief     Enable or disbale privacy.
 * @param[in] enable: Flag of enable
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_privacy(uint8_t enable);

/**
 * @fn        uint16_t rtk_bt_le_gap_set_privacy_mode(rtk_bt_le_set_privacy_mode_param_t *p_privacy_mode_param)
 * @brief     Set privacy mode in one resolving list.
 * @param[in] p_privacy_mode_param: Privacy mode parameters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_gap_set_privacy_mode(rtk_bt_le_set_privacy_mode_param_t *p_privacy_mode_param);
#endif

/**
 * @fn        uint16_t rtk_bt_le_sm_set_security_param(rtk_bt_le_security_param_t *p_sec_param)
 * @brief     Set security paramters.
 * @param[in] p_sec_param: Security paramters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_set_security_param(rtk_bt_le_security_param_t *p_sec_param);

/**
 * @fn        uint16_t rtk_bt_le_sm_start_security(uint16_t conn_handle)
 * @brief     Start security. If security is completed, will cause event @ref RTK_BT_LE_GAP_EVT_AUTH_COMPLETE_IND
 * @param[in] conn_handle: Handle of connection to start security
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_start_security(uint16_t conn_handle);

/**
 * @fn        uint16_t rtk_bt_le_sm_pairing_confirm(rtk_bt_le_pair_cfm_t *p_pair_cfm)
 * @brief     Confirm to continue pairing when use just work.
 * @param[in] p_pair_cfm: Pairing confirm parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_pairing_confirm(rtk_bt_le_pair_cfm_t *p_pair_cfm);

/**
 * @fn        uint16_t rtk_bt_le_sm_passkey_entry(rtk_bt_le_auth_key_input_t *p_key_input)
 * @brief     Input the authentication key.
 * @param[in] p_key_input: Auth key input parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_passkey_entry(rtk_bt_le_auth_key_input_t *p_key_input);

/**
 * @fn        uint16_t rtk_bt_le_sm_passkey_confirm(rtk_bt_le_auth_key_confirm_t *p_key_cfm)
 * @brief     Confirm the authentication key.
 * @param[in] p_key_cfm: Auth key confirmation parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_passkey_confirm(rtk_bt_le_auth_key_confirm_t *p_key_cfm);

#if defined(RTK_BLE_SMP_OOB_SUPPORT) && RTK_BLE_SMP_OOB_SUPPORT
/**
 * @fn        uint16_t rtk_bt_le_sm_set_oob_tk(rtk_bt_le_set_oob_key_t *p_set_oob_key)
 * @brief     Set OOB data.
 * @param[in] p_set_oob_key: OOB key data
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_set_oob_tk(rtk_bt_le_set_oob_key_t *p_set_oob_key);
#endif

/**
 * @fn        uint16_t rtk_bt_le_sm_get_bond_num(uint8_t *bond_num)
 * @brief     Get the number of bonded device.
 * @param[out] bond_num: The number of bonded device
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_get_bond_num(uint8_t *bond_num);

/**
 * @fn        uint16_t rtk_bt_le_sm_get_bond_info(rtk_bt_le_bond_info_t *p_bond_info, uint8_t *size)
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
 * @fn        uint16_t rtk_bt_le_sm_delete_bond_device(rtk_bt_le_addr_t *paddr)
 * @brief     Delete bonded device by address, will cause event @ref RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND
 * @param[in] paddr: Bonded device's address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_delete_bond_device(rtk_bt_le_addr_t *paddr);

/**
 * @fn        uint16_t rtk_bt_le_sm_clear_bond_list(void)
 * @brief     Clear bonded list, will cause event @ref RTK_BT_LE_GAP_EVT_BOND_MODIFY_IND
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_le_sm_clear_bond_list(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_LE_GAP_H__ */
