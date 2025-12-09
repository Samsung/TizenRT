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

__MSP_STACK_SIZE = (4 << 10);
__MIN_HEAP_SIZE  = (40 << 10);
PERSIST_MEM_AREA_SIZE = 4;
REBOOT_TAG_AREA_SIZE = 4;


__CPU0_APP_RAM_BASE = 0x28010000;

__CPU0_APP_RAM_SIZE = 0x70000;



/*****************************************************************************
The follow four shared memory address area mapping to the same physical memory
    0x08000000 ~ 0x080a0000  ---- instruction area, recommend put sram code
    0x18000000 ~ 0x180a0000  ---- instruction area, recommend put sram code
    0x28000000 ~ 0x280a0000  ---- data area, recommend put sram data
    0x38000000 ~ 0x380a0000  ---- data area, recommend put sram data
******************************************************************************/
__CPU0_APP_IRAM_OFFSET = 0x20000000;

__CPU0_APP_IRAM_BASE = __CPU0_APP_RAM_BASE - __CPU0_APP_IRAM_OFFSET;  /*0x08000000 = 0x28000000 - __CPU0_APP_IRAM_OFFSET*/
__CPU0_APP_IRAM_SIZE = __CPU0_APP_RAM_SIZE;

__CPU0_APP_VIRTUAL_CODE_START = 0x10020;
__CPU0_APP_VIRTUAL_CODE_SIZE = 0x200000;

