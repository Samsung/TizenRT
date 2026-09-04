/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * apps/examples/mmu_protect_test/mmu_protect_test_main.c
 *
 * Use-after-free detection test using MMU page protection.
 *
 * This test triggers a real use-after-free scenario:
 *   1. A page-aligned buffer is allocated in kernel space (via the
 *      os_api_test driver ioctl).
 *   2. The page is set to No-Access via mmu_set_page_no_access() —
 *      simulating a "free" that makes the memory inaccessible.
 *   3. The freed (protected) memory is then read or written.
 *   4. The MMU detects the illegal access and triggers a Data Abort.
 *
 * The Data Abort handler prints PC/DFAR/DFSR.  DFAR contains the address
 * of the freed memory that was illegally accessed — this confirms the
 * use-after-free was detected.
 *
 * The Data Abort is fatal: the system crashes/reboots (or recovers via
 * binary manager if CONFIG_BINMGR_RECOVERY is enabled).
 *
 * Usage from TASH:
 *   mmu_protect_test read    — UAF via read from freed page (Data Abort)
 *   mmu_protect_test write   — UAF via write to freed page (Data Abort)
 *
 * NOTE: Requires CONFIG_DRIVERS_OS_API_TEST and CONFIG_EXAMPLES_MMU_PROTECT_TEST.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/ioctl.h>

#include <tinyara/fs/ioctl.h>

#ifdef CONFIG_DRIVERS_OS_API_TEST
#include <tinyara/os_api_test_drv.h>
#else
/* Fallback definitions if the OS API test driver config is not enabled.
 * The ioctl numbers must match those in os_api_test_drv.h.
 */
#define OS_API_TEST_DRVPATH        "/dev/os_api_test"
#define _TESTIOC(nr)               _IOC(_IOC_NONE, 0, nr, 0)
#define TESTIOC_UAF_MMU_PROTECT    _TESTIOC(25)
#define TESTIOC_UAF_NO_MMU         _TESTIOC(26)
#define TESTIOC_WDOG_MMU_PROTECT_TEST _TESTIOC(27)
#endif

#ifdef CONFIG_WDOG_MMU_DEFECT_TEST
#ifndef _TESTIOC
#define _TESTIOC(nr)               _IOC(_IOC_NONE, 0, nr, 0)
#endif
#ifndef OS_API_TEST_DRVPATH
#define OS_API_TEST_DRVPATH        "/dev/os_api_test"
#endif
#define TESTIOC_WDOG_DEFECT_SMP            _TESTIOC(28)
#define TESTIOC_WDOG_DEFECT_DSB             _TESTIOC(29)
#define TESTIOC_WDOG_DEFECT_DACR_CLOBBER    _TESTIOC(30)
#define TESTIOC_WDOG_DEFECT_HEAP_OVERLAP    _TESTIOC(31)
#define TESTIOC_WDOG_DEFECT_WD_RECOVER      _TESTIOC(32)
#define TESTIOC_WDOG_DEFECT_ABORT_HANDLER   _TESTIOC(33)
#define TESTIOC_WDOG_DEFECT_BOOT_ORDER      _TESTIOC(34)
#define TESTIOC_WDOG_DEFECT_SECTION_SIZE    _TESTIOC(35)
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST */

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST
#ifndef _TESTIOC
#define _TESTIOC(nr)               _IOC(_IOC_NONE, 0, nr, 0)
#endif
#ifndef OS_API_TEST_DRVPATH
#define OS_API_TEST_DRVPATH        "/dev/os_api_test"
#endif
#define TESTIOC_WDOG_SCENARIO_NORMAL_OP      _TESTIOC(36)
#define TESTIOC_WDOG_SCENARIO_MULTI_EXPIRE    _TESTIOC(37)
#define TESTIOC_WDOG_SCENARIO_SMP_CONCURRENT _TESTIOC(38)
#define TESTIOC_WDOG_SCENARIO_SMP_TICK_CB     _TESTIOC(39)
#define TESTIOC_WDOG_SCENARIO_NESTING_STRESS  _TESTIOC(40)
#define TESTIOC_WDOG_SCENARIO_LONG_RUN        _TESTIOC(41)
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST */

