/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _ASCS_DEF_H_
#define _ASCS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"

/**
 * \defgroup    LEA_GAF_ASCS_Def Audio Stream Control Service Definition
 *
 * \brief   Audio Stream Control Service related definition.
 */

/**
 * \defgroup    ASCS_Def_Exported_Macros Audio Stream Control Service Definition Exported Macros
 *
 * \ingroup LEA_GAF_ASCS_Def
 * \{
 */

/**
 * ascs_def.h
 *
 * \defgroup    ASCS_UUID Audio Stream Control Service UUID
 *
 * \brief  Define Audio Stream Control Service UUID.
 *
 * \ingroup ASCS_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_ASCS                            0x184E      /**< Audio Stream Control Service */

#define ASCS_UUID_CHAR_SNK_ASE                    0x2BC4      /**< Sink ASE */
#define ASCS_UUID_CHAR_SRC_ASE                    0x2BC5      /**< Source ASE */
#define ASCS_UUID_CHAR_ASE_CONTROL_POINT          0x2BC6      /**< ASE Control Point */

#define ASCS_AES_CHAR_MAX_NUM 7
#define ASE_INVALID_ID 0x00
/**
 * End of ASCS_UUID
 * \}
 */

/**
 * ascs_def.h
 *
 * \defgroup    FRAMING_PARAM_BIT Framing Parameter Bit Mask
 *
 * \brief  Define Framing Parameter Bit Mask
 *         for an ASE in the Codec Configured state.
 *
 * \ingroup ASCS_Def_Exported_Macros
 * \{
 */
#define QOS_SUPPORTED_UNFRAMED_BIT         0x01      /**< bit0, Unframed */
#define QOS_SUPPORTED_FRAMED_BIT           0x02      /**< bit1, Framed */
/**
 * End of FRAMING_PARAM_BIT
 * \}
 */

/**
 * ascs_def.h
 *
 * \defgroup    QOS_PARAM_RANGE QoS Parameter Range
 *
 * \brief  Define Parameter Range for an ASE
 *         written by the client in the Config QoS operation.
 *
 * \ingroup ASCS_Def_Exported_Macros
 * \{
 */
#define QOS_SDU_INTERVAL_MIN               0x0000FF
#define QOS_SDU_INTERVAL_MAX               0xFFFFFF
#define QOS_MAX_SDU_MIN                    0x0000
#define QOS_MAX_SDU_MAX                    0x0FFF
#define QOS_RETRANSMISSION_NUMBER_MIN      0x00
#define QOS_RETRANSMISSION_NUMBER_MAX      0xFF
#define QOS_TRANSPORT_LATENCY_MIN          0x0005
#define QOS_TRANSPORT_LATENCY_MAX          0x0FA0
/**
 * End of QOS_PARAM_RANGE
 * \}
 */

/**
 * ascs_def.h
 *
 * \defgroup    PREFERRED_PHY_BIT Preferred_PHY Bit Mask
 *
 * \brief  Define Preferred_PHY Bit Mask for an ASE.
 *
 * \ingroup ASCS_Def_Exported_Macros
 * \{
 */
#define ASE_LE_1M_PHY_BIT                  0x01
#define ASE_LE_2M_PHY_BIT                  0x02
#define ASE_LE_CODED_PHY_BIT               0x04
#define ASE_LE_PHY_CFG_MASK                0x07
/**
 * End of PREFERRED_PHY_BIT
 * \}
 */

/**
 * ascs_def.h
 *
 * \defgroup    ASE_CP_RES_CODE ASE CP Response Code
 *
 * \brief  Define ASE Control Point characteristic
 *         Response_Code and Reason values when notified
 *
 *  Used when Response_Code = ASE_CP_RESP_UNSUPPORTED_PARAM, Response_Code = ASE_CP_RESP_REJECTED_PARAM,
 *  or ASE_CP_RESP_INVALID_PARAM.
 *
 * \ingroup ASCS_Def_Exported_Macros
 * \{
 */
#define ASE_CP_REASON_NONE                    0x00
#define ASE_CP_REASON_CODEC_ID                0x01
#define ASE_CP_REASON_CODEC_CFG               0x02
#define ASE_CP_REASON_SDU_INTERVAL            0x03
#define ASE_CP_REASON_FRAMING                 0x04
#define ASE_CP_REASON_PHY                     0x05
#define ASE_CP_REASON_MAX_SDU_SIZE            0x06
#define ASE_CP_REASON_RETRANS_NUM             0x07
#define ASE_CP_REASON_MAX_TRANSPORT_LATENCY   0x08
#define ASE_CP_REASON_PRESENTATION_DELAY      0x09
#define ASE_CP_REASON_INVALID_ASE_CIS_MAPPING 0x0A
/**
 * End of ASE_CP_RES_CODE
 * \}
 */

