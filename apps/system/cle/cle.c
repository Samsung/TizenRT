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
 * apps/system/cle/cle.c
 *
 *   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

#include <tinyara/config.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <syslog.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/ascii.h>
#include <tinyara/vt100.h>

#include <apps/cle.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Some environments may return CR as end-of-line, others LF, and others
 * both.  If not specified, the logic here assumes either (but not both) as
 * the default.
 */

#if defined(CONFIG_EOL_IS_CR)
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_BOTH_CRLF
#undef  CONFIG_EOL_IS_EITHER_CRLF
#elif defined(CONFIG_EOL_IS_LF)
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_BOTH_CRLF
#undef  CONFIG_EOL_IS_EITHER_CRLF
#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_EITHER_CRLF
#elif defined(CONFIG_EOL_IS_EITHER_CRLF)
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_BOTH_CRLF
#else
#undef  CONFIG_EOL_IS_CR
#undef  CONFIG_EOL_IS_LF
#undef  CONFIG_EOL_IS_BOTH_CRLF
#define CONFIG_EOL_IS_EITHER_CRLF 1
#endif

/* Control characters */

#undef  CTRL
#define CTRL(a)         ((a) & 0x1f)

#define CLE_BEL(priv)   cle_putch(priv, CTRL('G'))

/* Sizes of things */

#define TABSIZE         8		/* A TAB is eight characters */
#define TABMASK         7		/* Mask for TAB alignment */
#define NEXT_TAB(p)     (((p) + TABSIZE) & ~TABMASK)

/* Debug */

#ifndef CONFIG_SYSTEM_CLE_DEBUGLEVEL
#define CONFIG_SYSTEM_CLE_DEBUGLEVEL 0
#endif

#ifdef CONFIG_CPP_HAVE_VARARGS
#if CONFIG_SYSTEM_CLE_DEBUGLEVEL > 0
#define cledbg(format, ...) syslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define cledbg(...)
#endif

#if CONFIG_SYSTEM_CLE_DEBUGLEVEL > 1
#define clevdbg(format, ...) syslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define clevdbg(...)
#endif
#else
#if CONFIG_SYSTEM_CLE_DEBUGLEVEL > 0
#define cledbg  cle_debug
#else
#define cledbg  (void)
#endif

#if CONFIG_SYSTEM_CLE_DEBUGLEVEL > 1
#define clevdbg cle_debug
#else
#define clevdbg (void)
#endif
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* VI Key Bindings */

enum cle_key_e {
	KEY_BEGINLINE = CTRL('A'),	/* Move cursor to start of current line */
	KEY_LEFT      = CTRL('B'),	/* Move left one character */
	KEY_DEL       = CTRL('D'),	/* Delete a single character at the cursor position */
	KEY_ENDLINE   = CTRL('E'),	/* Move cursor to end of current line */
	KEY_RIGHT     = CTRL('F'),	/* Move right one character */
	KEY_DELLEFT   = CTRL('H'),	/* Delete character, left (backspace)  */
	KEY_DELEOL    = CTRL('K'),	/* Delete to the end of the line */
	KEY_DELLINE   = CTRL('U'),	/* Delete the entire line */
	KEY_QUOTE     = '\\',		/* The next character is quote (use literal value) */
};

/* This structure describes the overall state of the editor */

