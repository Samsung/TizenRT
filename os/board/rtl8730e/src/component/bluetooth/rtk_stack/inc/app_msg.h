/*
 * Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef _APP_MSG_H_
#define _APP_MSG_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2019

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup APP_MSG APP Message
 * @brief message definition for user application task.
 * @{
 */

/**
 * @defgroup APP_MSG_Exported_Types APP Message Exported Types
 * @{
 */

/**
 * @brief     Event group definitions.
 * @details   event code is 1 byte code to define messages type exchanging from/to different layers.
 *            first half byte is for group define, and second harf byte is for code define
 *            group code:
 *            0: from stack layer
 *            0x1: from peripheral layer
 *            0x2: from audio lib
 *            0xA: from instructment
 */
typedef enum {
	EVENT_GROUP_STACK = 0,        /**< message group from BT layer */
	EVENT_GROUP_IO = 1,           /**< message group from IO layer */
	EVENT_GROUP_AUDIOLIB = 2,     /**< message group from Audio lib layer */
	EVENT_GROUP_INSTRUMENT = 0xa, /**< message group from instructment layer */
} T_EVENT_GROUP;

/**  @brief     Event type definitions.
*/
typedef enum {
	EVENT_GAP_MSG = 0x01,  /**< message from gap layer for stack */
	EVENT_GAP_TIMER = 0x02, /**< message from gap layer for timer */

	EVENT_IO_TO_APP = 0x11, /**< message from IO to user application */

	EVENT_RWS_TIMER = 0x21,  /**< message from RWS timer to user application */
	EVENT_DSP_MSG = 0x22,    /**< message from DSP to user application */

	EVENT_CMD_FROM_8852B = 0xa1,  /**< message from 8852B to user application */

	EVENT_USER_HTTP_SERVER = 0xa2,  /**< message from user http server */

	EVENT_USER_API_REQ = 0xa3,  /**< message from user user api */
} T_EVENT_TYPE;

/** @brief get event code group definition */
#define EVENT_GROUP(event_code) (event_code >> 4)

