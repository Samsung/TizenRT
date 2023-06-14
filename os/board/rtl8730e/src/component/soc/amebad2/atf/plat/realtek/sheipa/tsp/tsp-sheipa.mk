# Copyright (c) 2019, Realtek Semiconductor Corp. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# TSP source files specific to sheipa platform
BL32_SOURCES		+=	plat/common/aarch64/platform_mp_stack.S	\
				plat/realtek/sheipa/tsp/tsp_plat_setup.c\
				$(SHEIPA_GIC_SOURCES)
