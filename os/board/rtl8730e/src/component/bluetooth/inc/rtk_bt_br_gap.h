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
 * @typedef   rtk_bt_br_io_cap_t
 * @brief     Bluetooth BREDR SM IO capability type definition.
 */
typedef enum {
	RTK_BR_IO_CAP_DISPALY_ONLY     = 0x00,     /*!< 0x00 DisplayOnly */
	RTK_BR_IO_CAP_DISPLAY_YES_NO   = 0x01,     /*!< 0x01 DisplayYesNo */
	RTK_BR_IO_CAP_KEYBOARD_ONLY    = 0x02,     /*!< 0x02 KeyboardOnly */
	RTK_BR_IO_CAP_NO_IN_NO_OUT     = 0x03,     /*!< 0x03 NoInputNoOutput */
	RTK_BR_IO_CAP_KEYBOARD_DISPALY = 0x04,     /*!< 0x04 KeyboardDisplay */
	RTK_BR_IO_CAP_MAX              = 0x05,     /*!< 0x05-0xFF Reserved for future use */
} rtk_bt_br_io_cap_t;

typedef enum
{
	RTK_BT_BR_GAP_RADIO_MODE_VISIBLE_CONNECTABLE  = 0x01,  /*!< Enable both inquiry scan and page scan. */
	RTK_BT_BR_GAP_RADIO_MODE_VISIBLE              = 0x02,  /*!< Only enable inquiry scan. */
	RTK_BT_BR_GAP_RADIO_MODE_CONNECTABLE          = 0x03,  /*!< Only enable page scan. */
	RTK_BT_BR_GAP_RADIO_MODE_NONE_DISCOVERABLE    = 0x04,  /*!< Disable both inquiry scan and page scan. */
} rtk_bt_br_gap_radio_mode_t;

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
	uint16_t			remote_hfp_brsf_capability;
	bool				is_inband_ring;
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
 * @struct    rtk_bt_br_sniff_mode_t
 * @brief     BR/EDR sniff mode parameters.
 */
typedef struct
{
	uint8_t		enable;                            /*!< 0 for disable, 1 for enable */
	uint8_t		bd_addr[6];                        /*!< address */
	uint16_t	min_interval;                      /*!< Min sniff interval, only even values between 0x0002 and 0xFFFE are valid */
	uint16_t	max_interval;                      /*!< Max sniff interval, only even values between 0x0002 and 0xFFFE are valid, should be larger than min_interval */
	uint16_t	sniff_attempt;                     /*!< Number of baseband receive slots for sniff attempt */
	uint16_t	sniff_timeout;                     /*!< Number of baseband receive slots for sniff timeout */
} rtk_bt_br_sniff_mode_t;

/**
 * @struct    rtk_bt_br_security_param_t
 * @brief     Bluetooth BREDR SM security parameter type definition.
 */
typedef struct {
	rtk_bt_br_io_cap_t io_cap;      /*!< IO capabilities */
	uint8_t oob_data_flag;          /*!< OOB data flag */
	uint8_t bond_flag;              /*!< Bonding flags */
	uint8_t br_bond_flag;           /*!< BR general bonding support flag */
	uint8_t mitm_flag;              /*!< MITM flag */
	uint8_t sec_pair_flag;          /*!< Secure connection pairing support flag */
	uint8_t br_sec_pair_flag;       /*!< BR secure connection pairing support flag */
} rtk_bt_br_security_param_t;

/**
 * @struct    rtk_bt_br_acl_sniff_t
 * @brief     indicate acl sniff mode.
 */
typedef struct
{
	uint8_t		bd_addr[6];                        /*!< address */
	uint16_t	interval;                          /*!< sniff mode interval */
} rtk_bt_br_acl_sniff_t;

/**
 * @struct    rtk_bt_br_acl_disc_t
 * @brief     acl disconnection event.
 */
