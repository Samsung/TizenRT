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
# LibTargets.mk
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

# Archive targets.  The target build sequence will first create a series of
# libraries, one per configured source file directory.  The final TinyAra
# execution will then be built from those libraries.  The following targets
# build those libraries.
#
# Possible kernel-mode builds

libc$(DELIM)libkc$(LIBEXT): context
	$(Q) $(MAKE) -C $(LIB_DIR)$(DELIM)libc TOPDIR="$(TOPDIR)" libkc$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libkc$(LIBEXT): libc$(DELIM)libkc$(LIBEXT)
	$(Q) install $(LIB_DIR)$(DELIM)libc$(DELIM)libkc$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libkc$(LIBEXT)

mm$(DELIM)libkmm$(LIBEXT): context
	$(Q) $(MAKE) -C mm TOPDIR="$(TOPDIR)" libkmm$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libkmm$(LIBEXT): mm$(DELIM)libkmm$(LIBEXT)
	$(Q) install mm$(DELIM)libkmm$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libkmm$(LIBEXT)

$(ARCH_SRC)$(DELIM)libkarch$(LIBEXT): context
	$(Q) $(MAKE) -C $(ARCH_SRC) TOPDIR="$(TOPDIR)" libkarch$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libkarch$(LIBEXT): $(ARCH_SRC)$(DELIM)libkarch$(LIBEXT)
	$(Q) install $(ARCH_SRC)$(DELIM)libkarch$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libkarch$(LIBEXT)

kernel$(DELIM)libkernel$(LIBEXT): context
	$(Q) $(MAKE) -C kernel TOPDIR="$(TOPDIR)" libkernel$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libkernel$(LIBEXT): kernel$(DELIM)libkernel$(LIBEXT)
	$(Q) install kernel$(DELIM)libkernel$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libkernel$(LIBEXT)

net$(DELIM)libnet$(LIBEXT): context
	$(Q) $(MAKE) -C net TOPDIR="$(TOPDIR)" libnet$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libnet$(LIBEXT): net$(DELIM)libnet$(LIBEXT)
	$(Q) install net$(DELIM)libnet$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libnet$(LIBEXT)

pm$(DELIM)libpm$(LIBEXT): context
	$(Q) $(MAKE) -C pm TOPDIR="$(TOPDIR)" libpm$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libpm$(LIBEXT): pm$(DELIM)libpm$(LIBEXT)
	$(Q) install pm$(DELIM)libpm$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libpm$(LIBEXT)

logm$(DELIM)liblogm$(LIBEXT): context
	$(Q) $(MAKE) -C logm TOPDIR="$(TOPDIR)" liblogm$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)liblogm$(LIBEXT): logm$(DELIM)liblogm$(LIBEXT)
	$(Q) install logm$(DELIM)liblogm$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)liblogm$(LIBEXT)

fs$(DELIM)libfs$(LIBEXT): context
	$(Q) $(MAKE) -C fs TOPDIR="$(TOPDIR)" libfs$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libfs$(LIBEXT): fs$(DELIM)libfs$(LIBEXT)
	$(Q) install fs$(DELIM)libfs$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libfs$(LIBEXT)

audio$(DELIM)libaudio$(LIBEXT): context
	$(Q) $(MAKE) -C audio TOPDIR="$(TOPDIR)" libaudio$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libaudio$(LIBEXT): audio$(DELIM)libaudio$(LIBEXT)
	$(Q) install audio$(DELIM)libaudio$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libaudio$(LIBEXT)

se$(DELIM)libse$(LIBEXT): context
	$(Q) $(MAKE) -C se TOPDIR="$(TOPDIR)" libse$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libse$(LIBEXT): se$(DELIM)libse$(LIBEXT)
	$(Q) install se$(DELIM)libse$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libse$(LIBEXT)

crypto$(DELIM)libcrypto$(LIBEXT): context
	$(Q) $(MAKE) -C crypto TOPDIR="$(TOPDIR)" libcrypto$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libcrypto$(LIBEXT): crypto$(DELIM)libcrypto$(LIBEXT)
	$(Q) install crypto$(DELIM)libcrypto$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libcrypto$(LIBEXT)

drivers$(DELIM)libdrivers$(LIBEXT): context
	$(Q) $(MAKE) -C drivers TOPDIR="$(TOPDIR)" libdrivers$(LIBEXT) KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libdrivers$(LIBEXT): drivers$(DELIM)libdrivers$(LIBEXT)
	$(Q) install drivers$(DELIM)libdrivers$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libdrivers$(LIBEXT)

# Special case

syscall$(DELIM)libstubs$(LIBEXT): context
	$(Q) $(MAKE) -C syscall TOPDIR="$(TOPDIR)" libstubs$(LIBEXT) # KERNEL=y EXTRADEFINES=$(KDEFINE)

