/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <app/bundle.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif


/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_DEFAULT        "http://tizen.org/appcontrol/operation/default"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_EDIT           "http://tizen.org/appcontrol/operation/edit"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_VIEW           "http://tizen.org/appcontrol/operation/view"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_PICK           "http://tizen.org/appcontrol/operation/pick"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_CREATE_CONTENT     "http://tizen.org/appcontrol/operation/create_content"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_CALL           "http://tizen.org/appcontrol/operation/call"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_SEND           "http://tizen.org/appcontrol/operation/send"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_SEND_TEXT      "http://tizen.org/appcontrol/operation/send_text"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_DIAL           "http://tizen.org/appcontrol/operation/dial"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_SEARCH         "http://tizen.org/appcontrol/operation/search"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_DOWNLOAD       "http://tizen.org/appcontrol/operation/download"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_SHARE          "http://tizen.org/appcontrol/operation/share"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_MULTI_SHARE    "http://tizen.org/appcontrol/operation/multi_share"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_SHARE_TEXT     "http://tizen.org/appcontrol/operation/share_text"
/** AUL_SVC_OPERATION_TYPE*/
#define AUL_SVC_OPERATION_COMPOSE        "http://tizen.org/appcontrol/operation/compose"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_LOCATION       "http://tizen.org/appcontrol/operation/configure/location"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_FONT_TYPE      "http://tizen.org/appcontrol/operation/configure/font/type"
/** AUL_SVC OPERATION TYPE*/
#define AUL_SVC_OPERATION_FONT_SIZE      "http://tizen.org/appcontrol/operation/configure/font/size"
#define AUL_SVC_OPERATION_LAUNCH_ON_EVENT    "http://tizen.org/appcontrol/operation/launch_on_event"



/** AUL_SVC DATA SUBJECT*/
#define AUL_SVC_DATA_SUBJECT     "http://tizen.org/appcontrol/data/subject"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_TO          "http://tizen.org/appcontrol/data/to"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_CC          "http://tizen.org/appcontrol/data/cc"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_BCC         "http://tizen.org/appcontrol/data/bcc"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_TEXT        "http://tizen.org/appcontrol/data/text"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_TITLE       "http://tizen.org/appcontrol/data/title"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_SELECTED        "http://tizen.org/appcontrol/data/selected"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_KEYWORD     "http://tizen.org/appcontrol/data/keyword"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_PATH        "http://tizen.org/appcontrol/data/path"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_SELECTION_MODE  "http://tizen.org/appcontrol/data/selection_mode"
/** AUL_SVC DATA TYPE*/
#define AUL_SVC_DATA_RETURN_RESULT   "http://tizen.org/appcontrol/data/return_result"

/** AUL SVC internal private key */
#define AUL_SVC_K_URI_R_INFO        "__AUL_SVC_URI_R_INFO__"

#define AUL_SVC_K_SELECTOR_EXTRA_LIST   "http://tizen.org/appcontrol/data/selector_extra_list"

/** AUL SVC internal private key */
#define AUL_SVC_OPERATION_LAUNCH_WIDGET         "http://tizen.org/appcontrol/operation/launch_widget"

#ifdef _APPFW_FEATURE_MULTI_INSTANCE
#define AUL_SVC_K_MULTI_INSTANCE    "multi_instance"
#endif

#define APP_SELECTOR "org.tizen.app-selector"
#define SHARE_PANEL "org.tizen.share-panel"

