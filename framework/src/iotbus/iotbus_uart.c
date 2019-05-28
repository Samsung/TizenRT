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

/**
 * @file iotbus_uart.c
 * @brief Iotbus APIs for UART
 */

#include <tinyara/config.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_uart.h>

#include "iotapi_dev_handler.h"
#include "iotbus_internal.h"

#ifndef CONFIG_IOTBUS_UART_EVENT_SIZE
#define CONFIG_IOTBUS_UART_EVENT_SIZE 3
#endif

#ifndef CONFIG_IOTBUS_UART_BUF_SIZE
#define CONFIG_IOTBUS_UART_BUF_SIZE 64
#endif

struct _iotbus_uart_s {
	int fd;
	iotapi_hnd evt_hnd[CONFIG_IOTBUS_UART_EVENT_SIZE];
	uart_write_cb callback;
	int timeout;
	uint8_t buf[CONFIG_IOTBUS_UART_BUF_SIZE];
	size_t len;
	iotbus_uart_state_e rx_state;
	iotbus_uart_state_e tx_state;
};

struct _iotbus_uart_wrapper_s {
	struct _iotbus_uart_s *handle;
};

int g_iotbus_uart_br[30] = {
B50, B75, B110, B134, B150,
B200, B300, B600, B1200, B1800,
B2400, B4800, B9600, B19200, B38400,
B57600, B115200, B128000, B230400, B256000,
B460800, B500000, B576000, B921600, B1000000,
B1152000, B1500000, B2000000, B2500000, B3000000, };
int g_iotbus_uart_size = 30;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Private Functions
 */
static void *iotbus_uart_out_handler(void *hnd)
{
	struct _iotbus_uart_s *handle;
	int ret;
	ssize_t nbytes;

	struct pollfd fds[1];

	handle = (struct _iotbus_uart_s *)((struct _iotbus_uart_wrapper_s *)hnd)->handle;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = handle->fd;
	fds[0].events = POLLOUT | POLLERR;

	handle->tx_state = IOTBUS_UART_BUSY;
	while (1) {
		ret = poll(fds, 1, handle->timeout);
		if (ret < 0) {
			continue;
		} else if (ret == 0) {
			ret = IOTBUS_ERROR_TIMED_OUT;
			break;
		}

		if (fds[0].revents & POLLOUT) {
			nbytes = write(handle->fd, handle->buf, handle->len);

			/* Handle unexpected return values */
			if (nbytes < 0) {
				idbg("[UART] Fail to write...\n");
				ret = IOTBUS_ERROR_UNKNOWN;
				break;
			} else if (nbytes == 0) {
				idbg("[UART] No data write, Ignoring\n");
			} else {
				ret = IOTBUS_ERROR_NONE;
				break;
			}
		}
	}
	handle->tx_state = IOTBUS_UART_RDY;
	if (handle->callback) {
		handle->callback((struct _iotbus_uart_wrapper_s *)hnd, ret);
	}
	idbg("[UART] exit iotbus_uart handler\n");

	return 0;
}

#ifdef CONFIG_SERIAL_TERMIOS
static int _iotbus_valid_baudrate(unsigned int rate)
{
	int i = 0;
	for (; i < g_iotbus_uart_size; i++)
		if (rate == g_iotbus_uart_br[i])
			return 1;

	return 0;
}
#endif

/*
 * Public Functions
 */
iotbus_uart_context_h iotbus_uart_init(const char *path)
{
	int fd;
	struct _iotbus_uart_s *handle;
	iotbus_uart_context_h dev;
	int i;

	fd = open(path, O_RDWR, 0666);
	if (fd < 0) {
		return NULL;
	}

	handle = (struct _iotbus_uart_s *)malloc(sizeof(struct _iotbus_uart_s));
	if (!handle) {
		goto errout_with_close;
	}

	dev = (struct _iotbus_uart_wrapper_s *)malloc(sizeof(struct _iotbus_uart_wrapper_s));
	if (!dev) {
		free(handle);
		goto errout_with_close;
	}

	handle->fd = fd;
	for (i = 0; i < CONFIG_IOTBUS_UART_EVENT_SIZE; i++) {
		handle->evt_hnd[i] = NULL;
	}
	handle->callback = NULL;
	dev->handle = handle;

	return dev;

errout_with_close:
	close(fd);
	return NULL;
}

