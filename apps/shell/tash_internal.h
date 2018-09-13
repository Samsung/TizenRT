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
/// @file   tash_internal.h
/// @brief  header file only for TASH (private)

#ifndef __APPS_SHELL_TASH_INTERNAL_H
#define __APPS_SHELL_TASH_INTERNAL_H
#include <tinyara/config.h>
#include <stdlib.h>
#include <debug.h>

#define tash_alloc(a) malloc(a)
#define tash_free(a)  free(a)

#ifdef CONFIG_CPP_HAVE_VARARGS
#ifdef CONFIG_DEBUG_TASH
#ifdef CONFIG_DEBUG_TASH_ERROR
#define shdbg(format, ...)        dbg(format, ##__VA_ARGS__)
#endif
#ifdef CONFIG_DEBUG_TASH_INFO
#define shvdbg(format, ...)       vdbg(format, ##__VA_ARGS__)
#endif
#else  /* !CONFIG_DEBUG_TASH */
#define shdbg(...)
#define shvdbg(...)
#endif /* CONFIG_DEBUG_TASH */
#else  /* !CONFIG_CPP_HAVE_VARARGS */
#ifdef CONFIG_DEBUG_TASH
#ifdef CONFIG_DEBUG_TASH_ERROR
#define shdbg        dbg
#endif
#ifdef CONFIG_DEBUG_TASH_INFO
#define shvdbg       vdbg
#endif
#else  /* !CONFIG_DEBUG_TASH */
#define shdbg        (void)
#define shvdbg       (void)
#endif /* CONFIG_DEBUG_TASH */
#endif /* CONFIG_CPP_HAVE_VARARGS */

extern void tash_register_basic_cmds(void);
extern int tash_execute_cmdline(char *buff);
extern int tash_execute_cmd(char **args, int argc);
extern int tash_init(void);
#ifdef CONFIG_TASH_SCRIPT
extern int tash_script(int argc, char **args);
#endif
#ifndef CONFIG_DISABLE_SIGNALS
extern int tash_sleep(int argc, char **args);
#ifdef CONFIG_TASH_USLEEP
extern int tash_usleep(int argc, char **args);
#endif
#endif

#endif							/* __APPS_SHELL_TASH_INTERNAL_H */
