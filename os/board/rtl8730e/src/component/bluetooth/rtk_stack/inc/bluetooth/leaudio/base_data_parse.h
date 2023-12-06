/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BASE_DATA_PARSE_H_
#define _BASE_DATA_PARSE_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include "codec_def.h"

/**
 * \defgroup    LEA_GAF_Base_Data Base Data
 *
 * \brief   BAP Base Data.
 */

/**
 * \defgroup    Base_Data_Exported_Types Base Data Exported Types
 *
 * \ingroup LEA_GAF_Base_Data
 * \{
 */

/**
 * base_data_parse.h
 *
 * \brief  Base Data BIS Parameters.
 *
 * \ingroup Base_Data_Exported_Types
 */
typedef struct
{
    uint8_t         subgroup_idx;
    uint8_t         bis_index;
    uint8_t         codec_id[CODEC_ID_LEN];
    T_CODEC_CFG     bis_codec_cfg;
} T_BASE_DATA_BIS_PARAM;

/**
 * base_data_parse.h
 *
 * \brief  Base Data Subgroup Parameters.
 *
 * \ingroup Base_Data_Exported_Types
 */
typedef struct
{
    uint8_t                 subgroup_idx;
    uint8_t                 num_bis;
    uint8_t                 metadata_len;
    uint32_t                bis_array;
    uint8_t                 *p_metadata;
    T_BASE_DATA_BIS_PARAM   *p_bis_param;
} T_BASE_DATA_SUBGROUP_PARAM;

/**
 * base_data_parse.h
 *
 * \brief  Base Data Mapping.
 *
 * \ingroup Base_Data_Exported_Types
 */
typedef struct
{
    uint8_t num_bis;
    uint8_t num_subgroups;
    uint32_t presentation_delay;
    T_BASE_DATA_SUBGROUP_PARAM *p_subgroup; /**< Level2 parameters. */
} T_BASE_DATA_MAPPING;
/**
 * End of Base_Data_Exported_Types
 * \}
 */

/**
 * \defgroup    Base_Data_Exported_Functions Base Data Exported Functions
 *
 * \ingroup LEA_GAF_Base_Data
 * \{
 */

/**
 * base_data_parse.h
 *
 * \brief  Base data parse data.
 *
 * \param[in]  pa_data_len      Periodic advertising data length.
 * \param[in]  p_pa_data        Pointer to periodic advertising data.
 *
 * \return         The pointer to base data mapping: @ref T_BASE_DATA_MAPPING.
 *
 * \ingroup Base_Data_Exported_Functions
 */
T_BASE_DATA_MAPPING *base_data_parse_data(uint16_t pa_data_len, uint8_t *p_pa_data);

/**
 * base_data_parse.h
 *
 * \brief  Compare two base data mappings.
 *
 * \param[in]  p_mapping_a      Pointer to base data mapping a: @ref T_BASE_DATA_MAPPING.
 * \param[in]  p_mapping_b      Pointer to base data mapping b: @ref T_BASE_DATA_MAPPING.
 *
 * \return         The result of compare two base data mappings.
 * \retval true    Base data mappings are the same.
 * \retval false   Base data mappings are not the same.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_cmp(T_BASE_DATA_MAPPING *p_mapping_a, T_BASE_DATA_MAPPING *p_mapping_b);

/**
 * base_data_parse.h
 *
 * \brief  Get base data BIS codec configuration.
 *
 * \param[in]  p_mapping      Pointer to base data mapping: @ref T_BASE_DATA_MAPPING.
 * \param[in]  bis_idx        BIS index.
 * \param[in,out]  p_cfg      Pointer to codec configuration: @ref T_CODEC_CFG.
 *
 * \return         The result of get BIS codec configuration.
 * \retval true    Get BIS codec configuration success.
 * \retval false   Get BIS codec configuration failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_get_bis_codec_cfg(T_BASE_DATA_MAPPING *p_mapping, uint8_t bis_idx,
                                 T_CODEC_CFG *p_cfg);

/**
 * base_data_parse.h
 *
 * \brief  Free base data.
 *
 * \param[in]  p_mapping      Pointer to base data mapping: @ref T_BASE_DATA_MAPPING.
 *
 * \ingroup Base_Data_Exported_Functions
 */
void base_data_free(T_BASE_DATA_MAPPING *p_mapping);
/**
 * End of Base_Data_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
