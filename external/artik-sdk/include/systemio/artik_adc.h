#ifndef	__ARTIK_ADC_H__
#define	__ARTIK_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_adc.h
	 *
	 *  \brief ADC module definition
	 *
	 *  Definitions and functions for accessing
	 *  to the ADC module and performing operations
	 *  on hardware IOs.
	 *
	 *  \example adc_test/artik_adc_test.c
	 */

	/*!
	 *  \brief Maximum length for ADC names
	 *
	 *  Maximum length allowed for user-friendly
	 *  names assigned when requesting a ADC.
	 */
#define MAX_NAME_LEN		64

	/*!
	 *  \brief ADC handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a ADC object.
	 */
	typedef void *artik_adc_handle;

	/*! \struct artik_adc_config
	 *  \brief ADC configuration structure
	 *
	 *  Structure containing the configuration elements
	 *  for a single requested ADC
	 */
	typedef struct {
		/*!
		 *  \brief Pin number of the ADC module.
		 */
		int pin_num;
		/*!
		 *  \brief Friendly name for the ADC module.
		 */
		char *name;
		/*!
		 *  \brief Pointer to data for internal use by the API.
		 */
		void *user_data;

	} artik_adc_config;

	/*! \struct artik_adc_module
	 *
	 *  \brief ADC module operations
	 *
	 *  Structure containing all the exposed operations exposed
	 *  by the module to operate on a ADC instance
	 */
	typedef struct {
		/*!
		 *  \brief Request an ADC instance
		 *
		 *  \param[in,out] handle Handle tied to the requested ADC instance returned by the function.
		 *  \param[in,out] config Configuration to apply to the requested ADC.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*request) (artik_adc_handle * handle,
				       artik_adc_config * config);
		/*!
		 *  \brief Release an ADC instance
		 *
		 *  \param[in] handle Handle tied to the requested ADC instance to be released.
		 *          This handle is returned by the 'request' function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release) (artik_adc_handle handle);
		/*!
		 *  \brief Get Value of an ADC instance
		 *
		 *  \param[in] handle Handle tied to the requested ADC instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *	\param[out] value Filled up with the value of the converted analog input
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*get_value) (artik_adc_handle handle, int *value);
	} artik_adc_module;

	extern artik_adc_module adc_module;

#ifdef __cplusplus
}
#endif
#endif				/* __ARTIK_ADC_H__ */
