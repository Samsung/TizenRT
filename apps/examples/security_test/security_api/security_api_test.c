/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * either express or implied. SEC the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <pthread.h>

#include <security/security_api.h>
/****************************************************************************
 * security_test_main
 ****************************************************************************/

extern void test_authenticate(void);
extern void test_crypto(void);
extern void test_keymgr(void);
extern void test_securestorage(void);



#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int security_api_test_main(int argc, char *argv[])
#endif
{
	security_handle hnd;
	int res = security_init(&hnd);
	if (res != 0) {
		printf("init fail\n");
		return 0;
	}

	res = security_deinit(hnd);
	if (res != 0) {
		printf("deinit fail\n");
		return 0;
	}

	test_authenticate();
	test_crypto();
	test_keymgr();
	test_securestorage();

	return 0;
}
