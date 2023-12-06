/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _CODEC_QOS_H_
#define _CODEC_QOS_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "codec_def.h"

/**
 * \defgroup    LEA_GAF_Codec_QoS BAP Codec QoS
 *
 * \brief   BAP Codec QoS.
 */

/**
 * \defgroup    Codec_QoS_Exported_Types BAP Codec QoS Exported Types
 *
 * \ingroup LEA_GAF_Codec_QoS
 * \{
 */

/**
 * codec_qos.h
 *
 * \brief  Codec Capability Configuration Items.
 *
 * \ingroup Codec_QoS_Exported_Types
 */
typedef enum
{
    CODEC_CFG_ITEM_8_1 = 0,
    CODEC_CFG_ITEM_8_2 = 1,
    CODEC_CFG_ITEM_16_1 = 2,
    CODEC_CFG_ITEM_16_2 = 3,
    CODEC_CFG_ITEM_24_1 = 4,
    CODEC_CFG_ITEM_24_2 = 5,
    CODEC_CFG_ITEM_32_1 = 6,
    CODEC_CFG_ITEM_32_2 = 7,
    CODEC_CFG_ITEM_441_1 = 8,
    CODEC_CFG_ITEM_441_2 = 9,
    CODEC_CFG_ITEM_48_1 = 10,
    CODEC_CFG_ITEM_48_2 = 11,
    CODEC_CFG_ITEM_48_3 = 12,
    CODEC_CFG_ITEM_48_4 = 13,
    CODEC_CFG_ITEM_48_5 = 14,
    CODEC_CFG_ITEM_48_6 = 15,
    CODEC_CFG_ITEM_LC3_MAX,

    CODEC_CFG_ITEM_VENDOR = 0xff,
} T_CODEC_CFG_ITEM;
/**
 * End of Codec_QoS_Exported_Types
 * \}
 */

/**
 * \defgroup    Codec_QoS_Exported_Macros BAP Codec QoS Exported Macros
 *
 * \ingroup LEA_GAF_Codec_QoS
 * \{
 */

/**
 * codec_qos.h
 *
 * \defgroup    CODEC_CAP_CONFIG_ITEM_BIT Codec Capability Configuration Items Bit Mask
 *
 * \brief  Define Codec Capability Configuration Items Bit Mask.
 *
 * \ingroup Codec_QoS_Exported_Macros
 * \{
 */
#define CODEC_CFG_ITEM_8_1_BIT   (1 << CODEC_CFG_ITEM_8_1)
#define CODEC_CFG_ITEM_8_2_BIT   (1 << CODEC_CFG_ITEM_8_2)
#define CODEC_CFG_ITEM_16_1_BIT  (1 << CODEC_CFG_ITEM_16_1)
#define CODEC_CFG_ITEM_16_2_BIT  (1 << CODEC_CFG_ITEM_16_2)
#define CODEC_CFG_ITEM_24_1_BIT  (1 << CODEC_CFG_ITEM_24_1)
#define CODEC_CFG_ITEM_24_2_BIT  (1 << CODEC_CFG_ITEM_24_2)
#define CODEC_CFG_ITEM_32_1_BIT  (1 << CODEC_CFG_ITEM_32_1)
#define CODEC_CFG_ITEM_32_2_BIT  (1 << CODEC_CFG_ITEM_32_2)
#define CODEC_CFG_ITEM_441_1_BIT (1 << CODEC_CFG_ITEM_441_1)
#define CODEC_CFG_ITEM_441_2_BIT (1 << CODEC_CFG_ITEM_441_2)
#define CODEC_CFG_ITEM_48_1_BIT  (1 << CODEC_CFG_ITEM_48_1)
#define CODEC_CFG_ITEM_48_2_BIT  (1 << CODEC_CFG_ITEM_48_2)
#define CODEC_CFG_ITEM_48_3_BIT  (1 << CODEC_CFG_ITEM_48_3)
#define CODEC_CFG_ITEM_48_4_BIT  (1 << CODEC_CFG_ITEM_48_4)
#define CODEC_CFG_ITEM_48_5_BIT  (1 << CODEC_CFG_ITEM_48_5)
#define CODEC_CFG_ITEM_48_6_BIT  (1 << CODEC_CFG_ITEM_48_6)
/**
 * End of CODEC_CAP_CONFIG_ITEM_BIT
 * \}
 */