/* ioctl arg values: 0 = read test, 1 = write test */

#define UAF_TEST_READ   0
#define UAF_TEST_WRITE  1



/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void mmu_protect_test_usage(void)
{
	printf("Usage: mmu_protect_test <command>\n");
	printf("\n");
	printf("UAF Tests:\n");
	printf("  read       — Trigger UAF by reading from freed (protected) page\n");
	printf("               MMU protection enabled → Data Abort (system crash)\n");
	printf("  write      — Trigger UAF by writing to freed (protected) page\n");
	printf("               MMU protection enabled → Data Abort (system crash)\n");
	printf("  nouaf      — UAF without MMU protection (no crash)\n");
	printf("               free() the buffer, then read freed memory.\n");
	printf("               Access succeeds silently — UAF goes undetected.\n");
	printf("               Shows why MMU protection is needed.\n");
	printf("  wdog       — Read from wdog protected page (safe, no crash)\n");
	printf("               Verifies wdog page is accessible for reads.\n");
	printf("  wdog_write — Write to wdog protected page from outside wdog code\n");
	printf("               If CONFIG_WDOG_MMU_PROTECT=y → Data Abort (crash)\n");
	printf("               If CONFIG_WDOG_MMU_PROTECT=n → write succeeds (no crash)\n");
#ifdef CONFIG_WDOG_MMU_DEFECT_TEST
	printf("\n");
	printf("Defect Reproduction Tests (requires CONFIG_WDOG_MMU_DEFECT_TEST):\n");
	printf("  defect_smp            — Defect #1: SMP DACR per-core issue\n");
	printf("  defect_dsb            — Defect #3: Missing dsb after DACR write\n");
	printf("  defect_dacr_clobber   — Defect #7: DACR clobbers other domains\n");
	printf("  defect_heap_overlap   — Defect #8: Heap overlap with wdog pool\n");
	printf("  defect_wd_recover     — Defect #11: wd_recover() missing protection\n");
	printf("  defect_abort_handler  — Defect #12: Data Abort handler guard\n");
	printf("  defect_boot_order     — Defect #13: Boot order issue\n");
	printf("  defect_section_size   — Defect #15: L1 entry cache coherency\n");
#endif
#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST
	printf("\n");
	printf("Scenario Tests (requires CONFIG_WDOG_MMU_SCENARIO_TEST):\n");
	printf("  scenario_normal       — TC-05: Normal wdog operation (100x start/cancel)\n");
	printf("  scenario_multi_expire — TC-06: Multiple watchdog expiration in same tick\n");
	printf("  scenario_smp_concurrent — TC-08: SMP concurrent wdog access (1000x)\n");
	printf("  scenario_smp_tick_cb — TC-09: SMP timer tick / callback cross-core\n");
	printf("  scenario_nesting     — TC-13: Nesting counter stress (1000 reentrant)\n");
	printf("  scenario_long_run    — TC-15: Long-run stability (60s default)\n");
	printf("                          arg: 0=60s, 1=300s, 2=86400s (24h)\n");
#endif
	printf("\n");


	printf("  read/write/wdog_write cause a fatal Data Abort.  Run separately.\n");
	printf("  nouaf and wdog do NOT crash — safe to run anytime.\n");
}




