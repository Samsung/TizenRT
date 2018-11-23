/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/// @file tc_libc_string.c
/// @brief Test Case Example for Libc String API

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <tinyara/float.h>
#include <tinyara/math.h>

#include "tc_internal.h"

#define BUFF_SIZE 5
#define BUFF_SIZE_10 10
#define BUFF_SIZE_12 12

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
* @fn                   :tc_libc_string_memcpy
* @brief                :Copies the values of num bytes from the location pointed\
*                        to by source directly to the memory block pointed to by destination.
* @Scenario             :Copy the values of num bytes from the location pointed\
*                        to by source directly to the memory block pointed to by destination.
* API's covered         :memcpy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_memcpy(void)
{
	char sz_src[BUFF_SIZE] = "test";
	char sz_dest[BUFF_SIZE] = "aaaa";
	char *res_ptr = NULL;

	res_ptr = (char *)memcpy(sz_dest, sz_src, BUFF_SIZE);
	TC_ASSERT_NEQ("memcpy", res_ptr, NULL);
	TC_ASSERT_EQ("memcpy", strncmp(sz_dest, res_ptr, BUFF_SIZE), 0);
	TC_ASSERT_EQ("memcpy", strncmp(sz_dest, sz_src, BUFF_SIZE), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_memset
* @brief                :Sets the first num bytes of the block of memory pointed by ptr to the specified value.
* @Scenario             :Set the first BUFF_SIZE bytes of the block of memory pointed by buffer to the specified value 'a'.
* API's covered         :memset
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_memset(void)
{
	char buffer[BUFF_SIZE] = "test";
	char ctarget[BUFF_SIZE] = "aaaa";
	char *res_ptr = NULL;

	res_ptr = (char *)memset(buffer, 'a', BUFF_SIZE - 1);
	TC_ASSERT_NEQ("memset", res_ptr, NULL);
	TC_ASSERT_EQ("memset", strncmp(res_ptr, ctarget, BUFF_SIZE), 0);
	TC_ASSERT_EQ("memset", strncmp(ctarget, buffer, BUFF_SIZE), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_memchr
* @brief                :Searches within the first num bytes of the block of memory pointed by ptr for the first occurrence of value and returns a pointer to it.
* @Scenario             :Search within the first BUFF_SIZE bytes of the block of memory pointed by buffer\
*                        for the first occurrence of 's' and returns a pointer to it.
* API's covered         :memchr
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_memchr(void)
{
	char buffer[BUFF_SIZE] = "test";
	char *res_ptr = NULL;

	res_ptr = (char *)memchr(buffer, 's', BUFF_SIZE);
	TC_ASSERT_NEQ("memchr", res_ptr, NULL);
	TC_ASSERT_EQ("memchr", *res_ptr, 's');

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_memcmp
* @brief                :Compares the first num bytes of the block of memory pointed \
*                        to the first num bytes pointed, returning zero if they all match.
* @Scenario             :Compare the first BUFF_SIZE bytes of the block of memory pointed by buffer1\
*                        to the first BUFF_SIZE bytes pointed , returning zero if they all match.
* API's covered         :memcmp
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_memcmp(void)
{
	int ret_chk = 0;
	char buffer1[BUFF_SIZE] = "test";
	char buffer2[BUFF_SIZE] = "test";
	char buffer3[BUFF_SIZE] = "tesz";
	char buffer4[BUFF_SIZE] = "tesa";

	ret_chk = memcmp(buffer1, buffer2, BUFF_SIZE);
	TC_ASSERT_EQ("memcmp", ret_chk, 0);

	ret_chk = memcmp(buffer1, buffer3, BUFF_SIZE);
	TC_ASSERT_EQ("memcmp", ret_chk, -1);

	ret_chk = memcmp(buffer1, buffer4, BUFF_SIZE);
	TC_ASSERT_EQ("memcmp", ret_chk, 1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_memmove
* @brief                :Copies the values from the location pointed by source to the memory block pointed by destination.
* @Scenario             :Copy the values of BUFF_SIZE bytes from the location pointed by buffer2 to the memory block pointed by buffer1.
* API's covered         :memmove
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_memmove(void)
{
	char buffer1[BUFF_SIZE] = "test";
	char buffer2[BUFF_SIZE] = "abcd";
	char *res_ptr = NULL;

	res_ptr = (char *)memmove(buffer1, buffer2, sizeof(buffer1));
	TC_ASSERT_NEQ("memmove", res_ptr, NULL);
	TC_ASSERT_EQ("memmove", strncmp(res_ptr, buffer2, BUFF_SIZE), 0);
	TC_ASSERT_EQ("memmove", strncmp(buffer1, buffer2, BUFF_SIZE), 0);

	res_ptr = (char *)memmove(buffer2, buffer1, sizeof(buffer1));
	TC_ASSERT_NEQ("memmove", res_ptr, NULL);
	TC_ASSERT_EQ("memmove", strncmp(res_ptr, buffer1, BUFF_SIZE), 0);
	TC_ASSERT_EQ("memmove", strncmp(buffer2, buffer1, BUFF_SIZE), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_stpcpy
* @brief                :Copies a string and return a pointer to the end of the result.
* @Scenario             :Copy a string and return a pointer to the end of the result.
* API's covered         :stpcpy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_stpcpy(void)
{
	char dest_arr[BUFF_SIZE] = "test";
	char src_buf[BUFF_SIZE] = "abcd";
	char src[BUFF_SIZE] = "abcd";
	char *res_ptr = NULL;

	res_ptr = stpcpy(dest_arr, src_buf);
	TC_ASSERT_NEQ("stpcpy", res_ptr, NULL);
	TC_ASSERT_EQ("stpcpy", *(res_ptr - 1), 'd');
	TC_ASSERT_EQ("stpcpy", strncmp(dest_arr, src, BUFF_SIZE), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strcasecmp
* @brief                :function compares two strings without sensitivity to case.
* @Scenario             :Compare string1 and string2 without sensitivity to case.
* API's covered         :strcasecmp
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strcasecmp(void)
{
	int ret_chk = 0;
	char buffer1[BUFF_SIZE] = "test";
	char buffer2[BUFF_SIZE] = "tESt";
	char buffer3[BUFF_SIZE] = "tesz";
	char buffer4[BUFF_SIZE] = "tesa";

	ret_chk = strcasecmp(buffer1, buffer2);
	TC_ASSERT_EQ("strcasecmp", ret_chk, 0);

	ret_chk = strcasecmp(buffer1, buffer3);
	TC_ASSERT_LT("strcasecmp", ret_chk, 0);

	ret_chk = strcasecmp(buffer1, buffer4);
	TC_ASSERT_GT("strcasecmp", ret_chk, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strcat
* @brief                :Appends a copy of the source string to the destination string.
* @Scenario             :Append a copy of the src string to the dest_arr string.
* API's covered         :strcat
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strcat(void)
{
	char dest_arr[BUFF_SIZE_10] = "test";
	char final_arr[BUFF_SIZE_10] = "testabcd";
	char src[BUFF_SIZE] = "abcd";
	char *res_ptr = NULL;

	res_ptr = strcat(dest_arr, src);
	TC_ASSERT_NEQ("strcat", res_ptr, NULL);
	TC_ASSERT_EQ("strcat", strncmp(res_ptr, final_arr, BUFF_SIZE_10), 0);
	TC_ASSERT_EQ("strcat", strncmp(dest_arr, final_arr, BUFF_SIZE_10), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strchr
* @brief                :Returns a pointer to the first occurrence of character in the C string str.
* @Scenario             :Character to be located('s') along with string and return a pointer to the first occurrence of character 's'.
* API's covered         :strchr
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strchr(void)
{
	char dest_arr[BUFF_SIZE] = "test";
	char *res_ptr = NULL;

	res_ptr = strchr(dest_arr, 'z');
	TC_ASSERT_EQ("strchr", res_ptr, NULL);

	res_ptr = strchr(dest_arr, 's');
	TC_ASSERT_NEQ("strchr", res_ptr, NULL);
	TC_ASSERT_EQ("strchr", *res_ptr, 's');

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strcmp
* @brief                :This function starts comparing the first character of each string. If they are equal to each other,\
*                        it continues with the following pairs until the characters differ or until a terminating null-character is reached.
* @Scenario             :String to be compared, compare the first character of each string, returning zero if they all match.
* API's covered         :strcmp
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strcmp(void)
{
	int ret_chk = 0;
	char buffer1[BUFF_SIZE] = "test";
	char buffer2[BUFF_SIZE] = "test";
	char buffer3[BUFF_SIZE] = "tesz";
	char buffer4[BUFF_SIZE] = "tesa";

	ret_chk = strcmp(buffer1, buffer2);
	TC_ASSERT_EQ("strcmp", ret_chk, 0);

	ret_chk = strcmp(buffer1, buffer3);
	TC_ASSERT_LT("strcmp", ret_chk, 0);

	ret_chk = strcmp(buffer1, buffer4);
	TC_ASSERT_GT("strcmp", ret_chk, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strcpy
* @brief                :Copies the C string pointed by source into the array pointed by destination, including the terminating null character.
* @Scenario             :Copy the C string pointed by src_buf into the array pointed by dest_arr, including the terminating null character.
* API's covered         :strcpy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strcpy(void)
{
	char dest_arr[BUFF_SIZE] = "test";
	char src[BUFF_SIZE] = "abcd";
	char *res_ptr = NULL;

	res_ptr = strcpy(dest_arr, src);
	TC_ASSERT_NEQ("strcpy", res_ptr, NULL);
	TC_ASSERT_EQ("strcpy", strncmp(res_ptr, src, BUFF_SIZE), 0);
	TC_ASSERT_EQ("strcpy", strncmp(dest_arr, src, BUFF_SIZE), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strcspn
* @brief                :Scans str1 for the first occurrence of any of the characters that are part of str2
* @Scenario             :Scans src for the first occurrence of any of the characters that are part of dest_arr,\
*                        returning the number of characters of src read before this first occurrence.
* API's covered         :strcspn
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strcspn(void)
{
	char dest_arr[BUFF_SIZE] = "tesa";
	char src1[BUFF_SIZE] = "abcd";
	char src2[BUFF_SIZE] = "bcd";
	int ret_chk = 0, pos_a = 3, lenDest = 4;
	/* "tesa" has "a" which is the only character that is in "abcd", strcspn will return characters read
	   in "tesa" before reaching "a", so return value will be 3 in this case */

	ret_chk = strcspn(dest_arr, src1);
	TC_ASSERT_EQ("strcspn", ret_chk, pos_a);

	/* src2 has no charater that matches with any character of dest_arr */

	ret_chk = strcspn(dest_arr, src2);
	TC_ASSERT_EQ("strcspn", ret_chk, lenDest);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strdup
* @brief                :function shall return a pointer to a new string, which is a duplicate of the string
* @Scenario             :It is available for C only when the program defines the __cplusplus__strings__ macro.\
*                        strdup reserves storage space for a copy of string by calling malloc. The string argument\
*                        to this function is expected to contain a null character (\0) marking the end of the string.
* API's covered         :strdup
* Preconditions         :none
* Postconditions        :free(dest_arr)
* @return               :void
*/
static void tc_libc_string_strdup(void)
{
	char *dest_arr = NULL;
	char src[BUFF_SIZE] = "test";

	dest_arr = strdup(src);
	TC_ASSERT_NEQ("strdup", dest_arr, NULL);
	TC_ASSERT_EQ_CLEANUP("strdup",
						 strncmp(dest_arr, src, BUFF_SIZE), OK,
						 TC_FREE_MEMORY(dest_arr));

	TC_FREE_MEMORY(dest_arr);
	TC_SUCCESS_RESULT();
}

#ifdef CONFIG_LIBC_STRERROR
/**
* @fn                   :tc_libc_string_strerror
* @brief                :Interprets the value of errnum, generating a string with a message that describes the error.
* @Scenario             :function returns a pointer to a string that describes\
*                        the error code passed in the argument errnum.
* API's covered         :strerror
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strerror(void)
{
	char *dest_arr = NULL;
	char src[BUFF_SIZE_12] = "Bad address";

	/* EFAULT is defined as 14 which gives Bad address in strerror */
	dest_arr = (char *)strerror(EFAULT);
	TC_ASSERT_NEQ("strerror", dest_arr, NULL);
	TC_ASSERT_EQ("strerror", strncmp(dest_arr, src, BUFF_SIZE_12), 0);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_libc_string_strlen
* @brief                :Returns the length of the C string str.
* @Scenario             :function determines the length of string src excluding the ending null character.
* API's covered         :strlen
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strlen(void)
{
	char src[BUFF_SIZE] = "test";
	int ret_chk = ERROR;

	ret_chk = strlen(src);
	TC_ASSERT_EQ("strlen", ret_chk, BUFF_SIZE - 1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strncasecmp
* @brief                :The strcasecmp() function compares the two strings s1 and s2, ignoring the case of the characters, it only compares the first n bytes of s1.
* @Scenario             :function determines the length of string src excluding the ending null character.
* API's covered         :strncasecmp
* Preconditions         :none
* Postconditions        :none
* @return               :pass on success.
*/
static void tc_libc_string_strncasecmp(void)
{
	int ret_chk = ERROR;
	char buffer1[BUFF_SIZE] = "test";
	char buffer2[BUFF_SIZE] = "tESt";
	char buffer3[BUFF_SIZE] = "tesz";
	char buffer4[BUFF_SIZE] = "tesa";

	ret_chk = strncasecmp(buffer1, buffer2, BUFF_SIZE);
	TC_ASSERT_EQ("strncasecmp", ret_chk, 0);

	ret_chk = strncasecmp(buffer1, buffer3, BUFF_SIZE);
	TC_ASSERT_LT("strncasecmp", ret_chk, 0);

	ret_chk = strncasecmp(buffer1, buffer4, BUFF_SIZE);
	TC_ASSERT_GT("strncasecmp", ret_chk, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strncat
* @brief                :Appends the first num characters of source to destination.
* @Scenario             :The function appends the first count characters of src to dest_arr.\
*                        If count is greater than the length of src, the length of src is used in place of count.
* API's covered         :strncat
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strncat(void)
{
	char dest_arr[BUFF_SIZE_10] = "test";
	char final_arr[BUFF_SIZE_10] = "testabcd";
	char src[BUFF_SIZE] = "abcd";

	char *res_ptr = NULL;

	res_ptr = strncat(dest_arr, src, BUFF_SIZE);
	TC_ASSERT_NEQ("strncat", res_ptr, NULL);
	TC_ASSERT_EQ("strncat", strncmp(res_ptr, final_arr, BUFF_SIZE_10), 0);
	TC_ASSERT_EQ("strncat", strncmp(dest_arr, final_arr, BUFF_SIZE_10), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strncmp
* @brief                :The strncmp() function compares string1 and string2 to the maximum of count.
* @Scenario             :String to be compared, compare the first character of each string untill given count, returning zero if they all match.
* API's covered         :strncmp
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strncmp(void)
{
	int ret_chk = 0;
	char buffer1[BUFF_SIZE] = "test";
	char buffer2[BUFF_SIZE] = "test";
	char buffer3[BUFF_SIZE] = "tesz";
	char buffer4[BUFF_SIZE] = "tesa";

	ret_chk = strncmp(buffer1, buffer2, BUFF_SIZE);
	TC_ASSERT_EQ("strncmp", ret_chk, 0);

	ret_chk = strncmp(buffer1, buffer3, BUFF_SIZE);
	TC_ASSERT_LT("strncmp", ret_chk, 0);

	ret_chk = strncmp(buffer1, buffer4, BUFF_SIZE);
	TC_ASSERT_GT("strncmp", ret_chk, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strncpy
* @brief                :Copies the first num characters of source to destination.
* @Scenario             :function copies at most BUFF_SIZE characters of src to dest_arr.
* API's covered         :strncpy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strncpy(void)
{
	char dest_arr[BUFF_SIZE] = "test";
	char src[BUFF_SIZE] = "abcd";
	char *res_ptr = NULL;

	res_ptr = strncpy(dest_arr, src, BUFF_SIZE);
	TC_ASSERT_NEQ("strncpy", res_ptr, NULL);
	TC_ASSERT_EQ("strncpy", strncmp(res_ptr, src, BUFF_SIZE), 0);
	TC_ASSERT_EQ("strncpy", strncmp(dest_arr, src, BUFF_SIZE), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strndup
* @brief                :function shall return a pointer to a new string, which is a duplicate of the string
* @Scenario             :It is available for C only when the program defines the __cplusplus__strings__ macro.\
*                        strdup reserves storage space for a copy of string by calling malloc. The string argument\
*                        to this function is expected to contain a null character (\0) marking the end of the string.
* API's covered         :strndup
* Preconditions         :none
* Postconditions        :free(dest_arr)
* @return               :void
*/
static void tc_libc_string_strndup(void)
{
	char *dest_arr = NULL;
	char src[BUFF_SIZE] = "test";

	dest_arr = strndup(src, BUFF_SIZE);
	TC_ASSERT_NEQ("strndup", dest_arr, NULL);
	TC_ASSERT_EQ_CLEANUP("strndup",
						 strncmp(dest_arr, src, BUFF_SIZE), 0,
						 TC_FREE_MEMORY(dest_arr));

	TC_FREE_MEMORY(dest_arr);
	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strnlen
* @brief                :Length of string to calculate with a maximum length to check.
* @Scenario             :strnlen() function computes the length of the string pointed to by src,\
*                        not including the terminating null character, up to a maximum of BUFF_SIZE\
*                        bytes. The function doesn't check any more than the first BUFF_SIZE bytes.
* API's covered         :strnlen
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strnlen(void)
{
	char src1[BUFF_SIZE] = "test";
	char src2[BUFF_SIZE_10] = "testabcd";
	int ret_chk = 0;

	ret_chk = strnlen(src1, BUFF_SIZE);
	TC_ASSERT_EQ("strnlen", ret_chk, BUFF_SIZE - 1);

	ret_chk = strnlen(src2, BUFF_SIZE);
	TC_ASSERT_EQ("strnlen", ret_chk, BUFF_SIZE);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strpbrk
* @brief                :Finds the first character in the string str1 that matches any character specified in str2.\
*                        This does not include the terminating null-characters.
* @Scenario             :A pointer to the first occurrence in dest_arr of any of the characters that are part of cbuf,\
*                        If none of the characters of cbuf is present in dest_arr, a null pointer is returned.
* API's covered         :strpbrk
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strpbrk(void)
{
	char dest_arr[BUFF_SIZE_10] = "Example";
	char *cbuf = "me";
	char *cStr = "win";
	char *res_ptr = NULL;
	char *res_ptrstr = "mple";

	res_ptr = strpbrk(dest_arr, cbuf);
	TC_ASSERT_NEQ("strpbrk", res_ptr, NULL);
	TC_ASSERT_EQ("strpbrk", strncmp(res_ptr, res_ptrstr, BUFF_SIZE_10), 0);
	TC_ASSERT_EQ("strpbrk", *res_ptr, 'm');

	res_ptr = strpbrk(dest_arr, cStr);
	TC_ASSERT_EQ("strpbrk", res_ptr, NULL);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strrchr
* @brief                :Returns a pointer to the last occurrence of character in the C string.
* @Scenario             :The strrchr function searches string for the last occurrence of 'e'.\
*                        The null character terminating string is included in the search.
* API's covered         :strrchr
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strrchr(void)
{
	char dest_arr[BUFF_SIZE_10] = "examples";
	char *res_ptr = NULL;
	char *res_ptrstr = "es";

	res_ptr = strrchr(dest_arr, 'e');
	TC_ASSERT_NEQ("strrchr", res_ptr, NULL);
	TC_ASSERT_EQ("strrchr", strncmp(res_ptr, res_ptrstr, BUFF_SIZE), 0);
	TC_ASSERT_EQ("strrchr", *res_ptr, 'e');

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strspn
* @brief                :Returns the length of the initial portion of dest_arr which consists only of characters that are part of cbuf.\
*                        The search does not include the terminating null-characters of either strings, but ends there.
* @Scenario             :The length of the initial portion of str1 containing only characters that appear in str2.\
*                        Therefore, if all of the characters in str1 are in str2, the function returns the length\
*                        of the entire str1 string, and if the first character in str1 is not in str2, the function returns zero.
* API's covered         :strspn
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strspn(void)
{
	char dest_arr1[BUFF_SIZE_10] = "example";
	char dest_arr2[BUFF_SIZE_10] = "exaple";
	char dest_arr3[BUFF_SIZE_10] = "rommm";
	int ret_chk = 0;
	char *cbuf = "me";

	ret_chk = strspn(cbuf, dest_arr1);
	TC_ASSERT_EQ("strspn", ret_chk, 2);

	ret_chk = strspn(cbuf, dest_arr2);
	TC_ASSERT_EQ("strspn", ret_chk, 0);

	ret_chk = strspn(cbuf, dest_arr3);
	TC_ASSERT_EQ("strspn", ret_chk, 1);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strstr
* @brief                :Returns a pointer to the first occurrence of str2 in str1,\
*                        or a null pointer if str2 is not part of str1
* @Scenario             :A pointer to the first occurrence in str1 of the entire sequence of characters specified in str2
* API's covered         :strstr
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strstr(void)
{
	char dest_arr[BUFF_SIZE_10] = "str test";
	char *res_ptr = NULL;
	char *psz = "str";

	res_ptr = strstr(dest_arr, psz);
	TC_ASSERT_NEQ("strstr", res_ptr, NULL);
	TC_ASSERT_EQ("strstr", strncmp(res_ptr, dest_arr, BUFF_SIZE_10), 0);

	psz = "test";
	res_ptr = strstr(dest_arr, psz);
	TC_ASSERT_NEQ("strstr", res_ptr, NULL);
	TC_ASSERT_EQ("strstr", strncmp(res_ptr, dest_arr + 4, BUFF_SIZE_10), 0);

	psz = "notfound";
	res_ptr = strstr(dest_arr, psz);
	TC_ASSERT_EQ("strstr", res_ptr, NULL);

	psz = "";
	res_ptr = strstr(dest_arr, psz);
	TC_ASSERT_NEQ("strstr", res_ptr, NULL);
	TC_ASSERT_EQ("strstr", strncmp(res_ptr, dest_arr, BUFF_SIZE_10), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strtok
* @brief                :A sequence of calls to this function split str into tokens, which are sequences \
*                        of contiguous characters separated by any of the characters that are part of delimiters.
* @Scenario             :If a token is found, a pointer to the beginning of the token. Otherwise, a null pointer.\
*                        A null pointer is always returned when the end of the string (i.e., a null character)\
*                        is reached in the string being scanned.
* API's covered         :strtok
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strtok(void)
{
	char *dest_arr[BUFF_SIZE - 1] = { "This", "is", "test", "string" };
	char szbuffer[] = "This is test string";
	int arr_idx = 0;
	char *res_ptr = NULL;
	res_ptr = strtok(szbuffer, " ");
	TC_ASSERT_NEQ("strtok", res_ptr, NULL);

	while (res_ptr != NULL) {
		TC_ASSERT_EQ("strtok", strncmp(res_ptr, dest_arr[arr_idx], BUFF_SIZE_10), 0);
		arr_idx++;
		res_ptr = strtok(NULL, " ");
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strtok_r
* @brief                :A sequence of calls to this function split str into tokens, which are sequences \
*                        of contiguous characters separated by any of the characters that are part of delimiters.
* @Scenario             :If a token is found, a pointer to the beginning of the token. Otherwise, a null pointer.\
*                        A null pointer is always returned when the end of the string (i.e., a null character)\
*                        is reached in the string being scanned.
* API's covered         :strtok_r
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strtok_r(void)
{
	int arr_idx = 0;
	char cbuf[] = "This is test string";
	char *dest_arr[BUFF_SIZE - 1] = { "This", "is", "test", "string" };
	char *psz_save_dest[BUFF_SIZE - 1] = { "is test string", "test string", "string", "" };
	char *res_ptr = NULL;
	char *psz_save;

	res_ptr = strtok_r(cbuf, " ", &psz_save);
	TC_ASSERT_NEQ("strtok_r", res_ptr, NULL);
	TC_ASSERT_NEQ("strtok_r", psz_save, NULL);

	while (res_ptr != NULL) {
		TC_ASSERT_EQ("strtok_r", strncmp(res_ptr, dest_arr[arr_idx], BUFF_SIZE_10), 0);
		TC_ASSERT_EQ("strtok_r", strncmp(psz_save, psz_save_dest[arr_idx], BUFF_SIZE_10), 0);
		arr_idx++;
		res_ptr = strtok_r(NULL, " ", &psz_save);
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strcasestr
* @brief                :Returns a pointer to the first occurrence of str2 in str1,\
*                        or a null pointer if str2 is not part of str1
*                        It is like strstr(), but it ignores the case of both arguments
* @Scenario             :A pointer to the first occurrence in str1 of the entire sequence of characters specified in str2
* API's covered         :strcasestr
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/

static void tc_libc_string_strcasestr(void)
{
	char dest_arr[BUFF_SIZE_10] = "Str Test";
	char *res_ptr = NULL;
	char *psz = "str";

	res_ptr = strcasestr(dest_arr, psz);
	TC_ASSERT_NEQ("strcasestr", res_ptr, NULL);
	TC_ASSERT_EQ("strcasestr", strncmp(res_ptr, dest_arr, BUFF_SIZE_10), 0);

	psz = "test";
	res_ptr = strcasestr(dest_arr, psz);
	TC_ASSERT_NEQ("strcasestr", res_ptr, NULL);
	TC_ASSERT_EQ("strcasestr", strncmp(res_ptr, dest_arr + 4, BUFF_SIZE_10), 0);

	psz = "notfound";
	res_ptr = strcasestr(dest_arr, psz);
	TC_ASSERT_EQ("strcasestr", res_ptr, NULL);

	psz = "";
	res_ptr = strcasestr(dest_arr, psz);
	TC_ASSERT_NEQ("strcasestr", res_ptr, NULL);
	TC_ASSERT_EQ("strcasestr", strncmp(res_ptr, dest_arr, BUFF_SIZE_10), 0);

	TC_SUCCESS_RESULT();
}

static void tc_libc_string_memccpy(void)
{
	char *test1_src = "abcdefghijkl";
	char some_str[50];
	char test_result[50] = "abcdefg";
	char *res_ptr;

	memset(some_str, 0, sizeof(some_str));

	res_ptr = (char *)memccpy(some_str, test1_src, 'g', strlen(test1_src));
	TC_ASSERT_NEQ("memccpy", res_ptr, NULL);
	TC_ASSERT_EQ("memccpy", strcmp(some_str, test_result), 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_string_strlcpy
* @brief                :copy the str for n size with NUL-terminating
* @Scenario             :after copying, check the return val which means length of copied str, check
*                       NUL-terminating, and check the contents whether copied well or not.
* API's covered         :strlcpy
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strlcpy(void)
{
	int cpy_len;
	char str_dest[BUFF_SIZE_10] = "abcdefghi";
	char str_src[BUFF_SIZE_10] = "jklmnopqr";

	/* 1. check that the return val is the total length of src
	 * 2. check that NUL-terminating
	 * 3. check that copied string is correct */

	cpy_len = strlcpy(str_dest, str_src, BUFF_SIZE_10 / 2);
	TC_ASSERT_EQ("strlcpy", cpy_len, strlen(str_src));
	TC_ASSERT_EQ("strlcpy", str_dest[(BUFF_SIZE_10 / 2) - 1], '\0');
	TC_ASSERT_EQ("strlcpy", strncmp(str_dest, str_src, (BUFF_SIZE_10 / 2) - 1), 0);

	TC_SUCCESS_RESULT();
}
/**
* @fn                   :tc_libc_string_strtof
* @brief                :convert the string to float value
* @Scenario             :put string to strtof and check the return which points the string value
* API's covered         :strtof
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strtof(void)
{
	char *str;
	char *ptr;
	float value;

	str = "123.456TizenRT";
	value = strtof(str, &ptr);
#ifdef CONFIG_LIBM
	TC_ASSERT_LEQ("strtof", roundf((fabsf(value - 123.456f) * 1000) / 1000), FLT_EPSILON);
#endif
	TC_ASSERT_EQ("strtof", strncmp(ptr, "TizenRT", strlen("TizenRT")), 0);

	str = "-78.9123TinyAra";
	value = strtof(str, &ptr);
#ifdef CONFIG_LIBM
	TC_ASSERT_LEQ("strtof", roundf((fabsf(value - (-78.9123f)) * 10000) / 10000), FLT_EPSILON);
#endif
	TC_ASSERT_EQ("strtof", strncmp(ptr, "TinyAra", strlen("TinyAra")), 0);

	TC_SUCCESS_RESULT();
}
/**
* @fn                   :tc_libc_string_strtold
* @brief                :convert the string to long double value
* @Scenario             :put string to strtold and check the return which points the string value
* API's covered         :strtold
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_libc_string_strtold(void)
{
	char *str;
	char *ptr;
	long double value;

	str = "123.456TizenRT";
	value = strtold(str, &ptr);
#ifdef CONFIG_LIBM
	TC_ASSERT_LEQ("strtold", roundl((fabsl(value - 123.456) * 1000) / 1000), DBL_EPSILON);
#endif
	TC_ASSERT_EQ("strtold", strncmp(ptr, "TizenRT", strlen("TizenRT")), 0);

	str = "-78.9123TinyAra";
	value = strtold(str, &ptr);
#ifdef CONFIG_LIBM
	TC_ASSERT_LEQ("strtold", roundl((fabsl(value - (-78.9123)) * 10000) / 10000), DBL_EPSILON);
#endif
	TC_ASSERT_EQ("strtold", strncmp(ptr, "TinyAra", strlen("TinyAra")), 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_string
 ****************************************************************************/

int libc_string_main(void)
{
	tc_libc_string_memcpy();
	tc_libc_string_memset();
	tc_libc_string_memchr();
	tc_libc_string_memcmp();
	tc_libc_string_memmove();
	tc_libc_string_stpcpy();
	tc_libc_string_strcasecmp();
	tc_libc_string_strcat();
	tc_libc_string_strchr();
	tc_libc_string_strcmp();
	tc_libc_string_strcpy();
	tc_libc_string_strcspn();
	tc_libc_string_strdup();
#ifdef CONFIG_LIBC_STRERROR
	tc_libc_string_strerror();
#endif
	tc_libc_string_strlen();
	tc_libc_string_strncasecmp();
	tc_libc_string_strncat();
	tc_libc_string_strncmp();
	tc_libc_string_strncpy();
	tc_libc_string_strndup();
	tc_libc_string_strnlen();
	tc_libc_string_strpbrk();
	tc_libc_string_strrchr();
	tc_libc_string_strspn();
	tc_libc_string_strstr();
	tc_libc_string_strtok();
	tc_libc_string_strtok_r();
	tc_libc_string_strcasestr();
	tc_libc_string_memccpy();
	tc_libc_string_strlcpy();
	tc_libc_string_strtof();
	tc_libc_string_strtold();

	return 0;
}
