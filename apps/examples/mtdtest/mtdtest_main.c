/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <tinyara/mtd/mtd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MTDTEST_DEFAULT_DEV    "/dev/mtdblock7"
#define MTDTEST_LG_BLKSIZE     512
#define MTDTEST_DEFAULT_EBLKS  4
#define MTDTEST_DEFAULT_PASSES 1
#define MTDTEST_OK             0
#define MTDTEST_FAIL           1

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mtdtest_protected_reason
 *
 * Description:
 *   The rtl8730e/tash partition table is
 *
 *     4: kernel   slot A (the image that is currently executed in place)
 *     5: kernel   slot B (the OTA backup image)
 *     7: reserved (FTL, unmounted -- the scratch target of this test)
 *     8: bootparam (8K, owned by the binary manager)
 *
 *   only /dev/mtdblock7 is a safe target.  Refusing the others avoids
 *   bricking the board by a typo.
 *
 ****************************************************************************/

static FAR const char *mtdtest_protected_reason(int minor)
{
	switch (minor) {
	case 4:
		return "kernel slot A (executed in place, writing would brick the board)";
	case 5:
		return "kernel slot B (OTA backup image)";
	case 8:
		return "boot parameters (owned by the binary manager)";
	default:
		return NULL;
	}
}

/****************************************************************************
 * Name: mtdtest_read_exact / mtdtest_write_exact
 ****************************************************************************/

static int mtdtest_read_exact(int fd, FAR uint8_t *buf, size_t len)
{
	size_t done = 0;
	ssize_t n;

	while (done < len) {
		n = read(fd, buf + done, len - done);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			return MTDTEST_FAIL;
		}
		if (n == 0) {
			return MTDTEST_FAIL;
		}
		done += n;
	}
	return MTDTEST_OK;
}

static int mtdtest_write_exact(int fd, FAR const uint8_t *buf, size_t len)
{
	size_t done = 0;
	ssize_t n;

	while (done < len) {
		n = write(fd, buf + done, len - done);
		if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			return MTDTEST_FAIL;
		}
		done += n;
	}
	return MTDTEST_OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mtdtest_main
 *
 * Description:
 *   mtdtest [-d dev] [-b erase-blocks] [-p passes] [-y] [-F] [-s]
 *
 *   For every pass and every erase block (starting at erase block 1,
 *   block 0 holds the FTL bookkeeping), write a deterministic pattern,
 *   read the data back and compare.  The write path goes through the
 *   FTL into the MTD erase and program operations of the flash driver,
 *   which is where the XIP-safe CPU pause has to happen on rtl8730e.
 *
 ****************************************************************************/

