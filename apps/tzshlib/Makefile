############################################################################
# apps/tzshlib/Makefile
#
#   Copyright (C) 2011-2013, 2016 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/Make.defs

# TZSH Library

CSRCS  = tzsh_init.c tzsh_parse.c tzsh_console.c tzsh_script.c tzsh_system.c
CSRCS += tzsh_command.c tzsh_fscmds.c tzsh_ddcmd.c tzsh_proccmds.c tzsh_mmcmds.c
CSRCS += tzsh_timcmds.c tzsh_envcmds.c tzsh_syscmds.c tzsh_dbgcmds.c

ifeq ($(CONFIG_NFILE_STREAMS),0)
CSRCS += tzsh_stdsession.c
ifeq ($(CONFIG_TZSH_CONSOLE_LOGIN),y)
CSRCS += tzsh_stdlogin.c
endif
else
CSRCS += tzsh_session.c
ifeq ($(CONFIG_TZSH_CONSOLE_LOGIN),y)
CSRCS += tzsh_login.c
endif
endif

ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
CSRCS += tzsh_fsutils.c
endif

ifeq ($(CONFIG_TZSH_BUILTIN_APPS),y)
CSRCS += tzsh_builtin.c
endif

ifeq ($(CONFIG_TZSH_FILE_APPS),y)
CSRCS += tzsh_fileapps.c
endif

ifeq ($(CONFIG_TZSH_VARS),y)
CSRCS += tzsh_vars.c
endif

ifeq ($(CONFIG_TZSH_ROMFSETC),y)
CSRCS += tzsh_romfsetc.c
endif

ifeq ($(CONFIG_NET),y)
CSRCS += tzsh_netinit.c tzsh_netcmds.c

ifeq ($(CONFIG_WIRELESS_WAPI),y)
ifeq ($(CONFIG_TZSH_NETINIT),y)
ifneq ($(CONFIG_TZSH_NETLOCAL),y)
CSRCS += tzsh_associate.c
endif
endif
endif

ifeq ($(CONFIG_NET_ROUTE),y)
CSRCS += tzsh_routecmds.c
endif
endif

ifneq ($(CONFIG_DISABLE_MOUNTPOINT),y)
CSRCS += tzsh_mntcmds.c
endif

ifeq ($(CONFIG_MODULE),y)
ifneq ($(CONFIG_TZSH_DISABLE_MODCMDS),y)
CSRCS += tzsh_modcmds.c
endif
endif

ifeq ($(CONFIG_TZSH_CONSOLE),y)
CSRCS += tzsh_consolemain.c
endif

ifneq ($(CONFIG_TZSH_DISABLE_PRINTF),y)
CSRCS += tzsh_printf.c
endif

ifeq ($(CONFIG_TZSH_TELNET),y)
CSRCS += tzsh_telnetd.c
ifeq ($(CONFIG_TZSH_TELNET_LOGIN),y)
CSRCS += tzsh_telnetlogin.c
endif
endif

ifneq ($(CONFIG_TZSH_DISABLESCRIPT),y)
CSRCS += tzsh_test.c
endif

ifeq ($(CONFIG_USBDEV),y)
CSRCS += tzsh_usbconsole.c
endif

ifeq ($(CONFIG_USBHOST),y)
CSRCS += tzsh_usbkeyboard.c
endif

ifeq ($(CONFIG_TZSH_USBDEV_TRACE),y)
CSRCS += tzsh_usbtrace.c
endif

ifeq ($(CONFIG_NETUTILS_CODECS),y)
CSRCS += tzsh_codeccmd.c
endif

ifeq ($(CONFIG_TZSH_LOGIN_PASSWD),y)
CSRCS += tzsh_passwdcmds.c
endif

include $(APPDIR)/Application.mk
