;/*
; * Copyright (c) 2009-2022 Arm Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; *
; * This file is derivative of CMSIS V5.00 gcc_arm.ld
; */

/***********  WARNING: This is an auto-generated file. Do not edit!  ***********/

/* Linker script to configure memory regions. */
/* This file will be run trough the pre-processor. */

#include "region_defs.h"

MEMORY
{
  FLASH    (rx)  : ORIGIN = S_CODE_START, LENGTH = S_CODE_SIZE
  RAM      (rwx) : ORIGIN = S_DATA_START, LENGTH = S_DATA_SIZE
#if defined(S_CODE_SRAM_ALIAS_BASE)
  CODE_RAM (rwx) : ORIGIN = S_CODE_SRAM_ALIAS_BASE, LENGTH = TOTAL_CODE_SRAM_SIZE
#endif
}

__heap_size__  = S_HEAP_SIZE;
__msp_init_stack_size__ = S_MSP_STACK_SIZE_INIT;

/* Library configurations */
GROUP(libgcc.a libc.a libm.a libnosys.a libc_nano.a)

ENTRY(Reset_Handler)

SECTIONS
{
    /* Start address of the code. */
    Image$$PT_RO_START$$Base = ADDR(.TFM_VECTORS);

    .TFM_VECTORS : ALIGN(4)
    {
        __vectors_start__ = .;
        KEEP(*(.vectors))
        __vectors_end__ = .;
        . = ALIGN(4);
    } > FLASH

#ifndef TFM_MULTI_CORE_TOPOLOGY
    ASSERT(. <= ADDR(.TFM_VECTORS) + S_CODE_VECTOR_TABLE_SIZE, ".TFM_VECTORS section size overflow.")
    . = ADDR(.TFM_VECTORS) + S_CODE_VECTOR_TABLE_SIZE;
    /*
     * Place the CMSE Veneers (containing the SG instruction) after the code, in
     * a separate 32 bytes aligned region so that the SAU can programmed to just
     * set this region as Non-Secure Callable.
     */
    .gnu.sgstubs ALIGN(32) : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > FLASH
    Image$$ER_VENEER$$Base = ADDR(.gnu.sgstubs);
    Image$$VENEER_ALIGN$$Limit = ADDR(.gnu.sgstubs) + SIZEOF(.gnu.sgstubs);
#endif

    .copy.table : ALIGN(4)
    {
        __copy_table_start__ = .;
        LONG (LOADADDR(.TFM_DATA))
        LONG (ADDR(.TFM_DATA))
        LONG (SIZEOF(.TFM_DATA))
        LONG (LOADADDR(.ER_TFM_SP_PS_RWZI))
        LONG (ADDR(.ER_TFM_SP_PS_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_PS_RWZI))
        LONG (LOADADDR(.ER_TFM_SP_ITS_RWZI))
        LONG (ADDR(.ER_TFM_SP_ITS_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_ITS_RWZI))
        LONG (LOADADDR(.ER_TFM_SP_CRYPTO_RWZI))
        LONG (ADDR(.ER_TFM_SP_CRYPTO_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_CRYPTO_RWZI))
        LONG (LOADADDR(.ER_TFM_SP_PLATFORM_RWZI))
        LONG (ADDR(.ER_TFM_SP_PLATFORM_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_PLATFORM_RWZI))
        LONG (LOADADDR(.ER_TFM_SP_INITIAL_ATTESTATION_RWZI))
        LONG (ADDR(.ER_TFM_SP_INITIAL_ATTESTATION_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_INITIAL_ATTESTATION_RWZI))
        LONG (LOADADDR(.ER_TFM_SP_FWU_RWZI))
        LONG (ADDR(.ER_TFM_SP_FWU_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_FWU_RWZI))
        LONG (LOADADDR(.ER_TFM_SP_SYSTEM_CLOCK_RWZI))
        LONG (ADDR(.ER_TFM_SP_SYSTEM_CLOCK_RWZI))
        LONG (SIZEOF(.ER_TFM_SP_SYSTEM_CLOCK_RWZI))
#if defined(S_CODE_SRAM_ALIAS_BASE)
        LONG (LOADADDR(.ER_EFLASH_DRIVER_RO))
        LONG (ADDR(.ER_EFLASH_DRIVER_RO))
        LONG (SIZEOF(.ER_EFLASH_DRIVER_RO))
#endif
        __copy_table_end__ = .;
    } > FLASH

    .zero.table : ALIGN(4)
    {
        __zero_table_start__ = .;
#if defined(CONFIG_TFM_PARTITION_META)
        LONG (ADDR(.TFM_SP_META_PTR))
        LONG (SIZEOF(.TFM_SP_META_PTR))
#endif
        LONG (ADDR(.TFM_BSS))
        LONG (SIZEOF(.TFM_BSS))
        LONG (ADDR(.TFM_SP_PS_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_PS_RWZI_BSS))
        LONG (ADDR(.TFM_SP_ITS_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_ITS_RWZI_BSS))
        LONG (ADDR(.TFM_SP_CRYPTO_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_CRYPTO_RWZI_BSS))
        LONG (ADDR(.TFM_SP_PLATFORM_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_PLATFORM_RWZI_BSS))
        LONG (ADDR(.TFM_SP_INITIAL_ATTESTATION_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_INITIAL_ATTESTATION_RWZI_BSS))
        LONG (ADDR(.TFM_SP_FWU_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_FWU_RWZI_BSS))
        LONG (ADDR(.TFM_SP_SYSTEM_CLOCK_RWZI_BSS))
        LONG (SIZEOF(.TFM_SP_SYSTEM_CLOCK_RWZI_BSS))
        __zero_table_end__ = .;
    } > FLASH

    /**** Section for holding partition RO load data */
    .TFM_SP_LOAD_LIST : ALIGN(4)
    {
       KEEP(*(.part_load))
    } > FLASH
    Image$$TFM_SP_LOAD_LIST$$RO$$Base = ADDR(.TFM_SP_LOAD_LIST);
    Image$$TFM_SP_LOAD_LIST$$RO$$Limit = ADDR(.TFM_SP_LOAD_LIST) + SIZEOF(.TFM_SP_LOAD_LIST);
    . = ALIGN(32);

    /**** PSA RoT RO CODE + RO-data starts here */
    .TFM_SP_ITS_RO : ALIGN(32)
    {
        *tfm_*partition_its.*:*(.text*)
        *tfm_*partition_its.*:*(.rodata*)
        *(TFM_SP_ITS_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_ITS_RO$$Base = ADDR(.TFM_SP_ITS_RO);
    Image$$TFM_SP_ITS_RO$$Limit = ADDR(.TFM_SP_ITS_RO) + SIZEOF(.TFM_SP_ITS_RO);

    .TFM_SP_CRYPTO_RO : ALIGN(32)
    {
        *tfm_*partition_crypto.*:*(.text*)
        *tfm_*partition_crypto.*:*(.rodata*)
        *(TFM_SP_CRYPTO_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_CRYPTO_RO$$Base = ADDR(.TFM_SP_CRYPTO_RO);
    Image$$TFM_SP_CRYPTO_RO$$Limit = ADDR(.TFM_SP_CRYPTO_RO) + SIZEOF(.TFM_SP_CRYPTO_RO);

    .TFM_SP_PLATFORM_RO : ALIGN(32)
    {
        *tfm_*partition_platform.*:*(.text*)
        *tfm_*partition_platform.*:*(.rodata*)
        *(TFM_SP_PLATFORM_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_PLATFORM_RO$$Base = ADDR(.TFM_SP_PLATFORM_RO);
    Image$$TFM_SP_PLATFORM_RO$$Limit = ADDR(.TFM_SP_PLATFORM_RO) + SIZEOF(.TFM_SP_PLATFORM_RO);

    .TFM_SP_INITIAL_ATTESTATION_RO : ALIGN(32)
    {
        *tfm_*partition_attestation.*:*(.text*)
        *tfm_*partition_attestation.*:*(.rodata*)
        *(TFM_SP_INITIAL_ATTESTATION_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_INITIAL_ATTESTATION_RO$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION_RO);
    Image$$TFM_SP_INITIAL_ATTESTATION_RO$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION_RO) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION_RO);

    .TFM_SP_FWU_RO : ALIGN(32)
    {
        *tfm_*partition_fwu*:*(.text*)
        *tfm_*partition_fwu*:*(.rodata*)
        *(TFM_SP_FWU_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_FWU_RO$$Base = ADDR(.TFM_SP_FWU_RO);
    Image$$TFM_SP_FWU_RO$$Limit = ADDR(.TFM_SP_FWU_RO) + SIZEOF(.TFM_SP_FWU_RO);

    .TFM_SP_SYSTEM_CLOCK_RO : ALIGN(32)
    {
        *tfm_*partition_system_clock*:*(.text*)
        *tfm_*partition_system_clock*:*(.rodata*)
        *(TFM_SP_SYSTEM_CLOCK_PSA-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_SYSTEM_CLOCK_RO$$Base = ADDR(.TFM_SP_SYSTEM_CLOCK_RO);
    Image$$TFM_SP_SYSTEM_CLOCK_RO$$Limit = ADDR(.TFM_SP_SYSTEM_CLOCK_RO) + SIZEOF(.TFM_SP_SYSTEM_CLOCK_RO);

    /**** PSA RoT RO CODE + data ends here */

    /**** APPLICATION RoT RO CODE + data starts here */
    .TFM_SP_PS_RO : ALIGN(32)
    {
        *tfm_*partition_ps.*:*(.text*)
        *tfm_*partition_ps.*:*(.rodata*)
        *(TFM_SP_PS_APP-ROT_ATTR_FN)
        . = ALIGN(32);
    } > FLASH
    Image$$TFM_SP_PS_CODE$$Base = ADDR(.TFM_SP_PS_RO);
    Image$$TFM_SP_PS_CODE$$Limit = ADDR(.TFM_SP_PS_RO) + SIZEOF(.TFM_SP_PS_RO);

    /**** APPLICATION RoT RO CODE + data ends here */

#if defined(S_CODE_SRAM_ALIAS_BASE)
    .ER_EFLASH_DRIVER_RO : ALIGN(4)
    {
        *Driver_GFC100_EFlash.o(.text*)
        *Driver_GFC100_EFlash.o(.rodata*)
        *gfc100_eflash_drv.o(.text*)
        *gfc100_eflash_drv.o(.rodata*)
        *musca_b1_eflash_drv.o(.text*)
        *musca_b1_eflash_drv.o(.rodata*)
        . = ALIGN(4); /* This alignment is needed to make the section size 4 bytes aligned */
    } > CODE_RAM AT > FLASH
    Image$$ER_EFLASH_DRIVER_RO$$Base = ADDR(.ER_EFLASH_DRIVER_RO);
    Image$$ER_EFLASH_DRIVER_RO$$Limit = ADDR(.ER_EFLASH_DRIVER_RO) + SIZEOF(.ER_EFLASH_DRIVER_RO);
#endif

    .ER_TFM_CODE : ALIGN(4)
    {
        *startup*(.text*)
        *libplatform_s*:*(.text*)
        *libtfm_spm*:*(.text*)

        *libplatform_s*:*(.rodata*)
        *libtfm_spm*:*(.rodata*)
    } > FLASH

    /**** Unprivileged Secure code starts here */
    .ER_UNPRIV_CODE : ALIGN(32)
    {
        *(SFN)
        *(.text*)

        KEEP(*(.init))
        KEEP(*(.fini))

        /* .ctors */
        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        /* .dtors */
         *crtbegin.o(.dtors)
         *crtbegin?.o(.dtors)
         *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
         *(SORT(.dtors.*))
         *(.dtors)

        *(.rodata*)

        KEEP(*(.eh_frame*))

        . = ALIGN(32);
    } > FLASH
    Image$$TFM_UNPRIV_CODE$$RO$$Base = ADDR(.ER_UNPRIV_CODE);
    Image$$TFM_UNPRIV_CODE$$RO$$Limit = ADDR(.ER_UNPRIV_CODE) + SIZEOF(.ER_UNPRIV_CODE);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_RO_END$$Base = .;

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    /**** Base address of secure data area */
    .tfm_secure_data_start :
    {
        . = ABSOLUTE(S_DATA_START) ;
    } > RAM

   /* Position tag */
    . = ALIGN(32);
    Image$$PT_PRIV_RWZI_START$$Base = .;

    /* shared_data and msp_stack are overlapping on purpose when
     * msp_stack is extended until the beginning of RAM, when shared_date
     * was read out by partitions
     */
    .tfm_bl2_shared_data : ALIGN(32)
    {
        . += BOOT_TFM_SHARED_DATA_SIZE;
    } > RAM

    .msp_stack : ALIGN(32)
    {
        . += __msp_init_stack_size__;
    } > RAM
    Image$$ARM_LIB_STACK$$ZI$$Base = ADDR(.msp_stack);
    Image$$ARM_LIB_STACK$$ZI$$Limit = ADDR(.msp_stack) + SIZEOF(.msp_stack);

    /**** PSA RoT DATA start here */

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_ITS_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_ITS_RWZI : ALIGN(32)
    {
        *tfm_*partition_its.*:*(.data*)
        *(TFM_SP_ITS_PSA-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$ER_TFM_SP_ITS_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_ITS_RWZI);
    Image$$ER_TFM_SP_ITS_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_ITS_RWZI) + SIZEOF(.ER_TFM_SP_ITS_RWZI);

    .TFM_SP_ITS_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_ITS_RWZI = .;
        *tfm_*partition_its.*:*(.bss*)
        *tfm_*partition_its.*:*(COMMON)
        *(TFM_SP_ITS_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_ITS_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_ITS_RWZI$$ZI$$Base = ADDR(.TFM_SP_ITS_RWZI_BSS);
    Image$$ER_TFM_SP_ITS_RWZI$$ZI$$Limit = ADDR(.TFM_SP_ITS_RWZI_BSS) + SIZEOF(.TFM_SP_ITS_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_ITS_PRIVATE_DATA_END$$Base = .;


    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_CRYPTO_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_CRYPTO_RWZI : ALIGN(32)
    {
        *tfm_*partition_crypto.*:*(.data*)
        *(TFM_SP_CRYPTO_PSA-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$ER_TFM_SP_CRYPTO_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_CRYPTO_RWZI);
    Image$$ER_TFM_SP_CRYPTO_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_CRYPTO_RWZI) + SIZEOF(.ER_TFM_SP_CRYPTO_RWZI);

    .TFM_SP_CRYPTO_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_CRYPTO_RWZI = .;
        *tfm_*partition_crypto.*:*(.bss*)
        *tfm_*partition_crypto.*:*(COMMON)
        *(TFM_SP_CRYPTO_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_CRYPTO_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_CRYPTO_RWZI$$ZI$$Base = ADDR(.TFM_SP_CRYPTO_RWZI_BSS);
    Image$$ER_TFM_SP_CRYPTO_RWZI$$ZI$$Limit = ADDR(.TFM_SP_CRYPTO_RWZI_BSS) + SIZEOF(.TFM_SP_CRYPTO_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_CRYPTO_PRIVATE_DATA_END$$Base = .;


    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_PLATFORM_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_PLATFORM_RWZI : ALIGN(32)
    {
        *tfm_*partition_platform.*:*(.data*)
        *(TFM_SP_PLATFORM_PSA-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$ER_TFM_SP_PLATFORM_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_PLATFORM_RWZI);
    Image$$ER_TFM_SP_PLATFORM_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_PLATFORM_RWZI) + SIZEOF(.ER_TFM_SP_PLATFORM_RWZI);

    .TFM_SP_PLATFORM_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_PLATFORM_RWZI = .;
        *tfm_*partition_platform.*:*(.bss*)
        *tfm_*partition_platform.*:*(COMMON)
        *(TFM_SP_PLATFORM_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_PLATFORM_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_PLATFORM_RWZI$$ZI$$Base = ADDR(.TFM_SP_PLATFORM_RWZI_BSS);
    Image$$ER_TFM_SP_PLATFORM_RWZI$$ZI$$Limit = ADDR(.TFM_SP_PLATFORM_RWZI_BSS) + SIZEOF(.TFM_SP_PLATFORM_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_PLATFORM_PRIVATE_DATA_END$$Base = .;


    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_INITIAL_ATTESTATION_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_INITIAL_ATTESTATION_RWZI : ALIGN(32)
    {
        *tfm_*partition_attestation.*:*(.data*)
        *(TFM_SP_INITIAL_ATTESTATION_PSA-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$ER_TFM_SP_INITIAL_ATTESTATION_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_INITIAL_ATTESTATION_RWZI);
    Image$$ER_TFM_SP_INITIAL_ATTESTATION_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_INITIAL_ATTESTATION_RWZI) + SIZEOF(.ER_TFM_SP_INITIAL_ATTESTATION_RWZI);

    .TFM_SP_INITIAL_ATTESTATION_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_INITIAL_ATTESTATION_RWZI = .;
        *tfm_*partition_attestation.*:*(.bss*)
        *tfm_*partition_attestation.*:*(COMMON)
        *(TFM_SP_INITIAL_ATTESTATION_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_INITIAL_ATTESTATION_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_INITIAL_ATTESTATION_RWZI$$ZI$$Base = ADDR(.TFM_SP_INITIAL_ATTESTATION_RWZI_BSS);
    Image$$ER_TFM_SP_INITIAL_ATTESTATION_RWZI$$ZI$$Limit = ADDR(.TFM_SP_INITIAL_ATTESTATION_RWZI_BSS) + SIZEOF(.TFM_SP_INITIAL_ATTESTATION_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_INITIAL_ATTESTATION_PRIVATE_DATA_END$$Base = .;


    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_FWU_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_FWU_RWZI : ALIGN(32)
    {
        *tfm_*partition_fwu*:*(.data*)
        *(TFM_SP_FWU_PSA-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$ER_TFM_SP_FWU_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_FWU_RWZI);
    Image$$ER_TFM_SP_FWU_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_FWU_RWZI) + SIZEOF(.ER_TFM_SP_FWU_RWZI);

    .TFM_SP_FWU_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_FWU_RWZI = .;
        *tfm_*partition_fwu*:*(.bss*)
        *tfm_*partition_fwu*:*(COMMON)
        *(TFM_SP_FWU_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_FWU_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_FWU_RWZI$$ZI$$Base = ADDR(.TFM_SP_FWU_RWZI_BSS);
    Image$$ER_TFM_SP_FWU_RWZI$$ZI$$Limit = ADDR(.TFM_SP_FWU_RWZI_BSS) + SIZEOF(.TFM_SP_FWU_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_FWU_PRIVATE_DATA_END$$Base = .;


    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_SYSTEM_CLOCK_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_SYSTEM_CLOCK_RWZI : ALIGN(32)
    {
        *tfm_*partition_system_clock*:*(.data*)
        *(TFM_SP_SYSTEM_CLOCK_PSA-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$ER_TFM_SP_SYSTEM_CLOCK_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_SYSTEM_CLOCK_RWZI);
    Image$$ER_TFM_SP_SYSTEM_CLOCK_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_SYSTEM_CLOCK_RWZI) + SIZEOF(.ER_TFM_SP_SYSTEM_CLOCK_RWZI);

    .TFM_SP_SYSTEM_CLOCK_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_SYSTEM_CLOCK_RWZI = .;
        *tfm_*partition_system_clock*:*(.bss*)
        *tfm_*partition_system_clock*:*(COMMON)
        *(TFM_SP_SYSTEM_CLOCK_PSA-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_SYSTEM_CLOCK_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_SYSTEM_CLOCK_RWZI$$ZI$$Base = ADDR(.TFM_SP_SYSTEM_CLOCK_RWZI_BSS);
    Image$$ER_TFM_SP_SYSTEM_CLOCK_RWZI$$ZI$$Limit = ADDR(.TFM_SP_SYSTEM_CLOCK_RWZI_BSS) + SIZEOF(.TFM_SP_SYSTEM_CLOCK_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_SYSTEM_CLOCK_PRIVATE_DATA_END$$Base = .;

    /**** PSA RoT DATA end here */
    . = ALIGN(32);

    /* Position tag */
    Image$$PT_PRIV_RWZI_END$$Base = .;

    /**** APPLICATION RoT DATA start here */
    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_PS_PRIVATE_DATA_START$$Base = .;

    .ER_TFM_SP_PS_RWZI : ALIGN(32)
    {
        *tfm_*partition_ps.*:*(.data*)
        *(TFM_SP_PS_APP-ROT_ATTR_RW)
        . = ALIGN(32);
    } > RAM AT> FLASH
    Image$$.ER_TFM_SP_PS_RWZI$$RW$$Base = ADDR(.ER_TFM_SP_PS_RWZI);
    Image$$.ER_TFM_SP_PS_RWZI$$RW$$Limit = ADDR(.ER_TFM_SP_PS_RWZI) + SIZEOF(.ER_TFM_SP_PS_RWZI);

    .TFM_SP_PS_RWZI_BSS : ALIGN(32)
    {
        start_of_TFM_SP_PS_RWZI = .;
        *tfm_*partition_ps.*:*(.bss*)
        *tfm_*partition_ps.*:*(COMMON)
        *(TFM_SP_PS_APP-ROT_ATTR_ZI)
        . += (. - start_of_TFM_SP_PS_RWZI) ? 0 : 4;
        . = ALIGN(32);
    } > RAM AT> RAM
    Image$$ER_TFM_SP_PS_RWZI$$ZI$$Base = ADDR(.TFM_SP_PS_RWZI_BSS);
    Image$$ER_TFM_SP_PS_RWZI$$ZI$$Limit = ADDR(.TFM_SP_PS_RWZI_BSS) + SIZEOF(.TFM_SP_PS_RWZI_BSS);

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_TFM_SP_PS_PRIVATE_DATA_END$$Base = .;

    /**** APPLICATION RoT DATA end here */

    /* Position tag */
    . = ALIGN(32);
    Image$$PT_APP_RWZI_END$$Base = .;

#ifdef TFM_PARTITION_TEST_SECURE_SERVICES
    .TFM_SP_SECURE_TEST_PARTITION_LINKER_DATA : ALIGN(32)
    {
        *libc_nano*:*(.data*)
        . = ALIGN(32);
    } > RAM AT> FLASH

    .TFM_SP_SECURE_TEST_PARTITION_LINKER_BSS : ALIGN(32)
    {
        /* FixMe:
         * Test framework use printf to print out test result. Implementation of
         * printf in GCC libc use static data and heap as well. To be able to
         * execute test suites with TFM_LVL=3 this workaround is needed to
         * allocate libc static data and heap within the data section of secure
         * test partition. This can be removed if test service will be executed
         * in privileged mode.
         */
        *libc_nano*:*(.bss*)
        *libc_nano*:*(COMMON)

        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */

        . = ALIGN(32);
    } > RAM AT> RAM
#else /* TFM_PARTITION_TEST_SECURE_SERVICES */
    .heap : ALIGN(8)
    {
        __end__ = .;
        PROVIDE(end = .);
        __HeapBase = .;
        . += __heap_size__;
        __HeapLimit = .;
        __heap_limit = .; /* Add for _sbrk */
    } > RAM AT> RAM
#endif /* TFM_PARTITION_TEST_SECURE_SERVICES */

    .TFM_DATA : ALIGN(4)
    {
        *(.data*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))
        . = ALIGN(4);

    } > RAM AT> FLASH
    Image$$ER_TFM_DATA$$RW$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$RW$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA);

#if defined(CONFIG_TFM_PARTITION_META)
    .TFM_SP_META_PTR : ALIGN(32)
    {
        *(.bss.SP_META_PTR_SPRTL_INST)
    } > RAM AT> RAM
    Image$$TFM_SP_META_PTR$$ZI$$Base = ADDR(.TFM_SP_META_PTR);
    Image$$TFM_SP_META_PTR$$ZI$$Limit = ADDR(.TFM_SP_META_PTR) + SIZEOF(.TFM_SP_META_PTR);
#endif

    .TFM_BSS : ALIGN(4)
    {
        __bss_start__ = .;
        __partition_runtime_start__ = .;
        KEEP(*(.bss.part_runtime))
        __partition_runtime_end__ = .;
        . = ALIGN(4);
        __service_runtime_start__ = .;
        KEEP(*(.bss.serv_runtime))
        __service_runtime_end__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM AT> RAM
    Image$$ER_TFM_DATA$$ZI$$Base = ADDR(.TFM_BSS);
    Image$$ER_TFM_DATA$$ZI$$Limit = ADDR(.TFM_BSS) + SIZEOF(.TFM_BSS);
    Image$$ER_PART_RT_POOL$$ZI$$Base = __partition_runtime_start__;
    Image$$ER_PART_RT_POOL$$ZI$$Limit = __partition_runtime_end__;
    Image$$ER_SERV_RT_POOL$$ZI$$Base = __service_runtime_start__;
    Image$$ER_SERV_RT_POOL$$ZI$$Limit = __service_runtime_end__;

    Image$$ER_TFM_DATA$$Base = ADDR(.TFM_DATA);
    Image$$ER_TFM_DATA$$Limit = ADDR(.TFM_DATA) + SIZEOF(.TFM_DATA) + SIZEOF(.TFM_BSS);

    Load$$LR$$LR_NS_PARTITION$$Base = NS_PARTITION_START;

#ifdef BL2
    Load$$LR$$LR_SECONDARY_PARTITION$$Base = SECONDARY_PARTITION_START;
#endif /* BL2 */

    PROVIDE(__stack = Image$$ARM_LIB_STACK$$ZI$$Limit);
}
