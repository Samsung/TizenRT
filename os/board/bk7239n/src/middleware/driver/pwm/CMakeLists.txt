if (CONFIG_PWM)
	list(APPEND incs pwm)

	if (CONFIG_PWM_V2P2)
		list(APPEND incs pwm/v2p2)
		list(APPEND srcs pwm/v2p2/pwm_driver.c)
	endif()

	if (CONFIG_PWM_V1PX)
		list(APPEND incs pwm/v1px)
		list(APPEND srcs pwm/v1px/pwm_driver.c)
	endif()

	if (CONFIG_PWM_TEST)
		# list(APPEND srcs pwm/pwm_test.c)
	endif()

	if (CONFIG_PWM_API_TEST)
		list(APPEND srcs pwm/cli_pwm_api.c)
	endif()

	set(incs ${incs} PARENT_SCOPE)
	set(srcs ${srcs} PARENT_SCOPE)
endif()