/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/boardctl.h>
#include <errno.h>
#include <stdio.h>

#include <arch/board/boardctl.h>

int virtio_blk_test_main(int argc, char *argv[])
{
	int ret;

	(void)argc;
	(void)argv;

	ret = boardctl(BIOC_QEMU_VIRTIO_BLK_TEST, 0);
	if (ret < 0) {
		printf("VIRTIO_BLK_TEST: FAIL (boardctl: %d)\n", errno);
		return ERROR;
	}

	return OK;
}
