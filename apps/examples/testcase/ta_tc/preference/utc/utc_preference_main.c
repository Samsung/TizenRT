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

#include <tinyara/config.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <tinyara/preference.h>
#include <preference/preference.h>

#include "tc_common.h"

#define INT_KEY		"INTEGER"
#define DOUBLE_KEY	"DOUBLE"
#define BOOL_KEY	"BOOL"
#define STRING_KEY	"STRING"
#define BINARY_KEY	"BINARY"

#define SHARED_KEY_PATH		"utc/preference"
#define SHARED_INTKEY_PATH	"utc/preference/INTEGER"
#define SHARED_DOUBLEKEY_PATH	"utc/preference/DOUBLE"
#define SHARED_BOOLKEY_PATH	"utc/preference/BOOL"
#define SHARED_STRINGKEY_PATH	"utc/preference/STRING"
#define SHARED_BINARYKEY_PATH	"utc/preference/BINARY"

#define INT_VALUE	300
#define DOUBLE_VALUE	4.21
#define BOOL_VALUE	true
#define STRING_VALUE	"preference string value"

#define INVALID_KEY	"INVALIDKEY"
#define CB_DATA		"PREFERENCE_CBDATA"

struct bin_pref_val_s {
	int val1;
	char val2;
} BINARY_VALUE;

static bool cb_flag;

bool check_binary_value(struct bin_pref_val_s *v, struct bin_pref_val_s b)
{
	return ((v->val1 == b.val1) && (v->val2 == b.val2));
}

static preference_changed_cb value_changed_cb(const char *key, void *user_data)
{
	if (key != NULL && !strncmp(key, INT_KEY, strlen(key) + 1) && !strncmp(user_data, CB_DATA, strlen(user_data) + 1)) {
		cb_flag = true;
	}
}

static preference_changed_cb shared_value_changed_cb(const char *key, void *user_data)
{
	if (key != NULL && !strncmp(key, SHARED_INTKEY_PATH, strlen(key) + 1) && !strncmp(user_data, CB_DATA, strlen(user_data) + 1)) {
		cb_flag = true;
	}
}

