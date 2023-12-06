/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CODEC_DEF_H_
#define _CODEC_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/**
 * \defgroup    LEA_GAF_CODEC Codec
 *
 * \brief   Codec LTV structures.
 */

/**
 * \defgroup    CODEC_Exported_Macros Codec Exported Macros
 *
 * \ingroup LEA_GAF_CODEC
 * \{
 */

/**
 * codec_def.h
 *
 * \defgroup    CODEC_ID_LEN Codec ID and Codec ID Length
 *
 * \brief  Define Codec ID and Codec ID Length.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define TRANSPARENT_CODEC_ID               0x03
#define LC3_CODEC_ID                       0x06
#define VENDOR_CODEC_ID                    0xFF
#define CODEC_ID_LEN                       5
/**
 * End of CODEC_ID_LEN
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    CODEC_CAP_TYPE Codec Specific Capability Type
 *
 * \brief  Define Codec Specific Capability Types.
 *         The Codec_Specific_Capabilities parameters field.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define CODEC_CAP_TYPE_SUPPORTED_SAMPLING_FREQUENCIES   0x01
#define CODEC_CAP_TYPE_SUPPORTED_FRAME_DURATIONS        0x02
#define CODEC_CAP_TYPE_AUDIO_CHANNEL_COUNTS             0x03
/*4 octets.
Octet 0-1: Minimum number of octets supported per codec frame
Octet 2-3: Maximum number of octets supported per codec frame*/
#define CODEC_CAP_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME 0x04
#define CODEC_CAP_TYPE_MAX_SUPPORTED_FRAMES_PER_SDU     0x05
/**
 * End of CODEC_CAP_TYPE
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    SUP_SAMPLING_FREQ Supported Sampling Frequency
 *
 * \brief  Define Supported Sampling Frequencies.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define SAMPLING_FREQUENCY_8K              0x0001      /**< 8 KHz */
#define SAMPLING_FREQUENCY_11K             0x0002      /**< 11 KHz */
#define SAMPLING_FREQUENCY_16K             0x0004      /**< 16 KHz */
#define SAMPLING_FREQUENCY_22K             0x0008      /**< 22 KHz */
#define SAMPLING_FREQUENCY_24K             0x0010      /**< 24 KHz */
#define SAMPLING_FREQUENCY_32K             0x0020      /**< 32 KHz */
#define SAMPLING_FREQUENCY_44_1K           0x0040      /**< 44.1 KHz */
#define SAMPLING_FREQUENCY_48K             0x0080      /**< 48 KHz */
#define SAMPLING_FREQUENCY_88K             0x0100      /**< 88 KHz */
#define SAMPLING_FREQUENCY_96K             0x0200      /**< 96 KHz */
#define SAMPLING_FREQUENCY_176K            0x0400      /**< 176 KHz */
#define SAMPLING_FREQUENCY_192K            0x0800      /**< 192 KHz */
#define SAMPLING_FREQUENCY_384K            0x1000      /**< 384 KHz */
#define SAMPLING_FREQUENCY_MASK            0x1FFF
/**
 * End of SUP_SAMPLING_FREQ
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    SUP_FRAME_DURA Supported Frame Durations
 *
 * \brief  Define Supported Frame Durations.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define FRAME_DURATION_7_5_MS_BIT           0x01
#define FRAME_DURATION_10_MS_BIT            0x02
#define FRAME_DURATION_PREFER_7_5_MS_BIT    0x10
#define FRAME_DURATION_PREFER_10_MS_BIT     0x20
/**
 * End of SUP_FRAME_DURA
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    SUP_CHANN_COUNT Supported Frame Durations
 *
 * \brief  Define Supported Audio Channel Counts.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define AUDIO_CHANNEL_COUNTS_1             0x01
#define AUDIO_CHANNEL_COUNTS_2             0x02
#define AUDIO_CHANNEL_COUNTS_3             0x04
#define AUDIO_CHANNEL_COUNTS_4             0x08
#define AUDIO_CHANNEL_COUNTS_5             0x10
#define AUDIO_CHANNEL_COUNTS_6             0x20
#define AUDIO_CHANNEL_COUNTS_7             0x40
#define AUDIO_CHANNEL_COUNTS_8             0x80
/**
 * End of SUP_CHANN_COUNT
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    CODEC_CONFIG_TYPE Codec Configuration Type
 *
 * \brief  Define Codec Configuration Types.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define CODEC_CFG_TYPE_SAMPLING_FREQUENCY       0x01      /**< Sample Frequency */
#define CODEC_CFG_TYPE_FRAME_DURATION           0x02      /**< Frame Duration */
#define CODEC_CFG_TYPE_AUDIO_CHANNEL_ALLOCATION 0x03      /**< Audio Channel Allocation */
#define CODEC_CFG_TYPE_OCTET_PER_CODEC_FRAME    0x04      /**< Octet preferred Codec Frame */
#define CODEC_CFG_TYPE_BLOCKS_PER_SDU           0x05      /**< Block preferred SDU */
/**
 * End of CODEC_CONFIG_TYPE
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    SAMPLING_FREQ_CONFIG Sampling Frequency Configuration
 *
 * \brief  Define Sampling Frequency Configurations.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define SAMPLING_FREQUENCY_CFG_8K              0x01          /**< 8 KHz */
