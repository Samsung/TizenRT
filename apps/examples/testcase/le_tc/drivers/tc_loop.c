/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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

/// @file tc_loop.c
/// @brief Test Case Example for loop driver
#include <tinyara/config.h>
#include <tinyara/fs/fs.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "tc_internal.h"
#include <errno.h>

/**
 * @brief Closes all open block devices
 */
static inline void close_fds(FAR struct inode *pnode[255], int count)
{
	for (; count >= 0; count--) {
		close_blockdriver(pnode[count]);
	}
}

/**
 * Closes the mentioned block driver node and free the buffer
 */
#define cleanup(pnode, buf) \
do {\
	free(buf);\
	close_blockdriver(pnode);\
} while (0)

/**
* @fn                   :tc_driver_loop_geometry
* @brief                :Test the loop driver geometry api
* @scenario             :Test the loop driver geometry api
* API's covered         :geometry
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_loop_geometry(void)
{
	int ret = 0;
	FAR struct inode *pnode;

	/* Positive testcases */
	ret = open_blockdriver("/dev/loop", 0, &pnode);
	TC_ASSERT_EQ("loop_open", ret, OK);

	FAR struct geometry geo;
	ret = pnode->u.i_bops->geometry(pnode, &geo);
	TC_ASSERT_EQ_CLEANUP("loop_geometry", ret, OK, close_blockdriver(pnode));
	TC_ASSERT_EQ_CLEANUP("loop_geometry", geo.geo_available, true, close_blockdriver(pnode));
	TC_ASSERT_EQ_CLEANUP("loop_geometry", geo.geo_mediachanged, false, close_blockdriver(pnode));
	TC_ASSERT_EQ_CLEANUP("loop_geometry", geo.geo_sectorsize, 512, close_blockdriver(pnode));
	TC_ASSERT_EQ_CLEANUP("loop_geometry", geo.geo_nsectors, 2, close_blockdriver(pnode));

	/* Negative test cases */
	ret = pnode->u.i_bops->geometry(pnode, NULL);
	TC_ASSERT_LT_CLEANUP("loop_geometry", ret, 0, close_blockdriver(pnode));

	close_blockdriver(pnode);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_loop_read
