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
 * tools/mksyscall.c
 *
 *   Copyright (C) 2011-2013 Gregory Nutt. All rights reserved.
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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "csvparser.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

static bool g_inline;
static FILE *g_stubstream;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static bool is_vararg(const char *type, int ndx, int nparms)
{
	if (strcmp(type, "...") == 0) {
		if (ndx != (nparms - 1)) {
			fprintf(stderr, "%d: ... is not the last in the argument list\n", get_lineno());
			exit(11);
		} else if (nparms < 2) {
			fprintf(stderr, "%d: Need one parameter before ...\n", get_lineno());
			exit(14);
		}

		return true;
	}

	return false;
}

static bool is_union(const char *type)
{
	return (strncmp(type, "union", 5) == 0);
}

static bool is_noreturn_func(const char *func_name)
{
	return ((strncmp(func_name, "_exit", 5) == 0) ||
			(strncmp(func_name, "exit", 4) == 0) ||
			(strncmp(func_name, "pthread_exit", 12) == 0));
}

static const char *check_funcptr(const char *type)
{
	const char *str = strstr(type, "(*)");
	if (str)
		return str + 2;

	return NULL;
}

static const char *check_array(const char *type)
{
	const char *str = strchr(type, '[');
	if (str)
		return str;

	return NULL;
}

static void print_formalparm(FILE * stream, const char *argtype, int parmno)
{
	const char *part2;
	int len;

	/* Function pointers and array formal parameter types are a little more work */

	if ((part2 = check_funcptr(argtype)) != NULL || (part2 = check_array(argtype)) != NULL) {
		len = part2 - argtype;
		(void)fwrite(argtype, 1, len, stream);
		fprintf(stream, "parm%d%s", parmno, part2);
	} else
		fprintf(stream, "%s parm%d", argtype, parmno);
}

static void get_formalparmtype(const char *arg, char *formal)
{
	/* The formal parm type is a pointer to everything up to the first'|' (or
	 * the end of the string if there is no '|' in the type description).
	 */

	while (*arg != '|' && *arg != '\0')
		*formal++ = *arg++;
	*formal = '\0';
}

static void get_actualparmtype(const char *arg, char *actual)
{
	const char *pstart = strchr(arg, '|');
	if (pstart) {
		/* The actual parameter type starts after the '|' */

		pstart++;
	} else {
		/* The actual parameter is the same as the formal parameter
		 * at starts at the beginning of the type string.
		 */

		pstart = arg;
	}

	/* The actual parm type is a pointer to everything up to the next '|' (or
	 * the end of the string if there is no '|' in the type description).
	 */

	while (*pstart != '|' && *pstart != '\0')
		*actual++ = *pstart++;
	*actual = '\0';
}

static void get_fieldname(const char *arg, char *fieldname)
{
	char *pactual = strchr(arg, '|');
	char *pstart;

	if (pactual) {
		/* The actual parameter type starts after the '|' */

		pactual++;
		pstart = strchr(pactual, '|');
		if (pstart) {
			/* The fieldname is everything past the second '|' to the end of the string */

			pstart++;
			strncpy(fieldname, pstart, MAX_PARMSIZE);
			return;
		}
	}

	fprintf(stderr, "%d: Missing union fieldname: %s\n", get_lineno(), arg);
	exit(15);
}

static FILE *open_proxy(void)
{
	char filename[MAX_PARMSIZE + 10];
	FILE *stream;

	snprintf(filename, MAX_PARMSIZE + 9, "PROXY_%s.c", get_parm(NAME_INDEX));
	filename[MAX_PARMSIZE + 9] = '\0';

	stream = fopen(filename, "w");
	if (stream == NULL) {
		fprintf(stderr, "Failed to open %s: %s\n", filename, strerror(errno));
		exit(10);
	}
	return stream;
}

