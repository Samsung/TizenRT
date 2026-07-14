/**
 *****************************************************************************************
 *               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file     cod.h
 * @brief    Head file for class of device definition
 * @details
 * @author   kyle_xu
 * @date     2015-11-21
 * @version  v1.0
 * ****************************************************************************************
 */
#ifndef __COD_H__
#define __COD_H__


#ifdef  __cplusplus
extern "C" {
#endif

/**
 * \defgroup    COD       COD
 * \brief   Define some macros to make up COD(class of device) for APP to use.
 *
 */

/**
 * cod.h
 *
 * \name    COD_MAJOR_CLASS
 * \brief   Major device class definitions.
 * \anchor  COD_MAJOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MAJOR_DEVICE_CLASS_MISCELLANEOUS          ((uint32_t)0x000000)
#define MAJOR_DEVICE_CLASS_COMPUTER               ((uint32_t)0x000100)
#define MAJOR_DEVICE_CLASS_PHONE                  ((uint32_t)0x000200)
#define MAJOR_DEVICE_CLASS_LAN_ACCESS_POINT       ((uint32_t)0x000300)
#define MAJOR_DEVICE_CLASS_AUDIO                  ((uint32_t)0x000400)
#define MAJOR_DEVICE_CLASS_PERIPHERAL             ((uint32_t)0x000500)
#define MAJOR_DEVICE_CLASS_IMAGING                ((uint32_t)0x000600)
#define MAJOR_DEVICE_CLASS_HEALTH                 ((uint32_t)0x000900)
#define MAJOR_DEVICE_CLASS_UNCLASSIFIED           ((uint32_t)0x001F00)

/**
 * cod.h
 *
 * \name    UNCLASSIFIED_MINOR_CLASS
 * \brief   Unclassified minor device class definition.
 * \anchor  UNCLASSIFIED_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_CLASS_UNCLASSIFIED           ((uint32_t)0x000000)

/**
 * cod.h
 *
 * \name    COMPUTER_MINOR_CLASS
 * \brief   Minor device class definitions for major class COMPUTER.
 * \anchor  COMPUTER_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_CLASS_DESKTOP_WORKSTATION    ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_SERVER_CLASS_COMPUTER  ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_LAPTOP                 ((uint32_t)0x00000C)
#define MINOR_DEVICE_CLASS_HANDHELD               ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_PALM_SIZED             ((uint32_t)0x000014)
#define MINOR_DEVICE_CLASS_WEARABLE_COMPUTER      ((uint32_t)0x000018)

/**
 * cod.h
 *
 * \name    PHONE_MINOR_CLASS
 * \brief   Minor device class definitions for major class PHONE.
 * \anchor  PHONE_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_CLASS_CELLULAR               ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_CORDLESS               ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_SMART_PHONE            ((uint32_t)0x00000C)
#define MINOR_DEVICE_CLASS_WIRED_MODEM            ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_VOICE_GATEWAY          ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_ISDN_GATEWAY           ((uint32_t)0x000014)

/**
 * cod.h
 *
 * \name    LAN_ACCESS_POINT_MINOR_CLASS
 * \brief   Minor device class definitions for major class LAN_ACCESS_POINT.
 * \anchor  LAN_ACCESS_POINT_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_CLASS_FULLY_AVAILABLE        ((uint32_t)0x000000)
#define MINOR_DEVICE_CLASS_1_17_PERCENT_UTILIZED  ((uint32_t)0x000020)
#define MINOR_DEVICE_CLASS_17_33_PERCENT_UTILIZED ((uint32_t)0x000040)
#define MINOR_DEVICE_CLASS_33_50_PERCENT_UTILIZED ((uint32_t)0x000060)
#define MINOR_DEVICE_CLASS_50_67_PERCENT_UTILIZED ((uint32_t)0x000080)
#define MINOR_DEVICE_CLASS_67_83_PERCENT_UTILIZED ((uint32_t)0x0000A0)
#define MINOR_DEVICE_CLASS_83_99_PERCENT_UTILIZED ((uint32_t)0x0000C0)
#define MINOR_DEVICE_CLASS_NO_SERVICE_AVAILABLE   ((uint32_t)0x0000E0)

/**
 * cod.h
 *
 * \name    AV_MINOR_CLASS
 * \brief   Minor device class definitions for major class AUDIO/VIDEO.
 * \anchor  AV_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_CLASS_HEADSET                ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_HANDSFREE              ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_MICROPHONE             ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_LOUDSPEAKER            ((uint32_t)0x000014)
#define MINOR_DEVICE_CLASS_HEADPHONES             ((uint32_t)0x000018)
#define MINOR_DEVICE_CLASS_PORTABLEAUDIOLE        ((uint32_t)0x00001c)
#define MINOR_DEVICE_CLASS_CARAUDIO               ((uint32_t)0x000020)
#define MINOR_DEVICE_CLASS_SETTOPBOX              ((uint32_t)0x000024)
#define MINOR_DEVICE_CLASS_HIFIAUDIO              ((uint32_t)0x000028)
#define MINOR_DEVICE_CLASS_VCR                    ((uint32_t)0x00002c)
#define MINOR_DEVICE_CLASS_VIDEOCAMERA            ((uint32_t)0x000030)
#define MINOR_DEVICE_CLASS_CAMCORDER              ((uint32_t)0x000034)
#define MINOR_DEVICE_CLASS_VIDEOMONITOR           ((uint32_t)0x000038)
#define MINOR_DEVICE_CLASS_VIDEODISPLAYSPEAKER    ((uint32_t)0x00003c)
#define MINOR_DEVICE_CLASS_VIDEOCONFERENCING      ((uint32_t)0x000040)
#define MINOR_DEVICE_CLASS_GAMINGTOY              ((uint32_t)0x000048)

/**
 * cod.h
 *
 * \name    PERIPHERAL_MINOR_CLASS
 * \brief   Minor device class definitions for major class PERIPHERAL.
 * \anchor  PERIPHERAL_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_CLASS_KEYBOARD               ((uint32_t)0x000040)
#define MINOR_DEVICE_CLASS_POINTING_DEVICE        ((uint32_t)0x000080)
#define MINOR_DEVICE_CLASS_COMBO_KEY_POINTING     ((uint32_t)0x0000C0)
#define MINOR_DEVICE_CLASS_PERIMULT_UNCATEGORIZED ((uint32_t)0x000000)
#define MINOR_DEVICE_CLASS_PERIMULT_JOYSTICK      ((uint32_t)0x000004)
#define MINOR_DEVICE_CLASS_PERIMULT_GAMEPAD       ((uint32_t)0x000008)
#define MINOR_DEVICE_CLASS_PERIMULT_REMOTECONTROL ((uint32_t)0x00000C)
#define MINOR_DEVICE_CLASS_PERIMULT_SENSING       ((uint32_t)0x000010)
#define MINOR_DEVICE_CLASS_PERIMULT_DIGITIZER     ((uint32_t)0x000014)
#define MINOR_DEVICE_CLASS_PERIMULT_CARDREADER    ((uint32_t)0x000018)

/**
 * cod.h
 *
 * \name    IMAGING_MINOR_CLASS
 * \brief   Minor device class definitions for major class IMAGING. May be combined use as mask.
 * \anchor  IMAGING_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_IMAGE_DISPLAY                ((uint32_t)0x000010)
#define MINOR_DEVICE_IMAGE_CAMERA                 ((uint32_t)0x000020)
#define MINOR_DEVICE_IMAGE_SCANNER                ((uint32_t)0x000040)
#define MINOR_DEVICE_IMAGE_PRINTER                ((uint32_t)0x000080)

/**
 * cod.h
 *
 * \name    HEALTH_MINOR_CLASS
 * \brief   Minor device class definitions for major class HEALTH.
 * \anchor  HEALTH_MINOR_CLASS
 */
/**
 * \ingroup     COD
 */
#define MINOR_DEVICE_HEALTH_BLOODPRESSURE         ((uint32_t)0x0004)
#define MINOR_DEVICE_HEALTH_THERMOMETER           ((uint32_t)0x0008)
#define MINOR_DEVICE_HEALTH_WEIGHINGSCALE         ((uint32_t)0x000C)
#define MINOR_DEVICE_HEALTH_GLUCOSEMETER          ((uint32_t)0x0010)
#define MINOR_DEVICE_HEALTH_PULSEOXIMETER         ((uint32_t)0x0014)
#define MINOR_DEVICE_HEALTH_HEARTPULSERATE        ((uint32_t)0x0018)
#define MINOR_DEVICE_HEALTH_DATADISPLAY           ((uint32_t)0x001C)
#define MINOR_DEVICE_HEALTH_STEPCOUNTER           ((uint32_t)0x0020)

/**
 * cod.h
 *
 * \name    SERVICE_CLASS
 * \brief   Service class definitions.
 * \anchor  SERVICE_CLASS
 */
/**
 * \ingroup     COD
 */
#define SERVICE_CLASS_LIMITED_DISCOVERABLE_MODE   ((uint32_t)0x002000)
#define SERVICE_CLASS_RESERVED1                   ((uint32_t)0x004000)
#define SERVICE_CLASS_RESERVED2                   ((uint32_t)0x008000)
#define SERVICE_CLASS_POSITIONING                 ((uint32_t)0x010000)
#define SERVICE_CLASS_NETWORKING                  ((uint32_t)0x020000)
#define SERVICE_CLASS_RENDERING                   ((uint32_t)0x040000)
#define SERVICE_CLASS_CAPTURING                   ((uint32_t)0x080000)
#define SERVICE_CLASS_OBJECT_TRANSFER             ((uint32_t)0x100000)
#define SERVICE_CLASS_AUDIO                       ((uint32_t)0x200000)
#define SERVICE_CLASS_TELEPHONY                   ((uint32_t)0x400000)
#define SERVICE_CLASS_INFORMATION                 ((uint32_t)0x800000)

/**
 * cod.h
 *
 * \name    COD_MASK
 * \brief   Device and service bitmask in COD.
 * \anchor  COD_MASK
 */
/**
 * \ingroup     COD
 */
#define SERVICE_CLASS_DEVICE_MASK                 ((uint32_t)0x00001FFF)
#define SERVICE_CLASS_SERVICE_MASK                ((uint32_t)0x00FFE000)

#ifdef __cplusplus
}
#endif
#endif /* __COD_H__ */
