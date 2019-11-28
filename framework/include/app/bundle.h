/*
 * bundle
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Jayoun Lee <airjany@samsung.com>, Sewook Park <sewook7.park@samsung.com>,
 * Jaeho Lee <jaeho81.lee@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __BUNDLE_H__
#define __BUNDLE_H__

/**
 * @file bundle.h
 * @version 0.1
 * @brief    This file declares API of bundle library
 */

/**
 * @addtogroup APPLICATION_FRAMEWORK
 * @{
 *
 * @defgroup bundle
 * @version    0.1
 *
 * @section    Header to use them:
 * @code
 * #include <bundle.h>
 * @endcode
 *
 * @addtogroup bundle
 * @{
 */

#include <errno.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define API __attribute__((visibility("default")))
#define likely(x) __builtin_expect(x, 1)
#define unlikely(x) __builtin_expect(x, 0)

/**
 * bundle is an opaque type pointing a bundle object
 */
typedef struct _bundle_t bundle;

/**
 * bundle_raw is an encoded data type
 * @see bundle_encode()
 * @see bundle_decode()
 */
typedef unsigned char bundle_raw;

/**
 * Each bundle keyval have a type.
 */
enum bundle_type_property {
	BUNDLE_TYPE_ARRAY = 0x0100,
	BUNDLE_TYPE_PRIMITIVE = 0x0200,
	BUNDLE_TYPE_MEASURABLE = 0x0400
};

enum bundle_type {
	BUNDLE_TYPE_NONE = -1,
	BUNDLE_TYPE_ANY = 0,
	BUNDLE_TYPE_STR = 1 | BUNDLE_TYPE_MEASURABLE, /* Default */
	BUNDLE_TYPE_STR_ARRAY = BUNDLE_TYPE_STR | BUNDLE_TYPE_ARRAY | BUNDLE_TYPE_MEASURABLE,
	BUNDLE_TYPE_BYTE = 2,
	BUNDLE_TYPE_BYTE_ARRAY = BUNDLE_TYPE_BYTE | BUNDLE_TYPE_ARRAY
};

/**
 * A keyval object in a bundle.
 * @see bundle_iterator_t
 */
typedef struct keyval_t bundle_keyval_t;

/**
 * bundle_iterator is a new iterator function type for bundle_foreach()
 * @see bundle_foreach()
 */
typedef void (*bundle_iterator_t)(
	const char *key,
	const int type,
	const bundle_keyval_t *kv,
	void *user_data);

/**
 * bundle_iterate_cb_t is an iterator function type for bundle_iterate()
 * @see bundle_iterate()
 * @remark This type is obsolete. Do not use this type any more.
 */
typedef void (*bundle_iterate_cb_t)(const char *key, const char *val, void *data);

/** 
 * @brief	Create a bundle object.
 * @pre		None
 * @post	None
 * @see		bundle_free()
 * @return	bundle object
 * @retval	NULL	on failure creating an object
 * @remark	When NULL is returned, errno is set to one of the following values; \n
 *			ENOMEM : No memory to create an object
 *
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_free(b); // free bundle
 @endcode
 */
API bundle *bundle_create(void);

/**
 * @brief		Free given bundle object with key/values in it
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_create()
 * @param[in]	b	bundle object to be freed
 * @return		Operation result;
 * @retval		0 success
 * @retval		-1 failure
 * @remark		None
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_free(b); // free bundle
 @endcode
 */
API int bundle_free(bundle *b);
/**
 * @brief		Add a string array type key-value pair into bundle. 
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_get_str_array()
 * @see			bundle_set_str_array_element()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	str_array string type value. If NULL, empty array is created. You can change an item with 
 * @param[in]	len Length of array.
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 char *sa = { "aaa", "bbb", "ccc" };	// A string array of length 3
 bundle *b = bundle_create();
 bundle_add_str_array(b, "foo", sa, 3); // add a key-val pair
 bundle_free(b);
 @endcode
 */
API int bundle_add_str_array(bundle *b, const char *key, const char **str_array, const int len);
/**
 * @brief		Add a string type key-value pair into bundle. 
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_str()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	val	value
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair

 bundle_free(b);
 @endcode
 */
