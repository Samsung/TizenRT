if (CONFIG_VND_CAL)

set(incs
	.
	include
	)
set(srcs
	vnd_cal.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_libs)