/** Internal operation for launching application which is other zone */
#define AUL_SVC_OPERATION_JUMP  "http://tizen.org/appcontrol/operation/jump"
#define AUL_SVC_K_JUMP_ZONE_NAME    "__K_JUMP_DOMAIN_NAME__"
#define AUL_SVC_K_JUMP_ORIGIN_OPERATION "__K_JUMP_ORIGIN_OPERATION__"
#define AUL_SVC_K_FOCUS_ZONE "__K_FOCUS_ZONE__"
#define AUL_SVC_K_LAUNCH_RESULT_APP_STARTED "__K_LAUNCH_RESULT_APP_STARTED__"
#define AUL_SVC_K_CAN_BE_LEADER "__K_CAN_BE_LEADER__"
#define AUL_SVC_K_REROUTE "__K_REROUTE__"
#define AUL_SVC_K_SHIFT_WINDOW "__K_SHIFT_WINDOW"
#define AUL_SVC_K_RECYCLE "__K_RECYCLE"
#define AUL_SVC_K_RELOCATE_BELOW "__K_RELOCATE_BELOW"
#define AUL_SVC_K_BG_LAUNCH "__K_BG_LAUNCH"
#define AUL_SVC_K_SPLASH_SCREEN "__K_SPLASH_SCREEN"
#define AUL_SVC_K_ALIAS_INFO "__K_ALIAS_INFO"

#define PAD_LOADER_ID_DIRECT	1

/**
 * @brief Return values in appsvc.
 */
typedef enum _aul_svc_return_val {
	AUL_SVC_RET_ECANCELED = -8,	/**< Operation is canceled */
	AUL_SVC_RET_EREJECTED = -7,  /**< application launch rejected */
	AUL_SVC_RET_ETERMINATING = -6,   /**< application terminating */
	AUL_SVC_RET_EILLACC = -5,        /**< Illegal Access */
	AUL_SVC_RET_ELAUNCH = -4,        /**< Failure on launching the app */
	AUL_SVC_RET_ENOMATCH = -3,       /**< No matching result Error */
	AUL_SVC_RET_EINVAL = -2,         /**< Invalid argument */
	AUL_SVC_RET_ERROR = -1,          /**< General error */
	AUL_SVC_RET_OK = 0           /**< General success */
} aul_svc_return_val;


/**
 * @brief result values in appsvc.
 */
typedef enum _aul_svc_result_val {
	AUL_SVC_RES_CANCEL = -2,     /**< Cancel by system */
	AUL_SVC_RES_NOT_OK = -1,     /**< Fail by user */
	AUL_SVC_RES_OK = 0       /**< Success by user */
} aul_svc_result_val;


/**
 * @brief aul_svc_res_fn is appsvc result function
 * @param[out]  b           result bundle
 * @param[out]  request_code    request code
 * @param[out]  result          result value
 * @param[out]  data        user-supplied data
*/
typedef void (*aul_svc_res_fn)(bundle *b, int request_code, aul_svc_result_val result, void *data);

/**
 * @brief Called when the result of the launch request is delivered.
 *
 * @param[in]   request_code    The request code
 * @param[in]   result          The result, the process ID of the callee on success otherwise a negative error value
 * @param[in]   user_data       The user data passed from the callback registration function
 */
typedef void (*aul_svc_err_cb)(int request_code, int result, void *user_data);

/**
 * @brief iterator function running with aul_svc_get_list
 * @param[out]  appid       appid retreived by aul_svc_get_list
 * @param[out]  data        user-supplied data
*/
typedef int (*aul_svc_info_iter_fn)(const char *appid, void *data);

typedef int (*aul_svc_host_res_fn)(void *data);

/**
 * @par Description:
 * This function sets an operation to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] operation operation
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks An application must call this function before using aul_svc_run_service API.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_VIEW);
}
 * @endcode
 *
 */
int aul_svc_set_operation(bundle *b, const char *operation);

/**
 * @par Description:
 * This function sets an uri to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] uri uri
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_VIEW);
    aul_svc_set_uri(b,"http://www.samsung.com");
}
 * @endcode
 *
 */
int aul_svc_set_uri(bundle *b, const char *uri);

/**
 * @par Description:
 * This function sets a mime-type to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] mime mime-type
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_PICK);
    aul_svc_set_mime(b,"image/jpg");
}
 * @endcode
 *
 */
int aul_svc_set_mime(bundle *b, const char *mime);

/**
 * @par Description:
 * This function sets an extra data to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] key key of extra data
 * @param[in] val data
 *
 * @return 0 if success, negative value(<0) if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_SEND);
    aul_svc_set_uri(b,"mailto:xxx1@xxx");
    aul_svc_add_data(b,AUL_SVC_DATA_CC,"xxx2@xxx");
}
 * @endcode
 *
 */
