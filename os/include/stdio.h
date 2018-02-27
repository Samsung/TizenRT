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
/****************************************************************************
 * include/stdio.h
 *
 *   Copyright (C) 2007-2009, 2011, 2013-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/
/**
 * @defgroup KERNEL KERNEL/LIBC
 */

/**
 * @defgroup STDIO_LIBC STDIO
 * @brief Provides APIs for Standard Input/Output
 * @ingroup KERNEL
 *
 * @{
 */

/// @file stdio.h
/// @brief Standard Input / Output APIs

#ifndef __INCLUDE_STDIO_H
#define __INCLUDE_STDIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdarg.h>
#include <sched.h>
#include <semaphore.h>
#include <time.h>

#include <tinyara/fs/fs.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* File System Definitions **************************************************/

#define FILENAME_MAX _POSIX_NAME_MAX

/* The (default) size of the I/O buffers */

#if (CONFIG_STDIO_BUFFER_SIZE > 0)
#  define BUFSSIZ   CONFIG_STDIO_BUFFER_SIZE
#else
#  define BUFSSIZ   64
#endif

/* The following three definitions are for ANSI C, used by setvbuf */

#define _IOFBF     0               /* Fully buffered */
#define _IOLBF     1               /* Line buffered */
#define _IONBF     2               /* Unbuffered */

/* File system error values *************************************************/

#define EOF        (-1)

/* The first three _iob entries are reserved for standard I/O */
#if CONFIG_NFILE_DESCRIPTORS > 0 && CONFIG_NFILE_STREAMS > 0
#define stdin  (&sched_getstreams()->sl_streams[0])
#define stdout (&sched_getstreams()->sl_streams[1])
#define stderr (&sched_getstreams()->sl_streams[2])
#else
#define stdin  (NULL)
#define stdout (NULL)
#define stderr (NULL)
#endif

/* These APIs are not implemented and/or can be synthesized from
 * supported APIs.
 */

/**
 * @brief put a byte on a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define putc(c, s) fputc((c), (s))
/**
 * @brief put a byte on a stdout stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define putchar(c) fputc(c, stdout)
/**
 * @brief get a byte from a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define getc(s)    fgetc(s)
/**
 * @brief get a byte from a stdin stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define getchar()  fgetc(stdin)
/**
 * @cond
 * @internal
 */
#define rewind(s)  ((void)fseek((s), 0, SEEK_SET))
/**
 * @endcond
 */

/* Path to the directory where temporary files can be created */

#ifndef CONFIG_LIBC_TMPDIR
#define CONFIG_LIBC_TMPDIR "/tmp"
#endif

#define P_tmpdir CONFIG_LIBC_TMPDIR

/* Maximum size of character array to hold tmpnam() output. */

#ifndef CONFIG_LIBC_MAX_TMPFILE
#define CONFIG_LIBC_MAX_TMPFILE 32
#endif

#define L_tmpnam CONFIG_LIBC_MAX_TMPFILE

/* the maximum number of unique temporary file names that can be generated */

#define TMP_MAX 56800235584ull

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/* Streams */

typedef struct file_struct FILE;

/****************************************************************************
 * Public Variables
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/* ANSI-like File System Interfaces */

/* Operations on streams (FILE) */
/**
 * @cond
 * @internal
 */
void clearerr(register FILE *stream);
/**
 * @endcond
 */
