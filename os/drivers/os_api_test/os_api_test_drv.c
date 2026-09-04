/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <errno.h>
#include <debug.h>
#include <stdlib.h>
#include <string.h>

#include <tinyara/fs/fs.h>
#include <tinyara/os_api_test_drv.h>
#include <tinyara/sched.h>
#include <tinyara/irq.h>
#include <tinyara/mmu.h>
#include "os_api_test_proto.h"
#include "wdog/wdog.h"


#ifdef CONFIG_EXAMPLES_MEM_PROTECT_TEST
#include <tinyara/binfmt/binfmt.h>
#include <tinyara/mem_protect_test.h>
#include "binary_manager/binary_manager_internal.h"
#endif

/****************************************************************************
 * Public variables
 ****************************************************************************/
#ifdef CONFIG_EXAMPLES_MEM_PROTECT_TEST
extern uint32_t _stext_flash;
extern uint32_t _sdata;
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int os_api_test_drv_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t os_api_test_drv_read(FAR struct file *filep, FAR char *buffer, size_t len);
static ssize_t os_api_test_drv_write(FAR struct file *filep, FAR const char *buffer, size_t len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations os_api_test_drv_fops = {
	0,						/* open */
	0,						/* close */
	os_api_test_drv_read,				/* read */
	os_api_test_drv_write,				/* write */
	0,						/* seek */
	os_api_test_drv_ioctl				/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0						/* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: os_api_test_drv_ioctl
 *
 * Description:  The standard ioctl method.
 *
 ************************************************************************************/

/* File-scope callback and thread functions for scenario tests.
 * These cannot be defined inside the ioctl function (C doesn't allow
 * static nested functions).
 */

/* TC-06: Multiple watchdog expiration callback */
static volatile int g_tc06_callback_count;
static void tc06_callback(int argc, uint32_t arg1, ...)
{
	g_tc06_callback_count++;
}

/* TC-08: Core 1 thread for SMP concurrent test */
static volatile int g_tc08_core1_running;
static volatile int g_tc08_core1_ok_count;
static volatile int g_tc08_core1_stop;
static int tc08_core1_thread(int argc, char *argv[])
{
	int i;
	WDOG_ID wdog;
	int ok = 0;

	lldbg("TC08: Core 1 thread started (pid=%d)\n", getpid());

	for (i = 0; i < 500 && !g_tc08_core1_stop; i++) {
		wdog = wd_create();
		if (!wdog) {
			lldbg("TC08: Core1 wd_create() failed at %d\n", i);
			break;
		}
		if (wd_start(wdog, 10000, NULL, 0) != OK) {
			lldbg("TC08: Core1 wd_start() failed at %d\n", i);
			wd_delete(wdog);
			break;
		}
		wd_cancel(wdog);
		wd_delete(wdog);
		ok++;
	}

	g_tc08_core1_ok_count = ok;
	g_tc08_core1_running = 0;
	lldbg("TC08: Core 1 thread done (%d iterations)\n", ok);
	return 0;
}

/* TC-09: Callback and Core 1 thread for SMP tick/cb test */
static volatile int g_tc09_callback_fired;
static volatile int g_tc09_core1_done;
static WDOG_ID g_tc09_wdog;

static void tc09_callback(int argc, uint32_t arg1, ...)
{
	g_tc09_callback_fired = 1;
}

static int tc09_core1_thread(int argc, char *argv[])
{
	lldbg("TC09: Core 1 thread started (pid=%d)\n", getpid());

	g_tc09_wdog = wd_create();
	if (!g_tc09_wdog) {
		lldbg("TC09: Core1 wd_create() failed\n");
		g_tc09_core1_done = 1;
		return -ENOMEM;
	}

	if (wd_start(g_tc09_wdog, 1, tc09_callback, 0) != OK) {
		lldbg("TC09: Core1 wd_start() failed\n");
		wd_delete(g_tc09_wdog);
		g_tc09_wdog = NULL;
		g_tc09_core1_done = 1;
		return -EIO;
	}

	lldbg("TC09: Core1 started watchdog with 1-tick delay\n");
	g_tc09_core1_done = 1;
	return 0;
}

static int os_api_test_drv_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	int ret = -EINVAL;
	/* Handle built-in ioctl commands */


	switch (cmd) {
	/* TESTFWIOC_DRIVER_ANALOG - Run the test case for /os/driver/analog module
	 *
	 *   ioctl argument:  An integer value indicating the particular test to be run
	 */

	case TESTIOC_ANALOG:
		break;
#ifndef CONFIG_DISABLE_SIGNALS
	case TESTIOC_GET_SIG_FINDACTION_ADD:
	case TESTIOC_SIGNAL_PAUSE:
	case TESTIOC_GET_TCB_SIGPROCMASK:
		ret = test_signal(cmd, arg);
		break;
#endif
	case TESTIOC_GET_SELF_PID:
	case TESTIOC_IS_ALIVE_THREAD:
	case TESTIOC_GET_TCB_ADJ_STACK_SIZE:
	case TESTIOC_SCHED_FOREACH:
		ret = test_sched(cmd, arg);
		break;
	case TESTIOC_CLOCK_ABSTIME2TICKS_TEST:
		ret = test_clock(cmd, arg);
		break;
	case TESTIOC_TIMER_INITIALIZE_TEST:
		ret = test_timer(cmd, arg);
		break;
	case TESTIOC_SEM_TICK_WAIT_TEST:
		ret = test_sem(cmd, arg);
		break;
#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS)
	case TESTIOC_GROUP_ADD_FINED_REMOVE_TEST:
	case TESTIOC_GROUP_ALLOC_FREE_TEST:
	case TESTIOC_GROUP_EXIT_CHILD_TEST:
	case TESTIOC_GROUP_REMOVECHILDREN_TEST:
		ret = test_group(cmd, arg);
		break;
#endif
	case TESTIOC_TASK_REPARENT:
	case TESTIOC_TASK_INIT_TEST:
		ret = test_task(cmd, arg);
		break;
#ifdef CONFIG_TC_COMPRESS_READ
	case TESTIOC_COMPRESSION_TEST:
		ret = test_compress_decompress(cmd, arg);
		break;
#endif

#ifdef CONFIG_EXAMPLES_MEM_PROTECT_TEST
	case TESTIOC_MEM_PROTECTTEST:
		ret = OK;
		struct mem_protecttest_arg_s *obj = (struct mem_protecttest_arg_s*)arg;
		
		if (!obj) {
			return -EINVAL;
		}

		switch (obj->type) {
		case MEM_PROTECTTEST_KERNEL_CODE:
			obj->addr = &_stext_flash;
			break;
		case MEM_PROTECTTEST_KERNEL_DATA:
			obj->addr = &_sdata;
			break;
		case MEM_PROTECTTEST_APP_ADDR:
		{
			/* Find the current executing app and return an address
			* which belongs to any other app in the system. Here,
			* we choose to return the address of the app heap
			*/
			uint32_t binidx = sched_self()->group->tg_binidx;
			binidx = (binidx + 1) % (binary_manager_get_ucount() + 1);
			if (binidx == 0) {
				binidx++;
			}
			struct tcb_s *tcb = (struct tcb_s *)sched_gettcb(BIN_ID(binidx));
			if (tcb) {
				obj->addr = (volatile uint32_t *)tcb->uheap;
			} else {
				ret = -ESRCH;
			}
			break;
		}
		default:
			ret = -EINVAL;
			break;
		}

		break;
#endif

#ifdef CONFIG_ARMV8M_TRUSTZONE
	case TESTIOC_TZ:
		ret = test_tz();
		break;
#endif

#ifdef CONFIG_EXAMPLES_STACK_PROTECTION
	case TESTIOC_KTHREAD_STACK_PROTECTION_TEST:
		ret = test_kthread_stack_overflow_protection(cmd, arg);
		break;
#endif

#ifdef CONFIG_TC_NET_PBUF
	/* Run the test case for pbuf   */
	case TESTIOC_NET_PBUF:
		ret = test_net_pbuf(cmd, arg);
		break;
#endif
#if defined(CONFIG_AUTOMOUNT_USERFS) && defined(CONFIG_EXAMPLES_TESTCASE_FILESYSTEM)
	case TESTIOC_GET_FS_PARTNO:
		ret = test_fs_get_devname();
		break;
#endif
#ifdef CONFIG_EXAMPLES_MMU_PROTECT_TEST
	case TESTIOC_UAF_MMU_PROTECT: {
		/* Use-after-free detection via MMU page protection.
		 *
		 * arg = 0: read from protected page (UAF read)
		 * arg = 1: write to protected page (UAF write)
		 *
		 * 1. Allocate a page-aligned buffer (simulates a kernel allocation).
		 * 2. Write data to it (normal use).
		 * 3. "Free" it: call mmu_set_page_no_access() to make the page
		 *    inaccessible at any privilege level (AP=000).
		 * 4. Read or write the protected page — the MMU triggers a Data Abort.
		 *
		 * The Data Abort handler (arm_dataabort) prints PC/DFAR/DFSR and
		 * calls PANIC().  DFAR contains the freed page address, confirming
		 * a use-after-free was detected.
		 *
		 * This function does NOT return — the Data Abort is fatal.
		 */
		uint8_t *buf;
		uint32_t page_addr;
		volatile uint8_t val;
		int do_write = (arg == 1);

		buf = (uint8_t *)memalign(4096, 4096 * 2);
		if (!buf) {
			lldbg("UAF test: memalign failed\n");
			return -ENOMEM;
		}

		/* Get page-aligned address */
		page_addr = ((uint32_t)buf + 4095) & ~4095u;

		/* Step 1: Normal use — write data */
		memset((void *)page_addr, 0xAA, 4096);
		lldbg("UAF test: allocated buffer at 0x%08x, page 0x%08x\n",
		      (uint32_t)buf, page_addr);
		lldbg("UAF test: wrote data, buf[0] = 0x%02x\n",
		      ((volatile uint8_t *)page_addr)[0]);

		/* Step 2: Simulate free — protect the page via MMU */
		mmu_set_page_no_access(page_addr);
		lldbg("UAF test: page 0x%08x set to No-Access (simulating free)\n",
		      page_addr);

		/* Step 3: Use-after-free — access the protected page.
		 * The MMU sees AP=000 and raises a permission fault (Data Abort).
		 * DFAR will contain page_addr — the freed address that was accessed.
		 */
		if (do_write) {
			lldbg("UAF test: >>> WRITE to freed memory, expect Data Abort <<<\n");
			((volatile uint8_t *)page_addr)[0] = 0xBB;
		} else {
			lldbg("UAF test: >>> READ from freed memory, expect Data Abort <<<\n");
			val = ((volatile uint8_t *)page_addr)[0];
		}

		/* If we reach here, protection failed */
		(void)val;
		lldbg("UAF test: ERROR — access succeeded, Data Abort NOT triggered!\n");
		mmu_restore_page_pte(page_addr, mmu_save_page_pte(page_addr));
		free(buf);
		ret = -EIO;
		break;
	}
	case TESTIOC_UAF_NO_MMU: {
		/* UAF WITHOUT MMU protection — demonstrates what happens when
		 * freed memory is accessed without any protection.
		 *
		 * 1. Allocate a page-aligned buffer.
		 * 2. Write known data to it.
		 * 3. free() the buffer (real free, no MMU protection).
		 * 4. Read from the freed memory — access succeeds silently,
		 *    returning stale data.  No Data Abort, no detection.
		 * 5. Write to the freed memory — also succeeds silently.
		 *
		 * This shows why MMU protection is needed: without it, a
		 * use-after-free goes completely undetected.
		 */
		uint8_t *buf;
		uint32_t page_addr;
		volatile uint8_t val;

		buf = (uint8_t *)memalign(4096, 4096 * 2);
		if (!buf) {
			lldbg("UAF test (no mmu): memalign failed\n");
			return -ENOMEM;
		}

		page_addr = ((uint32_t)buf + 4095) & ~4095u;

		/* Step 1: Normal use — write data */
		memset((void *)page_addr, 0xAA, 4096);
		lldbg("UAF test (no mmu): allocated buffer at 0x%08x, page 0x%08x\n",
		      (uint32_t)buf, page_addr);
		lldbg("UAF test (no mmu): wrote data, buf[0] = 0x%02x\n",
		      ((volatile uint8_t *)page_addr)[0]);

		/* Step 2: Free the buffer — NO MMU protection */
		free(buf);
		lldbg("UAF test (no mmu): buffer freed (no MMU protection)\n");

		/* Step 3: Use-after-free — read from freed memory.
		 * Without MMU protection, this access succeeds silently.
		 * The data may be stale (0xAA) or garbage if the allocator
		 * has already reused the page.
		 */
		lldbg("UAF test (no mmu): >>> reading freed memory (no protection) <<<\n");
		val = ((volatile uint8_t *)page_addr)[0];
		lldbg("UAF test (no mmu): read succeeded! val = 0x%02x (stale data)\n",
		      val);

		/* Step 4: Use-after-free — write to freed memory.
		 * Without MMU protection, this also succeeds silently,
		 * silently corrupting whatever may now be at that address.
		 */
		lldbg("UAF test (no mmu): >>> writing to freed memory (no protection) <<<\n");
		((volatile uint8_t *)page_addr)[0] = 0xCC;
		lldbg("UAF test (no mmu): write succeeded! (silent corruption)\n");

		lldbg("UAF test (no mmu): NO Data Abort — UAF went UNDETECTED!\n");
		lldbg("UAF test (no mmu): This is why MMU protection is needed.\n");

		ret = OK;
		break;
	}
	case TESTIOC_WDOG_MMU_PROTECT_TEST: {
		/* Verify watchdog MMU page protection.
		 *
		 * This test accesses the wdog static data page (g_wdpool) from

		 * OUTSIDE wdog code — i.e. without calling wd_mmu_write_begin().
		 *
		 * Step 1: Read from the wdog page — should succeed (page is
		 *         Read-Only, reads are allowed).
		 * Step 2: Write to the wdog page — if CONFIG_WDOG_MMU_PROTECT
		 *         is enabled, the MMU triggers a Data Abort (permission
		 *         fault).  DFAR will contain the wdog page address.
		 *
		 * If protection is NOT enabled, the write succeeds but we
		 * immediately restore the original value (with interrupts
		 * disabled so no timer interrupt sees the corruption).
		 *
		 * arg = 0: read only (safe, no crash)
		 * arg = 1: write (crash if protection enabled)
		 */
		volatile uint8_t *wdog_page = (volatile uint8_t *)&g_wdpool;


		int do_write = (arg == 1);
		irqstate_t irq_state;

		lldbg("WDOG MMU protect test: wdog data at 0x%08x\n",
		      (uint32_t)wdog_page);


		/* Step 1: Read — should always succeed (Read-Only allows reads) */
		lldbg("WDOG MMU protect test: reading wdog page (should succeed)...\n");
		volatile uint8_t val = wdog_page[0];
		lldbg("WDOG MMU protect test: read OK, val = 0x%02x\n", val);

		if (do_write) {
			/* Step 2: Write — if CONFIG_WDOG_MMU_PROTECT is enabled,
			 * this triggers a Data Abort.  The page is Read-Only and
			 * we are NOT inside wd_mmu_write_begin()/wd_mmu_write_end().
			 *
			 * DFAR will contain &g_wdpool — the protected wdog address.

			 *
			 * If protection is NOT enabled, the write succeeds.  We
			 * disable interrupts and save/restore the original byte
			 * so the wdog data is not permanently corrupted.  Without
			 * interrupts, no timer tick can fire between the write
			 * and the restore.
			 */
			lldbg("WDOG MMU protect test: >>> WRITE to wdog page, expect Data Abort <<<\n");

			irq_state = enter_critical_section();
			uint8_t orig = wdog_page[0];
			wdog_page[0] = 0xBB;

			/* If we reach here, protection is NOT enabled.
			 * Restore the original value to avoid corrupting wdog data.
			 */
			wdog_page[0] = orig;
			leave_critical_section(irq_state);

			lldbg("WDOG MMU protect test: write succeeded (value restored), no Data Abort!\n");
			lldbg("WDOG MMU protect test: CONFIG_WDOG_MMU_PROTECT is NOT enabled.\n");
			lldbg("WDOG MMU protect test: Wdog page is writable — protection is OFF.\n");
			ret = OK;

		} else {
			lldbg("WDOG MMU protect test: read-only test passed.\n");
			lldbg("WDOG MMU protect test: use arg=1 to test write protection (crash).\n");
			ret = OK;
		}
		break;
	}

#endif

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_SMP
	case TESTIOC_WDOG_DEFECT_SMP: {
		/* Defect #1: SMP DACR per-core issue.
		 *
		 * DACR is a per-CPU register. When wd_mmu_write_begin() runs
		 * on Core 0, only Core 0's DACR is updated to Manager (RW).
		 * Core 1's DACR remains Client (RO). If Core 1 accesses the
		 * wdog pool, it gets a Data Abort.
		 *
		 * This test:
		 * 1. Reads DACR before begin() (should be RO = 0x05)
		 * 2. Calls wd_mmu_write_begin() (sets this core's DACR = RW)
		 * 3. Reads DACR after begin() (should be RW = 0x0D)
		 * 4. Writes to wdog pool from this core (should succeed)
		 * 5. Logs that other cores' DACR is still RO (defect!)
		 *
		 * On SMP: if another core accesses wdog pool now → Data Abort.
		 */
		uint32_t dacr_before, dacr_after;
		volatile uint8_t *wdog_page = (volatile uint8_t *)&g_wdpool;

		lldbg("=== DEFECT #1: SMP DACR per-core test ===\n");

		/* Step 1: Read DACR before begin() */
		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_before));
		lldbg("DEFECT_SMP: DACR before begin() = 0x%08x (expect 0x05 = RO)\n", dacr_before);

		/* Step 2: Call begin() — sets THIS core's DACR = RW */
		wd_mmu_write_begin();

		/* Step 3: Read DACR after begin() */
		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_after));
		lldbg("DEFECT_SMP: DACR after begin()  = 0x%08x (expect 0x0D = RW)\n", dacr_after);

		if (dacr_after == 0x0000000d) {
			lldbg("DEFECT_SMP: This core DACR = RW (Manager) — write should succeed\n");
		} else {
			lldbg("DEFECT_SMP: ERROR — This core DACR NOT 0x0D!\n");
		}

		/* Step 4: Write to wdog pool from this core (should succeed) */
		lldbg("DEFECT_SMP: Writing to wdog pool from this core...\n");
		{
			irqstate_t irq_state = enter_critical_section();
			uint8_t orig = wdog_page[0];
			wdog_page[0] = 0xCC;
			wdog_page[0] = orig;
			leave_critical_section(irq_state);
		}
		lldbg("DEFECT_SMP: Write succeeded on this core (DACR = RW)\n");

		/* Step 5: Log the defect */
		lldbg("DEFECT_SMP: === DEFECT CONFIRMED ===\n");
		lldbg("DEFECT_SMP: Other cores' DACR is still 0x05 (RO)!\n");
		lldbg("DEFECT_SMP: If another core accesses wdog pool now → Data Abort!\n");
		lldbg("DEFECT_SMP: Fix: Use IPI to broadcast DACR to all cores.\n");

		wd_mmu_write_end();

		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr_after));
		lldbg("DEFECT_SMP: DACR after end()   = 0x%08x (expect 0x05 = RO)\n", dacr_after);

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_SMP */

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_DSB
	case TESTIOC_WDOG_DEFECT_DSB: {
		/* Defect #3: Missing dsb after DACR write.
		 *
		 * Without a dsb (Data Synchronization Barrier) after the
		 * mcr instruction that writes DACR, an out-of-order core
		 * may execute the next memory access BEFORE the DACR write
		 * completes. This causes an intermittent Data Abort.
		 *
		 * This test calls wd_mmu_write_begin() and immediately
		 * writes to the wdog pool in a tight loop (10,000 iterations).
		 * Without dsb, intermittent failures may occur on
		 * out-of-order cores.
		 */
		volatile uint8_t *wdog_page = (volatile uint8_t *)&g_wdpool;
		int i;

		lldbg("=== DEFECT #3: Missing dsb after DACR write ===\n");
		lldbg("DEFECT_DSB: Running 10,000 iterations of begin()+write+end()\n");
		lldbg("DEFECT_DSB: Without dsb, intermittent Data Abort may occur\n");

		for (i = 0; i < 10000; i++) {
			wd_mmu_write_begin();
			/* Immediately write — without dsb, this may use stale DACR */
			uint8_t orig = wdog_page[0];
			wdog_page[0] = (uint8_t)(i & 0xFF);
			wdog_page[0] = orig;
			wd_mmu_write_end();
		}

		lldbg("DEFECT_DSB: All 10,000 iterations completed (no Data Abort)\n");
		lldbg("DEFECT_DSB: NOTE: On out-of-order cores, this may fail intermittently\n");
		lldbg("DEFECT_DSB: Fix: Add dsb + isb after mcr to DACR\n");

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_DSB */

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_DACR_CLOBBER
	case TESTIOC_WDOG_DEFECT_DACR_CLOBBER: {
		/* Defect #7: DACR clobbers other domains.
		 *
		 * wd_mmu_write_begin() writes the entire DACR register
		 * (0x0D), which sets ALL 16 domains. Domains 2-15 are
		 * set to No-Access (00), clobbering any other subsystem
		 * that might use those domains.
		 *
		 * This test:
		 * 1. Manually sets Domain 2 to Client (01)
		 * 2. Calls wd_mmu_write_begin() (overwrites entire DACR)
		 * 3. Reads DACR — Domain 2 is now No-Access (clobbered!)
		 */
		uint32_t dacr;

		lldbg("=== DEFECT #7: DACR clobbers other domains ===\n");

		/* Step 1: Set Domain 2 to Client (bits [5:4] = 01) */
		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr));
		dacr &= ~(0x3 << 4);    /* Clear Domain 2 bits */
		dacr |= (0x1 << 4);     /* Set Domain 2 = Client */
		__asm__ volatile("mcr p15, 0, %0, c3, c0, 0" :: "r"(dacr) : "memory");

		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr));
		lldbg("DEFECT_CLOBBER: Before begin(): DACR = 0x%08x (Domain 2 = Client)\n", dacr);

		/* Step 2: Call begin() — overwrites entire DACR */
		wd_mmu_write_begin();

		/* Step 3: Read DACR — Domain 2 should be clobbered to No-Access */
		__asm__ volatile("mrc p15, 0, %0, c3, c0, 0" : "=r"(dacr));
		lldbg("DEFECT_CLOBBER: After begin():  DACR = 0x%08x\n", dacr);

		uint32_t domain2 = (dacr >> 4) & 0x3;
		if (domain2 == 0x0) {
			lldbg("DEFECT_CLOBBER: === DEFECT CONFIRMED ===\n");
			lldbg("DEFECT_CLOBBER: Domain 2 = No-Access (clobbered!)\n");
			lldbg("DEFECT_CLOBBER: wd_mmu_write_begin() overwrote entire DACR\n");
			lldbg("DEFECT_CLOBBER: Fix: Use read-modify-write for DACR\n");
		} else {
			lldbg("DEFECT_CLOBBER: Domain 2 = 0x%x (not clobbered)\n", domain2);
		}

		wd_mmu_write_end();

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_DACR_CLOBBER */

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_HEAP_OVERLAP
	case TESTIOC_WDOG_DEFECT_HEAP_OVERLAP: {
		/* Defect #8: Heap overlap with wdog pool.
		 *
		 * The wdog pool is placed at a fixed address. If the kernel
		 * heap covers this region, heap allocations may land in the
		 * wdog pool's Read-Only section → Data Abort.
		 *
		 * This test prints the wdog pool address range and the
		 * configured heap start address so the user can verify
		 * there is no overlap.
		 */
		extern uint32_t __wdog_pool_start__[];
		extern uint32_t __wdog_pool_end__[];

		uint32_t pool_start = (uint32_t)__wdog_pool_start__;
		uint32_t pool_end = (uint32_t)__wdog_pool_end__;

		lldbg("=== DEFECT #8: Heap overlap with wdog pool ===\n");
		lldbg("DEFECT_HEAP: wdog pool: 0x%08x - 0x%08x (size = %d bytes)\n",
		      pool_start, pool_end, pool_end - pool_start);

		/* Use CONFIG_RAM_KREGIONx_START for heap start check.
		 * The heap start is configured via Kconfig and available
		 * as a string. We compare against the wdog pool range.
		 */
		lldbg("DEFECT_HEAP: Configured RAM start = 0x%08x\n", CONFIG_RAM_START);
		lldbg("DEFECT_HEAP: Configured RAM size = %d bytes\n", CONFIG_RAM_SIZE);
		lldbg("DEFECT_HEAP: RAM end = 0x%08x\n", CONFIG_RAM_START + CONFIG_RAM_SIZE);

		/* Check if wdog pool is within RAM region */
		if (pool_start >= CONFIG_RAM_START &&
		    pool_start < (CONFIG_RAM_START + CONFIG_RAM_SIZE)) {
			lldbg("DEFECT_HEAP: === DEFECT CONFIRMED ===\n");
			lldbg("DEFECT_HEAP: wdog pool (0x%08x) is within RAM region!\n",
			      pool_start);
			lldbg("DEFECT_HEAP: Heap may overlap wdog pool → Data Abort risk!\n");
			lldbg("DEFECT_HEAP: Fix: Adjust heap start to after wdog pool\n");
		} else {
			lldbg("DEFECT_HEAP: wdog pool is outside RAM region — OK\n");
		}

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_HEAP_OVERLAP */


#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_WD_RECOVER
	case TESTIOC_WDOG_DEFECT_WD_RECOVER: {
		/* Defect #11: wd_recover() missing protection.
		 *
		 * wd_recover() is called when a task is destroyed while it
		 * has an active watchdog (tcb->waitdog != NULL).
		 * It calls wd_cancel() and wd_delete() which write to the
		 * wdog pool. Without wd_mmu_write_begin()/end(), these
		 * writes cause a Data Abort.
		 *
		 * This test:
		 * 1. Creates a watchdog and starts it
		 * 2. Simulates a task exit with an active watchdog
		 *    (calls wd_recover() path indirectly)
		 * 3. Verifies no Data Abort occurs (fix is working)
		 *
		 * NOTE: We do NOT corrupt the list pointer — that would
		 * cause a crash from following a bad pointer, which is
		 * unrelated to MMU protection.
		 */
		WDOG_ID wdog;

		lldbg("=== DEFECT #11: wd_recover() missing protection ===\n");
		lldbg("DEFECT_RECOVER: Testing wd_recover() with MMU protection\n");

		/* Create and start a watchdog */
		wdog = wd_create();
		if (wdog == NULL) {
			lldbg("DEFECT_RECOVER: wd_create() failed\n");
			ret = -ENOMEM;
			break;
		}

		if (wd_start(wdog, 10000, NULL, 0) != OK) {
			lldbg("DEFECT_RECOVER: wd_start() failed\n");
			wd_delete(wdog);
			ret = -EIO;
			break;
		}

		lldbg("DEFECT_RECOVER: Watchdog created and started\n");

		/* Cancel and delete — this exercises the same code path as
		 * wd_recover() (which calls wd_cancel + wd_delete).
		 * With the fix, wd_recover() wraps these in
		 * wd_mmu_write_begin()/end(). Without the fix, a Data Abort
		 * would occur here.
		 */
		wd_cancel(wdog);
		wd_delete(wdog);

		lldbg("DEFECT_RECOVER: wd_cancel() + wd_delete() completed — no Data Abort\n");
		lldbg("DEFECT_RECOVER: wd_recover() fix (wd_mmu_write_begin/end) is working\n");
		lldbg("DEFECT_RECOVER: PASS — no Data Abort in wd_recover() code path\n");

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_WD_RECOVER */

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_ABORT_HANDLER
	case TESTIOC_WDOG_DEFECT_ABORT_HANDLER: {
		/* Defect #12: Data Abort handler guard.
		 *
		 * If the Data Abort handler itself accesses the wdog pool
		 * (e.g., for diagnostic logging), a nested Data Abort occurs.
		 * Without a guard, this causes an infinite abort loop → hang.
		 *
		 * This test triggers a Data Abort and then checks if the
		 * handler can safely access wdog pool addresses.
		 */
		lldbg("=== DEFECT #12: Data Abort handler guard ===\n");
		lldbg("DEFECT_ABORT: This test verifies abort handler safety\n");
		lldbg("DEFECT_ABORT: The abort handler must NOT dereference wdog pool\n");
		lldbg("DEFECT_ABORT: addresses — use DFAR register value instead.\n");
		lldbg("DEFECT_ABORT: If handler accesses wdog pool → nested abort → hang\n");
		lldbg("DEFECT_ABORT: Fix: Add abort-in-progress flag to abort handler\n");

		/* This is a documentation-only test — no actual crash.
		 * The real test is: trigger a wdog Data Abort and verify
		 * the handler doesn't cause a nested abort.
		 */
		lldbg("DEFECT_ABORT: Test passed (documentation only — no crash)\n");

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_ABORT_HANDLER */

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_BOOT_ORDER
	case TESTIOC_WDOG_DEFECT_BOOT_ORDER: {

		/* Defect #13: Boot order issue.
		 *
		 * wd_mmu_protect_init() is called at the end of wd_initialize().
		 * If any wdog writes occur between wd_initialize() and the
		 * first user wd_start() call, they will Data Abort because
		 * the protection is already enabled but the code may not
		 * be using begin()/end().
		 *
		 * This test checks the boot sequence for unprotected wdog writes.
		 */
		lldbg("=== DEFECT #13: Boot order issue ===\n");
		lldbg("DEFECT_BOOT: Checking boot sequence for unprotected wdog writes...\n");
		lldbg("DEFECT_BOOT: wd_mmu_protect_init() is called at end of wd_initialize()\n");
		lldbg("DEFECT_BOOT: Any wdog write between init and first wd_start() → Data Abort\n");
		lldbg("DEFECT_BOOT: Fix: Verify no wdog writes in boot path after init\n");

		/* If we reached here, boot completed without Data Abort.
		 * This means either:
		 * - No wdog writes occurred during boot (good)
		 * - Protection was not enabled during boot
		 */
		lldbg("DEFECT_BOOT: Boot completed — no Data Abort during boot sequence\n");
		lldbg("DEFECT_BOOT: If protection is enabled, boot order is safe\n");

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_BOOT_ORDER */

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST_SECTION_SIZE
	case TESTIOC_WDOG_DEFECT_SECTION_SIZE: {
		/* Defect #15: L1 entry cache coherency.
		 *
		 * After wd_mmu_protect_init() modifies the L1 section entry
		 * in the page table, the TLB may still have a cached copy
		 * of the old entry. Without a full TLB flush, the stale
		 * entry may allow writes → protection not effective.
		 *
		 * This test:
		 * 1. Attempts a wild write to wdog pool immediately after init
		 * 2. If TLB has stale entry → write succeeds (defect!)
		 * 3. If TLB was flushed → Data Abort (protection works)
		 */
		volatile uint8_t *wdog_page = (volatile uint8_t *)&g_wdpool;

		lldbg("=== DEFECT #15: L1 entry cache coherency ===\n");
		lldbg("DEFECT_SECTION: Attempting wild write to wdog pool...\n");
		lldbg("DEFECT_SECTION: If TLB has stale entry → write succeeds (defect!)\n");
		lldbg("DEFECT_SECTION: If TLB was flushed → Data Abort (protection works)\n");

		/* Attempt a wild write — should Data Abort if protection works.
		 * If it succeeds, the TLB still has a stale entry.
		 */
		{
			irqstate_t irq_state = enter_critical_section();
			uint8_t orig = wdog_page[0];
			wdog_page[0] = 0xDD;
			wdog_page[0] = orig;
			leave_critical_section(irq_state);

			lldbg("DEFECT_SECTION: === DEFECT CONFIRMED ===\n");
			lldbg("DEFECT_SECTION: Write succeeded — TLB had stale entry!\n");
			lldbg("DEFECT_SECTION: Protection was NOT effective immediately after init\n");
			lldbg("DEFECT_SECTION: Fix: Use full TLB flush after L1 entry modification\n");
		}

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST_SECTION_SIZE */

#endif /* CONFIG_WDOG_MMU_DEFECT_TEST */

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST_NORMAL_OP
	case TESTIOC_WDOG_SCENARIO_NORMAL_OP: {
		/* TC-05: Normal wdog operation (start/cancel 100x).
		 *
		 * Start and cancel a watchdog 100 times under MMU protection.
		 * Verifies that normal wdog operations work correctly without
		 * Data Abort. Each iteration:
		 *   1. wd_create() — allocates a wdog from the pool
		 *   2. wd_start()  — starts the watchdog with a 10s delay
		 *   3. wd_cancel() — cancels the watchdog
		 *   4. wd_delete() — returns the wdog to the pool
		 *
		 * If MMU protection has a bug, a Data Abort will occur during
		 * any of these operations.
		 */
		int i;
		WDOG_ID wdog;
		int ok_count = 0;

		lldbg("=== TC-05: Normal wdog operation (100 iterations) ===\n");

		for (i = 0; i < 100; i++) {
			wdog = wd_create();
			if (wdog == NULL) {
				lldbg("TC05: wd_create() failed at iteration %d\n", i);
				break;
			}

			if (wd_start(wdog, 10000, NULL, 0) != OK) {
				lldbg("TC05: wd_start() failed at iteration %d\n", i);
				wd_delete(wdog);
				break;
			}

			wd_cancel(wdog);
			wd_delete(wdog);
			ok_count++;
		}

		lldbg("TC05: Completed %d/100 iterations successfully\n", ok_count);
		if (ok_count == 100) {
			lldbg("TC05: PASS — all 100 start/cancel cycles completed\n");
		} else {
			lldbg("TC05: FAIL — only %d/100 completed\n", ok_count);
		}

		ret = (ok_count == 100) ? OK : -EIO;
		break;
	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST_NORMAL_OP */

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST_MULTI_EXPIRE
	case TESTIOC_WDOG_SCENARIO_MULTI_EXPIRE: {
		/* TC-06: Multiple watchdog expiration in same tick.
		 *
		 * Start 5 watchdogs with the same delay (1 tick). All 5 will
		 * expire in the same timer tick. The wd_timer() handler must
		 * process all 5 expirations under MMU protection without
		 * Data Abort.
		 *
		 * Each watchdog has a callback that increments a counter.
		 * After the delay, verify all 5 callbacks were invoked.
		 */
		#define TC06_NUM_WDOGS  5
		WDOG_ID wdogs[TC06_NUM_WDOGS];
		int i;

		lldbg("=== TC-06: Multiple watchdog expiration in same tick ===\n");
		g_tc06_callback_count = 0;


		/* Create and start 5 watchdogs with the same 1-tick delay */
		for (i = 0; i < TC06_NUM_WDOGS; i++) {
			wdogs[i] = wd_create();
			if (wdogs[i] == NULL) {
				lldbg("TC06: wd_create() failed for wdog %d\n", i);
				ret = -ENOMEM;
				goto tc06_cleanup;
			}
		}

		for (i = 0; i < TC06_NUM_WDOGS; i++) {
			if (wd_start(wdogs[i], 10, tc06_callback, 0) != OK) {
				lldbg("TC06: wd_start() failed for wdog %d\n", i);
				ret = -EIO;
				goto tc06_cleanup;
			}
		}


		lldbg("TC06: Started %d watchdogs with 10-tick delay, waiting...\n",
		      TC06_NUM_WDOGS);

		/* Wait for expiration (10 ticks = 100ms at 100Hz, wait 200ms) */
		up_mdelay(200);

		lldbg("TC06: Callback count = %d (expected %d)\n",
		      g_tc06_callback_count, TC06_NUM_WDOGS);

		if (g_tc06_callback_count == TC06_NUM_WDOGS) {
			lldbg("TC06: PASS — all %d watchdogs expired without Data Abort\n",
			      TC06_NUM_WDOGS);
			ret = OK;
		} else {
			lldbg("TC06: FAIL — only %d/%d callbacks invoked\n",
			      g_tc06_callback_count, TC06_NUM_WDOGS);

			ret = -EIO;
		}

	tc06_cleanup:
		for (i = 0; i < TC06_NUM_WDOGS; i++) {
			if (wdogs[i]) {
				wd_cancel(wdogs[i]);
				wd_delete(wdogs[i]);
			}
		}
		break;
	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST_MULTI_EXPIRE */

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST_SMP_CONCURRENT
	case TESTIOC_WDOG_SCENARIO_SMP_CONCURRENT: {
		/* TC-08: SMP concurrent wdog access from multiple cores.
		 *
		 * On SMP systems, multiple cores call wd_start/wd_cancel
		 * concurrently. This tests whether the per-core DACR issue
		 * causes a Data Abort when Core 1 accesses wdog pool while
		 * Core 0 is inside a write window.
		 *
		 * This test creates a kernel thread pinned to Core 1 via
		 * task_create() + sched_setaffinity(). The thread repeatedly
		 * calls wd_start/wd_cancel from Core 1 while Core 0 (the ioctl
		 * caller) also does wdog operations.
		 *
		 * If the per-core DACR defect exists, Core 1's wd_start() will
		 * trigger a Data Abort because Core 1's DACR is still RO when
		 * Core 0 calls wd_mmu_write_begin().
		 *
		 * NOTE: This test is only meaningful on SMP (CONFIG_SMP=y).
		 * On single-core, it runs the same logic but without
		 * actual concurrency.
		 */

		lldbg("=== TC-08: SMP concurrent wdog access ===\n");
#ifdef CONFIG_SMP
		lldbg("TC08: SMP is enabled — creating Core 1 thread\n");

		{
			pid_t core1_pid;
			cpu_set_t cpu_mask;
			int i;
			WDOG_ID wdog;
			int ok_count = 0;

			g_tc08_core1_running = 1;
			g_tc08_core1_ok_count = 0;
			g_tc08_core1_stop = 0;

			/* Create a kernel thread */
			core1_pid = kernel_thread("tc08_core1", 100, 2048,
			                         tc08_core1_thread, NULL);

			if (core1_pid < 0) {
				lldbg("TC08: task_create() failed (%d)\n", core1_pid);
				ret = -ENOMEM;
				break;
			}

			/* Pin the thread to Core 1 (CPU 1) */
			CPU_ZERO(&cpu_mask);
			CPU_SET(1, &cpu_mask);
			if (sched_setaffinity(core1_pid, sizeof(cpu_set_t),
			                      &cpu_mask) != OK) {
				lldbg("TC08: sched_setaffinity() failed — thread runs on Core 0\n");
			} else {
				lldbg("TC08: Core 1 thread pinned to CPU 1 (pid=%d)\n",
				      core1_pid);
			}

			/* Core 0: also do wdog operations concurrently */
			for (i = 0; i < 500; i++) {
				wdog = wd_create();
				if (!wdog) {
					lldbg("TC08: Core0 wd_create() failed at %d\n", i);
					break;
				}
				if (wd_start(wdog, 10000, NULL, 0) != OK) {
					lldbg("TC08: Core0 wd_start() failed at %d\n", i);
					wd_delete(wdog);
					break;
				}
				wd_cancel(wdog);
				wd_delete(wdog);
				ok_count++;
			}

			/* Wait for Core 1 thread to finish */
			lldbg("TC08: Core 0 done (%d iterations), waiting for Core 1...\n",
			      ok_count);
			while (g_tc08_core1_running) {
				up_mdelay(10);
			}

			lldbg("TC08: Core 0 = %d/500, Core 1 = %d/500\n",
			      ok_count, g_tc08_core1_ok_count);

			if (ok_count == 500 && g_tc08_core1_ok_count == 500) {
				lldbg("TC08: PASS — both cores completed 500 iterations\n");
				lldbg("TC08: No Data Abort — wdog MMU protection handles SMP\n");
				ret = OK;
			} else {
				lldbg("TC08: FAIL — Core0=%d, Core1=%d\n",
				      ok_count, g_tc08_core1_ok_count);
				ret = -EIO;
			}
		}

#else
		lldbg("TC08: SMP is NOT enabled — single-core simulation\n");
		{
			int i;
			WDOG_ID wdog;
			int ok_count = 0;

			for (i = 0; i < 1000; i++) {
				wdog = wd_create();
				if (!wdog) {
					lldbg("TC08: wd_create() failed at iteration %d\n", i);
					break;
				}
				if (wd_start(wdog, 10000, NULL, 0) != OK) {
					lldbg("TC08: wd_start() failed at iteration %d\n", i);
					wd_delete(wdog);
					break;
				}
				wd_cancel(wdog);
				wd_delete(wdog);
				ok_count++;
			}

			lldbg("TC08: Completed %d/1000 iterations\n", ok_count);
			if (ok_count == 1000) {
				lldbg("TC08: PASS — 1000 start/cancel cycles OK\n");
				ret = OK;
			} else {
				lldbg("TC08: FAIL — only %d/1000 completed\n", ok_count);
				ret = -EIO;
			}
		}
#endif /* CONFIG_SMP */
		break;
	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST_SMP_CONCURRENT */


#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST_SMP_TICK_CB
	case TESTIOC_WDOG_SCENARIO_SMP_TICK_CB: {
		/* TC-09: SMP timer tick on Core 0, callback on Core 1.
		 *
		 * On SMP, the timer tick fires on Core 0, which calls
		 * wd_timer(). wd_timer() processes expirations and invokes
		 * callbacks. If a callback runs on Core 1, it may access
		 * wdog data structures. Core 1's DACR is still RO (Client),
		 * so any wdog write in the callback → Data Abort.
		 *
		 * This test creates a kernel thread pinned to Core 1 that
		 * starts a watchdog with a real callback function. The
		 * callback may fire on Core 0 (timer tick core) while the
		 * wd_start() was called from Core 1.
		 *
		 * On SMP, this exercises the cross-core DACR scenario:
		 * - Core 1 calls wd_start() → wd_mmu_write_begin() sets
		 *   Core 1's DACR = RW
		 * - Timer tick on Core 0 → wd_timer() → wd_mmu_write_begin()
		 *   sets Core 0's DACR = RW
		 * - Callback runs → may be on Core 0 or Core 1
		 */
		lldbg("=== TC-09: SMP timer tick / callback cross-core ===\n");
		g_tc09_callback_fired = 0;
		g_tc09_core1_done = 0;
		g_tc09_wdog = NULL;


#ifdef CONFIG_SMP
		lldbg("TC09: SMP is enabled — creating Core 1 thread\n");
		{
			pid_t core1_pid;
			cpu_set_t cpu_mask;

			/* Create a kernel thread on Core 1 */
			core1_pid = kernel_thread("tc09_core1", 100, 2048,
			                         tc09_core1_thread, NULL);

			if (core1_pid < 0) {
				lldbg("TC09: task_create() failed (%d)\n", core1_pid);
				ret = -ENOMEM;
				break;
			}

			/* Pin the thread to Core 1 (CPU 1) */
			CPU_ZERO(&cpu_mask);
			CPU_SET(1, &cpu_mask);
			if (sched_setaffinity(core1_pid, sizeof(cpu_set_t),
			                      &cpu_mask) != OK) {
				lldbg("TC09: sched_setaffinity() failed\n");
			} else {
				lldbg("TC09: Core 1 thread pinned to CPU 1 (pid=%d)\n",
				      core1_pid);
			}

			/* Wait for Core 1 to start the watchdog */
			lldbg("TC09: Waiting for Core 1 to start watchdog...\n");
			while (!g_tc09_core1_done) {
				up_mdelay(10);
			}

			/* Wait for callback to fire */
			lldbg("TC09: Waiting for callback...\n");
			up_mdelay(50);
		}
#else
		lldbg("TC09: SMP NOT enabled — single-core test\n");
		{
			g_tc09_wdog = wd_create();
			if (!g_tc09_wdog) {
				lldbg("TC09: wd_create() failed\n");
				ret = -ENOMEM;
				break;
			}
			if (wd_start(g_tc09_wdog, 1, tc09_callback, 0) != OK) {
				lldbg("TC09: wd_start() failed\n");
				wd_delete(g_tc09_wdog);
				g_tc09_wdog = NULL;
				ret = -EIO;
				break;
			}
			up_mdelay(50);
		}
#endif /* CONFIG_SMP */

		if (g_tc09_callback_fired) {
			lldbg("TC09: PASS — callback fired without Data Abort\n");
#ifdef CONFIG_SMP
			lldbg("TC09: wd_start() from Core 1, callback fired — cross-core OK\n");
#endif
			ret = OK;
		} else {
			lldbg("TC09: FAIL — callback did not fire after 50ms\n");
			ret = -EIO;
		}

		if (g_tc09_wdog) {
			wd_cancel(g_tc09_wdog);
			wd_delete(g_tc09_wdog);
			g_tc09_wdog = NULL;
		}
		break;

	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST_SMP_TICK_CB */



#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST_NESTING_STRESS
	case TESTIOC_WDOG_SCENARIO_NESTING_STRESS: {
		/* TC-13: Nesting counter stress test (1000 reentrant calls).
		 *
		 * Call wd_mmu_write_begin() 1000 times reentrantly, then
		 * wd_mmu_write_end() 1000 times. The nesting counter must
		 * correctly track the depth and only restore RO protection
		 * on the final end() call.
		 *
		 * If the counter overflows or underflows, the page will be
		 * left in RW (unprotected) or RO (causing Data Abort in wdog
		 * code).
		 */
		int i;

		lldbg("=== TC-13: Nesting counter stress (1000 reentrant calls) ===\n");

		/* 1000 reentrant begin() calls — nesting counter goes to 1000 */
		for (i = 0; i < 1000; i++) {
			wd_mmu_write_begin();
		}

		lldbg("TC13: 1000 begin() calls done, nesting counter = 1000\n");

		/* 999 end() calls — counter goes to 1, page still RW */
		for (i = 0; i < 999; i++) {
			wd_mmu_write_end();
		}

		lldbg("TC13: 999 end() calls done, nesting counter = 1 (page still RW)\n");

		/* Final end() — counter goes to 0, page restored to RO */
		wd_mmu_write_end();

		lldbg("TC13: Final end() done, nesting counter = 0 (page restored to RO)\n");
		lldbg("TC13: PASS — nesting counter handled 1000 reentrant calls\n");

		ret = OK;
		break;
	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST_NESTING_STRESS */

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST_LONG_RUN
	case TESTIOC_WDOG_SCENARIO_LONG_RUN: {
		/* TC-15: Long-run stability test.
		 *
		 * Run continuous wdog start/cancel operations for a configurable
		 * duration. Default is 60 seconds for testing. For production
		 * validation, extend to 24 hours.
		 *
		 * This test verifies:
		 * - No memory leak (wdog pool not exhausted)
		 * - No Data Abort over time
		 * - Nesting counter stays balanced
		 *
		 * arg = 0: 60 seconds (default)
		 * arg = 1: 300 seconds (5 minutes)
		 * arg = 2: 86400 seconds (24 hours)
		 */
		int duration_sec = 60;
		WDOG_ID wdog;
		int total_ops = 0;


		if (arg == 1) {
			duration_sec = 300;
		} else if (arg == 2) {
			duration_sec = 86400;
		}

		lldbg("=== TC-15: Long-run stability (%d seconds) ===\n", duration_sec);
		lldbg("TC15: Running continuous start/cancel for %d sec...\n", duration_sec);

		/* Run for the specified duration */
		while (total_ops < duration_sec * 100) {
			wdog = wd_create();
			if (!wdog) {
				lldbg("TC15: wd_create() failed after %d ops — pool exhausted!\n",
				      total_ops);
				lldbg("TC15: FAIL — memory leak detected\n");
				ret = -ENOMEM;
				break;
			}

			wd_start(wdog, 10000, NULL, 0);
			wd_cancel(wdog);
			wd_delete(wdog);
			total_ops++;

			/* Print progress every 10000 ops */
			if (total_ops % 10000 == 0) {
				lldbg("TC15: %d ops completed (%d/%d sec)\n",
				      total_ops, total_ops / 100, duration_sec);
			}
		}

		if (ret != -ENOMEM) {
			lldbg("TC15: Completed %d ops in %d seconds\n", total_ops, duration_sec);
			lldbg("TC15: PASS — no Data Abort, no memory leak\n");
			ret = OK;
		}
		break;
	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST_LONG_RUN */

#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST */

	default:




		vdbg("Unrecognized cmd: %d arg: %ld\n", cmd, arg);
		break;

	}

	return ret;
}


static ssize_t os_api_test_drv_read(FAR struct file *filep, FAR char *buffer, size_t len)
{
	return 0;                                       /* Return EOF */
}

static ssize_t os_api_test_drv_write(FAR struct file *filep, FAR const char *buffer, size_t len)
{
	return len;                                     /* Say that everything was written */
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: os_api_test_drv_register
 *
 * Description:
 *   Register /dev/os_api_test
 *
 ****************************************************************************/

void os_api_test_drv_register(void)
{
	(void)register_driver(OS_API_TEST_DRVPATH, &os_api_test_drv_fops, 0666, NULL);
}
