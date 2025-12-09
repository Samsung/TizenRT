/*
 * Copyright (C), 2018-2019, Arm Technology (China) Co., Ltd.
 * All rights reserved
 *
 * The content of this file or document is CONFIDENTIAL and PROPRIETARY
 * to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 * License Agreement between Licensee and Arm Technology (China) Co., Ltd
 * restricting among other things, the use, reproduction, distribution
 * and transfer.  Each of the embodiments, including this information and,,
 * any derivative work shall retain this copyright notice.
 */




OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

__MSP_STACK_SIZE = CONFIG_IDLETHREAD_STACKSIZE;  
__CPU0_SPE_RAM_SIZE = CONFIG_CPU0_SPE_RAM_SIZE;
__MIN_HEAP_SIZE = (90 << 10);
PERSIST_MEM_AREA_SIZE = 4;
REBOOT_TAG_AREA_SIZE = 4;
__CPU0_APP_RAM_BASE = 0x38000000 + __CPU0_SPE_RAM_SIZE;
__CPU0_APP_RAM_SIZE = 0x80000 - __CPU0_SPE_RAM_SIZE;
__CPU0_APP_IRAM_OFFSET = 0x20000000;
__CPU0_APP_IRAM_BASE = __CPU0_APP_RAM_BASE - __CPU0_APP_IRAM_OFFSET;
__CPU0_APP_IRAM_SIZE = __CPU0_APP_RAM_SIZE;
__CPU0_APP_VIRTUAL_CODE_START = 0xD0000;
__CPU0_APP_VIRTUAL_CODE_SIZE = 0x1a7860;

SOC_FLASH_DATA_BASE = 0x12000000;
SOC_PSRAM_DATA_BASE = 0x70000000;
SOC_PSRAM_CODE_BASE = 0x70080000;

OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)            : ORIGIN = (SOC_FLASH_DATA_BASE + __CPU0_APP_VIRTUAL_CODE_START), LENGTH = __CPU0_APP_VIRTUAL_CODE_SIZE
  IRAM (rx)             : ORIGIN = __CPU0_APP_IRAM_BASE, LENGTH = __CPU0_APP_IRAM_SIZE
  RAM (rwx)             : ORIGIN = __CPU0_APP_RAM_BASE, LENGTH = __CPU0_APP_RAM_SIZE
  PSRAM_DATA (rwx)      : ORIGIN = SOC_PSRAM_DATA_BASE, LENGTH = 0x80000
  PSRAM_CODE (rwx)      : ORIGIN = SOC_PSRAM_CODE_BASE, LENGTH = 0x80000
}

SECTIONS
{
    ASSERT((. == ALIGN(512)), "vector table address align fault.")
    .vectors :
    {
        __vector_table = .;
        KEEP(*(.vectors))
        *(.fix.reset_entry)
    } > FLASH

    .gnu.sgstubs ALIGN(32) : ALIGN(32)
    {
        *(.gnu.sgstubs*)
        . = ALIGN(32);
    } > FLASH


    .text :
    {
        . = ALIGN(4);
        _stext = .;
        _stext_flash = ABSOLUTE(.);

        /*EXCLUDE_FILE(*libboard.a:rwnx_rx.o*) *(.text .text.* .rodata .rodata.*)*/
        *(.text.* *.rodata *.rodata.*)

        . = ALIGN(4);
        __devconfig_start = .;
        *(".devconfig.*")
        KEEP(*(SORT_BY_NAME(".devconfig*")))
        __devconfig_end = .;

        . = ALIGN(4);
        __apps_start = .;
        KEEP (*(.apps_data))
        __apps_end = .;
        _etext_flash = ABSOLUTE(.);
        _etext = .;
        . = ALIGN(4);
    } > FLASH

    .a_device_null :
    {
      KEEP(*(.a_deviceobj_null))
    } > FLASH

    .a_devices :
    {
      __device_start = .;
      KEEP(*(.a_deviceobj_*))
      __device_end = .;
    } > FLASH

    .a_init_entries :
    {
      __a_init_start = .;
      KEEP(*(.a_init_entry_*))
      __a_init_end = .;
    } > FLASH

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    __exidx_end = .;

    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (__iram_flash_begin)
        LONG (__iram_start__)
        LONG ((__iram_end__ - __iram_start__) / 4)

        LONG (__data_flash_begin)
        LONG (__data_start__)
        LONG ((__data_end__ - __data_start__) / 4)

        __copy_table_end__ = .;
    } > FLASH

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (_bss_start)
        LONG ((_bss_end - _bss_start) / 4)
        LONG (_heap_start)
        LONG ((_heap_end - _heap_start) / 4)
        __zero_table_end__ = .;
    } > FLASH

    .iram :
    {
        . = ALIGN(512);
        PROVIDE(__iram_flash_begin = LOADADDR(.iram));
        __iram_start__ = .;
        _stext_ram = ABSOLUTE(.);

        . = ALIGN(512);
        __vector_iram_table = .;
        KEEP(*(.vectors_iram))
        . = ALIGN(512);

        *(.itcm)
        *(.itcm_sec_code)
        *(.iram)
        *(.interrupt)

        . = ALIGN(4);
        _etext_ram = ABSOLUTE(.);
        _iram_end_dummy_ = .;
        __iram_end__ = .;
    } > IRAM AT > FLASH

    .ramcode :
    {
        . = . +  SIZEOF(.iram);
        . = ALIGN(16);
    } > RAM AT > FLASH

    _eronly = ABSOLUTE(.);

    .data :
    {
        . = ALIGN(4);
        PROVIDE(__etext = LOADADDR(.data));
        PROVIDE(__data_flash_begin = LOADADDR(.data));

        _sdata = ABSOLUTE(.);
        __data_start__ = .;

        *(.dtcm)
        *(.dtcm_section*)
        *(.dtcm_sec_data*)
        *(.data)
        *(".data.*")
        *(.sdata)

        *(.gnu.linkonce.d*)
        __data_end__ = .;
        _edata = ABSOLUTE(.);
    } > RAM AT > FLASH

    _data_flash_begin = __data_flash_begin;
    _data_ram_begin = __data_start__;
    _data_ram_end = .;

    .uninitialized (NOLOAD):
    {
        . = ALIGN(32);
        __uninitialized_start = .;
        *(.uninitialized)
        *(".uninitialized.*")
        KEEP(*(.keep.uninitialized))
        . = ALIGN(32);
        __uninitialized_end = .;
    } > RAM

    .PERSIST_MEM : ALIGN(4)
    {
        __persist_mem__ = .;
        . += PERSIST_MEM_AREA_SIZE;
    } > RAM

    .REBOOT_TAG : ALIGN(4)
    {
        __reboot_tag__ = .;
        . += REBOOT_TAG_AREA_SIZE;
    } > RAM

    .bss (NOLOAD):
    {
        . = ALIGN(4);

       _sbss = ABSOLUTE(.);
        _bss_start = .;
        EXCLUDE_FILE(*libnet.a:mem.o*)  *(.bss .bss*)
        *(COMMON)

        . = ALIGN(8);
        _sidle_stack = ABSOLUTE(.);
        . = . + CONFIG_IDLETHREAD_STACKSIZE ;

        . = ALIGN(4);
        _bss_end = .;
        _ebss = ABSOLUTE(.);
    } > RAM

    bss_size = _bss_end - _bss_start;

    _msp_total_size = __MSP_STACK_SIZE;

    .heap (COPY) :
    {
        . = ALIGN(8);
        _heap_start = .;
        . = . + (ORIGIN(RAM) + LENGTH(RAM) - _msp_total_size - _heap_start - 8); /* 16 bytes for boundary protection */
        . = ALIGN(8);
        _heap_end = .;
    } > RAM

    .stack (ORIGIN(RAM) + LENGTH(RAM) - __MSP_STACK_SIZE) (COPY) :
    {
        . = ALIGN(8);
        /*_sstack = .; */
        __StackLimit = .;
        . = . + __MSP_STACK_SIZE;
        . = ALIGN(8);
        __StackTop = .;
        /*_estack = .; */
    } > RAM

