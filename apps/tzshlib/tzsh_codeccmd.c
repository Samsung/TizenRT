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
/****************************************************************************
 * apps/tzshlib/tzsh_codeccmd.c
 *
 * This file is part of NuttX, contributed by Darcy Gong
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Darcy Gong 2012-10-30
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#ifdef CONFIG_NETUTILS_CODECS

#include <sys/stat.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <fcntl.h>
#include <libgen.h>
#include <errno.h>
#include <debug.h>

#if defined(CONFIG_TZSH_DISABLE_URLENCODE) && defined(CONFIG_TZSH_DISABLE_URLDECODE)
#undef CONFIG_CODECS_URLCODE
#endif

#ifdef CONFIG_CODECS_URLCODE
#include "netutils/urldecode.h"
#endif

#if defined(CONFIG_TZSH_DISABLE_BASE64ENC) && defined(CONFIG_TZSH_DISABLE_BASE64DEC)
#undef CONFIG_CODECS_BASE64
#endif

#ifdef CONFIG_CODECS_BASE64
#include "netutils/base64.h"
#endif

#if defined(CONFIG_CODECS_HASH_MD5) && !defined(CONFIG_TZSH_DISABLE_MD5)
#include "netutils/md5.h"
#endif

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_TZSH_CODECS_BUFSIZE
#define CONFIG_TZSH_CODECS_BUFSIZE    128
#endif

#undef NEED_CMD_CODECS_PROC
#undef HAVE_CODECS_URLENCODE
#undef HAVE_CODECS_URLDECODE
#undef HAVE_CODECS_BASE64ENC
#undef HAVE_CODECS_BASE64DEC
#undef HAVE_CODECS_HASH_MD5

#if defined(CONFIG_CODECS_URLCODE) && !defined(CONFIG_TZSH_DISABLE_URLENCODE)
#define HAVE_CODECS_URLENCODE 1
#endif

#if defined(CONFIG_CODECS_URLCODE) && !defined(CONFIG_TZSH_DISABLE_URLDECODE)
#define HAVE_CODECS_URLDECODE 1
#endif

#if defined(CONFIG_CODECS_BASE64) && !defined(CONFIG_TZSH_DISABLE_BASE64ENC)
#define HAVE_CODECS_BASE64ENC 1
#endif

#if defined(CONFIG_CODECS_BASE64) && !defined(CONFIG_TZSH_DISABLE_BASE64DEC)
#define HAVE_CODECS_BASE64DEC 1
#endif

#if defined(CONFIG_CODECS_HASH_MD5) && !defined(CONFIG_TZSH_DISABLE_MD5)
#define HAVE_CODECS_HASH_MD5 1
#endif

#if defined(HAVE_CODECS_URLENCODE) || defined(HAVE_CODECS_URLDECODE) || \
	defined(HAVE_CODECS_BASE64ENC) || defined(HAVE_CODECS_BASE64DEC) || \
defined(HAVE_CODECS_HASH_MD5)
#define NEED_CMD_CODECS_PROC 1
#endif

#define CODEC_MODE_URLENCODE  1
#define CODEC_MODE_URLDECODE  2
#define CODEC_MODE_BASE64ENC  3
#define CODEC_MODE_BASE64DEC  4
#define CODEC_MODE_HASH_MD5   5

/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef void (*codec_callback_t)(FAR char *src, int srclen, FAR char *dest, FAR int *destlen, int mode);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: urlencode_cb
 ****************************************************************************/

#ifdef HAVE_CODECS_URLENCODE
static void urlencode_cb(FAR char *src, int srclen, FAR char *dest, FAR int *destlen, int mode)
{
	urlencode(src, srclen, dest, destlen);
}
#endif

/****************************************************************************
 * Name: urldecode_cb
 ****************************************************************************/

#ifdef HAVE_CODECS_URLDECODE
static void urldecode_cb(FAR char *src, int srclen, FAR char *dest, FAR int *destlen, int mode)
{
	urldecode(src, srclen, dest, destlen);
}
#endif

/****************************************************************************
 * Name: b64enc_cb
 ****************************************************************************/

#ifdef HAVE_CODECS_BASE64ENC
static void b64enc_cb(FAR char *src, int srclen, FAR char *dest, FAR int *destlen, int mode)
{
	if (mode == 0) {
		base64_encode((unsigned char *)src, srclen, (unsigned char *)dest, (size_t *) destlen);
	} else {
		base64w_encode((unsigned char *)src, srclen, (unsigned char *)dest, (size_t *) destlen);
	}
}
#endif

/****************************************************************************
 * Name: b64dec_cb
 ****************************************************************************/