/**
 * End of ASCS_Def_Exported_Macros
 * \}
 */

/**
 * \defgroup    ASCS_Def_Exported_Types Audio Stream Control Service Definition Exported Types
 *
 * \ingroup LEA_GAF_ASCS_Def
 * \{
 */

/**
 * ascs_def.h
 *
 * \brief  Server Support for Unframed ISOAL PDUs.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    AUDIO_UNFRAMED_SUPPORTED      = 0x00,
    AUDIO_UNFRAMED_NOT_SUPPORTED  = 0x01,
} T_AUDIO_UNFRAMED_SUPPORTED;

/**
 * ascs_def.h
 *
 * \brief  Framing Parameter Value for an ASE
 *         written by the client in the Config QoS operation.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    AUDIO_UNFRAMED = 0x00,
    AUDIO_FRAMED   = 0x01,
} T_AUDIO_FRAMING;

/**
 * ascs_def.h
 *
 * \brief  The States of the ASE State Machine.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    ASE_STATE_IDLE             = 0x00,
    ASE_STATE_CODEC_CONFIGURED = 0x01,
    ASE_STATE_QOS_CONFIGURED   = 0x02,
    ASE_STATE_ENABLING         = 0x03,
    ASE_STATE_STREAMING        = 0x04,
    ASE_STATE_DISABLING        = 0x05,
    ASE_STATE_RELEASING        = 0x06,

    ASE_STATE_UNKOWN           = 0xff,
} T_ASE_STATE;

/**
 * ascs_def.h
 *
 * \brief  The Data of ASE Codec Configured State.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t supported_framing;
    uint8_t preferred_phy;
    uint8_t preferred_retrans_number;
    uint8_t max_transport_latency[2];
    uint8_t presentation_delay_min[3];
    uint8_t presentation_delay_max[3];
    uint8_t preferred_presentation_delay_min[3];
    uint8_t preferred_presentation_delay_max[3];
    uint8_t codec_id[5];
    uint8_t codec_spec_cfg_len;
} T_ASE_CODEC_CFG_STATE_DATA;

/**
 * ascs_def.h
 *
 * \brief  The Format of the Additional_ASE_Parameters Field when
 *         ASE_State = 0x02 (QoS Configured).
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t cig_id;
    uint8_t cis_id;
    uint8_t sdu_interval[3];
    uint8_t framing;
    uint8_t phy;
    uint8_t max_sdu[2];
    uint8_t retransmission_number;
    uint8_t max_transport_latency[2];
    uint8_t presentation_delay[3];
} T_ASE_QOS_CFG_STATE_DATA;

/**
 * ascs_def.h
 *
 * \brief  The Format of the Additional_ASE_Parameters Field when
 *         ASE_State = 0x01 (Codec Configured).
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    T_ASE_CODEC_CFG_STATE_DATA data;
    uint8_t *p_codec_spec_cfg;
} T_ASE_DATA_CODEC_CONFIGURED;


/**
 * ascs_def.h
 *
 * \brief  The Format of the Additional_ASE_Parameters Field when
 *         ASE_State = 0x03 (Enabling), 0x04 (Streaming), or 0x05 (Disabling).
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t cig_id;
    uint8_t cis_id;
    uint8_t metadata_length;
    uint8_t *p_metadata;
} T_ASE_DATA_WITH_METADATA;

/**
 * ascs_def.h
 *
 * \brief  The Additional_ASE_Parameters Field Data for All ASE State.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef union
{
    T_ASE_DATA_CODEC_CONFIGURED codec_configured;
    T_ASE_QOS_CFG_STATE_DATA    qos_configured;
    T_ASE_DATA_WITH_METADATA    enabling;
    T_ASE_DATA_WITH_METADATA    streaming;
    T_ASE_DATA_WITH_METADATA    disabling;
} T_ASE_DATA;

/**
 * ascs_def.h
 *
 * \brief  ASE Characteristic Data.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t ase_id;
    uint8_t direction;
    uint8_t ase_state;
    T_ASE_DATA param;
} T_ASE_CHAR_DATA;

/**
 * ascs_def.h
 *
 * \brief  ASE Control Point Characteristic Response Code.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    ASE_CP_RESP_SUCCESS                   = 0x00,    /**< Success */
    ASE_CP_RESP_UNSUPPORTED_OPCODE        = 0x01,    /**< Unsupported Opcode */
    ASE_CP_RESP_INVALID_LENGTH            = 0x02,    /**< Invalid Length */
    ASE_CP_RESP_INVALID_ASE_ID            = 0x03,    /**< Invalid ASE_ID */
    ASE_CP_RESP_INVALID_ASE_SM_TRANSITION = 0x04,    /**< Invalid ASE State Machine Transition */
    ASE_CP_RESP_INVALID_ASE_DIRECTION     = 0x05,    /**< Invalid ASE direction */
    ASE_CP_RESP_UNSUPPORTED_AUDIO_CAP     = 0x06,    /**< Unsupported Audio Capabilities */
    ASE_CP_RESP_UNSUPPORTED_PARAM         = 0x07,    /**< Unsupported Configuration Parameter Value */
    ASE_CP_RESP_REJECTED_PARAM            = 0x08,    /**< Rejected Configuration Parameter Value */
    ASE_CP_RESP_INVALID_PARAM             = 0x09,    /**< Invalid Configuration Parameter Value */
    ASE_CP_RESP_UNSUPPORTED_METADATA      = 0x0A,    /**< Unsupported Metadata */
    ASE_CP_RESP_REJECTED_METADATA         = 0x0B,    /**< Rejected Metadata */
    ASE_CP_RESP_INVALID_METADATA          = 0x0C,    /**< Invalid Metadata */
    ASE_CP_RESP_INSUFFICIENT_RESOURCE     = 0x0D,    /**< Insufficient Resources */
    ASE_CP_RESP_UNSPECIFIED_ERROR         = 0x0E,    /**< Unspecified Error */
    ASE_CP_RESP_RFU
} T_ASE_CP_RESP;

