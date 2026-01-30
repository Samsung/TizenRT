list(APPEND incs rtc)

if (CONFIG_AON_RTC)
	if (CONFIG_AON_RTC_64BIT)
		list(APPEND srcs rtc/aon_rtc_driver_64bit.c)
	else()
		list(APPEND srcs rtc/aon_rtc_driver.c)
	endif()
elseif (CONFIG_ANA_RTC)
	list(APPEND srcs rtc/ana_rtc_driver.c)
endif()

set(incs ${incs} PARENT_SCOPE)
set(srcs ${srcs} PARENT_SCOPE)
