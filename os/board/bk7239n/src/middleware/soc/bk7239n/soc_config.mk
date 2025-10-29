SUPPORT_BOOTLOADER := true
ifeq ($(SUPPORT_BOOTLOADER),true)
	ARMINO_BOOTLOADER := $(ARMINO_DIR)/properties/modules/bootloader/aboot/arm_bootloader/
endif

ifeq ($(WIN32), 1)
	COMPILER_TOOLCHAIN_PATH := $(ARMINO_BASH_TOOLS_PATH)/gcc-arm-none-eabi-10.3-2021.10/bin
else
	COMPILER_TOOLCHAIN_PATH := /opt/gcc-arm-none-eabi-10.3-2021.10/bin
endif