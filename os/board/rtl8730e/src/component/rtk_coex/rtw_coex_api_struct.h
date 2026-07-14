#ifndef RTW_COEX_API_STRUCT_H
#define RTW_COEX_API_STRUCT_H
#include <stdbool.h>
#include <stdint.h>


/* ============================================================================
 * Protocol Bitmap Definitions
 * ============================================================================
 */

/**
 * @enum rtk_coex_protocol
 * @brief Bitmask representing protocols participating in coexistence.
 *
 * Multiple protocols can be combined using bitwise OR.
 */
enum rtk_coex_protocol {
	/** No protocol defined. */
	RTK_COEX_PROTOCOL_UDEF = 0x0,
	/** WiFi protocol (BIT0). */
	RTK_COEX_PROTOCOL_WL   = 1,
	/** Bluetooth protocol (BIT1). */
	RTK_COEX_PROTOCOL_BT   = 1 << 1,
	/** IEEE 802.15.4 MAC protocol (BIT2). */
	RTK_COEX_PROTOCOL_15_4_MAC = 1 << 2,
	/** ZigBee protocol (BIT3). */
	RTK_COEX_PROTOCOL_ZB   = 1 << 3,
	/** OpenThread protocol (BIT4). */
	RTK_COEX_PROTOCOL_OT   = 1 << 4,
	/** Reserved maximum bit. */
	RTK_COEX_PROTOCOL_MAX  = 1 << 15
};

/* ============================================================================
 * PTA Related Enumerations
 * ============================================================================
 */

/**
 * @enum pta_type
 * @brief PTA (Priority Traffic Arbitration) winner selection.
 */
enum pta_type {
	/** PTA winner decided automatically by coexistence logic. */
	PTA_AUTO    = 0,
	/** Force WiFi to win PTA arbitration. */
	PTA_WIFI    = 1,
	/** Force Bluetooth to win PTA arbitration. */
	PTA_BT      = 2,
	/** Disable PTA arbitration completely. */
	PTA_DISABLE = 0xFF,
};

/**
 * @enum pta_host_role
 * @brief Host initiating PTA request.
 */
enum pta_host_role {
	PTA_HOST_WIFI = 0, /**< Request initiated by WiFi */
	PTA_HOST_BT   = 1  /**< Request initiated by Bluetooth */
};

/**
 * @enum pta_process_action
 * @brief PTA operation context or action.
 */
enum pta_process_action {
	/** Normal PTA operation. */
	COMMON_ACTION      = 0,
	/** RF calibration start. */
	CALIBRATION_START  = 1,
	/** RF calibration stop. */
	CALIBRATION_STOP   = 2
};

/* ============================================================================
 * WiFi-Specific Enumerations
 * ============================================================================
 */

/**
 * @enum rtk_coex_wl_spec_packet_evt
 * @brief High-priority WiFi packet events relevant to coexistence.
 */
enum rtk_coex_wl_spec_packet_evt {
	RTK_COEX_WL_PACKET_EVT_NONE  = 0x0,
	RTK_COEX_WL_PACKET_EVT_DHCP  = 0x1,
	RTK_COEX_WL_PACKET_EVT_ARP   = 0x2,
	RTK_COEX_WL_PACKET_EVT_EAPOL = 0x3,
};

/**
 * @enum rtk_coex_wl_band_type
 * @brief WiFi operating band.
 */
enum rtk_coex_wl_band_type {
	RTK_COEX_WL_BAND_2G = 0x0, /**< 2.4 GHz band */
	RTK_COEX_WL_BAND_5G = 0x1  /**< 5 GHz band */
};

/**
 * @enum rtk_coex_wl_c2h_evt
 * @brief Firmware-to-host (C2H) event IDs reserved for coexistence.
 *
 * Range: 0x60 ~ 0x7F
 */
enum rtk_coex_wl_c2h_evt {
	RTK_COEX_C2H_WL2BT_MAILBOX_FAIL = 0x60,
	RTK_COEX_C2H_RCV_BT2WL_SCB      = 0x61,
	RTK_COEX_C2H_BT2WL_MAILBOX      = 0x62,
};

/**
 * @enum rtk_coex_halrf_rfk_process
 * @brief RF calibration (RFK) process types communicated to coexistence module.
 *
 **/
enum rtk_coex_halrf_rfk_process {
	RTK_COEX_WL_RFK_STOP		= 0,
	RTK_COEX_WL_RFK_START		= 1,
	RTK_COEX_WL_RFK_ONESHOT_START	= 2,
	RTK_COEX_WL_RFK_ONESHOT_STOP	= 3
};
#endif /* RTW_COEX_API_STRUCT_H */