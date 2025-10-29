#-------------------------------------------------------------------------------
# Copyright (c) 2020-2022, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

cmake_policy(SET CMP0076 NEW)

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

#========================= Platform region defs ===============================#
string(REGEX REPLACE "beken/" "" SOC "${TFM_PLATFORM}")
target_include_directories(platform_region_defs
	INTERFACE
		partition
		${ARMINO_MIDDLEWARE_REPO_PATH}/boards/bk7239n/partitions
		${PARTITION_HDR_DIR}
)

#========================= Platform common defs ===============================#
# Specify the location of platform specific build dependencies.
target_sources(tfm_s
	PRIVATE
	$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7239_s.S>
)

# Use a common scatter file for Isolation L1 and L2, a dedicated one for Isolation L3
# IAR is not supported for L3
target_add_scatter_file(tfm_s
	$<$<AND:$<VERSION_LESS:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_SOURCE_DIR}/platform/ext/common/gcc/tfm_common_s.ld>
	$<$<AND:$<VERSION_EQUAL:${TFM_ISOLATION_LEVEL},3>,$<C_COMPILER_ID:GNU>>:${CMAKE_BINARY_DIR}/generated/platform/ext/common/gcc/tfm_isolation_l3.ld>
)

if(NS)
	target_sources(tfm_ns
		PRIVATE
			$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7239_ns.S>
	)
	target_add_scatter_file(tfm_ns
		$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/bk7239_ns.ld>
	)
	
	target_compile_definitions(tfm_ns
	PUBLIC
		CONFIG_TFM_NS
		DOMAIN_NS
		__EN_ICACHE
		__EN_DCACHE
	)
	
	target_link_libraries(CMSIS_5_tfm_ns
		INTERFACE
			CMSIS_5_RTX_V8MBN
	)
endif()

if(BL2)
	target_sources(bl2
		PRIVATE
			$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7239_bl2.c>
	)
	target_add_scatter_file(bl2
			$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/bk7239_bl2.ld>
	)
	target_sources(bl2_B
		PRIVATE
			$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/startup_cmsdk_bk7239_bl2.c>
	)
	target_add_scatter_file(bl2_B
			$<$<C_COMPILER_ID:GNU>:${CMAKE_CURRENT_SOURCE_DIR}/gcc/bk7239_bl2_B.ld>
	)
endif()

#========================= Platform Secure ====================================#
set(platform_s_inc
		.
		${TFM_BEKEN}/
		${TFM_BEKEN}/common
		${TFM_BEKEN}/cmsis_drivers
		${TFM_BEKEN}/native_drivers
		${TFM_BEKEN}/partition
		partition
		${ARMINO_MIDDLEWARE_REPO_PATH}/boards/bk7239n/partitions
		${PARTITION_HDR_DIR}
		${TFM_BEKEN}/retarget
		cmsis_core
		${TFM_BEKEN}/native_drivers/timer_cmsdk
		${PLATFORM_DIR}/../bl2/ext/mcuboot/include
		${PLATFORM_DIR}/..
        	${PLATFORM_DIR}/ext/target/beken/armino/include/driver
)

