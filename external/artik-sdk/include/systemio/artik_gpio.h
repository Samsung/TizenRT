#ifndef INCLUDE_ARTIK_GPIO_H_
#define INCLUDE_ARTIK_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

	/*! \file artik_gpio.h
	 *
	 *  \brief GPIO module definition
	 *
	 *  Definitions and functions for accessing
	 *  the GPIO module and performing operations
	 *  on hardware IOs
	 *
	 *  \example gpio_test/artik_gpio_test.c
	 */

	/*!
	 *  \brief Maximum length for GPIO names
	 *
	 *  Maximum length allowed for user-friendly
	 *  names assigned when requesting a GPIO.
	 */
#define MAX_NAME_LEN	64

	/*!
	 *  \brief GPIO handle type
	 *
	 *  Handle type used to carry instance specific
	 *  information for a GPIO object
	 */
	typedef void *artik_gpio_handle;

	/*!
	 *  \brief GPIO ID type
	 *
	 *  ID type used to specify a single GPIO
	 */
	typedef unsigned int artik_gpio_id;

	/*!
	 *  \brief GPIO callback type
	 *
	 *  Callback prototype for GPIO change notification
	 */
	typedef void (*artik_gpio_callback)(void *user_data, int value);

	/*!
	 *  \brief GPIO direction type
	 *
	 *  Type for specifying the direction of the GPIO
	 */
	typedef enum {
		GPIO_OUT,
		GPIO_IN,
		GPIO_DIR_INVALID
	} artik_gpio_dir_t;

	/*!
	 *  \brief GPIO edge type
	 *
	 *  Type for specifying the edge type of the GPIO when used as
	 *  and interrupt
	 */
	typedef enum {
		GPIO_EDGE_NONE,
		GPIO_EDGE_RISING,
		GPIO_EDGE_FALLING,
		GPIO_EDGE_BOTH,
		GPIO_EDGE_INVALID
	} artik_gpio_edge_t;

	/*!
	 *  \brief GPIO configuration structure
	 *
	 *  Structure containing the configuration elements
	 *  for a single requested GPIO
	 */
	typedef struct {
		/*!
		 *  \brief ID of the GPIO to request
		 */
		artik_gpio_id id;
		/*!
		 *  \brief friendly name to assign to the GPIO
		 */
		char *name;
		/*!
		 *  \brief direction of the GPIO
		 */
		artik_gpio_dir_t dir;
		/*!
		 *  \brief edge to detect change state on. Ignored
		 *  when configured as an output.
		 */
		artik_gpio_edge_t edge;
		/*!
		 *  \brief initial value to set to the GPIO when
		 *  configured as an output. Ignored if configured
		 *  as an input.
		 */
		int initial_value;
		/*!
		 *  \brief pointer to data for internal use by the API.
		 */
		void *user_data;
	} artik_gpio_config;

	/*! \struct artik_gpio_module
	 *
	 *  \brief GPIO module operations
	 *
	 *  Structure containing all the exposed operations exposed
	 *  by the module to operate on a GPIO instance
	 */
	typedef struct {
		/*!
		 *  \brief Request a GPIO instance
		 *
		 *  \param[out] handle Handle tied to the requested GPIO instance returned by the function.
		 *  \param[in] config Configuration to apply to the requested GPIO.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*request)(artik_gpio_handle * handle, artik_gpio_config * config);
		/*!
		 *  \brief Release a GPIO instance
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to be released.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*release)(artik_gpio_handle handle);
		/*!
		 *  \brief Read the value of a GPIO instance
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to be read.
		 *                    This handle is returned by the \ref request function.
		 *
		 *  \return 1 if the signal is active, 0 if it is inactive
		 */
		int (*read)(artik_gpio_handle handle);
		/*!
		 *  \brief Write the value of a GPIO instance
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to be written
		 *                    This handle is returned by the \ref request function.
		 *  \param[in] value 1 to set the output active, 0 to set it inactive
		 *
		 *  \return S_OK on success, error code otherwise
		 */
		artik_error(*write)(artik_gpio_handle handle, int value);
		/*!
		 *  \brief Get the name of a GPIO instance
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to get the
		 *                    name of. This handle is returned by the \ref request function.
		 *
		 *  \return A string of MAX_NAME_LEN maximum length containing the user friendly
		 *				of the GPIO set at request time. This string is statically allocated
		 *              and does not need to be freed.
		 */
		char * (*get_name)(artik_gpio_handle handle);
		/*!
		 *  \brief Get the direction of a GPIO instance
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to get the direction
		 *                    of. This handle is returned by the \ref request function.
		 *
		 *  \return Direction of the GPIO
		 */
		artik_gpio_dir_t (*get_direction)(artik_gpio_handle handle);
		/*!
		 *  \brief Get the ID of a GPIO instance
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to get the
		 *                    ID of. This handle is returned by the \ref request function.
		 *
		 *  \return The ID of the GPIO
		 */
		artik_gpio_id (*get_id)(artik_gpio_handle handle);
		/*!
		 *  \brief Set a callback to be notified of GPIO state change
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance from which to monitor changes.
		 *					  This handle is returned by the \ref request function.
		 *  \param[in] callback Pointer to the callback function which will be called upon GPIO state
		 *						changes.
		 *  \param[in] user_data Pointer to user data that will be passed as a parameter to the callback
		 *
		 *	\return S_OK on success, error code otherwise
		 */
		artik_error(*set_change_callback)(artik_gpio_handle handle, artik_gpio_callback callback,
				void *user_data);
		/*!
		 *  \brief Unset a callback to stop being notified of GPIO state change
		 *
		 *  \param[in] handle Handle tied to the requested GPIO instance to which the callback
		 *                    was registered. This handle is returned by the \ref request function.
		 *
		 */
		void (*unset_change_callback)(artik_gpio_handle handle);
	} artik_gpio_module;

	extern const artik_gpio_module gpio_module;

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_GPIO_H_ */
