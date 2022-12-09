/**
* @file      rtk_bt_br_gap.h
* @author    benling_xu@realsil.com.cn
* @brief     Bluetooth LE GAP part type and function deffinition
* @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
*/

#ifndef __RTK_BT_BR_GAP_H__
#define __RTK_BT_BR_GAP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup APP_MAIN_Exported_Macros App Main Macros
	* @{
	*/
#define UART_BUD_RX     (P3_0)

#define RWS_PRIMARY_VALID_OK             0x01
#define RWS_SECONDARY_VALID_OK           0x02

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif
#define MAX_NUMBER_OF_GAP_TIMER         0x10    //!< indicate gap timer queue size
/** max BR/EDR link num */
#define MAX_BR_LINK_NUM                 1
/** bitmask of profiles */
#define A2DP_PROFILE_MASK               0x00000001    /**< A2DP profile bitmask */
#define AVRCP_PROFILE_MASK              0x00000002    /**< AVRCP profile bitmask */
#define HFP_PROFILE_MASK                0x00000004    /**< HFP profile bitmask */
#define RDTP_PROFILE_MASK               0x00000008    /**< Remote Control vendor profile bitmask */
#define SPP_PROFILE_MASK                0x00000010    /**< SPP profile bitmask */
#define IAP_PROFILE_MASK                0x00000020    /**< iAP profile bitmask */
#define PBAP_PROFILE_MASK               0x00000040    /**< PBAP profile bitmask */
#define HSP_PROFILE_MASK                0x00000080    /**< HSP profile bitmask */
#define GATT_PROFILE_MASK               0x00000100    /**< GATT profile bitmask */
#define GFPS_PROFILE_MASK               0x00010000    /**< GFPS profile bitmask */
#define XIAOAI_PROFILE_MASK             0x00020000    /**< XIAOAI profile bitmask */
#define AMA_PROFILE_MASK                0x00040000    /**< AMA profile bitmask */
#define AVP_PROFILE_MASK                0x00080000    /**< AVP profile bitmask */

#define ALL_PROFILE_MASK                (A2DP_PROFILE_MASK | AVRCP_PROFILE_MASK | HFP_PROFILE_MASK | \
										HSP_PROFILE_MASK | RDTP_PROFILE_MASK | SPP_PROFILE_MASK | \
										IAP_PROFILE_MASK | PBAP_PROFILE_MASK)

#define DEFAULT_SUPVISIONTIMEOUT        0x1f40 //0x7D00
#define RTK_BD_ADDR_LEN 6

/* ------------------------------- Data Types ------------------------------- */
typedef enum
{
    RTK_BT_BR_ADDR_TYPE_PUBLIC = 0x00,        /*!< Public Device Address */
    RTK_BT_BR_ADDR_TYPE_RANDOM = 0x01,        /*!< Random Device Address */
    RTK_BT_BR_ADDR_TYPE_RPA_PUBLIC = 0x02,    /*!< Public Identity Address (RPA) */
    RTK_BT_BR_ADDR_TYPE_RPA_RANDOM = 0x03,    /*!< Random (static) Identity Address (RPA)*/
    RTK_BT_BR_ADDR_TYPE_PEER_ANONYMOUS = 0xFF,/*!< Anonymous Device Address */
} rtk_bt_br_addr_type_t;

/**
 * @typedef   rtk_bt_bd_addr_t
 * @brief     Bluetooth device address type definition.
 */
typedef struct {
	uint8_t addr[RTK_BD_ADDR_LEN];
} rtk_bt_bd_addr_t;

/**  @brief  APP's Bluetooth BR/EDR link connection database */
typedef struct {
	void				*timer_handle_role_switch;
	void				*timer_handle_later_avrcp;
	uint8_t				bd_addr[6];
	bool				used;
	uint8_t				id;
	uint32_t			connected_profile;
	uint8_t				a2dp_codec_type;
	union {
		struct {
			uint8_t 	sampling_frequency;
			uint8_t 	channel_mode;
			uint8_t 	block_length;
			uint8_t 	subbands;
			uint8_t 	allocation_method;
			uint8_t 	min_bitpool;
			uint8_t 	max_bitpool;
		} sbc;
		struct {
			uint8_t 	object_type;
			uint16_t 	sampling_frequency;
			uint8_t 	channel_number;
			bool 		vbr_supported;
			uint32_t 	bit_rate;
		} aac;
		struct {
			uint8_t 	info[12];
		} vendor;
	} a2dp_codec_info;

	uint8_t				streaming_fg;
	uint8_t				avrcp_play_status;

} T_APP_BR_LINK;

typedef struct {
	T_APP_BR_LINK		br_link[MAX_BR_LINK_NUM];
	rtk_bt_bd_addr_t	factory_addr;               /**< local factory address */
} T_APP_DB;

typedef struct {
	uint16_t mtu_size;                              /*!< MTU size */
} rtk_bt_br_gap_default_param_t;

/**
 * @struct    rtk_bt_br_bd_addr_t
 * @brief     Bluetooth BR/EDR device address type definition.
 */
typedef struct
{
	rtk_bt_br_addr_type_t 	type;                   /*!< address type */
	rtk_bt_bd_addr_t 		bd_addr;                /*!< address */
} rtk_bt_br_bd_addr_t;

/**
 * @struct    rtk_bt_br_page_param_t
 * @brief     Bluetooth BR/EDR Page Parameter.
 */
typedef struct
{
	uint8_t pagescan_type;                           /*!< page scan type */
	uint16_t pagescan_interval;                      /*!< page scan interval */
	uint16_t pagescan_window;                        /*!< page scan window */
	uint16_t page_timeout;                           /*!< page scan timeout */
} rtk_bt_br_page_param_t;

/**
 * @struct    rtk_bt_br_inquiry_param_t
 * @brief     Bluetooth BR/EDR Inquiry Parameter.
 */
typedef struct
{
	uint8_t inquiry_mode;                             /*!< inquiry scan mode */
	uint8_t inquiryscan_type;                         /*!< inquiry scan type */
	uint16_t inquiryscan_window;                      /*!< inquiry scan window */
	uint16_t inquiryscan_interval;                    /*!< inquiry scan interval */
} rtk_bt_br_inquiry_param_t;

/**
 * @fn        uint16_t rtk_bt_br_gap_set_default_param(rtk_bt_br_gap_default_param_t *param)
 * @brief     Set BR/EDR default parameter.
 * @param[in] param: BR/EDR parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_default_param(rtk_bt_br_gap_default_param_t *param);

/**
 * @fn        uint16_t rtk_bt_br_gap_get_address(rtk_bt_br_bd_addr_t *paddr)
 * @brief     Get local device's address.
 * @param[out] paddr: Device address under use
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_get_address(rtk_bt_br_bd_addr_t *paddr);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_device_name(const uint8_t *name)
 * @brief     Set bluetooth Device name.
 * @param[in] name: Device name
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_device_name(const uint8_t *name);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_page_param(rtk_bt_br_page_param_t *page_param)
 * @brief     Set paging parameter.
 * @param[in] page_param: page parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_page_param(rtk_bt_br_page_param_t *page_param);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_inquiry_param(rtk_bt_br_inquiry_param_t *inquiry_param)
 * @brief     Set inquiry parameter.
 * @param[in] inquiry_param: inquiry parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_inquiry_param(rtk_bt_br_inquiry_param_t *inquiry_param);

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_BR_GAP_H__ */