set(platform_s_src
		${TFM_BEKEN}/bl2/flash_map.c
		${TFM_BEKEN}/partition/flash_partition.c
		${TFM_BEKEN}/cmsis_drivers/Driver_Flash.c
		${TFM_BEKEN}/cmsis_drivers/Driver_USART.c
		cmsis_core/system_cmsdk_bk7239.c
		cmsis_core/mpu.c
		cmsis_core/cache.c
		${TFM_BEKEN}/native_drivers/mpu_armv8m_drv.c
		${TFM_BEKEN}/spm_hal.c
		$<$<BOOL:${TFM_PSA_API}>:${TFM_BEKEN}/tfm_interrupts.c>
		${TFM_BEKEN}/tfm_hal_isolation.c
		${TFM_BEKEN}/tfm_hal_platform.c
		${TFM_BEKEN}/target_cfg.c
		${TFM_BEKEN}/native_drivers/arm_uart_drv.c
		${TFM_BEKEN}/nv_otp/rotpk.c
		${TFM_BEKEN}/nv_otp/otp_flash.c
		$<$<AND:$<NOT:$<BOOL:${TEST_NS_SLIH_IRQ}>>,$<NOT:$<BOOL:${TEST_NS_FLIH_IRQ}>>>:${CMAKE_CURRENT_SOURCE_DIR}/../native_drivers/timer_cmsdk/timer_cmsdk.c>
		${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_nvic.c
		${CMAKE_SOURCE_DIR}/platform/ext/common/tfm_hal_isolation_mpu_v8m.c
		$<$<OR:$<BOOL:${TFM_S_REG_TEST}>,$<BOOL:${TFM_NS_REG_TEST}>>:${TFM_BEKEN}/plat_test.c>
		$<$<BOOL:${TFM_PARTITION_PLATFORM}>:${TFM_BEKEN}/services/src/tfm_platform_system.c>
)

target_include_directories(platform_s PUBLIC ${platform_s_inc})
target_sources(platform_s PRIVATE ${platform_s_src})
target_compile_definitions(platform_s
	PRIVATE
	__EN_ICACHE=1
	__EN_DCACHE=1
)

target_sources(tfm_sprt
	PRIVATE
		# SLIH test Partition and FLIH test Partition access the timer as ARoT Partitions.
		# Put the driver to SPRT so that both SLIH and FLIH tests can access it.
		$<$<OR:$<BOOL:${TEST_NS_SLIH_IRQ}>,$<BOOL:${TEST_NS_FLIH_IRQ}>>:${CMAKE_CURRENT_SOURCE_DIR}/../native_drivers/timer_cmsdk/timer_cmsdk.c>
)

target_compile_options(platform_s
	PUBLIC
		${COMPILER_CMSE_FLAG}
)

target_compile_definitions(platform_s
	PUBLIC
		-DCONFIG_FLASH_FAST_ERASE=1
)
	
#========================= Platform Non-Secure ================================#
target_sources(platform_ns
	PRIVATE
		${TFM_BEKEN}/native_drivers/arm_uart_drv.c
		${TFM_BEKEN}/native_drivers/timer_cmsdk/timer_cmsdk.c
		${TFM_BEKEN}/cmsis_drivers/Driver_USART.c
		cmsis_core/system_cmsdk_bk7239.c
		cmsis_core/mpu.c
		cmsis_core/cache.c
		$<$<BOOL:${TFM_PARTITION_PLATFORM}>:${TFM_BEKEN}/plat_test.c>
)

target_include_directories(platform_ns
	PUBLIC
		.
		${TFM_BEKEN}
		${TFM_BEKEN}/common
		${PLATFORM_DIR}/..
		${TFM_BEKEN}/retarget
		cmsis_core
		${TFM_BEKEN}/native_drivers
		${TFM_BEKEN}/native_drivers/timer_cmsdk
)

#========================= Platform BL2 =======================================#
if(BL2)
set(platform_bl2_src
		PRIVATE
			cmsis_core/system_cmsdk_bk7239.c
			cmsis_core/mpu.c
			cmsis_core/cache.c
			${TFM_BEKEN}/cmsis_drivers/Driver_Flash.c
			${TFM_BEKEN}/native_drivers/arm_uart_drv.c
			${TFM_BEKEN}/cmsis_drivers/Driver_USART.c
			${TFM_BEKEN}/bl2/boot_hal_bl2.c
			${TFM_BEKEN}/bl2/int_bl2.c
			${TFM_BEKEN}/bl2/hooks_bl2.c
			${TFM_BEKEN}/bl2/flash_map.c
			${TFM_BEKEN}/partition/flash_partition.c
			${TFM_BEKEN}/nv_otp/rotpk.c
			${TFM_BEKEN}/nv_otp/otp_flash.c
	)

	target_compile_definitions(platform_bl2
		PUBLIC
		-DMCUBOOT_IMAGE_ACCESS_HOOKS=1
		-DCRYPTO_HW_ACCELERATOR
		-DCONFIG_ENABLE_MCUBOOT_BL2=1
		-DCONFIG_FLASH_FAST_ERASE=1
		-DCONFIG_BL2_REG_BASE=1
		__EN_ICACHE
		__EN_DCACHE
	)

if (CONFIG_BL2_VALIDATE_ENABLED_BY_EFUSE)
	target_compile_definitions(platform_bl2 PUBLIC
		-DCONFIG_BL2_VALIDATE_ENABLED_BY_EFUSE)
endif()

	set(platform_bl2_inc
		PUBLIC
			partition
			${ARMINO_MIDDLEWARE_REPO_PATH}/boards/bk7239n/partitions
			${PARTITION_HDR_DIR}
			${TFM_BEKEN}
			${TFM_BEKEN}/partition
			cmsis_core
			${TFM_BEKEN}/retarget
			${TFM_BEKEN}/bl2

		PRIVATE
			.
			${PLATFORM_DIR}/..
			${TFM_BEKEN}/native_drivers
			${MCUBOOT_PATH}/boot/bootutil/include
			${MCUBOOT_PATH}/boot/bootutil/config

			${CMAKE_BINARY_DIR}/bl2/ext/mcuboot
			${PLATFORM_DIR}/../bl2/ext/mcuboot/include
			${PLATFORM_DIR}/ext/target/beken/common/hal/accelerator/dubhe_alt/inc
	)

	if(CRYPTO_HW_ACCELERATOR)
		list(APPEND platform_bl2_inc
			${TFM_BEKEN}/common/hal/inc/
			${TFM_BEKEN}/common/hal/accelerator/spe/inc/hal
			${TFM_BEKEN}/common/hal/accelerator/spe/inc/pal
			${TFM_BEKEN}/common/hal/accelerator/dubhe_driver/inc/crypto
			${TFM_BEKEN}/armino/driver/sys_ctrl/
			./armino/driver/include/
			${TFM_BEKEN}/armino/include/driver/
		)
	endif()

		list(APPEND platform_bl2_inc
			${TFM_BEKEN}/secure_dbg/device_agent/inc/
			${TFM_BEKEN}/secure_dbg/device_agent/src/
		)


		list(APPEND platform_bl2_src
			${TFM_BEKEN}/secure_dbg/device_agent/src/hal_jtag.c)

	if(BL2_SECURE_DEBUG)
		list(APPEND platform_bl2_src
			${TFM_BEKEN}/secure_dbg/device_agent/src/hal_comm_adapt.c
			${TFM_BEKEN}/secure_dbg/device_agent/src/hal_comm_channel.c
			${TFM_BEKEN}/secure_dbg/device_agent/src/hal_secure_debug.c
			${TFM_BEKEN}/secure_dbg/device_agent/src/securedebug.c
			${TFM_BEKEN}/secure_dbg/device_agent/src/securedebug_protocol.c
			${TFM_BEKEN}/secure_dbg/device_agent/src/secure_debug_app.c
		)

		target_compile_definitions(platform_bl2
			PUBLIC
				CONFIG_BL2_SECURE_DEBUG
		)
	endif()
	if(CONFIG_BL2_WDT)
		target_compile_definitions(platform_bl2
			PUBLIC
				CONFIG_BL2_WDT
		)
	endif()

	if(MCUBOOT_UPGRADE_STRATEGY STREQUAL OVERWRITE_ONLY)
		list(APPEND platform_bl2_src
			${TFM_BEKEN}/bl2/decompress_bl2
			${ARMINO_SDK_PATH}/components/compress/decompress.c
			${ARMINO_SDK_PATH}/components/compress/lzma.c
			${ARMINO_SDK_PATH}/components/compress/lzma1900/LzmaDec.c
			${TFM_BEKEN}/armino/driver/flash/flash_driver.c
		)

		list(APPEND platform_bl2_inc
			${ARMINO_SDK_PATH}/components/compress/include
			${ARMINO_SDK_PATH}/components/compress/lzma1900
			${TFM_BEKEN}/armino/driver/flash
		)
	endif()

	if(CONFIG_XIP_NO_VERSION)
		target_compile_definitions(platform_bl2
			PUBLIC
				CONFIG_XIP_NO_VERSION
		)
	endif()

    if(CONFIG_STARTUP_PERFORMANCE)
    target_compile_definitions(platform_s PUBLIC CONFIG_STARTUP_PERFORMANCE)
    target_compile_definitions(platform_ns PUBLIC CONFIG_STARTUP_PERFORMANCE)
    target_compile_definitions(platform_bl2 PUBLIC CONFIG_STARTUP_PERFORMANCE)
    target_compile_definitions(bootutil PUBLIC CONFIG_STARTUP_PERFORMANCE)
    endif()


	target_include_directories(platform_bl2 PUBLIC ${platform_bl2_inc})
	target_sources(platform_bl2 PRIVATE ${platform_bl2_src})
endif()

add_subdirectory(armino)
