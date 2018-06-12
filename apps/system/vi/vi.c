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
 * apps/system/vi/vi.c
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

#include <sys/stat.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <syslog.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/ascii.h>
#include <tinyara/vt100.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_SYSTEM_VI_ROWS
#define CONFIG_SYSTEM_VI_ROWS 16
#endif

#ifndef CONFIG_SYSTEM_VI_COLS
#define CONFIG_SYSTEM_VI_COLS 64
#endif

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

#define VI_BEL(vi)      vi_putch(vi, CTRL('G'))

/* Sizes of things */

#define MAX_STRING      64		/* The maximum size of a filename or search string */
#define SCRATCH_BUFSIZE 128		/* The maximum size of the scratch buffer */

#define TEXT_GULP_SIZE  512		/* Text buffer allocations are managed with this unit */
#define TEXT_GULP_MASK  511		/* Mask for aligning buffer allocation sizes */
#define ALIGN_GULP(x)   (((x) + TEXT_GULP_MASK) & ~TEXT_GULP_MASK)

#define TABSIZE         8		/* A TAB is eight characters */
#define TABMASK         7		/* Mask for TAB alignment */
#define NEXT_TAB(p)     (((p) + TABSIZE) & ~TABMASK)

/* Parsed command action bits */

#define CMD_READ        (1 << 0)	/* Bit 0: Read */
#define CMD_WRITE_MASK  (3 << 1)	/* Bits 1-2: x1=Write operation */
#define CMD_WRITE       (1 << 1)	/*           01=Write (without overwriting) */
#define CMD_OWRITE      (3 << 1)	/*           11=Overwrite */
#define CMD_QUIT_MASK   (3 << 3)	/* Bits 3-4: x1=Quite operation */
#define CMD_QUIT        (1 << 3)	/*           01=Quit if saved */
#define CMD_DISCARD     (3 << 3)	/*           11=Quit without saving */

#define CMD_NONE        (0)		/* No command */
#define CMD_WRITE_QUIT  (CMD_WRITE | CMD_QUIT)	/* Write file and quit command */
#define CMD_OWRITE_QUIT (CMD_OWRITE | CMD_QUIT)	/* Overwrite file and quit command */

#define IS_READ(a)      (((uint8_t)(a) & CMD_READ) != 0)
#define IS_WRITE(a)     (((uint8_t)(a) & CMD_WRITE) != 0)
#define IS_OWRITE(a)    (((uint8_t)(a) & CMD_WRITE_MASK) == CMD_OWRITE)
#define IS_NOWRITE(a)   (((uint8_t)(a) & CMD_WRITE_MASK) == CMD_WRITE)
#define IS_QUIT(a)      (((uint8_t)(a) & CMD_QUIT) != 0)
#define IS_DISCARD(a)   (((uint8_t)(a) & CMD_QUIT_MASK) == CMD_DISCARD)
#define IS_NDISCARD(a)  (((uint8_t)(a) & CMD_QUIT_MASK) == CMD_QUIT)

#define CMD_FILE_MASK   (CMD_READ | CMD_WRITE)
#define USES_FILE(a)    (((uint8_t)(a) & CMD_FILE_MASK) != 0)

/* Debug */

#ifndef CONFIG_SYSTEM_VI_DEBUGLEVEL
#define CONFIG_SYSTEM_VI_DEBUGLEVEL 0
#endif

#ifdef CONFIG_CPP_HAVE_VARARGS
#if CONFIG_SYSTEM_VI_DEBUGLEVEL > 0
#define vidbg(format, ...) syslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#define vvidbg(format, ap) vsyslog(LOG_DEBUG, format, ap)
#else
#define vidbg(...)
#define vvidbg(...)
#endif

#if CONFIG_SYSTEM_VI_DEBUGLEVEL > 1
#define vivdbg(format, ...) syslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define vivdbg(...)
#endif
#else
#if CONFIG_SYSTEM_VI_DEBUGLEVEL > 0
#define vidbg  vi_debug
#define vvidbg vi_vdebug
#else
#define vidbg  (void)
#define vvidbg (void)
#endif

#if CONFIG_SYSTEM_VI_DEBUGLEVEL > 1
#define vivdbg vi_debug
#else
#define vivdbg (void)
#endif
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* VI Key Bindings */

enum vi_cmdmode_key_e {
	KEY_CMDMODE_BEGINLINE = '0',	/* Move cursor to start of current line */
	KEY_CMDMODE_APPEND    = 'a',	/* Enter insertion mode after current character */
	KEY_CMDMODE_DEL_LINE  = 'd',	/* "dd" deletes a lines */
	KEY_CMDMODE_LEFT      = 'h',	/* Move left one character */
	KEY_CMDMODE_INSERT    = 'i',	/* Enter insertion mode before current character */
	KEY_CMDMODE_DOWN      = 'j',	/* Move down one line */
	KEY_CMDMODE_UP        = 'k',	/* Move up one line */
	KEY_CMDMODE_RIGHT     = 'l',	/* Move right one character */
	KEY_CMDMODE_MARK      = 'm',	/* Place a mark beginning at the current cursor position */
	KEY_CMDMODE_OPENBELOW = 'o',	/* Enter insertion mode in new line below current */
	KEY_CMDMODE_PASTE     = 'p',	/* Paste line(s) from into text after current line */
	KEY_CMDMODE_REPLACECH = 'r',	/* Replace character(s) under cursor */
	KEY_CMDMODE_YANK      = 'y',	/* "yy" yanks the current line(s) into the buffer */
	KEY_CMDMODE_DEL       = 'x',	/* Delete a single character */

	KEY_CMDMODE_APPENDEND = 'A',	/* Enter insertion mode at the end of the current line */
	KEY_CMDMODE_GOTO      = 'G',	/* Got to line */
	KEY_CMDMODE_INSBEGIN  = 'I',	/* Enter insertion mode at the beginning of the current */
	KEY_CMDMODE_OPENABOVE = 'O',	/* Enter insertion mode in new line above current line */
	KEY_CMDMODE_REPLACE   = 'R',	/* Replace character(s) under cursor until ESC */

	KEY_CMDMODE_COLONMODE = ':',	/* The next character command prefaced with a colon */
	KEY_CMDMODE_FINDMODE  = '/',	/* Enter forward search string */
	KEY_CMDMODE_ENDLINE   = '$',	/* Move cursor to end of current line */

	KEY_CMDMODE_PAGEUP    = CTRL('b'),	/* Move backward one screen */
	KEY_CMDMODE_HALFDOWN  = CTRL('d'),	/* Move down (forward) one half screen */
	KEY_CMDMODE_PAGEDOWN  = CTRL('f'),	/* Move forward one screen */
	KEY_CMDMODE_REDRAW    = CTRL('l'),	/* Redraws the screen */
	KEY_CMDMODE_REDRAW2   = CTRL('r'),	/* Redraws the screen, removing deleted lines */
	KEY_CMDMODE_HALFUP    = CTRL('u')	/* Move up (back) one half screen */
};

enum vi_insmode_key_e {
	KEY_INSMODE_QUOTE = '\\',	/* The next character is quote (use literal value) */
};

enum vi_colonmode_key_e {
	KEY_COLMODE_READ  = 'r',	/* Read file */
	KEY_COLMODE_QUIT  = 'q',	/* Quit vi */
	KEY_COLMODE_WRITE = 'w',	/* Write file */
	KEY_COLMODE_FORCE = '!',	/* Force operation */
	KEY_COLMODE_QUOTE = '\\'	/* The next character is quote (use literal value) */
};

enum vi_findmode_key_e {
	KEY_FINDMODE_QUOTE = '\\'	/* The next character is quote (use literal value) */
};

/* VI modes */

enum vi_mode_s {
	MODE_COMMAND = 0,			/* ESC         Command mode */
	SUBMODE_COLON,				/* :           Command sub-mode */
	SUBMODE_FIND,				/* /           Search sub-mode */
	SUBMODE_REPLACECH,			/* r           Replace sub-mode 1 */
	MODE_INSERT,				/* i,I,a,A,o,O Insert mode */
	MODE_REPLACE				/* R           Replace sub-mode 2 */
};

/* This structure represents a cursor position */

struct vi_pos_s {
	uint16_t row;
	uint16_t column;
};

/* This structure describes the overall state of the editor */

struct vi_s {
	struct vi_pos_s cursor;		/* Current cursor position */
	struct vi_pos_s cursave;	/* Saved cursor position */
	struct vi_pos_s display;	/* Display size */
	off_t curpos;				/* The current cursor offset into the text buffer */
	off_t textsize;				/* The size of the text buffer */
	off_t winpos;				/* Offset corresponding to the start of the display */
	off_t prevpos;				/* Previous display position */
	uint16_t hscroll;			/* Horizontal display offset */
	uint16_t value;				/* Numeric value entered prior to a command */
	uint8_t mode;				/* See enum vi_mode_s */
	uint8_t cmdlen;				/* Length of the command in the scratch[] buffer */
	bool modified;				/* True: The file has modified */
	bool error;					/* True: There is an error message on the last line */
	bool delarm;				/* One more 'd' and the line(s) will be deleted */
	bool yankarm;				/* One more 'y' and the line(s) will be yanked */

	/* Buffers */

	FAR char *text;				/* Dynamically allocated text buffer */
	size_t txtalloc;			/* Current allocated size of the text buffer */
	FAR char *yank;				/* Dynamically allocated yank buffer */
	size_t yankalloc;			/* Current allocated size of the yank buffer */

