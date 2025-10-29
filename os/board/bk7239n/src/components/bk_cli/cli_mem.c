#include <stdlib.h>
#include "cli.h"
#include "bk_cli.h"
#include <os/os.h>
#include <os/mem.h>
#include <components/system.h>
#include "bk_rtos_debug.h"
#if defined(CONFIG_PSRAM)
#include <driver/psram.h>
#endif

/* Platform includes. */
#include "sys_driver.h"
#include <soc/soc.h>

#if defined(CONFIG_AON_RTC)
#include <driver/aon_rtc.h>
#include <driver/aon_rtc_types.h>
#endif

///TODO: Just for test
#define CONFIG_DEBUG_FIRMWARE 1

void bk_mem_dump(const char *title, uint32_t start, uint32_t len)
{
	uint32_t val = 0;
	uint32_t mem;

	BK_LOG_RAW("%s - dump(len=%u)\n", title, len);

	for (uint32_t i = 0; i < len; i += 4) {
		mem = start + i;

		if ((i % 32) == 0) {
			if (i)
				BK_LOG_RAW("\n");

			BK_LOG_RAW("%x:", mem);
		}

		val = *((volatile uint32_t *)(mem));
		BK_LOG_RAW(" %08x", val);
	}
	BK_LOG_RAW("\n");
}

void cli_memory_free_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t total_size,free_size,mini_size;
    CLI_LOGI("================Static memory================\r\n");
    os_show_memory_config_info();

	CLI_LOGI("================Dynamic memory================\r\n");
	cmd_printf("%-5s   %-5s   %-5s   %-5s   %-5s\r\n",
		"name", "total", "free", "minimum", "peak");
	
	total_size = rtos_get_total_heap_size();
	free_size  = rtos_get_free_heap_size();
	mini_size  = rtos_get_minimum_free_heap_size();
	cmd_printf("heap\t%d\t%d\t%d\t%d\r\n",  total_size,free_size,mini_size,total_size-mini_size);

#if defined(CONFIG_PSRAM_AS_SYS_MEMORY)
	total_size = rtos_get_psram_total_heap_size();
	free_size  = rtos_get_psram_free_heap_size();
	mini_size  = rtos_get_psram_minimum_free_heap_size();
	cmd_printf("psram\t%d\t%d\t%d\t%d\r\n", total_size,free_size,mini_size,total_size-mini_size);
#endif

}

void cli_memory_set_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_DEBUG_FIRMWARE)
    uint32_t address, value;
    CLI_LOGI("cli_memory_set_cmd\r\n");
    if (argc >= 3) {
        address = strtol(argv[1], NULL, 16);
        value = strtol(argv[2], NULL, 16);
        CLI_LOGI("memset,address: 0x%08X value: 0x%08X\r\n", address, value);

        os_write_word(address, value);
    } else {
        CLI_LOGI("memset <addr> <value>\r\n");
    }
#endif
}

#if defined(CONFIG_DEBUG_FIRMWARE)
const static uint32_t s_test_data[20] = {
    0x00000000, 0x800102a0, 0x30021e44, 0x30034088,
    0x5f696c63, 0x61727370, 0x616d5f6d, 0x636f6c6c,
    0x646d635f, 0x00000000, 0x736d656d, 0x6b636174,
    0x00696c63, 0x00000000, 0x00001ed9, 0x0000005c,
    0x00010240, 0x12345678, 0x99887766, 0xaabbccdd
};

__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
int32_t memtest_write_one_word(uint32_t addr, uint32_t count) {
    uint32_t i;
    uint32_t src_data = 0x30023456;
    uint32_t *p_uint32_dst = (uint32_t *)addr;
    for(i = 0; i < count; i++)
    {
        os_write_word(p_uint32_dst, src_data);
        src_data++;
    }
    return 0;
}


__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
int32_t memtest_wr(uint32_t addr, uint32_t count)
{
#if defined(CONFIG_DEBUG_FIRMWARE)
    int int_status = rtos_enter_critical();
    CLI_LOGI("memtest_wr begin!!\r\n");
    os_memcpy_word((uint32_t *)addr, &s_test_data[2], sizeof(s_test_data) - 8);

    bk_mem_dump("before test", addr - 8, sizeof(s_test_data));

    memtest_write_one_word(addr, count);

    bk_mem_dump("after test", addr - 8, sizeof(s_test_data));

    CLI_LOGI("memtest_wr done!!\r\n");
    rtos_exit_critical(int_status);
#endif //#if CONFIG_DEBUG_FIRMWARE

    return 0;
}