$(LIBRARIES_DIR)$(DELIM)libstubs$(LIBEXT): syscall$(DELIM)libstubs$(LIBEXT)
	$(Q) install syscall$(DELIM)libstubs$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libstubs$(LIBEXT)

# Possible user-mode builds

libc$(DELIM)libuc$(LIBEXT): context
	$(Q) $(MAKE) -C $(LIB_DIR)$(DELIM)libc TOPDIR="$(TOPDIR)" libuc$(LIBEXT) KERNEL=n

$(LIBRARIES_DIR)$(DELIM)libuc$(LIBEXT): libc$(DELIM)libuc$(LIBEXT)
	$(Q) install $(LIB_DIR)$(DELIM)libc$(DELIM)libuc$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libuc$(LIBEXT)

mm$(DELIM)libumm$(LIBEXT): context
	$(Q) $(MAKE) -C mm TOPDIR="$(TOPDIR)" libumm$(LIBEXT) KERNEL=n

$(LIBRARIES_DIR)$(DELIM)libumm$(LIBEXT): mm$(DELIM)libumm$(LIBEXT)
	$(Q) install mm$(DELIM)libumm$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libumm$(LIBEXT)

$(ARCH_SRC)$(DELIM)libuarch$(LIBEXT): context
	$(Q) $(MAKE) -C $(ARCH_SRC) TOPDIR="$(TOPDIR)" libuarch$(LIBEXT) KERNEL=n

$(LIBRARIES_DIR)$(DELIM)libuarch$(LIBEXT): $(ARCH_SRC)$(DELIM)libuarch$(LIBEXT)
	$(Q) install $(ARCH_SRC)$(DELIM)libuarch$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libuarch$(LIBEXT)

libxx$(DELIM)libcxx$(LIBEXT): context
	$(Q) $(MAKE) -C $(LIB_DIR)$(DELIM)libxx TOPDIR="$(TOPDIR)" libcxx$(LIBEXT) KERNEL=n
ifeq ($(CONFIG_LIBCXX),y)
	$(Q) $(MAKE) -C $(EXTDIR)$(DELIM)libcxx TOPDIR="$(TOPDIR)" all KERNEL=n
endif

$(LIBRARIES_DIR)$(DELIM)libcxx$(LIBEXT): libxx$(DELIM)libcxx$(LIBEXT)
	$(Q) install $(LIB_DIR)$(DELIM)libxx$(DELIM)libcxx$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libcxx$(LIBEXT)

$(APPDIR)$(DELIM)libapps$(LIBEXT): context
	$(Q) $(MAKE) -C $(APPDIR) TOPDIR="$(TOPDIR)" EXTDIR="$(EXTDIR)" libapps$(LIBEXT) KERNEL=n

$(LIBRARIES_DIR)$(DELIM)libapps$(LIBEXT): $(APPDIR)$(DELIM)libapps$(LIBEXT)
	$(Q) install $(APPDIR)$(DELIM)libapps$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libapps$(LIBEXT)

syscall$(DELIM)libproxies$(LIBEXT): context
	$(Q) $(MAKE) -C syscall TOPDIR="$(TOPDIR)" libproxies$(LIBEXT) KERNEL=n

$(LIBRARIES_DIR)$(DELIM)libproxies$(LIBEXT): syscall$(DELIM)libproxies$(LIBEXT)
	$(Q) install syscall$(DELIM)libproxies$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libproxies$(LIBEXT)

$(FRAMEWORK_LIB_DIR)$(DELIM)libframework$(LIBEXT): context
	$(Q) $(MAKE) -C $(FRAMEWORK_LIB_DIR) TOPDIR="$(TOPDIR)" libframework$(LIBEXT) KERNEL=n

$(LIBRARIES_DIR)$(DELIM)libframework$(LIBEXT): $(FRAMEWORK_LIB_DIR)$(DELIM)libframework$(LIBEXT)
	$(Q) install $(FRAMEWORK_LIB_DIR)$(DELIM)libframework$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libframework$(LIBEXT)

$(EXTDIR)$(DELIM)libexternal$(LIBEXT): context
	$(Q) $(MAKE) -C $(EXTDIR) TOPDIR="$(TOPDIR)" EXTDIR="$(EXTDIR)" libexternal$(LIBEXT) KERNEL=n
ifeq ($(CONFIG_ENABLE_IOTIVITY),y)
	$(Q) $(MAKE) -C $(EXTDIR)/iotivity TOPDIR="$(TOPDIR)" EXTDIR="$(EXTDIR)" KERNEL=n
endif
ifeq ($(CONFIG_ENABLE_IOTJS),y)
	$(Q) $(MAKE) -C $(EXTDIR)/iotjs/config/tizenrt TOPDIR="$(TOPDIR)" EXTDIR="$(EXTDIR)" KERNEL=n
endif

$(LIBRARIES_DIR)$(DELIM)libexternal$(LIBEXT): $(EXTDIR)$(DELIM)libexternal$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)libexternal$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libexternal$(LIBEXT)

