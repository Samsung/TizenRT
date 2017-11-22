#!/bin/bash
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

# File   : mksampleapp.sh
# Description : make sample app templete

WD=$(cd `dirname $0` && pwd)

# USAGE
usage() {
    cat <<EOF
Usage: `basename $0` [awesome_app]
EOF
  exit $1
}

if test $# -ne 1; then
    echo "`basename $0`: Invaild line width: '$@'"
    usage 1 1>&2
fi

SAMPLES_NAME_L=`echo $1 | tr '[a-z]' '[A-Z]'`
SAMPLES_NAME_S=`echo $1 | tr '[A-Z]' '[a-z]'`
SAMPLES_PATH=${WD}/../examples/${SAMPLES_NAME_S}
SAMPLES_YEAR=$(date +%Y)

# make directory
mkdir -p ${SAMPLES_PATH}

# Kconfig
tee ${SAMPLES_PATH}/Kconfig << __EOF__
#
# For a description of the syntax of this configuration file,
# see kconfig-language at https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt
#

config EXAMPLES_${SAMPLES_NAME_L}
	bool "${SAMPLES_NAME_S} example"
	default n
	---help---
		Enable ${SAMPLES_NAME_S} example

__EOF__

# Kconfig_ENTRY
tee ${SAMPLES_PATH}/Kconfig_ENTRY << __EOF__
#
# For a description of the syntax of this configuration file,
# see kconfig-language at https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt
#

config ENTRY_${SAMPLES_NAME_L}
	bool "${SAMPLES_NAME_S} example"
	depends on EXAMPLES_${SAMPLES_NAME_L}

__EOF__

# Make.defs
tee ${SAMPLES_PATH}/Make.defs << __EOF__
###########################################################################
#
# Copyright ${SAMPLES_YEAR} Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

ifeq (\$(CONFIG_EXAMPLES_${SAMPLES_NAME_L}),y)
CONFIGURED_APPS += examples/${SAMPLES_NAME_S}
endif

__EOF__

# Makefile
tee ${SAMPLES_PATH}/Makefile << __EOF__
###########################################################################
#
# Copyright ${SAMPLES_YEAR} Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

-include \$(TOPDIR)/.config
-include \$(TOPDIR)/Make.defs
include \$(APPDIR)/Make.defs

# built-in application info

APPNAME = ${SAMPLES_NAME_S}
THREADEXEC = TASH_EXECMD_SYNC

ASRCS =
CSRCS =
MAINSRC = ${SAMPLES_NAME_S}_main.c

AOBJS = \$(ASRCS:.S=\$(OBJEXT))
COBJS = \$(CSRCS:.c=\$(OBJEXT))
MAINOBJ = \$(MAINSRC:.c=\$(OBJEXT))

SRCS = \$(ASRCS) \$(CSRCS) \$(MAINSRC)
OBJS = \$(AOBJS) \$(COBJS)

ifneq (\$(CONFIG_BUILD_KERNEL),y)
  OBJS += \$(MAINOBJ)
endif

ifeq (\$(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\..\libapps\$(LIBEXT)
else
ifeq (\$(WINTOOL),y)
  BIN = ..\\\..\\\libapps\$(LIBEXT)
else
  BIN = ../../libapps\$(LIBEXT)
endif
endif

ifeq (\$(WINTOOL),y)
  INSTALL_DIR = "\${shell cygpath -w \$(BIN_DIR)}"
else
  INSTALL_DIR = \$(BIN_DIR)
endif

CONFIG_EXAMPLES_${SAMPLES_NAME_L}_PROGNAME ?= ${SAMPLES_NAME_S}\$(EXEEXT)
PROGNAME = \$(CONFIG_EXAMPLES_${SAMPLES_NAME_L}_PROGNAME)

ROOTDEPPATH = --dep-path .

# Common build

VPATH =

all: .built
.PHONY: clean depend distclean

\$(AOBJS): %\$(OBJEXT): %.S
	\$(call ASSEMBLE, $<, \$@)

\$(COBJS) \$(MAINOBJ): %\$(OBJEXT): %.c
	\$(call COMPILE, $<, \$@)

.built: \$(OBJS)
	\$(call ARCHIVE, \$(BIN), \$(OBJS))
	@touch .built

ifeq (\$(CONFIG_BUILD_KERNEL),y)
\$(BIN_DIR)\$(DELIM)\$(PROGNAME): \$(OBJS) \$(MAINOBJ)
	@echo "LD: \$(PROGNAME)"
	\$(Q) \$(LD) \$(LDELFFLAGS) \$(LDLIBPATH) -o \$(INSTALL_DIR)\$(DELIM)\$(PROGNAME) \$(ARCHCRT0OBJ) \$(MAINOBJ) \$(LDLIBS)
	\$(Q) \$(NM) -u  \$(INSTALL_DIR)\$(DELIM)\$(PROGNAME)

install: \$(BIN_DIR)\$(DELIM)\$(PROGNAME)

else
install:

endif

ifeq (\$(CONFIG_BUILTIN_APPS)\$(CONFIG_EXAMPLES_${SAMPLES_NAME_L}),yy)
\$(BUILTIN_REGISTRY)\$(DELIM)\$(APPNAME)_main.bdat: \$(DEPCONFIG) Makefile
	\$(call REGISTER,\$(APPNAME),\$(APPNAME)_main,\$(THREADEXEC))

context: \$(BUILTIN_REGISTRY)\$(DELIM)\$(APPNAME)_main.bdat

else
context:

endif

.depend: Makefile \$(SRCS)
	@\$(MKDEP) \$(ROOTDEPPATH) "\$(CC)" -- \$(CFLAGS) -- \$(SRCS) >Make.dep
	@touch \$@

depend: .depend

clean:
	\$(call DELFILE, .built)
	\$(call CLEAN)

distclean: clean
	\$(call DELFILE, Make.dep)
	\$(call DELFILE, .depend)

-include Make.dep
.PHONY: preconfig
preconfig:

__EOF__

# main file
tee ${SAMPLES_PATH}/${SAMPLES_NAME_S}_main.c << __EOF__
/****************************************************************************
 *
 * Copyright ${SAMPLES_YEAR} Samsung Electronics All Rights Reserved.
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

#include <stdio.h>

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ${SAMPLES_NAME_S}_main(int argc, FAR char *argv[])
#endif
{
	printf("%s\n", __func__);

	return 0;
}

__EOF__