int mtdtest_main(int argc, char **argv)
{
	FAR const char *dev = MTDTEST_DEFAULT_DEV;
	FAR const char *reason;
	FAR uint8_t *wbuf = NULL;
	FAR uint8_t *rbuf = NULL;
	struct mtd_geometry_s geo;
	static const char spin[] = "|-\\-/";
	char ans[8];
	size_t blkper;
	uint8_t pattern;
	off_t off;
	int fd;
	int minor = -1;
	int eblks = MTDTEST_DEFAULT_EBLKS;
	int passes = MTDTEST_DEFAULT_PASSES;
	int assume_yes = 0;
	int force = 0;
	int spinner = 0;
	int pass;
	int blk;
	int failed = 0;
	int i;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
			dev = argv[++i];
		} else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
			eblks = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
			passes = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-y") == 0) {
			assume_yes = 1;
		} else if (strcmp(argv[i], "-F") == 0) {
			force = 1;
		} else if (strcmp(argv[i], "-s") == 0) {
			spinner = 1;
		} else {
			printf("Usage: mtdtest [-d dev] [-b erase-blocks] [-p passes]"
			       " [-y] [-F] [-s]\n");
			return ERROR;
		}
	}

	/* Never touch the partitions the board depends on, unless forced */

	if (sscanf(dev, "/dev/mtdblock%d", &minor) != 1) {
		minor = -1;
	}
	reason = mtdtest_protected_reason(minor);
	if (reason != NULL && !force) {
		printf("mtdtest: refusing %s: %s\n", dev, reason);
		printf("mtdtest: use -F to override at your own risk\n");
		return ERROR;
	}

	fd = open(dev, O_RDWR);
	if (fd < 0) {
		printf("mtdtest: cannot open %s, errno %d\n", dev, get_errno());
		return ERROR;
	}

	if (ioctl(fd, MTDIOC_GEOMETRY, (unsigned long)((uintptr_t)&geo)) < 0) {
		printf("mtdtest: MTDIOC_GEOMETRY failed on %s, errno %d\n",
		       dev, get_errno());
		close(fd);
		return ERROR;
	}

	if (geo.erasesize == 0 || geo.erasesize % MTDTEST_LG_BLKSIZE != 0) {
		printf("mtdtest: unsupported erasesize %lu on %s\n",
		       (unsigned long)geo.erasesize, dev);
		close(fd);
		return ERROR;
	}
	blkper = geo.erasesize / MTDTEST_LG_BLKSIZE;

	if (eblks <= 0 || eblks > (int)geo.neraseblocks - 1) {
		eblks = (int)geo.neraseblocks - 1;
	}
	if (passes <= 0) {
		passes = MTDTEST_DEFAULT_PASSES;
	}

	printf("mtdtest: %s: %lu bytes block, %lu bytes erase, %lu erase blocks\n",
	       dev, (unsigned long)geo.blocksize, (unsigned long)geo.erasesize,
	       (unsigned long)geo.neraseblocks);
	printf("mtdtest: testing %d erase blocks (offset %lu ..), %d passes\n",
	       eblks, (unsigned long)geo.erasesize, passes);

	if (!assume_yes) {
		printf("mtdtest: %s will be erased and rewritten, proceed? (y/N) ", dev);
		fflush(stdout);
		if (fgets(ans, sizeof(ans), stdin) == NULL ||
			(ans[0] != 'y' && ans[0] != 'Y')) {
			printf("mtdtest: aborted\n");
			close(fd);
			return OK;
		}
	}

	wbuf = (FAR uint8_t *)malloc(geo.erasesize);
	rbuf = (FAR uint8_t *)malloc(geo.erasesize);
	if (wbuf == NULL || rbuf == NULL) {
		printf("mtdtest: out of memory\n");
		free(wbuf);
		free(rbuf);
		close(fd);
		return ERROR;
	}

	for (pass = 1; pass <= passes && !failed; pass++) {
		for (blk = 0; blk < eblks; blk++) {
			/* Skip erase block 0, it holds the FTL bookkeeping */

			off = (off_t)(blk + 1) * (off_t)geo.erasesize;
			pattern = (uint8_t)(0xA5 ^ (uint8_t)pass ^ (uint8_t)blk);

			if (lseek(fd, off, SEEK_SET) < 0) {
				printf("mtdtest: lseek to %ld failed, errno %d\n",
			       (long)off, get_errno());
				failed = 1;
				break;
			}

			memset(wbuf, pattern, geo.erasesize);
			if (mtdtest_write_exact(fd, wbuf, geo.erasesize) != MTDTEST_OK) {
				printf("mtdtest: write failed at offset %ld, errno %d\n",
			       (long)off, get_errno());
				failed = 1;
				break;
			}

			if (lseek(fd, off, SEEK_SET) < 0) {
				failed = 1;
				break;
			}
			memset(rbuf, ~pattern, geo.erasesize);
			if (mtdtest_read_exact(fd, rbuf, geo.erasesize) != MTDTEST_OK) {
				printf("mtdtest: read failed at offset %ld, errno %d\n",
			       (long)off, get_errno());
				failed = 1;
				break;
			}
			if (memcmp(wbuf, rbuf, geo.erasesize) != 0) {
				printf("mtdtest: verify failed at offset %ld (block %d,"
			       " pattern 0x%02x)\n", (long)off, blk, pattern);
				failed = 1;
				break;
			}

			if (spinner) {
				printf("\rmtdtest: [%d/%d] erase/write/verify %c",
			       blk + 1, eblks, spin[(blk + pass) & 3]);
			} else {
				printf("mtdtest: [%d/%d] block %d (offset %ld) OK\n",
			       blk + 1, eblks, blk, (long)off);
			}
			fflush(stdout);
		}
	}
	if (spinner) {
		printf("\n");
	}

	free(wbuf);
	free(rbuf);
	close(fd);

	if (failed) {
		printf("mtdtest: FAILED on %s\n", dev);
		return ERROR;
	}

	printf("mtdtest: PASS: %d erase blocks x %d passes erased, written"
	       " and verified on %s\n", eblks, passes, dev);
	return OK;
}
