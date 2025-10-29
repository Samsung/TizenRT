if (CONFIG_SUPPORT_IO_MATRIX)
if (CONFIG_IO_MATRIX_VER1_0)
	list(APPEND incs io_matrix/v1.0)
	list(APPEND srcs io_matrix/v1.0/iomx_driver.c)

	if (CONFIG_IOMX_TEST)
		list(APPEND srcs io_matrix/v1.0/iomx_test.c)
	endif()
endif () # CONFIG_IO_MATRIX_VER1_0

if (CONFIG_IO_MATRIX_VER2_0)
	list(APPEND incs io_matrix/v2.0)
	list(APPEND srcs io_matrix/v2.0/io_matrix_driver.c)
	list(APPEND srcs io_matrix/v2.0/gpio_adapter.c)

	if (CONFIG_IOMX_TEST)
		list(APPEND srcs io_matrix/v2.0/io_matrix_test.c)
	endif()
endif () # CONFIG_IO_MATRIX_VER2_0
endif()

set(incs ${incs} PARENT_SCOPE)
set(srcs ${srcs} PARENT_SCOPE)