static void generate_proxy(int nparms)
{
	FILE *stream = open_proxy();
	char formal[MAX_PARMSIZE];
	char fieldname[MAX_PARMSIZE];
	bool bvarargs = false;
	int nformal;
	int nactual;
	char *cond_parm;
	int i;

	/* Generate "up-front" information, include correct header files */

	fprintf(stream, "/* Auto-generated %s proxy file -- do not edit */\n\n", get_parm(NAME_INDEX));
	fprintf(stream, "#include <tinyara/config.h>\n");

	/* Does this function have a variable number of parameters?  If so then the
	 * final parameter type will be encoded as "..."
	 */

	if (is_vararg(get_parm(PARM1_INDEX + nparms - 1), nparms - 1, nparms)) {
		nformal = nparms - 1;
		bvarargs = true;
		fprintf(stream, "#include <stdarg.h>\n");
	} else
		nformal = nparms;

	if (get_parm(HEADER_INDEX) && strlen(get_parm(HEADER_INDEX)) > 0)
		fprintf(stream, "#include <%s>\n", get_parm(HEADER_INDEX));

	fprintf(stream, "#include <syscall.h>\n\n");

	cond_parm = get_parm(COND_INDEX);
	if (cond_parm[0] != '\0')
		fprintf(stream, "#if %s\n\n", cond_parm);

	/* Generate the function definition that matches standard function prototype */

	fprintf(stream, "%s %s(", get_parm(RETTYPE_INDEX), get_parm(NAME_INDEX));

	/* Generate the formal parameter list */

	if (nformal <= 0)
		fprintf(stream, "void");
	else {
		for (i = 0; i < nformal; i++) {
			/* The formal and actual parameter types may be encoded.. extra the
			 * formal parameter type.
			 */

			get_formalparmtype(get_parm(PARM1_INDEX + i), formal);

			/* Arguments after the first must be separated from the preceding
			 * parameter with a comma.
			 */

			if (i > 0)
				fprintf(stream, ", ");

			print_formalparm(stream, formal, i + 1);
		}
	}

	/* Handle the end of the formal parameter list */

	if (bvarargs) {
		fprintf(stream, ", ...)\n{\n");

		/* Get parm variables .. some from the parameter list and others from
		 * the varargs.
		 */

		if (nparms < 7) {
			fprintf(stream, "  va_list ap;\n");
			for (i = nparms; i < 7; i++)
				fprintf(stream, "  uintptr_t parm%d;\n", i);

			fprintf(stream, "\n  va_start(ap, parm%d);\n", nparms - 1);
			for (i = nparms; i < 7; i++)
				fprintf(stream, "  parm%d = va_arg(ap, uintptr_t);\n", i);
			fprintf(stream, "  va_end(ap);\n\n");
		}
	} else
		fprintf(stream, ")\n{\n");

	/* Generate the system call.  Functions that do not return or return void
	 * are special cases.
	 */

	nactual = bvarargs ? 6 : nparms;
	if (strcmp(get_parm(RETTYPE_INDEX), "void") == 0)
		fprintf(stream, "  (void)sys_call%d(", nactual);
	else
		fprintf(stream, "  return (%s)sys_call%d(", get_parm(RETTYPE_INDEX), nactual);

	/* Create the parameter list with the matching types.  The first parameter
	 * is always the syscall number.
	 */

	fprintf(stream, "(unsigned int)SYS_%s", get_parm(NAME_INDEX));

	for (i = 0; i < nactual; i++) {
		/* Is the parameter a union member */

		if (i < nparms && is_union(get_parm(PARM1_INDEX + i))) {
			/* Then we will have to pick a field name that can be cast to a
			 * uintptr_t.  There probably should be some error handling here
			 * to catch the case where the fieldname was not supplied.
			 */

			get_fieldname(get_parm(PARM1_INDEX + i), fieldname);
			fprintf(stream, ", (uintptr_t)parm%d.%s", i + 1, fieldname);
		} else
			fprintf(stream, ", (uintptr_t)parm%d", i + 1);
	}

	/* Handle the tail end of the function. */

	if (is_noreturn_func(get_parm(NAME_INDEX))) {
		fprintf(stream, ");%s\n}\n\n", "\n  while (1);");
	} else {
		fprintf(stream, ");\n}\n\n");
	}

	cond_parm = get_parm(COND_INDEX);
	if (cond_parm[0] != '\0')
		fprintf(stream, "#endif /* %s */\n", cond_parm);

	fclose(stream);
}

