/*******************************************************************
*
* Copyright 2020 Samsung Electronics All Rights Reserved.
*
* File Name : trap.h
* Description: Receive FS and Ram Dump using UART
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
******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define RAMDUMP_HANDSHAKE_STRING        "TIZENRTRMDUMP"
#define FSDUMP_HANDSHAKE_STRING         "TIZENRTFSDUMP"	//Dump the entire userfs flash partition
#define EXTFSDUMP_HANDSHAKE_STRING      "TIZENRTEXTFSD"	//Dump the entire userfs partition from external flash
#define TARGET_REBOOT_STRING            "TIZENRTREBOOT"
#define USERFS_FDUMP_HANDSHAKE_STRING   "TIZENRTFILDUM"	//Dump a particular file from Userfs
#define HANDSHAKE_STR_LEN_MAX           (13)
#define BINFILE_NAME_SIZE               (40)
#define KB_CHECK_COUNT                  (16 * 1024)

#define TTYPATH_LEN             25
#define TTYTYPE_LEN             7

#define CLEAR_DUMP_FLAGS        0
#define RAMDUMP_FLAG            1
#define USERFSDUMP_FLAG         2
#define REBOOT_DEVICE_FLAG      4
#define EXTUSERFS_DUMP_FLAG     8
#define FILE_DUMP_FLAG		16

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

/* Ramdump initialization data */
uint32_t  number_of_regions;
typedef struct {
        int rd_regionx_idx;
        uint32_t rd_regionx_start;
        uint32_t rd_regionx_size;
        int rd_regionx_mark;
} rd_regionx;

/****************************************************************************
 * Internal function prototypes
 ****************************************************************************/

int do_handshake(int dev_fd, char *handshake_string);

int b_read(int fd, uint8_t *buf, int size);

int configure_tty(int tty_fd);

int send_region_info(int dev_fd, char *host_region);

int get_dump(int dev_fd, FILE* fp, uint32_t size);
