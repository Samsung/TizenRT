armino_build_get_property(target ARMINO_SOC)
set(incs include)

set(srcs src/bk_pm.c)

if (CONFIG_PM_ENABLE)
	if (CONFIG_PM_CLIENT)
		list(APPEND srcs
			src/bk_pm_client.c
		)
	else()
		list(APPEND srcs
			src/core/bk_pm_core.c
			src/services/bk_pm_power.c
			src/services/bk_pm_sleep.c
			src/services/bk_pm_clock.c
			src/services/bk_pm_wakeup_source.c
			src/services/bk_pm_debug.c
			src/services/bk_pm_misc.c
			src/services/bk_pm_thread
			src/services/bk_pm_sleep_callback.c
		)
		if (CONFIG_PSRAM)
			list(APPEND srcs
				src/services/bk_pm_psram.c
			)
		endif()
	endif()
	if (CONFIG_MAILBOX)
		list(APPEND srcs
			src/bk_pm_mailbox.c
		)
	endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES "${priv_req}")