/**  @brief IO type definitions for IO message, may extend as requested */
typedef enum {
	IO_MSG_TYPE_BT_STATUS,  /**< BT status change with subtype @ref GAP_MSG_TYPE */
	IO_MSG_TYPE_KEYSCAN,    /**< Key scan message with subtype @ref T_IO_MSG_KEYSCAN */
	IO_MSG_TYPE_QDECODE,    /**< subtype to be defined */
	IO_MSG_TYPE_UART,       /**< Uart message with subtype @ref T_IO_MSG_UART */
	IO_MSG_TYPE_KEYPAD,     /**< subtype to be defined */
	IO_MSG_TYPE_IR,         /**< subtype to be defined */
	IO_MSG_TYPE_GDMA,       /**< subtype to be defined */
	IO_MSG_TYPE_ADC,        /**< subtype to be defined */
	IO_MSG_TYPE_D3DG,       /**< subtype to be defined */
	IO_MSG_TYPE_SPI,        /**< subtype to be defined */
	IO_MSG_TYPE_MOUSE_BUTTON,   /**< subtype to be defined */
	IO_MSG_TYPE_GPIO,       /**< Gpio message with subtype @ref T_IO_MSG_GPIO*/
	IO_MSG_TYPE_MOUSE_SENSOR,   /**< subtype to be defined */
	IO_MSG_TYPE_TIMER,      /**< App timer message with subtype @ref T_IO_MSG_TIMER */
	IO_MSG_TYPE_WRISTBNAD,  /**< wristband message with subtype @ref T_IO_MSG_WRISTBAND */
	IO_MSG_TYPE_MESH_STATUS,    /**< subtype to be defined */
	IO_MSG_TYPE_KEYBOARD_BUTTON, /**< subtype to be defined */
	IO_MSG_TYPE_ANCS,            /**< ANCS message*/
	IO_MSG_TYPE_AT_CMD,     /**< AT cmd message*/
	IO_MSG_TYPE_API_SYS_CALL,
	IO_MSG_TYPE_ADV,
} T_IO_MSG_TYPE;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_KEYSCAN type */
typedef enum {
	IO_MSG_KEYSCAN_RX_PKT        = 1, /**< Keyscan RX data event */
	IO_MSG_KEYSCAN_MAX           = 2, /**<  */
	IO_MSG_KEYSCAN_ALLKEYRELEASE = 3, /**< All keys are released event */
	IO_MSG_KEYSCAN_STUCK         = 4, /**<  key stuck message */
} T_IO_MSG_KEYSCAN;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_UART type */
typedef enum {
	IO_MSG_UART_RX              = 0x10, /**< UART RX event */
	IO_MSG_UART_RX_TIMEOUT      = 0x11, /**< UART RX TIMEOUT event */
	IO_MSG_UART_RX_OVERFLOW     = 0x12, /**< UART RX OVERFLOW event */
	IO_MSG_UART_RX_EMPTY        = 0x13, /**< UART RX EMPTY event */
	IO_MSG_UART_TX              = 0x20, /**< UART TX event */
} T_IO_MSG_UART;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_GPIO type */
typedef enum {
	IO_MSG_GPIO_KEY,               /**< KEY GPIO event */
	IO_MSG_GPIO_LINE_IN,           /**< LINE IN event */
	IO_MSG_GPIO_NFC,               /**< NFC event */
	IO_MSG_GPIO_UART_WAKE_UP,      /**< UART WAKE UP event */
	IO_MSG_GPIO_CHARGER,           /**< CHARGER event */
	IO_MSG_GPIO_USB_AUDIO_PLUG,    /**< USB PLUG IN event */
	IO_MSG_GPIO_USB_AUDIO_UNPLUG   /**< USB PLUG OUT event */
} T_IO_MSG_GPIO;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_TIMER type */
typedef enum {
	IO_MSG_TIMER_ALARM,
	IO_MSG_TIMER_RWS
} T_IO_MSG_TIMER;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_WRISTBNAD type */
typedef enum {
	IO_MSG_BWPS_TX_VALUE            = 1,
	IO_MSG_OS_TIMEROUT_SEND_DATA    = 2,
	IO_MSG_OS_TIMEROUT_SEND_DELAY   = 3,
	IO_MSG_OS_TIMEROUT_SENSOR       = 4,
	IO_MSG_RTC_TIMEROUT_WALL_CLOCK  = 5,
	IO_MSG_SENSOR_DOUBLE_CLICK      = 6,
	IO_MSG_ADC_INT                  = 7,
	IO_MSG_SENSOR_INT               = 8,
	IO_MSG_SENSOR_WAKE_UP           = 9,
	IO_MSG_LED_TWINKLE              = 10,
	IO_MSG_MOTOR_VIBRATE            = 11,
	IO_MSG_CHARGER_STATE            = 12,
	IO_MSG_RTC_LOW_BATTERY_RESTORE  = 13,
	IO_MSG_RTC_ALARM                = 14,
	IO_MSG_ADC_SAMPLE_READY         = 15,
	IO_MSG_GDMA_RECV                = 16,
	IO_MSG_SENSOR_LEN               = 17,
	IO_MSG_BUTTON_EVENT             = 18,
	IO_MSG_POWER_OFF                = 19,
	IO_MSG_POWER_ON                 = 20,
	IO_MSG_HRS_EVENT                = 21,
	IO_MSG_SENSOR_MOTION_INTERRUPT  = 22,
	IO_MSG_UART_CMD_DEBUG           = 23,
	IO_MSG_OTA_GDMA_READ            = 24,
	IO_MSG_OTA_GDMA_WRITE           = 25,
	IO_MSG_HRS_TIMEOUT_HANDLE       = 26,
	IO_MSG_UPDATE_CONPARA           = 27,
	IO_MSG_WAS_RX_VALUE             = 28,
	IO_MSG_WAS_ENABLE_CCCD          = 29,
	IO_MSG_WAS_TX_VALUE             = 30,
} T_IO_MSG_WRISTBAND;

/**  @brief IO message definition for communications between tasks*/
typedef struct {
	uint16_t type;
	uint16_t subtype;
	union {
		uint32_t  param;
		void     *buf;
	} u;
} T_IO_MSG;

/** @} */ /* End of group APP_MSG_Exported_Types */

/** @} */ /* End of group APP_MSG */

#ifdef __cplusplus
}
#endif

