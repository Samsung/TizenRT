/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _SECTION_CONFIG_H_
#define _SECTION_CONFIG_H_

#include "basic_types.h"
#include "platform_autoconf.h"

#define RAM_VECTOR_TABLE1_SECTION          \
        SECTION(".ram_vector_table1")
#define RAM_VECTOR_TABLE2_SECTION          \
        SECTION(".ram_vector_table2")
#define RAM_VECTOR_TABLE3_SECTION          \
        SECTION(".ram_vector_table3")

//3 //3 Hal Section
#define HAL_ROM_TEXT_SECTION                         \
        SECTION(".hal.rom.text")

#define HAL_ROM_ENTRY_NS_SECTION                         \
        SECTION(".hal.rom.entryns.rodata")

#define SIM_ROM_DATA_SECTION                         \
        SECTION(".sim.rom.rodata")

#define HAL_ROM_DATA_SECTION                         \
        SECTION(".hal.rom.rodata")

#define HAL_ROM_BSS_SECTION                         \
        SECTION(".hal.rom.bss")

#define HAL_ROM_BSS_SECTION_BANK		\
        SECTION(".hal.rom.bank.bss")

#define BOOT_RAM_TEXT_SECTION                         \
        SECTION(".boot.ram.text")

#define BOOT_RAM_RODATA_SECTION                         \
        SECTION(".boot.rodata")

#define BOOT_RAM_DATA_SECTION                         \
        SECTION(".boot.ram.data")

#define BOOT_RAM_BSS_SECTION                         \
        SECTION(".boot.ram.bss")

//3 Shell
#define SHELL_ROM_TEXT_SECTION	HAL_ROM_TEXT_SECTION
#define SHELL_ROM_BSS_SECTION		HAL_ROM_BSS_SECTION
#define SHELL_ROM_DATA_SECTION	HAL_ROM_DATA_SECTION

#define CMD_TABLE_DATA_SECTION				\
	SECTION(".cmd.table.data")

#define GIT_VER_TABLE_SECTION				\
	SECTION(".git.ver.data")

//3 Image 1 data
#define IMAGE1_ENTRY_SECTION                         \
        SECTION(".image1.entry.data")

#define IMAGE1_EXPORT_SYMB_SECTION                         \
		SECTION(".image1.export.symb")

#define IMAGE1_VALID_PATTEN_SECTION                   \
        SECTION(".image1.validate.rodata")

#define IMAGE1_DATA_SECTION                   \
        SECTION(".image1.rodata")

#define IMAGE2_VALID_PATTEN_SECTION                   \
        SECTION(".image2.validate.rodata")

//3 SRAM Config Section
#define SRAM_BD_DATA_SECTION                        \
        SECTION(".bdsram.data")
#define SRAM_NOCACHE_DATA_SECTION		\
        SECTION(".ncsram.data")

#define SRAM_BF_DATA_SECTION                        \
        SECTION(".bfsram.data")

#define IMAGE2_ENTRY_SECTION                     \
            SECTION(".image2.entry.data")

#define SDRAM_DATA_SECTION
#define PSRAM_TEXT_SECTION
#define PSRAM_DATA_SECTION
#define PSRAM_RODATA_SECTION
#define PSRAM_BSS_SECTION
#define PSRAM_HEAP_SECTION

#define TIMESENSITIVE_TEXT_SECTION			SECTION(".timesensitive.text")
#define TIMESENSITIVE_DATA_SECTION			SECTION(".timesensitive.data")

/* non.dram can put in Flash(No DeepPowerDown) or SRAM after psram disabled, such as pmc code */
#define NON_DRAM_TEXT_SECTION				SECTION(".non.dram.text")

/* .sramdram.only means cannot put in Flash, such as flash api, interrupt isr */
#define SRAMDRAM_ONLY_TEXT_SECTION			SECTION(".sramdram.only.text")

/* sram only used in pmc flow, such as deepsleep entry when flash deep down or after psram disabled */
#define SRAM_ONLY_TEXT_SECTION				SECTION(".sram.only.text")
#define SRAM_ONLY_DATA_SECTION				SECTION(".sram.only.data")

#define SBOOT_HEAP_SECTION			SECTION(".sboot.heap")

#define IPC_TABLE_DATA_SECTION				\
	SECTION(".ipc.table.data")

//3 Wlan  Section
#define WLAN_ROM_TEXT_SECTION

#define WLAN_ROM_DATA_SECTION

#define WLAN_RAM_MAP_SECTION

#ifdef AMEBAD2_TODO// move wifi code into flash to reduce ram size, revert this when psram or ddr ready
#undef CONFIG_WIFI_CRITICAL_CODE_SECTION
#define CONFIG_WIFI_CRITICAL_CODE_SECTION		SECTION(".image2.net.ram.text")
#endif
#define CONFIG_FW_CRITICAL_CODE_SECTION		//IMAGE2_RAM_TEXT_SECTION
//3 Apple  Section
#define APPLE_ROM_TEXT_SECTION                      \
        SECTION(".apple.rom.text")

#define APPLE_ROM_DATA_SECTION                      \
        SECTION(".apple.rom.rodata")

//3 Libc Section
#define LIBC_ROM_TEXT_SECTION                      \
        SECTION(".libc.rom.text")

#define LIBC_ROM_DATA_SECTION                      \
        SECTION(".libc.rom.rodata")


#define LIBC_HEAP_SECTION                         	\
        SECTION(".heap.stdlib")

//3 SSL Section
#define SSL_ROM_TEXT_SECTION				\
        SECTION(".ssl.rom.text")
#define SSL_ROM_DATA_SECTION				\
        SECTION(".ssl.rom.rodata")
#define SSL_RAM_MAP_SECTION				\
        SECTION(".ssl_ram_map")

//OS Section
#define OS_ROM_TEXT_SECTION                      \
        SECTION(".os.rom.text")

#define OS_ROM_DATA_SECTION                      \
        SECTION(".os.rom.rodata")

//FLASH RUN CODE
#define IMAGE2_CUSTOM_SIGNATURE		SECTION(".img2_custom_signature") /* 32B: for OTA update */

//RDP (read protect area just text)
#define RDP_TEXT_SECTION		SECTION(".rdp.ram.text")
#define RDP_DATA_SECTION		SECTION(".rdp.ram.data")

#define RETENTION_TEXT_SECTION		SECTION(".retention.ram.text")
#define RETENTION_DATA_SECTION		SECTION(".retention.ram.data")
#define RETENTION_ENTRY_SECTION		SECTION(".retention.entry.data")

/* rom map */
#define ROM_FUNCTION_MAP		SECTION(".rommap.data")

/* image3 secure image */
#define IMAGE3_ENTRY_SECTION                     \
	SECTION(".image3.nsc_entry.text")



/*USB_OTG define*/
#define OTG_ROM_TEXT_SECTION
#define START_OTG_RAM_FUN_SECTION
#define START_OTG_RAM_DATA_SECTION
#define OTG_ROM_DATA_SECTION

#endif //_SECTION_CONFIG_H_
