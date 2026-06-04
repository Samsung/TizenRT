#include <stdlib.h>
#include "cli.h"
#include "driver/flash.h"
#include <driver/flash_partition.h>
#include "sys_driver.h"
#include "flash_bypass.h"
#include "flash.h"
#include <driver/psram.h>
#include "bk_misc.h"
#include "driver/wdt.h"
#include "bk_wdt.h"
#include <driver/aon_rtc.h>
#include <common/bk_assert.h>

#define  TICK_PER_US    26
beken_thread_t idle_read_flash_handle = NULL;
beken_thread_t idle_read_psram_handle = NULL;

static bk_err_t test_flash_write(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t i;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	uint32_t tmp = addr + length;

	for (i = 0; i < 256; i++)
		buf[i] = i;

	for (; addr < tmp; addr += 256) {
		int int_level = rtos_enter_critical();
		BK_DUMP_OUT("write addr(size:256):%d\r\n", addr);
		rtos_exit_critical(int_level);
		bk_flash_write_bytes(addr, (uint8_t *)buf, 256);
	}

	return kNoErr;
}

static bk_err_t test_flash_erase(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t addr = start_addr;
	uint32_t length = len;
	uint32_t tmp = addr + length;

	for (; addr < tmp; addr += 0x1000) {
		int int_level = rtos_enter_critical();
		BK_DUMP_OUT("erase addr:%d\r\n", addr);
		rtos_exit_critical(int_level);
		bk_flash_erase_sector(addr);
	}
	return kNoErr;
}

static bk_err_t test_flash_read(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t i, j, tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	tmp = addr + length;

	for (; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
		int int_level = rtos_enter_critical();
		BK_DUMP_OUT("read addr:%x\r\n", addr);
		rtos_exit_critical(int_level);
		for (i = 0; i < 16; i++) {
			for (j = 0; j < 16; j++)
				BK_DUMP_OUT("%02x ", buf[i * 16 + j]);
			BK_DUMP_OUT("\r\n");
		}
	}

	return kNoErr;
}

static bk_err_t test_flash_read_without_print(volatile uint32_t start_addr, uint32_t len)
{
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	tmp = addr + length;

	for (; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
	}

	return kNoErr;
}

static bk_err_t test_flash_read_time(volatile uint32_t start_addr, uint32_t len)
{
	UINT32 time_start, time_end;
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;

	tmp = addr + length;
	beken_time_get_time((beken_time_t *)&time_start);
	BK_DUMP_OUT("read time start:%d\r\n", time_start);

	for (; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
	}
	beken_time_get_time((beken_time_t *)&time_end);
	BK_DUMP_OUT("read time end:%d\r\n", time_end);
	BK_DUMP_OUT("cost time:%d\r\n", time_end - time_start);

	return kNoErr;
}

