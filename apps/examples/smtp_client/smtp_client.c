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
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <net/if.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/
int smtpc_main(int argc, char *argv[])
{
	if (argc != 7) {
		printf("Usage:\n");
		printf("   %s [xx.xx.xx.xx] [yy.yy.yy.yy] [To:] [From:] [Subject] [Message]\n", argv[0]);
		printf("  xx.xx.xx.xx is the IP address of the SMTP server\n");
		printf("  yy.yy.yy.yy is the IP address of the SMTP client(localhost)\n");
		return 0;
	}

	int msglen;
	int result_smtp;

	printf("SMTP Client Start!!\n\n------------------------\n\n");
	msglen = strlen(argv[6]);

	//test smtp_open
	void *handle = NULL;
	handle = (void *)smtp_open();
	if (handle != NULL) {
		printf("1. smtp_open : Successed\n");
	} else {
		printf("1. smtp_open : failed\n");
	}

	//test smtp_configure
	struct in_addr addr;
	inet_pton(AF_INET, argv[1], &(addr.s_addr));
	smtp_configure(handle, argv[2], addr.s_addr);
	printf("2. smtp_configure : Successed\n");

	result_smtp = smtp_send(handle, argv[3], 0, argv[4], argv[5], argv[6], msglen);
	if (result_smtp == OK) {
		printf("3. smtp_send : Successed\n");
	} else {
		printf("3. smtp_send : failed\n");
	}

	smtp_close(handle);
	printf("------------------------\n\nstmp client test finish!!\n");
	return 0;
}
