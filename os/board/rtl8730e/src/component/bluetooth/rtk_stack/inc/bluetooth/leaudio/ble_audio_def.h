/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BLE_AUDIO_DEF_H_
#define _BLE_AUDIO_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

/**
 * \defgroup    LEA_LIB BLE Audio Lib
 *
 * \brief   General interface accessing to LE Audio Lib.
 */

/**
 * \defgroup    LEA_LIB_Exported_Macros BLE Audio Lib Exported Macros
 *
 * \ingroup LEA_LIB
 * \{
 */

/**
 * ble_audio_def.h
 *
 * \brief  Define Broadcast ID and Code Length.
 *
 * \ingroup LEA_LIB_Exported_Macros
 */
#define BROADCAST_ID_LEN                      3      /**< Length of Broadcast ID */
#define BROADCAST_CODE_LEN                    16     /**< Length of Broadcast Code */

/**
 * ble_audio_def.h
 *
 * \brief  Define Broadcast Name AD Type.
 *
 * \ingroup LEA_LIB_Exported_Macros
 */
#define GAP_ADTYPE_RSI                        0x2E
#define GAP_ADTYPE_BROADCAST_NAME             0x30

/**
 * ble_audio_def.h
 *
 * \brief  Define Broadcast and Basic Audio Announcement Service UUID.
 *
 * \ingroup LEA_LIB_Exported_Macros
 */
#define BROADCAST_AUDIO_ANNOUNCEMENT_SRV_UUID 0x1852
#define BASIC_AUDIO_ANNOUNCEMENT_SRV_UUID     0x1851

/**
 * ble_audio_def.h
 *
 * \brief  Define Audio Default Presentation Delay.
 *
 * \ingroup LEA_LIB_Exported_Macros
 */
#define AUDIO_DEFAULT_PRESENTATION_DELAY      40000

/**
 * ble_audio_def.h
 *
 * \brief  Define Advertising Announcements Type.
 *
 * \ingroup LEA_LIB_Exported_Macros
 */
#define ADV_TARGETED_ANNOUNCEMENT             0x01
#define ADV_GENERAL_ANNOUNCEMENT              0x00

/**
 * ble_audio_def.h
 *
 * \defgroup AUDIO_LOC Audio Locations
 *
 * \brief  Define Audio Locations.
 *
 * \ingroup LEA_LIB_Exported_Macros
 * \{
 */
#define AUDIO_LOCATION_MONO                0x00000000     /**< No Specified Channel Location */
#define AUDIO_LOCATION_FL                  0x00000001     /**< Front Left */
#define AUDIO_LOCATION_FR                  0x00000002     /**< Front Right */
#define AUDIO_LOCATION_FC                  0x00000004     /**< Front Center */
#define AUDIO_LOCATION_LFE1                0x00000008     /**< Low Frequency Effects 1 */
#define AUDIO_LOCATION_BL                  0x00000010     /**< Back Left */
#define AUDIO_LOCATION_BR                  0x00000020     /**< Back Right */
#define AUDIO_LOCATION_FLC                 0x00000040     /**< Front Left of Center */
#define AUDIO_LOCATION_FRC                 0x00000080     /**< Front Right of Center */
#define AUDIO_LOCATION_BC                  0x00000100     /**< Back Center */
#define AUDIO_LOCATION_LFE2                0x00000200     /**< Low Frequency Effects 2 */
#define AUDIO_LOCATION_SIL                 0x00000400     /**< Side Left */
#define AUDIO_LOCATION_SIR                 0x00000800     /**< Side Right */
#define AUDIO_LOCATION_TPFL                0x00001000     /**< Top Front Left */
#define AUDIO_LOCATION_TPFR                0x00002000     /**< Top Front Right */
#define AUDIO_LOCATION_TPFC                0x00004000     /**< Top Front Center */
#define AUDIO_LOCATION_TPC                 0x00008000     /**< Top Center */
#define AUDIO_LOCATION_TPBL                0x00010000     /**< Top Back Left */
#define AUDIO_LOCATION_TPBR                0x00020000     /**< Top Back Right */
#define AUDIO_LOCATION_TPSIL               0x00040000     /**< Top Side Left */
#define AUDIO_LOCATION_TPSIR               0x00080000     /**< Top Side Right */
#define AUDIO_LOCATION_TPBC                0x00100000     /**< Top Back Center */
#define AUDIO_LOCATION_BTFC                0x00200000     /**< Bottom Front Center */
#define AUDIO_LOCATION_BTFL                0x00400000     /**< Bottom Front Left */
#define AUDIO_LOCATION_BTFR                0x00800000     /**< Bottom Front Right */
#define AUDIO_LOCATION_FLW                 0x01000000     /**< Front Left Wide */
#define AUDIO_LOCATION_FRW                 0x02000000     /**< Front Right Wide */
#define AUDIO_LOCATION_LS                  0x04000000     /**< Left Surround */
#define AUDIO_LOCATION_RS                  0x08000000     /**< Right Surround */
#define AUDIO_LOCATION_RFU                 0xF0000000
#define AUDIO_LOCATION_MASK                0x0FFFFFFF
/**
 * End of AUDIO_LOC
 * \}
 */

/**
 * End of LEA_LIB_Exported_Macros
 * \}
 */

/**
 * \defgroup    LEA_LIB_Exported_Types BLE Audio Lib Exported Types
 *
 * \ingroup LEA_LIB
 * \{
 */

/**
 * ble_audio_def.h
 *
 * \brief  Audio Direction.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef enum
{
    SERVER_AUDIO_SINK = 0x01,
    SERVER_AUDIO_SOURCE = 0x02,
} T_AUDIO_DIRECTION;

/**
 * ble_audio_def.h
 *
 * \brief  Audio Input Types.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef enum
{
    AUDIO_INPUT_UNSPECIFIED         = 0x00,     /**< Unspecified Input */
    AUDIO_INPUT_BLUETOOTH           = 0x01,     /**< Bluetooth Audio Stream */
    AUDIO_INPUT_MICROPHONE          = 0x02,     /**< Microphone */
    AUDIO_INPUT_ANALOG              = 0x03,     /**< Analog Interface */
    AUDIO_INPUT_DIGITAL             = 0x04,     /**< Digital Interface */
    AUDIO_INPUT_RADIO               = 0x05,     /**< AM/FM/XM/etc. */
    AUDIO_INPUT_STREAMING           = 0x06,     /**< Streaming Audio Source */
} T_AUDIO_INPUT_TYPE;
/**
 * End of LEA_LIB_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