int aul_svc_add_data(bundle *b, const char *key, const char *val);

/**
 * @par Description:
 * This function sets an extra array data to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] key key of extra data
 * @param[in] val_array data
 * @param[in] len Length of array
 *
 * @return 0 if success, negative value(<0) if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;
    char *images[] = {"/opt/media/a.jpg", "/opt/media/b.jpg", "/opt/media/c.jpg"};

    b = bundle_create();

    aul_svc_add_data_array(b, AUL_SVC_DATA_SELECTED, images, 3);
}
 * @endcode
 *
 */
int aul_svc_add_data_array(bundle *b, const char *key, const char **val_array, int len);


/**
 * @par Description:
 * This function sets a package name to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] pkg_name package name for explict launch
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_PICK);
    aul_svc_set_mime(b,"image/jpg");
    aul_svc_set_pkgname(b, "org.tizen.mygallery");
}
 * @endcode
 *
 */
/* Deprecated API */
int aul_svc_set_pkgname(bundle *b, const char *pkg_name); // __attribute__((deprecated));


/**
 * @par Description:
 * This function sets a appid to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] appid application id for explict launch
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_PICK);
    aul_svc_set_mime(b,"image/jpg");
    aul_svc_set_appid(b, "org.tizen.mygallery");
}
 * @endcode
 *
 */
int aul_svc_set_appid(bundle *b, const char *appid);


/**
 * @par Description:
 * This function sets a appid to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] application category
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_VIEW);
    aul_svc_set_category(b, "http://tizen.org/category/app/browser");
}
 * @endcode
 *
 */
int aul_svc_set_category(bundle *b, const char *category);

/**
 * @par Description:
 * This API launch application based on appsvc.
 *
 * @param[in] b bundle to be passed to callee
 * @param[in] request_code request code
 * @param[in] cbfunc result callback function
 * @param[in] data user-supplied data passed to callback function
 *
 * @return callee's pid if success, negative value(<0) if fail
 * @retval callee's pid - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 * @retval AUL_SVC_RET_ENOMATCH - no matching result Error
 * @retval AUL_SVC_RET_ELAUNCH - failure on launching the app
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    bundle *b = NULL;
    static int num = 0;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_PICK);
    aul_svc_set_mime(b,"image/jpg");

    return aul_svc_run_service(b, 0, cb_func, (void*)NULL);
}
 * @endcode
 *
 */
int aul_svc_run_service(bundle *b, int request_code, aul_svc_res_fn cbfunc, void *data);
int aul_svc_run_service_for_uid(bundle *b, int request_code,
		aul_svc_res_fn cbfunc, void *data, uid_t uid);

/**
 * @par Description:
 * This API use to get application list that is matched with given bundle.
 *
 * @param[in] b bundle to resolve application
 * @param[in] iter_fn iterator function
 * @param[in] data user-supplied data for iter_fn
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 * @retval AUL_SVC_RET_ENOMATCH - no matching result Error
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

static int iter_fn(const char* appid, void *data)
{
    printf("\t==========================\n");
    printf("\t appid: %s\n", appid);
    printf("\t==========================\n");
    return 0;
}

...
{
    bundle *b = NULL;
    static int num = 0;

    b = bundle_create();

    aul_svc_set_operation(b, AUL_SVC_OPERATION_PICK);
    aul_svc_set_mime(b,"image/jpg");

    return aul_svc_get_list(b, iter_fn, (void*)NULL);
}
 * @endcode
 *
 */
int aul_svc_get_list(bundle *b, aul_svc_info_iter_fn iter_fn, void *data);
int aul_svc_get_list_for_uid(bundle *b, aul_svc_info_iter_fn iter_fn,
		void *data, uid_t uid);

