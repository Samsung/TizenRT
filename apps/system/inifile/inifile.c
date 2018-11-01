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
 * apps/system/inifile/inifile.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <debug.h>

#include <apps/inifile.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The maximum size of a line in the INI file */

#ifndef CONFIG_SYSTEM_INIFILE_MAXLINE
#define CONFIG_SYSTEM_INIFILE_MAXLINE    256
#endif

#ifndef CONFIG_SYSTEM_INIFILE_DEBUGLEVEL
#define CONFIG_SYSTEM_INIFILE_DEBUGLEVEL 0
#endif

#ifdef CONFIG_CPP_HAVE_VARARGS
#if CONFIG_SYSTEM_INIFILE_DEBUGLEVEL > 0
#define inidbg(format, ...) printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define inidbg(...)
#endif

#if CONFIG_SYSTEM_INIFILE_DEBUGLEVEL > 1
#define inivdbg(format, ...) printf(EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#else
#define inivdbg(...)
#endif
#else
#if CONFIG_SYSTEM_INIFILE_DEBUGLEVEL > 0
#define inidbg printf
#else
#define inidbg (void)
#endif

#if CONFIG_SYSTEM_INIFILE_DEBUGLEVEL > 1
#define inivdbg printf
#else
#define inivdbg (void)
#endif
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* A structure that describes one entry from the INI file */

struct inifile_var_s {
	FAR char *variable;
	FAR char *value;
};

/* This structure describes the state of one instance of the INI file parser */

