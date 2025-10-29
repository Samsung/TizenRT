set(incs include)
set(srcs bk_feature.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	# REQUIRES freertos_v9 app bk_event bk_rtos driver soc lwip_intf_v2_0 lwip_intf_v2_1 hal soc bk_log bk_ps bk_init
	REQUIRES release bk_startup os_source app bk_event bk_rtos driver bk_log bk_system bk_ps bk_init
	)