API int bundle_add(bundle *b, const char *key, const char *val);

/**
 * @brief		Delete val with given key
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			None
 * @param[in]	b	bundle object
 * @param[in]	key	given key
 * @return		Operation result
 * @retval		0	Success
 * @retval		-1	Failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EINVAL : b is invalid (NULL or sth) \n
				ENOKEY : No key exist \n
				EKEYREJECTED : key is invalid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair
 bundle_del(b, "foo_key"); // del "foo_key" from b

 bundle_free(b);
 @endcode
 */
API int bundle_del(bundle *b, const char *key);
/**
 * @brief		Get string array value from key
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_str_array()
 * @see			bundle_set_str_array_element()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[out]	len	array length
 * @return		Pointer to array of  string
 * @retval		NULL	If key is not found, returns NULL.
 * @remark		DO NOT free or modify returned string!
				When NULL is returned, errno is set to one of the following values; \n
				EINVAL : b is invalid \n
				ENOKEY : No key exists \n
				EKEYREJECTED : invalid key (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create();
 bundle_add_str_array(b, "foo", NULL, 3); // add a key-val pair
 bundle_set_str_array_element(b, "foo", 0, "aaa");
 bundle_set_str_array_element(b, "foo", 1, "bbb");
 bundle_set_str_array_element(b, "foo", 2, "ccc");

 char **str_array = NULL;
 int len_str_array = 0;

 str_array=bundle_get_str_array(b, "foo", &len_str_array);
 // str_array = { "aaa", "bbb", "ccc" }, and len_str_array = 3

 bundle_free(b);
 @endcode
 */

API const char **bundle_get_str_array(bundle *b, const char *key, int *len);
/**
 * @brief		Get value from key
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_get_str()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @return		Pointer for value string
 * @retval		NULL	If key is not found, returns NULL.
 * @remark		DO NOT free or modify returned string!
				When NULL is returned, errno is set to one of the following values; \n
				EINVAL : b is invalid \n
				ENOKEY : No key exists \n
				EKEYREJECTED : invalid key (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair
 char *val = bundle_get_val(b, "foo_key");	// val = "bar_val"

 bundle_free(b);	// After freeing b, val becomes a dangling pointer.
 val = NULL;
 @endcode
 */
API const char *bundle_get_val(bundle *b, const char *key);

/**
 * @brief	Get the number of bundle items
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			None
 * @param[in]	b	bundle object
 * @return		Number of bundle items
 * @remark		None
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "key1", "val1"); // add a key-val pair
 int count = bundle_get_count(b);	// count=1
 bundle_add(b, "key2", "val2"); // add another key-val pair
 count = bundle_get_count(b); // count=2

 bundle_free(b);
 @endcode
 */
API int bundle_get_count(bundle *b);

/**
 * @brief	Get a type of a value with certain key
 * @pre		b must be a valid bundle object
 * @post	None
 * @see		bundle_type_t
 * @param[in]	b	A bundle
 * @param[in]	key	A key in bundle
 * @return	Type of a key in b
 * @remark
 @code
 @endcode
 */
API int bundle_get_type(bundle *b, const char *key);

/**
 * @brief	Duplicate given bundle object
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			None
 * @param[in]	b_from	bundle object to be duplicated
 * @return		New bundle object
 * @retval		NULL	Failure
 * @remark		None
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair
 bundle *b_dup = bundle_dup(b);	// duplicate b 

 bundle_free(b);
 bundle_free(b_dup);
 @endcode
 */
API bundle *bundle_dup(bundle *b_from);

