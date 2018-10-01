/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
#include <net/lwip/netif/slipif.h>
#include <net/lwip/opt.h>

#include <tinyara/config.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/fs/fs.h>
#include <tinyara/net/net.h>

#include <tinyara/config.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <errno.h>
#include <assert.h>

#include <sys/socket.h>

#if LWIP_HAVE_SLIPIF

#include <net/lwip/def.h>
#include <net/lwip/pbuf.h>
#include <net/lwip/stats.h>
#include <net/lwip/snmp.h>
#include <net/lwip/sio.h>
#include <net/lwip/sys.h>

volatile static int i;
typedef void *sio_fd_t;
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */

sio_fd_t sio_open(u8_t devnum)
{
	int fd;

	fd = open("/dev/ttyDBG", O_RDWR, 0666);
	if (fd < 3) {
		fs_dupfd2(fd, 3);
		close(fd);
		return fs_getfilep(3);
	}

	return fs_getfilep(fd);
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
	file_write((struct file *)fd, &c, 1);
}

/**
 * Receives a single character from the serial device.
 *
 * @param fd serial device handle
 *
 * @note This function will block until a character is received.
 */
u8_t sio_recv(sio_fd_t fd)
{
	char c = 0;
	i = 1;
	while (i) ;

	if (file_read((struct file *)fd, &c, 1) <= 0) {
		while (1) ;				//ERROR!!!
	}

	return (c);
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
	return file_read((struct file *)fd, data, len);
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
	i = 1;
	while (i) ;
	return 0;
}

/**
 * Writes to the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data to send
 * @param len length (in bytes) of data to send
 * @return number of bytes actually sent
 *
 * @note This function will block until all data can be sent.
 */
u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len)
{
	i = 1;
	while (i) ;

	file_write((struct file *)fd, data, len);
	return 0;
}

/**
 * Aborts a blocking sio_read() call.
 *
 * @param fd serial device handle
 */
void sio_read_abort(sio_fd_t fd)
{
	i = 1;
	while (i) ;
}

#endif