#ifdef HAVE_CODECS_BASE64DEC
static void b64dec_cb(FAR char *src, int srclen, FAR char *dest, FAR int *destlen, int mode)
{
	if (mode == 0) {
		base64_decode((unsigned char *)src, srclen, (unsigned char *)dest, (size_t *) destlen);
	} else {
		base64w_decode((unsigned char *)src, srclen, (unsigned char *)dest, (size_t *) destlen);
	}
}
#endif

/****************************************************************************
 * Name: md5_cb
 ****************************************************************************/

#ifdef HAVE_CODECS_HASH_MD5
static void md5_cb(FAR char *src, int srclen, FAR char *dest, FAR int *destlen, int mode)
{
	MD5Update((MD5_CTX *) dest, (unsigned char *)src, srclen);
}
#endif

/****************************************************************************
 * Name: calc_codec_buffsize
 ****************************************************************************/

#ifdef NEED_CMD_CODECS_PROC
static int calc_codec_buffsize(int srclen, uint8_t mode)
{
	switch (mode) {
	case CODEC_MODE_URLENCODE:
		return srclen * 3 + 1;

	case CODEC_MODE_URLDECODE:
		return srclen + 1;

	case CODEC_MODE_BASE64ENC:
		return ((srclen + 2) / 3 * 4) + 1;

	case CODEC_MODE_BASE64DEC:
		return (srclen / 4 * 3 + 2) + 1;

	case CODEC_MODE_HASH_MD5:
		return 32 + 1;

	default:
		return srclen + 1;
	}
}
#endif

/****************************************************************************
 * Name: cmd_codecs_proc
 ****************************************************************************/

#ifdef NEED_CMD_CODECS_PROC
static int cmd_codecs_proc(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv, uint8_t mode, codec_callback_t func)
{
#ifdef HAVE_CODECS_HASH_MD5
	static const unsigned char hexchars[] = "0123456789abcdef";
	MD5_CTX ctx;
	unsigned char mac[16];
	FAR char *src;
	FAR char *dest;
#endif

	FAR char *localfile = NULL;
	FAR char *srcbuf = NULL;
	FAR char *destbuf = NULL;
	FAR char *fullpath = NULL;
	FAR const char *fmt;
	FAR char *sdata;
	bool badarg = false;
	bool isfile = false;
	bool iswebsafe = false;
	int option;
	int fd = -1;
	int buflen = 0;
	int srclen = 0;
	int ret = OK;

	/* Get the command options */

	while ((option = getopt(argc, argv, ":fw")) != ERROR) {
		switch (option) {
		case 'f':
			isfile = true;
			break;

#ifdef CONFIG_CODECS_BASE64
		case 'w':
			iswebsafe = true;

			if (!(mode == CODEC_MODE_BASE64ENC || mode == CODEC_MODE_BASE64DEC)) {
				badarg = true;
			}
			break;
#endif
		case ':':
			tzsh_output(vtbl, g_fmtargrequired, argv[0]);
			badarg = true;
			break;

		case '?':
		default:
			tzsh_output(vtbl, g_fmtarginvalid, argv[0]);
			badarg = true;
			break;
		}
	}

	/* If a bad argument was encountered, then return without processing the command */

	if (badarg) {
		return ERROR;
	}

	/* There should be exactly on parameter left on the command-line */

	if (optind == argc - 1) {
		sdata = argv[optind];
	} else if (optind >= argc) {
		fmt = g_fmttoomanyargs;
		goto errout;
	} else {
		fmt = g_fmtargrequired;
		goto errout;
	}

#ifdef HAVE_CODECS_HASH_MD5
	if (mode == CODEC_MODE_HASH_MD5) {
		MD5Init(&ctx);
	}
#endif

	if (isfile) {
		/* Get the local file name */

		localfile = sdata;

		/* Get the full path to the local file */

		fullpath = tzsh_getfullpath(vtbl, localfile);

		/* Open the local file for reading */

		fd = open(fullpath, O_RDONLY);
		if (fd < 0) {
			tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "open", TZSH_ERRNO);
			ret = ERROR;
			goto exit;
		}

		srcbuf = malloc(CONFIG_TZSH_CODECS_BUFSIZE + 2);
#ifdef HAVE_CODECS_BASE64ENC
		if (mode == CODEC_MODE_BASE64ENC) {
			srclen = CONFIG_TZSH_CODECS_BUFSIZE / 3 * 3;
		} else
#endif
		{
			srclen = CONFIG_TZSH_CODECS_BUFSIZE;
		}

		buflen = calc_codec_buffsize(srclen + 2, mode);
		destbuf = malloc(buflen);
		while (true) {
			memset(srcbuf, 0, srclen + 2);
			ret = read(fd, srcbuf, srclen);
			if (ret < 0) {
				tzsh_output(vtbl, g_fmtcmdfailed, argv[0], "read", TZSH_ERRNO);
				ret = ERROR;
				goto exit;
			} else if (ret == 0) {
				break;
			}
#ifdef HAVE_CODECS_URLDECODE
			if (mode == CODEC_MODE_URLDECODE) {
				if (srcbuf[srclen - 1] == '%') {
					ret += read(fd, &srcbuf[srclen], 2);
				} else if (srcbuf[srclen - 2] == '%') {
					ret += read(fd, &srcbuf[srclen], 1);
				}
			}
#endif
			memset(destbuf, 0, buflen);
			if (func) {
#ifdef HAVE_CODECS_HASH_MD5
				if (mode == CODEC_MODE_HASH_MD5) {
					func(srcbuf, ret, (char *)&ctx, &buflen, 0);
				} else
#endif
				{
					func(srcbuf, ret, destbuf, &buflen, (iswebsafe) ? 1 : 0);
					tzsh_output(vtbl, "%s", destbuf);
				}
			}

			buflen = calc_codec_buffsize(srclen + 2, mode);
		}

#ifdef HAVE_CODECS_HASH_MD5
		if (mode == CODEC_MODE_HASH_MD5) {
			int i;

			MD5Final(mac, &ctx);
			src = (FAR char *)&mac;
			dest = destbuf;
			for (i = 0; i < 16; i++, src++) {
				*dest++ = hexchars[(*src) >> 4];
				*dest++ = hexchars[(*src) & 0x0f];
			}

			*dest = '\0';
			tzsh_output(vtbl, "%s\n", destbuf);
		}
#endif
		ret = OK;
		goto exit;
	} else {
		srcbuf = sdata;
		srclen = strlen(sdata);
		buflen = calc_codec_buffsize(srclen, mode);
		destbuf = malloc(buflen);
		destbuf[0] = 0;
		if (!destbuf) {
			fmt = g_fmtcmdoutofmemory;
			goto errout;
		}

		memset(destbuf, 0, buflen);
		if (func) {
#ifdef HAVE_CODECS_HASH_MD5
			if (mode == CODEC_MODE_HASH_MD5) {
				int i;

				func(srcbuf, srclen, (char *)&ctx, &buflen, 0);
				MD5Final(mac, &ctx);
				src = (char *)&mac;
				dest = destbuf;
				for (i = 0; i < 16; i++, src++) {
					*dest++ = hexchars[(*src) >> 4];
					*dest++ = hexchars[(*src) & 0x0f];
				}

				*dest = '\0';
			} else
#endif
			{
				func(srcbuf, srclen, destbuf, &buflen, (iswebsafe) ? 1 : 0);
			}
		}

		tzsh_output(vtbl, "%s\n", destbuf);
		srcbuf = NULL;
		goto exit;
	}

