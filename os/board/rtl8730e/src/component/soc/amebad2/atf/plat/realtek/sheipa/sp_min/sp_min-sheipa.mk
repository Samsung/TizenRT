# Copyright (c) 2019, Realtek Semiconductor Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

BL32_SOURCES		+=	plat/realtek/sheipa/sp_min/sp_min_setup.c	\
				${SHEIPA_CPU_LIBS}			\
				plat/common/aarch32/platform_mp_stack.S \
				plat/common/plat_psci_common.c		\
				plat/realtek/sheipa/sheipa_pm.c		\
				plat/realtek/sheipa/topology.c		\
				plat/realtek/sheipa/service/rtk_svc_setup.c\
				plat/realtek/sheipa/service/bsec_svc.c\
				${SHEIPA_GIC_SOURCES}
