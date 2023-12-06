/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BAP_H_
#define _BAP_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "pacs_client.h"
#include "codec_qos.h"
#include "ascs_def.h"
#include "ble_audio_sync.h"

/**
 * \defgroup    LEA_GAF_BAP BAP Profile
 *
 * \brief   Basic Audio Profile.
 */

/**
 * \defgroup    BAP_Exported_Macros BAP Profile Exported Macros
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap.h
 *
 * \defgroup    BAP_ROLE BAP Role
 *
 * \brief  Define Basic Audio Profile Role.
 *
 * \ingroup BAP_Exported_Macros
 * \{
 */
#define BAP_BROADCAST_SOURCE_ROLE    0x01       /**< Broadcast Source Role. */
#define BAP_BROADCAST_SINK_ROLE      0x02       /**< Broadcast Sink Role. */
#define BAP_BROADCAST_ASSISTANT_ROLE 0x04       /**< Broadcast Assistant Role. */
#define BAP_SCAN_DELEGATOR_ROLE      0x08       /**< Scan Delegator Role. */
#define BAP_UNICAST_CLT_SRC_ROLE     0x10       /**< Unicast Client Source Role. */
#define BAP_UNICAST_CLT_SNK_ROLE     0x20       /**< Unicast Client Sink Role. */
#define BAP_UNICAST_SRV_SRC_ROLE     0x40       /**< Unicast Server Source Role. */
#define BAP_UNICAST_SRV_SNK_ROLE     0x80       /**< Unicast Server Sink Role. */
/**
 * End of BAP_ROLE
 * \}
 */

/**
 * bap.h
 *
 * \defgroup    PACS_CHECK_FILTER PACS Check Filter
 *
 * \brief  Define PACS Check Filter Type.
 *
 * \ingroup BAP_Exported_Macros
 * \{
 */
#define BA_PACS_CHECK_FILTER_CODEC      0x00
#define BA_PACS_CHECK_FILTER_ALLOCATION 0x01
/**
 * End of PACS_CHECK_FILTER
 * \}
 */

/**
 * bap.h
 *
 * \defgroup    PACS_CHAR_EXIST PACS Characteristic Value Exist
 *
 * \brief  Define PACS Characteristic Value Exist Mask.
 *
 * \ingroup BAP_Exported_Macros
 * \{
 */
#define PACS_AUDIO_AVAILABLE_CONTEXTS_EXIST 0x01
#define PACS_AUDIO_SUPPORTED_CONTEXTS_EXIST 0x02
#define PACS_SINK_AUDIO_LOC_EXIST           0x04
#define PACS_SINK_PAC_EXIST                 0x08
#define PACS_SOURCE_AUDIO_LOC_EXIST         0x10
#define PACS_SOURCE_PAC_EXIST               0x20
/**
 * End of PACS_CHAR_EXIST
 * \}
 */

/**
 * End of BAP_Exported_Macros
 * \}
 */

/**
 * \defgroup    BAP_Exported_Types BAP Profile Exported Types
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap.h
 *
 * \brief  Basic Audio Profile Role Information.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint8_t role_mask;      /**< BAP role mask. */
    uint8_t brs_num;        /**< BRS number. */

    uint8_t snk_ase_num;    /**< Sink ASE number. */
    uint8_t src_ase_num;    /**< Source ASE number. */

    bool    init_gap;

    /**<  For BAP_UNICAST_CLT_SRC_ROLE and BAP_UNICAST_CLT_SNK_ROLE. */
    uint8_t isoc_cig_max_num;           /**< ISOC CIG maximum number. */


    /**<  For BAP_UNICAST_CLT_SRC_ROLE, BAP_UNICAST_CLT_SNK_ROLE,
          BAP_UNICAST_SRV_SRC_ROLE and BAP_UNICAST_SRV_SNK_ROLE. */
    uint8_t isoc_cis_max_num;           /**< ISOC CIS maximum number. */

    /**<  For BAP_BROADCAST_SOURCE_ROLE. */
    uint8_t pa_adv_num;                 /**< PA advertiser number. */
    uint8_t isoc_big_broadcaster_num;   /**< ISOC BIG broadcaster number. */
    uint8_t isoc_bis_broadcaster_num;   /**< ISOC BIS broadcaster number. */

    /**<  For BAP_BROADCAST_SINK_ROLE and BAP_BROADCAST_ASSISTANT_ROLE. */
    uint8_t pa_sync_num;                /**< PA Sync number. */
    uint8_t isoc_big_receiver_num;      /**< ISOC BIG receiver number. */
    uint8_t isoc_bis_receiver_num;      /**< ISOC BIS receiver number. */
} T_BAP_ROLE_INFO;

