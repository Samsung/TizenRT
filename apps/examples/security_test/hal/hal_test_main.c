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
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <pthread.h>

#include "hal_test.h"

/****************************************************************************
 * security_test_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int security_hal_test_main(int argc, char *argv[])
#endif
{
	pthread_t tid;
	pthread_attr_t attr;
	int res = 0;

	res = pthread_attr_init(&attr);
	if (res != 0) {
		printf("Failed to pthread_attr_init\n");
		return -1;
	}

	res = pthread_attr_setstacksize(&attr, HAL_TEST_STACK_SIZE);
	if (res != 0) {
		printf("Failed to pthread_attr_setstacksize\n");
		return -1;
	}

	res = pthread_create(&tid, &attr, (pthread_startroutine_t)hal_keymgr_test, NULL);
	if (res < 0) {
		printf("hal key manager test fail\n");
		return -1;
	}
	res = pthread_join(tid, NULL);
	if (res < 0) {
		printf("hal key manager test fail\n");
		return -1;
	}
	res = pthread_create(&tid, &attr, (pthread_startroutine_t)hal_auth_test, NULL);
	if (res < 0) {
		printf("hal authenticate test fail\n");
		return -1;
	}
	res = pthread_join(tid, NULL);
	if (res < 0) {
		printf("hal authenticate test fail\n");
		return -1;
	}
	res = pthread_create(&tid, &attr, (pthread_startroutine_t)hal_ss_test, NULL);
	if (res < 0) {
		printf("hal secure storage test fail\n");
		return -1;
	}
	res = pthread_join(tid, NULL);
	if (res < 0) {
		printf("hal secure sotrage test fail\n");
		return -1;
	}
	res = pthread_create(&tid, &attr, (pthread_startroutine_t)hal_crypto_test, NULL);
	if (res < 0) {
		printf("hal crypto test fail\n");
		return -1;
	}
	res = pthread_join(tid, NULL);
	if (res < 0) {
		printf("hal crypto test fail\n");
		return -1;
	}

	return 0;
}