#if (defined(CONFIG_SOC_BK7256XX))
static bk_err_t test_flash_count_time(volatile uint32_t start_addr, uint32_t len, uint32_t test_times)
{
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	extern u64 riscv_get_mtimer(void);
	uint64_t start_tick, end_tick, tick_cnt = 0;
	uint32_t print_cnt = 100;
	uint32_t int_level = 0;

	bk_wdt_stop();
#if (defined(CONFIG_TASK_WDT))
	bk_task_wdt_stop();
#endif

	tmp = addr + length;

	BK_DUMP_OUT("----- FLASH COUNT TIME TEST BEGIN -----\r\n");
	BK_DUMP_OUT("===============================\r\n");

for(int i = 0; i <= test_times; i++) {

	start_tick = riscv_get_mtimer();
	for (addr = start_addr; addr < tmp; addr += 256) {
		os_memset(buf, 0, 256);
		bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	if(i % print_cnt == 0) {
		int_level = rtos_enter_critical();
		BK_DUMP_OUT("[read %d time] >>>>> cost tick time: %d us.\r\n", i, (uint32_t) (tick_cnt / TICK_PER_US));
		rtos_exit_critical(int_level);
	}

	start_tick = riscv_get_mtimer();
	for (addr = start_addr; addr < tmp; addr += 0x1000) {
		bk_flash_erase_sector(addr);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	if(i % print_cnt == 0) {
		int_level = rtos_enter_critical();
		BK_DUMP_OUT("[erase %d time] >>>>> cost tick time: %d us.\r\n", i, (uint32_t) (tick_cnt / TICK_PER_US));
		rtos_exit_critical(int_level);
	}

	//check erase data valid
	if(i % print_cnt == 0) {
		for (addr = start_addr; addr < tmp; addr += 256) {
			os_memset(buf, 0, 256);
			bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
			for (int j = 0; j < 256; j++) {
				if(buf[j] != 0xff)
					BK_DUMP_OUT("[erase %d time ERROR]: addr = 0x%x.\r\n", i, addr + j);
			}
		}
	}

	for (int j = 0; j < 256; j++)
		buf[j] = j;
	start_tick = riscv_get_mtimer();
	for (addr = start_addr; addr < tmp; addr += 256) {
		bk_flash_write_bytes(addr, (uint8_t *)buf, 256);
	}
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	if(i % print_cnt == 0) {
		int_level = rtos_enter_critical();
		BK_DUMP_OUT("[write %d time] >>>>> cost tick time: %d us.\r\n", i, (uint32_t) (tick_cnt / TICK_PER_US));
		rtos_exit_critical(int_level);
	}

	//check write data valid
	if(i % print_cnt == 0) {
		for (addr = start_addr; addr < tmp; addr += 256) {
			os_memset(buf, 0, 256);
			bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
			for (int j = 0; j < 256; j++) {
				if(buf[j] != j)
					BK_DUMP_OUT("[write %d time ERROR]: addr = 0x%x.\r\n", i, addr);
			}
		}
	}

	start_tick = riscv_get_mtimer();
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	if(i % print_cnt == 0) {
		int_level = rtos_enter_critical();
		BK_DUMP_OUT("[enable security %d time] >>>>> cost tick time: %d us.\r\n", i, (uint32_t)(tick_cnt / TICK_PER_US));
		BK_DUMP_OUT("===============================\r\n");
		rtos_exit_critical(int_level);
	}
}

	start_tick = riscv_get_mtimer();
	end_tick = riscv_get_mtimer();
	tick_cnt = end_tick - start_tick;
	int_level = rtos_enter_critical();
	BK_DUMP_OUT("[en/dis security 0 time] >>>>> cost tick time: %d us.\r\n", (uint32_t) ((tick_cnt / TICK_PER_US)));

	BK_DUMP_OUT("----- FLASH COUNT TIME TEST END -----\r\n");
	rtos_exit_critical(int_level);

	return kNoErr;
}
#else
static bk_err_t test_flash_count_time(volatile uint32_t start_addr, uint32_t len, uint32_t test_times)
{
	uint32_t tmp;
	u8 buf[256];
	uint32_t addr = start_addr;
	uint32_t length = len;
	int32_t tick_cnt = 0;
	uint32_t print_cnt = 100;
	struct timeval rtc_start_time = {0, 0};
	struct timeval rtc_end_time = {0, 0};
	void test_flash_set_protect_type_none(void);
	void test_flash_set_protect_type_all(void);

	bk_wdt_stop();
#if (defined(CONFIG_TASK_WDT))
	bk_task_wdt_stop();
#endif
	tmp = addr + length;

	BK_DUMP_OUT("----- FLASH COUNT TIME TEST BEGIN -----\r\n");
	BK_DUMP_OUT("===============================\r\n");

	for(int i = 0; i <= test_times; i++) {
		bk_rtc_gettimeofday(&rtc_start_time, 0);
		for (addr = start_addr; addr < tmp; addr += 256) {
			os_memset(buf, 0, 256);
			bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
		}
		bk_rtc_gettimeofday(&rtc_end_time, 0);
		tick_cnt = 1000000 * (rtc_end_time.tv_sec - rtc_start_time.tv_sec) + rtc_end_time.tv_usec - rtc_start_time.tv_usec;
		if(i % print_cnt == 0)
			BK_DUMP_OUT("[read %d time] >>>>> cost time: %d us.\r\n", i, tick_cnt);
	
	
		bk_rtc_gettimeofday(&rtc_start_time, 0);
		for (addr = start_addr; addr < tmp; addr += 0x1000) {
			bk_flash_erase_sector(addr);
		}
		bk_rtc_gettimeofday(&rtc_end_time, 0);
		tick_cnt = 1000000 * (rtc_end_time.tv_sec - rtc_start_time.tv_sec) + rtc_end_time.tv_usec - rtc_start_time.tv_usec;
		if(i % print_cnt == 0)
			BK_DUMP_OUT("[erase %d time] >>>>> cost time: %d us.\r\n", i, tick_cnt);
	
		//check erase data valid
		if(i % print_cnt == 0) {
			for (addr = start_addr; addr < tmp; addr += 256) {
				os_memset(buf, 0, 256);
				bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
				for (int j = 0; j < 256; j++) {
					if(buf[j] != 0xff)
						BK_DUMP_OUT("[erase %d time ERROR]: addr = 0x%x.\r\n", i, addr + j);
				}
			}
		}
	
	
		for (int j = 0; j < 256; j++)
			buf[j] = j;
	
		bk_rtc_gettimeofday(&rtc_start_time, 0);
		for (addr = start_addr; addr < tmp; addr += 256) {
			bk_flash_write_bytes(addr, (uint8_t *)buf, 256);
		}
		bk_rtc_gettimeofday(&rtc_end_time, 0);
		tick_cnt = 1000000 * (rtc_end_time.tv_sec - rtc_start_time.tv_sec) + rtc_end_time.tv_usec - rtc_start_time.tv_usec;
		if(i % print_cnt == 0)
			BK_DUMP_OUT("[write %d time] >>>>> cost time: %d us.\r\n", i, tick_cnt);
	
		//check write data valid
		if(i % print_cnt == 0) {
			for (addr = start_addr; addr < tmp; addr += 256) {
				os_memset(buf, 0, 256);
				bk_flash_read_bytes(addr, (uint8_t *)buf, 256);
				for (int j = 0; j < 256; j++) {
					if(buf[j] != j)
						BK_DUMP_OUT("[write %d time ERROR]: addr = 0x%x.\r\n", i, addr);
				}
			}
		}
	
	
		bk_rtc_gettimeofday(&rtc_start_time, 0);
		test_flash_set_protect_type_none();
		test_flash_set_protect_type_all();
		bk_rtc_gettimeofday(&rtc_end_time, 0);
		tick_cnt = 1000000 * (rtc_end_time.tv_sec - rtc_start_time.tv_sec) + rtc_end_time.tv_usec - rtc_start_time.tv_usec;
		if(i % print_cnt == 0) {
			BK_DUMP_OUT("[en/dis protect type %d time] >>>>> cost time: %d us.\r\n", i, tick_cnt);
			BK_DUMP_OUT("===============================\r\n");
		}
	}

	bk_rtc_gettimeofday(&rtc_start_time, 0);
	test_flash_set_protect_type_none();
	test_flash_set_protect_type_all();
	bk_rtc_gettimeofday(&rtc_end_time, 0);
	tick_cnt = 1000000 * (rtc_end_time.tv_sec - rtc_start_time.tv_sec) + rtc_end_time.tv_usec - rtc_start_time.tv_usec;
	BK_DUMP_OUT("[en/dis protect type 0 time] >>>>> cost time: %d us.\r\n", tick_cnt);

	BK_DUMP_OUT("----- FLASH COUNT TIME TEST END -----\r\n");

	return kNoErr;
}
#endif

static void test_idle_read_flash(void *arg) {
	while (1) {
		test_flash_read_without_print(0x1000, 1000);
		test_flash_read_without_print(0x100000, 1000);
		test_flash_read_without_print(0x200000, 1000);
		test_flash_read_without_print(0x300000, 0x1000);
	}
	rtos_delete_thread(&idle_read_flash_handle);
}

#define write_data(addr,val)                    *((volatile unsigned long *)(addr)) = val
#define read_data(addr,val)                     val = *((volatile unsigned long *)(addr))
#define get_addr_data(addr)                     *((volatile unsigned long *)(addr))

#if (defined(CONFIG_PSRAM_AS_SYS_MEMORY))
bool s_is_cacheable = false;
bool s_task_stop = false;
#define PSRAM_DATA_ADDR     0x60000000
static void test_idle_read_psram(void *arg) {
	uint32_t i, val = 0;
	uint32_t buf_len = 512;

	BK_DUMP_OUT("enter test_idle_read_psram\r\n");

	uint32_t *test_data = (uint32_t *)psram_malloc(buf_len);
	if (test_data == NULL) {
		CLI_LOGE("test_data buffer malloc failed\r\n");
		return;
	}

	while (1) {
		if(s_is_cacheable == false) {
			for(i=0;i<1024;i++){
				write_data((PSRAM_DATA_ADDR+i*0x4),0x11+i);
				write_data((PSRAM_DATA_ADDR + 0x1000 +i*0x4),0x22+i);
				write_data((PSRAM_DATA_ADDR + 0x2000 +i*0x4),0x33+i);
				write_data((PSRAM_DATA_ADDR + 0x3000 +i*0x4),0x44+i);
			}
			for(i=0;i<1024;i++){
				write_data((PSRAM_DATA_ADDR + 0x4000 +i*0x4),0x55+i);
				write_data((PSRAM_DATA_ADDR + 0x5000 +i*0x4),0x66+i);
				write_data((PSRAM_DATA_ADDR + 0x6000 +i*0x4),0x77+i);
				write_data((PSRAM_DATA_ADDR + 0x7000 +i*0x4),0x88+i);
			}
			for(i=0;i<4*1024;i++){
				get_addr_data(PSRAM_DATA_ADDR+i*0x4);
			}
			for(i=0;i<4*1024;i++){
				get_addr_data(PSRAM_DATA_ADDR + 0x4000 +i*0x4);
			}
			for(i=0;i<4*1024;i++){
				get_addr_data(PSRAM_DATA_ADDR + i*0x4);
			}
			for(i=0;i<4*1024;i++){
				get_addr_data(PSRAM_DATA_ADDR + 0x4000 +i*0x4);
			}

			if(s_task_stop == true) {
				BK_DUMP_OUT("exit test_idle_read_psram non-cacheable.\r\n");
				break;
			}
		} else {
			for (i = 0; i < buf_len / 4; i++) {
				test_data[i] = 0x11223344 + i;
			}
			for (i = 0; i < buf_len / 4; i++) {
				val = test_data[i];
			}
			if(s_task_stop == true) {
				BK_DUMP_OUT("exit test_idle_read_psram cacheable, val = 0x%x\r\n", val);
				break;
			}
		}
	}

	if (test_data) {
		os_free(test_data);
	}
	test_data = NULL;

	if (idle_read_psram_handle) {
		BK_DUMP_OUT("=====idle_read_psram task stop end========\n");
		rtos_delete_thread(&idle_read_psram_handle);
		idle_read_psram_handle = NULL;
	}
}
#endif

static bk_err_t test_flash_ewr_verify(uint32_t start_addr, uint32_t size,
	uint32_t test_times, uint32_t erase_yield_ms, uint32_t write_yield_ms)
{
	uint32_t addr, end_addr;
	uint32_t err_cnt = 0;
	uint32_t i, j;
	u8 wr_buf[256];
	u8 rd_buf[256];

	if (start_addr & 0xFFF) {
		BK_DUMP_OUT("ERROR: start_addr 0x%x is not 4K aligned!\r\n", start_addr);
		return kGeneralErr;
	}
	if (size & 0xFFF) {
		BK_DUMP_OUT("ERROR: size 0x%x is not 4K aligned!\r\n", size);
		return kGeneralErr;
	}

	end_addr = start_addr + size;
	uint32_t erase_sectors = size >> 12;
	uint32_t write_blocks = size >> 8;

	bk_wdt_stop();
#if (CONFIG_TASK_WDT)
	bk_task_wdt_stop();
#endif


	BK_DUMP_OUT("===== FLASH EWR VERIFY START =====\r\n");
	BK_DUMP_OUT("  start_addr : 0x%x\r\n", start_addr);
	BK_DUMP_OUT("  end_addr   : 0x%x\r\n", end_addr);
	BK_DUMP_OUT("  size       : 0x%x (%d bytes)\r\n", size, size);
	BK_DUMP_OUT("  times      : %d\r\n", test_times);
	BK_DUMP_OUT("  erase_yield: %d ms (after each 4K sector erase)\r\n", erase_yield_ms);
	BK_DUMP_OUT("  write_yield: %d ms (after each 256B block write)\r\n", write_yield_ms);
	BK_DUMP_OUT("  per round  : erase %d sectors(4K), write %d blocks(256B)\r\n",
		erase_sectors, write_blocks);
	BK_DUMP_OUT("==================================\r\n");

	for (i = 0; i < test_times; i++) {
		uint8_t pattern = (uint8_t)(i & 0xFF);
		int verbose = ((i % 100) == 0);

		if (verbose)
			BK_DUMP_OUT("[round %d/%d] erasing 0x%x ~ 0x%x, %d sectors ...\r\n",
				i, test_times, start_addr, end_addr, erase_sectors);

		for (addr = start_addr; addr < end_addr; addr += 0x1000) {
			bk_flash_erase_sector(addr);
			if (erase_yield_ms)
				rtos_delay_milliseconds(erase_yield_ms);
		}

		if (verbose)
			BK_DUMP_OUT("[round %d/%d] read & compare erase data (expect 0xFF) ...\r\n",
				i, test_times);

		for (addr = start_addr; addr < end_addr; addr += 256) {
			os_memset(rd_buf, 0, 256);
			bk_flash_read_bytes(addr, rd_buf, 256);
			for (j = 0; j < 256; j++) {
				if (rd_buf[j] != 0xFF) {
					err_cnt++;
					BK_DUMP_OUT("[ERR][%d] erase verify fail @ 0x%x: expect 0xFF, got 0x%02x\r\n",
						i, addr + j, rd_buf[j]);
					goto next_round;
				}
			}
		}

		if (verbose)
			BK_DUMP_OUT("[round %d/%d] writing 0x%x ~ 0x%x, pattern 0x%02x ...\r\n",
				i, test_times, start_addr, end_addr, pattern);

		for (j = 0; j < 256; j++)
			wr_buf[j] = (uint8_t)((pattern + j) & 0xFF);

		for (addr = start_addr; addr < end_addr; addr += 256) {
			bk_flash_write_bytes(addr, wr_buf, 256);
			if (write_yield_ms)
				rtos_delay_milliseconds(write_yield_ms);
		}

		if (verbose)
			BK_DUMP_OUT("[round %d/%d] read & compare write data (pattern 0x%02x) ...\r\n",
				i, test_times, pattern);

		for (addr = start_addr; addr < end_addr; addr += 256) {
			os_memset(rd_buf, 0, 256);
			bk_flash_read_bytes(addr, rd_buf, 256);
			for (j = 0; j < 256; j++) {
				uint8_t expect = (uint8_t)((pattern + j) & 0xFF);
				if (rd_buf[j] != expect) {
					err_cnt++;
					BK_DUMP_OUT("[ERR][%d] write verify fail @ 0x%x: expect 0x%02x, got 0x%02x\r\n",
						i, addr + j, expect, rd_buf[j]);
					goto next_round;
				}
			}
		}

next_round:
		if (verbose)
			BK_DUMP_OUT("[STATUS] round %d/%d done, errors: %d\r\n", i, test_times, err_cnt);
	}


	BK_DUMP_OUT("===== FLASH EWR VERIFY END =====\r\n");
	BK_DUMP_OUT("total rounds: %d, total errors: %d\r\n", test_times, err_cnt);

	return (err_cnt == 0) ? kNoErr : kGeneralErr;
}

typedef struct {
	uint32_t start;
	uint32_t size;
	uint32_t rounds;
	uint8_t tid;
	uint32_t *err_out;
} flash_mt_ewr_ctx_t;

static void flash_concurrent_ewr_worker(beken_thread_arg_t arg)
{
	flash_mt_ewr_ctx_t *c = (flash_mt_ewr_ctx_t *)arg;
	uint32_t err = 0;
	uint32_t end = c->start + c->size;
	uint8_t base_pat = (c->tid == 0) ? (uint8_t)0xA0 : (uint8_t)0x5A;
	u8 wr_buf[256];
	u8 rd_buf[256];
	uint32_t r, addr;
	int j;

	for (r = 0; r < c->rounds; r++) {
		uint8_t pat = (uint8_t)((base_pat + (uint8_t)(r & 0xFF)) & 0xFF);

		for (addr = c->start; addr < end; addr += 0x1000)
			bk_flash_erase_sector(addr);

		for (addr = c->start; addr < end; addr += 256) {
			os_memset(rd_buf, 0, 256);
			bk_flash_read_bytes(addr, rd_buf, 256);
			for (j = 0; j < 256; j++) {
				if (rd_buf[j] != 0xFF) {
					err++;
					if (err <= 8)
						BK_DUMP_OUT("[MT T%u R%u] erase chk @0x%x exp FF got %02x\r\n",
							(unsigned)c->tid, (unsigned)r, addr + (uint32_t)j, rd_buf[j]);
				}
			}
		}

		for (j = 0; j < 256; j++)
			wr_buf[j] = (uint8_t)((pat + (uint8_t)j) & 0xFF);
		for (addr = c->start; addr < end; addr += 256)
			bk_flash_write_bytes(addr, wr_buf, 256);

		for (addr = c->start; addr < end; addr += 256) {
			os_memset(rd_buf, 0, 256);
			bk_flash_read_bytes(addr, rd_buf, 256);
			for (j = 0; j < 256; j++) {
				uint8_t exp = (uint8_t)((pat + (uint8_t)j) & 0xFF);
				if (rd_buf[j] != exp) {
					err++;
					if (err <= 8)
						BK_DUMP_OUT("[MT T%u R%u] write chk @0x%x exp %02x got %02x\r\n",
							(unsigned)c->tid, (unsigned)r, addr + (uint32_t)j, exp, rd_buf[j]);
				}
			}
		}

		if (((r + 1) % 100) == 0)
			BK_DUMP_OUT("[MT T%u] round %u/%u done, err=%u\r\n",
				(unsigned)c->tid, (unsigned)(r + 1), (unsigned)c->rounds, (unsigned)err);
	}

	*(c->err_out) = err;
	rtos_delete_thread(NULL);
}

static bk_err_t test_flash_concurrent_2task_verify(uint32_t start_addr, uint32_t total_size, uint32_t rounds)
{
	beken_thread_t th0 = NULL;
	beken_thread_t th1 = NULL;
	flash_mt_ewr_ctx_t ctx0, ctx1;
	uint32_t err0 = 0, err1 = 0;
	uint32_t half;
	bk_err_t ret;

	if (rounds == 0) {
		BK_DUMP_OUT("MT: rounds must be > 0\r\n");
		return kParamErr;
	}
	if (start_addr & 0xFFF) {
		BK_DUMP_OUT("MT: start 0x%x must be 4K aligned\r\n", start_addr);
		return kGeneralErr;
	}
	if (total_size < 0x2000 || (total_size & 0xFFF)) {
		BK_DUMP_OUT("MT: total_size 0x%x need >= 8KB and 4K aligned\r\n", total_size);
		return kGeneralErr;
	}
	half = total_size / 2;
	if (half & 0xFFF) {
		BK_DUMP_OUT("MT: total_size/2 must be 4K aligned (even number of 4K sectors)\r\n");
		return kGeneralErr;
	}

	bk_wdt_stop();
#if (CONFIG_TASK_WDT)
	bk_task_wdt_stop();
#endif

	BK_DUMP_OUT("===== FLASH 2-thread concurrent EWR verify =====\r\n");
	BK_DUMP_OUT("T0: 0x%x ~ 0x%x | T1: 0x%x ~ 0x%x | rounds %u\r\n",
		start_addr, start_addr + half - 1,
		start_addr + half, start_addr + total_size - 1, rounds);

	ctx0.start = start_addr;
	ctx0.size = half;
	ctx0.rounds = rounds;
	ctx0.tid = 0;
	ctx0.err_out = &err0;

	ctx1.start = start_addr + half;
	ctx1.size = half;
	ctx1.rounds = rounds;
	ctx1.tid = 1;
	ctx1.err_out = &err1;

	ret = rtos_create_thread(&th0, 6, "flash_mt0",
		(beken_thread_function_t)flash_concurrent_ewr_worker,
		CONFIG_APP_MAIN_TASK_STACK_SIZE, (beken_thread_arg_t)&ctx0);
	if (ret != kNoErr) {
		BK_DUMP_OUT("MT: create flash_mt0 fail %d\r\n", ret);
		return ret;
	}
	ret = rtos_create_thread(&th1, 6, "flash_mt1",
		(beken_thread_function_t)flash_concurrent_ewr_worker,
		CONFIG_APP_MAIN_TASK_STACK_SIZE, (beken_thread_arg_t)&ctx1);
	if (ret != kNoErr) {
		BK_DUMP_OUT("MT: create flash_mt1 fail %d, join T0\r\n", ret);
		rtos_thread_join(&th0);
		return ret;
	}

	rtos_thread_join(&th0);
	rtos_thread_join(&th1);

	BK_DUMP_OUT("===== FLASH 2-thread concurrent EWR end =====\r\n");
	BK_DUMP_OUT("T0 mismatches: %u, T1 mismatches: %u\r\n", err0, err1);
	if (err0 == 0 && err1 == 0)
		BK_DUMP_OUT("====== FLASH 2-thread concurrent EWR test PASS ======\r\n");
	else
		BK_DUMP_OUT("====== FLASH 2-thread concurrent EWR test FAIL ======\r\n");

	return (err0 == 0 && err1 == 0) ? kNoErr : kGeneralErr;
}

static void flash_command_test(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *msg = NULL;
	char cmd = 0;
	uint32_t len = 0;
	uint32_t addr = 0;

#if (defined(CONFIG_SYSTEM_CTRL))
	if (os_strcmp(argv[1], "config") == 0) {
		uint32_t flash_src_clk = os_strtoul(argv[2], NULL, 10);
		uint32_t flash_div_clk = os_strtoul(argv[3], NULL, 10);
		uint32_t flash_line_mode = os_strtoul(argv[4], NULL, 10);

		if (FLASH_CLK_XTAL == flash_src_clk) {
			sys_drv_flash_cksel(flash_src_clk);
		}

		if((FLASH_CLK_XTAL != sys_drv_flash_get_clk_sel()) && (0 == flash_div_clk)) {
			BK_DUMP_OUT("Config fail. Please set src clk as 26M, or set div larger than 0 firstly.\n");
			return;
		}

		sys_drv_flash_set_clk_div(flash_div_clk);
		sys_drv_flash_cksel(flash_src_clk);
		bk_flash_set_line_mode(flash_line_mode);
		BK_DUMP_OUT("flash_src_clk = %u. [0:/26M,  1:/480M,  2:/98M]\n", flash_src_clk);
		BK_DUMP_OUT("flash_div_clk = %u. [0:/4,  1:/6,  2:/8,  3:/10]\n", flash_div_clk);
		BK_DUMP_OUT("flash_line_mode = %u.  \n", flash_line_mode);

		return;
	}

//used for set flash status in itcm, not used now
#if 0
	if (os_strcmp(argv[1], "qe") == 0) {
		uint32_t param = 0;
		uint32_t quad_enable = os_strtoul(argv[2], NULL, 10);
		uint32_t delay_cycle1 = os_strtoul(argv[3], NULL, 10);
		uint32_t delay_cycle2 = os_strtoul(argv[4], NULL, 10);

		for(int i = 0; i< 20; i++) {
			bk_flash_set_line_mode(2);
			flash_bypass_quad_test(quad_enable, delay_cycle1, delay_cycle2);
			while (REG_READ(REG_FLASH_OPERATE_SW) & BUSY_SW);
			param = bk_flash_read_status_reg();
			if (quad_enable) {
				if(param & 0x200){
					break;
				} else {
					BK_DUMP_OUT("retry quad test, i = %d, flash status: 0x%x.\n", i, param);
				}
			} else {
				if(param & 0x200){
					BK_DUMP_OUT("retry quad test, i = %d, flash status: 0x%x.\n", i, param);
				} else {
					break;
				}
			}
		}

		if (quad_enable) {
			if(param & 0x200){
				BK_DUMP_OUT("flash quad enable success, flash status: 0x%x.\n", param);
			} else {
				BK_DUMP_OUT("flash quad enable fail, flash status: 0x%x.\n", param);
			}
		} else {
			if(param & 0x200){
				BK_DUMP_OUT("flash quad disable fail, flash status: 0x%x.\n", param);
			} else {
				BK_DUMP_OUT("flash quad disable success, flash status: 0x%x.\n", param);
			}
		}
		return;
	}
#endif
#endif
	if (os_strcmp(argv[1], "idle_read_start") == 0) {
		uint32_t task_prio = os_strtoul(argv[2], NULL, 10);
		BK_DUMP_OUT("idle_read_flash task start: task_prio = %u.\n", task_prio);
		rtos_create_thread(&idle_read_flash_handle, task_prio,
			"idle_read_flash",
			(beken_thread_function_t) test_idle_read_flash,
			CONFIG_APP_MAIN_TASK_STACK_SIZE,
			(beken_thread_arg_t)0);

		return;
	} else if (os_strcmp(argv[1], "idle_read_stop") == 0) {
		if (idle_read_flash_handle) {
			rtos_delete_thread(&idle_read_flash_handle);
			idle_read_flash_handle = NULL;
			BK_DUMP_OUT("idle_read_flash task stop\n");
		} 
		return;
	}

#if (defined(CONFIG_PSRAM_AS_SYS_MEMORY))
	if (os_strcmp(argv[1], "idle_read_psram_start") == 0) {
		s_task_stop = false;
		uint32_t task_prio = os_strtoul(argv[2], NULL, 10);
		BK_DUMP_OUT("idle_read_psram task start: task_prio = %u.\n", task_prio);
		rtos_create_thread(&idle_read_psram_handle, task_prio,
			"idle_read_psram",
			(beken_thread_function_t) test_idle_read_psram,
			CONFIG_APP_MAIN_TASK_STACK_SIZE,
			(beken_thread_arg_t)0);

		return;
	} else if (os_strcmp(argv[1], "idle_read_psram_stop") == 0) {
		s_task_stop = true;
		BK_DUMP_OUT("=====idle_read_psram task stop begin========\n");
		return;
	} else if (os_strcmp(argv[1], "psram_cache") == 0) {
		uint32_t psram_cache_flag = os_strtoul(argv[2], NULL, 10);
		if(psram_cache_flag == 1){
			s_is_cacheable = true;
			BK_DUMP_OUT("idle_read_psram switch to cacheable.\n");
		} else {
			s_is_cacheable = false;
			BK_DUMP_OUT("idle_read_psram switch to non-cacheable.\n");
		}
		return;
	}
#endif

	if (os_strcmp(argv[1], "U") == 0) {
		return;
	} else if (os_strcmp(argv[1], "P") == 0) {
		return;
	} else if (os_strcmp(argv[1], "RSR") == 0) {
		uint16_t sts_val = bk_flash_read_status_reg();
		BK_DUMP_OUT("read sts_val = 0x%x\n", sts_val);
		return;
	} else if (os_strcmp(argv[1], "WSR") == 0) {
		uint16_t sts_val = os_strtoul(argv[2], NULL, 16);
		bk_flash_write_status_reg(sts_val);
		return;
	} else if (os_strcmp(argv[1], "C") == 0) {
		addr = os_strtoul(argv[2], NULL, 16);
		len = os_strtoul(argv[3], NULL, 16);
		uint32_t test_times = os_strtoul(argv[4], NULL, 10);
		test_flash_count_time(addr, len, test_times);
		return;
	} else if (os_strcmp(argv[1], "V") == 0) {
		if (argc < 5) {
			BK_DUMP_OUT("Usage: flash_test V <addr> <size> <times> [erase_yield_ms] [write_yield_ms]\r\n");
			BK_DUMP_OUT("  erase_yield_ms: delay(ms) after each sector erase, default 0\r\n");
			BK_DUMP_OUT("  write_yield_ms: delay(ms) after each block write, default 0\r\n");
			BK_DUMP_OUT("  WiFi coexist suggest: erase_yield=100 write_yield=2\r\n");
			return;
		}
		addr = os_strtoul(argv[2], NULL, 16);
		len = os_strtoul(argv[3], NULL, 16);
		uint32_t test_times = os_strtoul(argv[4], NULL, 10);
		uint32_t erase_yield = (argc >= 6) ? os_strtoul(argv[5], NULL, 10) : 0;
		uint32_t write_yield = (argc >= 7) ? os_strtoul(argv[6], NULL, 10) : 0;
		test_flash_ewr_verify(addr, len, test_times, erase_yield, write_yield);
		return;
	} else if (os_strcmp(argv[1], "MT") == 0) {
		if (argc < 5) {
			BK_DUMP_OUT("Usage: flash_test MT <start> <total_size> <rounds>\r\n");
			BK_DUMP_OUT("  Two threads concurrently erase/read/write on two disjoint halves.\r\n");
			BK_DUMP_OUT("  start: 4K aligned; total_size: >=8KB, 4K aligned, each half >= one 4K sector.\r\n");
			return;
		}
		addr = os_strtoul(argv[2], NULL, 16);
		len = os_strtoul(argv[3], NULL, 16);
		uint32_t mt_rounds = os_strtoul(argv[4], NULL, 10);
		test_flash_concurrent_2task_verify(addr, len, mt_rounds);
		return;
	}

	if (argc == 4) {
		cmd = argv[1][0];
		addr = os_strtoul(argv[2], NULL, 16);
		len = os_strtoul(argv[3], NULL, 16);

		switch (cmd) {
		case 'E':
			test_flash_erase(addr, len);
			msg = CLI_CMD_RSP_SUCCEED;
			break;

		case 'R':
			test_flash_read(addr, len);
			msg = CLI_CMD_RSP_SUCCEED;
			break;
		case 'W':
			test_flash_write(addr, len);
			msg = CLI_CMD_RSP_SUCCEED;
			break;
		//to check whether protection mechanism can work
		case 'N':
			test_flash_erase(addr, len);
			msg = CLI_CMD_RSP_SUCCEED;
			break;
		case 'M':
			test_flash_write(addr, len);
			msg = CLI_CMD_RSP_SUCCEED;
			break;
		case 'T':
			test_flash_read_time(addr, len);
			msg = CLI_CMD_RSP_SUCCEED;
			break;
		default:
		BK_DUMP_OUT("flash_test <R/W/E/M/N/T> <start_addr> <len>\r\n");
		BK_DUMP_OUT("flash_test V <addr> <size> <times> [erase_yield] [write_yield]\r\n");
		BK_DUMP_OUT("flash_test MT <start> <total_size> <rounds>\r\n");
		msg = CLI_CMD_RSP_ERROR;
		break;
		}
	} else {
		BK_DUMP_OUT("flash_test <R/W/E/M/N/T> <start_addr> <len>\r\n");
		BK_DUMP_OUT("flash_test V <addr> <size> <times> [erase_yield] [write_yield]\r\n");
		BK_DUMP_OUT("flash_test MT <start> <total_size> <rounds>\r\n");
		msg = CLI_CMD_RSP_ERROR;
	}
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

static void partShow_Command(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	bk_partition_t i;
	bk_logic_partition_t *partition;

	for (i = BK_PARTITION_BOOTLOADER; i < BK_PARTITION_MAX; i++) {
		partition = bk_flash_partition_get_info(i);
		if (partition == NULL)
			continue;

		BK_DUMP_OUT("%4d | %11s |  Dev:%d  | 0x%08lx | 0x%08lx |\r\n", i,
				  partition->partition_description, partition->partition_owner,
				  partition->partition_start_addr, partition->partition_length);
	};

}

#define FLASH_CMD_CNT (sizeof(s_flash_commands) / sizeof(struct cli_command))
static const struct cli_command s_flash_commands[] = {
	{"fmap_test",    "flash_test memory map",      partShow_Command},
	{"flash_test",   "flash_test <cmd(R/W/E/N/V/MT)>", flash_command_test},
};

int cli_flash_test_init(void)
{
	return cli_register_commands(s_flash_commands, FLASH_CMD_CNT);
}