static void cli_memtest_wr_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t address, count;

    if (argc >= 3) {
        address = strtol(argv[1], NULL, 16);
        count = strtol(argv[2], NULL, 16);
        CLI_LOGI("memtest_wr,address: 0x%08X count: 0x%08X\r\n", address, count);

        (void)memtest_wr(address, count);
    }  else {
        CLI_LOGI("memtest_wr <addr> <count> \r\n");
    }
}

#endif //#if CONFIG_DEBUG_FIRMWARE

void cli_memory_stack_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
#if defined(CONFIG_FREERTOS)
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
//	rtos_dump_stack_memory_usage();
	GLOBAL_INT_RESTORE();
#endif
}

#if defined(CONFIG_MEM_DEBUG) && defined(CONFIG_FREERTOS)
static void cli_memory_leak_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t start_tick = 0;
	uint32_t ticks_since_malloc = 0;
	uint32_t seconds_since_malloc = 0;
	char *task_name = NULL;

	if (argc >= 2) {
		seconds_since_malloc = os_strtoul(argv[1], NULL, 10);
		ticks_since_malloc = bk_get_ticks_per_second() * seconds_since_malloc;
	}

	if (argc >= 3)
		start_tick = os_strtoul(argv[2], NULL, 10);

	if (argc >= 4)
		task_name = argv[3];

	os_dump_memory_stats(start_tick, ticks_since_malloc, task_name);
}
#endif

#if defined(CONFIG_PSRAM_AS_SYS_MEMORY) && defined(CONFIG_FREERTOS)
void cli_psram_malloc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t *pstart;
	uint32_t length;

	if (argc != 2) {
		cmd_printf("Usage: psram_malloc <length>.\r\n");
		return;
	}

	length = strtoul(argv[1], NULL, 0);

	pstart = (uint8_t *)psram_malloc(length);

	cmd_printf("psram_malloc ret(%p).\r\n", pstart);
}

void cli_psram_free_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint8_t *pstart;
	uint32_t start;

	if (argc != 2) {
		cmd_printf("Usage: psram_free <addr>.\r\n");
		return;
	}

	start = strtoul(argv[1], NULL, 0);
	pstart = (uint8_t *)start;
	cmd_printf("psram_free addr(%p).\r\n", pstart);
	os_free(pstart);

}

void cli_psram_state_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
//    bk_psram_heap_get_used_state();
}


#endif //#if CONFIG_PSRAM_AS_SYS_MEMORY && CONFIG_FREERTOS

int32_t mem_test(uint32_t address, uint32_t size, uint8_t quiet_mode)
{
    uint32_t i;

    /**< 32bit test */
    {
        uint32_t * p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            *p_uint32_t++ = (uint32_t)i;
        }

        p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            if( *p_uint32_t != (uint32_t)i )
            {
                if (quiet_mode == 0) {
                    CLI_LOGI("32bit test fail @ 0x%08X\r\n",(uint32_t)p_uint32_t);
                    return -1;
                }
                while(1);
            }
            p_uint32_t++;
        }

        if (quiet_mode == 0) {
            CLI_LOGI("32bit test pass!!\r\n");
        }

    }

    /**< 32bit Loopback test */
    {
        uint32_t * p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            *p_uint32_t  = (uint32_t)p_uint32_t;
            p_uint32_t++;
        }

        p_uint32_t = (uint32_t *)address;
        for(i=0; i<size/sizeof(uint32_t); i++)
        {
            if( *p_uint32_t != (uint32_t)p_uint32_t )
            {
                if (quiet_mode == 0) {
                    CLI_LOGI("32bit Loopback test fail @ 0x%08X\r\n", (uint32_t)p_uint32_t);
                    CLI_LOGI(" data:0x%08X \r\n", (uint32_t)*p_uint32_t);
                    return -1;
                }

                while(1);
            }
            p_uint32_t++;
        }

        if (quiet_mode == 0) {
            CLI_LOGI("32bit Loopback test pass!!\r\n");
        }
    }


    /**< 16bit test */
    {
        uint16_t * p_uint16_t = (uint16_t *)address;
        for(i=0; i<size/sizeof(uint16_t); i++)
        {
            *p_uint16_t++ = (uint16_t)i;
        }

        p_uint16_t = (uint16_t *)address;
        for(i=0; i<size/sizeof(uint16_t); i++)
        {
            if( *p_uint16_t != (uint16_t)i )
            {
                if (quiet_mode == 0) {
                    CLI_LOGI("16bit test fail @ 0x%08X\r\n",(uint32_t)p_uint16_t);
                    return -1;
                }

                while(1);
            }
            p_uint16_t++;
        }

        if (quiet_mode == 0) {
            CLI_LOGI("16bit test pass!!\r\n");
        }

    }


    /**< 8bit test */
    {
        uint8_t * p_uint8_t = (uint8_t *)address;
        for(i=0; i<size/sizeof(uint8_t); i++)
        {
            *p_uint8_t++ = (uint8_t)i;
        }

        p_uint8_t = (uint8_t *)address;
        for(i=0; i<size/sizeof(uint8_t); i++)
        {
            if( *p_uint8_t != (uint8_t)i )
            {
                if (quiet_mode == 0) {
                    CLI_LOGI("8bit test fail @ 0x%08X\r\n",(uint32_t)p_uint8_t);
                    return -1;
                }
                while(1);
            }
            p_uint8_t++;
        }
        if (quiet_mode == 0) {
            CLI_LOGI("8bit test pass!!\r\n");
        }
    }

    return 0;
}

