###########################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
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
# syscall/Makefile
#
#   Copyright (C) 2011-2013 Gregory Nutt. All rights reserved.
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
###########################################################################

TOPDIR ?= $(abspath ../../../../..)

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
DELIM ?= $(strip /)

bsp_path := $(abspath ../..)

build_dir := $(abspath $(TOPDIR)/../build)
tfm_build_dir := $(abspath ./tfm_build)

security_h := $(tfm_build_dir)/_build/security.h

all: tfm postbuild
.PHONY: clean distclean tfm cmake_install py_libs prebuild

export PATH := $(CURDIR)/cmake-3.15.7/bin/:$(PATH)
export PYTHONPATH := $(abspath $(build_dir)/tools/armino/bk_py_libs):$(PYTHONPATH)
CONFIG_NUM_APPS ?= 0

ifeq ($(CONFIG_BINARY_SIGNING),y)
CONFIG_BINARY_SIGNING := 1
else
CONFIG_BINARY_SIGNING := 0
endif

ifeq ($(CONFIG_XIP_KERNEL),y)
CONFIG_XIP_KERNEL := 1
else
CONFIG_XIP_KERNEL := 0
endif

tfm: prebuild
	$(Q) cmake -S tfm -B $(tfm_build_dir) \
		-DCMAKE_PREFIX_PATH=/opt/gcc-arm-none-eabi-10.3-2021.10/bin \
		-DCMAKE_INSTALL_PREFIX=$(tfm_build_dir)/tfm_install \
		-DARMINO_SDK_PATH=$(bsp_path) \
		-DTFM_PLATFORM=beken/bk7239n \
		-DTFM_TOOLCHAIN_FILE=tfm/toolchain_GNUARM.cmake \
		-DCMAKE_BUILD_TYPE=minsizerel \
		-DTEST_S=OFF \
		-DTEST_NS=OFF \
		-DUSER=$(USER) \
		-DBL2=ON \
		-DTFM_PROFILE=profile_medium \
		-DFPGA=OFF \
		-DOTP_NV_COUNTERS_RAM_EMULATION=OFF \
		-DPLATFORM_DEFAULT_OTP_WRITEABLE=ON \
		-DCONFIG_TFM_LOG_LEVEL=OFF \
		-DMCUBOOT_LOG_LEVEL=ERROR \
		-DPARTITION_HDR_DIR=$(tfm_build_dir)/_build \
		-DMCUBOOT_UPGRADE_STRATEGY=DIRECT_XIP \
		-DTFM_AON_PMU_LL_NSC= \
		-DTFM_SYS_LL_NSC= \
		-DCONFIG_TFM_BK7236_V5=y \
		-DBL2_DOWNLOAD=y \
		-DBL2_SECURE_DEBUG= \
		-DCONFIG_REG_ACCESS_NSC=$(CONFIG_TFM_REG_ACCESS_NSC) \
		-DCONFIG_MPC_NSC=${CONFIG_TFM_MPC_NSC} \
		-DCONFIG_FLASH_NSC=$(CONFIG_TFM_FLASH_NSC) \
		-DCONFIG_DUBHE_KEY_LADDER_NSC=$(CONFIG_TFM_DUBHE_KEY_LADDER_NSC) \
		-DCONFIG_TFM_READ_FLASH_NSC=$(CONFIG_TFM_READ_FLASH_NSC) \
		-DCONFIG_OTP_NSC=$(CONFIG_TFM_OTP_NSC) \
		-DCONFIG_INT_TARGET_NSC=${CONFIG_TFM_INT_TARGET_NSC} \
		-DCONFIG_TFM_MPU=${CONFIG_TFM_MPU} \
		-DCONFIG_TFM_PANIC_DEAD_LOOP= \
		-DCONFIG_XIP_NO_VERSION=y \
		-DCONFIG_BL2_VALIDATE_ENABLED_BY_EFUSE=y \
		-DCONFIG_BL2_SKIP_VALIDATE= \
		-DCONFIG_BL2_WDT=${CONFIG_BL2_WDT} \
		-DCONFIG_PM_NSC=$(CONFIG_PM_NSC) \
		-DCONFIG_AES_GCM_NSC=$(CONFIG_TFM_AES_GCM_NSC) \
		-DCONFIG_ECP_NSC=$(CONFIG_TFM_ECP_NSC) \
		-DCONFIG_ASYM_ALGO_NSC=$(CONFIG_TFM_ASYM_ALGO_NSC) \
		-DCONFIG_HKDF_NSC=$(CONFIG_TFM_HKDF_NSC) \
		-DCONFIG_DRBG_NSC=$(CONFIG_TFM_DRBG_NSC) \
		-DCONFIG_DH_NSC=$(CONFIG_TFM_DH_NSC) \
		-DCONFIG_25519_NSC=$(CONFIG_TFM_25519_NSC) \
		-DCONFIG_SCA_AES_NSC=${CONFIG_TFM_SCA_AES_NSC} \
		-DCONFIG_STARTUP_PERFORMANCE=$(CONFIG_STARTUP_PERFORMANCE) \
		-DCONFIG_TFM_RAM_SIZE=${CONFIG_CPU0_SPE_RAM_SIZE} \
		-DCONFIG_TFM_HEAP_SIZE=${CONFIG_TFM_HEAP_SIZE} \
		-DCONFIG_BL2_WDT_PERIOD=${CONFIG_BL2_WDT_PERIOD} \
		-DCONFIG_INITIAL_ATTESTATION= \
		-DCONFIG_BL2_UPDATE_WITH_PC=${CONFIG_BL2_UPDATE_WITH_PC} \
		-DCONFIG_TFM_CRYPTO_IOVEC_BUFFER_SIZE=${CONFIG_TFM_CRYPTO_IOVEC_BUFFER_SIZE} \
		-DCONFIG_TFM_MBEDTLS_RSA_C=$(CONFIG_TFM_MBEDTLS_RSA_C) \
		-DCONFIG_TFM_MBEDTLS_PKCS1_V15=$(CONFIG_TFM_MBEDTLS_PKCS1_V15) \
		-DCONFIG_BL2_UPDATE_WITH_OTA=${CONFIG_BL2_UPDATE_WITH_OTA} \
		-DCONFIG_NUM_APPS=${CONFIG_NUM_APPS} \
		-DCONFIG_BINARY_SIGNING=${CONFIG_BINARY_SIGNING} \
		-DCONFIG_XIP_KERNEL=${CONFIG_XIP_KERNEL}



	$(Q) cmake --build $(tfm_build_dir) --target install -j16

