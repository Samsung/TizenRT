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
 * apps/tzshlib/tzsh_command.c
 *
 *   Copyright (C) 2007-2018 Gregory Nutt. All rights reserved.
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

#include <string.h>

#ifdef CONFIG_TZSH_BUILTIN_APPS
#include <tinyara/binfmt/builtin.h>
#endif

#if defined(CONFIG_SYSTEM_READLINE) && defined(CONFIG_READLINE_HAVE_EXTMATCH)
#include "apps/readline.h"
#endif

#include "tzsh.h"
#include "tzsh_console.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Help command summary layout */

#define MAX_CMDLEN    12
#define CMDS_PER_LINE 6

#define NUM_CMDS      ((sizeof(g_cmdmap)/sizeof(struct cmdmap_s)) - 1)
#define NUM_CMD_ROWS  ((NUM_CMDS + (CMDS_PER_LINE-1)) / CMDS_PER_LINE)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct cmdmap_s {
	FAR const char *cmd;		/* Name of the command */
	tzsh_cmd_t handler;			/* Function that handles the command */
	uint8_t minargs;			/* Minimum number of arguments (including command) */
	uint8_t maxargs;			/* Maximum number of arguments (including command) */
	FAR const char *usage;		/* Usage instructions for 'help' command */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_HELP
static int cmd_help(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv);
#endif

#ifndef CONFIG_TZSH_DISABLESCRIPT
static int cmd_true(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv);
static int cmd_false(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv);
#endif

#ifndef CONFIG_TZSH_DISABLE_EXIT
static int cmd_exit(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv);
#endif

