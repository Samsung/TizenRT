armino_build_get_property(target ARMINO_SOC)

set(incs)
set(srcs)

if("${CONFIG_SOC_ARCH}" STREQUAL ${COMPONENT_NAME})
	list(APPEND incs
		include
	)

	list(APPEND srcs
		stack_base.c
		trap_base.c
		arm.c
		arch_interrupt.c
		jump.c
		arch_delay.c
	)

if(CONFIG_MPU)
	list(APPEND srcs
		mpu.c
	)
endif()

if(CONFIG_CACHE_ENABLE)
	list(APPEND srcs
		cache.c
	)
endif()

if(CONFIG_FREERTOS)
	list(APPEND incs os/freertos)
	list(APPEND srcs os/freertos/cpu.c)
elseif(CONFIG_LITEOS_M)
	list(APPEND incs os/liteos_m)
	list(APPEND srcs os/liteos_m/boot_handlers.S os/liteos_m/cpu.c)
elseif(CONFIG_NON_OS)
	list(APPEND incs os/non_os)
	list(APPEND srcs os/non_os/boot_handlers.S os/non_os/cpu.c)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES cmsis coredump os_source tfm)

