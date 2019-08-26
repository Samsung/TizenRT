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
#include <tinyara/serial/serial.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_uart.h>

struct _iotbus_uart_s {
	int fd;
	int device;
	uart_write_cb callback;
	int timeout;
	uint8_t *buf;
	size_t len;
	iotbus_uart_state_e rx_state;
	iotbus_uart_state_e tx_state;
	iotbus_uart_cb cb[IOTBUS_UART_INTR_MAX];
	pid_t pid[IOTBUS_UART_INTR_MAX];
#ifdef CONFIG_IOTBUS_INTERRUPT_TIMESTAMP	
	uint32_t timestamp;
#endif
};

struct _iotbus_uart_wrapper_s {
	struct _iotbus_uart_s *handle;
};

static int g_iotbus_uart_br[30] = {
B50, B75, B110, B134, B150,
B200, B300, B600, B1200, B1800,
B2400, B4800, B9600, B19200, B38400,
B57600, B115200, B128000, B230400, B256000,
B460800, B500000, B576000, B921600, B1000000,
B1152000, B1500000, B2000000, B2500000, B3000000, };
static int g_iotbus_uart_size = 30;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Private Functions
 */

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

static int _iotbus_get_dev_number(const char *path)
{
	
	const char *ptr = path;
	int len = strlen(path);
	char p[8] = { 0, };
	int cnt = 0;
	
	while (*ptr != 'S') {
		if (*ptr == 0) return -1;
		if (ptr - path >= len) return -1;
		ptr++;
	}

	for (ptr = ptr + 1; ptr < path + len; ptr++) {
		p[cnt++] = *ptr;
	}

	return atoi(p);
}

/*
 * Public Functions
 */
