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

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include <tinyara/config.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "netcmd.h"
#include <protocols/tftp.h>

#if defined(CONFIG_NETUTILS_TFTPC)
struct tftpc_args_s {
	bool binary;				/* true:binary ("octet") false:text ("netascii") */
	bool allocated;				/* true: destpath is allocated */
	char *destpath;				/* Path at destination */
	const char *srcpath;			/* Path at src */
	in_addr_t ipaddr;			/* Host IP address */
};

// example : tftpc tftptest.txt /mnt/tftptest.txt 192.168.1.216
// example : tftpc tftptest.bin /mnt/tftptest.bin 192.168.1.216 b

int cmd_tftpc(int argc, char **argv)
{
	if (argc < 4) {
		printf("Usage:\n");
		printf("   %s [remote] [local] [addr] [binary]\n", argv[0]);
		printf("Where\n");
		printf("*************************************************************************\n");
		printf(" * Name: tftpc\n");
		printf(" *\n");
		printf(" * Input Parameters:\n");
		printf(" *	 remote - The name of the file on the TFTP server.\n");
		printf(" *	 local	- Path to the location on a mounted filesystem\n");
		printf(" *		  where the file will be stored.\n");
		printf(" *	 addr	- The IP address of the server in network order\n");
		printf(" *	 binary - b:  Perform binary ('octect') transfer\n");
		printf(" *		  If there is no input, Perform text ('netascii')\n");
		printf(" *		  transfer\n");
		printf("*************************************************************************\n");
		return 0;
	}

	in_addr_t ipaddr;
	ipaddr = inet_addr(argv[3]);

	bool isBinary = false;
	if (*argv[4] == 'b' || *argv[4] == 'B') {
		isBinary = true;
	}

	if (tftpget(argv[1], argv[2], ipaddr, isBinary) != OK) {
		printf(fmtcmdfailed, argv[0], "cmd_tftpc", "tftpget");
	}

	return OK;
}

#endif