/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: mmu_protect_test_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int mmu_protect_test_main(int argc, char *argv[])
#endif
{
	int fd;
	int ret;
	unsigned long ioctl_arg;

	if (argc < 2) {
		mmu_protect_test_usage();
		return ERROR;
	}

	if (strcmp(argv[1], "read") == 0) {
		ioctl_arg = UAF_TEST_READ;
	} else if (strcmp(argv[1], "write") == 0) {
		ioctl_arg = UAF_TEST_WRITE;
	} else if (strcmp(argv[1], "nouaf") == 0) {
		/* No-MMU mode: uses a different ioctl command */
		printf("=== UAF Test WITHOUT MMU Protection ===\n");
		printf("[mmu_protect_test] Opening %s...\n", OS_API_TEST_DRVPATH);

		fd = open(OS_API_TEST_DRVPATH, O_RDWR);
		if (fd < 0) {
			printf("[mmu_protect_test] FAIL: cannot open %s (errno %d)\n",
			       OS_API_TEST_DRVPATH, errno);
			printf("[mmu_protect_test] Is CONFIG_DRIVERS_OS_API_TEST enabled?\n");
			return ERROR;
		}

		printf("[mmu_protect_test] UAF without MMU protection (no crash expected)\n");

		/* This ioctl runs in kernel space and does:
		 *   1. Allocate a page-aligned buffer
		 *   2. Write data to it (normal use)
		 *   3. free() the buffer (real free, NO MMU protection)
		 *   4. Read from the freed memory — access succeeds silently
		 *   5. Write to the freed memory — also succeeds silently
		 *
		 * Without MMU protection, the UAF goes undetected.
		 * The test returns OK and demonstrates silent memory corruption.
		 */

		ret = ioctl(fd, TESTIOC_UAF_NO_MMU, 0);

		if (ret == OK) {
			printf("[mmu_protect_test] UAF went UNDETECTED without MMU protection.\n");
			printf("[mmu_protect_test] This demonstrates why MMU protection is needed.\n");
		} else {
			printf("[mmu_protect_test] ioctl returned %d (errno %d)\n", ret, errno);
		}

		close(fd);
		return ret;
	} else if (strcmp(argv[1], "wdog") == 0 || strcmp(argv[1], "wdog_write") == 0) {
		/* Watchdog MMU protection verification test */
		int do_write = (strcmp(argv[1], "wdog_write") == 0);

		printf("=== WDOG MMU Protection Test (%s) ===\n", argv[1]);
		printf("[mmu_protect_test] Opening %s...\n", OS_API_TEST_DRVPATH);

		fd = open(OS_API_TEST_DRVPATH, O_RDWR);
		if (fd < 0) {
			printf("[mmu_protect_test] FAIL: cannot open %s (errno %d)\n",
			       OS_API_TEST_DRVPATH, errno);
			printf("[mmu_protect_test] Is CONFIG_DRIVERS_OS_API_TEST enabled?\n");
			return ERROR;
		}

		if (do_write) {
			printf("[mmu_protect_test] Writing to wdog protected page from outside wdog code\n");
			printf("[mmu_protect_test] If CONFIG_WDOG_MMU_PROTECT=y → expect Data Abort (crash)\n");
			printf("[mmu_protect_test] If CONFIG_WDOG_MMU_PROTECT=n → write succeeds (no crash)\n");
		} else {
			printf("[mmu_protect_test] Reading from wdog protected page (safe, no crash)\n");
		}

		/* This ioctl runs in kernel space and does:
		 *   1. Read from g_wdpool (wdog static data page) — always succeeds

		 *   2. If arg=1: Write to g_wdpool without wd_mmu_write_begin()

		 *      → Data Abort if CONFIG_WDOG_MMU_PROTECT is enabled
		 *      → silent corruption if protection is disabled
		 *
		 * arg = 0: read only (safe)
		 * arg = 1: write (crash if protection enabled)
		 */
		ret = ioctl(fd, TESTIOC_WDOG_MMU_PROTECT_TEST, do_write);

		if (ret == OK) {
			if (do_write) {
				printf("[mmu_protect_test] Write to wdog page succeeded (no crash).\n");
				printf("[mmu_protect_test] CONFIG_WDOG_MMU_PROTECT is NOT enabled.\n");
				printf("[mmu_protect_test] Wdog page is writable — protection is OFF.\n");
				printf("[mmu_protect_test] Enable CONFIG_WDOG_MMU_PROTECT=y to get Data Abort on write.\n");
			} else {
				printf("[mmu_protect_test] Wdog page read test passed.\n");
			}
		} else {
			printf("[mmu_protect_test] ioctl returned %d (errno %d)\n", ret, errno);
		}


		close(fd);
		return ret;
	}
#ifdef CONFIG_WDOG_MMU_DEFECT_TEST
	else if (strcmp(argv[1], "defect_smp") == 0 ||
	         strcmp(argv[1], "defect_dsb") == 0 ||
	         strcmp(argv[1], "defect_dacr_clobber") == 0 ||
	         strcmp(argv[1], "defect_heap_overlap") == 0 ||
	         strcmp(argv[1], "defect_wd_recover") == 0 ||
	         strcmp(argv[1], "defect_abort_handler") == 0 ||
	         strcmp(argv[1], "defect_boot_order") == 0 ||
	         strcmp(argv[1], "defect_section_size") == 0) {

		int defect_cmd;

		if (strcmp(argv[1], "defect_smp") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_SMP;
		} else if (strcmp(argv[1], "defect_dsb") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_DSB;
		} else if (strcmp(argv[1], "defect_dacr_clobber") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_DACR_CLOBBER;
		} else if (strcmp(argv[1], "defect_heap_overlap") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_HEAP_OVERLAP;
		} else if (strcmp(argv[1], "defect_wd_recover") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_WD_RECOVER;
		} else if (strcmp(argv[1], "defect_abort_handler") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_ABORT_HANDLER;
		} else if (strcmp(argv[1], "defect_boot_order") == 0) {
			defect_cmd = TESTIOC_WDOG_DEFECT_BOOT_ORDER;
		} else {
			defect_cmd = TESTIOC_WDOG_DEFECT_SECTION_SIZE;
		}

		printf("=== WDOG MMU Defect Test (%s) ===\n", argv[1]);
		printf("[mmu_protect_test] Opening %s...\n", OS_API_TEST_DRVPATH);

		fd = open(OS_API_TEST_DRVPATH, O_RDWR);
		if (fd < 0) {
			printf("[mmu_protect_test] FAIL: cannot open %s (errno %d)\n",
			       OS_API_TEST_DRVPATH, errno);
			printf("[mmu_protect_test] Is CONFIG_DRIVERS_OS_API_TEST enabled?\n");
			return ERROR;
		}

		printf("[mmu_protect_test] Running defect reproduction test...\n");
		printf("[mmu_protect_test] Check debug log (lldbg) for DEFECT_ messages\n");

		ret = ioctl(fd, defect_cmd, 0);

		if (ret == OK) {
			printf("[mmu_protect_test] Defect test completed. Check debug log for results.\n");
		} else {
			printf("[mmu_protect_test] ioctl returned %d (errno %d)\n", ret, errno);
		}

		close(fd);
		return ret;
	}
#endif /* CONFIG_WDOG_MMU_DEFECT_TEST */
#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST
	else if (strcmp(argv[1], "scenario_normal") == 0 ||
	         strcmp(argv[1], "scenario_multi_expire") == 0 ||
	         strcmp(argv[1], "scenario_smp_concurrent") == 0 ||
	         strcmp(argv[1], "scenario_smp_tick_cb") == 0 ||
	         strcmp(argv[1], "scenario_nesting") == 0 ||
	         strcmp(argv[1], "scenario_long_run") == 0) {

		int scenario_cmd;
		unsigned long scenario_arg = 0;

		if (strcmp(argv[1], "scenario_normal") == 0) {
			scenario_cmd = TESTIOC_WDOG_SCENARIO_NORMAL_OP;
		} else if (strcmp(argv[1], "scenario_multi_expire") == 0) {
			scenario_cmd = TESTIOC_WDOG_SCENARIO_MULTI_EXPIRE;
		} else if (strcmp(argv[1], "scenario_smp_concurrent") == 0) {
			scenario_cmd = TESTIOC_WDOG_SCENARIO_SMP_CONCURRENT;
		} else if (strcmp(argv[1], "scenario_smp_tick_cb") == 0) {
			scenario_cmd = TESTIOC_WDOG_SCENARIO_SMP_TICK_CB;
		} else if (strcmp(argv[1], "scenario_nesting") == 0) {
			scenario_cmd = TESTIOC_WDOG_SCENARIO_NESTING_STRESS;
		} else {
			scenario_cmd = TESTIOC_WDOG_SCENARIO_LONG_RUN;
			/* long_run accepts optional duration arg */
			if (argc >= 3) {
				/* Manual string-to-int (atoi not available in kernel) */
				if (argv[2][0] >= '0' && argv[2][0] <= '9') {
					scenario_arg = (unsigned long)(argv[2][0] - '0');
				}
			}

		}

		printf("=== WDOG MMU Scenario Test (%s) ===\n", argv[1]);
		printf("[mmu_protect_test] Opening %s...\n", OS_API_TEST_DRVPATH);

		fd = open(OS_API_TEST_DRVPATH, O_RDWR);
		if (fd < 0) {
			printf("[mmu_protect_test] FAIL: cannot open %s (errno %d)\n",
			       OS_API_TEST_DRVPATH, errno);
			printf("[mmu_protect_test] Is CONFIG_DRIVERS_OS_API_TEST enabled?\n");
			return ERROR;
		}

		printf("[mmu_protect_test] Running scenario test...\n");
		printf("[mmu_protect_test] Check debug log (lldbg) for TC messages\n");

		ret = ioctl(fd, scenario_cmd, scenario_arg);

		if (ret == OK) {
			printf("[mmu_protect_test] Scenario test PASSED. Check debug log for details.\n");
		} else {
			printf("[mmu_protect_test] Scenario test FAILED (ret=%d, errno %d)\n",
			       ret, errno);
		}

		close(fd);
		return ret;
	}
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST */
	else {

		mmu_protect_test_usage();
		return ERROR;
	}




	printf("=== MMU UAF Detection Test (%s) ===\n", argv[1]);

	printf("[mmu_protect_test] Opening %s...\n", OS_API_TEST_DRVPATH);

	fd = open(OS_API_TEST_DRVPATH, O_RDWR);
	if (fd < 0) {
		printf("[mmu_protect_test] FAIL: cannot open %s (errno %d)\n",
		       OS_API_TEST_DRVPATH, errno);
		printf("[mmu_protect_test] Is CONFIG_DRIVERS_OS_API_TEST enabled?\n");
		return ERROR;
	}

	printf("[mmu_protect_test] Triggering use-after-free via MMU protection...\n");
	printf("[mmu_protect_test] >>> EXPECT DATA ABORT — system will crash <<<\n");

	/* This ioctl runs in kernel space and does:
	 *   1. Allocate a page-aligned buffer
	 *   2. Write data to it (normal use)
	 *   3. mmu_set_page_no_access() — make page inaccessible (simulate free)
	 *   4. Read or write the protected page → Data Abort (UAF detected)
	 *
	 * The Data Abort handler prints:
	 *   PC:   faulting instruction
	 *   DFAR: address of freed memory accessed
	 *   DFSR: permission fault status
	 *
	 * This function does NOT return — the Data Abort is fatal.
	 */
	ret = ioctl(fd, TESTIOC_UAF_MMU_PROTECT, ioctl_arg);

	/* If we reach here, the MMU protection did not work */
	printf("[mmu_protect_test] ERROR: ioctl returned %d, Data Abort was NOT triggered!\n",
	       ret);
	printf("[mmu_protect_test] MMU protection FAILED to detect use-after-free\n");

	close(fd);
	return ERROR;
}
