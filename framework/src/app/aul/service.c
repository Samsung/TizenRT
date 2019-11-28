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

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <app/bundle.h>
#include <glib.h>
#include <string.h>

#include "aul.h"
#include "aul_api.h"
#include "aul_svc.h"
#include "aul_svc_priv_key.h"

static int __set_bundle(bundle *b, const char *key, const char *value)
{
	const char *val = NULL;

	val = bundle_get_val(b, key);
	if (val) {
		if (bundle_del(b, key) != 0)
			return AUL_SVC_RET_ERROR;
	}

	if (!value)
		return AUL_SVC_RET_EINVAL;

	if (bundle_add(b, key, value) != 0)
		return AUL_SVC_RET_ERROR;

	// _D("__set_bundle");

	return AUL_SVC_RET_OK;
}

static int __set_bundle_array(bundle *b, const char *key,
				const char **value, int len)
{

	int type;
	type = aul_svc_data_is_array(b, key);

	if (type == 1) {
		if (bundle_del(b, key) != 0)
			return AUL_SVC_RET_ERROR;
	}

	if (!value)
		return AUL_SVC_RET_EINVAL;

	if (bundle_add_str_array(b, key, value, len) != 0)
		return AUL_SVC_RET_ERROR;

	// _D("__set_bundle_array");

	return AUL_SVC_RET_OK;
}

API int aul_svc_set_operation(bundle *b, const char *operation)
{
	if (b == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	return __set_bundle(b, AUL_SVC_K_OPERATION, operation);
}

API int aul_svc_set_uri(bundle *b, const char *uri)
{
	if (b == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	return __set_bundle(b, AUL_SVC_K_URI, uri);
}

API int aul_svc_set_mime(bundle *b, const char *mime)
{
	if (b == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	return __set_bundle(b, AUL_SVC_K_MIME, mime);
}

API int aul_svc_add_data(bundle *b, const char *key, const char *val)
{
	if (b == NULL || key == NULL)
		return AUL_SVC_RET_EINVAL;

	/* check key for data */
	/******************/

	return __set_bundle(b, key, val);
}

API int aul_svc_add_data_array(bundle *b, const char *key,
				const char **val_array, int len)
{
	if (b == NULL || key == NULL)
		return AUL_SVC_RET_EINVAL;

	/* check key for data */
	/******************/

	return __set_bundle_array(b, key, val_array, len);
}

API int aul_svc_set_pkgname(bundle *b, const char *pkg_name)
{
	if (b == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	return __set_bundle(b, AUL_SVC_K_PKG_NAME, pkg_name);
}

API int aul_svc_set_appid(bundle *b, const char *appid)
{
	char *alias_id = NULL;
	int ret;

	if (b == NULL || appid == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	// alias_id = __get_alias_appid((char *)appid);
	if (alias_id == NULL) {
		ret = __set_bundle(b, AUL_SVC_K_PKG_NAME, appid);
	} else {
		ret = __set_bundle(b, AUL_SVC_K_PKG_NAME, alias_id);
		free(alias_id);
		alias_id = NULL;
	}

	return ret;
}

API int aul_svc_set_category(bundle *b, const char *category)
{
	if (b == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	return __set_bundle(b, AUL_SVC_K_CATEGORY, category);
}

API int aul_svc_set_launch_mode(bundle *b, const char *mode)
{
	if (b == NULL) {
		// _E("bundle is NULL");
		return AUL_SVC_RET_EINVAL;
	}

	return __set_bundle(b, AUL_SVC_K_LAUNCH_MODE, mode);
}

API const char *aul_svc_get_operation(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_OPERATION);
}

API const char *aul_svc_get_uri(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_URI);
}

API const char *aul_svc_get_mime(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_MIME);
}

API const char *aul_svc_get_data(bundle *b, const char *key)
{
	return bundle_get_val(b, key);
}

API const char **aul_svc_get_data_array(bundle *b, const char *key, int *len)
{
	return bundle_get_str_array(b, key, len);
}

API const char *aul_svc_get_pkgname(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_PKG_NAME);
}

API const char *aul_svc_get_appid(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_PKG_NAME);
}

API const char *aul_svc_get_category(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_CATEGORY);
}

API const char *aul_svc_get_launch_mode(bundle *b)
{
	return bundle_get_val(b, AUL_SVC_K_LAUNCH_MODE);
}
