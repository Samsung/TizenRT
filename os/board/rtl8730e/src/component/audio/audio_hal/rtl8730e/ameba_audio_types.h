#ifndef AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_AMEBA_AUDIO_TYPES_H
#define AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_AMEBA_AUDIO_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Defines all the audio microphone categories.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/* [amebalite] support AMIC1-AMIC3, DMIC1-DMIC4
	 * [amebasmart] support AMIC1-AMIC5, DMIC1-DMIC8
	 */
	AMEBA_AUDIO_AMIC1         = 0,
	AMEBA_AUDIO_AMIC2         = 1,
	AMEBA_AUDIO_AMIC3         = 2,
	AMEBA_AUDIO_AMIC4         = 3,
	AMEBA_AUDIO_AMIC5         = 4,
	AMEBA_AUDIO_DMIC1         = 5,
	AMEBA_AUDIO_DMIC2         = 6,
	AMEBA_AUDIO_DMIC3         = 7,
	AMEBA_AUDIO_DMIC4         = 8,
	AMEBA_AUDIO_DMIC5         = 9,
	AMEBA_AUDIO_DMIC6         = 10,
	AMEBA_AUDIO_DMIC7         = 11,
	AMEBA_AUDIO_DMIC8         = 12,
	AMEBA_AUDIO_MIC_MAX_NUM   = 13,
};

/**
 * @brief Defines the audio capture main usages.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	AMEBA_AUDIO_CAPTURE_USAGE_AMIC         = 0,
	AMEBA_AUDIO_CAPTURE_USAGE_DMIC         = 1,
	AMEBA_AUDIO_CAPTURE_USAGE_DMIC_REF_AMIC = 2,
	AMEBA_AUDIO_CAPTURE_USAGE_LINE_IN      = 3,
	AMEBA_AUDIO_CAPTURE_USAGE_MAX_NUM      = 4,
};
/**
 * @brief Defines all the audio mic bst gain values.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	AMEBA_AUDIO_MICBST_GAIN_0DB          = 0,
	AMEBA_AUDIO_MICBST_GAIN_5DB          = 1,
	AMEBA_AUDIO_MICBST_GAIN_10DB         = 2,
	AMEBA_AUDIO_MICBST_GAIN_15DB         = 3,
	AMEBA_AUDIO_MICBST_GAIN_20DB         = 4,
	AMEBA_AUDIO_MICBST_GAIN_25DB         = 5,
	AMEBA_AUDIO_MICBST_GAIN_30DB         = 6,
	AMEBA_AUDIO_MICBST_GAIN_35DB         = 7,
	AMEBA_AUDIO_MICBST_GAIN_40DB         = 8,
	AMEBA_AUDIO_MICBST_GAIN_MAX_NUM      = 9,
};

/**
 * @brief Defines all the audio playback devices.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** play through speaker */
	AMEBA_AUDIO_DEVICE_SPEAKER         = 0,
	/** play through headphone*/
	AMEBA_AUDIO_DEVICE_HEADPHONE       = 1,
	/** play through I2S*/
	AMEBA_AUDIO_DEVICE_I2S             = 2,
	AMEBA_AUDIO_DEVICE_MAX_NUM         = 3,
};

/**
 * @brief Defines all the audio capture sources.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** microphones */
	AMEBA_AUDIO_IN_MIC                 = 0,
	/** play through headphone*/
	AMEBA_AUDIO_IN_I2S                 = 1,
	AMEBA_AUDIO_IN_MAX_NUM             = 2,
};

/**
 * @brief Defines all the audio stream DMA mode.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	/** DMA interrupt mode */
	AMEBA_AUDIO_DMA_IRQ_MODE         = 0,
	/** DMA no interrupt mode */
	AMEBA_AUDIO_DMA_NOIRQ_MODE       = 1,
};

/**
 * @brief Defines all the audio micbst type options.
 *
 * @since 1.0
 * @version 1.0
 */
enum {
	AMEBA_AUDIO_MICBST_LINEIN    = 0,
	AMEBA_AUDIO_MICBST_MICIN     = 1,
};

#ifdef __cplusplus
}
#endif


#endif // AMEBA_COMPONENT_AUDIO_HAL_SRC_INCLUDE_AMEBA_AUDIO_TYPES_H
/** @} */