static int cmd_unrecognized(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct cmdmap_s g_cmdmap[] = {
#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_TEST)
	{"[", cmd_lbracket, 4, CONFIG_TZSH_MAXARGUMENTS, "<expression> ]"},
#endif

#ifndef CONFIG_TZSH_DISABLE_HELP
	{"?", cmd_help, 1, 1, NULL},
#endif

#if defined(CONFIG_NET) && defined(CONFIG_NET_ROUTE) && !defined(CONFIG_TZSH_DISABLE_ADDROUTE)
	{"addroute", cmd_addroute, 3, 4, "<target> [<netmask>] <router>"},
#endif

#if defined(CONFIG_NET) && defined(CONFIG_NET_ARP) && !defined(CONFIG_TZSH_DISABLE_ARP)
	{"arp", cmd_arp, 3, 4, "[-a <ipaddr>|-d <ipaddr>|-s <ipaddr> <hwaddr>]"},
#endif

#if defined(CONFIG_NETUTILS_CODECS) && defined(CONFIG_CODECS_BASE64)
#ifndef CONFIG_TZSH_DISABLE_BASE64DEC
	{"base64dec", cmd_base64decode, 2, 4, "[-w] [-f] <string or filepath>"},
#endif
#ifndef CONFIG_TZSH_DISABLE_BASE64ENC
	{"base64enc", cmd_base64encode, 2, 4, "[-w] [-f] <string or filepath>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_BASENAME
	{"basename", cmd_basename, 2, 3, "<path> [<suffix>]"},
#endif

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_LOOPS)
	{"break", cmd_break, 1, 1, NULL},
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_CAT
	{"cat", cmd_cat, 2, CONFIG_TZSH_MAXARGUMENTS, "<path> [<path> [<path> ...]]"},
#endif
#ifndef CONFIG_DISABLE_ENVIRON
#ifndef CONFIG_TZSH_DISABLE_CD
	{"cd", cmd_cd, 1, 2, "[<dir-path>|-|~|..]"},
#endif
#endif
#ifndef CONFIG_TZSH_DISABLE_CP
	{"cp", cmd_cp, 3, 3, "<source-path> <dest-path>"},
#endif
#ifndef CONFIG_TZSH_DISABLE_CMP
	{"cmp", cmd_cmp, 3, 3, "<path1> <path2>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_DIRNAME
	{"dirname", cmd_dirname, 2, 2, "<path>"},
#endif

#ifndef CONFIG_TZSH_DISABLE_DATE
	{"date", cmd_date, 1, 3, "[-s \"MMM DD HH:MM:SS YYYY\"]"},
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_TZSH_DISABLE_DD)
	{"dd", cmd_dd, 3, 6, "if=<infile> of=<outfile> [bs=<sectsize>] [count=<sectors>] [skip=<sectors>]"},
#endif

#if defined(CONFIG_NET) && defined(CONFIG_NET_ROUTE) && !defined(CONFIG_TZSH_DISABLE_DELROUTE)
	{"delroute", cmd_delroute, 2, 3, "<target> [<netmask>]"},
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_MOUNTPOINT) && \
	defined(CONFIG_FS_READABLE) && !defined(CONFIG_TZSH_DISABLE_DF)
#ifdef TZSH_HAVE_CATFILE
#if defined(HAVE_DF_HUMANREADBLE) && defined(HAVE_DF_BLOCKOUTPUT)
	{"df", cmd_df, 1, 2, "[-h]"},
#else
	{"df", cmd_df, 1, 1, NULL},
#endif
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && defined(CONFIG_RAMLOG_SYSLOG) && \
	!defined(CONFIG_TZSH_DISABLE_DMESG)
	{"dmesg", cmd_dmesg, 1, 1, NULL},
#endif

#ifndef CONFIG_TZSH_DISABLE_ECHO
#ifndef CONFIG_DISABLE_ENVIRON
	{"echo", cmd_echo, 1, CONFIG_TZSH_MAXARGUMENTS, "[-n] [<string|$name> [<string|$name>...]]"},
#else
	{"echo", cmd_echo, 1, CONFIG_TZSH_MAXARGUMENTS, "[-n] [<string> [<string>...]]"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_ENV
	{"env", cmd_env, 1, 1, NULL},
#endif

#ifndef CONFIG_TZSH_DISABLE_EXEC
	{"exec", cmd_exec, 2, 3, "<hex-address>"},
#endif

#ifndef CONFIG_TZSH_DISABLE_EXIT
	{"exit", cmd_exit, 1, 1, NULL},
#endif

#ifndef CONFIG_TZSH_DISABLE_EXPORT
	{"export", cmd_export, 2, 3, "[<name> [<value>]]"},
#endif

#ifndef CONFIG_TZSH_DISABLESCRIPT
	{"false", cmd_false, 1, 1, NULL},
#endif

#ifndef CONFIG_TZSH_DISABLE_FREE
	{"free", cmd_free, 1, 1, NULL},
#endif

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_GET
	{"get", cmd_get, 4, 7, "[-b|-n] [-f <local-path>] -h <ip-address> <remote-path>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_HELP
#ifdef CONFIG_TZSH_HELP_TERSE
	{"help", cmd_help, 1, 2, "[<cmd>]"},
#else
	{"help", cmd_help, 1, 3, "[-v] [<cmd>]"},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_HEXDUMP
#ifndef CONFIG_TZSH_CMDOPT_HEXDUMP
	{"hexdump", cmd_hexdump, 2, 2, "<file or device>"},
#else
	{"hexdump", cmd_hexdump, 2, 4, "<file or device> [skip=<bytes>] [count=<bytes>]"},
#endif
#endif
#endif

#ifdef CONFIG_NET
#ifndef CONFIG_TZSH_DISABLE_IFCONFIG
	{"ifconfig", cmd_ifconfig, 1, 11, "[interface [<ip-address>|dhcp]] [dr|gw|gateway <dr-address>] [netmask <net-mask>] [dns <dns-address>] [hw <hw-mac>]"},
#endif
#ifndef CONFIG_TZSH_DISABLE_IFUPDOWN
	{"ifdown", cmd_ifdown, 2, 2, "<interface>"},
	{"ifup", cmd_ifup, 2, 2, "<interface>"},
#endif
#endif

#if defined(CONFIG_MODULE) && !defined(CONFIG_TZSH_DISABLE_MODCMDS)
	{"insmod", cmd_insmod, 3, 3, "<file-path> <module-name>"},
#endif

#ifdef HAVE_IRQINFO
	{"irqinfo", cmd_irqinfo, 1, 1, NULL},
#endif

#ifndef CONFIG_DISABLE_SIGNALS
#ifndef CONFIG_TZSH_DISABLE_KILL
	{"kill", cmd_kill, 3, 3, "-<signal> <pid>"},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_MOUNTPOINT)
#if defined(CONFIG_DEV_LOOP) && !defined(CONFIG_TZSH_DISABLE_LOSETUP)
	{"losetup", cmd_losetup, 3, 6, "[-d <dev-path>] | [[-o <offset>] [-r] <dev-path> <file-path>]"},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_MOUNTPOINT)
#if defined(CONFIG_SMART_DEV_LOOP) && !defined(CONFIG_TZSH_DISABLE_LOSMART)
	{"losmart", cmd_losmart, 2, 11, "[-d <dev-path>] | [[-m <minor>] [-o <offset>] [-e <erase-size>] [-s <sect-size>] [-r] <file-path>]"},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
#if !defined(CONFIG_TZSH_DISABLE_LN) && defined(CONFIG_PSEUDOFS_SOFTLINKS)
	{"ln", cmd_ln, 3, 4, "[-s] <target> <link>"},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_LS
	{"ls", cmd_ls, 1, 5, "[-lRs] <dir-path>"},
#endif
#endif

#if defined(CONFIG_MODULE) && !defined(CONFIG_TZSH_DISABLE_MODCMDS)
#if defined(CONFIG_FS_PROCFS) && !defined(CONFIG_FS_PROCFS_EXCLUDE_MODULE)
	{"lsmod", cmd_lsmod, 1, 1, NULL},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_MB
	{"mb", cmd_mb, 2, 3, "<hex-address>[=<hex-value>][ <hex-byte-count>]"},
#endif

#if defined(CONFIG_NETUTILS_CODECS) && defined(CONFIG_CODECS_HASH_MD5)
#ifndef CONFIG_TZSH_DISABLE_MD5
	{"md5", cmd_md5, 2, 3, "[-f] <string or filepath>"},
#endif
#endif

#ifdef TZSH_HAVE_DIROPTS
#ifndef CONFIG_TZSH_DISABLE_MKDIR
	{"mkdir", cmd_mkdir, 2, 2, "<path>"},
#endif
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && \
	defined(CONFIG_FSUTILS_MKFATFS)
#ifndef CONFIG_TZSH_DISABLE_MKFATFS
	{"mkfatfs", cmd_mkfatfs, 2, 6, "[-F <fatsize>] [-r <rootdirentries>] <block-driver>"},
#endif
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0
#if defined(CONFIG_PIPES) && CONFIG_DEV_FIFO_SIZE > 0 && \
	!defined(CONFIG_TZSH_DISABLE_MKFIFO)
	{"mkfifo", cmd_mkfifo, 2, 2, "<path>"},
#endif
#endif

#ifdef TZSH_HAVE_WRITABLE_MOUNTPOINT
#ifndef CONFIG_TZSH_DISABLE_MKRD
	{"mkrd", cmd_mkrd, 2, 6, "[-m <minor>] [-s <sector-size>] <nsectors>"},
#endif
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && \
	defined(CONFIG_FS_SMARTFS) && defined(CONFIG_FSUTILS_MKSMARTFS)
#ifndef CONFIG_TZSH_DISABLE_MKSMARTFS
#ifdef CONFIG_SMARTFS_MULTI_ROOT_DIRS
	{"mksmartfs", cmd_mksmartfs, 2, 6, "[-s <sector-size>] [-f] <path> [<num-root-directories>]"},
#else
	{"mksmartfs", cmd_mksmartfs, 2, 5, "[-s <sector-size>] [-f] <path>"},
#endif
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_MH
	{"mh", cmd_mh, 2, 3, "<hex-address>[=<hex-value>][ <hex-byte-count>]"},
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && defined(CONFIG_FS_READABLE)
#ifndef CONFIG_TZSH_DISABLE_MOUNT
#if defined(TZSH_HAVE_CATFILE) && defined(HAVE_MOUNT_LIST)
	{"mount", cmd_mount, 1, 7, "[-t <fstype> [-o <options>] [<block-device>] <mount-point>]"},
#else
	{"mount", cmd_mount, 4, 7, "-t <fstype> [-o <options>] [<block-device>] <mount-point>"},
#endif
#endif
#endif

#ifdef TZSH_HAVE_DIROPTS
#ifndef CONFIG_TZSH_DISABLE_MV
	{"mv", cmd_mv, 3, 3, "<old-path> <new-path>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_MW
	{"mw", cmd_mw, 2, 3, "<hex-address>[=<hex-value>][ <hex-byte-count>]"},
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && \
	defined(CONFIG_NET) && defined(CONFIG_NFS)
#ifndef CONFIG_TZSH_DISABLE_NFSMOUNT
	{"nfsmount", cmd_nfsmount, 4, 4, "<server-address> <mount-point> <remote-path>"},
#endif
#endif

#if defined(CONFIG_LIBC_NETDB) && defined(CONFIG_NETDB_DNSCLIENT) && \
	!defined(CONFIG_TZSH_DISABLE_NSLOOKUP)
	{"nslookup", cmd_nslookup, 2, 2, "<host-name>"},
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && \
	defined(CONFIG_FS_WRITABLE) && defined(CONFIG_TZSH_LOGIN_PASSWD) && \
	!defined(CONFIG_FSUTILS_PASSWD_READONLY)
#ifndef CONFIG_TZSH_DISABLE_PASSWD
	{"passwd", cmd_passwd, 3, 3, "<username> <password>"},
#endif
#endif

#if defined(CONFIG_BOARDCTL_POWEROFF) && !defined(CONFIG_TZSH_DISABLE_POWEROFF)
	{"poweroff", cmd_poweroff, 1, 2, NULL},
#endif

#ifndef CONFIG_TZSH_DISABLE_PRINTF
#ifndef CONFIG_DISABLE_ENVIRON
	{"printf", cmd_printf, 1, CONFIG_TZSH_MAXARGUMENTS, "[\\xNN] [\\n\\r\\t] [<string|$name> [<string|$name>...]]"},
#else
	{"printf", cmd_printf, 1, CONFIG_TZSH_MAXARGUMENTS, "[\\xNN] [\\n\\r\\t] [<string> [<string>...]]"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_PS
	{"ps", cmd_ps, 1, 1, NULL},
#endif

#if defined(CONFIG_NET_UDP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_PUT
	{"put", cmd_put, 4, 7, "[-b|-n] [-f <remote-path>] -h <ip-address> <local-path>"},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 && !defined(CONFIG_DISABLE_ENVIRON)
#ifndef CONFIG_TZSH_DISABLE_PWD
	{"pwd", cmd_pwd, 1, 1, NULL},
#endif
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0
#if !defined(CONFIG_TZSH_DISABLE_READLINK) && defined(CONFIG_PSEUDOFS_SOFTLINKS)
	{"readlink", cmd_readlink, 2, 2, "<link>"},
#endif
#endif

#if defined(CONFIG_BOARDCTL_RESET) && !defined(CONFIG_TZSH_DISABLE_REBOOT)
	{"reboot", cmd_reboot, 1, 2, NULL},
#endif

#ifdef TZSH_HAVE_DIROPTS
#ifndef CONFIG_TZSH_DISABLE_RM
	{"rm", cmd_rm, 2, 2, "<file-path>"},
#endif
#endif

#ifdef TZSH_HAVE_DIROPTS
#ifndef CONFIG_TZSH_DISABLE_RMDIR
	{"rmdir", cmd_rmdir, 2, 2, "<dir-path>"},
#endif
#endif

#if defined(CONFIG_MODULE) && !defined(CONFIG_TZSH_DISABLE_MODCMDS)
	{"rmmod", cmd_rmmod, 2, 2, "<module-name>"},
#endif

#ifndef CONFIG_TZSH_DISABLE_ROUTE
#if defined(CONFIG_NET_IPv4) && defined(CONFIG_NET_IPv6)
	{"route", cmd_route, 2, 2, "ipv4|ipv6"},
#elif defined(CONFIG_NET_IPv4)
	{"route", cmd_route, 1, 2, "[ipv4]"},
#elif defined(CONFIG_NET_IPv6)
	{"route", cmd_route, 1, 2, "[ipv6]"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_SET
#ifdef CONFIG_TZSH_VARS
#if !defined(CONFIG_DISABLE_ENVIRON) && !defined(CONFIG_TZSH_DISABLESCRIPT)
	{"set", cmd_set, 1, 4, "[{+|-}{e|x|xe|ex}] [<name> <value>]"},
#elif !defined(CONFIG_DISABLE_ENVIRON) && defined(CONFIG_TZSH_DISABLESCRIPT)
	{"set", cmd_set, 1, 3, "[<name> <value>]"},
#elif defined(CONFIG_DISABLE_ENVIRON) && !defined(CONFIG_TZSH_DISABLESCRIPT)
	{"set", cmd_set, 1, 2, "[{+|-}{e|x|xe|ex}]"},
#endif
#else
#if !defined(CONFIG_DISABLE_ENVIRON) && !defined(CONFIG_TZSH_DISABLESCRIPT)
	{"set", cmd_set, 2, 4, "[{+|-}{e|x|xe|ex}] [<name> <value>]"},
#elif !defined(CONFIG_DISABLE_ENVIRON) && defined(CONFIG_TZSH_DISABLESCRIPT)
	{"set", cmd_set, 3, 3, "<name> <value>"},
#elif defined(CONFIG_DISABLE_ENVIRON) && !defined(CONFIG_TZSH_DISABLESCRIPT)
	{"set", cmd_set, 2, 2, "{+|-}{e|x|xe|ex}"},
#endif
#endif
#endif							/* CONFIG_TZSH_DISABLE_SET */

#if  CONFIG_NFILE_DESCRIPTORS > 0 && CONFIG_NFILE_STREAMS > 0 && !defined(CONFIG_TZSH_DISABLESCRIPT)
#ifndef CONFIG_TZSH_DISABLE_SH
	{"sh", cmd_sh, 2, 2, "<script-path>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_SHUTDOWN
#if defined(CONFIG_BOARDCTL_POWEROFF) && defined(CONFIG_BOARDCTL_RESET)
	{"shutdown", cmd_shutdown, 1, 2, "[--reboot]"},
#elif defined(CONFIG_BOARDCTL_POWEROFF)
	{"shutdown", cmd_shutdown, 1, 1, NULL},
#elif defined(CONFIG_BOARDCTL_RESET)
	{"shutdown", cmd_shutdown, 2, 2, "--reboot"},
#endif
#endif

#ifndef CONFIG_DISABLE_SIGNALS
#ifndef CONFIG_TZSH_DISABLE_SLEEP
	{"sleep", cmd_sleep, 2, 2, "<sec>"},
#endif
#endif

#if !defined(CONFIG_TZSH_DISABLESCRIPT) && !defined(CONFIG_TZSH_DISABLE_TEST)
	{"test", cmd_test, 3, CONFIG_TZSH_MAXARGUMENTS, "<expression>"},
#endif

#if defined(CONFIG_TZSH_TELNET) && !defined(CONFIG_TZSH_DISABLE_TELNETD)
#if defined(CONFIG_NET_IPv4) && defined(CONFIG_NET_IPv6)
	{"telnetd", cmd_telnetd, 2, 2, "[ipv4|ipv6]"},
#else
	{"telnetd", cmd_telnetd, 1, 1, NULL},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_TIME
	{"time", cmd_time, 2, 2, "\"<command>\""},
#endif

#ifndef CONFIG_TZSH_DISABLESCRIPT
	{"true", cmd_true, 1, 1, NULL},
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_TRUNCATE
	{"truncate", cmd_truncate, 4, 4, "-s <length> <file-path>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_UNAME
#ifdef CONFIG_NET
	{"uname", cmd_uname, 1, 7, "[-a | -imnoprsv]"},
#else
	{"uname", cmd_uname, 1, 7, "[-a | -imoprsv]"},
#endif
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && defined(CONFIG_FS_READABLE)
#ifndef CONFIG_TZSH_DISABLE_UMOUNT
	{"umount", cmd_umount, 2, 2, "<dir-path>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_UNSET
	{"unset", cmd_unset, 2, 2, "<name>"},
#endif

#if defined(CONFIG_NETUTILS_CODECS) && defined(CONFIG_CODECS_URLCODE)
#ifndef CONFIG_TZSH_DISABLE_URLDECODE
	{"urldecode", cmd_urldecode, 2, 3, "[-f] <string or filepath>"},
#endif
#ifndef CONFIG_TZSH_DISABLE_URLENCODE
	{"urlencode", cmd_urlencode, 2, 3, "[-f] <string or filepath>"},
#endif
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT) && CONFIG_NFILE_DESCRIPTORS > 0 && \
	defined(CONFIG_FS_WRITABLE) && defined(CONFIG_TZSH_LOGIN_PASSWD) && \
	!defined(CONFIG_FSUTILS_PASSWD_READONLY)
#ifndef CONFIG_TZSH_DISABLE_USERADD
	{"useradd", cmd_useradd, 3, 3, "<username> <password>"},
#endif
#ifndef CONFIG_TZSH_DISABLE_USERDEL
	{"userdel", cmd_userdel, 2, 2, "<username>"},
#endif
#endif

#ifndef CONFIG_DISABLE_SIGNALS
#ifndef CONFIG_TZSH_DISABLE_USLEEP
	{"usleep", cmd_usleep, 2, 2, "<usec>"},
#endif
#endif

#if defined(CONFIG_NET_TCP) && CONFIG_NFILE_DESCRIPTORS > 0
#ifndef CONFIG_TZSH_DISABLE_WGET
	{"wget", cmd_wget, 2, 4, "[-o <local-path>] <url>"},
#endif
#endif

#ifndef CONFIG_TZSH_DISABLE_XD
	{"xd", cmd_xd, 3, 3, "<hex-address> <byte-count>"},
#endif

	{NULL, NULL, 1, 1, NULL}
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: help_cmdlist
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_HELP
static inline void help_cmdlist(FAR struct tzsh_vtbl_s *vtbl)
{
	unsigned int i;
	unsigned int j;
	unsigned int k;

	/* Print the command name in NUM_CMD_ROWS rows with CMDS_PER_LINE commands
	 * on each line.
	 */

	for (i = 0; i < NUM_CMD_ROWS; i++) {
		tzsh_output(vtbl, "  ");
		for (j = 0, k = i; j < CMDS_PER_LINE && k < NUM_CMDS; j++, k += NUM_CMD_ROWS) {
#ifdef CONFIG_NOPRINTF_FIELDWIDTH
			tzsh_output(vtbl, "%s\t", g_cmdmap[k].cmd);
#else
			tzsh_output(vtbl, "%-12s", g_cmdmap[k].cmd);
#endif
		}

		tzsh_output(vtbl, "\n");
	}
}
#endif

/****************************************************************************
 * Name: help_usage
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLE_HELP) && !defined(CONFIG_TZSH_HELP_TERSE)
static inline void help_usage(FAR struct tzsh_vtbl_s *vtbl)
{
	tzsh_output(vtbl, "TZSH command forms:\n");
#ifndef CONFIG_TZSH_DISABLEBG
	tzsh_output(vtbl, "  [nice [-d <niceness>>]] <cmd> [> <file>|>> <file>] [&]\n\n");
#else
	tzsh_output(vtbl, "  <cmd> [> <file>|>> <file>]\n\n");
#endif
#ifndef CONFIG_TZSH_DISABLESCRIPT
#ifndef CONFIG_TZSH_DISABLE_ITEF
	tzsh_output(vtbl, "OR\n");
	tzsh_output(vtbl, "  if <cmd>\n");
	tzsh_output(vtbl, "  then\n");
	tzsh_output(vtbl, "    [sequence of <cmd>]\n");
	tzsh_output(vtbl, "  else\n");
	tzsh_output(vtbl, "    [sequence of <cmd>]\n");
	tzsh_output(vtbl, "  fi\n\n");
#endif
#ifndef CONFIG_TZSH_DISABLE_LOOPS
	tzsh_output(vtbl, "OR\n");
	tzsh_output(vtbl, "  while <cmd>\n");
	tzsh_output(vtbl, "  do\n");
	tzsh_output(vtbl, "    [sequence of <cmd>]\n");
	tzsh_output(vtbl, "  done\n\n");
	tzsh_output(vtbl, "OR\n");
	tzsh_output(vtbl, "  until <cmd>\n");
	tzsh_output(vtbl, "  do\n");
	tzsh_output(vtbl, "    [sequence of <cmd>]\n");
	tzsh_output(vtbl, "  done\n\n");
#endif
#endif
}
#endif

/****************************************************************************
 * Name: help_showcmd
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_HELP
static void help_showcmd(FAR struct tzsh_vtbl_s *vtbl, FAR const struct cmdmap_s *cmdmap)
{
	if (cmdmap->usage) {
		tzsh_output(vtbl, "  %s %s\n", cmdmap->cmd, cmdmap->usage);
	} else {
		tzsh_output(vtbl, "  %s\n", cmdmap->cmd);
	}
}
#endif

/****************************************************************************
 * Name: help_cmd
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_HELP
static int help_cmd(FAR struct tzsh_vtbl_s *vtbl, FAR const char *cmd)
{
	FAR const struct cmdmap_s *cmdmap;

	/* Find the command in the command table */

	for (cmdmap = g_cmdmap; cmdmap->cmd; cmdmap++) {
		/* Is this the one we are looking for? */

		if (strcmp(cmdmap->cmd, cmd) == 0) {
			/* Yes... show it */

			tzsh_output(vtbl, "%s usage:", cmd);
			help_showcmd(vtbl, cmdmap);
			return OK;
		}
	}

	tzsh_output(vtbl, g_fmtcmdnotfound, cmd);
	return ERROR;
}
#endif

/****************************************************************************
 * Name: help_allcmds
 ****************************************************************************/

#if !defined(CONFIG_TZSH_DISABLE_HELP) && !defined(CONFIG_TZSH_HELP_TERSE)
static inline void help_allcmds(FAR struct tzsh_vtbl_s *vtbl)
{
	FAR const struct cmdmap_s *cmdmap;

	/* Show all of the commands in the command table */

	for (cmdmap = g_cmdmap; cmdmap->cmd; cmdmap++) {
		help_showcmd(vtbl, cmdmap);
	}
}
#endif

/****************************************************************************
 * Name: help_builtins
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_HELP
static inline void help_builtins(FAR struct tzsh_vtbl_s *vtbl)
{
#ifdef CONFIG_TZSH_BUILTIN_APPS
	FAR const char *name;
	int i;

	/* List the set of available built-in commands */

	tzsh_output(vtbl, "\nBuiltin Apps:\n");
	for (i = 0; (name = builtin_getname(i)) != NULL; i++) {
		tzsh_output(vtbl, "  %s\n", name);
	}
#endif
}
#endif

/****************************************************************************
 * Name: cmd_help
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_HELP
static int cmd_help(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	FAR const char *cmd = NULL;
#ifndef CONFIG_TZSH_HELP_TERSE
	bool verbose = false;
	int i;
#endif

	/* The command may be followed by a verbose option */

#ifndef CONFIG_TZSH_HELP_TERSE
	i = 1;
	if (argc > i) {
		if (strcmp(argv[i], "-v") == 0) {
			verbose = true;
			i++;
		}
	}

	/* The command line may end with a command name */

	if (argc > i) {
		cmd = argv[i];
	}

	/* Show the generic usage if verbose is requested */

	if (verbose) {
		help_usage(vtbl);
	}
#else
	if (argc > 1) {
		cmd = argv[1];
	}
#endif

	/* Are we showing help on a single command? */

	if (cmd) {
		/* Yes.. show the single command */

		help_cmd(vtbl, cmd);
	} else {
		/* In verbose mode, show detailed help for all commands */

#ifndef CONFIG_TZSH_HELP_TERSE
		if (verbose) {
			tzsh_output(vtbl, "Where <cmd> is one of:\n");
			help_allcmds(vtbl);
		}

		/* Otherwise, just show the list of command names */

		else
#endif
		{
			help_cmd(vtbl, "help");
			tzsh_output(vtbl, "\n");
			help_cmdlist(vtbl);
		}

		/* And show the list of built-in applications */

		help_builtins(vtbl);
	}

	return OK;
}
#endif

/****************************************************************************
 * Name: cmd_unrecognized
 ****************************************************************************/

static int cmd_unrecognized(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	tzsh_output(vtbl, g_fmtcmdnotfound, argv[0]);
	return ERROR;
}

/****************************************************************************
 * Name: cmd_true
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLESCRIPT
static int cmd_true(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return OK;
}

#endif

/****************************************************************************
 * Name: cmd_false
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLESCRIPT
static int cmd_false(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	return ERROR;
}
#endif

/****************************************************************************
 * Name: cmd_exit
 ****************************************************************************/

#ifndef CONFIG_TZSH_DISABLE_EXIT
static int cmd_exit(FAR struct tzsh_vtbl_s *vtbl, int argc, char **argv)
{
	tzsh_exit(vtbl, 0);
	return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: tzsh_command
 *
 * Description:
 *   Execute the command in argv[0]
 *
 * Returned Value:
 *   -1 (ERRROR) if the command was unsuccessful
 *    0 (OK)     if the command was successful
 *
 ****************************************************************************/

int tzsh_command(FAR struct tzsh_vtbl_s *vtbl, int argc, char *argv[])
{
	const struct cmdmap_s *cmdmap;
	const char *cmd;
	tzsh_cmd_t handler = cmd_unrecognized;
	int ret;

	/* The form of argv is:
	 *
	 * argv[0]:      The command name.  This is argv[0] when the arguments
	 *               are, finally, received by the command vtblr
	 * argv[1]:      The beginning of argument (up to CONFIG_TZSH_MAXARGUMENTS)
	 * argv[argc]:   NULL terminating pointer
	 */

	cmd = argv[0];

	/* See if the command is one that we understand */

	for (cmdmap = g_cmdmap; cmdmap->cmd; cmdmap++) {
		if (strcmp(cmdmap->cmd, cmd) == 0) {
			/* Check if a valid number of arguments was provided.  We
			 * do this simple, imperfect checking here so that it does
			 * not have to be performed in each command.
			 */

			if (argc < cmdmap->minargs) {
				/* Fewer than the minimum number were provided */

				tzsh_output(vtbl, g_fmtargrequired, cmd);
				return ERROR;
			} else if (argc > cmdmap->maxargs) {
				/* More than the maximum number were provided */

				tzsh_output(vtbl, g_fmttoomanyargs, cmd);
				return ERROR;
			} else {
				/* A valid number of arguments were provided (this does
				 * not mean they are right).
				 */

				handler = cmdmap->handler;
				break;
			}
		}
	}

	ret = handler(vtbl, argc, argv);
	return ret;
}

/****************************************************************************
 * Name: tzsh_extmatch_count
 *
 * Description:
 *   This support function is used to provide support for realine tab-
 *   completion logic  tzsh_extmatch_count() counts the number of matching
 *   tzsh command names
 *
 * Input Parameters:
 *   name    - A point to the name containing the name to be matched.
 *   matches - A table is size CONFIG_READLINE_MAX_EXTCMDS that can
 *             be used to remember matching name indices.
 *   namelen - The lenght of the name to match
 *
 * Returned Values:
 *   The number commands that match to the first namelen characters.
 *
 ****************************************************************************/

#if defined(CONFIG_TZSH_READLINE) && defined(CONFIG_READLINE_TABCOMPLETION) && \
	defined(CONFIG_READLINE_HAVE_EXTMATCH)
int tzsh_extmatch_count(FAR char *name, FAR int *matches, int namelen)
{
	int nr_matches = 0;
	int i;

	for (i = 0; i < NUM_CMDS; i++) {
		if (strncmp(name, g_cmdmap[i].cmd, namelen) == 0) {
			matches[nr_matches] = i;
			nr_matches++;

			if (nr_matches >= CONFIG_READLINE_MAX_EXTCMDS) {
				break;
			}
		}
	}

	return nr_matches;
}
#endif

/****************************************************************************
 * Name: tzsh_extmatch_getname
 *
 * Description:
 *   This support function is used to provide support for realine tab-
 *   completion logic  tzsh_extmatch_getname() will return the full command
 *   string from an index that was previously saved by tzsh_exmatch_count().
 *
 * Input Parameters:
 *   index - The index of the command name to be returned.
 *
 * Returned Values:
 *   The numb
 *
 ****************************************************************************/

#if defined(CONFIG_TZSH_READLINE) && defined(CONFIG_READLINE_TABCOMPLETION) && \
	defined(CONFIG_READLINE_HAVE_EXTMATCH)
FAR const char *tzsh_extmatch_getname(int index)
{
	DEBUGASSERT(index > 0 && index <= NUM_CMDS);
	return g_cmdmap[index].cmd;
}
#endif