struct cle_s {
	uint16_t curpos;			/* Current cursor position */
	uint16_t cursave;			/* Saved cursor position */
	uint16_t row;				/* This is the row that we are editing in */
	uint16_t coloffs;			/* Left cursor offset */
	uint16_t linelen;			/* Size of the line buffer */
	uint16_t nchars;			/* Size of data in the line buffer */
	int infd;					/* Input file descriptor */
	int outfd;					/* Output file descriptor */
	FAR char *line;				/* Line buffer */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#if !defined(CONFIG_CPP_HAVE_VARARGS) && CONFIG_SYSTEM_CLE_DEBUGLEVEL > 0
static int cle_debug(FAR const char *fmt, ...);
#endif

/* Low-level display and data entry functions */

static void cle_write(FAR struct cle_s *priv, FAR const char *buffer, uint16_t buflen);
static void cle_putch(FAR struct cle_s *priv, char ch);
static int  cle_getch(FAR struct cle_s *priv);
static void cle_cursoron(FAR struct cle_s *priv);
static void cle_cursoroff(FAR struct cle_s *priv);
static void cle_setcursor(FAR struct cle_s *priv, uint16_t column);
static int  cle_getcursor(FAR struct cle_s *priv, uint16_t *prow, uint16_t *pcolumn);
static void cle_clrtoeol(FAR struct cle_s *priv);

/* Editor function */

static bool cle_opentext(FAR struct cle_s *priv, uint16_t pos, uint16_t increment);
static void cle_closetext(FAR struct cle_s *priv, uint16_t pos, uint16_t size);
static void cle_showtext(FAR struct cle_s *priv);
static void cle_insertch(FAR struct cle_s *priv, char ch);
static int  cle_editloop(FAR struct cle_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* VT100 escape sequences */

static const char g_cursoron[]     = VT100_CURSORON;
static const char g_cursoroff[]    = VT100_CURSOROFF;
static const char g_getcursor[]    = VT100_GETCURSOR;
static const char g_erasetoeol[]   = VT100_CLEAREOL;
static const char g_fmtcursorpos[] = VT100_FMT_CURSORPOS;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cle_debug
 *
 * Description:
 *   Print a debug message to the syslog
 *
 ****************************************************************************/

#if !defined(CONFIG_CPP_HAVE_VARARGS) && CONFIG_SYSTEM_CLE_DEBUGLEVEL > 0
static int cle_debug(FAR const char *fmt, ...)
{
	va_list ap;
	int ret;

	/* Let vsyslog do the real work */

	va_start(ap, fmt);
	ret = vsyslog(LOG_DEBUG, fmt, ap);
	va_end(ap);
	return ret;
}
#endif

/****************************************************************************
 * Name: cle_write
 *
 * Description:
 *   Write a sequence of bytes to the console output device.
 *
 ****************************************************************************/

static void cle_write(FAR struct cle_s *priv, FAR const char *buffer, uint16_t buflen)
{
	ssize_t nwritten;
	uint16_t nremaining = buflen;

	//clevdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Loop until all bytes have been successfully written (or until a
	 * unrecoverable error is encountered)
	 */

	do {
		/* Take the next gulp */

		nwritten = write(priv->outfd, buffer, buflen);

		/* Handle write errors.  write() should neve return 0. */

		if (nwritten <= 0) {
			/* EINTR is not really an error; it simply means that a signal was
			 * received while waiting for write.
			 */

			int errcode = errno;
			if (nwritten == 0 || errcode != EINTR) {
				cledbg("ERROR: write to stdout failed: %d\n", errcode);
				return;
			}
		}

		/* Decrement the count of bytes remaining to be sent (to handle the
		 * case of a partial write)
		 */

		else {
			nremaining -= nwritten;
		}
	} while (nremaining > 0);
}

/****************************************************************************
 * Name: cle_putch
 *
 * Description:
 *   Write a single character to the console output device.
 *
 ****************************************************************************/

static void cle_putch(FAR struct cle_s *priv, char ch)
{
	cle_write(priv, &ch, 1);
}

/****************************************************************************
 * Name: cle_getch
 *
 * Description:
 *   Get a single character from the console input device.
 *
 ****************************************************************************/

static int cle_getch(FAR struct cle_s *priv)
{
	char buffer;
	ssize_t nread;

	/* Loop until we successfully read a character (or until an unexpected
	 * error occurs).
	 */

	do {
		/* Read one character from the incoming stream */

		nread = read(priv->infd, &buffer, 1);

		/* Check for error or end-of-file. */

		if (nread <= 0) {
			/* EINTR is not really an error; it simply means that a signal we
			 * received while waiting for input.
			 */

			int errcode = errno;
			if (nread == 0 || errcode != EINTR) {
				cledbg("ERROR: read from stdin failed: %d\n", errcode);
				return -EIO;
			}
		}
	} while (nread < 1);

	/* On success, return the character that was read */

	clevdbg("Returning: %c[%02x]\n", isprint(buffer) ? buffer : '.', buffer);
	return buffer;
}

/****************************************************************************
 * Name: cle_cursoron
 *
 * Description:
 *   Turn on the cursor
 *
 ****************************************************************************/

static void cle_cursoron(FAR struct cle_s *priv)
{
	/* Send the VT100 CURSORON command */

	cle_write(priv, g_cursoron, sizeof(g_cursoron));
}

/****************************************************************************
 * Name: cle_cursoroff
 *
 * Description:
 *   Turn off the cursor
 *
 ****************************************************************************/

static void cle_cursoroff(FAR struct cle_s *priv)
{
	/* Send the VT100 CURSOROFF command */

	cle_write(priv, g_cursoroff, sizeof(g_cursoroff));
}

/****************************************************************************
 * Name: cle_setcursor
 *
 * Description:
 *   Move the current cursor position to position (row,col)
 *
 ****************************************************************************/

static void cle_setcursor(FAR struct cle_s *priv, uint16_t column)
{
	char buffer[16];
	int len;

	clevdbg("row=%d column=%d offset=%d\n", priv->row, column, priv->coloffs);

	/* Format the cursor position command.  The origin is (1,1). */

	len = snprintf(buffer, 16, g_fmtcursorpos, priv->row, column + priv->coloffs);

	/* Send the VT100 CURSORPOS command */

	cle_write(priv, buffer, len);
}

/****************************************************************************
 * Name: cle_getcursor
 *
 * Description:
 *   Get the current cursor position.
 *
 ****************************************************************************/

static int cle_getcursor(FAR struct cle_s *priv, FAR uint16_t *prow, FAR uint16_t *pcolumn)
{
	uint32_t row;
	uint32_t column;
	int nbad;
	int ch;

	/* Send the VT100 GETCURSOR command */

	cle_write(priv, g_getcursor, sizeof(g_getcursor));

	/* We expect to get ESC[v;hR where v is the row and h is the column */

	nbad = 0;
	for (;;) {
		/* Get the next character from the input */

		ch = cle_getch(priv);
		if (ch == ASCII_ESC) {
			break;
		} else if (ch < 0) {
			return -EIO;
		} else if (++nbad > 3) {
			/* We are probably talking to a non-VT100 terminal! */

			return -EINVAL;
		}
	}

	/* Have ESC, now we expect '[' */

	nbad = 0;
	for (;;) {
		/* Get the next character from the input */

		ch = cle_getch(priv);
		if (ch == '[') {
			break;
		} else if (ch < 0) {
			return -EIO;
		} else {
			/* We are probably talking to a non-VT100 terminal! */

			return -EINVAL;
		}
	}

	/* Have ESC'['.  Now we expect to see a numeric value terminated with ';' */

	row = 0;
	for (;;) {
		/* Get the next character from the input */

		ch = cle_getch(priv);
		if (isdigit(ch)) {
			row = 10 * row + (ch - '0');
		} else if (ch == ';') {
			break;
		} else if (ch < 0) {
			return -EIO;
		} else {
			return -EINVAL;
		}
	}

	/* Have ESC'['v';'.  Now we expect to see another numeric value terminated with 'R' */

	column = 0;
	for (;;) {
		/* Get the next character from the input */

		ch = cle_getch(priv);
		if (isdigit(ch)) {
			column = 10 * column + (ch - '0');
		} else if (ch == 'R') {
			break;
		} else if (ch < 0) {
			return -EIO;
		} else {
			return -EINVAL;
		}
	}

	clevdbg("row=%ld column=%ld\n", row, column);

	/* Make sure that the values are within range */

	if (row <= UINT16_MAX && column <= UINT16_MAX) {
		*prow = row;
		*pcolumn = column;
		return OK;
	}

	return -ERANGE;
}

/****************************************************************************
 * Name: cle_clrtoeol
 *
 * Description:
 *   Clear the display from the current cursor position to the end of the
 *   current line.
 *
 ****************************************************************************/

static void cle_clrtoeol(FAR struct cle_s *priv)
{
	/* Send the VT100 ERASETOEOL command */

	cle_write(priv, g_erasetoeol, sizeof(g_erasetoeol));
}

/****************************************************************************
 * Name: cle_opentext
 *
 * Description:
 *   Make space for new text of size 'increment' at the specified cursor
 *   position.  This function will not allow text grow beyond ('linelen' - 1)
 *   in size.
 *
 ****************************************************************************/

static bool cle_opentext(FAR struct cle_s *priv, uint16_t pos, uint16_t increment)
{
	int i;

	clevdbg("pos=%ld increment=%ld\n", (long)pos, (long)increment);

	/* Check if there is space in the line buffer to open up a region the size
	 * of 'increment'
	 */

	if (priv->nchars + increment >= priv->linelen) {
		CLE_BEL(priv);
		return false;
	}

	/* Move text to make space for new text of size 'increment' at the current
	 * cursor position
	 */

	for (i = priv->nchars - 1; i >= pos; i--) {
		priv->line[i + increment] = priv->line[i];
	}

	/* Adjust end of file position */

	priv->nchars += increment;
	return true;
}

/****************************************************************************
 * Name: cle_closetext
 *
 * Description:
 *   Delete a region in the line buffer by copying the end of the line buffer
 *   over the deleted region and adjusting the size of the region.
 *
 ****************************************************************************/

static void cle_closetext(FAR struct cle_s *priv, uint16_t pos, uint16_t size)
{
	int i;

	clevdbg("pos=%ld size=%ld\n", (long)pos, (long)size);

	/* Close up the gap to remove 'size' characters at 'pos' */

	for (i = pos + size; i < priv->nchars; i++) {
		priv->line[i - size] = priv->line[i];
	}

	/* Adjust sizes and positions */

	priv->nchars -= size;

	/* Check if the cursor position is beyond the deleted region */

	if (priv->curpos > pos + size) {
		/* Yes... just subtract the size of the deleted region */

		priv->curpos -= size;
	}

	/* What if the position is within the deleted region?  Set it to the
	 * beginning of the deleted region.
	 */

	else if (priv->curpos > pos) {
		priv->curpos = pos;
	}
}

/****************************************************************************
 * Name: cle_showtext
 *
 * Description:
 *   Update the display based on the last operation.  This function is
 *   called at the beginning of the editor loop.
 *
 ****************************************************************************/

static void cle_showtext(FAR struct cle_s *priv)
{
	uint16_t column;
	uint16_t tabcol;

	/* Turn off the cursor during the update. */

	cle_cursoroff(priv);

	/* Set the cursor position to the beginning of this row */

	cle_setcursor(priv, 0);
	cle_clrtoeol(priv);

	/* Loop for each column */

	for (column = 0; column < priv->nchars;) {
		/* Perform TAB expansion */

		if (priv->line[column] == '\t') {
			tabcol = NEXT_TAB(column);
			if (tabcol < priv->linelen) {
				for (; column < tabcol; column++) {
					cle_putch(priv, ' ');
				}
			} else {
				/* Break out of the loop... there is nothing left on the
				 * line but whitespace.
				 */

				break;
			}
		}

		/* Add the normal character to the display */

		else {
			cle_putch(priv, priv->line[column]);
			column++;
		}
	}

	/* Turn the cursor back on */

	cle_cursoron(priv);
}

/****************************************************************************
 * Name: cle_insertch
 *
 * Description:
 *   Insert one character into the line buffer
 *
 ****************************************************************************/

static void cle_insertch(FAR struct cle_s *priv, char ch)
{
	clevdbg("curpos=%ld ch=%c[%02x]\n", priv->curpos, isprint(ch) ? ch : '.', ch);

	/* Make space in the buffer for the new character */

	if (cle_opentext(priv, priv->curpos, 1)) {
		/* Add the new character to the buffer */

		priv->line[priv->curpos++] = ch;
	}
}

/****************************************************************************
 * Name: cle_editloop
 *
 * Description:
 *   Command line editor loop
 *
 ****************************************************************************/

static int cle_editloop(FAR struct cle_s *priv)
{
	/* Loop while we are in command mode */

	for (;;) {
		int ch;

		/* Make sure that the display reflects the current state */

		cle_showtext(priv);
		cle_setcursor(priv, priv->curpos);

		/* Get the next character from the input */

		ch = cle_getch(priv);
		if (ch < 0) {
			return -EIO;
		}

		/* Then handle the character. */

		switch (ch) {
		case KEY_BEGINLINE: {	/* Move cursor to start of current line */
			priv->curpos = 0;
		}
		break;

		case KEY_LEFT: {		/* Move the cursor left 1 character */
			if (priv->curpos > 0) {
				priv->curpos--;
			} else {
				CLE_BEL(priv);
			}
		}
		break;

		case KEY_DEL:			/* Delete 1 character at the cursor */
		case ASCII_DEL: {
			if (priv->curpos < priv->nchars) {
				cle_closetext(priv, priv->curpos, 1);
			} else {
				CLE_BEL(priv);
			}
		}
		break;

		case KEY_ENDLINE: {	/* Move cursor to end of current line */
			priv->curpos = priv->nchars;
		}
		break;

		case KEY_RIGHT: {	/* Move the cursor right one character */
			if (priv->curpos < priv->nchars) {
				priv->curpos++;
			} else {
				CLE_BEL(priv);
			}
		}
		break;

		case KEY_DELLEFT:		/* Delete 1 character before the cursor */
			//case ASCII_BS:
		{
			if (priv->curpos > 0) {
				cle_closetext(priv, --priv->curpos, 1);
			} else {
				CLE_BEL(priv);
			}
		}
		break;

		case KEY_DELEOL: {	/* Delete to the end of the line */
			priv->nchars = (priv->nchars > 0 ? priv->curpos + 1 : 0);
		}
		break;

		case KEY_DELLINE: {	/* Delete to the end of the line */
			priv->nchars = 0;
			priv->curpos = 0;
		}
		break;

		case KEY_QUOTE: {	/* Quoted character follows */
			ch = cle_getch(priv);
			if (ch < 0) {
				return -EIO;
			}

			/* Insert the next character unconditionally */

			cle_insertch(priv, ch);
		}
		break;

			/* Newline terminates editing.  But what is a newline? */

#if defined(CONFIG_EOL_IS_CR) || defined(CONFIG_EOL_IS_EITHER_CRLF)
		case '\r':				/* CR terminates line */
#elif defined(CONFIG_EOL_IS_LF) || defined(CONFIG_EOL_IS_BOTH_CRLF) || defined(CONFIG_EOL_IS_EITHER_CRLF)
		case '\n':				/* LF terminates line */
#endif
		{
			/* Add the newline character to the buffer at the end of the line */

			priv->curpos = priv->nchars;
			cle_insertch(priv, '\n');
			cle_putch(priv, '\n');
			return OK;
		}
		break;

#if defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\r':				/* Wait for the LF */
			break;
#endif

		/* Text to insert or unimplemented/invalid keypresses */

		default: {
			/* Ignore all control characters except for tab and newline */

			if (!iscntrl(ch) || ch == '\t') {
				/* Insert the filtered character into the buffer */

				cle_insertch(priv, ch);
			} else {
				CLE_BEL(priv);
			}
		}
		break;
		}
	}

	return OK;
}

/****************************************************************************
 * Command line processing
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cle
 *
 * Description:
 *   EMACS-like command line editor.  This is actually more like readline
 *   than is the TinyAra readline!
 *
 ****************************************************************************/

int cle(FAR char *line, uint16_t linelen, FILE *instream, FILE *outstream)
{
	FAR struct cle_s priv;
	uint16_t column;
	int ret;

	/* Initialize the CLE state structure */

	memset(&priv, 0, sizeof(struct cle_s));

	priv.linelen = linelen;
	priv.line = line;

	/* REVISIT:  Non-standard, non-portable */

	priv.infd = instream->fs_fd;
	priv.outfd = outstream->fs_fd;

	/* Get the current cursor position */

	ret = cle_getcursor(&priv, &priv.row, &column);
	if (ret < 0) {
		return ret;
	}

	/* Turn the column number into an offset */

	if (column < 1) {
		return -EINVAL;
	}

	priv.coloffs = column - 1;

	clevdbg("row=%d column=%d\n", priv.row, column);

	/* The editor loop */

	ret = cle_editloop(&priv);

	/* Make sure that the line is NUL terminated */

	line[priv.nchars] = '\0';
	return ret;
}