/**
 * @par Description:
 * This API use to get default applications
 *
 * @param[in] iter_fn iterator function
 * @param[in] data user-supplied data for iter_fn
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 * @retval AUL_SVC_RET_ENOMATCH - no matching result Error
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

static int iter_fn(const char* appid, void *data)
{
    printf("\t==========================\n");
    printf("\t appid : %s\n", appid);
    printf("\t==========================\n");
    return 0;
}

...
{
    return aul_svc_get_all_defapps(iter_fn, (void*)NULL);
}
 * @endcode
 *
 */
int aul_svc_get_all_defapps(aul_svc_info_iter_fn iter_fn, void *data);
int aul_svc_get_all_defapps_for_uid(aul_svc_info_iter_fn iter_fn,
		void *data, uid_t uid);

/**
 * @par Description:
 * This function gets a operation from bundle.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for operation string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_operation(b);
}
 * @endcode
 *
 */
const char *aul_svc_get_operation(bundle *b);

/**
 * @par Description:
 * This function gets a uri from bundle.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for uri string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_uri(b);
}
 * @endcode
 *
 */
const char *aul_svc_get_uri(bundle *b);

/**
 * @par Description:
 * This function gets a mime-type from bundle.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for mime-type string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_mime(b);
}
 * @endcode
 *
 */
const char *aul_svc_get_mime(bundle *b);

/**
 * @par Description:
 * This function gets a package name from bundle.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for package name string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_pkgname(b);
}
 * @endcode
 *
 */
/* Deprecated API */
const char *aul_svc_get_pkgname(bundle *b); // __attribute__((deprecated));

/**
 * @par Description:
 * This function gets a application id from bundle.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for application id string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_appid(b);
}
 * @endcode
 *
 */
const char *aul_svc_get_appid(bundle *b);

/**
 * @par Description:
 * This function gets a application category from bundle.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for application category string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_category(b);
}
 * @endcode
 *
 */
const char *aul_svc_get_category(bundle *b);

/**
 * @par Description:
 * This function gets value from key.
 *
 * @param[in] b bundle object
 * @param[in] key key
 *
 * @return Pointer for value string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    char *val;
    val = aul_svc_get_data(b, AUL_SVC_DATA_CC);
}
 * @endcode
 *
 */
const char *aul_svc_get_data(bundle *b, const char *key);

/**
 * @par Description:
 * This function gets value from key.
 *
 * @param[in] b bundle object
 * @param[in] key key
 * @param[out] len length of array
 *
 * @return Pointer for value string array if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
	char **val_array;
	int len;
	char *val;

	if(aul_svc_data_is_array(b, AUL_SVC_DATA_SELECTED))
		val_array = aul_svc_get_data_array(b, AUL_SVC_DATA_SELECTED, &len);
	else
		val = aul_svc_get_data(b, AUL_SVC_DATA_SELECTED);
}
 * @endcode
 *
 */
const char **aul_svc_get_data_array(bundle *b, const char *key, int *len);

/**
 * @par Description:
 * This API create appsvc result bundle based on bundle received in reset event.
 *
 * @param[in] inb bundle received in reset event
 * @param[in] outb bundle to use for returning result
 *
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see aul_svc_send_result.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    struct appdata *ad = data;
    bundle* res_bundle;

    aul_svc_create_result_bundle(ad->b,&res_bundle);
    bundle_add(res_bundle, "result", "1");
    aul_svc_send_result(res_bundle, 0);
}
 * @endcode
 *
 */
int aul_svc_create_result_bundle(bundle *inb, bundle **outb);

/**
 * @par Description:
 * This API send appsvc result to caller with bundle.
 *
 * @param[in] b Result data in bundle format
 * @param[in] result result value
 *
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre aul_svc_create_result_bundle.
 * @post None.
 * @see aul_svc_send_result.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    struct appdata *ad = data;
    bundle* res_bundle;

    aul_svc_create_result_bundle(ad->b,&res_bundle);
    bundle_add(res_bundle, "result", "1");
    aul_svc_send_result(res_bundle, 0);
}
 * @endcode
 *
 */
int aul_svc_send_result(bundle *b, aul_svc_result_val result);

