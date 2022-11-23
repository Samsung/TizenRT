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
 * libc/stdio/lib_sscanf.c
 *
 *   Copyright (C) 2007, 2008, 2011-2014 Gregory Nutt. All rights reserved.
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/compiler.h>

#include <sys/types.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <debug.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MAXLN 128

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#define HH_MOD -2
#define H_MOD  -1
#define NO_MOD  0
#define L_MOD   1
#define LL_MOD  2

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Global Function Prototypes
 ****************************************************************************/

int vsscanf(FAR const char *buf, FAR const char *fmt, va_list ap);

/****************************************************************************
 * Global Constant Data
 ****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/****************************************************************************
 * Private Constant Data
 ****************************************************************************/

static const char spaces[] = " \t\n\r\f\v";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  findwidth
 *
 * Description:
 *    Try to figure out the width of the input data.
 *
 ****************************************************************************/

static int findwidth(FAR const char *buf, FAR const char *fmt)
{
	FAR const char *next = fmt + 1;

	/* No... is there a space after the format? Or does the format string end
	 * here?
	 */

	if (isspace(*next) || *next == 0) {
		/* Use the input up until the first white space is encountered. */

		return strcspn(buf, spaces);
	}

	/* No.. Another possibility is the format character is followed by
	 * some recognizable delimiting value.
	 */

	if (*next != '%') {
		/* If so we will say that the string ends there if we can find that
		 * delimiter in the input string.
		 */

		FAR const char *ptr = strchr(buf, *next);
		if (ptr) {
			return (int)(ptr - buf);
		}
	}

	/* No... the format has no delimiter and is back-to-back with the next
	 * format (or is followed by a delimiter that does not exist in the
	 * input string).  At this point we just bail and Use the input up until
	 * the first white space is encountered.
	 *
	 * NOTE:  This means that values from the following format may be
	 * concatenated with the first. This is a bug.  We have no generic way of
	 * determining the width of the data if there is no fieldwidth, no space
	 * separating the input, and no usable delimiter character.
	 */

	return strcspn(buf, spaces);
}

/****************************************************************************
 * Private Variables
 ****************************************************************************/

/****************************************************************************
 * Function:  findscanset
 *
 * Description:
 *    Fill in the given table from the scanset at the given format.
 *    Return a pointer to the character the closing ']'.
 *    The table has a 1 wherever characters should be considered part of the
 *    scanset.
 *
 *    Function findscanset based on source function __sccl of FreeBSD
 *   (https://github.com/lattera/freebsd/blob/master/sys/kern/subr_scanf.c)
 *
 ****************************************************************************/

#ifdef CONFIG_LIBC_SCANSET
static FAR const char *findscanset(FAR const char *fmt,
					FAR unsigned char set[32])
{
	int c;
	int n;
	int v;
	int i;

	fmt++;           /* Skip '[' */

	/* First `clear' the whole table */

	c = *fmt++;      /* First char hat => negated scanset */
	if (c == '^') {
		v = 1;       /* Default => accept */
		c = *fmt++;  /* Get new first char */
	} else {
		v = 0;       /* Default => reject */
	}

	memset(set, 0, 32);
	if (c == 0) {
		goto doexit;
	}

	/* Now set the entries corresponding to the actual scanset
	* to the opposite of the above.
	*
	* The first character may be ']' (or '-') without being special;
	* the last character may be '-'.
	*/

	for (;;) {
		set[c / 8] |= (1 << (c % 8));  /* Take character c */

doswitch:
		n = *fmt++;    /* Examine the next */
		switch (n) {
		case 0:      /* Format ended too soon */
		case ']':    /* End of scanset */
			goto doexit;

		case '-':
			/* A scanset of the form
			*
			*  [01+-]
			*
			* is defined as "the digit 0, the digit 1, the character +, the
			* character -", but the effect of a scanset such as
			*
			*  [a-zA-Z0-9]
			*
			* is implementation defined.  The V7 Unix scanf treats "a-z" as
			* "the letters a through z", but treats "a-a" as "the letter a,
			* the character -, and the letter a".
			*
			* For compatibility, the `-' is not considerd to define a range
			* if the character following it is either a close bracket
			* (required by ANSI) or is not numerically greater than the
			* character* we just stored in the table (c).
			*/

			n = *fmt;
			if (n == ']' || n < c) {
				c = '-';
				break;  /* Resume the for(;;) */
			}

			fmt++;
			do {
				/* Fill in the range */

				c++;
				set[c / 8] |= (1 << (c % 8));  /* Take character c */
			} while (c < n);

			/* Alas, the V7 Unix scanf also treats formats such as [a-c-e] as
			* "the letters a through e".  This too is permitted by the
			* standard.
			*/

			goto doswitch;

		default:    /* Just another character */
			c = n;
			break;
		}
	}

doexit:
	if (v) { /* Default => accept */
		for (i = 0; i < 32; i++) {  /* Invert all */
			set[i] ^= 0xFF;
		}
	}

	return (fmt - 1);
}
#endif

