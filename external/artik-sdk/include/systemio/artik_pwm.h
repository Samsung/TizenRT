#ifndef	__ARTIK_PWM_H__
#define	__ARTIK_PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_pwm.h
	 *
	 *  \brief PWM module definition
	 *
	 *  Definitions and functions for accessing
	 *  to the PWM module and performing operations
	 *  on hardware IOs
	 *
	 *  \example pwm_test/artik_pwm_test.c
	 */

	/*!
	 *  \brief Maximum length for PWM names
	 *
	 *  Maximum length allowed for user-friendly
	 *  names assigned when requesting a PWM.
	 */
#define MAX_NAME_LEN		64

	/*!
	 *  \brief PWM handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a PWM object.
	 */
	typedef void *artik_pwm_handle;

	/*!
	 *  \brief PWM polarity type
	 *
	 *  Type for specifying the polarity of the cycle duty of the PWM module.
	 */
	typedef enum {
		ARTIK_PWM_POLR_NORMAL = 0,
		ARTIK_PWM_POLR_INVERT
	} artik_pwm_polarity_t;

	/*! \struct artik_pwm_config
	 *  \brief PWM configuration structure
	 *
	 *  Structure containing the configuration elements
	 *  for a single requested PWM
	 */
	typedef struct {
		/*!
		 *  \brief Pin number of the PWM module.
		 */
		int pin_num;
		/*!
		 *  \brief Friendly name for the PWM module.
		 */
		char *name;
		/*!
		 *  \brief Period of a duty cycle for the PWM module.
		 */
		unsigned int period;
		/*!
		 *  \brief Frequency of a duty cycle for the PWM module.
		 */
		unsigned int duty_cycle;
		/*!
		 *  \brief Polarity of a duty cycle (high/low) for the PWM module.
		 */
		artik_pwm_polarity_t polarity;
		/*!
		 *  \brief Pointer to data for internal use by the API.
		 */
		void *user_data;

	} artik_pwm_config;

	/*! \struct artik_pwm_module
	 *
	 *  \brief PWM module operations
	 *
	 *  Structure containing all the exposed operations exposed
	 *  by the module to operate on a PWM instance
	 */
	typedef struct {
		/*!
		 *  \brief Request a PWM instance
		 *
		 *  \param[in,out] handle Handle tied to the newly requested PWM instance.
		 *  \param[in,out] config Configuration to apply to the requested PWM.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*request) (artik_pwm_handle * handle,
				       artik_pwm_config * config);
		/*!
		 *  \brief Release a PWM instance
		 *
		 *  \param[in] handle Handle tied to the requested PWM instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release) (artik_pwm_handle handle);
		/*!
		 *  \brief Enable a PWM instance
		 *
		 *  \param[in] handle Handle tied to the requested PWM instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*enable) (artik_pwm_handle handle);
		/*!
		 *  \brief Disable a PWM instance
		 *
		 *  \param[in] handle Handle tied to the requested PWM instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*disable) (artik_pwm_handle handle);
		/*!
		 *  \brief Set the period of a PWM instance
		 *
		 *  \param[in] handle Handle tied to the requested PWM instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] period Time in nanoseconds corresponding to the full period (active and inactive time)
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*set_period) (artik_pwm_handle handle,
					  unsigned int period);
		/*!
		 *  \brief Set the  polarity of a PWM instance
		 *
		 *  \param[in] handle Handle tied to the requested PWM instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] polarity Normal or inversed polarity.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*set_polarity) (artik_pwm_handle handle,
					    artik_pwm_polarity_t polarity);
		/*!
		 *  \brief Set the duty cycle of the PWM instance
		 *
		 *  \param[in] handle Handle tied to the requested PWM instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] duty_cycle Time in nanoseconds corresponding to the active time
		 *                        of the signal over a period.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*set_duty_cycle) (artik_pwm_handle handle,
					      unsigned int duty_cycle);

	} artik_pwm_module;

	extern artik_pwm_module pwm_module;

#ifdef __cplusplus
}
#endif
#endif				/* __ARTIK_PWM_H__ */
