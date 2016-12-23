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
# apps/Makefile
#
#   Copyright (C) 2011 Uros Platise. All rights reserved.
#   Copyright (C) 2011-2014 Gregory Nutt. All rights reserved.
#   Authors: Uros Platise <uros.platise@isotel.eu>
#            Gregory Nutt <gnutt@nuttx.org>
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

APPDIR = ${shell pwd}
TOPDIR ?= $(APPDIR)/import

-include $(TOPDIR)/Make.defs

# Application Directories

# CONFIGURED_APPS is the list of all configured built-in directories/built
#   action.
# SUBDIRS is the list of all directories containing Makefiles.  It is used
#   only for cleaning.

CONFIGURED_APPS =
SUBDIRS  = examples import
SUBDIRS += netutils platform shell system

# The list of configured directories is derived from TinyAra configuration
# file:  The selected applications are enabled settings in the configuration
# file.  For example,
#
#   CONFIG_EXAMPLES_HELLO=y
#
# Will cause the "Hello, World!" example at apps/examples/hello to be
# built and added int libapps.a.
# out.

# builtin/Make.defs must be included first

include examples/Make.defs
include netutils/Make.defs
include platform/Make.defs
include shell/Make.defs
include system/Make.defs
-include external/Make.defs

# INSTALLED_APPS is the list of currently available application directories.  It
# is the same as CONFIGURED_APPS, but filtered to exclude any non-existent
# application directory. builtin is always in the list of applications to be
# built.

INSTALLED_APPS =

# Create the list of available applications (INSTALLED_APPS)

define ADD_BUILTIN
  INSTALLED_APPS += $(if $(wildcard $1$(DELIM)Makefile),$1,)
endef

$(foreach BUILTIN, $(CONFIGURED_APPS), $(eval $(call ADD_BUILTIN,$(BUILTIN))))

# The external/ directory may also be added to the INSTALLED_APPS.  But there
# is no external/ directory in the repository.  Rather, this directory may be
# provided by the user (possibly as a symbolic link) to add libraries and
# applications to the standard build from the repository.

EXTERNAL_DIR := $(dir $(wildcard external$(DELIM)Makefile))

INSTALLED_APPS += $(EXTERNAL_DIR)
SUBDIRS += $(EXTERNAL_DIR)

# Library path

LIBPATH ?= $(TOPDIR)$(DELIM)libs

# The install path

BIN_DIR = $(APPDIR)$(DELIM)bin

# The final build target

BIN = libapps$(LIBEXT)

# Build targets

all: $(BIN)
.PHONY: import install context context_serialize context_rest .depdirs depend clean distclean

define SDIR_template
$(1)_$(2):
	$(Q) $(MAKE) -C $(1) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" BIN_DIR="$(BIN_DIR)"
endef

$(foreach SDIR, $(INSTALLED_APPS), $(eval $(call SDIR_template,$(SDIR),all)))
$(foreach SDIR, $(INSTALLED_APPS), $(eval $(call SDIR_template,$(SDIR),install)))
$(foreach SDIR, $(INSTALLED_APPS), $(eval $(call SDIR_template,$(SDIR),context)))
$(foreach SDIR, $(INSTALLED_APPS), $(eval $(call SDIR_template,$(SDIR),depend)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),clean)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),distclean)))

$(BIN): $(foreach SDIR, $(INSTALLED_APPS), $(SDIR)_all)

.install: $(foreach SDIR, $(INSTALLED_APPS), $(SDIR)_install)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

install: $(BIN_DIR) .install

.import: $(BIN) install

import:
	$(Q) $(MAKE) .import TOPDIR="$(APPDIR)$(DELIM)import"

context_rest: $(foreach SDIR, $(INSTALLED_APPS), $(SDIR)_context)

context_serialize:
	$(Q) $(MAKE) -C examples context TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"
	$(Q) $(MAKE) context_rest

context: context_serialize

.depdirs: $(foreach SDIR, $(INSTALLED_APPS), $(SDIR)_depend)

.depend: context Makefile .depdirs
	$(Q) touch $@

depend: .depend

clean: $(foreach SDIR, $(SUBDIRS), $(SDIR)_clean)
	$(call DELFILE, $(BIN))
	$(call DELDIR, $(BIN_DIR))
	$(call CLEAN)

distclean: $(foreach SDIR, $(SUBDIRS), $(SDIR)_distclean)
ifeq ($(CONFIG_WINDOWS_NATIVE),y)
	$(Q) ( if exist  external ( \
		echo ********************************************************" \
		echo * The external directory/link must be removed manually *" \
		echo ********************************************************" \
	)
else
	$(Q) ( if [ -e external ]; then \
		echo "********************************************************"; \
		echo "* The external directory/link must be removed manually *"; \
		echo "********************************************************"; \
	   fi; \
	)
endif
	$(call DELFILE, .depend)
	$(call DELDIR, $(BIN_DIR))


