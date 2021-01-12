/*******************************************************************
*
* Copyright 2020 Samsung Electronics All Rights Reserved.
*
* File Name : dump_tool_utils.c
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

#include <termios.h>
#include "dump_tool.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int do_handshake(int dev_fd, char *handshake_string)
{
        int ret;
        char ack;
        char host_handshake[HANDSHAKE_STR_LEN_MAX + 1] = { '\0' };

        /* Prepare the handshake string */
        strncpy(host_handshake, handshake_string, HANDSHAKE_STR_LEN_MAX);

        /* Send handshake command to TARGET */
        ret = write(dev_fd, host_handshake, HANDSHAKE_STR_LEN_MAX);
        if (ret != HANDSHAKE_STR_LEN_MAX) {
                printf("Sending handshake failed, ret = %d\n", ret);
                return -1;
        }

        /* Read ACK from TARGET */
        ret = read(dev_fd, &ack, sizeof(ack));
        if (ret != 1) {
                printf("Receiving handshake failed, ret = %d\n", ret);
                return -1;
        }

        /* Check ACK */
        if (ack != 'A') {
                printf("Wrong Target Handshake, ack = %c\n", ack);
                return -1;
        }

        printf("%s: Target Handshake successful\n", __func__);

        return 0;
}

int b_read(int fd, uint8_t *buf, int size)
{
        int i;
        int ret;

        for (i = 0; i < size; i++) {
                ret = read(fd, buf + i, 1);
                if (ret != 1) {
                        return i;
                }
        }

        return i;
}

int configure_tty(int tty_fd)
{
        struct termios ttyio;
        memset(&ttyio, 0, sizeof(ttyio));

        fcntl(tty_fd, F_SETFL, 0);
        tcgetattr(tty_fd, &ttyio);

        /* set baudrate */
        cfsetispeed(&ttyio, B115200);
        cfsetospeed(&ttyio, B115200);
        ttyio.c_cflag |= (CLOCAL | CREAD);

        cfmakeraw(&ttyio);

        ttyio.c_cflag &= ~CSTOPB;
        ttyio.c_cflag &= ~(PARENB | PARODD);
        ttyio.c_cflag &= ~CSIZE;
        ttyio.c_cflag &= ~CRTSCTS;
        ttyio.c_cflag |= CS8;

        ttyio.c_iflag &= ~ICRNL;
        ttyio.c_iflag &= ~INLCR;
        ttyio.c_oflag &= ~OCRNL;
        ttyio.c_oflag &= ~ONLCR;
        ttyio.c_lflag &= ~ICANON;
        ttyio.c_lflag &= ~ECHO;

        ttyio.c_cc[VMIN] = 1;
        ttyio.c_cc[VTIME] = 5;

        tcflush(tty_fd, TCIOFLUSH);

        tcsetattr(tty_fd, TCSANOW, &ttyio);
        return 0;
}

int send_region_info(int dev_fd, char *host_region)
{
        int ret;

        /* Send region info to TARGET */
        ret = write(dev_fd, host_region, 1);
        if (ret != 1) {
                printf("Sending region info failed, ret = %d\n", ret);
                return -1;
        }

        return 0;
}

int get_dump(int dev_fd, FILE* fp, uint32_t size)
{
        int ret;
        int count = 0;
        char buf;

        printf("[>");
        fflush(stdout);

        while (size) {
                ret = read(dev_fd, &buf, 1);
                if (ret != 1) {
                        printf("Receiving ramdump %dTH byte failed, ret = %d\n", count, ret);
                        return -1;
                }

                ret = fwrite(&buf, 1, 1, fp);
                if (ret != 1) {
                        printf("Writing ramdump %dTH byte failed, ret = %d\n", count, ret);
                        return -1;
                }

                count++;
                size--;

                if ((count % (KB_CHECK_COUNT)) == 0) {
                        printf("\b=>");
                        fflush(stdout);
                }
        }
        printf("]\n");
        fflush(stdout);

	return 0;
}