* @brief                :Test the loop driver read api
* @scenario             :Test the loop driver read api
* API's covered         :read
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_loop_read(void)
{
	int ret = 0;
	FAR struct inode *pnode;
	unsigned char *read_buf = malloc(1024);

	TC_ASSERT_NEQ("malloc", read_buf, NULL);

	/* Positive testcases */
	ret = open_blockdriver("/dev/loop", 0, &pnode);
	TC_ASSERT_EQ_CLEANUP("loop_open", ret, OK, free(read_buf));

	ret = pnode->u.i_bops->read(pnode, read_buf, 0, 2);
	TC_ASSERT_GEQ_CLEANUP("loop_read", ret, 0, cleanup(pnode, read_buf));

	/* Negative test cases */
	ret = pnode->u.i_bops->read(pnode, read_buf, 0, 5);
	TC_ASSERT_LT_CLEANUP("loop_read", ret, 0, cleanup(pnode, read_buf));

	close_blockdriver(pnode);
	free(read_buf);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_loop_write
* @brief                :Test the loop driver write api
* @scenario             :Test the loop driver write api
* API's covered         :write
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
#ifdef CONFIG_FS_WRITABLE
static void tc_driver_loop_write(void)
{
	int ret = 0;
	FAR struct inode *pnode;
	unsigned char *read_buf = malloc(1024);

	TC_ASSERT_NEQ("malloc", read_buf, NULL);

	strncpy((char *)read_buf, "Test loop driver", 16);

	/* Positive testcases */
	ret = open_blockdriver("/dev/loop", 0, &pnode);
	TC_ASSERT_EQ_CLEANUP("loop_open", ret, OK, free(read_buf));

	ret = pnode->u.i_bops->write(pnode, read_buf, 0, 2);
	TC_ASSERT_GEQ_CLEANUP("loop_write", ret, 0, cleanup(pnode, read_buf));

	memset(read_buf, 0, 1024);
	ret = pnode->u.i_bops->read(pnode, read_buf, 0, 1);
	TC_ASSERT_GEQ_CLEANUP("loop_read", ret, 0, cleanup(pnode, read_buf));
	TC_ASSERT_EQ_CLEANUP("loop_write", strncmp((const char *)read_buf, "Test loop driver", 16), 0, cleanup(pnode, read_buf));

	close_blockdriver(pnode);
	free(read_buf);

	TC_SUCCESS_RESULT();
}
#endif

/**
* @fn                   :tc_driver_loop_open_close
* @brief                :Test the loop driver open and close
* @scenario             :Test the loop driver open and close
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_loop_open_close(void)
{
	int ret = 0;
	FAR struct inode *pnode;

	/* Positiee testcases */
	ret = open_blockdriver("/dev/loop", 0, &pnode);
	TC_ASSERT_EQ("loop_open", ret, OK);

	ret = close_blockdriver(pnode);
	TC_ASSERT_EQ("loop_close", ret, OK);

	/* Negtive testcases */
	ret = close_blockdriver(pnode);
	TC_ASSERT_LT("loop_close", ret, 0);

	int count = 0;
	FAR struct inode *pnodes[255];
	for (count = 0; count < 255; count++) {
		ret = open_blockdriver("/dev/loop", 0, &pnodes[count]);
		if (ret < 0) {
			TC_ASSERT_EQ_CLEANUP("adc_open", ret, -EMFILE, close_fds(&pnodes[0], --count));
			break;
		}
	}

	close_fds(&pnodes[0], --count);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_loop_loteardown
* @brief                :Test the loop driver teardown api
* @scenario             :Test the loop driver teardown api
* API's covered         :loteardown
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_loop_teardown(void)
{
	int ret = 0;
	FAR struct inode *pnode;

	/* Negative testcases */
	ret = loteardown(NULL);
	TC_ASSERT_LT("loop_teardown", ret, 0);

	ret = open_blockdriver("/dev/loop", 0, &pnode);
	TC_ASSERT_EQ("loop_teardown", ret, OK);

	ret = loteardown("/dev/loop");
	TC_ASSERT_EQ_CLEANUP("loop_teardown", ret, -EBUSY, close_blockdriver(pnode));

	ret = close_blockdriver(pnode);
	TC_ASSERT_EQ("loop_close", ret, OK);

	/* Positive testcases */
	ret = loteardown("/dev/loop");
	TC_ASSERT_EQ("loop_teardown", ret, OK);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_loop_losetup
* @brief                :Test the loop driver setup api
* @scenario             :Test the loop driver setup api
* API's covered         :losetup
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_loop_losetup(void)
{
	int ret = 0;
	int fd = 0;

	/* Negative test cases */
	ret = losetup(NULL, "/mnt/loopfile", 512, 0, false);
	TC_ASSERT_LT("loop_losetup", ret, 0);

	ret = losetup("/dev/loop", NULL, 512, 0, false);
	TC_ASSERT_LT("loop_losetup", ret, 0);

	ret = losetup("/dev/loop", "/mnt/loopfile", 0, 0, false);
	TC_ASSERT_LT("loop_losetup", ret, 0);

	unlink("/mnt/loopfile");
	ret = losetup("/dev/loop", "/mnt/loopfile_false", 512, 0, false);
	TC_ASSERT_LT("loop_losetup", ret, 0);

	ret = losetup("/dev/loop", "/mnt/loopfile_false", 512, 0, true);
	TC_ASSERT_LT("loop_losetup", ret, 0);

	fd = open("/mnt/loopfile", O_RDWR | O_CREAT);
	TC_ASSERT_GT("loop_losetup", fd, 0);

	ret = losetup("/dev/loop", "/mnt/loopfile", 512, 0, false);
	TC_ASSERT_LT_CLEANUP("loop_losetup", ret, 0, close(fd));
	close(fd);
	unlink("/mnt/loopfile");

	/* Positive test cases */
	FILE *fp = fopen("/mnt/loopfile", "w");
	TC_ASSERT_NEQ("fopen", fp, NULL);

	int i;
	for (i = 0; i < 1024; i++) {
		fputc('a', fp);
	}
	fclose(fp);
	ret = losetup("/dev/loop", "/mnt/loopfile", 512, 0, false);
	TC_ASSERT_EQ("loop_losetup", ret, OK);

	ret = loteardown("/dev/loop");
	TC_ASSERT_EQ("loop_teardown", ret, OK);

	ret = losetup("/dev/loop", "/mnt/loopfile", 512, 0, true);
	TC_ASSERT_EQ("loop_losetup", ret, OK);

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: loop driver test
 ****************************************************************************/
void loop_main(void)
{
	tc_driver_loop_losetup();
	tc_driver_loop_open_close();
	tc_driver_loop_geometry();
	tc_driver_loop_read();
#ifdef CONFIG_FS_WRITABLE
	tc_driver_loop_write();
#endif
	tc_driver_loop_teardown();

	return;
}
