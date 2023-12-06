#ifndef _PACS_DEF_H_
#define _PACS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/**
 * \defgroup    LEA_GAF_PACS_Def Published Audio Capabilities Service Definition
 *
 * \brief   Published Audio Capabilities Service related definition.
 */

/**
 * \defgroup    PACS_Def_Exported_Macros Published Audio Capabilities Service Definition Exported Macros
 *
 * \ingroup LEA_GAF_PACS_Def
 * \{
 */

/**
 * pasc_def.h
 *
 * \brief  The Minimum Length of PAC Record.
 *
 * \ingroup PACS_Def_Exported_Macros
 */
#define PAC_RECORD_MIN_LEN 8

/**
 * pasc_def.h
 *
 * \defgroup  PACS_UUID PACS Service UUID
 *
 * \brief  Define Published Audio Capabilities Service UUID.
 *
 * \ingroup PACS_Def_Exported_Macros
 * \{
 */
#define GATT_UUID_PACS                              0x1850    /**< Published Audio Capabilities service. */

#define PACS_UUID_CHAR_SINK_PAC                     0x2BC9    /**< Sink PAC. */
#define PACS_UUID_CHAR_SINK_AUDIO_LOCATIONS         0x2BCA    /**< Sink Audio Locations. */
#define PACS_UUID_CHAR_SOURCE_PAC                   0x2BCB    /**< Source PAC. */
#define PACS_UUID_CHAR_SOURCE_AUDIO_LOCATIONS       0x2BCC    /**< Source Audio Locations. */
#define PACS_UUID_CHAR_AUDIO_AVAILABILITY_CONTEXTS  0x2BCD    /**< Available Audio Contexts. */
#define PACS_UUID_CHAR_SUPPORTED_AUDIO_CONTEXTS     0x2BCE    /**< Supported Audio Contexts. */
/**
 * End of PACS_UUID
 * \}
 */

/**
 * End of PACS_Def_Exported_Macros
 * \}
 */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