SOC_FLASH_DATA_BASE = 0x02000000;
SOC_PSRAM_DATA_BASE = 0x60000000;
SOC_PSRAM_CODE_BASE = 0x60080000;


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
        EXCLUDE_FILE(
            *armino_irq.o* 
            *up_exception.o* 
            *up_svcall.o* 
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
            ) *(.text .text.* .rodata .rodata.*)
        /*EXCLUDE_FILE( *libboard.a:rtos_pub.c.obj* *libboard.a:mem_arch.c.obj* *libboard.a:flash_driver.c.obj* *libboard.a:ckmn_driver.c.obj* *libboard.a:arch_interrupt.c.obj* *libboard.a:ke_event.c.obj* *libboard.a:vif_mgmt.c.obj* *libboard.a:rxl_cntrl.c.obj* *libboard.a:rxl_hwdesc.c.obj* *libboard.a:rwnx.c.obj* *libboard.a:rwnx_task.o* *libboard.a:rwnx_rx.o* *libboard.a:rwnx_td.o* *libboard.a:rwnx_tx.o* *libboard.a:wifi_v2.o*  *libwifi.a:macif_fhost.o* *libboard.a:rwnx_txq.o* *libboard.a:skbuff.o* *libboard.a:bk_ethernetif.o* *libwifi.a:me_strategy.o* *libwifi.a:rxu_cntrl.o* *libwifi.a:rc.o* *libwifi.a:td.o* *libwifi.a:mcc.o* *libwifi.a:hal_machw.o* *libwifi.a:txl_cfm.o* *libwifi.a:txl_agg.o* *libwifi.a:txl_cntrl.o* *libwifi.a:rxl_hwdesc.o* *libnet.a:netdev_lwip.o* *libnet.a:netif.o* *libnet.a:ip.o* *libnet.a:pbuf.o* *libnet.a:tcp.o* *libnet.a:tcp_in.o* *libnet.a:tcp_out.o* *libnet.a:udp.o* *libnet.a:inet_chksum.o* *libnet.a:tcpip.o* *libnet.a:ip4.o* *libnet.a:sys_arch.o* *libnet.a:netdev_wifi.o* *libnet.a:mem.o* *libnet.a:memp.o* *libnet.a:api_msg.o* *libkmm.a:umm_free.o* *libkmm.a:umm_malloc.o* *libkmm.a:kmm_calloc.o* *libkmm.a:kmm_free.o* *libkmm.a:kmm_malloc.o* *libkmm.a:kmm_realloc.o* *libkmm.a:kmm_zalloc.o* *libkmm.a:kmm_heapmember.o* *libkmm.a:mm_addfreechunk.o* *libkmm.a:mm_calloc.o* *libkmm.a:mm_free.o* *libkmm.a:mm_malloc.o* *libkmm.a:mm_realloc.o* *libkmm.a:mm_zalloc.o* *libkarch.a:up_blocktask.o* *libkarch.a:up_createstack.o* *libkarch.a:up_releasepending.o* *libkarch.a:up_releasestack.o* *libkarch.a:up_reprioritizertr.o* *libkarch.a:up_schedulesigaction.o* *libkarch.a:up_unblocktask.o* *libkarch.a:up_unblocktask_withoutsavereg.o* *libkarch.a:up_restoretask.o* *libkarch.a:up_fullcontextrestore.o* *libkarch.a:up_switchcontext.o* *libkernel.a:mq_send.o* *libkernel.a:mq_timedsend.o* *libkernel.a:mq_sndinternal.o* *libkernel.a:mq_receive.o* *libkernel.a:mq_timedreceive.o* *libkernel.a:mq_rcvinternal.o* *libkernel.a:mq_initialize.o* *libkernel.a:mq_msgfree.o* *libkernel.a:mq_getattr.o* *libkernel.a:mq_waitirq.o* *libkernel.a:mq_recover.o* *libkernel.a:mq_release.o* *libkernel.a:sched_addreadytorun.o* *libkernel.a:sched_removereadytorun.o* *libkernel.a:sched_addprioritized.o* *libkernel.a:sched_free.o* *libkernel.a:sched_releasetcb.o* *libkernel.a:sched_cpuload.o* *libkernel.a:sched_setpriority.o* *libkernel.a:sched_unlock.o* *libkernel.a:task_exithook.o* *libkernel.a:task_recover.o* *libkernel.a:task_getgroup.o* *libkernel.a:task_getpid.o* *libkernel.a:task_terminate.o* *libkernel.a:task_terminate_unloaded.o* *libkernel.a:task_setup.o* *libkernel.a:task_activate.o* *libkernel.a:task_create.o* *libkernel.a:task_start.o* *libkernel.a:task_restart.o* *libkernel.a:sched_gettcb.o* *libkernel.a:sched_getsockets.o* *libkernel.a:sched_setparam.o* *libkernel.a:sched_getparam.o* *libkernel.a:sched_getscheduler.o* *libkernel.a:sched_setscheduler.o* *libkernel.a:sched_yield.o* *libkernel.a:sched_get_priority_max.o* *libkernel.a:sched_get_priority_min.o* *libkernel.a:sched_rr_get_interval.o* *libkernel.a:sem_trywait.o* *libkernel.a:sem_timedwait.o* *libkernel.a:sem_wait.o* *libkernel.a:sem_recover.o* *libkernel.a:sem_reset.o* *libkernel.a:sem_list.o* *libkernel.a:sem_init.o* *libkernel.a:sem_getvalue.o* *libkernel.a:sem_destroy.o* *libkernel.a:sem_post.o* *libkernel.a:sem_setprotocol.o* ) *(.text .text.* .rodata .rodata.*)*/
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
        *armino_irq.o*(.text .text.* *.rodata *.rodata.*) 
        *up_exception.o*(.text .text.* *.rodata *.rodata.*) 
        *up_svcall.o*(.text .text.* *.rodata *.rodata.*) 
        *up_svcall.o*(.text .text.* *.rodata *.rodata.*)
        *up_switchcontext.o*(.text .text.* *.rodata *.rodata.*)
        *up_copyfullstate.o*(.text .text.* *.rodata *.rodata.*)
        *up_fullcontextrestore.o*(.text .text.* *.rodata *.rodata.*)
        *up_doirq.o*(.text .text.* *.rodata *.rodata.*)
        *irq_dispatch.o*(.text .text.* *.rodata *.rodata.*)
        *sched_addreadytorun.o*(.text .text.* *.rodata *.rodata.*)
        *sched_addblocked.o*(.text .text.* *.rodata *.rodata.*)
        *sched_addprioritized.o*(.text .text.* *.rodata *.rodata.*)
        *sched_removereadytorun.o*(.text .text.* *.rodata *.rodata.*)
        *sched_removeblocked.o*(.text .text.* *.rodata *.rodata.*)
        *sched_mergepending.o*(.text .text.* *.rodata *.rodata.*)
        *up_blocktask.o*(.text .text.* *.rodata *.rodata.*)
        *up_unblocktask.o*(.text .text.* *.rodata *.rodata.*)
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
        *armino_irq.o*(.text .text.* *.rodata *.rodata.*) 
        *up_exception.o*(.text .text.* *.rodata *.rodata.*) 
        *up_svcall.o*(.text .text.* *.rodata *.rodata.*) 
        *up_svcall.o*(.text .text.* *.rodata *.rodata.*)
        *up_switchcontext.o*(.text .text.* *.rodata *.rodata.*)
        *up_copyfullstate.o*(.text .text.* *.rodata *.rodata.*)
        *up_fullcontextrestore.o*(.text .text.* *.rodata *.rodata.*)
        *up_doirq.o*(.text .text.* *.rodata *.rodata.*)
        *irq_dispatch.o*(.text .text.* *.rodata *.rodata.*)
        *sched_addreadytorun.o*(.text .text.* *.rodata *.rodata.*)
        *sched_addblocked.o*(.text .text.* *.rodata *.rodata.*)
        *sched_addprioritized.o*(.text .text.* *.rodata *.rodata.*)
        *sched_removereadytorun.o*(.text .text.* *.rodata *.rodata.*)
        *sched_removeblocked.o*(.text .text.* *.rodata *.rodata.*)
        *sched_mergepending.o*(.text .text.* *.rodata *.rodata.*)
        *up_blocktask.o*(.text .text.* *.rodata *.rodata.*)
        *up_unblocktask.o*(.text .text.* *.rodata *.rodata.*)
        *tcp_in.o*(.text .text.* *.rodata *.rodata.*)
        *tcp_out.o*(.text .text.* *.rodata *.rodata.*)
        *def.o*(.text .text.* *.rodata *.rodata.*)
        *igmp.o*(.text .text.* *.rodata *.rodata.*)
        *pbuf.o*(.text .text.* *.rodata *.rodata.*)
        *inet_chksum.o*(.text .text.* *.rodata *.rodata.*)
        *api_msg.o*(.text .text.* *.rodata *.rodata.*)
        *netbuf.o*(.text .text.* *.rodata *.rodata.*)
        *sys_arch.o*(.text .text.* *.rodata *.rodata.*)
        *ethernetif_tizenrt.o*(.text .text.* *.rodata *.rodata.*)
        *lwip_intf_tizenrt.o*(.text .text.* *.rodata *.rodata.*)
        *mem.o*(.text .text.* *.rodata *.rodata.*)
        *memp.o*(.text .text.* *.rodata *.rodata.*)
        *netif.o*(.text .text.* *.rodata *.rodata.*)
        *ip.o*(.text .text.* *.rodata *.rodata.*)
        *ip4.o*(.text .text.* *.rodata *.rodata.*)
        *ip4_addr.o*(.text .text.* *.rodata *.rodata.*)
        *ip4_frag.o*(.text .text.* *.rodata *.rodata.*)
        *tcp.o*(.text .text.* *.rodata *.rodata.*)
        *udp.o*(.text .text.* *.rodata *.rodata.*)
        *socket.o*(.text .text.* *.rodata *.rodata.*)
        *bsd_socket_api.o*(.text .text.* *.rodata *.rodata.*)
        *tcpip.o*(.text .text.* *.rodata *.rodata.*)
        *ethernet.o*(.text .text.* *.rodata *.rodata.*)
        *etharp.o*(.text .text.* *.rodata *.rodata.*)
        *netdb.o*(.text .text.* *.rodata *.rodata.*)
        *api_lib.o*(.text .text.* *.rodata *.rodata.*)
        *netifapi.o*(.text .text.* *.rodata *.rodata.*)*/
        /*
        *rtos_pub.c.obj*(.text .text.* *.rodata *.rodata.*)
        *mem_arch.c.obj*(.text .text.* *.rodata *.rodata.*)
        *flash_driver.c.obj*(.text .text.* *.rodata *.rodata.*)
        *ke_event.c.obj*(.text .text.* *.rodata *.rodata.*)
        *rxl_cntrl.c.obj*(.text .text.* *.rodata *.rodata.*)
        *rxl_hwdesc.c.obj*(.text .text.* *.rodata *.rodata.*)
        *rwnx_task.o*(.text .text.* .rodata .rodata.*)
        *rwnx_rx.o*(.text .text.* .rodata .rodata.*)
        *rwnx_td.o*(.text .text.* .rodata .rodata.*)
        *rwnx_tx.o*(.text .text.* .rodata .rodata.*)
        *macif_fhost.o*(.text .text.* .rodata .rodata.*)
        *rwnx_tx.o*(.text .text.* .rodata .rodata.*)
        *rwnx_txq.o*(.text .text.* .rodata .rodata.*)
        *skbuff.o*(.text .text.* .rodata .rodata.*)
        *bk_lwipif_tizenrt.o*(.text .text.* .rodata .rodata.*)
        *bk_ethernetif.o*(.text .text.* .rodata .rodata.*)
        *rxu_cntrl.o*(.text .text.* .rodata .rodata.*)
        *rc.o*(.text .text.* .rodata .rodata.*)
        *td.o*(.text .text.* .rodata .rodata.*)
        *hal_machw.o*(.text .text.* .rodata .rodata.*)
        *txl_cfm.o*(.text .text.* .rodata .rodata.*)
        *txl_agg.o*(.text .text.* .rodata .rodata.*)
        *txl_cntrl.o*(.text .text.* .rodata .rodata.*)
        *rxl_hwdesc.o*(.text .text.* .rodata .rodata.*)
        *mem.o*(.text .text.* .rodata .rodata.*)
        *memp.o*(.text .text.* .rodata .rodata.*)
        *umm_free.o*(.text .text.* .rodata .rodata.*)
        *umm_malloc.o*(.text .text.* .rodata .rodata.*)
        *kmm_calloc.o*(.text .text.* .rodata .rodata.*)
        *kmm_free.o*(.text .text.* .rodata .rodata.*)
        *kmm_malloc.o*(.text .text.* .rodata .rodata.*)
        *kmm_realloc.o*(.text .text.* .rodata .rodata.*)
        *kmm_zalloc.o*(.text .text.* .rodata .rodata.*)
        *kmm_heapmember.o*(.text .text.* .rodata .rodata.*)
        *mm_addfreechunk.o*(.text .text.* .rodata .rodata.*)
        *mm_calloc.o*(.text .text.* .rodata .rodata.*)
        *mm_free.o*(.text .text.* .rodata .rodata.*)
        *mm_malloc.o*(.text .text.* .rodata .rodata.*)
        *mm_realloc.o*(.text .text.* .rodata .rodata.*)
        *mm_zalloc.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_blocktask.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_reprioritizertr.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_schedulesigaction.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_unblocktask.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_unblocktask_withoutsavereg.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_restoretask.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_signal_dispatch.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_fullcontextrestore.o*(.text .text.* .rodata .rodata.*)
        *libkarch.a:up_switchcontext.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_send.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_timedsend.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_sndinternal.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_receive.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_timedreceive.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_rcvinternal.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_initialize.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_msgfree.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_getattr.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_waitirq.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_recover.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:mq_release.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_addreadytorun.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_removereadytorun.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_addprioritized.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_free.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_releasetcb.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_setpriority.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_unlock.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:task_exithook.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:task_setup.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:task_activate.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:task_create.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:task_start.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:task_restart.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_gettcb.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_getsockets.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_setparam.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_getparam.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_getscheduler.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_setscheduler.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_yield.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_get_priority_max.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_get_priority_min.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sched_rr_get_interval.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_trywait.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_timedwait.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_wait.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_recover.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_reset.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_list.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_init.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_getvalue.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_destroy.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_post.o*(.text .text.* .rodata .rodata.*)
        *libkernel.a:sem_setprotocol.o*(.text .text.* .rodata .rodata.*)*/

        *rwnx_rx.o*(.text .text.* .rodata .rodata.*)

        . = ALIGN(4);
        __psram_code_end__ = .;
    } > PSRAM_CODE  AT > FLASH

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
        *libnet.a:memp.o*(.bss .bss.*)
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
    ASSERT(((_heap_end - _heap_start) >= __MIN_HEAP_SIZE), "Heap smaller than minimize size 40K!!!")
}

