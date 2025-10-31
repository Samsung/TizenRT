set(incs .)

	set(srcs bk_sensor.c temp_detect.c volt_detect.c)

armino_component_register(SRCS "${srcs}"
	INCLUDE_DIRS "${incs}"
	PRIV_REQUIRES bk_common
	)