/**
 * codec_qos.h
 *
 * \defgroup    SUP_SAMPLING_FREQ_MASK Supported Sampling Frequency Table Mask
 *
 * \brief  Define Supported Sampling Frequency Table Mask.
 *
 * \ingroup Codec_QoS_Exported_Macros
 * \{
 */
#define SAMPLE_FREQ_8K_TABLE_MASK       (CODEC_CFG_ITEM_8_1_BIT|CODEC_CFG_ITEM_8_2_BIT)
#define SAMPLE_FREQ_16K_TABLE_MASK      (CODEC_CFG_ITEM_16_1_BIT|CODEC_CFG_ITEM_16_2_BIT)
#define SAMPLE_FREQ_24K_TABLE_MASK      (CODEC_CFG_ITEM_24_1_BIT|CODEC_CFG_ITEM_24_2_BIT)
#define SAMPLE_FREQ_32K_TABLE_MASK      (CODEC_CFG_ITEM_32_1_BIT|CODEC_CFG_ITEM_32_2_BIT)
#define SAMPLE_FREQ_441K_TABLE_MASK     (CODEC_CFG_ITEM_441_1_BIT|CODEC_CFG_ITEM_441_2_BIT)
#define SAMPLE_FREQ_48K_TABLE_MASK      (CODEC_CFG_ITEM_48_1_BIT|CODEC_CFG_ITEM_48_2_BIT| \
                                         CODEC_CFG_ITEM_48_3_BIT|CODEC_CFG_ITEM_48_4_BIT| \
                                         CODEC_CFG_ITEM_48_5_BIT|CODEC_CFG_ITEM_48_6_BIT)
/**
 * End of SUP_SAMPLING_FREQ_MASK
 * \}
 */

/**
 * codec_qos.h
 *
 * \defgroup    SUP_FRAME_DURA_MASK Supported Frame Duration Table Mask
 *
 * \brief  Define Supported Frame Duration Table Mask.
 *
 * \ingroup Codec_QoS_Exported_Macros
 * \{
 */
#define FRAME_DURATION_7_5M_TABLE_MASK  (CODEC_CFG_ITEM_8_1_BIT|CODEC_CFG_ITEM_16_1_BIT| \
                                         CODEC_CFG_ITEM_24_1_BIT|CODEC_CFG_ITEM_32_1_BIT| \
                                         CODEC_CFG_ITEM_441_1_BIT|CODEC_CFG_ITEM_48_1_BIT| \
                                         CODEC_CFG_ITEM_48_3_BIT|CODEC_CFG_ITEM_48_5_BIT)   /**< 7.5 ms */
#define FRAME_DURATION_10M_TABLE_MASK   (CODEC_CFG_ITEM_8_2_BIT|CODEC_CFG_ITEM_16_2_BIT| \
                                         CODEC_CFG_ITEM_24_2_BIT|CODEC_CFG_ITEM_32_2_BIT| \
                                         CODEC_CFG_ITEM_441_2_BIT|CODEC_CFG_ITEM_48_2_BIT| \
                                         CODEC_CFG_ITEM_48_4_BIT|CODEC_CFG_ITEM_48_6_BIT)   /**< 10 ms */
/**
 * End of SUP_FRAME_DURA_MASK
 * \}
 */

/**
 * End of Codec_QoS_Exported_Macros
 * \}
 */

/**
 * \defgroup    Codec_QoS_Exported_Types BAP Codec QoS Exported Types
 *
 * \ingroup LEA_GAF_Codec_QoS
 * \{
 */

/**
 * codec_qos.h
 *
 * \brief  QoS Configuration Types.
 *
 * \ingroup Codec_QoS_Exported_Types
 */
typedef enum
{
    QOS_CFG_CIS_LOW_LATENCY,
    QOS_CFG_CIS_HIG_RELIABILITY,
    QOS_CFG_BIS_LOW_LATENCY,
    QOS_CFG_BIS_HIG_RELIABILITY,
} T_QOS_CFG_TYPE;

/**
 * codec_qos.h
 *
 * \brief  Preferred QoS Configuration.
 *
 * \ingroup Codec_QoS_Exported_Types
 */
