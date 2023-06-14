#
# Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2017, Realtek. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

BL1_SOURCES		+=	bl1/bl1_main.c				\
				bl1/${ARCH}/bl1_arch_setup.c		\
				bl1/${ARCH}/bl1_context_mgmt.c		\
				bl1/${ARCH}/bl1_entrypoint.S		\
				bl1/${ARCH}/bl1_entrypoint_rom.S		\
				bl1/${ARCH}/bl1_entrypoint_sram.S		\
				bl1/${ARCH}/bl1_exceptions.S		\
				plat/common/plat_bl1_common.c		\
				plat/common/${ARCH}/platform_up_stack.S \
				${MBEDTLS_SOURCES}

ifneq (${ARCH},mips)
BL1_SOURCES             +=      lib/cpus/${ARCH}/cpu_helpers.S          \
				lib/cpus/errata_report.c		\
				lib/el3_runtime/${ARCH}/context_mgmt.c
endif

ifeq (${ARCH},aarch32)
BL1_SOURCES		+=	lib/cpus/aarch32/dsu_helpers.S
endif

ifeq (${ARCH},aarch64)
BL1_SOURCES		+=	lib/cpus/aarch64/dsu_helpers.S		\
				lib/el3_runtime/aarch64/context.S
endif

ifeq (${TRUSTED_BOARD_BOOT},1)
ifneq (${ARCH},mips)
BL1_SOURCES		+=	bl1/bl1_fwu.c
endif
endif

ifneq (${ARCH},mips)
BL1_LINKERFILE		:=	bl1/bl1.ld.S
else
BL1_LINKERFILE		:=	bl1/mips/bl1.ld.S
endif
