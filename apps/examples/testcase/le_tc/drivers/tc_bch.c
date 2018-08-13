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

/// @file tc_bch.c
/// @brief Test Case Example for bch driver
#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "tc_internal.h"
#include <errno.h>
#include <sys/ioctl.h>

#define SECT_SIZE	512

/* Closes all open file descriptors */
static inline void close_fds(int *fds, int count)
{
	for (; count >= 0; count--) {
		close(fds[count]);
	}
}

/* Close given file descriptor and free the buffer */
#define cleanup(fd, buf) \
do {\
	close(fd);\
	free(buf);\
} while (0)

/* Close the file descriptor and unregister the bch device */
#define clean_unreg(fd) \
do {\
	close(fd);\
	bchdev_unregister("/dev/tmpbchdevrw");\
} while (0)

/**
* @fn                   :tc_driver_bch_register
* @brief                :Test the bch driver register
* @scenario             :Test the bch driver register
* API's covered         :register
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_bch_register(void)
{
	int ret = 0;

	ret = bchdev_register("/dev/mtdblock1", "/dev/tmpbchdevrw", false);
	TC_ASSERT_GEQ("bchdev_register", ret, 0);

	ret = bchdev_register("/dev/mtdblock1", "/dev/tmpbchdevro", true);
	TC_ASSERT_GEQ("bchdev_register", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_bch_unregister
* @brief                :Test the bch driver unregister
* @scenario             :Test the bch driver unregister
* API's covered         :unregister
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_bch_unregister(void)
{
	int ret = 0;

	ret = bchdev_unregister("/dev/tmpbchdevrw");
	TC_ASSERT_GEQ("bchdev_unregister", ret, 0);

	ret = bchdev_unregister("/dev/tmpbchdevro");
	TC_ASSERT_GEQ("bchdev_unregister", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_bch_open_close
* @brief                :Test the bch driver open and close
* @scenario             :Test the bch driver open and close
* API's covered         :open, close
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_bch_open_close(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/tmpbchdevrw", O_RDWR);
	TC_ASSERT_GT("bch_open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("bch_close", ret, OK);

	fd = open("/dev/tmpbchdevro", O_RDONLY);
	TC_ASSERT_GT("bch_open", fd, 0);

	ret = close(fd);
	TC_ASSERT_EQ("bch_close", ret, OK);

	/* Negative test cases */
	int count = 0;
	int fds[255] = { 0, };
	for (count = 0; count < 255; count++) {
		fds[count] = open("/dev/tmpbchdevrw", O_RDWR);
		if (fds[count] < 0) {
			TC_ASSERT_EQ_CLEANUP("bch_open", errno, EMFILE, close_fds(fds, --count));
			break;
		}
	}

	close_fds(fds, --count);

	/* Negative test cases */
	ret = close(fd);
	TC_ASSERT_LT("bch_close", ret, 0);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_bch_read_write