/**
 * bap.h
 *
 * \brief  BAP Discover All Service Result.
 *         The message data for LE_AUDIO_MSG_BAP_DIS_ALL_DONE.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint16_t conn_handle;
    bool    pacs_is_found;
    uint8_t sink_pac_num;
    uint8_t source_pac_num;
    bool    ascs_is_found;
    uint8_t sink_ase_num;
    uint8_t source_ase_num;
    bool    bass_is_found;
    uint8_t brs_char_num;
} T_BAP_DIS_ALL_DONE;

/**
 * bap.h
 *
 * \brief  PACS Characteristics Information.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint16_t
    value_exist;          /**< PACS characteristic value exist flag: @ref PACS_CHAR_EXIST. */
    uint8_t             sink_pac_num;         /**< Sink PAC number. */
    uint8_t             source_pac_num;       /**< Source PAC number. */
    uint32_t            snk_audio_loc;        /**< Sink Audio Locations. */
    uint32_t            src_audio_loc;        /**< Source Audio Locations. */
    uint16_t            snk_sup_context;      /**< Sink Audio Supported Audio Contexts. */
    uint16_t            src_sup_context;      /**< Source Audio Supported Audio Contexts. */
    uint16_t            snk_avail_context;    /**< Sink Audio Available Audio Contexts. */
    uint16_t            src_avail_context;    /**< Source Audio Available Audio Contexts. */
} T_BAP_PACS_INFO;

/**
 * bap.h
 *
 * \brief  BAP PAC Record Data.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    uint16_t                   pac_handle;
    uint8_t                    codec_id[CODEC_ID_LEN];
    T_CODEC_CAP                codec_cap;
    uint32_t                   lc3_sup_cfg_bits;
    uint16_t                   pref_audio_contexts;
    uint8_t                    metadata_length;
    uint8_t                   *p_metadata;
} T_BAP_PAC_RECORD;

/**
 * bap.h
 *
 * \brief  BAP PAC Record Notification Data.
 *         The message data for LE_AUDIO_MSG_BAP_PACS_NOTIFY.
 *
 * \ingroup BAP_Exported_Types
 */
typedef struct
{
    T_PACS_TYPE pacs_type;
    uint16_t conn_handle;
    uint16_t pac_handle;
} T_BAP_PACS_NOTIFY;
/**
 * End of BAP_Exported_Types
 * \}
 */

/**
 * \defgroup    BAP_Exported_Functions BAP Profile Exported Functions
 *
 * \ingroup LEA_GAF_BAP
 * \{
 */

