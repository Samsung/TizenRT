###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
# Author: Jihun Ahn <jhun.ahn@samsung.com>
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

# Control build verbosity
#
#  V=1,2: Enable echo of commands
#  V=2:   Enable bug/verbose options in tools and scripts

ifeq ($(V),1)
export Q :=
else
ifeq ($(V),2)
export Q :=
BUILDOPT := "VERBOSE=1"
else
export Q := @
endif
endif

define CD
	$(Q) $(eval _D=$(firstword $(1) $(@D)))
	$(Q) cd $(_D)
endef

define DELFILE
	@echo DEL: $(1)
	$(Q) rm -rf $(1)
endef

define INSTALL
	$(eval _target := $(1))
	@echo INSTALL: $(notdir $(_target))
	$(Q) cp -rf $(_target) $(2)
endef

CORES := $(shell getconf _NPROCESSORS_ONLN)
