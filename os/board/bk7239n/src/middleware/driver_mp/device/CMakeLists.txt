if (CONFIG_PM)
	list(APPEND srcs device/device.c)
	set(srcs ${srcs} PARENT_SCOPE)
endif()