static void utc_preference_set_int_p(void)
{
	int ret;

	ret = preference_set_int(INT_KEY, INT_VALUE);
	TC_ASSERT_EQ("preference_set_int", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_set_int_n(void)
{
	int ret;

	ret = preference_set_int(NULL, INT_VALUE);
	TC_ASSERT_EQ("preference_set_int", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_set_double_p(void)
{
	int ret;

	ret = preference_set_double(DOUBLE_KEY, DOUBLE_VALUE);
	TC_ASSERT_EQ("preference_set_double", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_set_double_n(void)
{
	int ret;

	ret = preference_set_double(NULL, DOUBLE_VALUE);
	TC_ASSERT_EQ("preference_set_double", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}


static void utc_preference_set_bool_p(void)
{
	int ret;

	ret = preference_set_bool(BOOL_KEY, BOOL_VALUE);
	TC_ASSERT_EQ("preference_set_bool", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_set_bool_n(void)
{
	int ret;

	ret = preference_set_bool(NULL, BOOL_VALUE);
	TC_ASSERT_EQ("preference_set_bool", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_set_string_p(void)
{
	int ret;

	ret = preference_set_string(STRING_KEY, STRING_VALUE);
	TC_ASSERT_EQ("preference_set_string", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_set_string_n(void)
{
	int ret;

	ret = preference_set_string(NULL, STRING_VALUE);
	TC_ASSERT_EQ("preference_set_string", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_set_string(STRING_KEY, NULL);
	TC_ASSERT_EQ("preference_set_string", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_set_binary_p(void)
{
	int ret;
	BINARY_VALUE.val1 = 123;
	BINARY_VALUE.val2 = 'x';

	ret = preference_set_binary(BINARY_KEY, (void *)&BINARY_VALUE, sizeof(BINARY_VALUE));
	TC_ASSERT_EQ("preference_set_binary", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_set_binary_n(void)
{
	int ret;
	BINARY_VALUE.val1 = 789;
	BINARY_VALUE.val2 = 'z';

	ret = preference_set_binary(BINARY_KEY, (void *)&BINARY_VALUE, 0);
	TC_ASSERT_EQ("preference_set_binary", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_set_binary(BINARY_KEY, NULL, sizeof(BINARY_VALUE));
	TC_ASSERT_EQ("preference_set_binary", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_set_binary(NULL, (void *)&BINARY_VALUE, sizeof(BINARY_VALUE));
	TC_ASSERT_EQ("preference_set_binary", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_int_p(void)
{
	int ret;
	int value;

	ret = preference_get_int(INT_KEY, &value);
	TC_ASSERT_EQ("preference_get_int", ret, OK);
	TC_ASSERT_EQ("preference_get_int", value, INT_VALUE);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_get_int_n(void)
{
	int ret;
	int value;

	ret = preference_get_int(NULL, &value);
	TC_ASSERT_EQ("preference_get_int", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_int(DOUBLE_KEY, &value);
	TC_ASSERT_EQ("preference_get_int", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_int(INVALID_KEY, &value);
	TC_ASSERT_EQ("preference_get_int", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_double_p(void)
{
	int ret;
	double value;

	ret = preference_get_double(DOUBLE_KEY, &value);
	TC_ASSERT_EQ("preference_get_double", ret, OK);
	TC_ASSERT_EQ("preference_get_double", value, DOUBLE_VALUE);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_double_n(void)
{
	int ret;
	double value;

	ret = preference_get_double(NULL, &value);
	TC_ASSERT_EQ("preference_get_double", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_double(INT_KEY, &value);
	TC_ASSERT_EQ("preference_get_double", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_double(INVALID_KEY, &value);
	TC_ASSERT_EQ("preference_get_double", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_bool_p(void)
{
	int ret;
	bool value;

	ret = preference_get_bool(BOOL_KEY, &value);
	TC_ASSERT_EQ("preference_get_bool", ret, OK);
	TC_ASSERT_EQ("preference_get_bool", value, BOOL_VALUE);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_get_bool_n(void)
{
	int ret;
	bool value;

	ret = preference_get_bool(NULL, &value);
	TC_ASSERT_EQ("preference_get_bool", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_bool(INT_KEY, &value);
	TC_ASSERT_EQ("preference_get_bool", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_bool(INVALID_KEY, &value);
	TC_ASSERT_EQ("preference_get_bool", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_string_p(void)
{
	int ret;
	char *value;

	ret = preference_get_string(STRING_KEY, &value);
	TC_ASSERT_NEQ("preference_get_string", value, NULL);
	TC_ASSERT_EQ_CLEANUP("preference_get_string", ret, OK, free(value));
	TC_ASSERT_EQ_CLEANUP("preference_get_string", strncmp(value, STRING_VALUE, strlen(value)), 0, free(value));

	/* Clean string value allocated from preference */
	free(value);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_string_n(void)
{
	int ret;
	char *value = NULL;

	ret = preference_get_string(NULL, &value);
	TC_ASSERT_EQ_CLEANUP("preference_get_string", value, NULL, free(value));
	TC_ASSERT_EQ("preference_get_string", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_string(INT_KEY, &value);
	TC_ASSERT_EQ_CLEANUP("preference_get_string", value, NULL, free(value));
	TC_ASSERT_EQ("preference_get_string", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_get_string(INVALID_KEY, &value);
	TC_ASSERT_EQ_CLEANUP("preference_get_string", value, NULL, free(value));
	TC_ASSERT_EQ("preference_get_string", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_binary_p(void)
{
	int ret;
	void *value = NULL;
	int len;

	BINARY_VALUE.val1 = 123;
	BINARY_VALUE.val2 = 'x';

	ret = preference_get_binary(BINARY_KEY, &value, &len);
	TC_ASSERT_NEQ("preference_get_binary", value, NULL);
	TC_ASSERT_EQ_CLEANUP("preference_get_binary", ret, OK, free(value));
	TC_ASSERT_EQ_CLEANUP("preference_get_binary", check_binary_value((struct bin_pref_val_s *)value, BINARY_VALUE), true, free(value));
	TC_ASSERT_EQ("preference_get_binary", len, sizeof(BINARY_VALUE));

	/* Clean binary value allocated from preference */
	free(value);

	TC_SUCCESS_RESULT();
}

static void utc_preference_get_binary_n(void)
{
	int ret;
	void *value = NULL;
	int len = -1;

	ret = preference_get_binary(NULL, &value, &len);
	TC_ASSERT_EQ_CLEANUP("preference_get_binary", value, NULL, free(value));
	TC_ASSERT_EQ("preference_get_binary", ret, PREFERENCE_INVALID_PARAMETER);
	TC_ASSERT_EQ("preference_get_binary", len, -1);

	ret = preference_get_binary(INT_KEY, &value, &len);
	TC_ASSERT_EQ_CLEANUP("preference_get_binary", value, NULL, free(value));
	TC_ASSERT_EQ("preference_get_binary", ret, PREFERENCE_INVALID_PARAMETER);
	TC_ASSERT_EQ("preference_get_binary", len, -1);

	ret = preference_get_binary(INVALID_KEY, &value, &len);
	TC_ASSERT_EQ_CLEANUP("preference_get_binary", value, NULL, free(value));
	TC_ASSERT_EQ("preference_get_binary", ret, PREFERENCE_KEY_NOT_EXIST);
	TC_ASSERT_EQ("preference_get_binary", len, -1);

	/* Clean binary value allocated from preference */
	free(value);

	TC_SUCCESS_RESULT();
}

static void utc_preference_set_changed_cb_p(void)
{
	int ret;
	cb_flag = false;

	/* Set callback for key changed */
	ret = preference_set_changed_cb(INT_KEY, value_changed_cb, CB_DATA);
	TC_ASSERT_EQ("preference_set_changed_cb", ret, OK);

	/* Change the value of key */
	ret = preference_set_int(INT_KEY, INT_VALUE + 1);
	TC_ASSERT_EQ("preference_set_int", ret, OK);

	sleep(1);

	TC_ASSERT_EQ("preference_set_changed_cb", cb_flag, true);

	TC_SUCCESS_RESULT();
}


static void utc_preference_set_changed_cb_n(void)
{
	int ret;

	ret = preference_set_changed_cb(INT_KEY, NULL, NULL);
	TC_ASSERT_EQ("preference_set_changed_cb", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_set_changed_cb(NULL, value_changed_cb, NULL);
	TC_ASSERT_EQ("preference_set_changed_cb", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_set_changed_cb(INVALID_KEY, value_changed_cb, NULL);
	TC_ASSERT_EQ("preference_set_changed_cb", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_unset_changed_cb_p(void)
{
	int ret;
	cb_flag = false;

	/* Unset callback for key changed */
	ret = preference_unset_changed_cb(INT_KEY);
	TC_ASSERT_EQ("preference_unset_changed_cb", ret, OK);

	/* Change the value of key */
	ret = preference_set_int(INT_KEY, INT_VALUE + 1);
	TC_ASSERT_EQ("preference_unset_changed_cb", ret, OK);

	sleep(1);

	TC_ASSERT_EQ("preference_unset_changed_cb", cb_flag, false);

	TC_SUCCESS_RESULT();
}

static void utc_preference_unset_changed_cb_n(void)
{
	int ret;

	ret = preference_unset_changed_cb(NULL);
	TC_ASSERT_EQ("preference_unset_changed_cb", ret, PREFERENCE_INVALID_PARAMETER);

	/* Try to unset with key which is valid but hasn't registered callback */
	ret = preference_unset_changed_cb(STRING_KEY);
	TC_ASSERT_EQ("preference_unset_changed_cb", ret, PREFERENCE_NOT_REGISTERED);

	/* Try to unset with invalid key */
	ret = preference_unset_changed_cb(INVALID_KEY);
	TC_ASSERT_EQ("preference_unset_changed_cb", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_remove_p(void)
{
	int ret;
	int int_value;

	ret = preference_remove(INT_KEY);
	TC_ASSERT_EQ("preference_remove", ret, OK);

	/* Try to get value with already removed key, it should be failed */
	ret = preference_get_int(INT_KEY, &int_value);
	TC_ASSERT_NEQ("preference_get_int", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_remove_n(void)
{
	int ret;

	ret = preference_remove(NULL);
	TC_ASSERT_EQ("preference_remove", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_remove(INVALID_KEY);
	TC_ASSERT_EQ("preference_remove", ret, PREFERENCE_KEY_NOT_EXIST);

	/* Try to remove again with already removed key, it should be failed */
	ret = preference_remove(INT_KEY);
	TC_ASSERT_NEQ("preference_remove", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_is_existing_p(void)
{
	int ret;
	bool is_existing = false;

	/* Check whether stored key is existing, it should be return true */
	ret = preference_is_existing(DOUBLE_KEY, &is_existing);
	TC_ASSERT_EQ("preference_is_existing", ret, OK);
	TC_ASSERT_EQ("preference_is_existing", is_existing, true);

	/* Check whether removed key is existing, it should be return false through passed paramter */
	ret = preference_is_existing(INT_KEY, &is_existing);
	TC_ASSERT_EQ("preference_is_existing", ret, OK);
	TC_ASSERT_EQ("preference_is_existing", is_existing, false);	

	TC_SUCCESS_RESULT();
}

static void utc_preference_is_existing_n(void)
{
	int ret;
	bool is_existing;

	ret = preference_is_existing(NULL, &is_existing);
	TC_ASSERT_EQ("preference_is_existing", ret, PREFERENCE_INVALID_PARAMETER);

	is_existing = true;
	/* Check whether invalid key is existing, it should be return false through passed paramter */
	ret = preference_is_existing(INVALID_KEY, &is_existing);
	TC_ASSERT_EQ("preference_is_existing", ret, OK);
	TC_ASSERT_EQ("preference_is_existing", is_existing, false);

	TC_SUCCESS_RESULT();
}

static void utc_preference_remove_all_p(void)
{
	int ret;

	ret = preference_remove_all();
	TC_ASSERT_EQ("preference_remove_all", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_int_p(void)
{
	int ret;

	ret = preference_shared_set_int(SHARED_INTKEY_PATH, INT_VALUE);
	TC_ASSERT_EQ("preference_shared_set_int", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_int_n(void)
{
	int ret;

	ret = preference_shared_set_int(NULL, INT_VALUE);
	TC_ASSERT_EQ("preference_shared_set_int", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_double_p(void)
{
	int ret;

	ret = preference_shared_set_double(SHARED_DOUBLEKEY_PATH, DOUBLE_VALUE);
	TC_ASSERT_EQ("preference_shared_set_double", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_double_n(void)
{
	int ret;

	ret = preference_shared_set_double(NULL, DOUBLE_VALUE);
	TC_ASSERT_EQ("preference_shared_set_double", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}


static void utc_preference_shared_set_bool_p(void)
{
	int ret;

	ret = preference_shared_set_bool(SHARED_BOOLKEY_PATH, BOOL_VALUE);
	TC_ASSERT_EQ("preference_shared_set_bool", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_bool_n(void)
{
	int ret;

	ret = preference_shared_set_bool(NULL, BOOL_VALUE);
	TC_ASSERT_EQ("preference_shared_set_bool", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_string_p(void)
{
	int ret;

	ret = preference_shared_set_string(SHARED_STRINGKEY_PATH, STRING_VALUE);
	TC_ASSERT_EQ("preference_shared_set_string", ret, OK);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_string_n(void)
{
	int ret;

	ret = preference_shared_set_string(NULL, STRING_VALUE);
	TC_ASSERT_EQ("preference_shared_set_string", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_set_string(SHARED_STRINGKEY_PATH, NULL);
	TC_ASSERT_EQ("preference_shared_set_string", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_binary_p(void)
{
	int ret;
	BINARY_VALUE.val1 = 123;
	BINARY_VALUE.val2 = 'x';

	ret = preference_shared_set_binary(BINARY_KEY, (void *)&BINARY_VALUE, sizeof(BINARY_VALUE));
	TC_ASSERT_EQ("preference_shared_set_binary", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_binary_n(void)
{
	int ret;
	BINARY_VALUE.val1 = 789;
	BINARY_VALUE.val2 = 'z';

	ret = preference_shared_set_binary(BINARY_KEY, (void *)&BINARY_VALUE, 0);
	TC_ASSERT_EQ("preference_shared_set_binary", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_set_binary(BINARY_KEY, NULL, sizeof(BINARY_VALUE));
	TC_ASSERT_EQ("preference_shared_set_binary", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_set_binary(NULL, (void *)&BINARY_VALUE, sizeof(BINARY_VALUE));
	TC_ASSERT_EQ("preference_shared_set_binary", ret, PREFERENCE_INVALID_PARAMETER);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_int_p(void)
{
	int ret;
	int value;

	ret = preference_shared_get_int(SHARED_INTKEY_PATH, &value);
	TC_ASSERT_EQ("preference_shared_get_int", ret, OK);
	TC_ASSERT_EQ("preference_shared_get_int", value, INT_VALUE);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_int_n(void)
{
	int ret;
	int value;

	ret = preference_shared_get_int(NULL, &value);
	TC_ASSERT_EQ("preference_shared_get_int", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_int(SHARED_DOUBLEKEY_PATH, &value);
	TC_ASSERT_EQ("preference_shared_get_int", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_int(INVALID_KEY, &value);
	TC_ASSERT_EQ("preference_shared_get_int", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_double_p(void)
{
	int ret;
	double value;

	ret = preference_shared_get_double(SHARED_DOUBLEKEY_PATH, &value);
	TC_ASSERT_EQ("preference_shared_get_double", ret, OK);
	TC_ASSERT_EQ("preference_shared_get_double", value, DOUBLE_VALUE);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_double_n(void)
{
	int ret;
	double value;

	ret = preference_shared_get_double(NULL, &value);
	TC_ASSERT_EQ("preference_shared_get_double", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_double(SHARED_INTKEY_PATH, &value);
	TC_ASSERT_EQ("preference_shared_get_double", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_double(INVALID_KEY, &value);
	TC_ASSERT_EQ("preference_shared_get_double", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_bool_p(void)
{
	int ret;
	bool value;

	ret = preference_shared_get_bool(SHARED_BOOLKEY_PATH, &value);
	TC_ASSERT_EQ("preference_shared_get_bool", ret, OK);
	TC_ASSERT_EQ("preference_shared_get_bool", value, BOOL_VALUE);
	
	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_bool_n(void)
{
	int ret;
	bool value;

	ret = preference_shared_get_bool(NULL, &value);
	TC_ASSERT_EQ("preference_shared_get_bool", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_bool(SHARED_INTKEY_PATH, &value);
	TC_ASSERT_EQ("preference_shared_get_bool", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_bool(INVALID_KEY, &value);
	TC_ASSERT_EQ("preference_shared_get_bool", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_string_p(void)
{
	int ret;
	char *value;

	ret = preference_shared_get_string(SHARED_STRINGKEY_PATH, &value);
	TC_ASSERT_NEQ("preference_shared_get_string", value, NULL);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_string", ret, OK, free(value));
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_string", strncmp(value, STRING_VALUE, strlen(value)), 0, free(value));

	/* Clean string value allocated from preference */
	free(value);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_string_n(void)
{
	int ret;
	char *value = NULL;

	ret = preference_shared_get_string(NULL, &value);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_string", value, NULL, free(value));
	TC_ASSERT_EQ("preference_shared_get_string", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_string(SHARED_INTKEY_PATH, &value);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_string", value, NULL, free(value));
	TC_ASSERT_EQ("preference_shared_get_string", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_get_string(INVALID_KEY, &value);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_string", value, NULL, free(value));
	TC_ASSERT_EQ("preference_shared_get_string", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_binary_p(void)
{
        int ret;
        void *value = NULL;
	int len;

        BINARY_VALUE.val1 = 123;
        BINARY_VALUE.val2 = 'x';

        ret = preference_shared_get_binary(BINARY_KEY, &value, &len);
        TC_ASSERT_NEQ("preference_shared_get_binary", value, NULL);
        TC_ASSERT_EQ_CLEANUP("preference_shared_get_binary", ret, OK, free(value));
        TC_ASSERT_EQ_CLEANUP("preference_shared_get_binary", check_binary_value((struct bin_pref_val_s *)value, BINARY_VALUE), true, free(value));
	TC_ASSERT_EQ("preference_shared_get_binary", len, sizeof(BINARY_VALUE));

        /* Clean binary value allocated from preference */
        free(value);

        TC_SUCCESS_RESULT();
}

static void utc_preference_shared_get_binary_n(void)
{
	int ret;
	void *value = NULL;
	int len = -1;

	ret = preference_shared_get_binary(NULL, &value, &len);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_binary", value, NULL, free(value));
	TC_ASSERT_EQ("preference_shared_get_binary", ret, PREFERENCE_INVALID_PARAMETER);
	TC_ASSERT_EQ("preference_shared_get_binary", len, -1);

	ret = preference_shared_get_binary(SHARED_INTKEY_PATH, &value, &len);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_binary", value, NULL, free(value));
	TC_ASSERT_EQ("preference_shared_get_binary", ret, PREFERENCE_INVALID_PARAMETER);
	TC_ASSERT_EQ("preference_shared_get_binary", len, -1);

	ret = preference_shared_get_binary(INVALID_KEY, &value, &len);
	TC_ASSERT_EQ_CLEANUP("preference_shared_get_binary", value, NULL, free(value));
	TC_ASSERT_EQ("preference_shared_get_binary", ret, PREFERENCE_KEY_NOT_EXIST);
	TC_ASSERT_EQ("preference_shared_get_binary", len, -1);

	/* Clean binary value allocated from preference */
	free(value);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_changed_cb_p(void)
{
	int ret;
	cb_flag = false;

	/* Set callback for key changed */
	ret = preference_shared_set_changed_cb(SHARED_INTKEY_PATH, shared_value_changed_cb, CB_DATA);
	TC_ASSERT_EQ("preference_shared_set_changed_cb", ret, OK);

	/* Change the value of key */
	ret = preference_shared_set_int(SHARED_INTKEY_PATH, INT_VALUE + 1);
	TC_ASSERT_EQ("preference_shared_set_int", ret, OK);

	sleep(1);

	TC_ASSERT_EQ("preference_shared_set_changed_cb", cb_flag, true);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_set_changed_cb_n(void)
{
	int ret;

	ret = preference_shared_set_changed_cb(SHARED_INTKEY_PATH, NULL, NULL);
	TC_ASSERT_EQ("preference_shared_set_changed_cb", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_set_changed_cb(NULL, shared_value_changed_cb, NULL);
	TC_ASSERT_EQ("preference_shared_set_changed_cb", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_set_changed_cb(INVALID_KEY, shared_value_changed_cb, NULL);
	TC_ASSERT_EQ("preference_shared_set_changed_cb", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_unset_changed_cb_p(void)
{
	int ret;
	cb_flag = false;

	/* Unset callback for key changed */
	ret = preference_shared_unset_changed_cb(SHARED_INTKEY_PATH);
	TC_ASSERT_EQ("preference_shared_unset_changed_cb", ret, OK);

	/* Change the value of key */
	ret = preference_shared_set_int(SHARED_INTKEY_PATH, INT_VALUE + 1);
	TC_ASSERT_EQ("preference_unset_changed_cb", ret, OK);

	sleep(1);

	TC_ASSERT_EQ("preference_shared_unset_changed_cb", cb_flag, false);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_unset_changed_cb_n(void)
{
	int ret;

	ret = preference_shared_unset_changed_cb(NULL);
	TC_ASSERT_EQ("preference_shared_unset_changed_cb", ret, PREFERENCE_INVALID_PARAMETER);

	/* Try to unset with key which is valid but hasn't registered callback */
	ret = preference_shared_unset_changed_cb(SHARED_STRINGKEY_PATH);
	TC_ASSERT_EQ("preference_shared_unset_changed_cb", ret, PREFERENCE_NOT_REGISTERED);

	/* Try to unset with invalid key */
	ret = preference_shared_unset_changed_cb(INVALID_KEY);
	TC_ASSERT_EQ("preference_shared_unset_changed_cb", ret, PREFERENCE_KEY_NOT_EXIST);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_remove_p(void)
{
	int ret;
	int int_value;

	ret = preference_shared_remove(SHARED_INTKEY_PATH);
	TC_ASSERT_EQ("preference_shared_remove", ret, OK);

	/* Try to get value with already removed key, it should be failed */
	ret = preference_shared_get_int(SHARED_INTKEY_PATH, &int_value);
	TC_ASSERT_NEQ("preference_shared_get_int", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_remove_n(void)
{
	int ret;

	ret = preference_shared_remove(NULL);
	TC_ASSERT_EQ("preference_shared_remove", ret, PREFERENCE_INVALID_PARAMETER);

	ret = preference_shared_remove(INVALID_KEY);
	TC_ASSERT_EQ("preference_shared_remove", ret, PREFERENCE_KEY_NOT_EXIST);

	/* Try to remove again with already removed key, it should be failed */
	ret = preference_shared_remove(SHARED_INTKEY_PATH);
	TC_ASSERT_NEQ("preference_shared_remove", ret, OK);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_is_existing_p(void)
{
	int ret;
	bool is_existing = false;

	/* Check whether stored key is existing, it should be return true */
	ret = preference_shared_is_existing(SHARED_DOUBLEKEY_PATH, &is_existing);
	TC_ASSERT_EQ("preference_shared_is_existing", ret, OK);
	TC_ASSERT_EQ("preference_shared_is_existing", is_existing, true);

	/* Check whether removed key is existing, it should be return false through passed paramter */
	ret = preference_shared_is_existing(SHARED_INTKEY_PATH, &is_existing);
	TC_ASSERT_EQ("preference_shared_is_existing", ret, OK);
	TC_ASSERT_EQ("preference_shared_is_existing", is_existing, false);	

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_is_existing_n(void)
{
	int ret;
	bool is_existing;

	ret = preference_shared_is_existing(NULL, &is_existing);
	TC_ASSERT_EQ("preference_shared_is_existing", ret, PREFERENCE_INVALID_PARAMETER);

	is_existing = true;
	/* Check whether invalid key is existing, it should be return false through passed paramter */
	ret = preference_shared_is_existing(INVALID_KEY, &is_existing);
	TC_ASSERT_EQ("preference_shared_is_existing", ret, OK);
	TC_ASSERT_EQ("preference_shared_is_existing", is_existing, false);

	TC_SUCCESS_RESULT();
}

static void utc_preference_shared_remove_all_p(void)
{
	int ret;

	ret = preference_shared_remove_all(SHARED_KEY_PATH);
	TC_ASSERT_EQ("preference_shared_remove_all", ret, OK);

	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int utc_preference_main(int argc, char *argv[])
#endif
{
	if (testcase_state_handler(TC_START, "Preference UTC") == ERROR) {
		return ERROR;
	}

	/* Testcases for preference APIs */
	utc_preference_set_int_p();
	utc_preference_set_int_n();
	utc_preference_set_double_p();
	utc_preference_set_double_n();
	utc_preference_set_bool_p();
	utc_preference_set_bool_n();
	utc_preference_set_string_p();
	utc_preference_set_string_n();
	utc_preference_set_binary_p();
	utc_preference_set_binary_n();
	utc_preference_get_int_p();
	utc_preference_get_int_n();
	utc_preference_get_double_p();
	utc_preference_get_double_n();
	utc_preference_get_bool_p();
	utc_preference_get_bool_n();
	utc_preference_get_string_p();
	utc_preference_get_string_n();
	utc_preference_get_binary_p();
	utc_preference_get_binary_n();
	utc_preference_set_changed_cb_p();
	utc_preference_set_changed_cb_n();
	utc_preference_unset_changed_cb_p();
	utc_preference_unset_changed_cb_n();
	utc_preference_remove_p();
	utc_preference_remove_n();
	utc_preference_is_existing_p();
	utc_preference_is_existing_n();
	utc_preference_remove_all_p();

	/* Testcases for shared preference APIs */
	utc_preference_shared_set_int_p();
	utc_preference_shared_set_int_n();
	utc_preference_shared_set_double_p();
	utc_preference_shared_set_double_n();
	utc_preference_shared_set_bool_p();
	utc_preference_shared_set_bool_n();
	utc_preference_shared_set_string_p();
	utc_preference_shared_set_string_n();
	utc_preference_shared_set_binary_p();
	utc_preference_shared_set_binary_n();
	utc_preference_shared_get_int_p();
	utc_preference_shared_get_int_n();
	utc_preference_shared_get_double_p();
	utc_preference_shared_get_double_n();
	utc_preference_shared_get_bool_p();
	utc_preference_shared_get_bool_n();
	utc_preference_shared_get_string_p();
	utc_preference_shared_get_string_n();
	utc_preference_shared_get_binary_p();
	utc_preference_shared_get_binary_n();
	utc_preference_shared_set_changed_cb_p();
	utc_preference_shared_set_changed_cb_n();
	utc_preference_shared_unset_changed_cb_p();
	utc_preference_shared_unset_changed_cb_n();
	utc_preference_shared_remove_p();
	utc_preference_shared_remove_n();
	utc_preference_shared_is_existing_p();
	utc_preference_shared_is_existing_n();
	utc_preference_shared_remove_all_p();

	(void)testcase_state_handler(TC_END, "Preference UTC");

	return 0;
}