* @brief                :Test the bch driver read and write
* @scenario             :Test the bch driver read and write
* API's covered         :read, write, seek
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_bch_read_write(void)
{
	int fd = 0;
	int ret = 0;
	char *buf = malloc(2 * SECT_SIZE);

	TC_ASSERT_GT("malloc", buf, 0);

	fd = open("/dev/tmpbchdevrw", O_RDWR);
	TC_ASSERT_GT_CLEANUP("bch_open", fd, 0, free(buf));

	/* Positive test cases */
	/* Test case for zero length */
	ret = read(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, 0, cleanup(fd, buf));
	ret = write(fd, NULL, 0);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, 0, cleanup(fd, buf));

	/* Test case for boundary condition */
	ret = lseek(fd, 100, SEEK_END);
	TC_ASSERT_GT_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = read(fd, buf, 1);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, 0, cleanup(fd, buf));

	ret = lseek(fd, 100, SEEK_END);
	TC_ASSERT_GT_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = write(fd, buf, 1);
	TC_ASSERT_LT_CLEANUP("bch_write", ret, 0, cleanup(fd, buf));

	/* Test case for small size data (less than sector size) */
	ret = lseek(fd, 1000, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 1000, cleanup(fd, buf));
	ret = read(fd, buf, 20);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, 20, cleanup(fd, buf));

	ret = lseek(fd, 1000, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 1000, cleanup(fd, buf));
	ret = write(fd, buf, 20);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, 20, cleanup(fd, buf));

	/* Test case for small size data, but accross sector boundaries */
	ret = lseek(fd, 1020, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 1020, cleanup(fd, buf));
	ret = read(fd, buf, 20);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, 20, cleanup(fd, buf));

	ret = lseek(fd, 1020, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 1020, cleanup(fd, buf));
	ret = write(fd, buf, 20);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, 20, cleanup(fd, buf));

	/* Test case for large size data (more than sector size) */
	ret = lseek(fd, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = read(fd, buf, SECT_SIZE + 20);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, SECT_SIZE + 20, cleanup(fd, buf));

	ret = lseek(fd, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = write(fd, buf, SECT_SIZE + 20);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, SECT_SIZE + 20, cleanup(fd, buf));

	/* Test case for large size data beyond block device boundary */
	ret = lseek(fd, 0, SEEK_END);
	TC_ASSERT_GT_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = lseek(fd, -SECT_SIZE, SEEK_CUR);
	TC_ASSERT_GT_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = read(fd, buf, SECT_SIZE + 20);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, SECT_SIZE, cleanup(fd, buf));

	ret = lseek(fd, 0, SEEK_END);
	TC_ASSERT_GT_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = lseek(fd, -SECT_SIZE, SEEK_CUR);
	TC_ASSERT_GT_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = write(fd, buf, SECT_SIZE + 20);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, SECT_SIZE, cleanup(fd, buf));

	/* Test case for verifying write data integrity */
	char tmp[15];
	ret = lseek(fd, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = read(fd, buf, 20);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, 20, cleanup(fd, buf));
	ret = lseek(fd, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = write(fd, "Test bch write", 14);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, 14, cleanup(fd, buf));
	ret = lseek(fd, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = read(fd, tmp, 14);
	TC_ASSERT_EQ_CLEANUP("bch_read", ret, 14, cleanup(fd, buf));
	TC_ASSERT_EQ_CLEANUP("bch_write", strncmp("Test bch write", tmp, 14), 0, cleanup(fd, buf));
	ret = lseek(fd, 0, SEEK_SET);
	TC_ASSERT_EQ_CLEANUP("bch_seek", ret, 0, cleanup(fd, buf));
	ret = write(fd, buf, 20);
	TC_ASSERT_EQ_CLEANUP("bch_write", ret, 20, cleanup(fd, buf));

	cleanup(fd, buf);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_bch_ioctl
* @brief                :Test the bch driver ioctl
* @scenario             :Test the bch driver ioctl
* API's covered         :ioctl
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
static void tc_driver_bch_ioctl(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/tmpbchdevrw", O_RDWR);
	TC_ASSERT_GT("bch_open", fd, 0);

	/* Positive test cases */
#ifdef CONFIG_BCH_ENCRYPTION
	char buf[CONFIG_BCH_ENCRYPTION_KEY_SIZE];
	ret = ioctl(fd, DIOC_SETKEY, buf);
	TC_ASSERT_EQ_CLEANUP("bch_ioctl", ret, OK, close(fd));
#endif

	/* Negative test cases */
	ret = ioctl(fd, DIOC_GETPRIV, 0);
	TC_ASSERT_LT_CLEANUP("bch_ioctl", ret, 0, close(fd));

	ret = ioctl(fd, -1, 0);
	TC_ASSERT_LT_CLEANUP("bch_ioctl", ret, 0, close(fd));

	close(fd);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_driver_bch_unlink
* @brief                :Test the bch driver unlink
* @scenario             :Test the bch driver unlink
* API's covered         :unlink
* Preconditions         :none
* Postconditions        :none
* @return               :void
*/
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
static void tc_driver_bch_unlink(void)
{
	int ret = 0;
	int fd = 0;

	ret = bchdev_register("/dev/mtdblock1", "/dev/tmpbchdevrw", false);
	TC_ASSERT_GEQ("bchdev_register", ret, 0);

	fd = open("/dev/tmpbchdevrw", O_RDWR);
	TC_ASSERT_GT_CLEANUP("bch_open", fd, 0, bchdev_unregister("/dev/tmpbchdevrw"));

	ret = unlink("/dev/tmpbchdevrw");
	TC_ASSERT_EQ_CLEANUP("bch_unlink", ret, OK, clean_unreg(fd));

	ret = close(fd);
	TC_ASSERT_EQ_CLEANUP("bch_close", ret, OK, bchdev_unregister("/dev/tmpbchdevrw"));

	ret = bchdev_register("/dev/mtdblock1", "/dev/tmpbchdevrw", false);
	TC_ASSERT_GEQ("bchdev_register", ret, 0);

	ret = unlink("/dev/tmpbchdevrw");
	TC_ASSERT_EQ_CLEANUP("bch_unlink", ret, OK, bchdev_unregister("/dev/tmpbchdevrw"));

	TC_SUCCESS_RESULT();
}
#endif

/****************************************************************************
 * Name: bch driver test
 ****************************************************************************/
void bch_main(void)
{
	tc_driver_bch_register();
	tc_driver_bch_open_close();
	tc_driver_bch_read_write();
	tc_driver_bch_ioctl();
	tc_driver_bch_unregister();
#ifndef CONFIG_DISABLE_PSEUDOFS_OPERATIONS
	tc_driver_bch_unlink();
#endif

	return;
}