#define SAMPLING_FREQUENCY_CFG_11K             0x02          /**< 11 KHz */
#define SAMPLING_FREQUENCY_CFG_16K             0x03          /**< 16 KHz */
#define SAMPLING_FREQUENCY_CFG_22K             0x04          /**< 22 KHz */
#define SAMPLING_FREQUENCY_CFG_24K             0x05          /**< 24 KHz */
#define SAMPLING_FREQUENCY_CFG_32K             0x06          /**< 32 KHz */
#define SAMPLING_FREQUENCY_CFG_44_1K           0x07          /**< 44.1 KHz */
#define SAMPLING_FREQUENCY_CFG_48K             0x08          /**< 48 KHz */
#define SAMPLING_FREQUENCY_CFG_88K             0x09          /**< 88 KHz */
#define SAMPLING_FREQUENCY_CFG_96K             0x0A          /**< 96 KHz */
#define SAMPLING_FREQUENCY_CFG_176K            0x0B          /**< 176 KHz */
#define SAMPLING_FREQUENCY_CFG_192K            0x0C          /**< 192 KHz */
#define SAMPLING_FREQUENCY_CFG_384K            0x0D          /**< 384 KHz */
/**
 * End of SAMPLING_FREQ_CONFIG
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    FRAME_DURA_CONFIG Frame Duration Configuration
 *
 * \brief  Define Frame Duration Configurations.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define FRAME_DURATION_CFG_7_5_MS              0x00
#define FRAME_DURATION_CFG_10_MS               0x01
/**
 * End of FRAME_DURA_CONFIG
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    CODEC_CAP_EXIST Codec Specific Capabilities Exist Flag
 *
 * \brief  Define CAP Codec Specific Capabilities Exist Flags.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define CODEC_CAP_SUPPORTED_SAMPLING_FREQUENCIES_EXIST   0x0001
#define CODEC_CAP_SUPPORTED_FRAME_DURATIONS_EXIST        0x0002
#define CODEC_CAP_AUDIO_CHANNEL_COUNTS_EXIST             0x0004
#define CODEC_CAP_SUPPORTED_OCTETS_PER_CODEC_FRAME_EXIST 0x0008
#define CODEC_CAP_MAX_SUPPORTED_FRAMES_PER_SDU_EXIST     0x0010
/**
 * End of CODEC_CAP_EXIST
 * \}
 */

/**
 * codec_def.h
 *
 * \defgroup    CODEC_CONFIG_EXIST Codec Configuration Exist Flag
 *
 * \brief  Define Codec Configuration Exist Flags.
 *
 * \ingroup CODEC_Exported_Macros
 * \{
 */
#define CODEC_CFG_SAMPLING_FREQUENCY_EXIST       0x0001
#define CODEC_CFG_FRAME_DURATION_EXIST           0x0002
#define CODEC_CFG_AUDIO_CHANNEL_ALLOCATION_EXIST 0x0004
#define CODEC_CFG_OCTET_PER_CODEC_FRAME_EXIST    0x0008
#define CODEC_CFG_TYPE_BLOCKS_PER_SDU_EXIST      0x0010
/**
 * End of CODEC_CONFIG_EXIST
 * \}
 */

/**
 * End of CODEC_Exported_Macros
 * \}
 */

/**
 * \defgroup    CODEC_Exported_Types Codec Exported Types
 *
 * \ingroup LEA_GAF_CODEC
 * \{
 */

/**
 * codec_def.h
 *
 * \brief  CAP Codec Specific Capabilities Data.
 *
 * \ingroup CODEC_Exported_Types
 */
typedef struct
{
    uint16_t type_exist;
    uint16_t supported_sampling_frequencies;
    uint8_t  supported_frame_durations;
    uint8_t  audio_channel_counts;
    uint8_t  max_supported_codec_frames_per_sdu;
    uint16_t min_octets_per_codec_frame;
    uint16_t max_octets_per_codec_frame;
} T_CODEC_CAP;

/**
 * codec_def.h
 *
 * \brief  Codec Configuration Data.
 *
 * \ingroup CODEC_Exported_Types
 */
typedef struct
{
    uint16_t type_exist;
    uint8_t  frame_duration;
    uint8_t  sample_frequency;
    uint8_t  codec_frame_blocks_per_sdu;
    uint16_t octets_per_codec_frame;
    uint32_t audio_channel_allocation;
} T_CODEC_CFG;
/**
 * End of CODEC_Exported_Types
 * \}
 */

/**
 * \defgroup    CODEC_Exported_Functions Codec Exported Functions
 *
 * \ingroup LEA_GAF_CODEC
 * \{
 */

/**
 * codec_def.h
 *
 * \brief  Codec configuration parse.
 *
 * \param[in]  value_len  The value length of p_value.
 * \param[in]  p_value    Pointer to codec configuration data.
 * \param[in,out]  p_cfg  Pointer to codec configuration parsed data: @ref T_CODEC_CFG.
 *
 * \return         The result of codec configuration parse.
 * \retval true    Codec configuration parse success.
 * \retval false   Codec configuration parse failed.
 *
 * \ingroup CODEC_Exported_Functions
 */
bool codec_cfg_parse(uint8_t value_len, uint8_t *p_value, T_CODEC_CFG *p_cfg);

/**
 * codec_def.h
 *
 * \brief  Generate codec specific configuration data.
 *
 * \param[in,out]  p_value_len Pointer to codec data length.
 * \param[in,out]  p_value     Pointer to codec data.
 * \param[in]  p_cfg           Pointer to codec configuration: @ref T_CODEC_CFG.
 *
 * \return         The result of generate codec specific configuration data.
 * \retval true    Generate codec specific configuration data success.
 * \retval false   Generate codec specific configuration data failed.
 *
 * \ingroup CODEC_Exported_Functions
 */
bool codec_cfg_gen(uint8_t *p_value_len, uint8_t *p_value, T_CODEC_CFG *p_cfg);
/**
 * End of CODEC_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