/**
 * bap.h
 *
 * \brief  Get PACS characteristics information.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in,out]  p_pacs_info  Pointer to PASC characteristics information: @ref T_BAP_PACS_INFO.
 *
 * \return         The result of get PACS characteristics information.
 * \retval true    Get PACS characteristics information success.
 * \retval false   Get PACS characteristics information failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool bap_pacs_get_info(uint16_t conn_handle, T_BAP_PACS_INFO *p_pacs_info);

/**
 * bap.h
 *
 * \brief  Get PAC record data.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  direction        Pointer to audio direction: @ref T_AUDIO_DIRECTION.
 * \param[in]  p_pac_num        Pointer to PAC number.
 * \param[in,out]  p_pac_tbl    Pointer to PAC record data: @ref T_BAP_PAC_RECORD.
 *
 * \return         The result of get PAC record data.
 * \retval true    Get PAC record data success.
 * \retval false   Get PAC record data failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool bap_pacs_get_pac_record(uint16_t conn_handle, T_AUDIO_DIRECTION direction, uint8_t *p_pac_num,
                             T_BAP_PAC_RECORD *p_pac_tbl);

/**
 * bap.h
 *
 * \brief  Get PAC record data by handle.
 *
 * \param[in]  conn_handle      Connection handle.
 * \param[in]  pac_handle       PAC record handle.
 * \param[in]  p_pac_num        Pointer to PAC number.
 * \param[in,out]  p_pac_tbl    Pointer to PAC record data: @ref T_BAP_PAC_RECORD.
 *
 * \return         The result of get PAC record data by handle.
 * \retval true    Get PAC record data by handle success.
 * \retval false   Get PAC record data by handle failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool bap_pacs_get_pac_record_by_handle(uint16_t conn_handle, uint16_t pac_handle,
                                       uint8_t *p_pac_num,
                                       T_BAP_PAC_RECORD *p_pac_tbl);

/**
 * bap.h
 *
 * \brief  Get LC3 sink table mask.
 *
 * \param[in]  conn_handle          Connection handle.
 * \param[in]  pref_audio_contexts  Preferred audio context: @ref AUDIO_CONTEXT.
 * \param[in]  channel_count        Channel count.
 * \param[in]  block_num            Block number.
 *
 * \return         The LC3 sink table mask.
 *
 * \ingroup BAP_Exported_Functions
 */
uint32_t bap_pacs_get_lc3_snk_table_msk(uint16_t conn_handle, uint16_t pref_audio_contexts,
                                        uint8_t channel_count,
                                        uint8_t block_num);

/**
 * bap.h
 *
 * \brief  Get LC3 source table mask.
 *
 * \param[in]  conn_handle          Connection handle.
 * \param[in]  pref_audio_contexts  Preferred audio context: @ref AUDIO_CONTEXT.
 * \param[in]  channel_count        Channel count.
 * \param[in]  block_num            Block number.
 *
 * \return         The LC3 source table mask.
 *
 * \ingroup BAP_Exported_Functions
 */
uint32_t bap_pacs_get_lc3_src_table_msk(uint16_t conn_handle, uint16_t pref_audio_contexts,
                                        uint8_t channel_count,
                                        uint8_t block_num);

/**
 * bap.h
 *
 * \brief  PACS filter the codec configuration by sync handle.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  handle          BLE audio sync handle: @ref T_BLE_AUDIO_SYNC_HANDLE.
 * \param[in]  conn_handle     Connection handle.
 * \param[in]  filter          Check filter type: @ref PACS_CHECK_FILTER.
 *
 * \return        BIS Array.
 *
 * \ingroup BAP_Exported_Functions
 */
uint32_t bap_pacs_check_cfg_by_sync_info(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                                         uint8_t filter);
/**
 * bap.h
 *
 * \brief  PACS filter the codec configuration by BASE data.
 *
 * \xrefitem Added_API_2_13_1_0 "Added Since 2.13.1.0" "Added API"
 *
 * \param[in]  p_base_mapping  BASE data.
 * \param[in]  conn_handle     Connection handle.
 * \param[in]  filter          Check filter type: @ref PACS_CHECK_FILTER.
 *
 * \return        BIS Array.
 *
 * \ingroup BAP_Exported_Functions
 */
uint32_t bap_pacs_check_cfg_by_base_data(T_BASE_DATA_MAPPING *p_base_mapping, uint16_t conn_handle,
                                         uint8_t filter);

/**
 * bap.h
 *
 * \brief  Initialize the BAP role.
 *
 * \param[in]  p_role_info      Pointer to BAP role information: @ref T_BAP_ROLE_INFO.
 *
 * \return         The result of get PAC record data by handle.
 * \retval true    Get PAC record data by handle success.
 * \retval false   Get PAC record data by handle failed.
 *
 * \ingroup BAP_Exported_Functions
 */
bool bap_role_init(T_BAP_ROLE_INFO *p_role_info);
/**
 * End of BAP_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