__attribute__ ((__optimize__ ("-fno-tree-loop-distribute-patterns"))) \
void mem_time(uint32_t *base, uint32_t count, uint8_t mode)
{
    /** write addr x times*/
__maybe_unused volatile uint32_t data = 0;
    volatile uint32_t *address = base;
    uint64_t saved_aon_time = 0, cur_aon_time = 0, diff_time = 0;
    uint32_t diff_ms = 0;

    uint32_t intbk = rtos_enter_critical();

#if defined(CONFIG_AON_RTC)
    saved_aon_time = bk_aon_rtc_get_us();
#endif

    switch(mode)
    {
        case 0://single read
            for(uint32_t i=0; i<count; i++)
            {
                data = REG_READ(address);
            }
            break;
        case 1://single write
            for(uint32_t i=0; i<count; i++)
            {
                REG_WRITE(address, i);
            }
            break;
        case 2://read&write
            for(uint32_t i=0; i<count; i++)
            {
                REG_WRITE(address, i);
                data = REG_READ(address);
            }
            break;
        case 3://multi-mem read
            for(uint32_t i=0; i<count; i++)
            {
                data = REG_READ(address);
                address++;
                if(address == (base+256) ) address = base;
            }
            break;
        case 4://multi-mem write
            for(uint32_t i=0; i<count; i++)
            {
                REG_WRITE(address, i);
                address++;
                if(address == (base+256) ) address = base;
            }
        case 5://multi-mem read&write
            for(uint32_t i=0; i<count; i++)
            {
                data = REG_READ(address);
                REG_WRITE(address, i);
                address++;
                if(address == (base+256) ) address = base;
            }
            break;

        default:
            CLI_LOGI("error!!\r\n");
    }

#if defined(CONFIG_AON_RTC)
    cur_aon_time = bk_aon_rtc_get_us();
    diff_time = (cur_aon_time - saved_aon_time);
    diff_ms = (uint32_t)diff_time/1000;
#endif

    rtos_exit_critical(intbk);

    BK_DUMP_OUT("saved time: 0x%x:0x%08x\r\n", (u32)(saved_aon_time >> 32), (u32)(saved_aon_time & 0xFFFFFFFF));
    BK_DUMP_OUT("curr time: 0x%x:0x%08x\r\n", (u32)(cur_aon_time >> 32), (u32)(cur_aon_time & 0xFFFFFFFF));
    BK_DUMP_OUT("diff time: 0x%x:0x%08x\r\n", (u32)(diff_time >> 32), (u32)(diff_time & 0xFFFFFFFF));

    BK_DUMP_OUT("memtime end, time consume=%d ms\r\n", diff_ms);
}