iotbus_uart_context_h iotbus_uart_open(int device)
{
	iotbus_uart_context_h dev;
	char dev_path[16];

	if (device < 0) {
		return NULL;
	}

	snprintf(dev_path, sizeof(dev_path), "/dev/ttyS%d", device);
	dev = iotbus_uart_init(dev_path);

	return dev;
}

int iotbus_uart_stop(iotbus_uart_context_h hnd)
{
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	close(handle->fd);
	free(handle);
	hnd->handle = NULL;
	free(hnd);

	return IOTBUS_ERROR_NONE;
}

#ifdef CONFIG_SERIAL_TERMIOS
int iotbus_uart_flush(iotbus_uart_context_h hnd)
{
	// tcdrain is not working even CONFIG_SERIAL_TERMIOS is defined.
	int fd;
	int ret;
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;
	fd = handle->fd;

	ret = tcflush(fd, TCIOFLUSH);
	if (ret < 0) {
		return IOTBUS_ERROR_NOT_SUPPORTED;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_uart_set_baudrate(iotbus_uart_context_h hnd, unsigned int baud)
{
	int fd;
	int ret;
	struct termios tio;
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!_iotbus_valid_baudrate(baud)) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;
	fd = handle->fd;

	ret = tcgetattr(fd, &tio);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}
	tio.c_speed = baud;

	ret = tcsetattr(fd, TCSANOW, &tio);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

// 8N1 would be set by calling "iotbus_uart_set_mode(hnd, 8,MRAA_UART_PARITY_NONE , 1)"
int iotbus_uart_set_mode(iotbus_uart_context_h hnd, int bytesize, iotbus_uart_parity_e parity, int stopbits)
{
	int fd;
	int ret;
	struct termios tio;
	struct _iotbus_uart_s *handle;
	int byteinfo[4] = { CS5, CS6, CS7, CS8 };

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;
	fd = handle->fd;

	ret = tcgetattr(fd, &tio);
	if (ret) {
		return IOTBUS_ERROR_UNKNOWN;
	}
	// set byte size
	if (bytesize < 5 || bytesize > 8) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	tio.c_cflag &= ~CSIZE;
	tio.c_cflag |= byteinfo[bytesize - 5];

	// set parity info
	switch (parity) {
	case IOTBUS_UART_PARITY_EVEN:
		tio.c_cflag |= PARENB;
		tio.c_cflag &= ~PARODD;
		break;
	case IOTBUS_UART_PARITY_ODD:
		tio.c_cflag |= PARENB;
		tio.c_cflag |= PARODD;
		break;
	case IOTBUS_UART_PARITY_NONE:
		tio.c_cflag &= ~PARENB;
		tio.c_cflag &= ~PARODD;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	// set stop bit
	switch (stopbits) {
	case 1:
		tio.c_cflag &= ~CSTOPB;
		break;
	case 2:
		tio.c_cflag |= CSTOPB;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = tcsetattr(fd, TCSANOW, &tio);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

// parameter
// rtscts => 1: rts/cts on, 0: off
int iotbus_uart_set_flowcontrol(iotbus_uart_context_h hnd, int xonxoff, int rtscts)
{
	int fd;
	int ret;
	struct termios tio;
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (xonxoff != 1 && xonxoff != 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (rtscts != 1 && rtscts != 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	fd = handle->fd;

	ret = tcgetattr(fd, &tio);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	if (rtscts) {
		tio.c_cflag |= CRTS_IFLOW | CCTS_OFLOW;
	} else {
		tio.c_cflag &= ~(CRTS_IFLOW | CCTS_OFLOW);
	}

	if (xonxoff) {
		tio.c_iflag |= (IXON | IXOFF | IXANY);
	}

	ret = tcsetattr(fd, TCSANOW, &tio);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}
#endif

int iotbus_uart_read(iotbus_uart_context_h hnd, char *buf, unsigned int length)
{
	int fd;
	int ret;
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!buf || length <= 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	fd = handle->fd;
	ret = read(fd, buf, length);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}

	return ret;
}

int iotbus_uart_read_wait(iotbus_uart_context_h hnd, char *buf, unsigned int length, int timeout)
{
	struct _iotbus_uart_s *handle;
	int ret;
	ssize_t nbytes;

	struct pollfd fds[1];

	if (!hnd || !hnd->handle || timeout < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	if (handle->rx_state == IOTBUS_UART_BUSY) {
		return IOTBUS_ERROR_DEVICE_NOT_READY;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = handle->fd;
	fds[0].events = POLLIN | POLLERR;

	handle->rx_state = IOTBUS_UART_BUSY;

	ssize_t received = 0;
	while (1) {
		ret = poll(fds, 1, timeout);
		if (ret < 0) {
			continue;
		} else if (ret == 0) {
			idbg("[UART] POLL timeout[%d]\n", received);
			if (received == 0) {
				ret = IOTBUS_ERROR_TIMED_OUT;
			} else {
				ret = received;
			}
			break;
		}


		if (fds[0].revents & POLLIN) {
			nbytes = read(handle->fd, buf + received, length);

			/* Handle unexpected return values */
			if (nbytes < 0) {
				idbg("[UART] Fail to read...\n");
				ret = IOTBUS_ERROR_UNKNOWN;
				break;
			} else if (nbytes == 0) {
				idbg("[UART] No data read, Ignoring\n");
			} else {
				received += nbytes;
				length -= nbytes;
				if (length <= 0) {
					idbg("[UART] RX buffer is full.\n");
					ret = received;
					break;
				}
			}
		}
	}
	handle->rx_state = IOTBUS_UART_RDY;
	idbg("[UART] exit iotbus_uart_read_wait \n");

	return ret;
}

int iotbus_uart_write(iotbus_uart_context_h hnd, const char *buf, unsigned int length)
{
	int fd;
	int ret;
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	if (!buf || length <= 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	fd = handle->fd;
	ret = write(fd, buf, length);
	if (ret < 0) {
		return IOTBUS_ERROR_UNKNOWN;
	}
	return ret;
}

int iotbus_uart_async_write(iotbus_uart_context_h hnd, const char *buf, unsigned int length, uart_write_cb cb, int timeout)
{
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle || !cb) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	if (handle->tx_state == IOTBUS_UART_BUSY) {
		return IOTBUS_ERROR_DEVICE_NOT_READY;
	}

	handle->callback = cb;
	handle->timeout = timeout;
	memcpy(handle->buf, buf, length);
	handle->len = length;

	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, iotbus_uart_out_handler, (void *)hnd);
	if (ret < 0) {
		idbg("[UART] create iotapi handler fail(%d)\n", ret);
		return IOTBUS_ERROR_UNKNOWN;
	}
	pthread_detach(tid);

	return IOTBUS_ERROR_NONE;
}

int iotbus_uart_set_int(iotbus_uart_context_h hnd, iotbus_int_type_e int_type, bool enable, uart_isr_cb cb)
{
	struct _iotbus_uart_s *handle;
	int i;

	if (!hnd || !hnd->handle || !cb) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	if (enable) {
		// To check already registered int_type;
		for (i = 0; i < CONFIG_IOTBUS_UART_EVENT_SIZE; i++) {
			if (!handle->evt_hnd[i]) {
				iotapi_dev_init(&handle->evt_hnd[i]);
				iotapi_dev_register(handle->evt_hnd[i], int_type, cb);
				break;
			}
		}
		if (i >= CONFIG_IOTBUS_UART_EVENT_SIZE) {
			return IOTBUS_ERROR_QUEUE_FULL;
		}
	} else {
		// Find int_type
		for (i = 0; i < CONFIG_IOTBUS_UART_EVENT_SIZE; i++) {
			if (!handle->evt_hnd[i]) {
				if (iotapi_dev_get_int_type(handle->evt_hnd[i]) == int_type) {
					iotapi_dev_unregister(handle->evt_hnd[i]);
					handle->evt_hnd[i] = NULL;
				}
			}
		}
	}

	return IOTBUS_ERROR_NONE;
}

#ifdef __cplusplus
}
#endif
