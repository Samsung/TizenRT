#-------------------------------------------------------------------------------
# Copyright (c) 2020, Arm Limited. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
#-------------------------------------------------------------------------------

target_include_directories(${CC312_LIB_TARGET}
    PUBLIC
        src/cc3x_lib
        src/cc3x_productionlib/common
        src/cc_mng
        src/hal
)

target_sources(${CC312_LIB_TARGET}
    PRIVATE
        src/cc3x_lib/cc_lib.c
        src/cc3x_lib/cc_rng_plat.c
        src/cc3x_lib/cc_util_cmac.c
        src/cc3x_lib/mbedtls_cc_util_asset_prov.c
        src/cc3x_productionlib/cmpu/cmpu_llf_rnd.c
        src/cc_mng/mbedtls_cc_mng.c
        src/cc_mng/mbedtls_cc_mng_int.c
        src/hal/cc3x/cc_hal.c
        src/utils/mbedtls_cc_util_key_derivation.c
)

########################## PAL #################################################

target_include_directories(${CC312_PAL_TARGET}
    PUBLIC
        src/hal
)

target_sources(${CC312_PAL_TARGET}
    PRIVATE
        src/pal/cc_pal_trng.c
        src/pal/no_os/cc_pal_abort_plat.c
        src/pal/no_os/cc_pal_apbc.c
        src/pal/no_os/cc_pal.c
        src/pal/no_os/cc_pal_dma.c
        src/pal/no_os/cc_pal_interrupt_ctrl.c
        src/pal/no_os/cc_pal_mem.c
        src/pal/no_os/cc_pal_memmap.c
        src/pal/no_os/cc_pal_mutex.c
        src/pal/no_os/cc_pal_pm.c
)

########################## CDMPU ###############################################

target_include_directories(${CC312_CDMPU_TARGET}
    INTERFACE
        src/cc3x_productionlib/dmpu
        src/cc3x_productionlib/cmpu
        src/cc3x_productionlib/common
)

target_sources(${CC312_CDMPU_TARGET}
    INTERFACE
        src/cc3x_lib/cc_rng_plat.c
        src/cc3x_productionlib/cmpu/cmpu.c
        src/cc3x_productionlib/cmpu/cmpu_derivation.c
        src/cc3x_productionlib/cmpu/cmpu_llf_rnd.c
        src/cc3x_productionlib/common/prod_crypto_driver.c
        src/cc3x_productionlib/common/prod_util.c
        src/cc3x_productionlib/dmpu/dmpu.c
        src/cc_mng/mbedtls_cc_mng.c
        src/cc_mng/mbedtls_cc_mng_int.c
        src/hal/cc3x/cc_hal.c
)
