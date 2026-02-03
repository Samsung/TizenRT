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
__MIN_HEAP_SIZE = (84 << 10);
PERSIST_MEM_AREA_SIZE = 4;
REBOOT_TAG_AREA_SIZE = 4;
__CPU0_APP_RAM_BASE = 0x38000000;
__CPU0_APP_RAM_SIZE = 0x80000 - __CPU0_SPE_RAM_SIZE;
__CPU0_APP_IRAM_OFFSET = 0x20000000;
__CPU0_APP_IRAM_BASE = __CPU0_APP_RAM_BASE - __CPU0_APP_IRAM_OFFSET;
__CPU0_APP_IRAM_SIZE = __CPU0_APP_RAM_SIZE;
__CPU0_APP_VIRTUAL_CODE_START = 0xD0000;
/* Set a large value here to ensure sufficient space during linking, actual size check is performed during bin packaging */
__CPU0_APP_VIRTUAL_CODE_SIZE = 0x600000;

SOC_FLASH_DATA_BASE = 0x12000000;
SOC_PSRAM_DATA_BASE = CONFIG_BK_PSRAM_DATA_BASE;
SOC_PSRAM_CODE_BASE = CONFIG_BK_PSRAM_CODE_BASE;

OUTPUT_ARCH(arm)
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)            : ORIGIN = (SOC_FLASH_DATA_BASE + __CPU0_APP_VIRTUAL_CODE_START), LENGTH = __CPU0_APP_VIRTUAL_CODE_SIZE
  IRAM (rx)             : ORIGIN = __CPU0_APP_IRAM_BASE, LENGTH = __CPU0_APP_IRAM_SIZE
  RAM (rwx)             : ORIGIN = __CPU0_APP_RAM_BASE, LENGTH = __CPU0_APP_RAM_SIZE
  PSRAM_DATA (rwx)      : ORIGIN = SOC_PSRAM_DATA_BASE, LENGTH = CONFIG_BK_PSRAM_DATA_SIZE
  PSRAM_CODE (rwx)      : ORIGIN = SOC_PSRAM_CODE_BASE, LENGTH = CONFIG_BK_PSRAM_CODE_SIZE
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

        EXCLUDE_FILE(
            *armino_irq.o* 
            *up_exception.o* 
            *up_svcall.o* 
            *up_switchcontext.o*
            *up_copyfullstate.o*
            *up_fullcontextrestore.o*
            *up_doirq.o*
            *irq_dispatch.o*
            *sched_addreadytorun.o*
            *sched_addblocked.o*
            *sched_addprioritized.o*
            *sched_removereadytorun.o*
            *sched_removeblocked.o*
            *sched_mergepending.o*
            *up_blocktask.o*
            *up_unblocktask.o*
            *libkernel.a:mq_send.o*
            *libkernel.a:mq_timedsend.o*
            *libkernel.a:mq_sndinternal.o*
            *libkernel.a:mq_receive.o*
            *libkernel.a:mq_timedreceive.o*
            *libkernel.a:mq_rcvinternal.o*
            *libkernel.a:mq_initialize.o*
            *libkernel.a:mq_msgfree.o*
            *libkernel.a:mq_getattr.o*
            *libkernel.a:mq_waitirq.o*
            *libkernel.a:mq_recover.o*
            *libkernel.a:mq_release.o*
            *libkernel.a:wd_start.o*
            *libkernel.a:wd_create.o*
            *libkernel.a:wd_cancel.o*
            *libkernel.a:timer_settime.o*
            ) *(.text .text.* .rodata .rodata.*)
        /**(.text.* *.rodata *.rodata.*)*/

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
        *armino_irq.o*                   (.text .text.* .rodata .rodata.*)
        *up_exception.o*                 (.text .text.* .rodata .rodata.*)
        *up_svcall.o*                    (.text .text.* .rodata .rodata.*)
        *up_switchcontext.o*             (.text .text.* .rodata .rodata.*)
        *up_copyfullstate.o*             (.text .text.* .rodata .rodata.*)
        *up_fullcontextrestore.o*        (.text .text.* .rodata .rodata.*)
        *up_doirq.o*                     (.text .text.* .rodata .rodata.*)
        *irq_dispatch.o*                 (.text .text.* .rodata .rodata.*)
        *sched_addreadytorun.o*          (.text .text.* .rodata .rodata.*)
        *sched_addblocked.o*             (.text .text.* .rodata .rodata.*)
        *sched_addprioritized.o*         (.text .text.* .rodata .rodata.*)
        *sched_removereadytorun.o*       (.text .text.* .rodata .rodata.*)
        *sched_removeblocked.o*          (.text .text.* .rodata .rodata.*)
        *sched_mergepending.o*           (.text .text.* .rodata .rodata.*)
        *up_blocktask.o*                 (.text .text.* .rodata .rodata.*)
        *up_unblocktask.o*               (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_send.o*          (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_timedsend.o*     (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_sndinternal.o*   (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_receive.o*       (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_timedreceive.o*  (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_rcvinternal.o*   (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_initialize.o*    (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_msgfree.o*       (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_getattr.o*       (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_waitirq.o*       (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_recover.o*       (.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_release.o*       (.text .text.* .rodata .rodata.*)
        *libkernel.a:wd_start.o*         (.text .text.* .rodata .rodata.*)
        *libkernel.a:wd_create.o*        (.text .text.* .rodata .rodata.*)
        *libkernel.a:wd_cancel.o*        (.text .text.* .rodata .rodata.*)
        *libkernel.a:timer_settime.o*    (.text .text.* .rodata .rodata.*)
        *(.itcm)
        *(.itcm_sec_code)
        *(.iram)
        *(.iram2)
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
        /*EXCLUDE_FILE(*libnet.a:mem.o*)  *(.bss .bss*)*/
        *(.bss .bss*)
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
        . = ALIGN(4);
        __psram_code_end__ = .;
    } > PSRAM_CODE AT > FLASH

    .psram_data :
    {
        . = ALIGN(8);
        PROVIDE(__psram_data_text = LOADADDR(.psram_data));
        __psram_data_start__ = .;
        *(.psram.data)
        *(.psram.data*)

        . = ALIGN(4);
        __psram_data_end__ = .;
    } > PSRAM_DATA AT > FLASH

    .psram_bss (NOLOAD):
    {
        . = ALIGN(8);
        __psram_bss_start__ = .;
        *(.psram.bss)
        *(.psram.bss*)

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
    ASSERT(((_heap_end - _heap_start) >= __MIN_HEAP_SIZE), "Heap smaller than minimize size 85K!!!")
}
