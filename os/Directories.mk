###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
############################################################################
# Directories.mk
#
#   Copyright (C) 2007-2012, 2014 Gregory Nutt. All rights reserved.
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

# All add-on directories.
#
# TINYARA_ADDONS is the list of directories built into the TinyAra kernel.
# USER_ADDONS is the list of directories that will be built into the user
#   application

TINYARA_ADDONS :=
USER_ADDONS :=

# In the protected build, the applications in the apps/ directory will be
# into the userspace; in the flat build, the applications will b built into
# the kernel space.  But in the kernel build, the applications will not be
# built at all by this Makefile.

ifeq ($(CONFIG_BUILD_PROTECTED),y)
USER_ADDONS += $(APPDIR)
else
ifneq ($(CONFIG_BUILD_KERNEL),y)
TINYARA_ADDONS += $(APPDIR)
endif
endif

# Add external directories

ifeq ($(CONFIG_BUILD_PROTECTED),y)
USER_ADDONS += $(EXTDIR)
else
ifneq ($(CONFIG_BUILD_KERNEL),y)
TINYARA_ADDONS += $(EXTDIR)
endif
endif

# Add Framework directories

ifeq ($(CONFIG_BUILD_PROTECTED),y)
USER_ADDONS += $(FRAMEWORK_LIB_DIR)
else
ifneq ($(CONFIG_BUILD_KERNEL),y)
TINYARA_ADDONS += $(FRAMEWORK_LIB_DIR)
endif
endif

# Add tools directories

ifeq ($(CONFIG_BUILD_PROTECTED),y)
USER_ADDONS += $(TOOLSDIR)
else
ifneq ($(CONFIG_BUILD_KERNEL),y)
TINYARA_ADDONS += $(TOOLSDIR)
endif
endif

# Lists of build directories.
#
# FSDIRS depend on file descriptor support; NONFSDIRS do not (except for parts
#   of FSDIRS).  We will exclude FSDIRS from the build if file descriptor
#   support is disabled
# CONTEXTDIRS include directories that have special, one-time pre-build
#   requirements.  Normally this includes things like auto-generation of
#   configuration specific files or creation of configurable symbolic links
# USERDIRS - When TinyAra is build is a monolithic kernel, this provides the
#   list of directories that must be built
# OTHERDIRS - These are directories that are not built but probably should
#   be cleaned to prevent garbage from collecting in them when changing
#   configurations.

NONFSDIRS = kernel $(ARCH_SRC) $(TINYARA_ADDONS)
FSDIRS = fs drivers
CONTEXTDIRS = $(APPDIR)
CONTEXTDIRS += $(TOOLSDIR)
USERDIRS =
OTHERDIRS = $(LIBRARIES_DIR)

ifeq ($(CONFIG_BUILD_PROTECTED),y)

USERDIRS += $(LIB_DIR)$(DELIM)libc mm $(USER_ADDONS)
ifeq ($(CONFIG_HAVE_CXX),y)
USERDIRS += $(LIB_DIR)$(DELIM)libxx
endif

else
ifeq ($(CONFIG_BUILD_KERNEL),y)

USERDIRS += $(LIB_DIR)$(DELIM)libc mm
ifeq ($(CONFIG_HAVE_CXX),y)
USERDIRS += $(LIB_DIR)$(DELIM)libxx
endif

else

NONFSDIRS += $(LIB_DIR)$(DELIM)libc mm
OTHERDIRS += $(USER_ADDONS)
ifeq ($(CONFIG_HAVE_CXX),y)
NONFSDIRS += $(LIB_DIR)$(DELIM)libxx
else
OTHERDIRS += $(LIB_DIR)$(DELIM)libxx
endif

endif
endif

ifeq ($(CONFIG_LIB_SYSCALL),y)
NONFSDIRS += syscall
CONTEXTDIRS += syscall
USERDIRS += syscall
else
OTHERDIRS += syscall
endif


# CLEANDIRS are the directories that will clean in.  These are
#   all directories that we know about.
# KERNDEPDIRS are the directories in which we will build target dependencies.
#   If TinyAra and applications are built separately (CONFIG_BUILD_PROTECTED or
#   CONFIG_BUILD_KERNEL), then this holds only the directories containing
#   kernel files.
# USERDEPDIRS. If TinyAra and applications are built separately (CONFIG_BUILD_PROTECTED),
#   then this holds only the directories containing user files. If
#   CONFIG_BUILD_KERNEL is selected, then applications are not build at all.

CLEANDIRS   = $(NONFSDIRS) $(FSDIRS) $(USERDIRS) $(OTHERDIRS)
KERNDEPDIRS = $(NONFSDIRS)
USERDEPDIRS = $(USERDIRS)

# Add file system directories to KERNDEPDIRS (they are already in CLEANDIRS)

ifeq ($(CONFIG_NFILE_DESCRIPTORS),0)
ifeq ($(CONFIG_NET),y)
ifneq ($(CONFIG_NSOCKET_DESCRIPTORS),0)
KERNDEPDIRS += fs
endif
KERNDEPDIRS += drivers
endif
else
KERNDEPDIRS += $(FSDIRS)
endif

# Add Audio Module

ifeq ($(CONFIG_AUDIO),y)
KERNDEPDIRS += audio
endif
CLEANDIRS += audio

# Add Crypto Module

ifeq ($(CONFIG_CRYPTO),y)
KERNDEPDIRS += crypto
endif
CLEANDIRS += crypto

# Add networking directories to KERNDEPDIRS and CLEANDIRS

ifeq ($(CONFIG_NET),y)
KERNDEPDIRS += net
endif
CLEANDIRS += net

# Add Power Managment Module

ifeq ($(CONFIG_PM),y)
KERNDEPDIRS += pm
endif
CLEANDIRS += pm

# Add Logger Module

ifeq ($(CONFIG_LOGM),y)
KERNDEPDIRS += logm
endif
CLEANDIRS += logm

