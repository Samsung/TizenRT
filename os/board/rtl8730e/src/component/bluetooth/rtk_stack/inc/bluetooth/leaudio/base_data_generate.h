/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BASE_DATA_GENERATE_H_
#define _BASE_DATA_GENERATE_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>

/**
 * \defgroup    LEA_GAF_Base_Data Base Data
 *
 * \brief   BAP Base Data.
 */

/**
 * \defgroup    Base_Data_Exported_Functions Base Data Exported Functions
 *
 * \ingroup LEA_GAF_Base_Data
 * \{
 */

/**
 * base_data_generate.h
 *
 * \brief  Add base data group.
 *
 * \param[in,out]  p_group_idx      Pointer to base data group index.
 * \param[in]  presentation_delay   Base data presentation delay.
 *
 * \return         The result of add base data group.
 * \retval true    Add base data group success.
 * \retval false   Add base data group failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_add_group(uint8_t *p_group_idx, uint32_t presentation_delay);

/**
 * base_data_generate.h
 *
 * \brief  Delete base data group.
 *
 * \param[in]  group_idx        Base data group index.
 *
 * \return         The result of delete base data group.
 * \retval true    Delete base data group success.
 * \retval false   Delete base data group failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_del_group(uint8_t group_idx);

/**
 * base_data_generate.h
 *
 * \brief  Base data add subgroup.
 *
 * \param[in,out]  p_subgroup_idx    Pointer to base data subgroup index.
 * \param[in]  group_idx             Base data group index.
 * \param[in]  codec_id              Pointer to codec id.
 * \param[in]  codec_cfg_len         Codec configuration length.
 * \param[in]  p_codec_cfg           Pointer to codec configuration.
 * \param[in]  metadata_len          Metadata length.
 * \param[in]  p_metadata            Pointer to metadata.
 *
 * \return         The result of base data add subgroup.
 * \retval true    Base data add subgroup success.
 * \retval false   Base data add subgroup failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_add_subgroup(uint8_t *p_subgroup_idx, uint8_t group_idx, uint8_t *codec_id,
                            uint8_t codec_cfg_len, uint8_t *p_codec_cfg,
                            uint8_t metadata_len, uint8_t *p_metadata);

/**
 * base_data_generate.h
 *
 * \brief  Base data add BIS.
 *
 * \param[in,out]  p_bis_idx      Pointer to base data BIS index.
 * \param[in]  group_idx          Base data group index.
 * \param[in]  subgroup_idx       Pointer to codec id.
 * \param[in]  codec_cfg_len      Codec configuration length.
 * \param[in]  p_codec_cfg        Pointer to codec configuration.
 *
 * \return         The result of base data add BIS.
 * \retval true    Base data add BIS success.
 * \retval false   Base data add BIS failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_add_bis(uint8_t *p_bis_idx, uint8_t group_idx, uint8_t subgroup_idx,
                       uint8_t codec_cfg_len, uint8_t *p_codec_cfg);

/**
 * base_data_generate.h
 *
 * \brief  Base data generate periodic advertising data.
 *
 * \param[in]  group_idx          Base data group index.
 * \param[in,out]  p_pa_len       Pointer to periodic advertising data length.
 * \param[in,out]  pp_pa_data     Pointer to periodic advertising data.
 *
 * \return         The result of base data generate periodic advertising data.
 * \retval true    Base data generate periodic advertising data success.
 * \retval false   Base data generate periodic advertising data failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_gen_pa_data(uint8_t group_idx, uint16_t *p_pa_len, uint8_t **pp_pa_data);

/**
 * base_data_generate.h
 *
 * \brief  Base data update metadata.
 *
 * \param[in]  group_idx         Base data group index.
 * \param[in]  subgroup_idx      Base data subgroup index.
 * \param[in]  metadata_len      Metadata length.
 * \param[in]  p_metadata        Pointer to metadata.
 *
 * \return         The result of base data update metadata.
 * \retval true    Base data update metadata success.
 * \retval false   Base data update metadata failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_update_metadata(uint8_t group_idx, uint8_t subgroup_idx,
                               uint8_t metadata_len, uint8_t *p_metadata);
/**
 * base_data_generate.h
 *
 * \brief  Get base data BIS number.
 *
 * \param[in]  group_idx          Base data group index.
 * \param[in,out]  p_bis_num      Pointer to base data BIS number.
 *
 * \return         The result of get base data BIS number.
 * \retval true    Get base data BIS number success.
 * \retval false   Get base data BIS number failed.
 *
 * \ingroup Base_Data_Exported_Functions
 */
bool base_data_get_bis_num(uint8_t group_idx, uint8_t *p_bis_num);
/**
 * End of Base_Data_Exported_Functions
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