	char filename[MAX_STRING];	/* Holds the currently selected filename */
	char findstr[MAX_STRING];	/* Holds the current search string */
	char scratch[SCRATCH_BUFSIZE];	/* For general, scratch usage */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Low-level display and data entry functions */

static void vi_write(FAR struct vi_s *vi, FAR const char *buffer, size_t buflen);
static void vi_putch(FAR struct vi_s *vi, char ch);
static char vi_getch(FAR struct vi_s *vi);
#if 0							/* Not used */
static void vi_blinkon(FAR struct vi_s *vi);
#endif
static void vi_boldon(FAR struct vi_s *vi);
static void vi_reverseon(FAR struct vi_s *vi);
static void vi_attriboff(FAR struct vi_s *vi);
static void vi_cursoron(FAR struct vi_s *vi);
static void vi_cursoroff(FAR struct vi_s *vi);
#if 0							/* Not used */
static void vi_cursorhome(FAR struct vi_s *vi);
#endif
static void vi_setcursor(FAR struct vi_s *vi, uint16_t row, uint16_t column);
static void vi_clrtoeol(FAR struct vi_s *vi);
#if 0							/* Not used */
static void vi_clrscreen(FAR struct vi_s *vi);
#endif

/* Error display */

static void vi_error(FAR struct vi_s *vi, FAR const char *fmt, ...);

/* Line positioning */

static off_t vi_linebegin(FAR struct vi_s *vi, off_t pos);
static off_t vi_prevline(FAR struct vi_s *vi, off_t pos);
static off_t vi_lineend(FAR struct vi_s *vi, off_t pos);
static off_t vi_nextline(FAR struct vi_s *vi, off_t pos);

/* Text buffer management */

static bool vi_extendtext(FAR struct vi_s *vi, off_t pos, size_t increment);
static void vi_shrinkpos(off_t delpos, size_t delsize, FAR off_t *pos);
static void vi_shrinktext(FAR struct vi_s *vi, off_t pos, size_t size);

/* File access */

static bool vi_insertfile(FAR struct vi_s *vi, off_t pos, FAR const char *filename);
static bool vi_savetext(FAR struct vi_s *vi, FAR const char *filename, off_t pos, size_t size);
static bool vi_checkfile(FAR struct vi_s *vi, FAR const char *filename);

/* Mode management */

static void vi_setmode(FAR struct vi_s *vi, uint8_t mode, long value);
static void vi_setsubmode(FAR struct vi_s *vi, uint8_t mode, char prompt, long value);
static void vi_exitsubmode(FAR struct vi_s *vi, uint8_t mode);

/* Display management */

static void vi_windowpos(FAR struct vi_s *vi, off_t start, off_t end, uint16_t *pcolumn, off_t *ppos);
static void vi_scrollcheck(FAR struct vi_s *vi);
static void vi_showtext(FAR struct vi_s *vi);

/* Command mode */

static void vi_cusorup(FAR struct vi_s *vi, int nlines);
static void vi_cursordown(FAR struct vi_s *vi, int nlines);
static off_t vi_cursorleft(FAR struct vi_s *vi, off_t curpos, int ncolumns);
static off_t vi_cursorright(FAR struct vi_s *vi, off_t curpos, int ncolumns);
static void vi_delforward(FAR struct vi_s *vi);
static void vi_delbackward(FAR struct vi_s *vi);
static void vi_linerange(FAR struct vi_s *vi, off_t *start, off_t *end);
static void vi_delline(FAR struct vi_s *vi);
static void vi_yank(FAR struct vi_s *vi);
static void vi_paste(FAR struct vi_s *vi);
static void vi_gotoline(FAR struct vi_s *vi);
static void vi_cmd_mode(FAR struct vi_s *vi);

/* Command sub-modes */

static void vi_cmdch(FAR struct vi_s *vi, char ch);
static void vi_cmdbackspace(FAR struct vi_s *vi);

static void vi_parsecolon(FAR struct vi_s *vi);
static void vi_cmd_submode(FAR struct vi_s *vi);

static bool vi_findstring(FAR struct vi_s *vi);
static void vi_parsefind(FAR struct vi_s *vi);
static void vi_find_submode(FAR struct vi_s *vi);

static void vi_replacech(FAR struct vi_s *vi, char ch);
static void vi_replacech_submode(FAR struct vi_s *vi);

/* Insert and replace modes */

static void vi_insertch(FAR struct vi_s *vi, char ch);
static void vi_insert_mode(FAR struct vi_s *vi);

static void vi_replace_mode(FAR struct vi_s *vi);

/* Command line processing */

static void vi_release(FAR struct vi_s *vi);
static void vi_showusage(FAR struct vi_s *vi, FAR const char *progname, int exitcode);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* VT100 escape sequences */

static const char g_cursoron[]   = VT100_CURSORON;
static const char g_cursoroff[]  = VT100_CURSOROFF;
#if 0							/* Not used */
static const char g_cursorhome[] = VT100_CURSORHOME;
#endif
static const char g_erasetoeol[] = VT100_CLEAREOL;
#if 0							/* Not used */
static const char g_clrscreen[]  = VT100_CLEARSCREEN;
#endif
static const char g_index[]      = VT100_INDEX;
static const char g_revindex[]   = VT100_REVINDEX;
static const char g_attriboff[]  = VT100_MODESOFF;
static const char g_boldon[]     = VT100_BOLD;
static const char g_reverseon[]  = VT100_REVERSE;
#if 0							/* Not used */
static const char g_blinkon[]    = VT100_BLINK;
static const char g_boldoff[]    = VT100_BOLDOFF;
static const char g_reverseoff[] = VT100_REVERSEOFF;
static const char g_blinkoff[]   = VT100_BLINKOFF;
#endif

static const char g_fmtcursorpos[] = VT100_FMT_CURSORPOS;

/* Error format strings */

static const char g_fmtallocfail[]  = "Failed to allocate memory";
static const char g_fmtcmdfail[]    = "%s failed: %d";
static const char g_fmtnotfile[]    = "%s is not a regular file";
static const char g_fmtfileexists[] = "File exists (add ! to override)";
static const char g_fmtmodified[]   = "No write since last change (add ! to override)";
static const char g_fmtnotvalid[]   = "Command not valid";
static const char g_fmtnotcmd[]     = "Not an editor command: %s";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_vdebug and vi_debug
 *
 * Description:
 *   Print a debug message to the syslog
 *
 ****************************************************************************/

#if !defined(CONFIG_CPP_HAVE_VARARGS) && CONFIG_SYSTEM_VI_DEBUGLEVEL > 0
static inline int vi_vdebug(FAR const char *fmt, va_list ap)
{
	return vsyslog(LOG_DEBUG, fmt, ap);
}

static int vi_debug(FAR const char *fmt, ...)
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
 * Low-level display and data entry functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_write
 *
 * Description:
 *   Write a sequence of bytes to the console device (stdout, fd = 1).
 *
 ****************************************************************************/

static void vi_write(FAR struct vi_s *vi, FAR const char *buffer, size_t buflen)
{
	ssize_t nwritten;
	size_t nremaining = buflen;

	//vivdbg("buffer=%p buflen=%d\n", buffer, (int)buflen);

	/* Loop until all bytes have been successfully written (or until a
	 * unrecoverable error is encountered)
	 */

	do {
		/* Take the next gulp */

		nwritten = write(1, buffer, buflen);

		/* Handle write errors.  write() should neve return 0. */

		if (nwritten <= 0) {
			/* EINTR is not really an error; it simply means that a signal was
			 * received while waiting for write.
			 */

			int errcode = errno;
			if (nwritten == 0 || errcode != EINTR) {
				fprintf(stderr, "ERROR: write to stdout failed: %d\n", errcode);
				exit(EXIT_FAILURE);
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
 * Name: vi_putch
 *
 * Description:
 *   Write a single character to the console device.
 *
 ****************************************************************************/

static void vi_putch(FAR struct vi_s *vi, char ch)
{
	vi_write(vi, &ch, 1);
}

/****************************************************************************
 * Name: vi_getch
 *
 * Description:
 *   Get a single character from the console device (stdin, fd = 0).
 *
 ****************************************************************************/

static char vi_getch(FAR struct vi_s *vi)
{
	char buffer;
	ssize_t nread;

	/* Loop until we successfully read a character (or until an unexpected
	 * error occurs).
	 */

	do {
		/* Read one character from the incoming stream */

		nread = read(0, &buffer, 1);

		/* Check for error or end-of-file. */

		if (nread <= 0) {
			/* EINTR is not really an error; it simply means that a signal we
			 * received while waiting for input.
			 */

			int errcode = errno;
			if (nread == 0 || errcode != EINTR) {
				fprintf(stderr, "ERROR: read from stdin failed: %d\n", errcode);
				exit(EXIT_FAILURE);
			}
		}
	} while (nread < 1);

	/* On success, return the character that was read */

	vivdbg("Returning: %c[%02x]\n", isprint(buffer) ? buffer : '.', buffer);
	return buffer;
}

/****************************************************************************
 * Name: vi_blinkon
 *
 * Description:
 *   Enable the blinking attribute at the current cursor location
 *
 ****************************************************************************/

#if 0							/* Not used */
static void vi_blinkon(FAR struct vi_s *vi)
{
	/* Send the VT100 BLINKON command */

	vi_write(vi, g_blinkon, sizeof(g_blinkon));
}
#endif

/****************************************************************************
 * Name: vi_boldon
 *
 * Description:
 *   Enable the blinking attribute at the current cursor location
 *
 ****************************************************************************/

static void vi_boldon(FAR struct vi_s *vi)
{
	/* Send the VT100 BOLDON command */

	vi_write(vi, g_boldon, sizeof(g_boldon));
}

/****************************************************************************
 * Name: vi_reverseon
 *
 * Description:
 *   Enable the blinking attribute at the current cursor location
 *
 ****************************************************************************/

static void vi_reverseon(FAR struct vi_s *vi)
{
	/* Send the VT100 REVERSON command */

	vi_write(vi, g_reverseon, sizeof(g_reverseon));
}

/****************************************************************************
 * Name:
 *
 * Description:
 *   Disable all previously selected attributes.
 *
 ****************************************************************************/

static void vi_attriboff(FAR struct vi_s *vi)
{
	/* Send the VT100 ATTRIBOFF command */

	vi_write(vi, g_attriboff, sizeof(g_attriboff));
}

/****************************************************************************
 * Name: vi_cursoron
 *
 * Description:
 *   Turn on the cursor
 *
 ****************************************************************************/

static void vi_cursoron(FAR struct vi_s *vi)
{
	/* Send the VT100 CURSORON command */

	vi_write(vi, g_cursoron, sizeof(g_cursoron));
}

/****************************************************************************
 * Name: vi_cursoroff
 *
 * Description:
 *   Turn off the cursor
 *
 ****************************************************************************/

static void vi_cursoroff(FAR struct vi_s *vi)
{
	/* Send the VT100 CURSOROFF command */

	vi_write(vi, g_cursoroff, sizeof(g_cursoroff));
}

/****************************************************************************
 * Name: vi_cursorhome
 *
 * Description:
 *   Move the current cursor to the upper left hand corner of the display
 *
 ****************************************************************************/

#if 0							/* Not used */
static void vi_cursorhome(FAR struct vi_s *vi)
{
	/* Send the VT100 CURSORHOME command */

	vi_write(vi, g_cursorhome, sizeof(g_cursorhome));
}
#endif

/****************************************************************************
 * Name: vi_setcursor
 *
 * Description:
 *   Move the current cursor position to position (row,col)
 *
 ****************************************************************************/

static void vi_setcursor(FAR struct vi_s *vi, uint16_t row, uint16_t column)
{
	char buffer[16];
	int len;

	vivdbg("row=%d column=%d\n", row, column);

	/* Format the cursor position command.  The origin is (1,1). */

	len = snprintf(buffer, 16, g_fmtcursorpos, row + 1, column + 1);

	/* Send the VT100 CURSORPOS command */

	vi_write(vi, buffer, len);
}

/****************************************************************************
 * Name: vi_clrtoeol
 *
 * Description:
 *   Clear the display from the current cursor position to the end of the
 *   current line.
 *
 ****************************************************************************/

static void vi_clrtoeol(FAR struct vi_s *vi)
{
	/* Send the VT100 ERASETOEOL command */

	vi_write(vi, g_erasetoeol, sizeof(g_erasetoeol));
}

/****************************************************************************
 * Name: vi_clrscreen
 *
 * Description:
 *   Clear the entire display
 *
 ****************************************************************************/

#if 0							/* Not used */
static void vi_clrscreen(FAR struct vi_s *vi)
{
	/* Send the VT100 CLRSCREEN command */

	vi_write(vi, g_clrscreen, sizeof(g_clrscreen));
}
#endif

/****************************************************************************
 * Name: vi_scrollup
 *
 * Description:
 *   Scroll the display up 'nlines' by sending the VT100 INDEX command.
 *
 ****************************************************************************/

static void vi_scrollup(FAR struct vi_s *vi, uint16_t nlines)
{
	vivdbg("nlines=%d\n", nlines);

	/* Scroll for the specified number of lines */

	for (; nlines; nlines--) {
		/* Send the VT100 INDEX command */

		vi_write(vi, g_index, sizeof(g_index));
	}
}

/****************************************************************************
 * Name: vi_scrolldown
 *
 * Description:
 *   Scroll the display down 'nlines' by sending the VT100 REVINDEX command.
 *
 ****************************************************************************/

static void vi_scrolldown(FAR struct vi_s *vi, uint16_t nlines)
{
	vivdbg("nlines=%d\n", nlines);

	/* Scroll for the specified number of lines */

	for (; nlines; nlines--) {
		/* Send the VT100 REVINDEX command */

		vi_write(vi, g_revindex, sizeof(g_revindex));
	}
}

/****************************************************************************
 * Name: vi_error
 *
 * Description:
 *   Show a highlighted error message at the final line of the display.
 *
 ****************************************************************************/

static void vi_error(FAR struct vi_s *vi, FAR const char *fmt, ...)
{
	struct vi_pos_s cursor;
	va_list ap;
	int len;

	/* Save the current cursor position */

	cursor.row = vi->cursor.row;
	cursor.column = vi->cursor.column;

	/* Set up for a reverse text message on the final line */

	vi_setcursor(vi, vi->display.row - 1, 0);
	vi_reverseon(vi);

	/* Expand the error message in the scratch buffer */

	len = snprintf(vi->scratch, SCRATCH_BUFSIZE, "ERROR: ");

	va_start(ap, fmt);
	len += vsnprintf(vi->scratch + len, SCRATCH_BUFSIZE - len, fmt, ap);
	vvidbg(fmt, ap);
	va_end(ap);

	/* Write the error message to the display in reverse text */

	vi_write(vi, vi->scratch, len);

	/* Restore normal attributes */

	vi_attriboff(vi);

	/* Reposition the cursor */

	vi_setcursor(vi, cursor.row, cursor.column);

	/* Remember that there is an error message on the last line of the display.
	 * When the display is refreshed, the last line will not be altered until
	 * the error is cleared.
	 */

	vi->error = true;
	VI_BEL(vi);
}

/****************************************************************************
 * Line positioning
 ****************************************************************************/

/****************************************************************************
 * Name: vi_linebegin
 *
 * Description:
 *   Search backward for the beginning of the current line
 *
 ****************************************************************************/

static off_t vi_linebegin(FAR struct vi_s *vi, off_t pos)
{
	/* Search backward to find the previous newline character (or, possibly,
	 * the beginning of the text buffer).
	 */

	while (pos && vi->text[pos - 1] != '\n') {
		pos--;
	}

	vivdbg("Return pos=%ld\n", (long)pos);
	return pos;
}

/****************************************************************************
 * Name: vi_prevline
 *
 * Description:
 *   Search backward for the beginning of the previous line
 *
 ****************************************************************************/

static off_t vi_prevline(FAR struct vi_s *vi, off_t pos)
{
	/* Find the beginning the of current line */

	pos = vi_linebegin(vi, pos);

	/* If this not the first line, then back up one more character to position
	 * at the last byte of the previous line.
	 */

	if (pos > 0) {
		pos = vi_linebegin(vi, pos - 1);
	}

	vivdbg("Return pos=%ld\n", (long)pos);
	return pos;
}

/****************************************************************************
 * Name: vi_lineend
 *
 * Description:
 *   Search forward for the end of the current line
 *
 ****************************************************************************/

static off_t vi_lineend(FAR struct vi_s *vi, off_t pos)
{
	/* Search forward to find the next newline character. (or, possibly,
	 * the end of the text buffer).
	 */

	while (pos < vi->textsize && vi->text[pos] != '\n') {
		pos++;
	}

	vivdbg("Return pos=%ld\n", (long)pos);
	return pos;
}

/****************************************************************************
 * Name: vi_nextline
 *
 * Description:
 *   Search backward for the start of the next line
 *
 ****************************************************************************/

static off_t vi_nextline(FAR struct vi_s *vi, off_t pos)
{
	/* Position at the end of the current line */

	pos = vi_lineend(vi, pos);

	/* If this is not the last byte in the buffer, then increment by one
	 * for position of the first byte of the next line.
	 */

	if (pos < vi->textsize) {
		pos++;
	}

	vivdbg("Return pos=%ld\n", (long)pos);
	return pos;
}

/****************************************************************************
 * Text buffer management
 ****************************************************************************/

/****************************************************************************
 * Name: vi_extendtext
 *
 * Description:
 *   Reallocate the in-memory file memory by (at least) 'increment' and make
 *   space for new text of size 'increment' at the specified cursor position.
 *
 ****************************************************************************/

static bool vi_extendtext(FAR struct vi_s *vi, off_t pos, size_t increment)
{
	FAR char *alloc;
	int i;

	vivdbg("pos=%ld increment=%ld\n", (long)pos, (long)increment);

	/* Check if we need to reallocate */

	if (!vi->text || vi->textsize + increment > vi->txtalloc) {
		/* Allocate in chunksize so that we do not have to reallocate so
		 * often.
		 */

		size_t allocsize = ALIGN_GULP(vi->textsize + increment);
		alloc = realloc(vi->text, allocsize);
		if (!alloc) {
			/* Reallocation failed */

			vi_error(vi, g_fmtallocfail);
			return false;
		}

		/* Save the new buffer information */

		vi->text = alloc;
		vi->txtalloc = allocsize;
	}

	/* Move text to make space for new text of size 'increment' at the current
	 * cursor position
	 */

	for (i = vi->textsize - 1; i >= pos; i--) {
		vi->text[i + increment] = vi->text[i];
	}

	/* Adjust end of file position */

	vi->textsize += increment;
	vi->modified = true;
	return true;
}

/****************************************************************************
 * Name: vi_shrinkpos
 *
 * Description:
 *   This is really part of vi_shrinktext.  When any text is deleted, any
 *   positions lying beyond the deleted region in the text buffer must be
 *   adjusted.
 *
 * Input Parameters:
 *   delpos  The position where text was deleted
 *   delsize The number of bytes deleted.
 *   pos     A pointer to a position that may need to be adjusted.
 *
 ****************************************************************************/

static void vi_shrinkpos(off_t delpos, size_t delsize, FAR off_t *pos)
{
	vivdbg("delpos=%ld delsize=%ld pos=%ld\n", (long)delpos, (long)delsize, (long)*pos);

	/* Check if the position is beyond the deleted region */

	if (*pos > delpos + delsize) {
		/* Yes... just subtract the size of the deleted region */

		*pos -= delsize;
	}

	/* What if the position is within the deleted region?  Set it to the
	 * beginning of the deleted region.
	 */

	else if (*pos > delpos) {
		*pos = delpos;
	}
}

/****************************************************************************
 * Name: vi_shrinktext
 *
 * Description:
 *   Delete a region in the text buffer by copying the end of the text buffer
 *   over the deleted region and adjusting the size of the region.  The text
 *   region may be reallocated in order to recover the unused memory.
 *
 ****************************************************************************/

static void vi_shrinktext(FAR struct vi_s *vi, off_t pos, size_t size)
{
	FAR char *alloc;
	size_t allocsize;
	int i;

	vivdbg("pos=%ld size=%ld\n", (long)pos, (long)size);

	/* Close up the gap to remove 'size' characters at 'pos' */

	for (i = pos + size; i < vi->textsize; i++) {
		vi->text[i - size] = vi->text[i];
	}

	/* Adjust sizes and positions */

	vi->textsize -= size;
	vi->modified = true;
	vi_shrinkpos(pos, size, &vi->curpos);
	vi_shrinkpos(pos, size, &vi->winpos);
	vi_shrinkpos(pos, size, &vi->prevpos);

	/* Reallocate the buffer to free up memory no longer in use */

	allocsize = ALIGN_GULP(vi->textsize);
	if (allocsize < vi->txtalloc) {
		alloc = realloc(vi->text, allocsize);
		if (!alloc) {
			vi_error(vi, g_fmtallocfail);
			return;
		}

		/* Save the new buffer information */

		vi->text = alloc;
		vi->txtalloc = allocsize;
	}
}

/****************************************************************************
 * File access
 ****************************************************************************/

/****************************************************************************
 * Name: vi_insertfile
 *
 * Description:
 *   Insert the contents of a file into the text buffer
 *
 ****************************************************************************/

static bool vi_insertfile(FAR struct vi_s *vi, off_t pos, FAR const char *filename)
{
	struct stat buf;
	FILE *stream;
	off_t filesize;
	size_t nread;
	int result;
	bool ret;

	vivdbg("pos=%ld filename=\"%s\"\n", (long)pos, filename);

	/* Get the size of the file  */

	result = stat(filename, &buf);
	if (result < 0) {
		vi_error(vi, g_fmtcmdfail, "stat", errno);
		return false;
	}

	/* Check for zero-length file */

	filesize = buf.st_size;
	if (filesize < 1) {
		return false;
	}

	/* Open the file for reading */

	stream = fopen(filename, "r");
	if (!stream) {
		vi_error(vi, g_fmtcmdfail, "open", errno);
		return false;
	}

	/* [Re]allocate the text buffer to hold the file contents at the current
	 * cursor position.
	 */

	ret = false;
	if (vi_extendtext(vi, pos, filesize)) {
		/* Read the contents of the file into the text buffer at the
		 * current cursor position.
		 */

		nread = fread(vi->text + pos, 1, filesize, stream);
		if (nread < filesize) {
			/* Report the error (or partial read), EINTR is not handled */

			vi_error(vi, g_fmtcmdfail, "fread", errno);
			vi_shrinktext(vi, pos, filesize);
		} else {
			ret = true;
		}
	}

	(void)fclose(stream);
	return ret;
}

/****************************************************************************
 * Name: vi_savetext
 *
 * Description:
 *   Save a region of the text buffer to 'filename'
 *
 ****************************************************************************/

static bool vi_savetext(FAR struct vi_s *vi, FAR const char *filename, off_t pos, size_t size)
{
	FILE *stream;
	size_t nwritten;

	vivdbg("filename=\"%s\" pos=%ld size=%ld\n", filename, (long)pos, (long)size);

	/* Open the file for writing */

	stream = fopen(filename, "w");
	if (!stream) {
		vi_error(vi, g_fmtcmdfail, "fopen", errno);
		return false;
	}

	/* Write the region of the text buffer beginning at pos and extending
	 * through pos + size -1.
	 */

	nwritten = fwrite(vi->text + pos, 1, size, stream);
	if (nwritten < size) {
		/* Report the error (or partial write).  EINTR is not handled. */

		vi_error(vi, g_fmtcmdfail, "fwrite", errno);
		(void)fclose(stream);
		return false;
	}

	(void)fclose(stream);
	return true;
}

/****************************************************************************
 * Name: vi_checkfile
 *
 * Description:
 *   Check if a file by this name already exists.
 *
 ****************************************************************************/

static bool vi_checkfile(FAR struct vi_s *vi, FAR const char *filename)
{
	struct stat buf;
	int ret;

	vivdbg("filename=\"%s\"\n", filename);

	/* Get the size of the file  */

	ret = stat(filename, &buf);
	if (ret < 0) {
		/* The file does not exist */

		return false;
	}

	/* It exists, but is it a regular file */

	if (!S_ISREG(buf.st_mode)) {
		/* Report the error... there is really no good return value in
		 * this case.
		 */

		vi_error(vi, g_fmtnotfile, filename);
	}

	return true;
}

/****************************************************************************
 * Mode Management Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_setmode
 *
 * Description:
 *   Set the new mode (or command sub-mode) and reset all other common state
 *   variables.  NOTE that a numeric value may be passed to the new mode in
 *   the value field.
 *
 ****************************************************************************/

static void vi_setmode(FAR struct vi_s *vi, uint8_t mode, long value)
{
	vivdbg("mode=%d value=%ld\n", mode, value);

	/* Set the mode and clear mode-dependent states that are not preserved
	 * across mode changes.
	 */

	vi->mode = mode;
	vi->delarm = false;
	vi->yankarm = false;
//vi->error          = false; need to preserve until vi_showtext is called
	vi->value = value;
	vi->cmdlen = 0;
}

/****************************************************************************
 * Name: vi_setsubmode
 *
 * Description:
 *   Set up one of the data entry sub-modes of the command mode.  These are
 *   modes in which commands or search data will be entered on the final line
 *   of the display.
 *
 ****************************************************************************/

static void vi_setsubmode(FAR struct vi_s *vi, uint8_t mode, char prompt, long value)
{
	vivdbg("mode=%d prompt='%c' value=%ld\n", mode, prompt, value);

	/* Set up the new mode */

	vi_setmode(vi, mode, value);

	/* Save the previous cursor position (not required by all modes) */

	vi->cursave.row = vi->cursor.row;
	vi->cursave.column = vi->cursor.column;

	/* Set up for data entry on the final line */

	vi->cursor.row = vi->display.row - 1;
	vi->cursor.column = 0;
	vi_setcursor(vi, vi->cursor.row, vi->cursor.column);

	/* Output the prompt character in bold text */

	vi_boldon(vi);
	vi_putch(vi, prompt);
	vi_attriboff(vi);

	/* Clear to the end of the line */

	vi_clrtoeol(vi);

	/* Update the cursor position */

	vi->cursor.column = 1;
}

/****************************************************************************
 * Name:  vi_exitsubmode
 *
 * Description:
 *   Exit the data entry sub-mode and return to normal command mode.
 *
 ****************************************************************************/

static void vi_exitsubmode(FAR struct vi_s *vi, uint8_t mode)
{
	vivdbg("mode=%d\n", mode);

	/* Set up the new mode */

	vi_setmode(vi, mode, 0);

	/* Restore the saved cursor position */

	vi->cursor.row = vi->cursave.row;
	vi->cursor.column = vi->cursave.column;
}

/****************************************************************************
 * Display Management
 ****************************************************************************/

/****************************************************************************
 * Name: vi_windowpos
 *
 * Description:
 *   Based on the position of the cursor in the text buffer, determine the
 *   horizontal display cursor position, performing TAB expansion as
 *   necessary.
 *
 ****************************************************************************/

static void vi_windowpos(FAR struct vi_s *vi, off_t start, off_t end, uint16_t *pcolumn, off_t *ppos)
{
	uint16_t column;
	off_t pos;

	vivdbg("start=%ld end=%ld\n", (long)start, (long)end);

	/* Make sure that the end position is not beyond the end of the text. We
	 * assume that the start position is okay.
	 */

	if (end > vi->textsize) {
		end = vi->textsize;
	}

	/* Loop incrementing the text buffer position while text buffer position
	 * is within range.
	 */

	for (pos = start, column = 0; pos < end; pos++) {
		/* Is there a newline terminator at this position? */

		if (vi->text[pos] == '\n') {
			/* Yes... break out of the loop return the cursor column */

			break;
		}

		/* No... Is there a TAB at this position? */

		else if (vi->text[pos] == '\t') {
			/* Yes.. expand the TAB */

			column = NEXT_TAB(column);
		}

		/* No, then just increment the cursor column by one character */

		else {
			column++;
		}
	}

	/* Now return the requested values */

	if (ppos) {
		*ppos = pos;
	}

	if (pcolumn) {
		*pcolumn = column;
	}
}

/****************************************************************************
 * Name: vi_scrollcheck
 *
 * Description:
 *   Check if any operations will require that we scroll the display.
 *
 ****************************************************************************/

static void vi_scrollcheck(FAR struct vi_s *vi)
{
	off_t curline;
	off_t pos;
	uint16_t tmp;
	int column;
	int nlines;

	/* Get the text buffer offset to the beginning of the current line */

	curline = vi_linebegin(vi, vi->curpos);

	/* Check if the current line is above the first line on the display */

	while (curline < vi->winpos) {
		/* Yes.. move the window position up to the beginning of the previous
		 * line line and check again */

		vi->winpos = vi_prevline(vi, vi->winpos);
	}

	/* Reset the cursor row position so that it is relative to the
	 * top of the display.
	 */

	vi->cursor.row = 0;
	for (pos = vi->winpos; pos < curline; pos = vi_nextline(vi, pos)) {
		vi->cursor.row++;
	}

	/* Check if the cursor row position is below the bottom of the display */

	for (; vi->cursor.row >= vi->display.row; vi->cursor.row--) {
		/* Yes.. move the window position down by one line and check again */

		vi->winpos = vi_nextline(vi, vi->winpos);
	}

	/* Check if the cursor column is on the display.  vi_windowpos returns the
	 * unrestricted column number of cursor.  hscroll is the horizontal offset
	 * in characters.
	 */

	vi_windowpos(vi, curline, vi->curpos, &tmp, NULL);
	column = (int)tmp - (int)vi->hscroll;

	/* Force the cursor column to lie on the display.  First check if the
	 * column lies to the left of the horizontal scrolling position.  If it
	 * does, move the scroll position to the left by tabs until the cursor
	 * lies on the display.
	 */

	while (column < 0) {
		column += TABSIZE;
		vi->hscroll -= TABSIZE;
	}

	/* If the the cursor column lies to the right of the display, then adjust
	 * the horizontal scrolling position so that the cursor position does
	 * lie on the display.
	 */

	while (column >= vi->display.column) {
		column -= TABSIZE;
		vi->hscroll += TABSIZE;
	}

	/* That final adjusted position is the display cursor column */

	vi->cursor.column = column;

	/* Check if new window position is below the previous position.
	 * In this case, we will need to scroll up until the new window
	 * position is at the top of the display.
	 */

	if (vi->winpos > vi->prevpos) {
		/* We will need to scroll up.  Count how many lines we
		 * need to scroll.
		 */

		for (nlines = 0, pos = vi->prevpos; pos != vi->winpos && nlines < vi->display.row; nlines++) {
			pos = vi_nextline(vi, pos);
		}

		/* Then scroll up that number of lines */

		if (nlines < vi->display.row) {
			vi_scrollup(vi, nlines);
		}
	}

	/* Check if new window position is above the previous position.
	 * In this case, we will need to scroll down until the new window
	 * position is at the top of the display.
	 */

	else if (vi->winpos < vi->prevpos) {

		for (nlines = 0, pos = vi->prevpos; pos != vi->winpos && nlines < vi->display.row; nlines++) {
			pos = vi_prevline(vi, pos);
		}

		/* Then scroll down that number of lines */

		if (nlines < vi->display.row) {
			vi_scrolldown(vi, nlines);
		}
	}

	/* Save the previous top-of-display position for next time around.
	 * This can be modified asynchronously by text deletion operations.
	 */

	vi->prevpos = vi->winpos;
	vivdbg("winpos=%ld hscroll=%d\n", (long)vi->winpos, (long)vi->hscroll);
}

/****************************************************************************
 * Name: vi_showtext
 *
 * Description:
 *   Update the display based on the last operation.  This function is
 *   called at the beginning of the processing loop in Command and Insert
 *   modes (and also in the continuous replace mode).
 *
 ****************************************************************************/

static void vi_showtext(FAR struct vi_s *vi)
{
	off_t pos;
	uint16_t row;
	uint16_t endrow;
	uint16_t column;
	uint16_t endcol;
	uint16_t tabcol;

	/* Check if any of the preceding operations will cause the display to
	 * scroll.
	 */

	vi_scrollcheck(vi);

	/* If there is an error message at the bottom of the display, then
	 * do not update the last line.
	 */

	endrow = vi->display.row;
	if (vi->error) {
		endrow--;
	}

	/* Make sure that all character attributes are disabled; Turn off the
	 * cursor during the update.
	 */

	vi_attriboff(vi);
	vi_cursoroff(vi);

	/* Write each line to the display, handling horizontal scrolling and
	 * tab expansion.
	 */

	for (pos = vi->winpos, row = 0; pos < vi->textsize && row < endrow; row++) {
		/* Get the last column on this row.  Avoid writing into the last byte
		 * on the screen which may trigger a scroll.
		 */

		endcol = vi->display.column;
		if (row >= vi->display.row - 1) {
			endcol--;
		}

		/* Get the position into this line corresponding to display column 0,
		 * accounting for horizontal scrolling and tab expansion.  Add that to
		 * the line start offset to get the first offset to consider for
		 * display.
		 */

		vi_windowpos(vi, pos, pos + vi->hscroll, NULL, &pos);

		/* Set the cursor position to the beginning of this row and clear to
		 * the end of the line.
		 */

		vi_setcursor(vi, row, 0);
		vi_clrtoeol(vi);

		/* Loop for each column */

		for (column = 0; pos < vi->textsize && column < endcol; pos++) {
			/* Break out of the loop if we encounter the newline before the
			 * last column is encountered.
			 */

			if (vi->text[pos] == '\n') {
				break;
			}

			/* Perform TAB expansion */

			else if (vi->text[pos] == '\t') {
				tabcol = NEXT_TAB(column);
				if (tabcol < endcol) {
					for (; column < tabcol; column++) {
						vi_putch(vi, ' ');
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
				vi_putch(vi, vi->text[pos]);
				column++;
			}
		}

		/* Skip to the beginning of the next line */

		pos = vi_nextline(vi, pos);
	}

	/* If there was not enough text to fill the display, clear the
	 * remaining lines (except for any possible error line at the
	 * bottom of the display).
	 */

	for (; row < endrow; row++) {
		/* Set the cursor position to the beginning of the row and clear to
		 * the end of the line.
		 */

		vi_setcursor(vi, row, 0);
		vi_clrtoeol(vi);
	}

	/* Turn the cursor back on */

	vi_cursoron(vi);
}

/****************************************************************************
 * Command Mode Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_cusorup
 *
 * Description:
 *   Move the cursor up one line in the text buffer.
 *
 ****************************************************************************/

static void vi_cusorup(FAR struct vi_s *vi, int nlines)
{
	int remaining;
	off_t start;
	off_t end;

	vivdbg("nlines=%d\n", nlines);

	/* How many lines do we need to move?  Zero means 1 (so does 1) */

	remaining = (nlines < 1 ? 1 : nlines);

	/* Get the offset to the start of the current line */

	start = vi_linebegin(vi, vi->curpos);

	/* Now move the cursor position back the correct number of lines */

	for (; remaining > 0; remaining--) {
		/* Get the start position of the previous line */

		start = vi_prevline(vi, start);

		/* Find the cursor position on the next line corresponding to the
		 * cursor position on the current line.
		 */

		end = start + vi->cursor.column + vi->hscroll;
		vi_windowpos(vi, start, end, NULL, &vi->curpos);
	}
}

/****************************************************************************
 * Name: vi_cursordown
 *
 * Description:
 *   Move the cursor down one line in the text buffer
 *
 ****************************************************************************/

static void vi_cursordown(FAR struct vi_s *vi, int nlines)
{
	int remaining;
	off_t start;
	off_t end;

	vivdbg("nlines=%d\n", nlines);

	/* How many lines do we need to move?  Zero means 1 (so does 1) */

	remaining = (nlines < 1 ? 1 : nlines);

	/* Get the offset to the start of the current line */

	start = vi_linebegin(vi, vi->curpos);

	/* Now move the cursor position forward the correct number of lines */

	for (; remaining > 0; remaining--) {
		/* Get the start of the next line. */

		start = vi_nextline(vi, start);

		/* Find the cursor position on the next line corresponding to the
		 * cursor position on the current line.
		 */

		end = start + vi->cursor.column + vi->hscroll;
		vi_windowpos(vi, start, end, NULL, &vi->curpos);
	}
}

/****************************************************************************
 * Name: vi_cursorleft
 *
 * Description:
 *   Move the cursor left 'ncolumns' columns in the text buffer (without moving
 *   to the preceding line).  Note that a repetition count of 0 means to
 *   perform the movement once.
 *
 ****************************************************************************/

static off_t vi_cursorleft(FAR struct vi_s *vi, off_t curpos, int ncolumns)
{
	int remaining;

	vivdbg("curpos=%ld ncolumns=%d\n", curpos, ncolumns);

	/* Loop decrementing the cursor position for each repetition count.  Break
	 * out early if we hit either the beginning of the text buffer, or the end
	 * of the previous line.
	 */

	for (remaining = (ncolumns < 1 ? 1 : ncolumns);
		 curpos > 0 && remaining > 0 && vi->text[curpos - 1] != '\n';
		 curpos--, remaining--);

	return curpos;
}

/****************************************************************************
 * Name: vi_cursorright
 *
 * Description:
 *   Move the cursor right 'ncolumns' columns in the text buffer (without
 *   moving to the next line).  Note that a repetition count of 0 means to
 *   perform the movement once.
 *
 ****************************************************************************/

static off_t vi_cursorright(FAR struct vi_s *vi, off_t curpos, int ncolumns)
{
	int remaining;

	vivdbg("curpos=%ld ncolumns=%d\n", curpos, ncolumns);

	/* Loop incrementing the cursor position for each repetition count.  Break
	 * out early if we hit either the end of the text buffer, or the end of the line.
	 */

	for (remaining = (ncolumns < 1 ? 1 : ncolumns);
		 curpos < vi->textsize && remaining > 0 && vi->text[curpos] != '\n';
		 curpos++, remaining--);

	return curpos;
}

/****************************************************************************
 * Name: vi_delforward
 *
 * Description:
 *   Delete characters from the current cursor position forward
 *
 ****************************************************************************/

static void vi_delforward(FAR struct vi_s *vi)
{
	off_t end;
	size_t size;

	vivdbg("curpos=%ld value=%ld\n", (long)vi->curpos, vi->value);

	/* Get the cursor position as if we would have move the cursor right N
	 * times (which might be <N characters).
	 */

	end = vi_cursorright(vi, vi->curpos, vi->value);

	/* The difference from the current position then is the number of
	 * characters to be deleted.
	 */

	size = end - vi->curpos;
	vi_shrinktext(vi, vi->curpos, size);
}

/****************************************************************************
 * Name: vi_delbackward
 *
 * Description:
 *   Delete characters before the current cursor position
 *
 ****************************************************************************/

static void vi_delbackward(FAR struct vi_s *vi)
{
	off_t start;
	off_t end;
	size_t size;

	vivdbg("curpos=%ld value=%ld\n", (long)vi->curpos, vi->value);

	/* Back up one character.  This is where the deletion will end */

	end = vi_cursorleft(vi, vi->curpos, 1);

	/* Get the cursor position as if we would have move the cursor left N
	 * times (which might be <N characters).
	 */

	start = vi_cursorleft(vi, end, vi->value);

	/* The difference from the current position then is the number of
	 * characters to be deleted.
	 */

	size = end - start;
	vi_shrinktext(vi, start, size);
}

/****************************************************************************
 * Name: vi_linerange
 *
 * Description:
 *   Return the start and end positions for N lines in the text buffer,
 *   beginning at the current line.  This is logic common to yanking and
 *   deleting lines.
 *
 ****************************************************************************/

static void vi_linerange(FAR struct vi_s *vi, off_t *start, off_t *end)
{
	off_t next;
	int nlines;

	/* Get the offset in the text buffer to the beginning of the current line */

	*start = vi_linebegin(vi, vi->curpos);

	/* Move one line unless a repetition count was provided */

	nlines = (vi->value > 0 ? vi->value : 1);

	/* Search ahead to find the end of the last line to yank */

	for (next = *start; nlines > 1; nlines--) {
		next = vi_nextline(vi, next);
	}

	*end = vi_lineend(vi, next);
}

/****************************************************************************
 * Name: vi_delline
 *
 * Description:
 *   Delete N lines from the text buffer, beginning at the current line.
 *
 ****************************************************************************/

static void vi_delline(FAR struct vi_s *vi)
{
	off_t delsize;
	off_t start;
	off_t end;

	/* Get the offset in the text buffer corresponding to the range of lines to
	 * be deleted
	 */

	vi_linerange(vi, &start, &end);
	vivdbg("start=%ld end=%ld\n", (long)start, (long)end);

	/* Remove the text from the text buffer */

	delsize = end - start + 1;
	vi_shrinktext(vi, start, delsize);
}

/****************************************************************************
 * Name: vi_yank
 *
 * Description:
 *   Remove N lines from the text buffer, beginning at the current line and
 *   copy the lines to an allocated yank buffer.
 *
 ****************************************************************************/

static void vi_yank(FAR struct vi_s *vi)
{
	off_t start;
	off_t end;

	/* Get the offset in the text buffer corresponding to the range of lines to
	 * be yanked
	 */

	vi_linerange(vi, &start, &end);
	vivdbg("start=%ld end=%ld\n", (long)start, (long)end);

	/* Free any previously yanked lines */

	if (vi->yank) {
		free(vi->yank);
	}

	/* Allocate a yank buffer biggest enough to hold the lines */

	vi->yankalloc = end - start + 1;
	vi->yank = (FAR char *)malloc(vi->yankalloc);

	if (!vi->yank) {
		vi_error(vi, g_fmtallocfail);
		vi->yankalloc = 0;
		return;
	}

	/* Copy the block from the text buffer to the yank buffer */

	memcpy(vi->yank, &vi->text[start], vi->yankalloc);

	/* Remove the yanked text from the text buffer */

	vi_shrinktext(vi, start, vi->yankalloc);
}

/****************************************************************************
 * Name: vi_paste
 *
 * Description:
 *   Copy line(s) from the yank buffer, and past them after the current line.
 *   The contents of the yank buffer are released.
 *
 ****************************************************************************/

static void vi_paste(FAR struct vi_s *vi)
{
	off_t start;

	vivdbg("curpos=%ld yankalloc=%d\n", (long)vi->curpos, (long)vi->yankalloc);

	/* Make sure there is something to be yanked */

	if (!vi->yank || vi->yankalloc <= 0) {
		return;
	}

	/* Paste at the beginning of the next line */

	start = vi_nextline(vi, vi->curpos);

	/* Reallocate the text buffer to hold the yank buffer contents at the
	 * the beginning of the next line.
	 */

	if (vi_extendtext(vi, start, vi->yankalloc)) {
		/* Copy the contents of the yank buffer into the text buffer at the
		 * position where the start of the next line was.
		 */

		memcpy(&vi->text[start], vi->yank, vi->yankalloc);
	}

	/* Free the yank buffer in any event */

	free(vi->yank);

	vi->yank = NULL;
	vi->yankalloc = 0;
}

/****************************************************************************
 * Name: vi_gotoline
 *
 * Description:
 *   Position the the cursor at the line specified by vi->value.  If
 *   vi->value is zero, then the cursor is position at the end of the text
 *   buffer.
 *
 ****************************************************************************/

static void vi_gotoline(FAR struct vi_s *vi)
{
	vivdbg("curpos=%ld value=%ld\n", (long)vi->curpos, vi->value);

	/* Special case the first line */

	if (vi->value == 1) {
		vi->curpos = 0;
	}

	/* Work harder to position to lines in the middle */

	else if (vi->value > 0) {
		uint32_t line;

		/* Got to the line == value */

		for (line = vi->value, vi->curpos = 0; --line > 0 && vi->curpos < vi->textsize;) {
			vi->curpos = vi_nextline(vi, vi->curpos);
		}
	}

	/* No value means to go to beginning of the last line */

	else {
		/* Get the beginning of the last line */

		vi->curpos = vi_linebegin(vi, vi->textsize);
	}
}

/****************************************************************************
 * Name: vi_cmd_mode
 *
 * Description:
 *   Command mode loop
 *
 ****************************************************************************/

static void vi_cmd_mode(FAR struct vi_s *vi)
{
	vivdbg("Enter command mode\n");

	/* Loop while we are in command mode */

	while (vi->mode == MODE_COMMAND) {
		bool preserve;
		int ch;

		/* Make sure that the display reflects the current state */

		vi_showtext(vi);
		vi_setcursor(vi, vi->cursor.row, vi->cursor.column);

		/* Get the next character from the input */

		ch = vi_getch(vi);

		/* Anything other than 'd' disarms line deletion */

		if (ch != 'd') {
			vi->delarm = false;
		}

		/* Anything other than 'y' disarms line yanking */

		if (ch != 'y') {
			vi->yankarm = false;
		}

		/* Any key press clears the error message */

		vi->error = false;

		/* Handle numeric input.  Zero (0) with no preceding value is a
		 * special case: It means to go to the beginning o the line.
		 */

		if (isdigit(ch) && (vi->value > 0 || ch != '0')) {
			uint32_t tmp = 10 * vi->value + (ch - '0');
			if (tmp > UINT16_MAX) {
				tmp = UINT16_MAX;
			}

			/* Update the command repetition count */

			vi->value = tmp;

			vivdbg("Value=%ld\n", vi->value);
			continue;
		}

		/* Then handle the non-numeric character.  Normally the accumulated
		 * value will be reset after processing the command.  There are a few
		 * exceptions; 'preserve' will be set to 'true' in those exceptional
		 * cases.
		 */

		preserve = false;
		switch (ch) {
		case KEY_CMDMODE_UP: {	/* Move the cursor up one line */
			vi_cusorup(vi, vi->value);
		}
		break;

		case KEY_CMDMODE_DOWN: {	/* Move the cursor down one line */
			vi_cursordown(vi, vi->value);
		}
		break;

		case KEY_CMDMODE_LEFT: {	/* Move the cursor left N characters */
			vi->curpos = vi_cursorleft(vi, vi->curpos, vi->value);
		}
		break;

		case KEY_CMDMODE_RIGHT: {	/* Move the cursor right one character */
			vi->curpos = vi_cursorright(vi, vi->curpos, vi->value);
		}
		break;

		case KEY_CMDMODE_BEGINLINE: {	/* Move cursor to start of current line */
			vi->curpos = vi_linebegin(vi, vi->curpos);
		}
		break;

		case KEY_CMDMODE_ENDLINE: {	/* Move cursor to end of current line */
			vi->curpos = vi_lineend(vi, vi->curpos);
		}
		break;

		case KEY_CMDMODE_PAGEUP: {	/* Move up (backward) one screen */
			vi_cusorup(vi, vi->display.row);
		}
		break;

		case KEY_CMDMODE_PAGEDOWN: {	/* Move down (forward) one screen */
			vi_cursordown(vi, vi->display.row);
		}
		break;

		case KEY_CMDMODE_HALFUP: {	/* Move up (backward) one screen */
			vi_cusorup(vi, vi->display.row >> 1);
		}
		break;

		case KEY_CMDMODE_HALFDOWN: {	/*  Move down (forward) one half screen */
			vi_cursordown(vi, vi->display.row >> 1);
		}
		break;

		case KEY_CMDMODE_DEL:	/* Delete N characters at the cursor */
		case ASCII_DEL: {
			vi_delforward(vi);
		}
		break;

		case ASCII_BS: {		/* Delete N characters before the cursor */
			vi_delbackward(vi);
		}
		break;

		case KEY_CMDMODE_DEL_LINE: {	/* Delete the current line */
			if (vi->delarm) {
				vi_delline(vi);
				vi->delarm = false;
			} else {
				vi->delarm = true;
				preserve = true;
			}
		}
		break;

		case KEY_CMDMODE_YANK: {	/* Yank the current line(s) into the buffer */
			if (vi->yankarm) {
				vi_yank(vi);
				vi->yankarm = false;
			} else {
				vi->yankarm = true;
				preserve = true;
			}
		}
		break;

		case KEY_CMDMODE_PASTE: {	/* Paste line(s) from into text after current line */
			vi_paste(vi);
		}
		break;

		case KEY_CMDMODE_REPLACECH: {	/* Replace character(s) under cursor */
			vi_setmode(vi, SUBMODE_REPLACECH, vi->value);
			preserve = true;
		}
		break;

		case KEY_CMDMODE_REPLACE: {	/* Replace character(s) under cursor until ESC */
			vi_setmode(vi, MODE_REPLACE, 0);
		}
			break;				/* Not implemented */

		case KEY_CMDMODE_OPENBELOW: {	/* Enter insertion mode in new line below current */
			/* Go forward to the end of the current line */

			vi->curpos = vi_lineend(vi, vi->curpos);

			/* Insert a newline to break the line.  The cursor now points
			 * beginning of the new line.
			 */

			vi_insertch(vi, '\n');

			/* Then enter insert mode */

			vi_setmode(vi, MODE_INSERT, 0);
		}
		break;

		case KEY_CMDMODE_OPENABOVE: {	/* Enter insertion mode in new line above current */
			/* Back up to the beginning of the end of the previous line */

			off_t pos = vi_prevline(vi, vi->curpos);
			vi->curpos = vi_lineend(vi, pos);

			/* Insert a newline to open the line.  The cursor will now point to the
			 * beginning of newly openly line before the current line.
			 */

			vi_insertch(vi, '\n');

			/* Then enter insert mode */

			vi_setmode(vi, MODE_INSERT, 0);
		}
		break;

		case KEY_CMDMODE_APPEND: {	/* Enter insertion mode after the current cursor position */
			vi->curpos = vi_cursorright(vi, vi->curpos, 1);
			vi_setmode(vi, MODE_INSERT, 0);
		}
		break;

		case KEY_CMDMODE_APPENDEND: {	/* Enter insertion mode at the end of the current line */
			vi->curpos = vi_lineend(vi, vi->curpos);
			vi_setmode(vi, MODE_INSERT, 0);
		}
		break;

		case KEY_CMDMODE_INSBEGIN: {	/* Enter insertion mode at the beginning of the current line */
			vi->curpos = vi_linebegin(vi, vi->curpos);
		}
		/* Fall through */

		case KEY_CMDMODE_INSERT: {	/* Enter insertion mode before the current cursor position */
			vi_setmode(vi, MODE_INSERT, 0);
		}
		break;

		case KEY_CMDMODE_COLONMODE: {	/* Enter : command sub-mode */
			vi_setsubmode(vi, SUBMODE_COLON, ':', 0);
		}
		break;

		case KEY_CMDMODE_FINDMODE: {	/* Enter / find sub-mode */
			vi_setsubmode(vi, SUBMODE_FIND, '/', 0);
		}
		break;

		case KEY_CMDMODE_GOTO: {	/* Go to line specified by the accumulated value */
			vi_gotoline(vi);
		}
		break;

		/* Unimplemented and invalid commands */

		case KEY_CMDMODE_REDRAW:	/* Redraws the screen */
		case KEY_CMDMODE_REDRAW2:	/* Redraws the screen, removing deleted lines */
		case KEY_CMDMODE_MARK:	/* Place a mark beginning at the current cursor position */
		default: {
			VI_BEL(vi);
		}
		break;
		}

		/* Any non-numeric input will reset the accumulated value (after it has
		 * been used).  There are a few exceptions:
		 *
		 * - For the double character sequences, we need to retain the value
		 *   until the next character is entered.
		 * - If we are changing modes, then we may need to preserve the 'value'
		 *   as well; in some cases settings are passed to the new mode in
		 *   'value' (vi_setmode() will have set or cleared 'value'
		 *   appropriately).
		 */

		if (!preserve) {
			vi->value = 0;
		}
	}
}

/****************************************************************************
 * Common Sub-Mode Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_cmdch
 *
 * Description:
 *   Insert one character into the data entry line
 *
 ****************************************************************************/

static void vi_cmdch(FAR struct vi_s *vi, char ch)
{
	int index = vi->cmdlen;
	int next = index + 1;

	vivdbg("cmdlen=%d ch=%c[%02x]\n", vi->cmdlen, isprint(ch) ? ch : '.', ch);

	/* Abort gracelessly if the scratch buffer becomes full */

	if (next >= SCRATCH_BUFSIZE) {
		vi_exitsubmode(vi, MODE_COMMAND);
		return;
	}

	/* Add the new character to the scratch buffer */

	vi->scratch[index] = ch;
	vi->cmdlen = next;

	/* Update the cursor position */

	vi->cursor.column = next + 1;

	/* And add the new character to the display */

	vi_putch(vi, ch);
}

/****************************************************************************
 * Name:  vi_cmdbackspace
 *
 * Description:
 *   Process a backspace character in the data entry line
 *
 ****************************************************************************/

static void vi_cmdbackspace(FAR struct vi_s *vi)
{
	vivdbg("cmdlen=%d\n", vi->cmdlen);

	if (vi->cmdlen > 0) {
		vi_setcursor(vi, vi->display.row - 1, vi->cmdlen);
		vi_clrtoeol(vi);

		/* Update the command index and cursor position */

		vi->cursor.column = vi->cmdlen;

		/* Decrement the number of characters on in the command */

		vi->cmdlen--;
	}
}

/****************************************************************************
 * Colon Data Entry Sub-Mode Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  vi_parsecolon
 *
 * Description:
 *   Parse the colon command collected in the scratch buffer
 *
 ****************************************************************************/

static void vi_parsecolon(FAR struct vi_s *vi)
{
	FAR const char *filename = NULL;
	uint8_t cmd = CMD_NONE;
	bool done = false;
	bool forced;
	int col;
	int ch;

	vivdbg("Parse: \"%s\"\n", vi->scratch);

	/* NUL terminate the command */

	vi->scratch[vi->cmdlen] = '\0';

	/* Then parse the contents of the scratch buffer */

	for (col = 0; col < vi->cmdlen && !done; col++) {
		/* Get the next command character from the scratch buffer */

		ch = vi->scratch[col];

		/* Check if the next after that is KEY_COLMODE_FORCE */

		forced = false;
		if (col < vi->cmdlen && vi->scratch[col + 1] == KEY_COLMODE_FORCE) {
			/* Yes.. the operation is forced */

			forced = true;
			col++;
		}

		/* Then process the command character */

		switch (ch) {
		case KEY_COLMODE_READ: {
			/* Reading a file should not be forced */

			if (cmd == CMD_NONE && !forced) {
				cmd = CMD_READ;
			} else {
				/* The read operation is not compatible with writing or
				 * quitting
				 */

				goto errout_bad_command;
			}
		}
		break;

		case KEY_COLMODE_WRITE: {
			/* Are we just writing?  Or writing then quitting? */

			if (cmd == CMD_NONE) {
				/* Just writing.. do we force overwriting? */

				cmd = (forced ? CMD_OWRITE : CMD_WRITE);
			} else if (cmd == CMD_QUIT) {
				/* Both ... do we force overwriting the file? */

				cmd = (forced ? CMD_OWRITE_QUIT : CMD_WRITE_QUIT);
			} else {
				/* Anything else, including a forced quit is a syntax error */

				goto errout_bad_command;
			}
		}
		break;

		case KEY_COLMODE_QUIT: {
			/* Are we just quitting?  Or writing then quitting? */

			if (cmd == CMD_NONE) {
				/* Just quitting... should we discard any changes? */

				cmd = (forced ? CMD_DISCARD : CMD_QUIT);
			}

			/* If we are also writing, then it makes no sense to force the
			 * quit operation.
			 */

			else if (cmd == CMD_WRITE && !forced) {
				cmd = CMD_WRITE_QUIT;
			} else if (cmd == CMD_OWRITE && !forced) {
				cmd = CMD_OWRITE_QUIT;
			} else {
				/* Quit is not compatible with reading */

				goto errout_bad_command;
			}
		}
		break;

		default: {
			/* Ignore whitespace */

			if (ch != ' ') {
				/* Anything else terminates the loop */

				done = true;

				/* If there is anything else on the line, then it must be
				 * a file name.  If we are writing (or reading with an
				 * empty text buffer), then we will need to copy the file
				 * into the filename storage area.
				 */

				if (ch != '\0') {
					/* For now, just remember where the file is in the
					 * scratch buffer.
					 */

					filename = &vi->scratch[col];
				}
			}
		}
		break;
		}
	}

	/* Did we find any valid command?  A read command requires a filename.
	 * A filename where one is not needed is also an error.
	 */

	vivdbg("cmd=%d filename=\"%s\"\n", cmd, vi->filename);

	if (cmd == CMD_NONE || (IS_READ(cmd) && !filename) || (!USES_FILE(cmd) && filename)) {
		goto errout_bad_command;
	}

	/* Are we writing to a new filename?  If we are not forcing the write,
	 * then we have to check if the file exists.
	 */

	if (filename && IS_NOWRITE(cmd)) {
		/* Check if the file exists */

		if (vi_checkfile(vi, filename)) {
			/* It does... show an error and exit */

			vi_error(vi, g_fmtfileexists);
			goto errout;
		}
	}

	/* Check if we are trying to quit with un-saved changes.  The user must
	 * force quitting in this case.
	 */

	if (vi->modified && IS_NDISCARD(cmd)) {
		/* Show an error and exit */

		vi_error(vi, g_fmtmodified);
		goto errout;
	}

	/* Are we now committed to reading the file? */

	if (IS_READ(cmd)) {
		/* Was the text buffer empty? */

		bool empty = (vi->textsize == 0);

		/* Yes.. get the cursor position of the beginning of the next line */

		off_t pos = vi_nextline(vi, vi->curpos);

		/* Then read the file into the text buffer at that position. */

		if (vi_insertfile(vi, pos, filename)) {
			/* Was the text buffer empty before we inserted the file? */

			if (empty) {
				/* Yes.. then we want to save the filename and mark the text
				 * as unmodified.
				 */

				strncpy(vi->filename, filename, MAX_STRING - 1);

				/* Make sure that the (possibly truncated) file name is NUL
				 * terminated
				 */

				vi->filename[MAX_STRING - 1] = '\0';
				vi->modified = false;
			} else {
				/* No.. then we want to retain the filename and mark the text
				 * as modified.
				 */

				vi->modified = true;
			}
		}
	}

	/* Are we now committed to writing the file? */

	if (IS_WRITE(cmd)) {
		/* If we are writing to a new file, then we need to copy the filename
		 * from the scratch buffer to the filename buffer.
		 */

		if (filename) {
			strncpy(vi->filename, filename, MAX_STRING - 1);

			/* Make sure that the (possibly truncated) file name is NUL
			 * terminated
			 */

			vi->filename[MAX_STRING - 1] = '\0';
		}

		/* If it is not a new file and if there are no changes to the text
		 * buffer, then ignore the write.
		 */

		if (filename || vi->modified) {
			/* Now, finally, we can save the file */

			if (!vi_savetext(vi, vi->filename, 0, vi->textsize)) {
				/* An error occurred while saving the file and we are
				 * not forcing the quit operation.  So error out without
				 * quitting until the user decides what to do about
				 * the save failure.
				 */

				goto errout;
			}

			/* The text buffer contents are no longer modified */

			vi->modified = false;
		}
	}

	/* Are we committed to exit-ing? */

	if (IS_QUIT(cmd)) {
		/* Yes... free resources and exit */

		vi_release(vi);
		exit(EXIT_SUCCESS);
	}

	/* Otherwise, revert to command mode */

	vi_exitsubmode(vi, MODE_COMMAND);
	return;

errout_bad_command:
	vi_error(vi, g_fmtnotcmd, vi->scratch);

errout:
	vi_exitsubmode(vi, MODE_COMMAND);
}

/****************************************************************************
 * Name: vi_cmd_submode
 *
 * Description:
 *   Colon command sub-mode of the command mode processing
 *
 ****************************************************************************/

static void vi_cmd_submode(FAR struct vi_s *vi)
{
	int ch;

	vivdbg("Enter colon command sub-mode\n");

	/* Loop while we are in colon command mode */

	while (vi->mode == SUBMODE_COLON) {
		/* Get the next character from the input */

		ch = vi_getch(vi);

		/* Handle the newly received character */

		switch (ch) {
		case KEY_COLMODE_QUOTE: {	/* Quoted character follows */
			/* Insert the next character unconditionally */

			vi_cmdch(vi, vi_getch(vi));
		}
		break;

		case ASCII_BS: {		/* Delete the character(s) before the cursor */
			vi_cmdbackspace(vi);
		}
		break;

		case ASCII_ESC: {	/* Escape exits colon mode */
			vi_exitsubmode(vi, MODE_COMMAND);
		}
		break;

			/* What do we do with carriage returns? line feeds? */

#if defined(CONFIG_EOL_IS_CR)
		case '\r': {			/* CR terminates line */
			vi_parsecolon(vi);
		}
		break;

#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\r':				/* Wait for the LF */
			break;
#endif

#if defined(CONFIG_EOL_IS_LF) || defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\n': {			/* LF terminates line */
			vi_parsecolon(vi);
		}
		break;
#endif

#ifdef CONFIG_EOL_IS_EITHER_CRLF
		case '\r':				/* Either CR or LF terminates line */
		case '\n': {
			vi_parsecolon(vi);
		}
		break;
#endif

		default: {
			/* Ignore all but printable characters */

			if (isprint(ch)) {
				/* Insert the filtered character into the scratch buffer */

				vi_cmdch(vi, ch);
			} else {
				VI_BEL(vi);
			}
		}
		break;
		}
	}
}

/****************************************************************************
 * Find Data Entry Sub-Mode Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_findstring
 *
 * Description:
 *   Find the the string in the findstr buffer by searching for a matching
 *   sub-string in the text buffer, starting at the current cursor position.
 *
 ****************************************************************************/

static bool vi_findstring(FAR struct vi_s *vi)
{
	off_t pos;
	int len;

	vivdbg("findstr: \"%s\"\n", vi->findstr);

	/* The search string is in the find buffer */

	len = strlen(vi->findstr);
	if (!len) {
		return false;
	}

	/* Search from the current cursor position forward for a
	 * matching sub-string.  Stop loo
	 */

	for (pos = vi->curpos; pos + len <= vi->textsize; pos++) {
		/* Check for the matching sub-string */

		if (strncmp(vi->text + pos, vi->scratch, len) == 0) {
			/* Found it... save the cursor position and
			 * return success.
			 */

			vi->curpos = pos;
			return true;
		}
	}

	/* If we get here, then the search string was not found anywhere after the
	 * current cursor position.
	 */

	return false;
}

/****************************************************************************
 * Name:  vi_parsefind
 *
 * Description:
 *   Find the string collected in the scratch buffer.
 *
 ****************************************************************************/

static void vi_parsefind(FAR struct vi_s *vi)
{
	/* Make certain that the scratch buffer contents are NUL terminated */

	vi->scratch[vi->cmdlen] = '\0';

	/* Is there anything in the scratch buffer? If not, then we will use the
	 * string from the previous find operation.
	 */

	vivdbg("scratch: \"%s\"\n", vi->scratch);

	if (vi->cmdlen > 0) {
		/* Copy the new search string from the scratch to the find buffer */

		strncpy(vi->findstr, vi->scratch, MAX_STRING - 1);

		/* Make sure that the (possibly truncated) search string is NUL
		 * terminated
		 */

		vi->findstr[MAX_STRING - 1] = '\0';
	}

	/* Then attempt to find the string */

	(void)vi_findstring(vi);

	/* Exit the sub-mode and revert to command mode */

	vi_exitsubmode(vi, MODE_COMMAND);
}

/****************************************************************************
 * Name: vi_find_submode
 *
 * Description:
 *   Find command sub-mode of the command mode processing
 *
 ****************************************************************************/

static void vi_find_submode(FAR struct vi_s *vi)
{
	int ch;

	vivdbg("Enter find sub-mode\n");

	/* Loop while we are in find mode */

	while (vi->mode == SUBMODE_FIND) {
		/* Get the next character from the input */

		ch = vi_getch(vi);

		/* Handle the newly received character */

		switch (ch) {
		case KEY_FINDMODE_QUOTE: {	/* Quoted character follows */
			/* Insert the next character unconditionally */

			vi_cmdch(vi, vi_getch(vi));
		}
		break;

		case ASCII_BS: {		/* Delete the character before the cursor */
			vi_cmdbackspace(vi);
		}
		break;

		case ASCII_ESC: {	/* Escape exits find mode */
			vi_exitsubmode(vi, MODE_COMMAND);
		}
		break;

			/* What do we do with carriage returns? line feeds? */

#if defined(CONFIG_EOL_IS_CR)
		case '\r': {			/* CR terminates line */
			vi_parsefind(vi);
		}
		break;

#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\r':				/* Wait for the LF */
			break;
#endif

#if defined(CONFIG_EOL_IS_LF) || defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\n': {			/* LF terminates line */
			vi_parsefind(vi);
		}
		break;
#endif

#ifdef CONFIG_EOL_IS_EITHER_CRLF
		case '\r':				/* Either CR or LF terminates line */
		case '\n': {
			vi_parsefind(vi);
		}
		break;
#endif

		default: {
			/* Ignore all but printable characters */

			if (isprint(ch)) {
				/* Insert the filtered character into the scratch buffer */

				vi_cmdch(vi, ch);
			} else {
				VI_BEL(vi);
			}
		}
		break;
		}
	}
}

/****************************************************************************
 * Replace Text Sub-Mode Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_replacech
 *
 * Description:
 *   Replace the character at the current position.  If the current position
 *   is the end of line, then insert the character.
 *
 ****************************************************************************/

static void vi_replacech(FAR struct vi_s *vi, char ch)
{
	vivdbg("curpos=%ld ch=%c[%02x]\n", vi->curpos, isprint(ch) ? ch : '.', ch);

	/* Is there a newline at the current cursor position? */

	if (vi->text[vi->curpos] == '\n') {
		/* Yes, then insert the new character before the newline */

		vi_insertch(vi, ch);
	} else {
		/* No, just replace the character and increment the cursor position */

		vi->text[vi->curpos++] = ch;
	}
}

/****************************************************************************
 * Name: vi_replacech_submode
 *
 * Description:
 *   Replace character command sub-mode of the command mode processing
 *
 ****************************************************************************/

static void vi_replacech_submode(FAR struct vi_s *vi)
{
	off_t end;
	long nchars;
	bool found = false;
	int ch = 0;

	/* Get the number of characters to replace */

	nchars = (vi->value > 0 ? vi->value : 1);

	vivdbg("Enter replaces character(s) sub-mode: nchars=%d\n", nchars);

	/* Are there that many characters left on the line to be replaced? */

	end = vi_lineend(vi, vi->curpos);
	if (vi->curpos + nchars > end) {
		vi_error(vi, g_fmtnotvalid);
		vi_setmode(vi, MODE_COMMAND, 0);
	}

	/* Loop until we get the replacement character */

	while (vi->mode == SUBMODE_REPLACECH && !found) {
		/* Get the next character from the input */

		ch = vi_getch(vi);

		/* Handle the newly received character */

		switch (ch) {
		case KEY_FINDMODE_QUOTE: {	/* Quoted character follows */
			/* Insert the next character unconditionally */

			ch = vi_getch(vi);
			found = true;
		}
		break;

		case ASCII_ESC: {	/* Escape exits replace mode */
			vi_setmode(vi, MODE_COMMAND, 0);
		}
		break;

			/* What do we do with carriage returns? line feeds? */

#if defined(CONFIG_EOL_IS_CR)
		case '\r': {			/* CR terminates line */
			ch = '\n';
			found = true;
		}
		break;

#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\r':				/* Wait for the LF */
			break;
#endif

#if defined(CONFIG_EOL_IS_LF) || defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\n': {			/* LF terminates line */
			found = true;
		}
		break;
#endif

#ifdef CONFIG_EOL_IS_EITHER_CRLF
		case '\r':				/* Either CR or LF terminates line */
		case '\n': {
			ch = '\n';
			found = true;
		}
		break;
#endif

		default: {
			/* Ignore all but printable characters and tab */

			if (isprint(ch) || ch == '\t') {
				found = true;
			} else {
				VI_BEL(vi);
			}
		}
		break;
		}
	}

	/* Now replace with the character nchar times */

	for (; nchars > 0; nchars--) {
		vi_replacech(vi, ch);
	}

	/* Revert to command mode */

	vi_setmode(vi, MODE_COMMAND, 0);
}

/****************************************************************************
 * Insert and Replace Mode Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_insertch
 *
 * Description:
 *   Insert one character into the text buffer
 *
 ****************************************************************************/

static void vi_insertch(FAR struct vi_s *vi, char ch)
{
	vivdbg("curpos=%ld ch=%c[%02x]\n", vi->curpos, isprint(ch) ? ch : '.', ch);

	/* Make space in the buffer for the new character */

	if (vi_extendtext(vi, vi->curpos, 1)) {
		/* Add the new character to the buffer */

		vi->text[vi->curpos++] = ch;
	}
}

/****************************************************************************
 * Name: vi_insert_mode
 *
 * Description:
 *   Insert mode loop
 *
 ****************************************************************************/

static void vi_insert_mode(FAR struct vi_s *vi)
{
	int ch;

	vivdbg("Enter insert mode\n");

	/* Loop while we are in insert mode */

	while (vi->mode == MODE_INSERT) {
		/* Make sure that the display reflects the current state */

		vi_showtext(vi);
		vi_setcursor(vi, vi->cursor.row, vi->cursor.column);

		/* Get the next character from the input */

		ch = vi_getch(vi);

		/* Any key press clears the error message */

		vi->error = false;

		/* Handle the newly received character */

		switch (ch) {
		case KEY_INSMODE_QUOTE: {	/* Quoted character follows */
			/* Insert the next character unconditionally */

			vi_insertch(vi, vi_getch(vi));
		}
		break;

		case ASCII_DEL: {
			if (vi->curpos < vi->textsize) {
				vi_shrinktext(vi, vi->curpos, 1);
			}
		}
		break;

		case ASCII_BS: {
			if (vi->curpos > 0) {
				vi_shrinktext(vi, --vi->curpos, 1);
			}
		}
		break;

		case ASCII_ESC: {	/* Escape exits insert mode */
			vi_setmode(vi, MODE_COMMAND, 0);
		}
		break;

			/* What do we do with carriage returns? */

#if defined(CONFIG_EOL_IS_CR)
		case '\r': {			/* CR terminates line */
			vi_insertch(vi, '\n');
		}
		break;

#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\r':				/* Wait for the LF */
			break;
#endif

#if defined(CONFIG_EOL_IS_LF) || defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\n': {			/* LF terminates line */
			vi_insertch(vi, '\n');
		}
		break;
#endif

#ifdef CONFIG_EOL_IS_EITHER_CRLF
		case '\r':				/* Either CR or LF terminates line */
		case '\n': {
			vi_insertch(vi, '\n');
		}
		break;
#endif

		default: {
			/* Ignore all control characters except for tab and newline */

			if (!iscntrl(ch) || ch == '\t') {
				/* Insert the filtered character into the buffer */

				vi_insertch(vi, ch);
			} else {
				VI_BEL(vi);
			}
		}
		break;
		}
	}
}

/****************************************************************************
 * Name: vi_replace_mode
 *
 * Description:
 *   Replace mode loop
 *
 ****************************************************************************/

static void vi_replace_mode(FAR struct vi_s *vi)
{
	off_t start = vi->curpos;
	int ch;

	vivdbg("Enter replace mode\n");

	/* Loop until ESC is pressed */

	while (vi->mode == MODE_REPLACE) {
		/* Make sure that the display reflects the current state */

		vi_showtext(vi);
		vi_setcursor(vi, vi->cursor.row, vi->cursor.column);

		/* Get the next character from the input */

		ch = vi_getch(vi);

		/* Any key press clears the error message */

		vi->error = false;

		/* Handle the newly received character */

		switch (ch) {
		case KEY_FINDMODE_QUOTE: {	/* Quoted character follows */
			/* Replace the next character unconditionally */

			vi_replacech(vi, ch);
		}
		break;

		case ASCII_BS: {		/* Delete the character before the cursor */
			if (vi->curpos > start) {
				vi->curpos = vi_cursorleft(vi, vi->curpos, 1);
			}
		}
		break;

		case ASCII_ESC: {	/* Escape exits find mode */
			vi_setmode(vi, MODE_COMMAND, 0);
		}
		break;

			/* What do we do with carriage returns? line feeds? */

#if defined(CONFIG_EOL_IS_CR)
		case '\r': {			/* CR terminates line */
			vi_replacech(vi, '\n');
		}
		break;

#elif defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\r':				/* Wait for the LF */
			break;
#endif

#if defined(CONFIG_EOL_IS_LF) || defined(CONFIG_EOL_IS_BOTH_CRLF)
		case '\n': {			/* LF terminates line */
			vi_replacech(vi, '\n');
		}
		break;
#endif

#ifdef CONFIG_EOL_IS_EITHER_CRLF
		case '\r':				/* Either CR or LF terminates line */
		case '\n': {
			vi_replacech(vi, '\n');
		}
		break;
#endif

		default: {
			/* Ignore all but printable characters and TABs */

			if (isprint(ch) || ch == '\t') {
				/* Insert the filtered character into the text buffer */

				vi_replacech(vi, ch);
			} else {
				VI_BEL(vi);
			}
		}
		break;
		}
	}
}

/****************************************************************************
 * Command line processing
 ****************************************************************************/

/****************************************************************************
 * Name: vi_showusage
 *
 * Description:
 *   Show command line arguments and exit.
 *
 ****************************************************************************/

static void vi_release(FAR struct vi_s *vi)
{
	if (vi) {
		if (vi->text) {
			free(vi->text);
		}

		if (vi->yank) {
			free(vi->yank);
		}

		free(vi);
	}
}

/****************************************************************************
 * Name: vi_showusage
 *
 * Description:
 *   Show command line arguments and exit.
 *
 ****************************************************************************/

static void vi_showusage(FAR struct vi_s *vi, FAR const char *progname, int exitcode)
{
	fprintf(stderr, "\nUSAGE:\t%s [-c <columns] [-r <rows>] [<filename>]\n", progname);
	fprintf(stderr, "\nUSAGE:\t%s -h\n\n", progname);
	fprintf(stderr, "Where:\n");
	fprintf(stderr, "\t<filename>:\n");
	fprintf(stderr, "\t\tOptional name of the file to open\n");
	fprintf(stderr, "\t-c <columns>:\n");
	fprintf(stderr, "\t\tOptional width of the display in columns.  Default: %d\n", CONFIG_SYSTEM_VI_COLS);
	fprintf(stderr, "\t-r <rows>:\n");
	fprintf(stderr, "\t\tOptional height of the display in rows.  Default: %d\n", CONFIG_SYSTEM_VI_ROWS);
	fprintf(stderr, "\t-h:\n");
	fprintf(stderr, "\t\tShows this message and exits.\n");

	/* Release all allocated resources and exit */

	vi_release(vi);
	exit(exitcode);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vi_main
 *
 * Description:
 *   The main entry point into vi.
 *
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int vi_main(int argc, char **argv)
#endif
{
	FAR struct vi_s *vi;
	int option;

	/* Allocate a vi state structure */

	vi = (FAR struct vi_s *)zalloc(sizeof(struct vi_s));
	if (!vi) {
		fprintf(stderr, "ERROR: %s\n", g_fmtallocfail);
		return EXIT_FAILURE;
	}

	/* Initialize non-zero elements of the vi state structure */

	vi->display.row = CONFIG_SYSTEM_VI_ROWS;
	vi->display.column = CONFIG_SYSTEM_VI_COLS;

	/* Parse command line arguments */

	while ((option = getopt(argc, argv, ":c:r:h")) != ERROR) {
		switch (option) {
		case 'c': {			/* Display width in columns */
			unsigned long value = strtoul(optarg, NULL, 10);
			if (value <= UINT16_MAX) {
				vi->display.column = (uint16_t)value;
			} else {
				fprintf(stderr, "ERROR: Column value out of range: %lu\n", value);
				vi_showusage(vi, argv[0], EXIT_FAILURE);
			}
		}
		break;

		case 'r': {			/* Display width in columns */
			unsigned long value = strtoul(optarg, NULL, 10);
			if (value <= UINT16_MAX) {
				vi->display.row = (uint16_t)value;
			} else {
				fprintf(stderr, "ERROR: Row value out of range: %lu\n", value);
				vi_showusage(vi, argv[0], EXIT_FAILURE);
			}
		}
		break;

		case 'h': {
			vi_showusage(vi, argv[0], EXIT_SUCCESS);
		}
		break;

		case ':': {
			fprintf(stderr, "ERROR: Missing parameter argument\n");
			vi_showusage(vi, argv[0], EXIT_FAILURE);
		}
		break;

		case '?':
		default: {
			fprintf(stderr, "ERROR: Unrecognized parameter\n");
			vi_showusage(vi, argv[0], EXIT_FAILURE);
		}
		break;
		}
	}

	/* There may be one additional argument on the command line:  The filename */

	if (optind < argc) {
		/* Copy the file name into the file name buffer */

		strncpy(vi->filename, argv[optind], MAX_STRING - 1);

		/* Make sure that the (possibly truncated) file name is NUL terminated */

		vi->filename[MAX_STRING - 1] = '\0';

		/* Load the file into memory */

		(void)vi_insertfile(vi, 0, vi->filename);

		/* Skip over the filename argument.  There should nothing after this */

		optind++;
	}

	if (optind != argc) {
		fprintf(stderr, "ERROR: Too many arguments\n");
		vi_showusage(vi, argv[0], EXIT_FAILURE);
	}

	/* The editor loop */

	for (;;) {
		/* We loop, processing each mode change */

		vivdbg("mode=%d\n", vi->mode);

		switch (vi->mode) {
		default:
		case MODE_COMMAND:		/* Command mode */
			vi_cmd_mode(vi);
			break;

		case SUBMODE_COLON:	/* Colon data entry in command mode */
			vi_cmd_submode(vi);
			break;

		case SUBMODE_FIND:		/* Find data entry in command mode */
			vi_find_submode(vi);
			break;

		case SUBMODE_REPLACECH:	/* Replace characters in command mode */
			vi_replacech_submode(vi);
			break;

		case MODE_INSERT:		/* Insert mode */
			vi_insert_mode(vi);
			break;

		case MODE_REPLACE:		/* Replace character(s) under cursor until ESC */
			vi_replace_mode(vi);
			break;
		}
	}

	/* We won't get here */
}