typedef struct
{
    uint32_t sdu_interval;
    uint8_t  framing;
    uint16_t max_sdu;
    uint8_t  retransmission_number;
    uint16_t max_transport_latency;
    uint32_t presentation_delay;
} T_QOS_CFG_PREFERRED;
/**
 * End of Codec_QoS_Exported_Types
 * \}
 */

/**
 * \defgroup    Codec_QoS_Exported_Functions BAP Codec QoS Exported Functions
 *
 * \ingroup LEA_GAF_Codec_QoS
 * \{
 */

/**
 * codec_qos.h
 *
 * \brief  Get preferred codec configuration.
 *
 * \param[in]  item       Codec capability configuration item: @ref T_CODEC_CFG_ITEM.
 * \param[in,out]  p_cfg  Pointer to codec configuration: @ref T_CODEC_CFG.
 *
 * \return         The result of get codec preferred configuration.
 * \retval true    Get codec preferred configuration success.
 * \retval false   Get codec preferred configuration failed.
 *
 * \ingroup Codec_QoS_Exported_Functions
 */
bool codec_preferred_cfg_get(T_CODEC_CFG_ITEM item, T_CODEC_CFG *p_cfg);

/**
 * codec_qos.h
 *
 * \brief  Get maximum SDU length by codec configuration.
 *
 * \param[in]  p_cfg          Pointer to codec configuration: @ref T_CODEC_CFG.
 * \param[in,out]  p_max_len  Pointer to maximum SDU length.
 *
 * \return         The result of get maximum SDU length by codec configuration.
 * \retval true    Get maximum SDU length by codec configuration success.
 * \retval false   Get maximum SDU length by codec configuration failed.
 *
 * \ingroup Codec_QoS_Exported_Functions
 */
bool codec_max_sdu_len_get(T_CODEC_CFG *p_cfg, uint16_t *p_max_len);

/**
 * codec_qos.h
 *
 * \brief  Get SDU interval by codec configuration.
 *
 * \param[in]  p_cfg          Pointer to codec configuration: @ref T_CODEC_CFG.
 * \param[in,out]  p_sdu_int  Pointer to SDU interval.
 *
 * \return         The result of get SDU interval by codec configuration.
 * \retval true    Get SDU interval by codec configuration success.
 * \retval false   Get SDU interval by codec configuration failed.
 *
 * \ingroup Codec_QoS_Exported_Functions
 */
bool codec_sdu_interval_get(T_CODEC_CFG *p_cfg, uint32_t *p_sdu_int);


/**
 * codec_qos.h
 *
 * \brief  Get preferred qos configuration.
 *
 * \param[in]  item       Codec capability configuration item: @ref T_CODEC_CFG_ITEM.
 * \param[in]  type       QoS configuration type: @ref T_QOS_CFG_TYPE.
 * \param[in,out]  p_qos  Pointer to preferred QoS configuration: @ref T_QOS_CFG_PREFERRED.
 *
 * \return         The result of get qos preferred configuration.
 * \retval true    Get qos preferred configuration success.
 * \retval false   Get qos preferred configuration failed.
 *
 * \ingroup Codec_QoS_Exported_Functions
 */
bool qos_preferred_cfg_get(T_CODEC_CFG_ITEM item, T_QOS_CFG_TYPE type, T_QOS_CFG_PREFERRED *p_qos);

/**
 * codec_qos.h
 *
 * \brief  Get preferred qos configuration by codec configuration.
 *
 * \param[in]  p_cfg           Pointer to codec configuration: @ref T_CODEC_CFG.
 * \param[in]  target_latency  Target latency.
 * \param[in,out]  p_qos       Pointer to QoS preferred configuration: @ref T_QOS_CFG_PREFERRED.
 *
 * \return         The result of get preferred qos configuration by codec configuration.
 * \retval true    Get preferred qos configuration by codec configuration success.
 * \retval false   Get preferred qos configuration by codec configuration failed.
 *
 * \ingroup Codec_QoS_Exported_Functions
 */
bool qos_preferred_cfg_get_by_codec(T_CODEC_CFG *p_cfg, uint8_t target_latency,
                                    T_QOS_CFG_PREFERRED *p_qos);
/**
 * End of Codec_QoS_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
