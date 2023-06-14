/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#if defined( __ICCARM__)
#include <time.h>
#include <stdint.h>
#else

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif
#include "cstub.h"

#include <errno.h>

#ifndef NULL
#define NULL 0
#endif

#define __MYPID 1

#if defined(__arm__)
#define CSTUB_READ       _read
#define CSTUB_WRITE      _write
#define CSTUB_OPEN       _open
#define CSTUB_CLOSE      _close
#define CSTUB_SBRK       _sbrk
#define CSTUB_ISATTY     _isatty
#define CSTUB_LSEEK      _lseek
#define CSTUB_FSTAT      _fstat
#define CSTUB_GETPID     _getpid
#define CSTUB_KILL       _kill
#define CSTUB_GETTIMEOFDAY _gettimeofday

#elif defined(__mips__)
#define CSTUB_READ       read
#define CSTUB_WRITE      write
#define CSTUB_OPEN       open
#define CSTUB_CLOSE      close
#define CSTUB_SBRK       sbrk
#define CSTUB_ISATTY     isatty
#define CSTUB_LSEEK      lseek
#define CSTUB_FSTAT      fstat
#define CSTUB_GETPID     getpid
#define CSTUB_KILL       kill
#define CSTUB_GETTIMEOFDAY gettimeofday
#else
#define CSTUB_READ       read
#define CSTUB_WRITE      write
#define CSTUB_OPEN       open
#define CSTUB_CLOSE      close
#define CSTUB_SBRK       sbrk
#define CSTUB_ISATTY     isatty
#define CSTUB_LSEEK      lseek
#define CSTUB_FSTAT      fstat
#define CSTUB_GETPID     getpid
#define CSTUB_KILL       kill
#define CSTUB_GETTIMEOFDAY gettimeofday
#endif

//void write_thread_id_priv(void)
//{
//  return;
//}
//
void __cpu_idle(void)
{
	while (1);
	return;
}
/* external function */
extern void _exit(int sig);
extern void __PutCharacter(char c1, char c2);

/* internal function */
static int
outbyte(char ch)
{
	__PutCharacter(ch, ch);
	return 1;
}

static unsigned char
inbyte(void)
{
	return 0;
}
#undef errno
int errno;

void
__main(void)
{
	return;
}

/*
 * read  -- read bytes from the serial port. Ignore fd, since
 *          we only have stdin.
 */
int
CSTUB_READ(int fd, char *buf, int nbytes)
{
	int i = 0;

	(void)fd;
	for (i = 0; i < nbytes; i++) {
		*(buf + i) = inbyte();
		if ((*(buf + i) == '\n') || (*(buf + i) == '\r')) {
			(*(buf + i)) = 0;
			break;
		}
	}
	return (i);
}

/*
 * write -- write bytes to the serial port. Ignore fd, since
 *          stdout and stderr are the same. Since we have no filesystem,
 *          open will only return an error.
 */
int
CSTUB_WRITE(int fd, char *buf, int nbytes)
{
	int i;
	(void)fd;
	for (i = 0; i < nbytes; i++) {
		outbyte(*(buf + i));
	}
	return (nbytes);
}

/*
 * open -- open a file descriptor. We don't have a filesystem, so
 *         we return an error.
 */
int
CSTUB_OPEN(char *buf, int flags, int mode)
{
	(void)buf;
	(void)flags;
	(void)mode;
	errno = EIO;
	return (-1);
}

/*
 * close -- close a file descriptor. We don't need
 *          to do anything, but pretend we did.
 */
int
CSTUB_CLOSE(int fd)
{
	(void)fd;
	return (0);
}

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */
caddr_t
CSTUB_SBRK(int nbytes)
{
	static caddr_t heap_ptr = (caddr_t)&HEAP_START;
	caddr_t        base;

	if (((int)&HEAP_END - (int)heap_ptr) >= 0) {
		base = heap_ptr;
		heap_ptr += nbytes;
		return (base);
	} else {
		errno = ENOMEM;
		return ((caddr_t) -1);
	}
}

/*
 * isatty -- returns 1 if connected to a terminal device,
 *           returns 0 if not. Since we're hooked up to a
 *           serial port, we'll say yes and return a 1.
 */
int
CSTUB_ISATTY(int fd)
{
	(void)fd;
	return (1);
}

/*
 * lseek -- move read/write pointer. Since a serial port
 *          is non-seekable, we return an error.
 */
off_t
CSTUB_LSEEK(int fd, off_t offset, int whence)
{
	(void)fd;
	(void)offset;
	(void)whence;
	errno = ESPIPE;
	return ((off_t) -1);
}

/*
 * fstat -- get status of a file. Assume it is a
 * terminal (character special file).
 */
int
CSTUB_FSTAT(int fd, struct stat *buf)
{
	(void)fd;
	buf->st_mode = S_IFCHR;
	return 0;
}

/*
 * getpid -- only one process, so just return 1.
 */
int
CSTUB_GETPID(void)
{
	return __MYPID;
}

/*
 * kill -- go out via exit...
 */
int
CSTUB_KILL(int pid, int sig)
{
	if (pid == __MYPID) {
		_exit(sig);
	}
	return 0;
}

/*
 * print -- do a raw print of a string
 */
int
print(char *ptr)
{
	while (*ptr) {
		outbyte(*ptr++);
	}
	return 0;
}

/*
 * putnum -- print a 32 bit number in hex
 */
int
putnum(uint32_t num)
{
	char  buffer[9];
	int   count;
	char  *bufptr = buffer;
	int   digit;

	for (count = 7 ; count >= 0 ; count--) {
		digit = (num >> (count * 4)) & 0xf;

		if (digit <= 9) {
			*bufptr++ = (char)('0' + digit);
		} else {
			*bufptr++ = (char)('a' - 10 + digit);
		}
	}

	*bufptr = (char) 0;
	print(buffer);
	return 0;
}

int
CSTUB_GETTIMEOFDAY(
	struct timeval __attribute__((unused)) * tp,
	void __attribute__((unused)) * tzvp)
{
	return 0;
}

