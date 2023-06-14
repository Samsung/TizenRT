#
# Realtek Semiconductor Corp.
#
# Tony Wu (tonywu@realtek.com)
# Aug. 15, 2015
#
DIR_ATF		:= $(shell pwd)
DIR_OPTEE	:= $(shell pwd)/boot/optee
DIR_UBOOT	:= $(DIR_ROOT)/package/boot/uboot
DIR_FREERTOS	:= $(DIR_ROOT)/package/boot/freertos
DIR_ATF_BUILD	:= $(DIR_ATF)/build/sheipa/debug

UBOOT_CONFIG 	:= /dev/null

ifneq (,$(wildcard $(DIR_UBOOT)/.config))
UBOOT_CONFIG	:= $(DIR_UBOOT)/.config
endif

SPD		:= none

ifeq ($(CONFIG_BOOT_BL32_TSP),y)
SPD		:= tspd
endif

ATF_DEP_PKGS	:= boot/freertos

ifeq ($(CONFIG_BOOT_BL32_OPTEE),y)
BL32		:= $(DIR_OPTEE)/optee_os/out/arm/core/tee-pager.bin
SPD		:= opteed
ATF_DEP_PKGS	+= boot/optee
endif

ifeq ($(CONFIG_BOOT_BL32_SP_MIN),y)
AARCH32_SP	:= sp_min
endif

ifeq ($(CONFIG_ARCH_atf),"aarch32")
NEED_BL32	:= yes
SPD		:= none
endif

ifeq ($(CONFIG_BOOT_BL33_UBOOT),y)
UBOOT_OF_CTRL	:= $(shell \
		     grep "^CONFIG_OF_CONTROL=*" $(UBOOT_CONFIG) | \
		     sed 's/.*=//g')

ifeq ($(UBOOT_OF_CTRL),y)
BL33		:= $(DIR_UBOOT)/u-boot-dtb.bin
else
BL33		:= $(DIR_UBOOT)/u-boot.bin
endif
endif

BL33 := $(DIR_ATF)/../../../../project/realtek_amebaD2_va0_example/GCC-RELEASE/project_ap/asdk/image/ca32_image2_all.bin
#$(DIR_FREERTOS)/Boards/Realtek_Sheipa_ARM_CA35_32/rtosdemo.bin

ifeq ($(CONFIG_SOC_CPU_ARMv7), y)
ARM_ARCH_MAJOR		:= 7
ARM_CORTEX_A7		:= $(if $(CONFIG_SOC_CPU_ARMA7),yes)
endif

ifeq ($(CONFIG_SOC_CPU_ARMA55), y)
HW_ASSISTED_COHERENCY	:= 1
USE_COHERENT_MEM	:= 0
endif

ifeq ($(CONFIG_SOC_CPU_ARMA75), y)
HW_ASSISTED_COHERENCY	:= 1
USE_COHERENT_MEM	:= 0
endif

BUILD_STRING	:= $(shell git log -1 --pretty=format:"%h")

$(eval $(call BuildPackage,boot/atf,$(ATF_DEP_PKGS)))

define boot/atf/romfs
endef

define boot/atf/image
	$(Q)$(call PKG_EXEC,make -C $(DIR_ATF) fiptool fip)
endef
#	$(Q)mkdir -p $(DIR_IMAGE) && \
#	cp -r $(DIR_ATF_BUILD)/bl1.bin $(DIR_IMAGE) && \
#	cp -r $(DIR_ATF_BUILD)/fip.bin $(DIR_IMAGE)

define boot/atf/clean
	$(Q)rm -rf $(DIR_ATF)/build
endef
