# set(incs freertos)

set(srcs
	# freertos/rtos_init.c
	system_main.c)

if(CONFIG_FREERTOS)
	set(incs freertos)
	list(APPEND srcs freertos/rtos_init.c)
elseif(CONFIG_LITEOS_M OR CONFIG_LITEOS_M_V3)
	set(incs liteos_m)
	list(APPEND srcs liteos_m/rtos_init.c)
elseif(CONFIG_NON_OS)
	set(incs non_os)
	list(APPEND srcs non_os/rtos_init.c)
endif()

set(PRIV_REQUIRES_LIST bk_common riscv arm9 embeded_gcov)

armino_component_register(SRCS "${srcs}"
			INCLUDE_DIRS "${incs}"
			#REQUIRES easy_flash usb bk_init
			REQUIRES easy_flash bk_init
			PRIV_REQUIRES ${PRIV_REQUIRES_LIST})
