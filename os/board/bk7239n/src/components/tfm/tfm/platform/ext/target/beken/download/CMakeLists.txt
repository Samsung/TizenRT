if (BL2_DOWNLOAD)
	add_library(download
		bl_uart.c
		boot_protocol.c
		driver_uart0.c
		bootloader.c
		crc32.c
		system.c
		boot_update.c
	)

	string(REGEX REPLACE "beken/" "" SOC "${TFM_PLATFORM}")
	set(download_inc
		.
		./inc
		./inc/pal
		./hal/inc
		${TFM_BEKEN}/bk7239n/armino/driver/hal
		${TFM_BEKEN}/bk7239n/armino/driver
		../${TFM_SOC}/cmsis_core
		${TFM_BEKEN}/retarget
		${TFM_BEKEN}/partition
		../armino/include
		../${TFM_SOC}/armino/config
		./../../../cmsis/
		${ARMINO_MIDDLEWARE_REPO_PATH}/boards/bk7239n/partitions
		${PARTITION_HDR_DIR}
		./../../../../../bl2/ext/mcuboot/include/
	)

	target_compile_definitions(download
		PUBLIC
			CONFIG_BL2_DOWNLOAD
	)

	if(CONFIG_BL2_UPDATE_WITH_PC)
		target_compile_definitions(download
			PUBLIC
				CONFIG_BL2_UPDATE_WITH_PC
		)
	endif()

	target_compile_options(download PUBLIC ${BL2_COMPILER_CP_FLAG})
	target_include_directories(download PRIVATE ${download_inc})
	target_link_libraries(platform_bl2 PUBLIC download)
	link_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()


