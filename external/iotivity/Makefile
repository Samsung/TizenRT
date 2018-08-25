###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

IOTIVITYDIR=$(TOPDIR)/$(EXTDIR)/iotivity
ifeq ($(CONFIG_IOTIVITY_RELEASE),y)
IOTIVITY_LIBS=release
else
IOTIVITY_LIBS=debug
endif
IOTIVITY_TARGET_ARCH := ${shell echo $(CONFIG_ARCH_FAMILY) | sed 's/"//g'}
IOTIVITY_RELEASE=${shell echo $(CONFIG_IOTIVITY_RELEASE_VERSION) | sed 's/"//g'}
IOTIVITY_BASE_DIR=$(IOTIVITYDIR)/iotivity_$(IOTIVITY_RELEASE)
IOTIVITY_CONFIG_DIR=$(IOTIVITY_BASE_DIR)/resource/c_common
IOTIVITY_LIBS_DIR=$(IOTIVITY_BASE_DIR)/out/tizenrt/$(IOTIVITY_TARGET_ARCH)/$(IOTIVITY_LIBS)

ifeq ($(Q),)
DEBUG=1
endif

all: build
.PHONY: depend clean distclean

build:
	$(Q) touch $(IOTIVITY_BASE_DIR)/iotivity.built
	$(Q) $(call DELDIR, $(IOTIVITY_BASE_DIR)/out)
	$(Q) $(call DELDIR, $(IOTIVITY_BASE_DIR)/.sconf_temp)
	$(Q) $(call DELFILE, $(IOTIVITY_BASE_DIR)/.sconsign.dblite)
	$(Q) $(call DELFILE, $(IOTIVITY_BASE_DIR)/config.log)
	$(Q) echo "Launching IoTivity Build"
	$(Q) TOPDIR="$(TOPDIR)" DEBUG="${DEBUG}" $(IOTIVITYDIR)/build_iotivity.sh
	$(Q) cp $(IOTIVITY_LIBS_DIR)/*.a $(IOTIVITYDIR)

depend:

context:
	$(Q) touch $(IOTIVITY_CONFIG_DIR)/iotivity_config.h

clean:
	$(Q) $(call DELFILE, $(IOTIVITY_BASE_DIR)/iotivity.built)
	$(Q) echo "Cleaning IoTivity Build"
	$(Q) TOPDIR="$(TOPDIR)" DEBUG="${DEBUG}" $(IOTIVITYDIR)/clean_iotivity.sh
	$(Q) $(call DELDIR, $(IOTIVITY_BASE_DIR)/out)
	$(Q) $(call DELDIR, $(IOTIVITY_BASE_DIR)/.sconf_temp)
	$(Q) $(call DELDIR, $(IOTIVITY_BASE_DIR)/extlibs/mbedtls/mbedtls)
	$(Q) $(call DELFILE, $(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/*.o)
	$(Q) $(call DELFILE, $(IOTIVITY_BASE_DIR)/.sconsign.dblite)
	$(call CLEAN)

distclean:
	$(Q) $(call DELFILE, $(IOTIVITY_CONFIG_DIR)/iotivity_config.h)