/**
 * @brief	iterate callback function with each key/val pairs in bundle. (NOTE: Only BUNDLE_TYPE_STR type values come!)
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			None
 * @param[in]	b	bundle object
 * @param[in]	callback	iteration callback function
 * @param[in]	data	data for callback function
 * @remark		This function is obsolete, and does not give values whose types are not BUNDLE_TYPE_STR.
 @code
 @include <stdio.h>
 #include <bundle.h>
 void sample_cb(const char *k, const char *v, void *data) {
   printf("%s -> %s\n", k, v);
 }

 int main(void) {
	 bundle *b = bundle_create(); // Create new bundle object
	 bundle_add(b, "k1", "v1"); // add a key-val pair
	 bundle_add(b, "k2", "v2"); // add a key-val pair
	 bundle_add(b, "k3", "v3"); // add a key-val pair
	 bundle_iterate(b, sample_cb, NULL);	// iterate sample_cb for each key/val
	 return 0;
 } 
 @endcode
 */
API void bundle_iterate(bundle *b, bundle_iterate_cb_t callback, void *cb_data);

/**
 * @brief	iterate callback function with each key/val pairs in bundle. (Supports all types of value)
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_keyval_get_type bundle_keyval_type_is_array bundle_keyval_get_basic_val bundle_keyval_get_array_val
 * @param[in]	b	bundle object
 * @param[in]	iter	iteration callback function
 * @param[in]	user_data	data for callback function
 * @remark		This function supports all types.
 @code
 @include <stdio.h>
 #include <bundle.h>
 void sample_cb(const char *key, const int type, const bundle_keyval_t *kv, void *user_data) {
   void *basic_val = NULL;
   size_t basic_size = 0;
   void **array_val = NULL;
   int array_len = 0;
   size_t *array_elem_size = NULL;

   printf("Key:%s, Type:%d\n", key, type);
   if(bundle_keyval_type_is_array(kv)) {
     bundle_keyval_get_array_val(kv, &array_val, &array_len, &array_elem_size);
	 // Do something...
   }
   else {
     bundle_keyval_get_basic_val(kv, &basic_val, &size);
	 // Do something...
   }
 }
 
 int main(void) {
	 bundle *b = bundle_create(); // Create new bundle object
	 bundle_add_str(b, "k1", "v1"); // add a key-val pair
	 bundle_add_byte(b, "k2", "v2", 3); // add a key-val pair
	 char *s_arr[] = {"abc", "bcd", "cde"};
	 bundle_add_str_array(b, "k3", s_arr, 3); // add a key-val pair
	 bundle_iterate(b, sample_cb, NULL);	// iterate sample_cb for each key/val
	 return 0;
 } 
 @endcode
 */
API void bundle_foreach(bundle *b, bundle_iterator_t iter, void *user_data);

/**
 * @brief	Get type for a bundle_keyval_t object.
 * @pre		kv must be a valid bundle_keyval_t object.
 * @post	None
 * @see		bundle_foreach
 * @param[in]	kv	A bundle_keyval_t object
 * @return	Type of kv
 * @retval	-1	Operation failure.	errno is set.
 * @remark
 */
API int bundle_keyval_get_type(bundle_keyval_t *kv);

/**
 * @brief	Determine if kv is array type or not.
 * @pre		kv must be a valid bundle_keyval_t object.
 * @post	None
 * @see		bundle_foreach
 * @param[in]	kv	A bundle_keyval_t object
 * @return		Operation result
 * @retval		1	kv is an array.
 * @retval		0	kv is not an array.
 * @remark
 */
API int bundle_keyval_type_is_array(bundle_keyval_t *kv);

/**
 * @brief	Get value and size of the value from kv of basic type.
 * @pre		kv must be a valid bundle_keyval_t object.
 * @post	val, size are set.
 * @see		bundle_foreach
 * @param[in]	kv		A bundle_keyval_t object
 * @param[out]	val		Value
 * @param[out]	size	Size of val
 * @return	Operation result
 * @retval	0	Success
 * @remark	Do not free val.
 */
API int bundle_keyval_get_basic_val(bundle_keyval_t *kv, void **val, size_t *size);

/**
 * @brief	Get value array, length of array, and size of each array item
 * @pre		kv must be a valid bundle_keyval_t object.
 * @post	array_val, array_len, array_item_size are set.
 * @see		bundle_foreach
 * @param[in]	kv		A bundle_keyval_t object
 * @param[out]	array_val	Array pointer of values
 * @param[out]	array_len	Length of array_val
 * @param[out]	array_element_size	Array of size of each array element
 * @return	Operation result
 * @retval	0	Success
 * @retval	0	Failure
 * @remark
 */
