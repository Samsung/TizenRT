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
############################################################################
# apps/Directory.mk
#
#   Copyright (C) 2011-2015 Gregory Nutt. All rights reserved.
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

-include $(TOPDIR)/.config # Current configuration
include $(APPDIR)/Make.defs
include $(APPDIR)/import/Make.defs

# Sub-directories

SUBDIRS = $(dir $(wildcard */Makefile))

all: nothing

.PHONY: nothing context depend clean distclean

define SDIR_template
$(1)_$(2):
	$(Q) $(MAKE) -C $(1) $(2) TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" EXTDIR="$(EXTDIR)"
endef

$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),preconfig)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),context)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),depend)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),clean)))
$(foreach SDIR, $(SUBDIRS), $(eval $(call SDIR_template,$(SDIR),distclean)))

nothing:

install:

preconfig: $(foreach SDIR, $(SUBDIRS), $(SDIR)_preconfig)
	$(Q) $(MKKCONFIG) -m $(MENUDESC)
	$(Q) $(MKKCONFIG) -o Kconfig_ENTRY

context: $(foreach SDIR, $(SUBDIRS), $(SDIR)_context)

depend: $(foreach SDIR, $(SUBDIRS), $(SDIR)_depend)

clean: $(foreach SDIR, $(SUBDIRS), $(SDIR)_clean)

distclean: $(foreach SDIR, $(SUBDIRS), $(SDIR)_distclean)
	$(call DELFILE, Kconfig*)

-include Make.dep
