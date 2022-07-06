/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gls_config.h
  * @brief    glucose service configuration file.
  * @details  Configure the optional characteristic and fields in the glucose service.
  * @author   bill
  * @date     2017-6-8
  * @version  v1.0
  * *************************************************************************************
  */

#ifndef _GLS_CONFIG_H_
#define _GLS_CONFIG_H_

/** @defgroup GLS Glucose Service
  * @brief Glucose service
  * @{
  */


/** @defgroup GLS_CONFIG Glusose Service Config
  * @brief Glusose service configuration file
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GLS_Common_Exported_Macros GLS Common Exported Macros
  * @{
  */

/** @brief glucose maximum number of records in database */
#define GLC_RACP_MAX_NBR_OF_STORED_RECS             10

/** @defgroup GLS_Optional_Characteristic GLS Optional Characteristic
  * @brief  glucose optional characteristic configuration
  * @{
  */
#define GLC_MEASUREMENT_CONTEXT_SUPPORT             1
/** @} */

/** @defgroup GLS_Optional_Field GLS Optional Field
  * @brief  glucose optional field.
  * @{
  */
/** glucose measurement optional fields configuration */
#define GLC_INCLUDE_TIME_OFFSET                     1
#define GLC_INCLUDE_CONC_TS_LOC                     1
#define GLC_INCLUDE_SS_ANNUNC                       1

/** glucose measurement context optional fields configuration */
#if (GLC_MEASUREMENT_CONTEXT_SUPPORT)
#define GLC_INCLUDE_CARBOHYDRATE                    1
#define GLC_INCLUDE_MEAL                            1
#define GLC_INCLUDE_TESTER_HEALTH                   1
#define GLC_INCLUDE_EXCERCISE                       1
#define GLC_INCLUDE_MEDICATION                      1
#define GLC_INCLUDE_HbA1c                           1
#define GLC_INCLUDE_EXT_FLAGS                       0

#define GLC_MS_CTXT_INCLUDE_XXX (GLC_INCLUDE_CARBOHYDRATE + \
                                 GLC_INCLUDE_MEAL + \
                                 GLC_INCLUDE_TESTER_HEALTH + \
                                 GLC_INCLUDE_EXCERCISE + \
                                 GLC_INCLUDE_MEDICATION + \
                                 GLC_INCLUDE_HbA1c)
#if (GLC_MS_CTXT_INCLUDE_XXX == 0)
#error "Glucose measurement context characteristic value shall include at least one field, in addition to the Flags field and Sequence Number field!"
#endif
#endif
/** @} */

/** @defgroup GLS_Attribute_Index GLS Attribute Index
  * @brief  Index of each characteristic in service database
  * @{
  */
#define INDEX_OFFSET0                               (0)
#define GLS_CHAR_GLC_MEASUREMENT_INDEX              (INDEX_OFFSET0+0x02)
#define GLS_CHAR_GLC_MEASUREMENT_CCCD_INDEX         (GLS_CHAR_GLC_MEASUREMENT_INDEX + 1)

#if (GLC_MEASUREMENT_CONTEXT_SUPPORT)
#define GLS_CHAR_GLC_MEASUREMENT_CONTEXT_INDEX      (INDEX_OFFSET0+0x05)
#define GLS_CHAR_GLC_MEASUREMENT_CONTEXT_CCCD_INDEX (GLS_CHAR_GLC_MEASUREMENT_CONTEXT_INDEX + 1)
#endif

#define INDEX_OFFSET1                               (INDEX_OFFSET0+GLC_MEASUREMENT_CONTEXT_SUPPORT*3)
#define GLS_CHAR_GLC_FEATURE_INDEX                  (INDEX_OFFSET1+0x05)
#define GLS_CHAR_GLC_RACP_INDEX                     (INDEX_OFFSET1+0x07)
#define GLS_CHAR_GLC_RACP_CCCD_INDEX                (GLS_CHAR_GLC_RACP_INDEX + 1)
/** @} */


/** @} End of GLS_Common_Exported_Macros */

/** @} End of GLS_CONFIG */

/** @} End of GLS */
#endif