struct inifile_state_s {
	FILE *instream;
	int   nextch;
	char  line[CONFIG_SYSTEM_INIFILE_MAXLINE + 1];
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static bool      inifile_next_line(FAR struct inifile_state_s *priv);
static int       inifile_read_line(FAR struct inifile_state_s *priv);
static int       inifile_read_noncomment_line(FAR struct inifile_state_s *priv);
static bool      inifile_seek_to_section(FAR struct inifile_state_s *priv, FAR const char *section);
static bool      inifile_read_variable(FAR struct inifile_state_s *priv, FAR struct inifile_var_s *varinfo);
static FAR char *inifile_find_section_variable(FAR struct inifile_state_s *priv, FAR const char *variable);
static FAR char *inifile_find_variable(FAR struct inifile_state_s *priv, FAR const char *section, FAR const char *variable);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  inifile_next_line
 *
 * Description:
 *   Skip to the first character of the next line.  Returns true if the end
 *   of file was not encountered.
 *
 ****************************************************************************/

static bool inifile_next_line(FAR struct inifile_state_s *priv)
{
	/* Search ahead for the end of line mark (or possibly the end of file mark) */

	while ((priv->nextch != '\n') && (priv->nextch != EOF)) {
		priv->nextch = getc(priv->instream);
	}

	/* Re-prime the pump with the first character from the next line.  NOTE:
	 * this logic depends on the fact that getc() will return EOF repeatedly.
	 */

	priv->nextch = getc(priv->instream);
	return (priv->nextch != EOF);
}

/****************************************************************************
 * Name:  inifile_read_line
 *
 * Description:
 *   Read the next line from the INI file into the line buffer and return
 *   the number of characters read into the buffer. If we hit the end of a
 *   section (or the end of a file), this function will return a count of
 *   zero.
 *
 ****************************************************************************/

static int inifile_read_line(FAR struct inifile_state_s *priv)
{
	int nbytes;

	/* Assuming that the file pointer is correctly positioned at the beginning
	 * of the next line, read until the end of line indication is found (or
	 * until the line buffer is full).  This is basically fgets().
	 */

	nbytes = 0;
	while ((nbytes < CONFIG_SYSTEM_INIFILE_MAXLINE) && (priv->nextch != EOF) && (priv->nextch != '\n')) {
		/* Always ignore carriage returns */

		if (priv->nextch != '\r') {
			/* Ignore any leading whitespace on the line */

			if (nbytes || (priv->nextch != ' ' && priv->nextch != '\t')) {
				/* Add the new character to the line buffer */

				priv->line[nbytes] = priv->nextch;
				nbytes++;
			}
		}

		/* Get the next character from the INI file */

		priv->nextch = getc(priv->instream);
	}

	/* NUL terminate the string */

	priv->line[nbytes] = '\0';

	/* Skip to the first character of the next line.  This should normally
	 * just amount to skipping over the newline, but could be more involved
	 * if we had to truncate the line to fit into the line buffer.
	 */

	if (priv->nextch != EOF) {
		(void)inifile_next_line(priv);
	}

	/* And return the number of bytes read (excluding the NUL terminator and
	 * leading whitespace).
	 */

	return nbytes;
}

/****************************************************************************
 * Name:  inifile_read_noncomment_line
 *
 * Description:
 *   Read until either a (1) no further lines are found in the file, or (2)
 *   a line that does not begin with a semi-colon is found
 *
 ****************************************************************************/

static int inifile_read_noncomment_line(FAR struct inifile_state_s *priv)
{
	int nbytes;

	/* Read until either a (1) no further lines are found in
	 * the file, or (2) a line that does not begin with a semi-colon
	 * is found */

	do {
		nbytes = inifile_read_line(priv);
	} while (nbytes > 0 && priv->line[0] == ';');

	return nbytes;
}

/****************************************************************************
 * Name:  inifile_seek_to_section
 *
 * Description:
 *   Positions the file pointer to the line containing the first variable
 *   description within the INI file. Returns 1 if the section was found.
 *   In this case, the file pointer will be positioned at the beginning of
 *   the first variable line.
 *
 ****************************************************************************/

static bool inifile_seek_to_section(FAR struct inifile_state_s *priv, FAR const char *section)
{
	int nbytes;

	/* Rewind to the beginning of the INI file and re-prime the pump with the
	 * first character from the INI file.
	 */

	rewind(priv->instream);
	priv->nextch = getc(priv->instream);

	/* Loop until either the section is found, or until we hit the end of the
	 * INI file.
	 */

	do {
		/* Read the next line into the input buffer.  A returned value of zero
		 * bytes means nothing special here -- could be EOF or a blank line.
		 */

		nbytes = inifile_read_noncomment_line(priv);

		/* It takes at least three bytes of data to be a candidate for a
		 * section header.
		 */

		if (nbytes >= 3) {
			/* A section header must begin with a left bracket */

			if (priv->line[0] == '[') {
				/* The section name should start with the first character
				 * after the left bracket.
				 */

				FAR char *sectend;

				/* The section name should extend to the right bracket. */

				sectend = strchr(&priv->line[1], ']');

				/* Replace the right bracket (if found) with a NULL
				 * terminator.
				 */

				if (sectend) {
					*sectend = '\0';
				}

				/* Then compare the section name to the one we are looking for */

				if (strcasecmp(&priv->line[1], section) == 0) {
					/* The section names match!  Return success */

					return true;
				}
			}
		}
	} while (priv->nextch != EOF);

	/* If we got here, we search the whole INI file without finding
	 * the requested section
	 */

	inidbg("ERROR: Section \"%s\" not found\n", section);
	return false;
}

/****************************************************************************
 * Name:  inifile_read_variable
 *
 * Description:
 *   Obtain variable info from the next line in the section.  This assumes
 *   that the file pointer is pointing to the beginning of the next line.
 *   If there is no further data in the section, false is returned.
 *
 ****************************************************************************/

static bool inifile_read_variable(FAR struct inifile_state_s *priv, FAR struct inifile_var_s *varinfo)
{
	FAR char *ptr;

	/* Read until either (1) the end of file is found, (2) the end of
	 * the section is found, or (3) a valid variable assignment is found.
	 */

	for (;;) {
		/* Read the next line in the buffer */

		int nbytes = inifile_read_noncomment_line(priv);

		/* Make sure that the line is non-NULL and that this is not the
		 * beginning of a new section
		 */

		if (!nbytes || priv->line[0] == '[') {
			return false;
		}

		/* Search for the '=' delimiter.  NOTE  the line is guaranteed to
		 * be NULL terminated by inifile_read_noncomment_line().
		 */

		ptr = strchr(&priv->line[1], '=');

		/* If the delimiter was found, return success */

		if (ptr) {
			/* Put NUL termination between the variable name and the
			 * variable value (replacing the equal sign).
			 */

			*ptr = '\0';

			/* Set up the return structure.  NOTE:  value may point at
			 * a NULL string
			 */

			varinfo->variable = (char *)priv->line;
			varinfo->value = (ptr + 1);
			return true;
		}
	}
}

/****************************************************************************
 * Name:  inifile_find_section_variable
 *
 * Description:
 *   Find the value string associated with the variable name.  This function
 *   will return NULL on failure to find the variable.  It will return a
 *   pointer to an empty string is the variable is found, but not assigned a
 *   value.
 *
 ****************************************************************************/

static FAR char *inifile_find_section_variable(FAR struct inifile_state_s *priv, FAR const char *variable)
{
	/* Loop until either (1) we hit the end of file, (2) we hit the end
	 * of the section, or (3) we find the variable that we are looking
	 * for/
	 */

	inivdbg("variable=\"%s\"\n", variable);

	for (;;) {
		/* Get the next variable from this section. */

		struct inifile_var_s varinfo;
		bool found = inifile_read_variable(priv, &varinfo);

		/* Is there anything left in the section? */

		if (!found) {
			inivdbg("Returning NULL\n");
			return NULL;
		}

		inivdbg("varinfo.variable=\"%s\"\n", varinfo.variable);

		/* Does the the variable name match the one we are looking for? */

		if (strcasecmp(varinfo.variable, variable) == 0) {
			/* Yes... then we have it! */

			inivdbg("Returning \"%s\"\n", varinfo.value);
			return varinfo.value;
		}
	}
}

/****************************************************************************
 * Name:  inifile_find_variable
 *
 * Description:
 *   Obtains the specified string value for the specified variable name
 *   within the specified section of the INI file.
 *
 ****************************************************************************/

static FAR char *inifile_find_variable(FAR struct inifile_state_s *priv, FAR const char *section, FAR const char *variable)
{
	FAR char *ret = NULL;

	inivdbg("section=\"%s\" variable=\"%s\"\n", section, variable);

	/* Seek to the first variable in the specified section of the INI file */

	if (priv->instream && inifile_seek_to_section(priv, section)) {
		/* If the seek was successful, then find the value string within
		 * the section
		 */

		FAR char *value = inifile_find_section_variable(priv, variable);
		inivdbg("variable_value=0x%p\n", value);

		if (value && *value) {
			inivdbg("variable_value=\"%s\"\n", value);
			ret = value;
		}
	}

	/* Return the string that we found. */

	inivdbg("Returning 0x%p\n", ret);
	return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  inifile_initialize
 *
 * Description:
 *   Initialize for access to the INI file 'inifile_name'
 *
 ****************************************************************************/

INIHANDLE inifile_initialize(FAR const char *inifile_name)
{
	/* Allocate an INI file parser state structure */

	FAR struct inifile_state_s *priv = (FAR struct inifile_state_s *)malloc(sizeof(struct inifile_state_s));

	if (!priv) {
		inidbg("ERROR: Failed to allocate state structure\n");
		return (INIHANDLE)NULL;
	}

	/* Open the specified INI file for reading */

	priv->instream = fopen(inifile_name, "r");

	/* Prime the pump */

	if (priv->instream) {
		priv->nextch = getc(priv->instream);
		return (INIHANDLE)priv;
	} else {
		inidbg("ERROR: Could not open \"%s\"\n", inifile_name);
		return (INIHANDLE)NULL;
	}
}

/****************************************************************************
 * Name:  inifile_uninitialize
 *
 * Description:
 *   Free resources commit to INI file parsing
 *
 ****************************************************************************/

void inifile_uninitialize(INIHANDLE handle)
{
	FAR struct inifile_state_s *priv = (FAR struct inifile_state_s *)handle;

	if (priv) {
		/* Close the INI file stream */

		if (priv->instream) {
			fclose(priv->instream);
		}

		/* Release the state structure */

		free(priv);
	}
}

/****************************************************************************
 * Name: inifile_read_string
 *
 * Description:
 *   Obtains the specified string value for the specified variable name
 *   within the specified section of the INI file.  The receiver of the
 *   value string should call inifile_free_string when it no longer needs
 *   the memory held by the value string.
 *
 ****************************************************************************/

FAR char *inifile_read_string(INIHANDLE handle, FAR const char *section, FAR const char *variable, FAR const char *defvalue)
{
	FAR struct inifile_state_s *priv = (FAR struct inifile_state_s *)handle;
	FAR char *ret = NULL;
	FAR const char *value;

	/* Get a reference to the volatile version of the string */

	value = inifile_find_variable(priv, section, variable);

	/* If the variable was not found, then use the default value */

	if (!value) {
		/* Selecting the default string */

		value = defvalue;
	}

	/* If this was successful, create a non-volatile copy of the string
	 * We do this even if the default value is used because the caller
	 * will (eventually) deallocate it.
	 */

	if (value) {
		ret = strdup(value);
	}

	/* Return the string that we found. */

	return ret;
}

/****************************************************************************
 * Name:  inifile_read_integer
 *
 * Description:
 *   Obtains the specified integer value for the specified variable name
 *   within the specified section of the INI file
 *
 ****************************************************************************/

long inifile_read_integer(INIHANDLE handle, FAR const char *section, FAR const char *variable, FAR long defvalue)
{
	FAR struct inifile_state_s *priv = (FAR struct inifile_state_s *)handle;
	FAR char *value;
	long ret = defvalue;

	/* Assume failure to find the requested value */

	inivdbg("section=\"%s\" variable=\"%s\" defvalue=%d\n", section, variable, defvalue);

	/* Get the value as a string first */

	value = inifile_find_variable(priv, section, variable);

	/* If this was successful, then convert the string to an integer value. */

	if (value) {
		/* Then convert the string to an integer value (accept any base, and
		 * ignore all conversion errors.
		 */

		inivdbg("%s=\"%s\"\n", variable, value);
		ret = strtol(value, NULL, 0);
	}

	/* Return the value that we found. */

	inivdbg("Returning %d\n", ret);
	return ret;
}

/****************************************************************************
 * Name:  inifile_free_string
 *
 * Description:
 *   Release resources allocated for the value string previously obtained
 *   from inifile_read_string.  The purpose of this inline function is to
 *   hide the memory allocator used by this implementation.
 *
 ****************************************************************************/

void inifile_free_string(FAR char *value)
{
	if (value) {
		free(value);
	}
}