int32_t mem_read_test(uint32_t src, uint32_t dst, uint32_t size)
{
#if defined(CONFIG_DEBUG_FIRMWARE)
    uint32_t i;
    uint32_t test_count = size/sizeof(uint32_t);

    os_memcpy_word((uint32_t *)dst, (uint32_t *)src, size);

    /**< 32bit test */
    {
        uint32_t *p_uint32_src = (uint32_t *)src;
        uint32_t *p_uint32_dst = (uint32_t *)dst;

        for(i = 0; i < test_count; i++)
        {
            if( *p_uint32_src != *p_uint32_dst)
            {
                CLI_LOGI("32bit test fail @ 0x%08X\r\n!",(uint32_t)p_uint32_src);
                return -1;
            }
            p_uint32_src++;
            p_uint32_dst++;
        }

        CLI_LOGI("32bit test pass!!\r\n");
    }

    /**< 16bit test */
    {
        uint16_t *p_uint16_src = (uint16_t *)src;
        uint16_t *p_uint16_dst = (uint16_t *)dst;

        test_count = size/sizeof(uint16_t);
        for(i = 0; i < test_count; i++)
        {
            if( *p_uint16_src != *p_uint16_dst )
            {
                CLI_LOGI("16bit test fail @ 0x%08X\r\nsystem halt!!!!!",(uint32_t)p_uint16_src);
                return -1;
            }
            p_uint16_src++;
            p_uint16_dst++;
        }

        CLI_LOGI("16bit test pass!!\r\n");
    }

    /**< 8bit test */
    {
        uint8_t *p_uint8_src = (uint8_t *)src;
        uint8_t *p_uint8_dst = (uint8_t *)dst;

        test_count = size/sizeof(uint8_t);
        for(i = 0; i < test_count; i++)
        {
            if( *p_uint8_src != *p_uint8_dst )
            {
                CLI_LOGI("8bit test fail @ 0x%08X\r\n", (uint32_t)p_uint8_src);
                return -1;
            }
            p_uint8_src++;
            p_uint8_dst++;
        }

        CLI_LOGI("8bit test pass!!\r\n");
    }

    /**< 32bit test write one address*/
    {
        uint32_t *p_uint32_src = (uint32_t *)src;
        uint32_t *p_uint32_dst = (uint32_t *)dst;
        uint32_t *p_uint32_next = (uint32_t *)dst + 1;

        test_count = size/sizeof(uint32_t);
        for(i = 0; i < test_count; i++)
        {
            *p_uint32_dst = *p_uint32_src;
            if(*p_uint32_next != *(p_uint32_dst + 1)) {
                CLI_LOGI("32bit test write one address fail @ 0x%08X\r\n!",(uint32_t)p_uint32_next);
                CLI_LOGI("==== next o:%08X,next n:%08X\r\n!",*p_uint32_next, *(p_uint32_dst + 1));
                return -1;
            }
            if( *p_uint32_src != *p_uint32_dst)
            {
                CLI_LOGI("32bit test write one address fail @ 0x%08X\r\n!",(uint32_t)p_uint32_src);
                CLI_LOGI("==== src:%08X,dest:%08X\r\n!",*p_uint32_src, *p_uint32_dst);
                return -1;
            }
            p_uint32_src++;
        }

        CLI_LOGI("32bit test write one address pass!!\r\n");
    }
#endif
    return 0;
}


static void cli_mem_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t address, size;

    if (argc >= 3) {
        address = strtol(argv[1], NULL, 16);
        size = strtol(argv[2], NULL, 16);
        CLI_LOGI("memtest,address: 0x%08X size: 0x%08X\r\n", address, size);

        mem_test(address, size, 0);
    } else if (argc == 1) {
        // auto_mem_test();
    } else {
        CLI_LOGI("memtest <addr> <length> \r\n");
    }
}


static void cli_mem_time(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t address, count, mode;
    if (argc >= 4) {
        address = strtol(argv[1], NULL, 16);
        count = strtol(argv[2], NULL, 16);
        mode = strtol(argv[3], NULL, 16);
    } else {
        CLI_LOGI("memtime <addr> <count> <0:write,1:read> \r\n");
        return;
    }
    CLI_LOGI("memtime, address: 0x%08X count: 0x%08X, read=%d\r\n", address, count, mode);
    mem_time((uint32_t *)address, count, mode);
}


static void cli_memread_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t src, dest, size;

    if (argc >= 4) {
        src = strtol(argv[1], NULL, 16);
        dest = strtol(argv[2], NULL, 16);
        size = strtol(argv[3], NULL, 16);
        CLI_LOGI("memread, src: 0x%08X dest: 0x%08X size: 0x%08X\r\n", src, dest, size);
        mem_read_test(src, dest, size);
    } else {
        CLI_LOGI("memread <src> <dest> <size> \r\n");
    }
}

#if  0   //defined(CONFIG_MPU)
void mpu_cfg(int index, uint32_t rbar, uint32_t rlar);
void mpu_init(void);

static void cli_mpucfg_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t index = 0, rbar = 0, rlar = 0;

    if (argc >= 4) {
        index = strtol(argv[1], NULL, 10);
        rbar = strtol(argv[2], NULL, 16);
        rlar = strtol(argv[3], NULL, 16);
    } else {
        CLI_LOGI("mpucfg <index> <rbar> <rlar>\r\n");
        return;
    }

    CLI_LOGI("mpucfg, index:%d rbar: 0x%08X rlar: 0x%08X.\r\n", index, rbar, rlar);
    mpu_cfg(index, rbar, rlar);
}

