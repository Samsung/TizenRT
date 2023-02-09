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

#define RTK_BT_GAP_DEVICE_NAME_LEN      (39+1)//!< Max length of device name, if device name length exceeds it, it will be truncated.

/* ------------------------------- Data Types ------------------------------- */
typedef enum
{
    RTK_BT_BR_ADDR_TYPE_PUBLIC = 0x00,        /*!< Public Device Address */
    RTK_BT_BR_ADDR_TYPE_RANDOM = 0x01,        /*!< Random Device Address */
    RTK_BT_BR_ADDR_TYPE_RPA_PUBLIC = 0x02,    /*!< Public Identity Address (RPA) */
    RTK_BT_BR_ADDR_TYPE_RPA_RANDOM = 0x03,    /*!< Random (static) Identity Address (RPA)*/
    RTK_BT_BR_ADDR_TYPE_PEER_ANONYMOUS = 0xFF,/*!< Anonymous Device Address */
} rtk_bt_br_addr_type_t;

typedef enum
{
	RTK_BT_BR_GAP_PAGE_SCAN_TYPE_STANDARD  = 0,/*!< BR/EDR page scan type standard. */
	RTK_BT_BR_GAP_PAGE_SCAN_TYPE_INTERLACED,   /*!< BR/EDR page scan type interlaced. */
} rtk_bt_br_page_scan_t;

typedef enum
{
	RTK_BT_BR_GAP_INQUIRY_MODE_STANDARD_RESULT = 0, /*!< Inquiry reusult standard format. */
	RTK_BT_BR_GAP_INQUIRY_MODE_RESULT_WITH_RSSI,    /*!< Inquiry result format with RSSI. */
	RTK_BT_BR_GAP_INQUIRY_MODE_EXTENDED_RESULT,     /*!< Inquiry result with RSSI format or Extended inquiry result format. */
} rtk_bt_br_inquiry_mode_t;

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
	void				*a2dp_track_handle;
	void				*sco_track_handle;
	bool				is_streaming;
	uint8_t				avrcp_play_status;
	uint16_t			handle;
	uint16_t			sco_handle;
	uint32_t			connected_profile;
	uint8_t				a2dp_codec_type;
	uint8_t				hfp_codec_type;
	uint8_t				streaming_fg;
	uint16_t 			rfc_spp_frame_size;			/**< spp tx/rx mtu size */
	uint8_t				rfc_spp_credit; 			/**< remained spp tx credit */
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
	union {
		struct {
			uint8_t 	channel_mode;
			int 		sample_rate;
			uint8_t 	bitpool;
			uint8_t 	allocation_method;
			uint8_t 	subbands;
			uint8_t 	block_length;
		} msbc;
		struct {
			uint8_t 	channel_num;
			int 		sample_rate;
			uint8_t 	frame_duration;
		} cvsd;
	} hfp_sco_codec_info;
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
	uint8_t		pagescan_type;                      /*!< page scan type */
	uint16_t	pagescan_interval;                  /*!< page scan interval */
	uint16_t	pagescan_window;                    /*!< page scan window */
	uint16_t	page_timeout;                       /*!< page scan timeout */
} rtk_bt_br_page_param_t;

/**
 * @struct    rtk_bt_br_inquiry_param_t
 * @brief     Bluetooth BR/EDR Inquiry Parameter.
 */
typedef struct
{
	uint8_t		inquiry_mode;                       /*!< inquiry scan mode */
	uint8_t		inquiryscan_type;                   /*!< inquiry scan type */
	uint16_t	inquiryscan_window;                 /*!< inquiry scan window */
	uint16_t	inquiryscan_interval;               /*!< inquiry scan interval */
} rtk_bt_br_inquiry_param_t;

/**
 * @struct    rtk_bt_br_inquiry_start_t
 * @brief     Bluetooth BR/EDR start Inquiry Parameter.
 */
typedef struct
{
	bool		limited_inquiry;                    /*!< limited inquiry indication */
	uint8_t		timeout;                            /*!< Inquiry duration in 1.28s. The real inquiry time is timeout*1.28s */
} rtk_bt_br_inquiry_start_t;

/**
 * @struct    rtk_bt_br_inquiry_result_t
 * @brief     Bluetooth BR/EDR Inquiry Result.(Copy of T_BT_EVENT_PARAM_INQUIRY_RESULT)
 */
typedef struct
{
	uint8_t		bd_addr[6];
	uint32_t	cod;
	int8_t		rssi;
	uint8_t		name[RTK_BT_GAP_DEVICE_NAME_LEN];
	uint8_t		eir_len;
	uint8_t		*p_eir;
} rtk_bt_br_inquiry_result_t;


/* ------------------------------ Functions Declaration ------------------------------ */
/**
 * @defgroup  bt_br_gap BT BR GAP APIs
 * @brief     BT BR GAP related function APIs
 * @ingroup   BT_APIs
 * @{
 */

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

/**
 * @fn        uint16_t rtk_bt_br_gap_start_inquiry(rtk_bt_br_inquiry_start_t *inquiry_start_param)
 * @brief     start inquiry.
 * @param[in] inquiry_start_param: inquiry start parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_start_inquiry(rtk_bt_br_inquiry_start_t *inquiry_start_param);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_pincode(uint8_t *pin_code, uint32_t length)
 * @brief     set pincode.
 * @param[in] pin_code: pointer of pin code
 * @param[in] length: length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_pincode(uint8_t *pin_code, uint32_t length);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_cod(uint32_t *bt_cod)
 * @brief     set bt code of class(should be invoked before rtk_bt_enable).
 * @param[in] bt_cod: pointer of cod
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_cod(uint32_t *bt_cod);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_supvisiontimeout(uint16_t *supv_timeout)
 * @brief     set supvisiontimeout(should be invoked before rtk_bt_enable).
 * @param[in] pin_code: pointer of supvisiontimeout
 * @param[in] length: length
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_supvisiontimeout(uint16_t *supv_timeout);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_BR_GAP_H__ */
