if (CONFIG_HW_ROTATE_PFC)
	list(APPEND srcs hw_rotate/rott_driver.c)
	set(srcs ${srcs} PARENT_SCOPE)
endif()