#Iotivity Libs

ifeq ($(CONFIG_ENABLE_IOTIVITY),y)
$(LIBRARIES_DIR)$(DELIM)libiotivity$(LIBEXT): $(EXTDIR)$(DELIM)iotivity$(DELIM)libiotivity$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)iotivity$(DELIM)libiotivity$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libiotivity$(LIBEXT)
endif # CONFIG_ENABLE_IOTIVITY

#IoTjs Libs

ifeq ($(CONFIG_ENABLE_IOTJS),y)
$(LIBRARIES_DIR)$(DELIM)libhttpparser$(LIBEXT): $(EXTDIR)$(DELIM)iotjs$(DELIM)libhttpparser$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)iotjs$(DELIM)libhttpparser$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libhttpparser$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libiotjs$(LIBEXT): $(EXTDIR)$(DELIM)iotjs$(DELIM)libiotjs$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)iotjs$(DELIM)libiotjs$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libiotjs$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libjerry-core$(LIBEXT): $(EXTDIR)$(DELIM)iotjs$(DELIM)libjerry-core$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)iotjs$(DELIM)libjerry-core$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libjerry-core$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libtuv$(LIBEXT): $(EXTDIR)$(DELIM)iotjs$(DELIM)libtuv$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)iotjs$(DELIM)libtuv$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libtuv$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libjerry-libm$(LIBEXT): $(EXTDIR)$(DELIM)iotjs$(DELIM)libjerry-libm$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)iotjs$(DELIM)libjerry-libm$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libjerry-libm$(LIBEXT)
endif

# Possible non-kernel builds

libc$(DELIM)libc$(LIBEXT): context
	$(Q) $(MAKE) -C $(LIB_DIR)$(DELIM)libc TOPDIR="$(TOPDIR)" libc$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libc$(LIBEXT): libc$(DELIM)libc$(LIBEXT)
	$(Q) install $(LIB_DIR)$(DELIM)libc$(DELIM)libc$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libc$(LIBEXT)

mm$(DELIM)libmm$(LIBEXT): context
	$(Q) $(MAKE) -C mm TOPDIR="$(TOPDIR)" libmm$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libmm$(LIBEXT): mm$(DELIM)libmm$(LIBEXT)
	$(Q) install mm$(DELIM)libmm$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libmm$(LIBEXT)

$(ARCH_SRC)$(DELIM)libarch$(LIBEXT): context
	$(Q) $(MAKE) -C $(ARCH_SRC) TOPDIR="$(TOPDIR)" libarch$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libarch$(LIBEXT): $(ARCH_SRC)$(DELIM)libarch$(LIBEXT)
	$(Q) install $(ARCH_SRC)$(DELIM)libarch$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libarch$(LIBEXT)

# External WICED Lib builds
ifeq ($(CONFIG_WL_BCM4390X),y)
$(LIBRARIES_DIR)$(DELIM)libbcmexternal$(LIBEXT): $(EXTDIR)$(DELIM)WICED$(DELIM)libbcmexternal$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)WICED$(DELIM)libbcmexternal$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libbcmexternal$(LIBEXT)
endif

ifeq ($(CONFIG_RTK_WLAN),y)
$(LIBRARIES_DIR)$(DELIM)librtl$(LIBEXT): $(TOPDIR)$(DELIM)drivers$(DELIM)wireless$(DELIM)realtek$(DELIM)librtl$(LIBEXT)
	$(Q) install $(TOPDIR)$(DELIM)drivers$(DELIM)wireless$(DELIM)realtek$(DELIM)librtl$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)librtl$(LIBEXT)
endif

# External esp32 wifi static Lib builds
ifeq ($(CONFIG_ESP32_WIFI_SUPPORT),y)
$(LIBRARIES_DIR)$(DELIM)libcoexist$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libcoexist$(LIBEXT)
	$(Q)install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libcoexist$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libcoexist$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libcore$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libcore$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libcore$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libcore$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libespnow$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libespnow$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libespnow$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libespnow$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libmesh$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libmesh$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libmesh$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libmesh$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libnet80211$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libnet80211$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libnet80211$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libnet80211$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libphy$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libphy$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libphy$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libphy$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libpp$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libpp$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libpp$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libpp$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)librtc$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)librtc$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)librtc$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)librtc$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libsmartconfig$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libsmartconfig$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libsmartconfig$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libsmartconfig$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libwpa2$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libwpa2$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libwpa2$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libwpa2$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libwpa$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libwpa$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libwpa$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libwpa$(LIBEXT)

$(LIBRARIES_DIR)$(DELIM)libwps$(LIBEXT): $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libwps$(LIBEXT)
	$(Q) install $(EXTDIR)$(DELIM)esp_idf_port/esp32$(DELIM)lib$(DELIM)libwps$(LIBEXT) $(LIBRARIES_DIR)$(DELIM)libwps$(LIBEXT)
endif
