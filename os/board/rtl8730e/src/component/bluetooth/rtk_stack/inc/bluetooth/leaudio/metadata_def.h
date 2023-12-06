/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _METADATA_DEF_H_
#define _METADATA_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/**
 * \defgroup    LEA_GAF_METADATA Metadata
 *
 * \brief   Metadata LTV structures.
 */

/**
 * \defgroup    METADATA_Exported_Macros Metadata Exported Macros
 *
 * \ingroup LEA_GAF_METADATA
 * \{
 */

/**
 * metadata_def.h
 *
 * \defgroup    METADATA_LTV_STRUCTURE Metadata LTV Structure Type
 *
 * \brief  Define Metadata LTV Structure Type.
 *
 * \ingroup METADATA_Exported_Macros
 * \{
 */
#define METADATA_TYPE_PREFERRED_AUDIO_CONTEXTS 0x01     /**< Preferred_Audio_Contexts metadata LTV structure. 
                                                             The Preferred_Audio_Contexts LTV structure is typically included
                                                             in the Metadata field of PAC records exposed by Unicast Server
                                                             and Broadcast Sinks.*/
#define METADATA_TYPE_STREAMING_AUDIO_CONTEXTS 0x02     /**< Streaming_Audio_Contexts metadata LTV structure.
                                                             The Streaming_Audio_Contexts LTV structure is typically included in
                                                             a Metadata parameter value when initiating the Enable or Update 
                                                             Metadata ASE Control operations for unicast Audio Streams,or in the
                                                             Metadata parameter value included in a BASE structure for broadcast
                                                             Audio Streams. */
#define METADATA_TYPE_PROGRAM_INFO             0x03     /**< Program_Info.
                                                             Title and/or summary of Audio Stream content: UTF-8 format. */
#define METADATA_TYPE_LANGUAGE                 0x04     /**< Language.
                                                             3-byte, lower case language code as defined in ISO 639-3. */
#define METADATA_TYPE_CCID_LIST                0x05     /**< CCID_LIST metadata LTV structure. Array of CCID values. */
#define METADATA_TYPE_PARENTAL_RATING          0x06     /**< Parental_Rating. */
#define METADATA_TYPE_PROGRAM_INFO_URI         0x07     /**< Program_Info_URI.
                                                             A UTF-8 formatted URL link used to present more information about
                                                             Program_Info. */
#define METADATA_TYPE_EXTENDED                 0xFE     /**< Extended Metadata. */
#define METADATA_TYPE_VENDOR_SPECIFIC          0xFF     /**< Vendor_Specific. */
/**
 * End of METADATA_LTV_STRUCTURE
 * \}
 */

/**
 * metadata_def.h
 *
 * \defgroup    AUDIO_CONTEXT Audio Context Type
 *
 * \brief  Define Audio Context Type.
 *
 * \ingroup METADATA_Exported_Macros
 * \{
 */
#define AUDIO_CONTEXT_PROHIBITED           0x0000
#define AUDIO_CONTEXT_UNSPECIFIED          0x0001     /**< Unspecified. */
#define AUDIO_CONTEXT_CONVERSATIONAL       0x0002     /**< Conversational. Conversation between humans. */
#define AUDIO_CONTEXT_MEDIA                0x0004     /**< Media. */
#define AUDIO_CONTEXT_GAME                 0x0008     /**< Game. Audio associated with video gaming. */
#define AUDIO_CONTEXT_INSTRUCTIONAL        0x0010     /**< Instructional. Instructional audio. */
#define AUDIO_CONTEXT_VOICE_ASSISTANTS     0x0020     /**< Voice assistants. Man-machine communication. */
#define AUDIO_CONTEXT_LIVE                 0x0040     /**< Live. Live audio */
#define AUDIO_CONTEXT_SOUND_EFFECTS        0x0080     /**< Sound effects. Sound effects including keyboard and touch feedback;
                                                           menu and user interface sounds; and other system sounds. */
#define AUDIO_CONTEXT_NOTIFICATIONS        0x0100     /**< Notifications. Notification and reminder sounds; attention-seeking audio. */
#define AUDIO_CONTEXT_RINGTONE             0x0200     /**< Ringtone. Alerts the user to an incoming call. */
#define AUDIO_CONTEXT_ALERTS               0x0400     /**< Alerts. Alarms and timers; immediate alerts. */
#define AUDIO_CONTEXT_EMERGENCY_ALERT      0x0800     /**< Emergency alarm. Emergency alarm Emergency sounds. */
#define AUDIO_CONTEXT_MASK                 0x0FFF
/**
 * End of AUDIO_CONTEXT
 * \}
 */

/**
 * End of METADATA_Exported_Macros
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
