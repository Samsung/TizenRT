/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/**
 * @defgroup Preference Preference
 * @ingroup Preference
 * @brief Provides APIs for Preference
 * @{
 */
/**
 * @file preference/preference.h
 */
#ifndef __PREFERENCE_H__
#define __PREFERENCE_H__

enum preference_result_error_e {
	PREFERENCE_IO_ERROR = -1,
	PREFERENCE_INVALID_PARAMETER = -2,
	PREFERENCE_PATH_NOT_FOUND = -3,
	PREFERENCE_OUT_OF_MEMORY = -4,
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/**
 * @brief Set int value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value an int value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_set_int(char *key, int value);

/**
 * @brief Set double value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a double value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_set_double(char *key, double value);

/**
 * @brief Set bool value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a bool value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_set_bool(char *key, bool value);

/**
 * @brief Set string value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a string value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_set_string(char *key, char *value);

/**
 * @brief Get int value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value an int value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_get_int(char *key, int *value);

/**
 * @brief Get double value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a double value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_get_double(char *key, double *value);

/**
 * @brief Get bool value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a bool value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_get_bool(char *key, bool *value);

/**
 * @brief Get string value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a string value for the given key
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_get_string(char *key, char **value);

/**
 * @brief Remove the value with the given key from the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to remove
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_remove(char *key);

/**
 * @brief Remove all key-value pairs from the preference
 * @details @b #include <preference/preference.h>
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_remove_all(void);

/**
 * @brief Check whether the given key exists in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to check
 * @param[out] existing true if the key exist, otherwise false
 * @return On success, OK is returned. On failure, ERROR is returned.
 * @since TizenRT v2.0 PRE
 */
int preference_is_existing(char *key, bool *existing);
#endif
/**
 * @}
 */
