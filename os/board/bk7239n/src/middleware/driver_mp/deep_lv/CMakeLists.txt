if (CONFIG_DEEP_LV)
	list(APPEND incs deep_lv)

	list(APPEND srcs
		deep_lv/deep_lv.c
	)

	set(incs ${incs} PARENT_SCOPE)
	set(srcs ${srcs} PARENT_SCOPE)
endif()
