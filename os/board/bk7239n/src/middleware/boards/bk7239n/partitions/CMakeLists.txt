armino_component_register(INCLUDE_DIRS ./)

if (CMAKE_BUILD_EARLY_EXPANSION)
return()
endif()

set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/_build)
armino_build_set_property(PARTITIONS_HDR_DIR ${GEN_DIR})
set(GEN_FILE ${GEN_DIR}/partitions_gen.h)
set(CONFIG_FILE_LISTS
	../csv/partitions.csv
	../csv/ota.csv
	../csv/security.csv
	../csv/ppc.csv
	../csv/mpc.csv
	)

armino_build_get_property(python PYTHON)
set(GEN_TOOL $ENV{ARMINO_PATH}/middleware/boards/${ARMINO_SOC}/bk7239.wrapper)

add_custom_command(OUTPUT ${GEN_FILE}
	COMMAND ${python} ${GEN_TOOL} pack --prebuild
	DEPENDS ${CONFIG_FILE_LISTS} ${GEN_TOOL}
)

add_custom_target(generated_files SOURCES ${GEN_FILE})
add_dependencies(${COMPONENT_LIB} generated_files)
target_include_directories(${COMPONENT_LIB} INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/_build)

