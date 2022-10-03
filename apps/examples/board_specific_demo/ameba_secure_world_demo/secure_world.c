/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>

#define SHOW_USAGE printf("Usage: secure_world [opt1] opt2\n  \
		opt2: One of the option is mandatory\n \
		-r: run the read test\n \
		-w: run the write test\n \
		opt1:\n \
	        -a secure_world_address: take the address as input, default: 0x735C021\n \
		Valid Range: 0x7034C020 - 0x7039C000\n ");

#ifdef  CONFIG_AMEBALITE_TRUSTZONE
volatile uint32_t  *address = CONFIG_AMEBALITE_SECURE_WORLD_ADDRESS;
#endif

/****************************************************************************
 * secure_world_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int secure_world_main(int argc, char *argv[])
#endif
{
	int option;
	volatile int value;
	bool r_flag = false;
	bool w_flag = false;
	while ((option = getopt(argc, argv, "a:rw")) != -1) {
		switch (option) {
		case 'a':
			sscanf(optarg, "%x", &address);
			break;
		case 'r':
			r_flag = true;
			break;
		case 'w':
			w_flag = true;
			break;
		default:
			printf("INVALID INPUT\n");
			SHOW_USAGE
			return 0;
		}
	}
	if (!r_flag && !w_flag) {
		SHOW_USAGE
	} else if (r_flag) {
                printf("Trying to read data from %x\n", address);
                value = *address;
        } else {
                printf("Trying to write data at %x\n", address);
                *address = 1234;
        }
	return 0;
}
