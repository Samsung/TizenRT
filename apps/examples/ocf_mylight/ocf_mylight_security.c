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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "ocf_mylight.h"
#include <pinoxmcommon.h>

#define MEMFILE

#ifdef MEMFILE
#define MEMFILE_SIZE 20000
#define MEMFILE_FLUSH_MAX 200

static unsigned char *memfile;
static int mf_pos_read;
static int mf_pos_write;
static int mf_size;
static int mf_flush_cnt;
static int mf_peak_size;

static void flush_memfile(void)
{
	FILE *fp;

	mf_flush_cnt = 0;

	DBG("flush size = %d", mf_size);

	if (mf_size <= 0)
		return;

	fp = fopen(SVR_DB_PATH, "wb");
	if (!fp) {
		DBG("fopen failed");
		return;
	}

	fwrite(memfile, 1, mf_size, fp);

	fclose(fp);
}

static void load_memfile(void)
{
	FILE *fp;

	fp = fopen(SVR_DB_PATH, "rb");
	if (!fp) {
		DBG("fopen failed");
		return;
	}

	fseek(fp, 0, SEEK_END);
	mf_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	DBG("file size = %d", mf_size);

	fread(memfile, 1, mf_size, fp);

	fclose(fp);
}
#endif

static FILE *on_open(const char *path, const char *mode)
{
	FILE *fp;

	if (!path)
		return NULL;

	if (!strcmp(path, "oic_svr_db.dat")) {
#ifdef MEMFILE
		fp = (FILE *)memfile;
		mf_pos_read = 0;
		mf_pos_write = 0;

		if (mode[0] == 'w' && mode[1] == 'b') {
			memset(memfile, 0, MEMFILE_SIZE);
			mf_size = 0;
		}
#else
		fp = fopen(SVR_DB_PATH, mode);
#endif
	} else if (!strcmp(path, "device_properties.dat"))
		fp = fopen(DEVICE_DB_PATH, mode);
	else if (!strcmp(path, "introspection.dat"))
		fp = fopen(INTROSPECTION_DB_PATH, mode);
	else {
		DBG("unknown storage file '%s'", path);
		fp = fopen(path, mode);
	}

	if (!fp) {
		DBG("fopen() failed. path=%s, mode=%s", path, mode);
		return NULL;
	}

#ifdef TRACE_STORAGE_OPERATION
	DBG("open: path=%s, mode=%s", path, mode);
#endif

	return fp;
}

static size_t on_read(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	size_t nread;

#ifdef TRACE_STORAGE_OPERATION
	DBG("read: size=%zd, nmemb=%zd, fp=0x%p", size, nmemb, fp);
#endif

#ifdef MEMFILE
	if (fp == (FILE *)memfile) {
		nread = size * nmemb;
		if ((int)(mf_pos_read + nread) > mf_size)
			nread = mf_size - mf_pos_read;

		memcpy(ptr, memfile + mf_pos_read, nread);
		mf_pos_read += nread;
#ifdef TRACE_STORAGE_OPERATION
		DBG("read-m: result nread=%zd", nread);
#endif
	} else {
#endif
		nread = fread(ptr, size, nmemb, fp);
#ifdef TRACE_STORAGE_OPERATION
		DBG("read: result nread=%zd", nread);
#endif

#ifdef MEMFILE
	}
#endif

	return nread;
}

static size_t on_write(const void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	size_t nwrite;

#ifdef TRACE_STORAGE_OPERATION
	DBG("write: size=%zd, nmemb=%zd, fp=0x%p", size, nmemb, fp);
#endif

#ifdef MEMFILE
	if (fp == (FILE *)memfile) {
		nwrite = size * nmemb;
		memcpy(memfile + mf_pos_write, ptr, nwrite);
		mf_pos_write += nwrite;
		if (mf_pos_write > mf_size) {
			mf_size = mf_pos_write;
			if (mf_size > mf_peak_size)
				mf_peak_size = mf_size;
		}

		mf_flush_cnt++;

		if (mf_flush_cnt > MEMFILE_FLUSH_MAX)
			flush_memfile();

		DBG("write-m: result nwrite=%ld", nwrite);
	} else {
#endif

		nwrite = fwrite(ptr, size, nmemb, fp);
#ifdef TRACE_STORAGE_OPERATION
		DBG("write: result nwrite=%zd", nwrite);
#endif

#ifdef MEMFILE
	}
#endif

	return nwrite;
}

static int on_close(FILE *fp)
{
#ifdef TRACE_STORAGE_OPERATION
	DBG("close: fp=0x%p", fp);
#endif

#ifdef MEMFILE
	if (fp == (FILE *)memfile) {
		mf_pos_read = 0;
		mf_pos_write = 0;
		return 0;
	} else {
#endif
		return fclose(fp);
#ifdef MEMFILE
	}
#endif
}

static int on_unlink(const char *path)
{
#ifdef TRACE_STORAGE_OPERATION
	DBG("unlink: path=%s", path);
#endif

#ifdef MEMFILE
	if (!strcmp(path, "oic.svr_db.dat")) {
		memset(memfile, 0, MEMFILE_SIZE);
		mf_size = 0;
		mf_pos_read = 0;
		mf_pos_write = 0;
		return 0;
	} else {
#endif
		return unlink(path);
#ifdef MEMFILE
	}
#endif
}

static void on_display_pin(char *pin, size_t length, void *user_data _UNUSED_)
{
	MSG("PIN CODE: '%s' (pin length=%zd)", pin, length);
}

static void on_close_pin(void)
{
	MSG("PIN CLOSED");
}

OCPersistentStorage ps = {
	.open = on_open,
	.read = on_read,
	.write = on_write,
	.close = on_close,
	.unlink = on_unlink
};

int ocf_mylight_security_init(void)
{
	OCStackResult ret;

#ifdef MEMFILE
	memfile = calloc(1, MEMFILE_SIZE);
	mf_flush_cnt = 0;
	mf_peak_size = 0;
	load_memfile();
#endif

	OCRegisterPersistentStorageHandler(&ps);

	ret = SetDisplayPinWithContextCB(on_display_pin, NULL);
	if (ret != OC_STACK_OK) {
		DBG("SetDisplayPinWithContextCB failed! (ret=%d)", ret);
		return -1;
	}

	SetClosePinDisplayCB(on_close_pin);

	ret = SetRandomPinPolicy(4, NUM_PIN);
	if (ret != OC_STACK_OK) {
		DBG("SetRandomPinPolicy failed! (ret=%d)", ret);
		return -1;
	}

	return 0;
}

void ocf_mylight_security_free(void)
{
#ifdef MEMFILE
	DBG("peak memfile size = %d", mf_peak_size);
	flush_memfile();
	free(memfile);
	memfile = NULL;
#endif
}