iotbus_uart_context_h iotbus_uart_init(const char *path)
{
	int fd;
	int i;
	int ret;
	struct _iotbus_uart_s *handle;
	iotbus_uart_context_h dev;

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
	handle->device = _iotbus_get_dev_number(path);
	handle->callback = NULL;
	for (i = 0; i < IOTBUS_UART_INTR_MAX; i++) {
		handle->cb[i] = NULL;
		handle->pid[i] = 0;
	}
#ifdef CONFIG_IOTBUS_INTERRUPT_TIMESTAMP		
	handle->timestamp = 0;
#endif
	dev->handle = handle;

	ret = ioctl(fd, TIOCSETIOTBUS, (unsigned long)dev);
	if (ret != 0) {
		ibdbg("ioctl failed \n");
		free(handle);
		free(dev);
		goto errout_with_close;
	}

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
	int i;
	int ret;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	for (i = 0; i < IOTBUS_UART_INTR_MAX; i++) {
		if (handle->cb[i] != NULL) {
			ret = iotbus_uart_unset_interrupt(hnd, i);
			if (ret != IOTBUS_ERROR_NONE) {
				return ret;
			}
		}
	}

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

static int _uart_sig[IOTBUS_UART_INTR_MAX] = {
	SIG_IOTBUS_UART_TX_EMPTY,
	SIG_IOTBUS_UART_TX_RDY,
	SIG_IOTBUS_UART_RX_AVAIL,
	SIG_IOTBUS_UART_RECEIVED
};

static pthread_addr_t uart_intr_thread(pthread_addr_t arg)
{
	struct intr_args *param = (struct intr_args *)arg;
	iotbus_uart_context_h dev = (iotbus_uart_context_h)param->dev;
	iotbus_uart_intr_e int_type = (iotbus_uart_intr_e)param->int_type;

	if (int_type < 0 || int_type >= IOTBUS_UART_INTR_MAX) {
		ibdbg("Error : invalid int type\n");
		pthread_exit(NULL);
		return NULL;
	}

	ibvdbg("Thread for %p[%d]\n", dev, int_type);

	siginfo_t info;
	sigset_t sig_set;
	uint32_t val;
	
	struct _iotbus_uart_s *handle = (struct _iotbus_uart_s *)dev->handle;

	if (handle->cb[int_type] == NULL) {
		ibdbg("Error : Callback is not registered\n");
		return NULL;
	}

	sigemptyset(&sig_set);
	sigaddset(&sig_set, _uart_sig[int_type]);

	pthread_sigmask(SIG_BLOCK, &sig_set, NULL);

	while (1) {
		sigwaitinfo(&sig_set, &info);

#ifdef CONFIG_IOTBUS_INTERRUPT_TIMESTAMP		
#ifdef CONFIG_CAN_PASS_STRUCTS
		val = info.si_value.sival_int;
#else
		val = info.si_value;
#endif
		handle->timestamp = val;
#endif
		// Call callback function.
		(handle->cb[int_type])(dev);
	}

	pthread_sigmask(SIG_UNBLOCK, &sig_set, NULL);
	pthread_exit(NULL);
	return NULL;
}

int iotbus_uart_set_interrupt(iotbus_uart_context_h hnd, iotbus_uart_intr_e int_type, iotbus_uart_cb cb, uint8_t priority)
{
	int ret = -1;
	pid_t pid;
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle || int_type < 0) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	handle->cb[int_type] = cb;
	if (handle->pid[int_type] > 0) {
		return IOTBUS_ERROR_NONE;
	}

	struct intr_attr val = { priority, IOTBUS_UART, getpid() };
	struct intr_args arg = { 
		.dev = (void *)hnd, 
		.int_type = (int)int_type 
	};

	pid = create_intr_pthread(uart_intr_thread, (void *)&arg, &val);
	if (pid < 0) {
		handle->cb[int_type] = NULL;
		handle->pid[int_type] = 0;
		return IOTBUS_ERROR_UNKNOWN;
	}
	handle->pid[int_type] = pid;

	struct uart_notify_s noti = { 0, };
	noti.pid = pid;

	switch (int_type) {
	case IOTBUS_UART_TX_EMPTY:
		noti.type = UART_TX_EMPTY;
		break;
	case IOTBUS_UART_TX_RDY:
		noti.type = UART_TX_RDY;
		break;
	case IOTBUS_UART_RX_AVAIL:
		noti.type = UART_RX_AVAIL;
		break;
	case IOTBUS_UART_RECEIVED:
		noti.type = UART_RECEIVED;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = ioctl(handle->fd, TIOCSETINTR, (unsigned long)&noti);
	if (ret != 0) {
		ibdbg("ioctl failed, errno : %d\n", get_errno());
		pid = handle->pid[int_type];
		handle->pid[int_type] = 0;
		handle->cb[int_type] = NULL;
		if (pthread_cancel(pid) < 0) {
			ibdbg("pthread cancel failed.\n");
			return IOTBUS_ERROR_UNKNOWN;
		}
		return ret;
	}

	return 0;
}

int iotbus_uart_unset_interrupt(iotbus_uart_context_h hnd, iotbus_uart_intr_e int_type)
{
	struct _iotbus_uart_s *handle;
	FAR struct uart_notify_s noti;
	int ret;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;

	// Disable Interrupt
	switch (int_type) {
	case IOTBUS_UART_TX_EMPTY:
		noti.type = UART_TX_EMPTY;
		break;
	case IOTBUS_UART_TX_RDY:
		noti.type = UART_TX_RDY;
		break;
	case IOTBUS_UART_RX_AVAIL:
		noti.type = UART_RX_AVAIL;
		break;
	case IOTBUS_UART_RECEIVED:
		noti.type = UART_RECEIVED;
		break;
	default:
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	noti.pid = 0;

	ret = ioctl(handle->fd, TIOCSETINTR, (unsigned long)&noti);
	if (ret != 0) {
		ibdbg("ioctl failed \n");
		return ret;
	}

	// Clear Callback
	handle->cb[int_type] = NULL;

	// Kill Thread
	if (handle->pid[int_type] > 0) {
		int pid = handle->pid[int_type];
		handle->pid[int_type] = 0;
		ret = pthread_cancel(pid);
		if (ret < 0) {
			ibdbg("pthread cancel failed[%d].\n", ret);
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	return 0;
}

/**
 * @brief Gets a device number of the UART.
 */
int iotbus_uart_get_device(iotbus_uart_context_h hnd)
{
	struct _iotbus_uart_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_uart_s *)hnd->handle;
	return handle->device;
}

uint32_t iotbus_uart_get_timestamp(iotbus_uart_context_h hnd)
{
#ifdef CONFIG_IOTBUS_INTERRUPT_TIMESTAMP
	struct _iotbus_uart_s *handle;
	handle = (struct _iotbus_uart_s *)hnd->handle;
	return handle->timestamp;
#else
	return IOTBUS_ERROR_NOT_SUPPORTED;	
#endif
}

#ifdef __cplusplus
}
#endif
