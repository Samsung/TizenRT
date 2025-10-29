armino_build_get_property(target ARMINO_SOC)
set(incs include)

set(srcs src/pm.c)

if (CONFIG_PM_ENABLE)
	if (CONFIG_PM_CLIENT)
		list(APPEND srcs
			src/pm_client.c
		)
	else()
		list(APPEND srcs
			src/core/pm_core.c
			src/services/pm_power.c
			src/services/pm_sleep.c
			src/services/pm_clock.c
			src/services/pm_wakeup_source.c
			src/services/pm_debug.c
			src/services/pm_misc.c
		)
		if (CONFIG_PSRAM)
			list(APPEND srcs
				src/services/pm_psram.c
			)
		endif()
	endif()
	if (CONFIG_MAILBOX)
		list(APPEND srcs
			src/pm_mailbox.c
		)
	endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES "${priv_req}")