exit:
	if (fd >= 0) {
		close(fd);
	}

	if (fullpath) {
		free(fullpath);
	}

	if (srcbuf) {
		free(srcbuf);
	}

	if (destbuf) {
		free(destbuf);
	}

	return ret;

errout:
	tzsh_output(vtbl, fmt, argv[0]);
	ret = ERROR;
	goto exit;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cmd_urlencode
 ****************************************************************************/

#ifdef HAVE_CODECS_URLENCODE
int cmd_urlencode(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return cmd_codecs_proc(vtbl, argc, argv, CODEC_MODE_URLENCODE, urlencode_cb);
}
#endif

/****************************************************************************
 * Name: cmd_urldecode
 ****************************************************************************/

#ifdef HAVE_CODECS_URLDECODE
int cmd_urldecode(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return cmd_codecs_proc(vtbl, argc, argv, CODEC_MODE_URLDECODE, urldecode_cb);
}
#endif

/****************************************************************************
 * Name: cmd_base64encode
 ****************************************************************************/

#ifdef HAVE_CODECS_BASE64ENC
int cmd_base64encode(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return cmd_codecs_proc(vtbl, argc, argv, CODEC_MODE_BASE64ENC, b64enc_cb);
}
#endif

/****************************************************************************
 * Name: cmd_base64decode
 ****************************************************************************/

#ifdef HAVE_CODECS_BASE64DEC
int cmd_base64decode(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return cmd_codecs_proc(vtbl, argc, argv, CODEC_MODE_BASE64DEC, b64dec_cb);
}
#endif

/****************************************************************************
 * Name: cmd_md5
 ****************************************************************************/

#ifdef HAVE_CODECS_HASH_MD5
int cmd_md5(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return cmd_codecs_proc(vtbl, argc, argv, CODEC_MODE_HASH_MD5, md5_cb);
}
#endif

#endif							/* CONFIG_NETUTILS_CODECS */
