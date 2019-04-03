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

/****************************************************************************
 * security_test_main
 ****************************************************************************/

extern int hal_keymgr_test(void);
extern int hal_auth_test(void);
extern int hal_ss_test(void);
extern int hal_crypto_test(void);


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int security_test_main(int argc, char *argv[])
#endif
{
	int res = 0;

	res = hal_keymgr_test();
	if (res < 0) {
		printf("hal key manager test fail\n");
	}
	res = hal_auth_test();
	if (res < 0) {
		printf("hal authenticate test fail\n");
	}
	res = hal_ss_test();
	if (res < 0) {
		printf("hal secure storage test fail\n");
	}
	res = hal_crypto_test();
	if (res < 0) {
		printf("hal crypto test fail\n");
	}

	return 0;
}
