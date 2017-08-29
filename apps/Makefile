###########################################################################
#
# Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
-include $(APPDIR)/Make.defs

# Application Directories

# BUILDIRS is the list of top-level directories containing Make.defs files
# CLEANDIRS is the list of all top-level directories containing Makefiles.
#   It is used only for cleaning.

BUILDIRS   := $(dir $(filter-out import/Make.defs,$(wildcard */Make.defs)))
CLEANDIRS  := $(dir $(wildcard */Makefile))

# CONFIGURED_APPS is the application directories that should be built in
#   the current configuration.

CONFIGURED_APPS =

define Add_Application
  include $(1)Make.defs
endef

$(foreach BDIR, $(BUILDIRS), $(eval $(call Add_Application,$(BDIR))))

# Library path

LIBPATH ?= $(TOPDIR)$(DELIM)libs

# The install path

BIN_DIR = $(APPDIR)$(DELIM)bin

# The final build target

BIN = libapps$(LIBEXT)

# Build targets

all: $(BIN)
.PHONY: import install context context_serialize context_rest .depdirs preconfig depend clean distclean applist appupdate

define MAKE_template
	$(Q) $(MAKE) -C $(1) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" EXTDIR="$(EXTDIR)" BIN_DIR="$(BIN_DIR)"

endef

define SDIR_template
$(1)_$(2):
	$(Q) $(MAKE) -C $(1) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" EXTDIR="$(EXTDIR)" BIN_DIR="$(BIN_DIR)"

endef

$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),all)))
$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),install)))
$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),context)))
$(foreach SDIR, $(CONFIGURED_APPS), $(eval $(call SDIR_template,$(SDIR),depend)))
$(foreach SDIR, $(CLEANDIRS), $(eval $(call SDIR_template,$(SDIR),clean)))
$(foreach SDIR, $(CLEANDIRS), $(eval $(call SDIR_template,$(SDIR),distclean)))

$(BIN): $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_all)

.install: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_install)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

install: $(BIN_DIR) .install

.import: $(BIN) install

import:
	$(Q) $(MAKE) .import TOPDIR="$(APPDIR)$(DELIM)import"

context_rest: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_context)

context_serialize:
	$(Q) $(MAKE) -C examples context TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)"
	$(Q) $(MAKE) context_rest

context: context_serialize

preconfig:
	$(foreach SDIR, $(BUILDIRS), $(call MAKE_template,$(SDIR),preconfig))
	$(Q) $(MKKCONFIG)
	$(Q) $(MKKCONFIG) -o Kconfig_ENTRY

.depdirs: $(foreach SDIR, $(CONFIGURED_APPS), $(SDIR)_depend)

.depend: context Makefile .depdirs
	$(Q) touch $@

depend: .depend

clean: $(foreach SDIR, $(CLEANDIRS), $(SDIR)_clean)
	$(call DELFILE, $(BIN))
	$(call DELDIR, $(BIN_DIR))
	$(call CLEAN)

applist:
	$(Q) ./tools/create_appspec.sh

appupdate:
	$(Q) ./tools/update_config.sh -sw
	$(Q) rm ../os/.appSpec

distclean: $(foreach SDIR, $(CLEANDIRS), $(SDIR)_distclean)
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
	$(call DELFILE, $(BIN))
	$(call DELFILE, Kconfig*)
	$(call DELDIR, $(BIN_DIR))
	$(call CLEAN)