API int bundle_keyval_get_array_val(bundle_keyval_t *kv, void ***array_val, unsigned int *array_len, size_t **array_element_size);

/**
 * @brief	Encode bundle to bundle_raw format (uses base64 format)
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			None
 * @param[in]	b	bundle object
 * @param[out]	r	returned bundle_raw data(byte data)
 *					r MUST BE FREED by free(r).
 * @param[out]	len	size of r (in bytes)
 * @return	size of raw data
 * @retval		0		Success
 * @retval		-1		Failure
 * @remark		None
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair
 bundle_raw *r;
 int len;
 bundle_encode(b, &r, &len);	// encode b

 bundle_free_encoded_rawdata(r);
 bundle_free(b);
 @endcode
 */
API int bundle_encode(bundle *b, bundle_raw **r, int *len);

/**
 * @brief	Free encoded rawdata from memory
 * @pre		r is a valid rawdata generated by bundle_encode().
 * @post	None
 * @see		bundle_encode
 * @param[in]	r	is a rawdata
 * @return		Operation result
 * @retval		0	Success
 * @retval		-1	Failure
 * @remark		None
 */
API int bundle_free_encoded_rawdata(bundle_raw **r);

/**
 * @brief	deserialize bundle_raw, and get bundle object
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			None
 * @param[in]	r	bundle_raw data to be converted to bundle object
 * @param[in]	len	size of r
 * @return	bundle object
 * @retval	NULL	Failure
 * @remark		None
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair

 bundle_raw *encoded_b;
 int len;
 bundle_encode(b, &encoded_b, &len);	// encode b

 bundle *b_dup;
 b_dup = bundle_decode(encoded_b, len);	// decoded bundle object

 bundle_free(b);
 free(encoded_b);
 bundle_free(b_dup);
 @endcode
 */
API bundle *bundle_decode(const bundle_raw *r, const int len);

/**
 * @brief	Export bundle to argv
 * @pre		b is a valid bundle object.
 * @post	argv is a pointer of newly allocated memory. It must be freed. 
 *          Each item of argv points the string in the bundle object b. If b is freed, argv will have garbage pointers. DO NOT FREE b BEFORE ACCESSING argv!!
 * @see		bundle_import_from_argv
 * @param[in]	b	bundle object
 * @param[out]	argv	Pointer of string array.
 *                      This array has NULL values for first and last item.
 *                      First NULL is for argv[0], and last NULL is a terminator for execv().
 * @return	Number of item in argv. This value is equal to actual count of argv - 1. (Last NULL terminator is not counted.)
 * @retval	-1		Function failure. Check errno to get the reason.
 * @remark	None
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add(b, "foo_key", "bar_val"); // add a key-val pair

 int argc = 0;
 char **argv = NULL;
 argc = bundle_export_to_argv(b, &argv);	// export to argv
 if(0 > argc) error("export failure");
 
 int i;
 for(i=0; i < argc; i++) {
   printf("%s\n", argv[i]);		// print argv 
 }
 bundle_free_exported_argv(argc, argv);	// argv must be freed after being used.

 bundle_free(b);
 @endcode
 */
API int bundle_export_to_argv(bundle *b, char ***argv);

/**
 * @brief	Free exported argv
 * @pre		argv is a valid string array generated from bundle_export_to_argv().
 * @post	None
 * @see		bundle_export_to_argv
 * @param[in]	argc	number of args, which is the return value of bundle_export_to_argv().
 * @param[in]	argv array from bundle_export_to_argv().
 * @return	Operation result.
 * @retval	0	on success
 * @retval	-1	on failure
 * @remark	You must not use this API when you use global argv.
 @code
 bundle *b = bundle_create();
 bundle_add_str(b, "foo", "bar");
 
 int argc = 0;
 char **argv = NULL;
 argc = bundle_export_to_argv(b, &argv);
 if(0 > argc) error("export failure");

 // Use argv...

 bundle_free_export_argv(argc, argv);
 argv = NULL;

 bundle_free(b);
 @endcode
 */