void mpu_clear(uint32_t rnr);
static void cli_mpuclr_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv) {
    uint32_t rnr;

    if (argc >= 2) {
        rnr = strtol(argv[1], NULL, 10);
    } else {
        CLI_LOGI("mpuclr <rnr>.\r\n");
        return;
    }

    CLI_LOGI("mpuclr, rnr:%d.\r\n", rnr);
    mpu_clear(rnr);
}

void mpu_dump(void);
static void cli_mpudump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv) {
    mpu_dump();
}
#endif //#if CONFIG_MPU

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
void bk_print_dump_hex(const uint8_t *ptr, size_t buflen)
{
    unsigned char *buf = (unsigned char*)ptr;
    int i, j;

    for (i=0; i<buflen; i+=16)
    {
        CLI_LOGI("%08X: ", i);

        for (j=0; j<16; j++)
            if (i+j < buflen)
                CLI_LOGI("%02X ", buf[i+j]);
            else
                CLI_LOGI("   ");
        CLI_LOGI(" ");

        for (j=0; j<16; j++)
            if (i+j < buflen)
                CLI_LOGI("%c", __is_print(buf[i+j]) ? buf[i+j] : '.');
        CLI_LOGI("\n");
    }
}


static void cli_memory_dump_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t address, size;

    if (argc >= 3) {
        address = strtol(argv[1], NULL, 16);
        size = strtol(argv[2], NULL, 16);
        CLI_LOGI("dump,address: 0x%08X size: 0x%08X\r\n", address, size);

        if (argc == 3) {
            bk_print_dump_hex((const uint8_t *)address, size);
        } else {
            bk_mem_dump("cli", address, size);
        }
    } else {
        CLI_LOGI("Usage: memdump <addr> <length>.\r\n");
        return;
    }
}

#if defined(CONFIG_ARCH_CM33)
extern void arch_dwt_trap_write(uint32_t addr, uint32_t data);
extern void arch_dwt_trap_disable(void);

static void cli_trap_write(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint32_t address, data;

    if (argc >= 3) {
        address = strtol(argv[1], NULL, 16);
        data = strtol(argv[2], NULL, 16);

		arch_dwt_trap_write(address, data);
		
        CLI_LOGI("trap_write,address: 0x%08X data: 0x%08X\r\n", address, data);
    } else {
        CLI_LOGI("Usage: trap_write <addr> <data> \r\n");
        return;
    }
}

static void cli_trap_disable(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	arch_dwt_trap_disable();
	CLI_LOGI("trap_disabled!\r\n");
}
#endif

#define MEM_CMD_CNT (sizeof(s_mem_commands) / sizeof(struct cli_command))
static const struct cli_command s_mem_commands[] = {
    {"memstack", "show stack memory usage", cli_memory_stack_cmd},
    {"memshow", "show free heap", cli_memory_free_cmd},
#if defined(CONFIG_MEM_DEBUG) && defined(CONFIG_FREERTOS)
    {"memleak", "[show memleak", cli_memory_leak_cmd},
#endif
#if defined(CONFIG_DEBUG_FIRMWARE)
    {"memdump", "<addr> <length>", cli_memory_dump_cmd},
    {"memset", "<addr> <value 1> [<value 2> ... <value n>]", cli_memory_set_cmd},
    {"memtest", "<addr> <length>", cli_mem_test},
    {"memtest_r", "<src> <dest> <size>", cli_memread_test},
    {"memtest_wr", "<addr> <count>", cli_memtest_wr_cmd},
	{"memtime", "<addr> <count> <0:write,1:read>", cli_mem_time},
#if 0  //defined(CONFIG_MPU)
    {"mpucfg", "<rnr> <rbar> <rlar>", cli_mpucfg_cmd},
    {"mpuclr", "<rnr>", cli_mpuclr_cmd},
    {"mpudump", "dump mpu config", cli_mpudump_cmd},
#endif //#if CONFIG_MPU
#endif //#if CONFIG_DEBUG_FIRMWARE
#if defined(CONFIG_PSRAM_AS_SYS_MEMORY) && defined(CONFIG_FREERTOS)
    {"psram_malloc", "psram_malloc <length>", cli_psram_malloc_cmd},
    {"psram_free", "psram_free <addr>", cli_psram_free_cmd},
    {"psram_state", "psram_state", cli_psram_state_cmd},
#endif
};

int cli_mem_init(void)
{
	return cli_register_commands(s_mem_commands, MEM_CMD_CNT);
}