static FILE *open_stub(void)
{
	if (g_inline) {
		if (!g_stubstream) {
			g_stubstream = fopen("STUB.h", "w");
			if (g_stubstream == NULL) {
				fprintf(stderr, "Failed to open STUB.h: %s\n", strerror(errno));
				exit(9);
			}
			fprintf(g_stubstream, "/* Autogenerated STUB header file */\n\n");
			fprintf(g_stubstream, "#ifndef __STUB_H\n");
			fprintf(g_stubstream, "#define __STUB_H\n\n");
		}

		return g_stubstream;
	} else {
		char filename[MAX_PARMSIZE + 8];
		FILE *stream;

		snprintf(filename, MAX_PARMSIZE + 7, "STUB_%s.c", get_parm(NAME_INDEX));
		filename[MAX_PARMSIZE + 7] = '\0';

		stream = fopen(filename, "w");
		if (stream == NULL) {
			fprintf(stderr, "Failed to open %s: %s\n", filename, strerror(errno));
			exit(9);
		}
		return stream;
	}
}

static void stub_close(FILE * stream)
{
	if (!g_inline)
		fclose(stream);
}

static void generate_stub(int nparms)
{
	FILE *stream = open_stub();
	char formal[MAX_PARMSIZE];
	char actual[MAX_PARMSIZE];
	char *cond_parm;
	int i;
	int j;

	/* Generate "up-front" information, include correct header files */

	fprintf(stream, "/* Auto-generated %s stub file -- do not edit */\n\n", get_parm(0));
	fprintf(stream, "#include <tinyara/config.h>\n");
	fprintf(stream, "#include <stdint.h>\n");

	if (get_parm(HEADER_INDEX) && strlen(get_parm(HEADER_INDEX)) > 0)
		fprintf(stream, "#include <%s>\n", get_parm(HEADER_INDEX));

	putc('\n', stream);

	cond_parm = get_parm(COND_INDEX);
	if (cond_parm[0] != '\0')
		fprintf(stream, "#if %s\n\n", cond_parm);

	/* Generate the function definition that matches standard function prototype */

	if (g_inline)
		fprintf(stream, "static inline ");

	fprintf(stream, "uintptr_t STUB_%s(int nbr", get_parm(NAME_INDEX));

	/* Generate the formal parameter list */

	for (i = 0; i < nparms; i++) {
		/* Check for a variable number of arguments */

		if (is_vararg(get_parm(PARM1_INDEX + i), i, nparms)) {
			/* Always receive six arguments in this case */

			for (j = i + 1; j <= 6; j++)
				fprintf(stream, ", uintptr_t parm%d", j);
		} else
			fprintf(stream, ", uintptr_t parm%d", i + 1);
	}

	fprintf(stream, ")\n{\n");

	/* Then call the proxied function.  Functions that have no return value are
	 * a special case.
	 */

	if (strcmp(get_parm(RETTYPE_INDEX), "void") == 0)
		fprintf(stream, "  %s(", get_parm(NAME_INDEX));
	else
		fprintf(stream, "  return (uintptr_t)%s(", get_parm(NAME_INDEX));

	/* The pass all of the system call parameters, casting to the correct type
	 * as necessary.
	 */

	for (i = 0; i < nparms; i++) {
		/* Get the formal type of the parameter, and get the type that we
		 * actually have to cast to.  For example for a formal type like 'int parm[]'
		 * we have to cast the actual parameter to 'int*'.  The worst is a union
		 * type like 'union sigval' where we have to cast to (union sigval)((FAR void *)parm)
		 * -- Yech.
		 */

		get_formalparmtype(get_parm(PARM1_INDEX + i), formal);
		get_actualparmtype(get_parm(PARM1_INDEX + i), actual);

		/* Treat the first argument in the list differently from the others..
		 * It does not need a comma before it.
		 */

		if (i > 0) {
			/* Check for a variable number of arguments */

			if (is_vararg(actual, i, nparms)) {
				/* Always pass six arguments */

				for (j = i + 1; j <= 6; j++)
					fprintf(stream, ", parm%d", j);
			} else {
				if (is_union(formal))
					fprintf(stream, ", (%s)((%s)parm%d)", formal, actual, i + 1);
				else
					fprintf(stream, ", (%s)parm%d", actual, i + 1);
			}
		} else {
			if (is_union(formal))
				fprintf(stream, "(%s)((%s)parm%d)", formal, actual, i + 1);
			else
				fprintf(stream, "(%s)parm%d", actual, i + 1);
		}
	}

	/* Tail end of the function.  If the proxied function has no return
	 * value, just return zero (OK).
	 */

	if (strcmp(get_parm(RETTYPE_INDEX), "void") == 0)
		fprintf(stream, ");\n  return 0;\n}\n\n");
	else
		fprintf(stream, ");\n}\n\n");

	cond_parm = get_parm(COND_INDEX);
	if (cond_parm[0] != '\0')
		fprintf(stream, "#endif /* %s */\n", cond_parm);
	stub_close(stream);
}