PRE_HANDLE_SCRIPT := $(abspath $(TOPDIR)/../build/tools/armino/misc/tfm_prehandle.py)

ifeq ($(CONFIG_FLASH_ENCRYPT_ENABLE), y)
	CFG_FLASH_ENCRYPT_ENABLE := 1
else
	CFG_FLASH_ENCRYPT_ENABLE := 0
endif

$(security_h): py_libs
	$(Q) mkdir -p $(tfm_build_dir)/_build
	$(Q) python3 -B $(PRE_HANDLE_SCRIPT) $(bsp_path)/middleware/boards/bk7239n $(tfm_build_dir)/_build $(CFG_FLASH_ENCRYPT_ENABLE)


cmake_exe := cmake-3.15.7/bin/cmake
$(cmake_exe):
	$(Q) tar -zxvf cmake-3.15.7.tgz
	$(Q) find "$(CURDIR)/cmake-3.15.7" -type d -exec chmod a+wx {} \;

py_libs_depend := $(build_dir)/tools/armino/bk_py_libs/.depend
$(py_libs_depend):
	$(Q) tar -zxvf $(build_dir)/tools/armino/bk_py_libs.tgz -C $(build_dir)/tools/armino
	$(Q) find "$(build_dir)/tools/armino/bk_py_libs" -type d -exec chmod a+wx {} \;
	$(Q) touch $(build_dir)/tools/armino/bk_py_libs/.depend

py_libs: $(py_libs_depend)

prebuild: py_libs $(security_h) $(cmake_exe)

ifeq ($(CONFIG_XIP_KERNEL), 1)
tfm_firmware_dir := $(abspath ./tfm_xip_build)
else
tfm_firmware_dir := $(abspath ./tfm_loadable_build)
endif

postbuild: tfm
	$(Q) cp $(tfm_build_dir)/bin/tfm_s.bin $(tfm_build_dir)/_build/
	$(if $(filter y,$(CONFIG_BL2_UPDATE_WITH_PC)),$(Q) cp $(tfm_build_dir)/bin/bl2_B.bin $(tfm_build_dir)/_build/)
	$(Q) cp $(tfm_build_dir)/bin/bl2.bin $(tfm_build_dir)/_build/
	$(Q) find "$(tfm_build_dir)" -type d -exec chmod a+wx {} \;
	$(Q) if [ -d "$(tfm_firmware_dir)" ]; then rm -rf "$(tfm_firmware_dir)"; fi
	$(Q) cp -rf $(tfm_build_dir) $(tfm_firmware_dir)
	$(Q) find "$(tfm_firmware_dir)" -type d -exec chmod a+wx {} \;

clean:
	$(Q) echo clean
	# $(Q) rm -rf $(tfm_build_dir)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
	$(call DELFILE, proxies$(DELIM)*.c)
	$(call DELFILE, stubs$(DELIM)*.c)

# -include Make.dep

