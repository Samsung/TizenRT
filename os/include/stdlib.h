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
 * include/stdlib.h
 *
 *   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
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
 * @defgroup STDLIB_LIBC STDLIB
 * @brief Provides APIs for Standard Library
 * @ingroup KERNEL
 * {
 */

/// @file stdlib.h
/// @brief standard library APIs

#ifndef __INCLUDE_STDLIB_H
#define __INCLUDE_STDLIB_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <tinyara/compiler.h>

#include <sys/types.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The C standard specifies two constants, EXIT_SUCCESS and
 * EXIT_FAILURE, that may be passed to exit() to indicate
 * successful or unsucessful termination, respectively.
 */

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/* The NULL pointer should be defined in this file but is currently defined
 * in sys/types.h.
 */

/* Maximum value returned by rand(). Must be a minimum of 32767. */

#define RAND_MAX INT_MAX

/* Integer expression whose value is the maximum number of bytes in a
 * character specified by the current locale.
 */

#define MB_CUR_MAX 1

/* The environ variable, normally 'extern char **environ;' is
 * not implemented as a function call.  However, get_environ_ptr()
 * can be used in its place.
 */

#ifndef CONFIG_DISABLE_ENVIRON
#define environ get_environ_ptr()
#endif

/****************************************************************************
 * Global Type Definitions
 ****************************************************************************/
/** @brief structure of memory information */
struct mallinfo {
	int arena;					/* This is the total size of memory allocated
								 * for use by malloc in bytes. */
	int ordblks;				/* This is the number of free (not in use) chunks */
	int mxordblk;				/* Size of the largest free (not in use) chunk */
	int uordblks;				/* This is the total size of memory occupied by
								 * chunks handed out by malloc. */
	int fordblks;				/* This is the total size of memory occupied
								 * by free (not in use) chunks.*/
};

/* Structure type returned by the div() function. */

struct div_s {
	int quot;					/* Quotient */
	int rem;					/* Remainder */
};

typedef struct div_s div_t;

/* Structure type returned by the ldiv() function. */

struct ldiv_s {
	long quot;					/* Quotient */
	long rem;					/* Remainder */
};

typedef struct ldiv_s ldiv_t;

/* Structure type returned by the lldiv() function. */

struct lldiv_s {
	long quot;					/* Quotient */
	long rem;					/* Remainder */
};

typedef struct lldiv_s lldiv_t;
/**
 * @}
 */

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Random number generation */
/**
 * @ingroup STDLIB_LIBC
 * @brief initialize random number generator
 *
 * @details @b #include <stdlib.h>
 * @param[in] seed An integer value to be used as seed
 * @return none
 * @since TizenRT v1.0
 */
void srand(unsigned int seed);
/**
 * @ingroup STDLIB_LIBC
 * @brief pseudo-random number generator
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int rand(void);

#define srandom(s) srand(s)

/**
 * @ingroup STDLIB_LIBC
 * @brief pseudo-random number generator
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.0
 */
long random(void);

/* Environment variable support */

#ifndef CONFIG_DISABLE_ENVIRON
/**
 * @ingroup STDLIB_LIBC
 * @brief Return a pointer to the thread specific environ variable
 * @details @b #include <stdlib.h> \n
 * This API is not a standard API. But another APIs which are related to ENV are in Stdlib,
 *         so this API is in Stdlib for ease.
 * @param[in] envsize A pointer to notify a size of environ variable
 * @return A pointer to the current thread environ variable
 * @since TizenRT v1.0
 */
FAR char *get_environ_ptr(size_t *envsize);
/**
 * @ingroup STDLIB_LIBC
 * @brief get value of an environment variable
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR char *getenv(FAR const char *name);
/**
 * @ingroup STDLIB_LIBC
 * @brief change or add a value to an environment
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int putenv(FAR const char *string);
/**
 * @ingroup STDLIB_LIBC
 * @brief clears the environment of all name-value pairs
 *        and sets the value of the external variable environ to NULL
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API
 * @since TizenRT v1.0
 */
int clearenv(void);
/**
 * @ingroup STDLIB_LIBC
 * @brief add or change environment variable
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int setenv(const char *name, const char *value, int overwrite);
/**
 * @ingroup STDLIB_LIBC
 * @brief remove an environment variable
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int unsetenv(const char *name);
#endif

/* Process exit functions */
/**
 * @ingroup STDLIB_LIBC
 * @brief terminate a process
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
void exit(int status) noreturn_function;
/**
 * @ingroup STDLIB_LIBC
 * @brief generate an abnormal process abort
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
void abort(void) noreturn_function;
#ifdef CONFIG_SCHED_ATEXIT
/**
 * @ingroup STDLIB_LIBC
 * @brief register a function to run at process termination
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int atexit(CODE void (*func)(void));
#endif
#ifdef CONFIG_SCHED_ONEXIT
/**
 * @ingroup STDLIB_LIBC
 * @brief register a function to be called at program exit
 * @details @b #include <stdlib.h> \n
 * SYSTEM CALL API
 *
 * @param[in] func A pointer to the function to be called when the task exits.
 * @param[in] arg An argument that will be provided to the on_exit() function when
 *                the task exits.
 * @return On success, 0 is returned. On failure, none-zero is returned.
 * @since TizenRT v1.0
 */
int on_exit(CODE void (*func)(int, FAR void *), FAR void *arg);
#endif

/* _Exit() is a stdlib.h equivalent to the unistd.h _exit() function */
/**
 * @cond
 * @internal
 */
void _exit(int status);			/* See unistd.h */
/**
 * @endcond
 */
#define   _Exit(s) _exit(s)

/* String to binary conversions */
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to a long integer
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
long strtol(const char *, char **, int);
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to an unsigned long
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
unsigned long strtoul(const char *, char **, int);
#ifdef CONFIG_HAVE_LONG_LONG
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to a long integer
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
long long strtoll(const char *, char **, int);
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to an unsigned long
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
unsigned long long strtoull(const char *, char **, int);
#endif
#ifdef CONFIG_HAVE_DOUBLE
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to a double-precision number
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
double strtod(FAR const char *str, FAR char **endptr);
#endif
#ifdef CONFIG_HAVE_LONG_DOUBLE
/**
 * @cond
 * @internal
 */
long double strtold(FAR const char *str, FAR char **endptr);
#endif
/**
 * @internal
 */
float strtof(FAR const char *str, FAR char **endptr);
/**
 * @endcond
 */

/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to an integer
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define atoi(nptr)  strtol((nptr), NULL, 10)
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to a long integer
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define atol(nptr)  strtol((nptr), NULL, 10)
#ifdef CONFIG_HAVE_LONG_LONG
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to a long integer
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define atoll(nptr) strtoll((nptr), NULL, 10)
#endif
/**
 * @ingroup STDLIB_LIBC
 * @brief convert a string to a double-precision number
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
#define atof(nptr)  strtod((nptr), NULL)

/* Binary to string conversions */
/**
 * @ingroup STDLIB_LIBC
 * @brief convert integer to string
 *
 * @details @b #include <stdlib.h>
 * @param[in] value Integer value
 * @param[out] str String where store the result
 * @param[in] base Numerical base
 * @return A pointer to the resulting null-terminated string, same as parameter str.
 * @since TizenRT v1.0
 */
char *itoa(int value, char *str, int base);

/* Wide character operations */

#ifdef CONFIG_LIBC_WCHAR
/**
 * @cond
 * @internal
 */
int mbtowc(FAR wchar_t *pwc, FAR const char *s, size_t n);
/**
 * @internal
 */
int wctomb(FAR char *s, wchar_t wchar);
/**
 * @endcond
 */
#endif

/* Memory Management */
/**
 * @ingroup STDLIB_LIBC
 * @brief a memory allocator
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR void *malloc(size_t);
/**
 * @ingroup STDLIB_LIBC
 * @brief free allocated memory
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
void free(FAR void *);
/**
 * @ingroup STDLIB_LIBC
 * @brief memory reallocator
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR void *realloc(FAR void *, size_t);
/**
 * @ingroup STDLIB_LIBC
 * @brief allocates size bytes and returns a pointer to the allocated memory
 *
 * @details @b #include <stdlib.h>
 * @param[in] alignment A power of two
 * @param[in] size Allocated memory size
 * @return On success, A pointer to the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v1.0
 */
FAR void *memalign(size_t, size_t);
/**
 * @ingroup STDLIB_LIBC
 * @brief Allocate and zero memory from the user heap.
 *
 * @details @b #include <stdlib.h>
 * @param[in] size Size (in bytes) of the memory region to be allocated.
 * @return On success, A pointer to the allocated memory is returned. On failure, NULL is returned.
 * @since TizenRT v1.0
 */
FAR void *zalloc(size_t);
/**
 * @ingroup STDLIB_LIBC
 * @brief a memory allocator
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
FAR void *calloc(size_t, size_t);

/* Misc */
/**
 * @ingroup STDLIB_LIBC
 * @brief return an integer absolute value
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
int abs(int j);
/**
 * @ingroup STDLIB_LIBC
 * @brief return a long integer absolute value
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
long int labs(long int j);
#ifdef CONFIG_HAVE_LONG_LONG
/**
 * @ingroup STDLIB_LIBC
 * @brief return a long integer absolute value
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
long long int llabs(long long int j);
#endif

#ifdef CONFIG_CAN_PASS_STRUCTS
/**
 * @ingroup STDLIB_LIBC
 * @brief compute the quotient and remainder of an integer division
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
div_t div(int numer, int denom);
/**
 * @cond
 * @internal
 */
ldiv_t ldiv(long numer, long denom);
#ifdef CONFIG_HAVE_LONG_LONG
/**
 * @internal
 */
lldiv_t lldiv(long long numer, long long denom);
#endif
#endif
/**
 * @endcond
 */

/**
 * @ingroup STDLIB_LIBC
 * @brief generates a unique temporary filename from template.
 * @details @b #include <stdlib.h> \n
 * @return always returns template.
 * @since TizenRT v2.0
 */
FAR char *mktemp(FAR char *path_template);

/**
 * @ingroup STDLIB_LIBC
 * @brief create a regular file with a unique name derived from template and return a file descriptor for the file open for reading and writing.
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v2.0
 */
int mkstemp(FAR char *path_template);

/* Sorting */
/**
 * @ingroup STDLIB_LIBC
 * @brief sort a table of data
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.0
 */
void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *));

/* Binary search */
/**
 * @ingroup STDLIB_LIBC
 * @brief binary search a sorted table
 * @details @b #include <stdlib.h> \n
 * POSIX API (refer to : http://pubs.opengroup.org/onlinepubs/9699919799/)
 * @since TizenRT v1.1
 */
FAR void *bsearch(FAR const void *key, FAR const void *base, size_t nel, size_t width, CODE int (*compar)(FAR const void *, FAR const void *));

#ifdef CONFIG_CAN_PASS_STRUCTS
/**
 * @ingroup STDLIB_LIBC
 * @brief returns a copy of updated current heap information for the user heap
 * @details @b #include <stdlib.h>
 * @return Current mallinfo structure returned.
 * @since TizenRT v1.0
 */
struct mallinfo mallinfo(void);
#else
/**
 * @ingroup STDLIB_LIBC
 * @brief returns a copy of updated current heap information for the user heap
 * @details @b #include <stdlib.h>
 * @param[out] info mallinfo structure to be updated
 * @return OK returned.
 * @since TizenRT v1.0
 */
int mallinfo(struct mallinfo *info);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif							/* __INCLUDE_STDLIB_H */
