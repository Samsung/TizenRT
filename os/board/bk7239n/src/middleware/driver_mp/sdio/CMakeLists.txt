if (CONFIG_SDIO_SLAVE)
	list(APPEND incs sdio)

	if (CONFIG_SDIO_V2P0)
		list(APPEND incs sdio/v2p0)
		list(APPEND srcs
			sdio/v2p0/sdio_slave_driver.c
			sdio/v2p0/sdio_utils.c
			sdio/v2p0/sdio_test.c
		)
	endif()

	if (CONFIG_SDIO_V1P0)
		list(APPEND incs sdio/v1p0)
		list(APPEND srcs
			sdio/v1p0/sdio.c
			sdio/v1p0/sdma.c
			sdio/v1p0/sutil.c
		)
	endif()

	if (CONFIG_SDIO_TEST_EN)
		list(APPEND srcs sdio/sdio_slave_test.c)
	endif()

	if (CONFIG_SDIO_V2P1)
		list(APPEND incs
			"sdio/v2p1"
		)

		list(APPEND srcs
			"sdio/v2p1/sdio_slave_driver.c"
			"sdio/v2p1/sdio_utils.c"
			"sdio/v2p1/sdio_test.c"
			)
	endif() #if (CONFIG_SDIO_V2P1)

	if (CONFIG_SDIO_V2P2)
		list(APPEND incs
			"sdio/v2p2"
		)

		list(APPEND srcs
			"sdio/v2p2/sdio_slave_driver.c"
			"sdio/v2p2/sdio_utils.c"
			"sdio/v2p2/sdio_test.c"
			)
	endif() #if (CONFIG_SDIO_V2P2)

	set(incs ${incs} PARENT_SCOPE)
	set(srcs ${srcs} PARENT_SCOPE)
endif()