/****************************************************************************
 * Function:  scansetwidth
 ****************************************************************************/

#ifdef CONFIG_LIBC_SCANSET
static int scansetwidth(FAR const char *buf,
			FAR const unsigned char set[32])
{
	FAR const char *next = buf;
	int c;

	while (*next) {
		c = *next;
		if ((set[c / 8] & (1 << (c % 8))) == 0) {
			break;
		}

		next++;
	}

	return (next - buf);
}
#endif



/****************************************************************************
 * Function:  sscanf
 *
 * Description:
 *    ANSI standard sscanf implementation.
 *
 ****************************************************************************/

int sscanf(FAR const char *buf, FAR const char *fmt, ...)
{
	va_list ap;
	int count;

	va_start(ap, fmt);
	count = vsscanf((FAR const char *)buf, fmt, ap);
	va_end(ap);
	return count;
}

/****************************************************************************
 * Function:  vsscanf
 *
 * Description:
 *    ANSI standard vsscanf implementation.
 *
 ****************************************************************************/

int vsscanf(FAR const char *buf, FAR const char *fmt, va_list ap)
{
	FAR const char *bufstart;
	FAR char *tv;
	FAR const char *tc;
	int modifier;
	bool noassign;
	int count;
	int width;
	int fwidth;
	int base = 10;
	char tmp[MAXLN];
#ifdef CONFIG_LIBC_LONG_LONG
	unsigned long long *plonglong = NULL;
#endif
	unsigned long  *plong  = NULL;
	unsigned int   *pint   = NULL;
	unsigned short *pshort = NULL;
	unsigned char  *pchar  = NULL;
#ifdef CONFIG_LIBC_SCANSET
	unsigned char   set[32]; /* Bit field (256 / 8) */
#endif

	lvdbg("vsscanf: buf=\"%s\" fmt=\"%s\"\n", buf, fmt);

	/* Remember the start of the input buffer.  We will need this for %n
	 * calculations.
	 */

	bufstart = buf;

	/* Parse the format, extracting values from the input buffer as needed */

	count = 0;
	width = 0;
	noassign = false;
	modifier = NO_MOD;

	/* Loop until all characters in the fmt string have been processed.  We
	 * may have to continue loop after reaching the end the input data in
	 * order to handle trailing %n format specifiers.
	 */

	while (*fmt) {
		/* Skip over white space */

		while (isspace(*fmt)) {
			fmt++;
		}

		/* Check for a conversion specifier */

		if (*fmt == '%') {
			lvdbg("vsscanf: Specifier found\n");

			/* Check for qualifiers on the conversion specifier */

			fmt++;
			for (; *fmt; fmt++) {
				lvdbg("vsscanf: Processing %c (next %c)\n", *fmt, *(fmt + 1));
#ifdef CONFIG_LIBC_SCANSET
				if (strchr("dibouxcsefgn[%", *fmt)) {
#else
				if (strchr("dibouxcsefgn%", *fmt)) {
#endif
					break;
				}

				if (*fmt == '*') {
					noassign = true;
				} else if (*fmt == 'l' || *fmt == 'L') {
					modifier = L_MOD;

					if (*(fmt + 1) == 'l' || *(fmt + 1) == 'L') {
						modifier = LL_MOD;
						fmt++;
					}
				} else if (*fmt == 'h' || *fmt == 'H') {
					modifier = H_MOD;

					if (*(fmt + 1) == 'h' || *(fmt + 1) == 'H') {
						modifier = HH_MOD;
						fmt++;
					}
				} else if (*fmt >= '1' && *fmt <= '9') {
					for (tc = fmt; isdigit(*fmt); fmt++);
					strncpy(tmp, tc, fmt - tc);
					tmp[fmt - tc] = '\0';
					width = atol(tmp);
					fmt--;
				}
			}

			/* Process %s:  String conversion */

			if (*fmt == 's') {
				lvdbg("vsscanf: Performing string conversion\n");

				/* Get a pointer to the char * value.  We need to do this even
				 * if we have reached the end of the input data in order to
				 * update the 'ap' variable.
				 */

				tv = NULL;		/* To avoid warnings about begin uninitialized */
				if (!noassign) {
					tv = va_arg(ap, char *);
					tv[0] = '\0';
				}

				/* But we only perform the data conversion is we still have
				 * bytes remaining in the input data stream.
				 */

				if (*buf) {
					/* Skip over white space */

					while (isspace(*buf)) {
						buf++;
					}

					/* Guess a field width using some heuristics */

					fwidth = findwidth(buf, fmt);

					/* Use the actual field's width if 1) no fieldwidth
					 * specified or 2) the actual field's width is smaller
					 * than fieldwidth specified
					 */

					if (!width || fwidth < width) {
						width  = fwidth;
					}


					/* Copy the string (if we are making an assignment) */

					if (!noassign) {
						strncpy(tv, buf, width);
						tv[width] = '\0';
						count++;
					}

					/* Update the buffer pointer past the string in the input */

					buf += width;
				}
			}

#ifdef CONFIG_LIBC_SCANSET
			/* Process %[:  Scanset conversion */

			if (*fmt == '[') {
				lvdbg("vsscanf: Performing scanset conversion\n");

				fmt = findscanset(fmt, set); /* find scanset */

				/* Get a pointer to the char * value.  We need to do this even
				* if we have reached the end of the input data in order to
				* update the 'ap' variable.
				*/

				tv = NULL;      /* To avoid warnings about begin uninitialized */
				if (!noassign) {
					tv    = va_arg(ap, FAR char *);
					tv[0] = '\0';
				}

				/* But we only perform the data conversion is we still have
				* bytes remaining in the input data stream.
				*/

				if (*buf) {
					/* Skip over white space */

					while (isspace(*buf)) {
						buf++;
					}

					/* Guess a field width using some heuristics */

					fwidth = scansetwidth(buf, set);

					/* Use the actual field's width if 1) no fieldwidth
					* specified or 2) the actual field's width is smaller
					* than fieldwidth specified
					*/

					if (!width || fwidth < width) {
						width  = fwidth;
					}


					/* Copy the string (if we are making an assignment) */

					if (!noassign) {
						strncpy(tv, buf, width);
						tv[width] = '\0';
						count++;
					}

					/* Update the buffer pointer past the string in the input */

					buf += width;
				}
			}
#endif

			/* Process %c:  Character conversion */

			else if (*fmt == 'c') {
				lvdbg("vsscanf: Performing character conversion\n");

				/* Get a pointer to the char * value.  We need to do this even
				 * if we have reached the end of the input data in order to
				 * update the 'ap' variable.
				 */

				tv = NULL;		/* To avoid warnings about beign uninitialized */
				if (!noassign) {
					tv = va_arg(ap, char *);
					tv[0] = '\0';
				}

				/* But we only perform the data conversion is we still have
				 * bytes remaining in the input data stream.
				 */

				if (*buf) {
					/* Was a field width specified? */

					if (!width) {
						/* No, then width is this one single character */

						width = 1;
					}

					/* Copy the character(s) (if we are making an assignment) */

					if (!noassign) {
						strncpy(tv, buf, width);
						count++;
					}

					/* Update the buffer pointer past the character(s) in the
					 * input
					 */

					buf += width;
				}
			}

			/* Process %d, %o, %b, %x, %u:  Various integer conversions */

			else if (strchr("dobxu", *fmt)) {
				bool sign;

				lvdbg("vsscanf: Performing integer conversion\n");

				/* Get a pointer to the integer value.  We need to do this even
				 * if we have reached the end of the input data in order to
				 * update the 'ap' variable.
				 */

				if (!noassign) {
					/* We have to check whether we need to return a long or an
					 * int.
					 */

					switch (modifier) {
					case HH_MOD:
						pchar = va_arg(ap, unsigned char *);
						*pchar = 0;
						break;
					case H_MOD:
						pshort = va_arg(ap, unsigned short *);
						*pshort = 0;
						break;
					case NO_MOD:
						pint = va_arg(ap, unsigned int *);
						*pint = 0;
						break;
#ifdef CONFIG_LIBC_LONG_LONG
					case LL_MOD:
						plonglong = va_arg(ap, unsigned long long *);
						*plonglong = 0;
						break;
#endif
					case L_MOD:
					default:
						plong = va_arg(ap, unsigned long *);
						*plong = 0;
						break;
					}
				}

				/* But we only perform the data conversion if we still have
				 * bytes remaining in the input data stream.
				 */

				if (*buf) {
					FAR char *endptr;
					int errsave;
					unsigned long tmplong = 0;
#ifdef CONFIG_LIBC_LONG_LONG
					unsigned long long tmplonglong = 0;
#endif

					/* Skip over any white space before the integer string */

					while (isspace(*buf)) {
						buf++;
					}

					/* The base of the integer conversion depends on the
					 * specific conversion specification.
					 */

					sign = false;
					switch (*fmt) {
					default:
					case 'd':
						sign = true;

					/* FALLTHROUGH */

					case 'u':
						base = 10;
						break;

					case 'X':
					case 'x':
						base = 16;
						break;

					case 'o':
						base = 8;
						break;

					case 'b':
						base = 2;
						break;
					}

					/* Was a field width specified? */

					if (!width) {
						/* No... Guess a field width using some heuristics */

						width = findwidth(buf, fmt);
					}

					width = MIN(sizeof(tmp) - 1, width);

					/* Copy the numeric string into a temporary working
					 * buffer.
					 */

					strncpy(tmp, buf, width);
					tmp[width] = '\0';

					lvdbg("vsscanf: tmp[]=\"%s\"\n", tmp);

					/* Perform the integer conversion */
					/* Preserve the errno value */

					errsave = get_errno();
					set_errno(0);

					switch (modifier) {
					case HH_MOD:
					case H_MOD:
					case NO_MOD:
					case L_MOD:
					default:
						if (sign) {
							tmplong = strtol(tmp, &endptr, base);
						} else {
							tmplong = strtoul(tmp, &endptr, base);
						}
						break;

#ifdef CONFIG_LIBC_LONG_LONG
					case LL_MOD:
						if (sign) {
							tmplonglong = strtoll(tmp, &endptr, base);
						} else {
							tmplonglong = strtoull(tmp, &endptr, base);
						}
						break;
#endif
					}

					/* Check if the number was successfully converted */

					if (tmp == endptr || get_errno() == ERANGE) {
						return count;
					}

					/* Move by the actual number of characters converted */

					buf += (endptr - tmp);
					set_errno(errsave);
					if (!noassign) {

						/* We have to check whether we need to return a long
						 * or an int.
						 */

						switch (modifier) {
						case HH_MOD:
							lvdbg("Return %ld to 0x%p\n", tmplong, pchar);
							*pchar = (unsigned char)tmplong;
							break;
						case H_MOD:
							lvdbg("Return %ld to 0x%p\n", tmplong, pshort);
							*pshort = (unsigned short)tmplong;
							break;
						case NO_MOD:
							lvdbg("Return %ld to 0x%p\n", tmplong, pint);
							*pint = (unsigned int)tmplong;
							break;
#ifdef CONFIG_LIBC_LONG_LONG
						case LL_MOD:
							lvdbg("Return %ld to 0x%p\n", tmplonglong, plonglong);
							*plonglong = tmplonglong;
							break;
#endif
						case L_MOD:
						default:
							lvdbg("Return %ld to 0x%p\n", tmplong, plong);
							*plong = tmplong;
							break;
						}

						count++;
					}
				}
			}

			/* Process %a, %A, %f, %F, %e, %E, %g, and %G:  Floating point
			 * conversions
			 */

			else if (strchr("aAfFeEgG", *fmt) != NULL) {
#ifdef CONFIG_HAVE_DOUBLE
				FAR double_t *pd = NULL;
#endif
				FAR float *pf = NULL;

				lvdbg("vsscanf: Performing floating point conversion\n");

				/* Get a pointer to the double value.  We need to do this even
				 * if we have reached the end of the input data in order to
				 * update the 'ap' variable.
				 */

				if (!noassign) {
					/* We have to check whether we need to return a float or a
					 * double.
					 */

#ifdef CONFIG_HAVE_DOUBLE
					if (modifier >= L_MOD) {
						pd = va_arg(ap, double_t *);
						*pd = 0.0;
					} else
#endif
					{
						pf = va_arg(ap, float *);
						*pf = 0.0;
					}
				}
#ifdef CONFIG_LIBC_FLOATINGPOINT
				/* But we only perform the data conversion is we still have
				 * bytes remaining in the input data stream.
				 */

				if (*buf) {
					FAR char *endptr;
					int errsave;
#ifdef CONFIG_HAVE_DOUBLE
					double dvalue;
#endif
					float fvalue;

					/* Skip over any white space before the real string */

					while (isspace(*buf)) {
						buf++;
					}

					/* Was a fieldwidth specified? */

					if (!width) {
						/* No... Guess a field width using some heuristics */

						width = findwidth(buf, fmt);
					}

					width = MIN(sizeof(tmp) - 1, width);

					/* Copy the real string into a temporary working buffer. */

					strncpy(tmp, buf, width);
					tmp[width] = '\0';

					lvdbg("vsscanf: tmp[]=\"%s\"\n", tmp);

					/* Preserve the errno value */

					errsave = get_errno();
					set_errno(0);

					/* Perform the floating point conversion */

#ifdef CONFIG_HAVE_DOUBLE
					if (modifier >= L_MOD) {
						/* Get the converted double value */

						dvalue = strtod(tmp, &endptr);
					} else
#endif
					{
						fvalue = strtof(tmp, &endptr);
					}

					/* Check if the number was successfully converted */

					if (tmp == endptr || get_errno() == ERANGE) {
						return count;
					}

					/* Move by the actual number of characters converted */

					buf += (endptr - tmp);
					set_errno(errsave);

					if (!noassign) {

						/* We have to check whether we need to return a float
						 * or a double.
						 */

#ifdef CONFIG_HAVE_DOUBLE
						if (modifier >= L_MOD) {
							lvdbg("vsscanf: Return %f to %p\n", dvalue, pd);
							*pd = dvalue;
						} else
#endif
						{
							lvdbg("vsscanf: Return %f to %p\n", (double)fvalue, pf);
							*pf = fvalue;
						}

						count++;
					}
				}
#endif
			}

			/* Process %n:  Character count */

			else if (*fmt == 'n') {
				lvdbg("vsscanf: Performing character count\n");

				if (!noassign) {
					size_t nchars = (size_t)(buf - bufstart);

					/* Note %n does not count as a conversion */

					switch (modifier) {
					case HH_MOD:
						pchar = va_arg(ap, unsigned char *);
						*pchar = (unsigned char)nchars;
						break;
					case H_MOD:
						pshort = va_arg(ap, unsigned short *);
						*pshort = (unsigned short)nchars;
						break;
					case NO_MOD:
						pint = va_arg(ap, unsigned int *);
						*pint = (unsigned int)nchars;
						break;
#ifdef CONFIG_LIBC_LONG_LONG
					case LL_MOD:
						plonglong = va_arg(ap, unsigned long long *);
						*plonglong = (unsigned long long)nchars;
						break;
#endif
					case L_MOD:
					default:
						plong = va_arg(ap, unsigned long *);
						*plong = (unsigned long)nchars;
						break;
					}
				}
			}

			width = 0;
			noassign = false;
			modifier = NO_MOD;

			fmt++;
		}

		/* It is not a conversion specifier */

		else if (*buf) {
			/* Skip over any leading spaces in the input buffer */

			while (isspace(*buf)) {
				buf++;
			}

			/* Skip over matching characters in the buffer and format */

			if (*fmt != *buf) {
				break;
			} else {
				fmt++;
				buf++;
			}
		} else {
			/* NULL terminator encountered */

			break;
		}
	}

	/* sscanf is required to return EOF if the input ends before the first
	 * matching failure or conversion.
	 */

	return count ? count : EOF;
}