static void show_usage(const char *progname)
{
	fprintf(stderr, "USAGE: %s [-p|s|i] <CSV file>\n\n", progname);
	fprintf(stderr, "Where:\n\n");
	fprintf(stderr, "\t-p : Generate proxies\n");
	fprintf(stderr, "\t-s : Generate stubs\n");
	fprintf(stderr, "\t-i : Generate proxies as static inline functions\n");
	fprintf(stderr, "\t-d : Enable debug output\n");
	exit(1);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char **argv, char **envp)
{
	char *csvpath;
	bool proxies = false;
	FILE *stream;
	char *ptr;
	int ch;

	/* Parse command line options */

	set_debug(false);
	g_inline = false;

	while ((ch = getopt(argc, argv, ":dps")) > 0) {
		switch (ch) {
		case 'd':
			set_debug(true);
			break;

		case 'p':
			proxies = true;
			break;

		case 's':
			proxies = false;
			break;

		case 'i':
			g_inline = true;
			break;

		case '?':
			fprintf(stderr, "Unrecognized option: %c\n", optopt);
			show_usage(argv[0]);
			break;

		case ':':
			fprintf(stderr, "Missing option argument, option: %c\n", optopt);
			show_usage(argv[0]);
			break;

		default:
			fprintf(stderr, "Unexpected option: %c\n", ch);
			show_usage(argv[0]);
			break;
		}
	}

	if (optind >= argc) {
		fprintf(stderr, "Missing <CSV file>\n");
		show_usage(argv[0]);
	}

	csvpath = argv[optind];
	if (++optind < argc) {
		fprintf(stderr, "Unexpected garbage at the end of the line\n");
		show_usage(argv[0]);
	}

	/* Open the CSV file */

	stream = fopen(csvpath, "r");
	if (!stream) {
		fprintf(stderr, "open %s failed: %s\n", csvpath, strerror(errno));
		exit(3);
	}

	/* Process each line in the CVS file */

	while ((ptr = read_line(stream)) != NULL) {
		/* Parse the line from the CVS file */

		int nargs = parse_csvline(ptr);
		if (nargs < PARM1_INDEX) {
			fprintf(stderr, "Only %d arguments found: %s\n", nargs, get_line());
			exit(8);
		}

		if (proxies)
			generate_proxy(nargs - PARM1_INDEX);
		else {
			g_stubstream = NULL;
			generate_stub(nargs - PARM1_INDEX);
			if (g_stubstream != NULL) {
				fprintf(g_stubstream, "\n#endif /* __STUB_H */\n");
				fclose(g_stubstream);
			}
		}
	}

	/* Close the CSV file */

	fclose(stream);
	return 0;
}
