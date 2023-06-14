#
# Copyright (c) 2016-2020, ARM Limited and Contributors. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Default, static values for build variables, listed in alphabetic order.
# Dependencies between build options, if any, are handled in the top-level
# Makefile, after this file is included. This ensures that the former is better
# poised to handle dependencies, as all build variables would have a default
# value by then.

# Use T32 by default
AARCH32_INSTRUCTION_SET		:= T32

# The AArch32 Secure Payload to be built as BL32 image
AARCH32_SP			?= none

# The Target build architecture. Supported values are: aarch64, aarch32.
ARCH				:= $(patsubst "%",%,$(CONFIG_ARCH_atf))

# ARM Architecture major and minor versions: 8.0 by default.
ARM_ARCH_MAJOR			?= 8
ARM_ARCH_MINOR			:= 0

#ifeq (${CONFIG_BOOT_MODE_SECURE}, y)
#BOOT_FLASH			:= 1
#else
BOOT_FLASH			:= 0
#endif

# Base commit to perform code check on
BASE_COMMIT			:= origin/master

# Execute BL2 at EL3
BL2_AT_EL3			:= 0

# BL2 image is stored in XIP memory, for now, this option is only supported
# when BL2_AT_EL3 is 1.
BL2_IN_XIP_MEM			:= 0

# Do dcache invalidate upon BL2 entry at EL3
BL2_INV_DCACHE			:= 1

# Select the branch protection features to use.
BRANCH_PROTECTION		:= 0

# By default, consider that the platform may release several CPUs out of reset.
# The platform Makefile is free to override this value.
COLD_BOOT_SINGLE_CPU		:= 0

# Flag to compile in coreboot support code. Exclude by default. The coreboot
# Makefile system will set this when compiling TF as part of a coreboot image.
COREBOOT			:= 0

# For Chain of Trust
CREATE_KEYS			:= 1

# Build flag to include AArch32 registers in cpu context save and restore during
# world switch. This flag must be set to 0 for AArch64-only platforms.
CTX_INCLUDE_AARCH32_REGS	:= 1

# Include FP registers in cpu context
CTX_INCLUDE_FPREGS		:= 0

# Include pointer authentication (ARMv8.3-PAuth) registers in cpu context. This
# must be set to 1 if the platform wants to use this feature in the Secure
# world. It is not needed to use it in the Non-secure world.
CTX_INCLUDE_PAUTH_REGS		:= 0

# Debug build
DEBUG				:= 1

# By default disable authenticated decryption support.
DECRYPTION_SUPPORT		:= none

# Build platform
DEFAULT_PLAT			:= $(patsubst "%",%,$(CONFIG_PLAT_atf))

# Disable the generation of the binary image (ELF only).
DISABLE_BIN_GENERATION		:= 0

# Enable capability to disable authentication dynamically. Only meant for
# development platforms.
DYN_DISABLE_AUTH		:= 0

# Build option to enable MPAM for lower ELs
ENABLE_MPAM_FOR_LOWER_ELS	:= 0

# Flag to Enable Position Independant support (PIE)
ENABLE_PIE			:= 0

# Flag to enable Performance Measurement Framework
ENABLE_PMF			:= 0

# Flag to enable PSCI STATs functionality
ENABLE_PSCI_STAT		:= 0

# Flag to enable runtime instrumentation using PMF
ENABLE_RUNTIME_INSTRUMENTATION	:= 0

# Flag to enable stack corruption protection
ENABLE_STACK_PROTECTOR		:= 0

# Flag to enable exception handling in EL3
EL3_EXCEPTION_HANDLING		:= 0

# Flag to enable Branch Target Identification.
# Internal flag not meant for direct setting.
# Use BRANCH_PROTECTION to enable BTI.
ENABLE_BTI			:= 0

# Flag to enable Pointer Authentication.
# Internal flag not meant for direct setting.
# Use BRANCH_PROTECTION to enable PAUTH.
ENABLE_PAUTH			:= 0

# By default BL31 encryption disabled
ENCRYPT_BL31			:= 0

# By default BL32 encryption disabled
ENCRYPT_BL32			:= 0

# Default dummy firmware encryption key
ENC_KEY	:= 1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef

# Default dummy nonce for firmware encryption
ENC_NONCE			:= 1234567890abcdef12345678

# Build flag to treat usage of deprecated platform and framework APIs as error.
ERROR_DEPRECATED		:= 0

# Fault injection support
FAULT_INJECTION_SUPPORT		:= 0

# Byte alignment that each component in FIP is aligned to
FIP_ALIGN			:= 0

# Default FIP file name
FIP_NAME			:= fip.bin

# Default FWU_FIP file name
FWU_FIP_NAME			:= fwu_fip.bin

# By default firmware encryption with SSK
FW_ENC_STATUS			:= 0

# For Chain of Trust
GENERATE_COT			:= 0

# Hint platform interrupt control layer that Group 0 interrupts are for EL3. By
# default, they are for Secure EL1.
GICV2_G0_FOR_EL3		:= 0

