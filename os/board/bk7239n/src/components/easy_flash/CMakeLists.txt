if (CONFIG_EASY_FLASH)
	if(CONFIG_EASY_FLASH_V4)
		set(incs
			.
			easy_flash_V4.X/inc
			easy_flash_V4.X/port
			)
	endif()
else()
	set(incs
		.
		)
endif()

set(srcs)

if (CONFIG_EASY_FLASH)
	if(CONFIG_EASY_FLASH_V4)
	list(APPEND srcs
		bk_ef.c
		easy_flash_V4.X/src/easyflash.c
		easy_flash_V4.X/src/ef_env.c
		easy_flash_V4.X/src/ef_env_legacy_wl.c
		easy_flash_V4.X/src/ef_env_legacy.c
		easy_flash_V4.X/src/ef_iap.c
		easy_flash_V4.X/src/ef_log.c
		easy_flash_V4.X/src/ef_utils.c
		easy_flash_V4.X/port/ef_port.c
		)
	endif()
endif()

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common)
