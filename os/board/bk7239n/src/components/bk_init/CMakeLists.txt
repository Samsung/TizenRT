armino_build_get_property(target ARMINO_SOC)
set(incs include)
set(srcs
	bk_init.c
	components_init.c
	)


set(priv_req bk_common bk_netif bk_ate driver easy_flash bk_libs riscv tfm bk_bluetooth bk_wifi bk_phy psa_mbedtls)

if (("${target}" STREQUAL "bk7237") OR ("${target}" STREQUAL "bk7256") OR ("${target}" STREQUAL "bk7234") OR ("${target}" STREQUAL "bk7236") OR ("${target}" STREQUAL "bk7239") OR ("${target}" STREQUAL "bk7286") OR ("${target}" STREQUAL "bk7258") OR ("${target}" STREQUAL "bk723L"))
list(APPEND priv_req vnd_cal)
endif()

if (("${target}" STREQUAL "bk7235") OR ("${target}" STREQUAL "bk7235a") OR ("${target}" STREQUAL "bk7235b") OR ("${target}" STREQUAL "bk7235m"))
list(APPEND priv_req vnd_cal)
endif()

if (("${target}" STREQUAL "bk7236n"))
list(APPEND priv_req vnd_cal)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES "${priv_req}")