/**
 * @brief close a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fclose(FAR FILE *stream);
/**
 * @brief flush a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fflush(FAR FILE *stream);
/**
 * @brief test end-of-file indicator on a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int feof(FAR FILE *stream);
/**
 * @brief test error indicator on a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int ferror(FAR FILE *stream);
/**
 * @brief map a stream pointer to a file descriptor
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fileno(FAR FILE *stream);
/**
 * @brief get a byte from a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fgetc(FAR FILE *stream);
/**
 * @brief get current file position information
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fgetpos(FAR FILE *stream, FAR fpos_t *pos);
/**
 * @brief get a string from a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
char *fgets(FAR char *s, int n, FAR FILE *stream);
/**
 * @brief open a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR FILE *fopen(FAR const char *path, FAR const char *type);
/**
 * @brief open a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
FAR FILE *freopen(FAR const char *path, FAR const char *mode, FAR FILE *stream);
/**
 * @brief assign buffering to a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
void   setbuf(FAR FILE *stream, FAR char *buf);
/**
 * @brief assign buffering to a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
int    setvbuf(FAR FILE *stream, FAR char *buffer, int mode, size_t size);
/**
 * @brief print formatted output
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fprintf(FAR FILE *stream, FAR const char *format, ...);
/**
 * @brief put a byte on a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fputc(int c, FAR FILE *stream);
/**
 * @brief put a string on a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fputs(FAR const char *s, FAR FILE *stream);
/**
 * @brief binary input
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
size_t fread(FAR void *ptr, size_t size, size_t n_items, FAR FILE *stream);
/**
 * @brief reposition a file-position indicator in a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fseek(FAR FILE *stream, long int offset, int whence);
/**
 * @brief set current file position
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int fsetpos(FAR FILE *stream, FAR fpos_t *pos);
/**
 * @brief return a file offset in a stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
long ftell(FAR FILE *stream);
/**
 * @brief binary output
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
size_t fwrite(FAR const void *ptr, size_t size, size_t n_items, FAR FILE *stream);
/**
 * @brief get a string from a stdin stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR char *gets(FAR char *s);
/**
 * @brief reads a line from stdin into the buffer
 * @details @b #include <stdio.h> \n
 *   gets() reads a line from stdin into the buffer pointed to by s until
 *   either a terminating newline or EOF, which it replaces with '\0'.  Reads
 *   at most n-1 characters from stdin into the array pointed to by str until
 *   new-line character, end-of-file condition, or read error.   The newline
 *   character, if encountered, is not saved in the arraay.  A NUL character
 *   is written immediately after the last character read into the array, or
 *   to str[0] if no characters were read.
 *
 *   If n is zero or is greater than RSIZE_MAX, a null character is written
 *   to str[0] but the function reads and discards characters from stdin
 *   until new-line character, end-of-file condition, or read error (not
 *   implemented).
 *
 *   If n-1 characters have been read, continues reading and discarding the
 *   characters from stdin until new-line character, end-of-file condition,
 *   or read error.
 *
 * @param[out] s buffer to save string from stdin
 * @param[in] n size of string to save
 * @return On success, a pointer of buffer is returned. On failure, NULL is returned.
 * @since TizenRT v1.0
 */
FAR char *gets_s(FAR char *s, rsize_t n);
/**
 * @brief push byte back into input stream
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int ungetc(int c, FAR FILE *stream);

/* Operations on the stdout stream, buffers, paths, and the whole printf-family */

/**
 * @brief print formatted output
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int printf(FAR const char *format, ...);
/**
 * @brief put a string on standard output
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int puts(FAR const char *s);
/**
 * @cond
 * @internal
 */
int rename(FAR const char *oldpath, FAR const char *newpath);
/**
 * @endcond
 */
/**
 * @brief print formatted output
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sprintf(FAR char *buf, FAR const char *format, ...);
/**
 * @brief print formatted output
 * @details @b #include <stdio.h> \n
 * This function is similar to sprintf, except that it dynamically
 * allocates a string (as with malloc) to hold the output, instead of
 * putting the output in a buffer you allocate in advance.  The ptr
 * argument should be the address of a char * object, and a successful
 * call to asprintf stores a pointer to the newly allocated string at that
 * location.
 * @return The returned value is the number of characters allocated for the buffer,
 * or less than zero if an error occurred. Usually this means that the buffer
 * could not be allocated.
 * @since TizenRT v1.0
 */
int asprintf(FAR char **ptr, FAR const char *fmt, ...);
/**
 * @brief print formatted output
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int snprintf(FAR char *buf, size_t size, FAR const char *format, ...);
/**
 * @brief convert formatted input
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int sscanf(FAR const char *buf, FAR const char *fmt, ...);
/**
 * @brief write error messages to standard error
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
void perror(FAR const char *s);

/**
 * @brief format output of a stdarg argument list
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int vprintf(FAR const char *format, va_list ap);
/**
 * @brief format output of a stdarg argument list
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int vfprintf(FAR FILE *stream, const char *format, va_list ap);
/**
 * @brief format output of a stdarg argument list
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int vsprintf(FAR char *buf, const char *format, va_list ap);
/**
 * @cond
 * @internal
 */
int vasprintf(FAR char **ptr, const char *fmt, va_list ap);
/**
 * @endcond
 */
/**
 * @brief format output of a stdarg argument list
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int vsnprintf(FAR char *buf, size_t size, const char *format, va_list ap);
/**
 * @brief format input of a stdarg argument list
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int vsscanf(FAR const char *buf, FAR const char *s, va_list ap);

/* Operations on file descriptors including:
 *
 * POSIX-like File System Interfaces (fdopen), and
 * Extensions from the Open Group Technical Standard, 2006, Extended API Set
 *   Part 1 (dprintf and vdprintf)
 */
/**
 * @brief associate a stream with a file descriptor
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR FILE *fdopen(int fd, FAR const char *type);
/**
 * @cond
 * @internal
 */
int dprintf(int fd, FAR const char *fmt, ...);
/**
 * @internal
 */
int vdprintf(int fd, FAR const char *fmt, va_list ap);

/* Operations on paths */
/**
 * @internal
 */
FAR char *tmpnam(FAR char *s);
/**
 * @internal
 */
FAR char *tempnam(FAR const char *dir, FAR const char *pfx);
/**
 * @brief remove a file
 * @details @b #include <stdio.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
int remove(FAR const char *path);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_STDIO_H */
/**
 * @}
 */
