

set(incs)


if(CONFIG_COMMON_IO)
	list(APPEND incs
				common_io/include
				common_io/test
	)
	list(APPEND srcs
				common_io/iot_uart.c
				common_io/iot_adc.c
				common_io/iot_spi.c
				common_io/iot_gpio.c
				common_io/iot_flash.c
				common_io/iot_timer.c
				common_io/iot_reset.c
				common_io/iot_power.c
	)

if(CONFIG_AON_RTC)
	list(APPEND srcs	common_io/iot_rtc.c)
endif()

if(CONFIG_PWM)
	list(APPEND srcs	common_io/iot_pwm.c)
endif()
if(CONFIG_EFUSE)
	list(APPEND srcs	common_io/iot_efuse.c)
endif()

if(CONFIG_TEMP_DETECT)
	list(APPEND srcs	common_io/iot_tsensor.c)
endif()

if(CONFIG_INT_WDT)
	list(APPEND srcs	common_io/iot_watchdog.c)
endif()

	list(APPEND srcs
				common_io/iot_test_common_io_internal.c
				common_io/test/test_iot_adc.c
				common_io/test/test_iot_gpio.c
				common_io/test/test_iot_i2c.c
				common_io/test/test_iot_pwm.c
				common_io/test/test_iot_uart.c
				common_io/test/test_iot_spi.c
				common_io/test/test_iot_flash.c
				common_io/test/test_iot_timer.c
				common_io/test/test_iot_tsensor.c
				common_io/test/test_iot_watchdog.c
				common_io/test/test_iot_reset.c
				common_io/test/test_iot_efuse.c
				common_io/test/test_iot_power.c
				common_io/test/all_tests.c
	)

if(CONFIG_AON_RTC)
	list(APPEND srcs
				common_io/test/test_iot_rtc.c
	)
endif()
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_common)