/**
 * @par Description:
 * This API set the default application(package name) associated with op, uri and mime-type.
 *
 * @param[in] op        operation
 * @param[in] mime_type mime-type
 * @param[in] scheme    scheme of uri
 * @param[in] defapp    default application
 *
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    aul_svc_set_defapp(AUL_SVC_OPERATION_VIEW, NULL,"http", "org.tizen.mybrowser");
}
 * @endcode
 *
 */
int aul_svc_set_defapp(const char *op, const char *mime_type, const char *uri,
						const char *defapp);
int aul_svc_set_defapp_for_uid(const char *op, const char *mime_type, const char *uri,
						const char *defapp, uid_t uid);

/**
 * @par Description:
 * This API unset the default application(package name) associated with op, uri and mime-type.
 *
 * @param[in] defapp    default application
 *
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    aul_svc_unset_defapp("org.tizen.test");
}
 * @endcode
 *
 */
int aul_svc_unset_defapp(const char *defapp);
int aul_svc_unset_defapp_for_uid(const char *defapp, uid_t uid);

/**
 * @par Description:
 * This API unset all of default applications associated with op, uri and mime-type.
 *
 *
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    aul_svc_unset_all_defapps();
}
 * @endcode
 *
 */
int aul_svc_unset_all_defapps();
int aul_svc_unset_all_defapps_for_uid(uid_t uid);

/**
 * @par Description:
 *  This API ask a application is default application or not.
 *
 * @param[in]   appid   application appid
 * @return  true / false
 * @retval  1   app_name is default application in appsvc.
 * @retval  0   app_name is NOT default application in appsvc.
 *
  * @pre None.
  * @post None.
  * @see None.
  * @remarks None.
  *
  * @par Sample code:
  * @code
#include <aul_svc.h>

 ...

 * int is_defapp_browser_app()
 * {
 *      return aul_svc_is_defapp("org.tizen.browser");
 * }
 *
 * @endcode
 * @remark
 *  None
*
*/
int aul_svc_is_defapp(const char *appid);
int aul_svc_is_defapp_for_uid(const char *pkg_name, uid_t uid);


/**
 * @par Description:
 *  This API ask a extra data is array or not.
 *
 * @param[in] b bundle object
 * @param[in] key key of extra data
 * @return  true / false
 * @retval  1   a extra data is array.
 * @retval  0   a extra data is not array.
 *
  * @pre None.
  * @post None.
  * @see None.
  * @remarks None.
  *
  * @par Sample code:
  * @code
#include <aul_svc.h>

 ...

 * int aul_svc_data_is_array(bundle *b, char *key)
 * {
 *      return aul_svc_data_is_array(b, key);
 * }
 *
 * @endcode
 * @remark
 *  None
*
*/
int aul_svc_data_is_array(bundle *b, const char *key);

int aul_svc_subapp_terminate_request_pid(int pid);

/**
 * @par Description:
 * This function sets an uri to launch application based on appsvc.
 *
 * @param[in] b bundle object
 * @param[in] char *mode
 *
 * @return 0 if success, negative value(<0) if fail
 * @retval AUL_SVC_RET_OK - success
 * @retval AUL_SVC_RET_ERROR - general error
 * @retval AUL_SVC_RET_EINVAL - invalid argument(content)
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    aul_svc_set_launch_mode(app_control->data, mode);
}
 * @endcode
 *
 */
int aul_svc_set_launch_mode(bundle *b, const char *mode);

/**
 * @par Description:
 * This function sets an uri to launch application based on appsvc.
 *
 * @param[in] b bundle object
 *
 * @return Pointer for launch mode string if success, NULL if fail
 *
 * @pre None.
 * @post None.
 * @see None.
 * @remarks None.
 *
 * @par Sample code:
 * @code
#include <aul_svc.h>

...
{
    aul_svc_get_launch_mode(app_control->data);
}
 * @endcode
 *
 */
const char *aul_svc_get_launch_mode(bundle *b);

int aul_svc_allow_transient_app(bundle *b, int wid);

int aul_svc_request_transient_app(bundle *b, int callee_wid,
		aul_svc_host_res_fn cbfunc, void *data);