#elif UPPER_STACK_VERSION == VERSION_2021

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @defgroup APP_MSG APP Message
 * @brief message definition for user application task.
 * @{
 */

/**
 * @defgroup APP_MSG_Exported_Types APP Message Exported Types
 * @{
 */

/**
 * @brief     Event group definitions.
 * @details   event code is 1 byte code to define messages type exchanging from/to different layers.
 *            first half byte is for group define, and second harf byte is for code define
 *            group code:
 *            0: from stack layer
 *            0x1: from peripheral layer
 *            0x2: from audio lib
 *            0xA: from instructment
 */
typedef enum
{
    EVENT_GROUP_STACK = 0,        /**< message group from BT layer */
    EVENT_GROUP_IO = 1,           /**< message group from IO layer */
    EVENT_GROUP_FRAMEWORK = 2,     /**< message group from Framework layer */
    EVENT_GROUP_INSTRUMENT = 0xa, /**< message group from instructment layer */
} T_EVENT_GROUP;

/**  @brief     Event type definitions.
*/
typedef enum
{
    EVENT_GAP_MSG = 0x01,  /**< message from gap layer for stack */
    EVENT_GAP_TIMER = 0x02, /**< message from gap layer for timer */
    EVENT_HCI_MSG   = 0x03, /**< message from HCI layer for test */

    EVENT_IO_TO_APP = 0x11, /**< message from IO to user application */

    EVENT_SYS_MSG = 0x21,   /**< message from SYS timer to user application */
    EVENT_DSP_MSG = 0x22,    /**< message from DSP to user application */
    EVENT_CODEC_MSG = 0x23,  /**< message from CODEC to user application */

    EVENT_MESH = 0x80,

    EVENT_CMD_FROM_8852B = 0xa1,  /**< message from 8852B to user application */
    EVENT_USER_API_REQ = 0xa3,  /**< message from user api */
} T_EVENT_TYPE;

/** @brief get event code group definition */
#define EVENT_GROUP(event_code) (event_code >> 4)

/**  @brief IO type definitions for IO message, may extend as requested */
typedef enum
{
    IO_MSG_TYPE_BT_STATUS,  /**< BT status change with subtype @ref GAP_MSG_TYPE */
    IO_MSG_TYPE_KEYSCAN,    /**< Key scan message with subtype @ref T_IO_MSG_KEYSCAN */
    IO_MSG_TYPE_QDECODE,    /**< subtype to be defined */
    IO_MSG_TYPE_UART,       /**< Uart message with subtype @ref T_IO_MSG_UART */
    IO_MSG_TYPE_KEYPAD,     /**< subtype to be defined */
    IO_MSG_TYPE_IR,         /**< subtype to be defined */
    IO_MSG_TYPE_GDMA,       /**< subtype to be defined */
    IO_MSG_TYPE_ADC,        /**< subtype to be defined */
    IO_MSG_TYPE_D3DG,       /**< subtype to be defined */
    IO_MSG_TYPE_SPI,        /**< subtype to be defined */
    IO_MSG_TYPE_MOUSE_BUTTON,   /**< subtype to be defined */
    IO_MSG_TYPE_GPIO,       /**< Gpio message with subtype @ref T_IO_MSG_GPIO*/
    IO_MSG_TYPE_MOUSE_SENSOR,   /**< subtype to be defined */
    IO_MSG_TYPE_TIMER,      /**< App timer message with subtype @ref T_IO_MSG_TIMER */
    IO_MSG_TYPE_WRISTBNAD,  /**< wristband message with subtype @ref T_IO_MSG_WRISTBAND */
    IO_MSG_TYPE_MESH_STATUS,    /**< subtype to be defined */
    IO_MSG_TYPE_KEYBOARD_BUTTON, /**< subtype to be defined */
    IO_MSG_TYPE_ANCS,            /**< ANCS message*/
    IO_MSG_TYPE_CONSOLE,    /**< Console message with subtype @ref T_IO_CONSOLE */
    IO_MSG_TYPE_LE_MGR,
    IO_MSG_TYPE_LE_AUDIO,
    IO_MSG_TYPE_AT_CMD,     /**< AT cmd message*/
    IO_MSG_TYPE_API_SYS_CALL, /**< API Sys calling message */
    IO_MSG_TYPE_ADV,
    IO_MSG_TYPE_LE_MESH,
} T_IO_MSG_TYPE;

/**  @brief IO subtype definitions for @ref T_IO_CONSOLE type */
typedef enum
{
    IO_MSG_CONSOLE_STRING_RX    = 0x01, /**< Console CLI RX event */
    IO_MSG_CONSOLE_STRING_TX    = 0x02, /**< Console CLI TX event */
    IO_MSG_CONSOLE_BINARY_RX    = 0x03, /**< Console protocol RX event */
    IO_MSG_CONSOLE_BINARY_TX    = 0x04, /**< Console protocol TX event */
} T_IO_CONSOLE;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_KEYSCAN type */
typedef enum
{
    IO_MSG_KEYSCAN_RX_PKT        = 1, /**< Keyscan RX data event */
    IO_MSG_KEYSCAN_MAX           = 2, /**<  */
    IO_MSG_KEYSCAN_ALLKEYRELEASE = 3, /**< All keys are released event */
    IO_MSG_KEYSCAN_STUCK         = 4, /**<  key stuck message */
} T_IO_MSG_KEYSCAN;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_UART type */
typedef enum
{
    IO_MSG_UART_RX              = 0x10, /**< UART RX event */

    IO_MSG_UART_TX              = 0x20, /**< UART TX event */
} T_IO_MSG_UART;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_GPIO type */
typedef enum
{
    IO_MSG_GPIO_KEY,               /**< KEY GPIO event */
    IO_MSG_GPIO_UART_WAKE_UP,      /**< UART WAKE UP event */
    IO_MSG_GPIO_CHARGER,           /**< CHARGER event */
    IO_MSG_GPIO_NFC,               /**< NFC event */
    IO_MSG_GPIO_USB_AUDIO_PLUG,    /**< USB PLUG IN event */
    IO_MSG_GPIO_USB_AUDIO_UNPLUG,  /**< USB PLUG OUT event */
    IO_MSG_GPIO_ADAPTOR_PLUG,         /**< ADAPTOR PLUG IN event */
    IO_MSG_GPIO_ADAPTOR_UNPLUG,       /**< ADAPTOR PLUG OUT event */
    IO_MSG_GPIO_CHARGERBOX_DETECT,     /**< CHARGERBOX DETECT event*/
    IO_MSG_GPIO_USB_AUDIO_CHECK,    /**< USB CHECK event*/
    IO_MSG_GPIO_GSENSOR,            /**< Gsensor detect event*/
    IO_MSG_GPIO_SENSOR_LD,            /**< sensor light detect event*/
} T_IO_MSG_GPIO;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_TIMER type */
typedef enum
{
    IO_MSG_TIMER_ALARM,
    IO_MSG_TIMER_RWS
} T_IO_MSG_TIMER;

/**  @brief IO subtype definitions for @ref IO_MSG_TYPE_WRISTBNAD type */
typedef enum
{
    IO_MSG_BWPS_TX_VALUE            = 1,
    IO_MSG_OS_TIMEROUT_SEND_DATA    = 2,
    IO_MSG_OS_TIMEROUT_SEND_DELAY   = 3,
    IO_MSG_OS_TIMEROUT_SENSOR       = 4,
    IO_MSG_RTC_TIMEROUT_WALL_CLOCK  = 5,
    IO_MSG_SENSOR_DOUBLE_CLICK      = 6,
    IO_MSG_ADC_INT                  = 7,
    IO_MSG_SENSOR_INT               = 8,
    IO_MSG_SENSOR_WAKE_UP           = 9,
    IO_MSG_LED_TWINKLE              = 10,
    IO_MSG_MOTOR_VIBRATE            = 11,
    IO_MSG_CHARGER_STATE            = 12,
    IO_MSG_RTC_LOW_BATTERY_RESTORE  = 13,
    IO_MSG_RTC_ALARM                = 14,
    IO_MSG_ADC_SAMPLE_READY         = 15,
    IO_MSG_GDMA_RECV                = 16,
    IO_MSG_SENSOR_LEN               = 17,
    IO_MSG_BUTTON_EVENT             = 18,
    IO_MSG_POWER_OFF                = 19,
    IO_MSG_POWER_ON                 = 20,
    IO_MSG_HRS_EVENT                = 21,
    IO_MSG_SENSOR_MOTION_INTERRUPT  = 22,
    IO_MSG_UART_CMD_DEBUG           = 23,
    IO_MSG_OTA_GDMA_READ            = 24,
    IO_MSG_OTA_GDMA_WRITE           = 25,
    IO_MSG_HRS_TIMEOUT_HANDLE       = 26,
    IO_MSG_UPDATE_CONPARA           = 27,
} T_IO_MSG_WRISTBAND;

/**  @brief IO message definition for communications between tasks*/
typedef struct
{
    uint16_t type;
    uint16_t subtype;
    union
    {
        uint32_t  param;
        void     *buf;
    } u;
} T_IO_MSG;


/** @} */ /* End of group APP_MSG_Exported_Types */

/** @} */ /* End of group APP_MSG */

#ifdef __cplusplus
}
#endif

#endif

#endif /* _APP_MSG_H_ */
