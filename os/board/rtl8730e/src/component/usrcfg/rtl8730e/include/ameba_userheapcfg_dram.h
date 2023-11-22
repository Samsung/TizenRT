#ifndef _AMEBA_USERHEAPCFG_XIP_H_
#define _AMEBA_USERHEAPCFG_XIP_H_

//HeapRegion Addresses need to be sorted from smallest to largest
#if defined ( __ICCARM__ )
#if (defined ARM_CORE_CM4)
//The flow heap size shall great than 300K defined in configTOTAL_HEAP_SIZE

//#define SRAM_HEAP0_START				__bdram_heap_buffer_start__
//#define SRAM_HEAP0_SIZE				__bdram_heap_buffer_size__

#define PSRAM_HEAP0_START				(NOCACHE_DATA$$Limit + 0x2000)
#define PSRAM_HEAP0_SIZE				((u32)__km4_bd_dram_end__- (u32)PSRAM_HEAP0_START)

/* KM4 in Linux does not need heap extend */
#ifndef CONFIG_LINUX_FW_EN
#define PSRAM_HEAP1_START				__km4_heap_ext_start__
#define PSRAM_HEAP1_SIZE				__km4_heap_ext_size__
#endif

#elif (defined ARM_CORE_CM0)
//The flow heap size shall great than 20K defined in configTOTAL_HEAP_SIZE

#define SRAM_HEAP0_START				(NOCACHE_DATA$$Limit + 0x1000)
#define SRAM_HEAP0_SIZE				((u32)__km0_bd_ram_end__ - (u32)SRAM_HEAP0_START)

//#define PSRAM_HEAP0_START				__psram_heap_buffer_start__
//#define PSRAM_HEAP0_SIZE				__psram_heap_buffer_size__

#else
//The flow heap size shall great than 250K defined in configTOTAL_HEAP_SIZE

#define PSRAM_HEAP0_START 			IMAGENCSRAM$$Limit
#define PSRAM_HEAP0_SIZE				((u32)__ca32_dram_end__ - (u32)PSRAM_HEAP0_START)
//#define PSRAM_HEAP0_START				__psram_heap_buffer_start__
//#define PSRAM_HEAP0_SIZE				__psram_heap_buffer_size__

#endif

#else

#if (defined ARM_CORE_CM4)
//The flow heap size shall great than 300K defined in configTOTAL_HEAP_SIZE

//#define SRAM_HEAP0_START				__bdram_heap_buffer_start__
//#define SRAM_HEAP0_SIZE				__bdram_heap_buffer_size__

#define PSRAM_HEAP0_START				__psram_heap_buffer_start__
#define PSRAM_HEAP0_SIZE				__psram_heap_buffer_size__

/* KM4 in Linux does not need heap extend */
#ifndef CONFIG_LINUX_FW_EN
#define PSRAM_HEAP1_START				__psram_heap_extend_start__
#define PSRAM_HEAP1_SIZE				__psram_heap_extend_size__
#endif

#elif (defined ARM_CORE_CM0)
//The flow heap size shall great than 20K defined in configTOTAL_HEAP_SIZE

#define SRAM_HEAP0_START				__bdram_heap_buffer_start__
#define SRAM_HEAP0_SIZE					__bdram_heap_buffer_size__

#define PSRAM_HEAP0_START				__psram_heap_buffer_start__
#define PSRAM_HEAP0_SIZE				__psram_heap_buffer_size__

#else
//The flow heap size shall great than 250K defined in configTOTAL_HEAP_SIZE

#define PSRAM_HEAP0_START				__psram_heap_buffer_start__
#define PSRAM_HEAP0_SIZE				__psram_heap_buffer_size__

#endif

#endif

#endif