int aul_svc_subscribe_launch_result(bundle *b, const char *event);

/**
 * @par Description:
 * This function sets loader ID into bundle.
 *
 * @param[in] b Bundle object
 * @param[in] loader_id Loader ID from aul_add_loader() or PAD_LOADER_ID_DIRECT to launch apps without loader
 *
 * @return 0 if success, negative value(<0) if fail
 * @see aul_add_loader, aul_remove_loader
 * @remarks This API is only for Appfw internally.
 */
int aul_svc_set_loader_id(bundle *b, int loader_id);

/**
 * @par Description:
 * This function sets loader name into bundle. Launchpad will assign a static slot to launch an app.
 *
 * @param[in] b Bundle object
 * @param[in] loader_name Loader name to assign
 *
 * @return 0 if success, negative value(<0) if fail
 * @remarks This API is only for Appfw internally.
 */
int aul_svc_set_loader_name(bundle *b, const char *loader_name);

/**
 * @par Description:
 * This API sets the background launch mode.
 * If the background launch mode is enabled, a window of the callee process doesn't appear on the screen.
 *
 * @param[in] b Bundle object
 * @param[in] enabled boolean (true / false)
 *
 * @return 0 if success, negative value(<0) if fail
 * @remarks This API is only for Appfw internally.
 */
int aul_svc_set_background_launch(bundle *b, int enabled);

/**
 * @par Description:
 * This API set the alias appid.
 *
 * @param[in] alias_appid	an alias application ID
 * @param[in] appid		an application ID
 * @return 0 if success, negative value(<0) if fail
 */
int aul_svc_set_alias_appid(const char *alias_appid, const char *appid);
int aul_svc_set_alias_appid_for_uid(const char *alias_appid,
		const char *appid, uid_t uid);

/**
 * @par Description:
 * This API unset the alias appid.
 *
 * @param[in] alias_appid an alias application ID
 *
 * @return 0 if success, negative value(<0) if fail
 */
int aul_svc_unset_alias_appid(const char *alias_appid);
int aul_svc_unset_alias_appid_for_uid(const char *alias_appid, uid_t uid);

/**
 * @par Description:
 * This API retrieves all alias information.
 *
 * @param[in]	callback	The callback function to be invoked
 * @param[in]	user_data	The user data to be passed to the callback function
 *
 * @return	0 if success, negative value(<0) if fail
 */
int aul_svc_foreach_alias_info(void (*callback)(const char *alias_appid,
			const char *appid, void *data), void *user_data);
int aul_svc_foreach_alias_info_for_uid(void (*callback)(const char *alias_appid,
			const char *appid, void *data),
		uid_t uid, void *user_data);

/**
 * @par Description:
 * This API activates the alias information based on the given appid.
 *
 * @param[in]	appid	an application ID
 * @return	0 if success, negative value(<0) if fail
 */
int aul_svc_enable_alias_info(const char *appid);
int aul_svc_enable_alias_info_for_uid(const char *appid, uid_t uid);

/**
 * @par Description:
 * This API deactivates the alias information based on the given appid.
 *
 * @param[in]	appid	an application ID
 * @return	0 if success, negative value(<0) if fail
 */
int aul_svc_disable_alias_info(const char *appid);
int aul_svc_disable_alias_info_for_uid(const char *appid, uid_t uid);

/**
 * @par Description:
 * This API gets an application ID by an alias application ID.
 *
 * @param[in]   alias_appid   An alias application ID
 * @param[out]  appid         An application ID
 *
 * @return      0 if success, negative value(<0) if fail
 */
int aul_svc_get_appid_by_alias_appid(const char *alias_appid, char **appid);
int aul_svc_get_appid_by_alias_appid_for_uid(const char *alias_appid,
		char **appid, uid_t uid);

/**
 * @par Description:
 * This API retrieves all alias information based on the given appid.
 *
 * @param[in]   callback        The callback function to be invoked
 * @param[in]   appid           An application ID
 * @param[in]   user_data       The user data to be passed to the callback function
 *
 * @return      0 if success, negative value(<0) if fail
 */