typedef struct
{
	uint8_t		bd_addr[6];                        /*!< address */
	uint16_t	cause;                             /*!< disc cause */
} rtk_bt_br_acl_disc_t;

/**
 * @struct    rtk_bt_br_bond_key_t
 * @brief     bond key set.
 */
typedef struct
{
	uint8_t		bd_addr[6];                        /*!< address */
	uint8_t		link_key[16];                      /*!< link key */
	uint8_t		key_type;                          /*!< key_type */
} rtk_bt_br_bond_key_t;

/**
 * @struct    rtk_bt_br_bond_addr_t
 * @brief     bond address get.
 */
typedef struct
{
	uint8_t		bd_addr[6];                        /*!< address */
	uint8_t		priority;                          /*!< priority of bonded device.Bond priority number starts at 1, rather than 0. Bond priority is descending
                                                    with the corresponding number increasing.*/
} rtk_bt_br_bond_addr_t;

/**
 * @struct    rtk_bt_br_bond_index_t
 * @brief     bond index get.
 */
typedef struct
{
	uint8_t		bd_addr[6];                        /*!< address */
	uint8_t		index;                             /*!< bond index */
} rtk_bt_br_bond_index_t;

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

/**
 * @struct    rtk_bt_br_inquiry_result_t
 * @brief     Bluetooth BR/EDR Inquiry Result.(Copy of T_BT_EVENT_PARAM_INQUIRY_RESULT)
 */
typedef struct
{
	uint8_t		bd_addr[6];
	uint16_t	cause;
	uint8_t		name[RTK_BT_GAP_DEVICE_NAME_LEN];
} rtk_bt_br_remote_name_rsp_t;

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
 * @brief     Set paging parameter(Init page parameters).
 * @param[in] page_param: page parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_page_param(rtk_bt_br_page_param_t *page_param);

