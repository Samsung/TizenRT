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

/// @file libc_libgen.c

/// @brief Test Case Example for Libc Libgen API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>
#include "tc_internal.h"

/**
 * @fn                  :tc_libc_libgen_basename
 * @brief               :The basename command strips any "path" name components from a filename, leaving you with a "pure" filename.
 * @scenario            :parsing pathname components
 * @API's covered       :basename
 * @Preconditions       :None
 * @Postonditions       :None
 * @return              :void
 */
static void tc_libc_libgen_basename(void)
{
	char *psz_str = NULL;
	char *path = "/etc/passwd";

	psz_str = basename(path);
	TC_ASSERT_EQ("basename", strncmp(psz_str, "passwd", strlen(psz_str)), 0);

	path = "/etc";
	psz_str = basename(path);
	TC_ASSERT_EQ("basename", strncmp(psz_str, "etc", strlen(psz_str)), 0);

	path = "etc";
	psz_str = basename(path);
	TC_ASSERT_EQ("basename", strncmp(psz_str, "etc", strlen(psz_str)), 0);

	path = "/";
	psz_str = basename(path);
	TC_ASSERT_EQ("basename", strncmp(psz_str, "/", strlen(psz_str)), 0);

	path = ".";
	psz_str = basename(path);
	TC_ASSERT_EQ("basename", strncmp(psz_str, ".", strlen(psz_str)), 0);

	path = "..";
	psz_str = basename(path);
	TC_ASSERT_EQ("basename", strncmp(psz_str, "..", strlen(psz_str)), 0);

	TC_SUCCESS_RESULT();
}

/**
 * @fn                  :tc_libc_libgen__dirname
 * @brief               :The dirname() function shall take a pointer to a character string that contains a pathname,
 *                       and return a pointer to string that is a pathname of the parent directory of that file
 * @scenario            :parsing pathname components
 * @API's covered       :dirname
 * @Preconditions       :None
 * @Postonditions       :None
 * @return              :void
 */
static void tc_libc_libgen_dirname(void)
{
	char *psz_str = NULL;
	char path[] = "/etc/passwd";

	psz_str = dirname(path);
	TC_ASSERT_EQ("dirname", strncmp(psz_str, "/etc", strlen(psz_str)), 0);

	strncpy(path, "/etc", strlen(path));
	psz_str = dirname(path);
	TC_ASSERT_EQ("dirname", strncmp(psz_str, "/", strlen(psz_str)), 0);

	strncpy(path, "etc", strlen(path));
	psz_str = dirname(path);
	TC_ASSERT_EQ("dirname", strncmp(psz_str, ".", strlen(psz_str)), 0);

	strncpy(path, "/", strlen(path));
	psz_str = dirname(path);
	TC_ASSERT_EQ("dirname", strncmp(psz_str, "/", strlen(psz_str)), 0);

	strncpy(path, ".", strlen(path));
	psz_str = dirname(path);
	TC_ASSERT_EQ("dirname", strncmp(psz_str, ".", strlen(psz_str)), 0);

	strncpy(path, "..", strlen(path));
	psz_str = dirname(path);
	TC_ASSERT_EQ("dirname", strncmp(psz_str, ".", strlen(psz_str)), 0);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_libgen
 ****************************************************************************/

int libc_libgen_main(void)
{
	tc_libc_libgen_basename();
	tc_libc_libgen_dirname();

	return 0;
}