API int bundle_free_exported_argv(int argc, char ***argv);

/**
 * @brief	import a bundle from argv
 * @pre		argv is a valid string array, which is created by bundle_export_to_argv().
 * @post	Returned bundle b must be freed.
 * @see		bundle_export_to_argv
 * @param[in]	argc	argument count
 * @param[in]	argv	argument vector
 * @return	New bundle object
 * @retval	NULL	Function failure
 * @remark	None
 @code
 #include <bundle.h>

 int main(int argc, char **argv) {
   bundle *b = bundle_import_from_argv(argc, argv); // import from argc+argv
   char *val = bundle_get_val(b, "foo_key");	// value for "foo_key"
   // ......
   bundle_free(b);	// After freeing b, val becomes a dangling pointer.
   val = NULL;
 }
 @endcode
 */
API bundle *bundle_import_from_argv(int argc, char **argv);

#if 0
/**
 * @brief		Add a string type key-value pair into bundle. 
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_get_str()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	str string type value
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add_str(b, "foo", "bar"); // add a key-val pair

 bundle_free(b);
 @endcode
 */
API int bundle_add_str(bundle *b, const char *key, const char *str);

/**
 * @brief		Set a value of string array element
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_str_array()
 * @see			bundle_get_str_array()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	idx index of array element to be changed
 * @param[in]	val string type value. If NULL, empty array is created. You can change an item with 
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create();
 bundle_add_str_array(b, "foo", NULL, 3); // add a key-val pair
 bundle_set_str_array_element(b, "foo", 0, "aaa");
 bundle_set_str_array_element(b, "foo", 1, "bbb");
 bundle_set_str_array_element(b, "foo", 2, "ccc");

 char **str_array = NULL;
 int len_str_array = 0;

 str_array=bundle_get_str_array(b, "foo", &len_str_array);
 // str_array = { "aaa", "bbb", "ccc" }, and len_str_array = 3

 bundle_free(b);
 @endcode
 */
API int bundle_set_str_array_element(bundle *b, const char *key, const unsigned int idx, const char *val);

/**
 * @brief		Add a byte type key-value pair into bundle. 
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_get_byte()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	byte string type value
 * @param[in]	size size of byte
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add_byte(b, "foo", "bar\0", 4); // add a key-val pair

 bundle_free(b);
 @endcode
 */

API int bundle_add_byte(bundle *b, const char *key, const void *byte, const size_t size);

/**
 * @brief		Add a byte array type key-value pair into bundle. 
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_get_str_array()
 * @see			bundle_set_byte_array_element()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	byte_array  Not used.
 * @param[in]	len Length of array to be created
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create();
 bundle_add_byte_array(b, "foo", NULL, 3); // add a byte-array with length 3

 bundle_set_byte_array_element(b, "foo", 0, "aaa\0", 4);	array[0] = "aaa\0"
 bundle_set_byte_array_element(b, "foo", 1, "bbb\0", 4);	array[1] = "bbb\0"
 bundle_set_byte_array_element(b, "foo", 2, "ccc\0", 4);	array[2] = "ccc\0"

 bundle_free(b);
 @endcode
 */
API int bundle_add_byte_array(bundle *b, const char *key, void **byte_array, const unsigned int len);

/**
 * @brief		Set a value of byte array element
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_str_array()
 * @see			bundle_get_str_array()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[in]	idx index of array element to be changed
 * @param[in]	val string type value. If NULL, empty array is created. You can change an item with 
 * @param[in]	size Size of value in byte
 * @return		Operation result
 * @retval		0	success
 * @retval		-1	failure
 *
 * @remark		When -1 is returned, errno is set to one of the following values; \n
				EKEYREJECTED : key is rejected (NULL or sth) \n
				EPERM : key is already exist, not permitted to overwrite value \n
				EINVAL : b or val is not valid (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create();
 bundle_add_byte_array(b, "foo", NULL, 3); // add a key-val pair
 bundle_set_byte_array_element(b, "foo", 0, "aaa\0", 4);
 bundle_set_byte_array_element(b, "foo", 1, "bbb\0", 4);
 bundle_set_byte_array_element(b, "foo", 2, "ccc\0", 4);

 unsigned char **byte_array = NULL;
 int len_byte_array = 0;

 byte_array=bundle_get_str_array(b, "foo", &len_byte_array);
 // byte_array = { "aaa\0", "bbb\0", "ccc\0" }, and len_byte_array = 3

 bundle_free(b);
 @endcode
 */
