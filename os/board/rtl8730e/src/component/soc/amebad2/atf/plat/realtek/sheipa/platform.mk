#
# Copyright (c) 2013-2019, Realtek Semiconductor Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

ifeq (${CONFIG_SOC_CPU_ARMv8_2}, y)
ARM_GIC_ARCH		:=	3
else
ARM_GIC_ARCH		:=	2
endif

ifeq (${SPD},opteed)
add-lib-optee 		:= 	yes
endif

include lib/libfdt/libfdt.mk

DIR_ATF		:= $(shell pwd)

SHEIPA_CPU_LIBS		:=	lib/cpus/$(ARCH)/aem_generic.S
ifeq (${HW_ASSISTED_COHERENCY}, 0)
SHEIPA_CPU_LIBS		+=	lib/cpus/$(ARCH)/cortex_a7.S	\
				lib/cpus/$(ARCH)/cortex_a32.S

#lib/cpus/$(ARCH)/cortex_a53.S		
#lib/cpus/$(ARCH)/cortex_a57.S

else
SHEIPA_CPU_LIBS		+=	lib/cpus/$(ARCH)/cortex_a55.S		\
				lib/cpus/$(ARCH)/cortex_a75.S
endif

PLAT_INCLUDES		:=	-Iinclude/plat/arm/common/		\
				-Iinclude/plat/arm/common/aarch64/	\
				-Iplat/realtek/sheipa/include		\
				-Iinclude/common/tbbr

include lib/xlat_tables_v2/xlat_tables.mk
PLAT_BL_COMMON_SOURCES	:=	plat/realtek/sheipa/sheipa_common.c	\
				plat/realtek/sheipa/sheipa_console_setup.c \
				plat/realtek/sheipa/$(ARCH)/plat_helpers.S \
				drivers/sheipa/uart/$(ARCH)/sheipa_console.S \
				${XLAT_TABLES_LIB_SRCS}

BL1_SOURCES		+=	drivers/io/io_semihosting.c		\
				drivers/io/io_storage.c			\
				drivers/io/io_fip.c			\
				drivers/io/io_memmap.c			\
				lib/semihosting/semihosting.c		\
				lib/semihosting/$(ARCH)/semihosting_call.S \
				plat/realtek/sheipa/sheipa_io_storage.c	\
				${SHEIPA_CPU_LIBS}			\
				plat/realtek/sheipa/sheipa_bl1_setup.c
ifeq (${BOOT_FLASH}, 1)
BL1_SOURCES		+=	plat/realtek/sheipa/spi/spi-sheipa.c	\
			  	plat/realtek/sheipa/ddr_init/ddr_init_sheipa.c
endif


BL2_SOURCES		+=	drivers/io/io_semihosting.c		\
				drivers/io/io_storage.c			\
				drivers/io/io_fip.c			\
				drivers/io/io_memmap.c			\
				lib/semihosting/semihosting.c		\
				lib/semihosting/$(ARCH)/semihosting_call.S		\
				plat/realtek/sheipa/sheipa_io_storage.c			\
				plat/realtek/sheipa/sheipa_bl2_setup.c			\
				plat/realtek/sheipa/sheipa_bl2_mem_params_desc.c	\
				plat/realtek/sheipa/sheipa_image_load.c			\
				common/desc_image_load.c		\
				$(LIBFDT_SRCS)

ifeq ($(add-lib-optee),yes)
BL2_SOURCES		+=	lib/optee/optee_utils.c
endif

ifeq (${ARM_GIC_ARCH},2)
SHEIPA_GIC_SOURCES	=	drivers/arm/gic/common/gic_common.c	\
				drivers/arm/gic/v${ARM_GIC_ARCH}/gicv${ARM_GIC_ARCH}_main.c	\
				drivers/arm/gic/v${ARM_GIC_ARCH}/gicv${ARM_GIC_ARCH}_helpers.c
endif

ifeq (${ARM_GIC_ARCH},3)
include drivers/arm/gic/v3/gicv3.mk

SHEIPA_GIC_SOURCES	=	${GICV3_SOURCES}
endif

SHEIPA_GIC_SOURCES	+=	plat/common/plat_gicv${ARM_GIC_ARCH}.c	\
				plat/realtek/sheipa/sheipa_gic.c

BL31_SOURCES		+=	${SHEIPA_CPU_LIBS}			\
				plat/common/plat_psci_common.c		\
				plat/realtek/sheipa/sheipa_pm.c		\
				plat/realtek/sheipa/topology.c		\
				plat/realtek/sheipa/sheipa_bl31_setup.c	\
				${SHEIPA_GIC_SOURCES}

