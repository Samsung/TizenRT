/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#ifndef __FRAMEWORK_INCLUDE_PREFERENCE_PREFERENCE_H__
#define __FRAMEWORK_INCLUDE_PREFERENCE_PREFERENCE_H__

#include <stdbool.h>
#include <tinyara/preference.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
/**
 * @brief Set int value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value an int value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_set_int(const char *key, int value);

/**
 * @brief Set double value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a double value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_set_double(const char *key, double value);

/**
 * @brief Set bool value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a bool value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_set_bool(const char *key, bool value);

/**
 * @brief Set string value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a string value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_set_string(const char *key, char *value);

/**
 * @brief Set binary value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the preference value to set
 * @param[in] value a binary value for the given key
 * @param[in] len length of the binary value in bytes
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_set_binary(const char *key, void *value, int len);

/**
 * @brief Get int value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value an int value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_get_int(const char *key, int *value);

/**
 * @brief Get double value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a double value for the given key
 * @return On success, OK is returned. On failure, Ea negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_get_double(const char *key, double *value);

/**
 * @brief Get bool value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a bool value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_get_bool(const char *key, bool *value);

/**
 * @brief Get string value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a string value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_get_string(const char *key, char **value);

/**
 * @brief Get binary value with key in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to retrieve
 * @param[out] value a binary value for the given key
 * @param[out] len length of the binary value in bytes
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_get_binary(const char *key, void **value, int *len);

/**
 * @brief Remove the value with the given key from the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to remove
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_remove(const char *key);

/**
 * @brief Remove all key-value pairs from the preference
 * @details @b #include <preference/preference.h>
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_remove_all(void);

/**
 * @brief Check whether the given key exists in the preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to check
 * @param[out] existing true if the key exist, otherwise false
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_is_existing(const char *key, bool *existing);

/**
 * @brief Register callback to be invoked when value of the given key in the preference changes
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to monitor
 * @param[in] callback a callback function to register
 * @param[in] cb_data a user data to be passed to the callback function
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_set_changed_cb(const char *key, preference_changed_cb callback, void *cb_data);

/**
 * @brief Unregister the callback function
 * @details @b #include <preference/preference.h>
 * @param[in] key a name of the key to monitor
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_unset_changed_cb(const char *key);

/**
 * @brief Set int value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to store
 * @param[in] value an int value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_set_int(const char *key, int value);

/**
 * @brief Set double value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to store
 * @param[in] value a double value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_set_double(const char *key, double value);

/**
 * @brief Set bool value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to store
 * @param[in] value a bool value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_set_bool(const char *key, bool value);

/**
 * @brief Set string value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to store
 * @param[in] value a string value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_set_string(const char *key, char *value);

/**
 * @brief Set binary value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to store
 * @param[in] value a binary value for the given key
 * @param[in] len length of the binary value in bytes
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_set_binary(const char *key, void *value, int len);

/**
 * @brief Get int value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to retrieve
 * @param[out] value an int value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_get_int(const char *key, int *value);

/**
 * @brief Get double value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to retrieve
 * @param[out] value a double value for the given key
 * @return On success, OK is returned. On failure, Ea negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_get_double(const char *key, double *value);

/**
 * @brief Get bool value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to retrieve
 * @param[out] value a bool value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_get_bool(const char *key, bool *value);

/**
 * @brief Get string value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to retrieve
 * @param[out] value a string value for the given key
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_get_string(const char *key, char **value);

/**
 * @brief Get binary value with key path in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to retrieve
 * @param[out] value a binary value for the given key
 * @param[out] len length of the binary value in bytes
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_get_binary(const char *key, void **value, int *len);

/**
 * @brief Remove the value with the given key path from the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to remove
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_remove(const char *key);

/**
 * @brief Remove all key-value pairs from the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] path a directory path of the keys to remove
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_remove_all(char *path);

/**
 * @brief Check whether the given key path exists in the shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to check
 * @param[out] existing true if the key exist, otherwise false
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_is_existing(const char *key, bool *existing);

/**
 * @brief Register callback to be invoked when value of the given key in the shared preference changes
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to monitor
 * @param[in] callback a callback function to register
 * @param[in] cb_data a user data to be passed to the callback function
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_set_changed_cb(const char *key, preference_changed_cb callback, void *cb_data);

/**
 * @brief Unregister the callback function for shared preference
 * @details @b #include <preference/preference.h>
 * @param[in] key a full path of key to monitor
 * @return On success, OK is returned. On failure, a negative value defined in preference_result_error_e is returned.
 * @since TizenRT v3.1 PRE
 */
int preference_shared_unset_changed_cb(const char *key);

#ifdef __cplusplus
}
#endif

#endif
/**
 * @}
 */
