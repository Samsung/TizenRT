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

#include <stdio.h>
#include <time.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "simple.pb.h"

#define TEST_N (100000)

int nanopb_simple_main(int argc, char *argv[])
{
	uint8_t buffer[128];
	size_t message_length;
	bool status;

	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	for (int i = 0; i < TEST_N; i++) {

		/* Encode */
		{
			SimpleMessage message = SimpleMessage_init_zero;
			pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
			message.lucky_number = i;
			status = pb_encode(&stream, SimpleMessage_fields, &message);
			message_length = stream.bytes_written;
			if (!status) {
				printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
				return 1;
			}
		}

		/* Decode */
		{
			SimpleMessage message = SimpleMessage_init_zero;
			pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
			status = pb_decode(&stream, SimpleMessage_fields, &message);
			if (!status) {
				printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
				return 1;
			}

			if ((int)message.lucky_number != i) {
				printf("Decode failure! lucky_number[%d] should be %d", (int)message.lucky_number, i);
			}
		}
	}

	clock_gettime(CLOCK_REALTIME, &end);

	printf("%d times encode/decode takes %lld(ms)\n", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec)) / 1000000);
	return 0;
}
