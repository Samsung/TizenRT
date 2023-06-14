#
# Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
# Copyright (c) 2017, Realtek. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

BL2_SOURCES		+=	bl2/bl2_image_load_v2.c			\
				bl2/bl2_main.c				\
				bl2/${ARCH}/bl2_arch_setup.c		\
				plat/common/${ARCH}/platform_up_stack.S	\
				${MBEDTLS_SOURCES}	\
				drivers/arm/systimer/systimer.c

ifneq (${ARCH},mips)
BL2_SOURCES		+=	lib/locks/exclusive/${ARCH}/spinlock.S
endif

ifeq (${ARCH},aarch64)
BL2_SOURCES		+=	common/aarch64/early_exceptions.S
endif

ifeq (${BL2_AT_EL3},0)
BL2_SOURCES		+=	bl2/${ARCH}/bl2_entrypoint.S
BL2_LINKERFILE		:=	bl2/bl2.ld.S

else
BL2_SOURCES		+=	bl2/${ARCH}/bl2_el3_entrypoint.S	\
				bl2/${ARCH}/bl2_el3_exceptions.S	\
				lib/cpus/${ARCH}/cpu_helpers.S		\
				lib/cpus/errata_report.c

ifneq (${ARCH},mips)
BL2_SOURCES		+=	lib/cpus/${ARCH}/dsu_helpers.S
endif

BL2_LINKERFILE		:=	bl2/bl2_el3.ld.S
endif