/**
 * @fn        uint16_t rtk_bt_br_gap_cfg_page_param(uint8_t type, uint16_t interval, uint16_t window)
 * @brief     Config paging parameter(after init).
 * @param[in] type: Page scan type value to set(0 -> STAND, 1 -> INTERLACED)
 * @param[in] interval: Page scan interval value to set.
 * @param[in] window: Page scan window value to set.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_cfg_page_param(uint8_t type, uint16_t interval, uint16_t window);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_inquiry_param(rtk_bt_br_inquiry_param_t *inquiry_param)
 * @brief     Set inquiry parameter(Init inquiry parameters)
 * @param[in] inquiry_param: inquiry parameter
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_inquiry_param(rtk_bt_br_inquiry_param_t *inquiry_param);

/**
 * @fn        uint16_t rtk_bt_br_gap_cfg_inquiry_param(uint8_t type, uint16_t interval, uint16_t window)
 * @brief     Config inquiry parameter(after init).
 * @param[in] type: Inquiry scan type value to set(0 -> STAND, 1 -> INTERLACED)
 * @param[in] interval: Inquiry scan interval value to set.
 * @param[in] window: Inquiry scan window value to set.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_cfg_inquiry_param(uint8_t type, uint16_t interval, uint16_t window);

/**
 * @fn        uint16_t rtk_bt_br_sm_set_security_param(rtk_bt_br_security_param_t *p_sec_param)
 * @brief     Set security paramters.
 * @param[in] p_sec_param: Security paramters
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_sm_set_security_param(rtk_bt_br_security_param_t *p_sec_param);

/**
 * @fn        uint16_t rtk_bt_br_gap_disconnect(uint8_t *bd_addr)
 * @brief     Disconnect bt br link.
 * @param[in] bd_addr: The Bluetooth device address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_disconnect(uint8_t *bd_addr);

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
 * @fn        uint16_t rtk_bt_br_gap_get_remote_name(uint8_t *bd_addr)
 * @brief     get remote device name.(Return to br gap callback)
 * @param[in] bd_addr: The Bluetooth device address
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_get_remote_name(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_br_gap_max_bond_num_get(uint8_t *pmax_bond_num)
 * @brief     get supported max bond number.
 * @param[in] pmax_bond_num: max bond number
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_max_bond_num_get(uint8_t *pmax_bond_num);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_num_get(uint8_t *pbond_num)
 * @brief     get used bond number.
 * @param[in] pmax_bond_num: bond number
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_num_get(uint8_t *pbond_num);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_addr_get(uint8_t priority, uint8_t *bd_addr)
 * @brief     get bond address ().
 * @param[in] priority: The priority of bonded device.
 * @param[in] bd_addr   The Bluetooth device address got by priority.
 * \note    Bond priority number starts at 1, rather than 0. Bond priority is descending
 *          with the corresponding number increasing.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_addr_get(uint8_t priority, uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_index_get(uint8_t *pindex, uint8_t *bd_addr)
 * @brief     Get bond index from device address.
 * @param[in] pindex: The bond index.Bond index starts at 0.
 * @param[in] bd_addr   The Bluetooth device address got by priority.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_index_get(uint8_t *pindex, uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_key_get(uint8_t *bd_addr, uint8_t *key_type, uint8_t *bond_key)
 * @brief     Get link key with Bluetooth device address from bond storage.
 * @param[in] bd_addr   The Bluetooth device address got by priority.
 * @param[in] key_type   The type of the key.
 * @param[in] bond_key   The link key.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_key_get(uint8_t *bd_addr, uint8_t *key_type, uint8_t *bond_key);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_key_set(rtk_bt_br_bond_key_t *pbond_key_t)
 * @brief     Save link key.
 * @param[in] pbond_key_t   bond key struct.
 *\note    bd_addr   The Bluetooth device address got by priority.
           key_type   The type of the key.
           bond_key   The link key.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_key_set(rtk_bt_br_bond_key_t *pbond_key_t);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_priority_set(uint8_t *bd_addr)
 * @brief     Adjust priority to highest in bond storage.
 * @param[in] bd_addr   The Bluetooth device address got by priority.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_priority_set(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_delete(uint8_t *bd_addr)
 * @brief     Delete bond information in bond storage.
 * @param[in] bd_addr   The Bluetooth device address got by priority.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_delete(uint8_t *bd_addr);

/**
 * @fn        uint16_t rtk_bt_br_gap_bond_clear()
 * @brief     delete bond information in bond storage
 * @param[in] void: NUll
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_bond_clear(void);

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
 * @fn        uint16_t rtk_bt_br_gap_set_radio_mode(uint8_t radio_mode)
 * @brief     set radio mode(rtk_bt_br_gap_radio_mode_t). 
 * @param[in] radio_mode: radio mode
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_radio_mode(uint8_t radio_mode);

/**
 * @fn        uint16_t rtk_bt_br_gap_set_sniff_mode(uint8_t *bd_addr, uint16_t min_interval, uint16_t max_interval, uint16_t sniff_attempt, uint16_t sniff_timeout)
 * @brief     configure sniff mode.
 * @param[in] enable: 0 for disable, 1 for enable.
 * @param[in] bd_addr The Bluetooth device address 
 * @param[in] min_interval: (0 for disable)Min sniff interval, only even values between 0x0002 and 0xFFFE are valid.
 * @param[in] max_interval: (0 for disable)Max sniff interval, only even values between 0x0002 and 0xFFFE are valid, should be larger than min_interval.
 * @param[in] sniff_attempt: (0 for disable)Number of baseband receive slots for sniff attempt.
 * @param[in] sniff_timeout: (0 for disable)Number of baseband receive slots for sniff timeout.
 * @return    
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_br_gap_set_sniff_mode(uint8_t enable, uint8_t *bd_addr, uint16_t min_interval, uint16_t max_interval, uint16_t sniff_attempt, uint16_t sniff_timeout);

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
