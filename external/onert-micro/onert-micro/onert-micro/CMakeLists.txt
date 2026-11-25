set(OM_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(OM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(OM_PAL_COMMON_DIR "${OM_INCLUDE_DIR}/pal/common")
# if no path is specified, use classic to mcu
if (NOT OM_PAL_DIR)
    set(OM_PAL_DIR "${OM_INCLUDE_DIR}/pal/mcu")
endif()

# if no path is specified, use classic to KernelsToBuild list, else use generated one
if (NOT OM_KERNELS_BUILD_LIST)
    set(KERNEL_REGISTER_FILE "${OM_PAL_DIR}/KernelsToBuild.lst")
else()
    set(KERNEL_REGISTER_FILE ${OM_KERNELS_BUILD_LIST})
endif()

# if no path is specified, use classic to KernelsToBuild list, else use generated one
if (NOT OM_KERNELS_TRAIN_LIST)
    set(KERNEL_TRAIN_REGISTER_FILE "${OM_PAL_DIR}/KernelsToTrain.lst")
else()
    set(KERNEL_TRAIN_REGISTER_FILE ${OM_KERNELS_TRAIN_LIST})
endif()

set(PASS_REGISTER_FILE "${OM_INCLUDE_DIR}/optimize/BuildPass.lst")
set(CUSTOM_KERNEL_REGISTER_FILE "${OM_PAL_DIR}/CustomKernelsToBuild.lst")

# To build onert-micro for eval-driver x86-64 version (to avoid name conflicts)
if (NOT DEFINED CUSTOM_OM_SUFFIX)
    set(OM_SUFFIX "")
else()
    set(OM_SUFFIX ${CUSTOM_OM_SUFFIX})
endif()

# To disable quantize kernels
if (DIS_QUANT)
    add_definitions(-DDIS_QUANT)
endif()

# To disable float kernels
if (DIS_FLOAT)
    add_definitions(-DDIS_FLOAT)
endif()

# To enable memory estimate
if (OM_MEMORY_ESTIMATE)
    add_definitions(-DOM_MEMORY_ESTIMATE)
endif()

# To enable training part
if (ENABLE_TRAINING)
    add_definitions(-DENABLE_TRAINING)
endif()

# TODO move it to specific cmake for platforms
add_compile_options(-fno-exceptions)
add_compile_options(-Os)

# AFAIK, this will enable leak sanitizer, too
if(ENABLE_SANITIZER)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif(ENABLE_SANITIZER)

add_subdirectory(src)
