/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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
 * examples/avs_test/avs_test_main.cpp
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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

#include <tinyara/config.h>
#include <tinyara/init.h>
#include <apps/platform/cxxinitialize.h>

#include <fcntl.h>
#include <wifi_manager/wifi_manager.h>

#include <iostream>

#include <avs-device-sdk/avs-device-sdk.h>

#ifndef CONFIG_EXAMPLES_AVS_TEST_SSID
#define CONFIG_EXAMPLES_AVS_TEST_SSID "Unknown"
#endif
#ifndef CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE
#define CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE "Unknown"
#endif
#ifndef CONFIG_EXAMPLES_AVS_TEST_AUTHENTICATION
#define CONFIG_EXAMPLES_AVS_TEST_AUTHENTICATION 3
#endif
#ifndef CONFIG_EXAMPLES_AVS_TEST_CRYPTO
#define CONFIG_EXAMPLES_AVS_TEST_CRYPTO 3
#endif

#define SAY_OK_OR_ERROR(condition) if (ret == (condition)) { printf("Ok\n"); } else { printf("Error\n"); }

extern "C" {

static void wifi_sta_connected(wifi_manager_result_e result) {
	printf("%s\n", __FUNCTION__);
}

static void wifi_sta_disconnected() {
	printf("%s\n", __FUNCTION__);
}

static wifi_manager_cb_s wifi_callbacks = {
	wifi_sta_connected,
	wifi_sta_disconnected,
	NULL,
	NULL,
	NULL,
};

int avs_test_main(int argc, char **argv)
{
#ifdef CONFIG_EXAMPLES_AVS_TEST_CXXINITIALIZE
	up_cxxinitialize();
#endif	
	
	wifi_manager_result_e ret = WIFI_MANAGER_FAIL;
	wifi_manager_ap_config_s config;

	printf("wifi_manager_init()...\n");
	ret = wifi_manager_init(&wifi_callbacks);
	SAY_OK_OR_ERROR(WIFI_MANAGER_SUCCESS);

	config.ssid_length = strlen(CONFIG_EXAMPLES_AVS_TEST_SSID);
	config.passphrase_length = strlen(CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE);
	strncpy(config.ssid, CONFIG_EXAMPLES_AVS_TEST_SSID, config.ssid_length + 1);
	strncpy(config.passphrase, CONFIG_EXAMPLES_AVS_TEST_PASSPHRASE, config.passphrase_length + 1);
	config.ap_auth_type = (wifi_manager_ap_auth_type_e)CONFIG_EXAMPLES_AVS_TEST_AUTHENTICATION;
	config.ap_crypto_type = (wifi_manager_ap_crypto_type_e)CONFIG_EXAMPLES_AVS_TEST_CRYPTO;

	printf("AP config: %s(%d) %s(%d) %d, %d\n",
		config.ssid,
		config.ssid_length,
		config.passphrase,
		config.passphrase_length,
		config.ap_auth_type,
		config.ap_crypto_type
	);

	/* current wifi mode is station, then this try will succeed */
	printf("wifi_manager_connect_ap()...\n");
	ret = wifi_manager_connect_ap(&config);
	SAY_OK_OR_ERROR(WIFI_MANAGER_SUCCESS);

	printf("AVS Device SDK SampleApplication returned %d\n", avsSampleAppEntry());

	printf("wifi_manager_deinit()...\n");
	ret = wifi_manager_deinit();
	SAY_OK_OR_ERROR(WIFI_MANAGER_SUCCESS);
	
	return 0;
}

}