# Route External Aborts to EL3. Disabled by default; External Aborts are handled
# by lower ELs.
HANDLE_EA_EL3_FIRST		:= 0

# Whether system coherency is managed in hardware, without explicit software
# operations.
HW_ASSISTED_COHERENCY		?= 0

# Set the default algorithm for the generation of Trusted Board Boot keys
KEY_ALG				:= rsa

# Option to build TF with Measured Boot support
MEASURED_BOOT			:= 0

# NS timer register save and restore
NS_TIMER_SWITCH			:= 0

# Include lib/libc in the final image
OVERRIDE_LIBC			:= 0

# Build PL011 UART driver in minimal generic UART mode
PL011_GENERIC_UART		:= 0

# By default, consider that the platform's reset address is not programmable.
# The platform Makefile is free to override this value.
PROGRAMMABLE_RESET_ADDRESS	:= 0

# Flag used to choose the power state format: Extended State-ID or Original
PSCI_EXTENDED_STATE_ID		:= 0

# Enable RAS support
RAS_EXTENSION			:= 0

# By default, BL1 acts as the reset handler, not BL31
RESET_TO_BL31			:= 0

# For Chain of Trust
SAVE_KEYS			:= 0

# Software Delegated Exception support
SDEI_SUPPORT            	:= 0

# Whether code and read-only data should be put on separate memory pages. The
# platform Makefile is free to override this value.
SEPARATE_CODE_AND_RODATA	:= 0

# Put NOBITS sections (.bss, stacks, page tables, and coherent memory) in a
# separate memory region, which may be discontiguous from the rest of BL31.
SEPARATE_NOBITS_REGION		:= 0

# If the BL31 image initialisation code is recalimed after use for the secondary
# cores stack
RECLAIM_INIT_CODE		:= 0

# SPD choice
SPD				?= none

# Enable the Management Mode (MM)-based Secure Partition Manager implementation
SPM_MM				:= 0

# Use SPM at S-EL2 as a default config for SPMD
SPMD_SPM_AT_SEL2		:= 1

# Flag to introduce an infinite loop in BL1 just before it exits into the next
# image. This is meant to help debugging the post-BL2 phase.
SPIN_ON_BL1_EXIT		:= 0

# Flags to build TF with Trusted Boot support
TRUSTED_BOARD_BOOT		:= 0

# Build option to choose whether Trusted Firmware uses Coherent memory or not.
USE_COHERENT_MEM		?= 1

# Build option to add debugfs support
USE_DEBUGFS			:= 0

# Build option to fconf based io
ARM_IO_IN_DTB		:= 0

# Build option to choose whether Trusted Firmware uses library at ROM
USE_ROMLIB			:= 0

# Build option to choose whether the xlat tables of BL images can be read-only.
# Note that this only serves as a higher level option to PLAT_RO_XLAT_TABLES,
# which is the per BL-image option that actually enables the read-only tables
# API. The reason for having this additional option is to have a common high
# level makefile where we can check for incompatible features/build options.
ALLOW_RO_XLAT_TABLES		:= 0

# Chain of trust.
COT				:= tbbr

# Use tbbr_oid.h instead of platform_oid.h
USE_TBBR_DEFS			:= 1

# Build verbosity
V				:= 0

# Whether to enable D-Cache early during warm boot. This is usually
# applicable for platforms wherein interconnect programming is not
# required to enable cache coherency after warm reset (eg: single cluster
# platforms).
WARMBOOT_ENABLE_DCACHE_EARLY	:= 0

# Build option to enable/disable the Statistical Profiling Extensions
ENABLE_SPE_FOR_LOWER_ELS	:= 1

# SPE is only supported on AArch64 so disable it on AArch32.
ifeq (${ARCH},aarch32)
    override ENABLE_SPE_FOR_LOWER_ELS := 0
endif

# Include Memory Tagging Extension registers in cpu context. This must be set
# to 1 if the platform wants to use this feature in the Secure world and MTE is
# enabled at ELX.
CTX_INCLUDE_MTE_REGS := 0

ENABLE_AMU			:= 0

# By default, enable Scalable Vector Extension if implemented for Non-secure
# lower ELs
# Note SVE is only supported on AArch64 - therefore do not enable in AArch32
ifneq (${ARCH},aarch32)
    ENABLE_SVE_FOR_NS		:= 1
else
    override ENABLE_SVE_FOR_NS	:= 0
endif

SANITIZE_UB := off

# For ARMv8.1 (AArch64) platforms, enabling this option selects the spinlock
# implementation variant using the ARMv8.1-LSE compare-and-swap instruction.
# Default: disabled
USE_SPINLOCK_CAS := 0

# Enable Link Time Optimization
ENABLE_LTO			:= 0

# Build flag to include EL2 registers in cpu context save and restore during
# S-EL2 firmware entry/exit. This flag is to be used with SPD=spmd option.
# Default is 0.
CTX_INCLUDE_EL2_REGS		:= 0

# Enable Memory tag extension which is supported for architecture greater
# than Armv8.5-A
# By default it is set to "no"
SUPPORT_STACK_MEMTAG		:= no