/**
 * ascs_def.h
 *
 * \brief  ASE Control Point Opcode.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    ASE_CP_OP_NONE            = 0x00,
    ASE_CP_OP_CONFIG_CODEC    = 0x01,    /**< Config Codec. Initiated by Client or Server. */
    ASE_CP_OP_CONFIG_QOS      = 0x02,    /**< Config QoS. Initiated by Client only. */
    ASE_CP_OP_ENABLE          = 0x03,    /**< Enable. Initiated by Client only. */
    ASE_CP_OP_REC_START_READY = 0x04,    /**< Receiver Start Ready. Initiated by Client or Server. */
    ASE_CP_OP_DISABLE         = 0x05,    /**< Disable. Initiated by Client or Server. */
    ASE_CP_OP_REC_STOP_READY  = 0x06,    /**< Receiver Stop Ready. Initiated by Client or Server. */
    ASE_CP_OP_UPDATE_METADATA = 0x07,    /**< Update Metadata. Initiated by Client or Server. */
    ASE_CP_OP_RELEASE         = 0x08,    /**< Release. Initiated by Client or Server. */
    ASE_CP_OP_MAX,
} T_ASE_CP_OP;

/**
 * ascs_def.h
 *
 * \brief  QoS Preferences Target Latency in Codec Configured State.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    ASE_TARGET_LOWER_LATENCY      = 0x01,    /**< Target low latency. */
    ASE_TARGET_BALANCED           = 0x02,    /**< Target balanced latency and reliability. */
    ASE_TARGET_HIGHER_RELIABILITY = 0x03,    /**< Target high reliability. */
} T_ASE_TARGET_LATENCY;

/**
 * ascs_def.h
 *
 * \brief  PHY Parameter Target to Achieve the Target_Latency Value.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef enum
{
    ASE_TARGET_PHY_1M    = 0x01,    /**< LE 1M PHY. */
    ASE_TARGET_PHY_2M    = 0x02,    /**< LE 2M PHY. */
    ASE_TARGET_PHY_CODED = 0x03,    /**< LE Coded PHY. */
} T_ASE_TARGET_PHY;

/**
 * ascs_def.h
 *
 * \brief  ASE Control Point Notify Parameter.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t ase_id;
    uint8_t response_code;
    uint8_t reason;
} T_ASE_CP_NOTIFY_ARRAY_PARAM;

/**
 * ascs_def.h
 *
 * \brief  Format of ASE Control Point Characteristic Value when Notified.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t opcode;
    uint8_t number_of_ase;
    T_ASE_CP_NOTIFY_ARRAY_PARAM param[1];
} T_ASE_CP_NOTIFY_DATA;

/**
 * ascs_def.h
 *
 * \brief  ASE Control Point Codec Config Parameter.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t ase_id;
    uint8_t target_latency;
    uint8_t target_phy;
    uint8_t codec_id[5];
    uint8_t codec_spec_cfg_len;
} T_ASE_CP_CODEC_CFG_ARRAY_PARAM;

/**
 * ascs_def.h
 *
 * \brief  ASE Control Point QoS Config Parameter.
 *
 * \ingroup ASCS_Def_Exported_Types
 */
typedef struct
{
    uint8_t ase_id;
    uint8_t cig_id;
    uint8_t cis_id;
    uint8_t sdu_interval[3];
    uint8_t framing;
    uint8_t phy;
    uint8_t max_sdu[2];
    uint8_t retransmission_number;
    uint8_t max_transport_latency[2];
    uint8_t presentation_delay[3];
} T_ASE_CP_QOS_CFG_ARRAY_PARAM;
/**
 * End of ASCS_Def_Exported_Macros
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
