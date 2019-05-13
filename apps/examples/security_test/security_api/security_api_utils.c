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

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <security/security_api.h>

void PrintBuffer(const char *header, unsigned char* buffer, uint32_t len)
{
	register uint32_t i = 0;
	printf("%s : %d\n", header, len);
	for (i = 0; i < len; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf(" %02X", buffer[i]);
	}
	printf("\n");
}

void free_security_data(security_data *data)
{
	if (data->length) {
		free(data->data);
		data->length = 0;
		data->data = 0;
	}
}