SEPARATE_CODE_AND_RODATA	:= 1

BL32_RAM_LOCATION	:=	tdram
ifeq (${BL32_RAM_LOCATION}, tsram)
BL32_RAM_LOCATION_ID = SEC_SRAM_ID
else ifeq (${BL32_RAM_LOCATION}, tdram)
BL32_RAM_LOCATION_ID = SEC_DRAM_ID
else
$(error "Unsupported BL32_RAM_LOCATION value")
endif

ifneq (${TRUSTED_BOARD_BOOT},0)

# By default, ARM platforms use RSA keys
KEY_ALG	:=	rsa
# ARM platforms use RSA keys
MBEDTLS_DIR	:= $(DIR_ATF)/mbedtls

# use the ROTPK hash that is hardcoded in the ARM platform port. The
# private/public RSA key pair may be found in plat/arm/board/common/rotpk.
# TODO: Fast model did't have OTP HW, so we can't enable "regs" config
ARM_ROTPK_LOCATION	=	devel_rsa

# ROTPK key location, using arm example
# TODO: we use rotpk of arm and you have to generate your rotpk for your
# products.
ROT_KEY	=	$(DIR_ATF)/plat/arm/board/common/rotpk/arm_rotprivk_rsa.pem
# Include common TBB sources
AUTH_SOURCES	:=	drivers/auth/auth_mod.c			\
			drivers/auth/crypto_mod.c		\
			drivers/auth/img_parser_mod.c		\
			drivers/auth/tbbr/tbbr_cot.c		\
			plat/common/tbbr/plat_tbbr.c

BL1_SOURCES		+=	${AUTH_SOURCES}			\
			bl1/tbbr/tbbr_img_desc.c		\
			plat/realtek/sheipa/sheipa_bl1_fwu.c	\
			plat/realtek/sheipa/board_sheipa_trusted_boot.c

BL2_SOURCES		+=	${AUTH_SOURCES}			\
			plat/realtek/sheipa/board_sheipa_trusted_boot.c

$(eval $(call TOOL_ADD_IMG,ns_bl2u,--fwu,FWU_))

# We expect to locate the *.mk files under the directories specified below
CRYPTO_LIB_MK := drivers/auth/mbedtls/mbedtls_crypto.mk
IMG_PARSER_LIB_MK := drivers/auth/mbedtls/mbedtls_x509.mk

$(info Including ${CRYPTO_LIB_MK})
include ${CRYPTO_LIB_MK}

$(info Including ${IMG_PARSER_LIB_MK})
include ${IMG_PARSER_LIB_MK}

# ROTPK hash location
ifeq (${ARM_ROTPK_LOCATION}, regs)
ARM_ROTPK_LOCATION_ID = ARM_ROTPK_REGS_ID
else ifeq (${ARM_ROTPK_LOCATION}, devel_rsa)
ARM_ROTPK_LOCATION_ID = ARM_ROTPK_DEVEL_RSA_ID
else ifeq (${ARM_ROTPK_LOCATION}, devel_ecdsa)
ARM_ROTPK_LOCATION_ID = ARM_ROTPK_DEVEL_ECDSA_ID
else
$(error "Unsupported ARM_ROTPK_LOCATION value")
endif
$(eval $(call add_define,ARM_ROTPK_LOCATION_ID))

# Certificate NV-Counters. Use values corresponding to tied off values in
# ARM development platforms
TFW_NVCTR_VAL	?=	31
NTFW_NVCTR_VAL	?=	223
endif

# Add the build options to pack Trusted OS Extra1 and Trusted OS Extra2 images
# in the FIP if the platform requires.
ifneq ($(BL32_EXTRA1),)
$(eval $(call TOOL_ADD_IMG,bl32_extra1,--tos-fw-extra1))
endif
ifneq ($(BL32_EXTRA2),)
$(eval $(call TOOL_ADD_IMG,bl32_extra2,--tos-fw-extra2))
endif

# Process CPU_ARMv8_2 flag
ifeq (${CONFIG_SOC_CPU_ARMv8_2},y)
CPU_ARMv8_2		:=	1
else
CPU_ARMv8_2		:=	0
endif
$(eval $(call assert_boolean,ARM_PLAT_MT))
$(eval $(call add_define,CPU_ARMv8_2))
$(eval $(call add_define,ARM_GIC_ARCH))

# Process flags
$(eval $(call add_define,BL32_RAM_LOCATION_ID))

# Enable loading bl32 image
ifneq (${CONFIG_BOOT_BL32_NONE},y)
$(eval $(call add_define,BL32_LOAD_IMAGE))
endif
