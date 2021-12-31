/****************************************************************************
 *
 * Copyright 2021 Samsung Electronics All Rights Reserved.
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
#include <tinyara/seclink.h>
#include "sltool.h"
#include "sltool_utils.h"

#define SLT_OUTPUT_SIZE 16384 // 16KB

int sltool_handle_remove_ss(sl_options_s *opt)
{
	printf("[sltool] idx %d  %s:%d\n", opt->idx, __FUNCTION__, __LINE__);
	int fres = 0;
	int res = 0;
	sl_ctx hnd;
	if ((res = sl_init(&hnd)) != SECLINK_OK) {
		printf("[sltool] error to sl_init %d\n", res);
		return -1;
	}
	if ((res = sl_delete_storage(hnd, opt->idx)) != SECLINK_OK) {
		printf("[sltool] error delete message reason(%s) %s:%d\n", sl_strerror(res),
			   __FUNCTION__, __LINE__);
		fres = -1;
	}
	if ((res = sl_deinit(hnd)) != SECLINK_OK) {
		printf("[sltool] error %d %s:%d\n", res, __FUNCTION__, __LINE__);
	}
	return fres;
}

int sltool_handle_write_ss(sl_options_s *opt)
{
	printf("[sltool] idx %d  data %02x %ld size %d %s:%d\n",
		   opt->idx, opt->ss_data, sizeof(opt->ss_data),
		   opt->ss_write_size, __FUNCTION__, __LINE__);
	int fres = 0;
	int res = 0;
	sl_ctx hnd;
	char *data = NULL;
	hal_data input;

	if ((res = sl_init(&hnd)) != SECLINK_OK) {
		printf("[sltool] error to sl_init %d\n", res);
		return -1;
	}

	input.priv = NULL;
	input.priv_len = 0;
	input.data_len = opt->ss_write_size;
	data = (char *)malloc(input.data_len);
	if (!data) {
		fres = -1;
		goto out;
	}
	for (int i = 0; i < input.data_len; i++) {
		data[i] = opt->ss_data;
	}
	input.data = data;
	if ((res = sl_write_storage(hnd, opt->idx, &input)) != SECLINK_OK) {
		printf("[sltool] error to write data %s %s:%d\n", sl_strerror(res),
			   __FUNCTION__, __LINE__);
		fres = -1;
		goto out;
	}
out:
	if ((res = sl_deinit(hnd)) != SECLINK_OK) {
		printf("[sltool] error %d %s:%d\n", res, __FUNCTION__, __LINE__);
	}
	if (data) {
		free(data);
	}
	return fres;
}

int sltool_handle_get_ss(sl_options_s *opt)
{
	printf("[sltool] idx %d  %s:%d\n", opt->idx, __FUNCTION__, __LINE__);
	int fres = 0;
	int res = 0;
	sl_ctx hnd;
	char *data = NULL;
	hal_data output;

	if ((res = sl_init(&hnd)) != SECLINK_OK) {
		printf("[sltool] error to sl_init %d\n", res);
		return -1;
	}

	output.priv = NULL;
	output.priv_len = 0;
	output.data_len = SLT_OUTPUT_SIZE;
	data = (char *)malloc(output.data_len);
	if (!data) {
		fres = -1;
		goto out;
	}
	output.data = data;
	/*  If output size is less thatn stored size in a secure storage then 
	 * it would occur unexpected behavior */
	if ((res = sl_read_storage(hnd, opt->idx, &output)) != SECLINK_OK) {
		printf("[sltool] error to write data %s %s:%d\n",
			   sl_strerror(res),
			   __FUNCTION__, __LINE__);
		fres = -1;
		goto out;
	}
	sltool_print_buffer(output.data, output.data_len, "Secure Storage Output Data");

out:
	if ((res = sl_deinit(hnd)) != SECLINK_OK) {
		printf("[sltool] error %d %s:%d\n", res, __FUNCTION__, __LINE__);
	}
	if (data) {
		free(data);
	}
	return fres;
}
