#ifndef INCLUDE_ARTIK_MODULE_H_
#define INCLUDE_ARTIK_MODULE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_types.h"
#include "artik_error.h"

	/*! \file artik_module.h
	 *
	 *  \brief Module management definition
	 *
	 *  Definitions and functions for managing
	 *  the various modules exposed by the API
	 *
	 *  \example module_test/artik_module_test.c
	 */

	/*!
	 *  \brief Maximum length of version string
	 */
#define MAX_VERSION_STRING		16

	/*!
	 *  \brief Maximum length of module names string
	 */
#define MAX_MODULE_NAME			32

	/*!
	 *  \brief Maximum length of platform names string
	 */
#define MAX_PLATFORM_NAME		64

	/*!
	 *  \brief Type for module operations
	 */
	typedef void *artik_module_ops;

	/*!
	 *  \brief IDs of all the available modules
	 */
	typedef enum {
		ARTIK_MODULE_LOG = 0,
		ARTIK_MODULE_LOOP,
		ARTIK_MODULE_GPIO,
		ARTIK_MODULE_I2C,
		ARTIK_MODULE_SERIAL,
		ARTIK_MODULE_PWM,
		ARTIK_MODULE_ADC,
		ARTIK_MODULE_HTTP,
		ARTIK_MODULE_CLOUD,
		ARTIK_MODULE_WIFI,
		ARTIK_MODULE_MEDIA,
		ARTIK_MODULE_TIME,
		ARTIK_MODULE_SECURITY,
		ARTIK_MODULE_SPI,
		ARTIK_MODULE_BLUETOOTH,
		ARTIK_MODULE_SENSOR,
		ARTIK_MODULE_ZIGBEE,
		ARTIK_MODULE_NETWORK,
		ARTIK_MODULE_WEBSOCKET,
		ARTIK_MODULE_LWM2M,
		ARTIK_MODULE_MQTT
	} artik_module_id_t;

	/*!
	 *  \brief Structure defining the API version information
	 */
	typedef struct {
		/*!
		 *  \brief Major version number
		 */
		unsigned int major;
		/*!
		 *  \brief Minor version number
		 */
		unsigned int minor;
		/*!
		 *  \brief Patch version number
		 */
		unsigned int patch;
		/*!
		 *  \brief Version exposed as a string
		 */
		char version[MAX_VERSION_STRING];
	} artik_api_version;

	/*!
	 *  \brief Structure defining the elements of a module
	 */
	typedef struct {
		/*!
		 *  \brief Module ID
		 */
		artik_module_id_t id;
		/*!
		 *  \brief Module name string
		 */
		char *name;
		/*!
		 *	\brief Object name where the module is in
		 */
		char *object;
	} artik_api_module;

	/*!
	 *  \brief Get API version
	 *
	 *  \param[out] version String returned by the function. Must be preallocated
	 *            for a size at least MAX_VERSION_STRING bytes long.
	 *
	 *  \return S_OK always
	 */
	artik_error artik_get_api_version(artik_api_version *version);

	/*!
	 *  \brief Get API module operations structure
	 *
	 *  \param[in] name String containing the name of the module to operate on.
	 *
	 *  \return a pointer to a structure containing the module operations.
	 */
	artik_module_ops artik_request_api_module(const char *name);

	/*!
	 *  \brief Release API module operations structure
	 *
	 *  \param[in] module Pointer on the address of the module symbol to release.
	 *
	 *  \return S_OK on success, otherwise error code.
	 */
	artik_error artik_release_api_module(const artik_module_ops module);

	/*!
	 *  \brief Get platform ID
	 *
	 *  \return ID of the current platform
	 */
	int artik_get_platform(void);

	/*!
	 *  \brief Get platform name
	 *
	 *  \param[out] name String returned by the function with the name of the
	 *            platform the API has been built for. Must be preallocated
	 *            for a size at least MAX_PLATFORM_NAME bytes long.
	 *
	 *  \return S_OK always
	 */
	artik_error artik_get_platform_name(char *name);

	/*!
	 *  \brief Get modules available for the platform
	 *
	 *  \param[out] modules Pointer to an array of module definition structures for each
	 *            module available for the platform. This array does not need
	 *            to be preallocated, it points to a static array.
	 *  \param[out] num  Number of modules structures available in the array. This value
	 *            shall be used to browse through the modules array.
	 *
	 *  \return S_OK always
	 */
	artik_error artik_get_available_modules(artik_api_module **modules, int *num);

	/*!
	 *  \brief Check if a specific module is available for the platform
	 *
	 *  \param[in] id ID of the module whose availability is to be checked.
	 *
	 *  \return true if the module is available, false otherwise
	 */
	bool artik_is_module_available(artik_module_id_t id);

	/*!
	 *  \brief Get device information
	 *
	 *  \return JSON string filled up by the function and containing information
	 *            about the current device. String is allocated by the function
	 *            and must be freed by the calling function.
	 *
	 */
	char *artik_get_device_info(void);

#ifdef __cplusplus
}
#endif
#endif				/* INCLUDE_ARTIK_MODULE_H_ */