int aul_svc_foreach_alias_info_by_appid(int (*callback)(
			const char *alias_appid, const char *appid, void *data),
		const char *appid, void *user_data);
int aul_svc_foreach_alias_info_by_appid_for_uid(int (*callback)(
			const char *alias_appid, const char *appid, void *data),
		const char *appid, uid_t uid, void *user_data);

/**
 * @par Description:
 * This API retrieves all allowed information.
 *
 * @param[in]   callback        The callback function to be invoked
 * @param[in]   user_data       The user data to be passed to the callback function
 *
 * @return      0 if success, negative value(<0) if fail
 */
int aul_svc_foreach_allowed_info(int (*callback)(const char *appid,
			const char *allowed_appid, void *data),
		void *user_data);
int aul_svc_foreach_allowed_info_for_uid(int (*callback)(const char *appid,
			const char *allowed_appid, void *data),
		uid_t uid, void *user_data);

/**
 * @par Description:
 * This API retrieves all allowed information based on the given appid.
 *
 * @param[in]   callback        The callback function to be invoked
 * @param[in]   appid           An application ID
 * @param[in]   user_data       The user data to be passed to the callback function
 *
 * @return      0 if success, negative value(<0) if fail
 */
int aul_svc_foreach_allowed_info_by_appid(int (*callback)(
			const char *appid, const char *allowed_appid, void *data),
		const char *appid, void *user_data);
int aul_svc_foreach_allowed_info_by_appid_for_uid(int (*callback)(
			const char *appid, const char *allowed_appid, void *data),
		const char *appid, uid_t uid, void *user_data);

/**
 * @par Description:
 *      Gets the instance ID.
 *
 * @param[in]   b               Bundle object
 *
 * @return Pointer for application id string if success, NULL if fail
 */
const char *aul_svc_get_instance_id(bundle *b);

/**
 * @par Description:
 *      Sets the instance ID.
 *
 * @param[in]  b                Bundle object
 * @param[in]  instance ID      Instance ID
 *
 * @return 0 if success, negative value(<0) if fail
 */
int aul_svc_set_instance_id(bundle *b, const char *instance_id);

/**
 * @par Description:
 *      Sends the launch request asynchronous.
 *
 * @param[in]  b                Bundle object
 * @param[in]  request_code     Request Code
 * @param[in]  cbfunc           Callback function
 * @param[in]  data             User data
 *
 * @return     a pid of the callee on success,
 *             otherwise a negative error value
 */
int aul_svc_run_service_async(bundle *b, int request_code,
		aul_svc_res_fn cbfunc, void *data);
int aul_svc_run_service_async_for_uid(bundle *b, int request_code,
		aul_svc_res_fn cbfunc, void *data, uid_t uid);


/**
 * @brief Sends the launch request asynchronous.
 *
 * @param[in]   b               The bundle object
 * @param[in]   request_code    The request code
 * @param[in]   cbfunc          The reply callback function
 * @param[in]   err_cb          The result callback function
 * @param[in]   user_data       The user data to be passed to the callback function
 *
 * @return      a pid of the callee on success,
 *              otherwise a negative error value
 */
int aul_svc_send_launch_request(bundle *b, int request_code,
		aul_svc_res_fn cbfunc, aul_svc_err_cb err_cb,
		void *user_data);

/**
 * @brief Sends the launch request asynchronous.
 *
 * @param[in]   b               The bundle object
 * @param[in]   request_code    The request code
 * @param[in]   cbfunc          The reply callback function
 * @param[in]   err_cb          The result callback function
 * @param[in]   user_data       The user data to be passed to the callback function
 * @param[in]   uid             The user ID
 *
 * @return      a pid of the callee on success,
 *              otherwise a negative error value
 */
int aul_svc_send_launch_request_for_uid(bundle *b, int request_code,
		aul_svc_res_fn cbfunc, aul_svc_err_cb err_cb,
		void *user_data, uid_t uid);

#ifdef __cplusplus
}
#endif