/*#if CONFIG_PSRAM*/
    .psram_code :
    {
        . = ALIGN(8);
        PROVIDE(__psram_code_text = LOADADDR(.psram_code));
        __psram_code_start__ = .;
        /*
        *libboard.a:rwnx_rx.c.o*(.text .text.* .rodata .rodata.*)
        *libboard.a:rwnx_tx.c.o*(.text .text.* .rodata .rodata.*)
        *libboard.a:wifi_v2.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:macif_fhost.c.o*(.text .text.* .rodata .rodata.*)
        *libboard.a:rtos_pub.c.o*(.text .text.* .rodata .rodata.*)
        *libboard.a:bk_lwipif_tizenrt.c.o*(.text .text.* .rodata .rodata.*)
        *libboard.a:bk_ethernetif.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:me_strategy.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:rxu_cntrl.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:rc.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:mcc.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:hal_machw.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:txl_cntrl.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:rxl_hwdesc.c.o*(.text .text.* .rodata .rodata.*)
        *libwifi.a:phy_karst_bk7236.o*(.text .text.* .rodata .rodata.*)*/

        /**rwnx_rx.o*(.text .text.* .rodata .rodata.*)*/

        . = ALIGN(4);
        __psram_code_end__ = .;
    } > PSRAM_CODE AT > FLASH

    .psram_data :
    {
        . = ALIGN(8);
        PROVIDE(__psram_data_text = LOADADDR(.psram_data));
        __psram_data_start__ = .;

        /**rwnx_rx.o*(.data .data.*)*/

        . = ALIGN(4);
        __psram_data_end__ = .;
    } > PSRAM_DATA AT > FLASH

    .psram_bss (NOLOAD):
    {
        . = ALIGN(8);
        __psram_bss_start__ = .;

        /**rwnx_rx.o*(.bss .bss.*)*/
        *libnet.a:mem.o*(.bss .bss.*)

        . = ALIGN(4);
        __psram_bss_end__ = .;
    } > PSRAM_DATA

/*#endif*/

    s_rom_end = LOADADDR(.data) + SIZEOF(.data);

    /* Set stack top to end of RAM, and stack limit move down by
     * size of stack_dummy section */
    __MSPTop = ORIGIN(RAM) + LENGTH(RAM);
    __MSPLimit = __MSPTop - __MSP_STACK_SIZE;

    ASSERT((s_rom_end < ORIGIN(FLASH) + LENGTH(FLASH)), "ROM overflow!!!")
    ASSERT(((ORIGIN(RAM) + LENGTH(RAM)) > (_heap_start + __MSP_STACK_SIZE + 8)), "Stack overflowed with bss")
    ASSERT(((_heap_end - _heap_start) >= __MIN_HEAP_SIZE), "Heap smaller than minimize size 90K!!!")
}