API int bundle_set_byte_array_element(bundle *b, const char *key, const unsigned int idx, const void *val, const size_t size);

/**
 * @brief		Get string value from key
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_str()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[out]	str returned value
 * @return		Operation result
 * @retval		0 on success
 * @retval		-1 on failure
 * @remark		Do not free str!
				When -1 is returned, errno is set to one of the following values; \n
				EINVAL : b is invalid \n
				ENOKEY : No key exists \n
				EKEYREJECTED : invalid key (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add_str(b, "foo_key", "bar_val"); // add a key-val pair

 char *v = NULL;
 bundle_get_str(b, "foo_key", &v);	// v = "bar_val"

 bundle_free(b);	// After freeing b, v becomes a dangling pointer.
 v = NULL;
 @endcode
 */
API int bundle_get_str(bundle *b, const char *key, char **str);

/**
 * @brief		Get byte value from key
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_byte()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[out]	byte returned value
 * @param[out]	size Size of byte
 * @return		Operation result
 * @retval		0 on success
 * @retval		-1 on failure
 * @remark		Do not free str!
				When -1 is returned, errno is set to one of the following values; \n
				EINVAL : b is invalid \n
				ENOKEY : No key exists \n
				EKEYREJECTED : invalid key (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create(); // Create new bundle object
 bundle_add_byte(b, "foo", "bar\0", 4); // add a key-val pair

 unsigned char *v = NULL;
 bundle_get_str(b, "foo", &v);	// v = "bar\0"

 bundle_free(b);	// After freeing b, v becomes a dangling pointer.
 @endcode
 */
API int bundle_get_byte(bundle *b, const char *key, void **byte, size_t *size);

/**
 * @brief		Get byte array value from key
 * @pre			b must be a valid bundle object.
 * @post		None
 * @see			bundle_add_str_array()
 * @see			bundle_set_str_array_element()
 * @param[in]	b	bundle object
 * @param[in]	key	key
 * @param[out]	byte_array returned value
 * @param[out]	len	array length
 * @param[out]	array_element_size	an array of sizes of each byte_array element
 * @return		Operation result
 * @retval		0 on success
 * @retval		-1 on failure
 * @remark		Do not free str!
				When -1 is returned, errno is set to one of the following values; \n
				EINVAL : b is invalid \n
				ENOKEY : No key exists \n
				EKEYREJECTED : invalid key (NULL or sth) \n
 @code
 #include <bundle.h>
 bundle *b = bundle_create();
 bundle_add_byte_array(b, "foo", NULL, 3);
 bundle_set_byte_array_element(b, "foo", 0, "aaa\0", 4);
 bundle_set_byte_array_element(b, "foo", 1, "bbb\0", 4);
 bundle_set_byte_array_element(b, "foo", 2, "ccc\0", 4);

 char **byte_array = NULL;
 int len_byte_array = 0;
 size_t *size_byte_array = NULL;

 byte_array = bundle_get_str_array(b, "foo", &len_byte_array, &size_byte_array);
 // byte_array = { "aaa\0", "bbb\0", "ccc\0" }, len_byte_array = 3, and size_byte_array = { 4, 4, 4 }

 bundle_free(b);
 @endcode
 */
API int bundle_get_byte_array(bundle *b, const char *key, void ***byte_array, unsigned int *len, unsigned int **array_element_size);
#endif

#ifdef __cplusplus
}
#endif

/**
 * @}
 * @}
 */

#endif /* __BUNDLE_H__ */
