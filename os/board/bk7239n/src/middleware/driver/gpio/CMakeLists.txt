if (CONFIG_GPIO)
	list(APPEND incs gpio)
	if (CONFIG_AON_GPIO)
		list(APPEND srcs gpio/gpio_driver_base.c)

		if (CONFIG_GPIO_API_TEST)
			list(APPEND srcs gpio/cli_gpio_api.c)
		endif()
	endif()

	if (CONFIG_ANA_GPIO)
		list(APPEND srcs gpio/ana_gpio_driver.c)
	endif()

	if (CONFIG_GPIO_CLOCK_PIN_SUPPORT)
		list(APPEND srcs gpio/gpio_clk_pin.c)
	endif()

	if (CONFIG_GPIO_TEST)
		list(APPEND srcs gpio/gpio_test.c)
	endif()

	set(incs ${incs} PARENT_SCOPE)
	set(srcs ${srcs} PARENT_SCOPE)
endif()