if (CONFIG_HW_SCALE)
	list(APPEND srcs hw_scale/hw_scale_driver.c)
	set(srcs ${srcs} PARENT_SCOPE)
endif()