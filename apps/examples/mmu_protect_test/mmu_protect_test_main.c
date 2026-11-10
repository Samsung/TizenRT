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
 * Watchdog MMU page protection verification test.
 *
 * This test accesses the watchdog static data page (g_wdpool) from
 * outside wdog code — i.e. without calling wd_mmu_write_begin().
 *
 * Step 1: Read from the wdog page — should succeed (page is Read-Only,
 *         reads are allowed).
 * Step 2: Write to the wdog page — if CONFIG_WDOG_MMU_PROTECT is enabled,
 *         the MMU triggers a Data Abort (permission fault).  DFAR will
 *         contain the wdog page address.
 *
 * Usage from TASH:
 *   mmu_protect_test wdog       — Read from wdog protected page (safe)
 *   mmu_protect_test wdog_write — Write to wdog protected page (crash if protected)
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
#define OS_API_TEST_DRVPATH            "/dev/os_api_test"
#define _TESTIOC(nr)                   _IOC(_IOC_NONE, 0, nr, 0)
#define TESTIOC_WDOG_MMU_PROTECT_TEST  _TESTIOC(27)
#endif

#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST
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
#define TESTIOC_WDOG_SCENARIO_NORMAL_OP      _TESTIOC(36)
#define TESTIOC_WDOG_SCENARIO_MULTI_EXPIRE    _TESTIOC(37)
#define TESTIOC_WDOG_SCENARIO_SMP_CONCURRENT _TESTIOC(38)
#define TESTIOC_WDOG_SCENARIO_SMP_TICK_CB     _TESTIOC(39)
#define TESTIOC_WDOG_SCENARIO_NESTING_STRESS  _TESTIOC(40)
#define TESTIOC_WDOG_SCENARIO_LONG_RUN        _TESTIOC(41)
#endif /* CONFIG_WDOG_MMU_SCENARIO_TEST */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void mmu_protect_test_usage(void)
{
	printf("Usage: mmu_protect_test <command>\n");
	printf("\n");
	printf("Wdog MMU Protection Tests:\n");
	printf("  wdog       — Read from wdog protected page (safe, no crash)\n");
	printf("               Verifies wdog page is accessible for reads.\n");
	printf("  wdog_write — Write to wdog protected page from outside wdog code\n");
	printf("               If CONFIG_WDOG_MMU_PROTECT=y → Data Abort (crash)\n");
	printf("               If CONFIG_WDOG_MMU_PROTECT=n → write succeeds (no crash)\n");
#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST
	printf("\n");
	printf("Defect Reproduction Tests (requires CONFIG_WDOG_MMU_SCENARIO_TEST):\n");
	printf("  defect_smp            — Defect #1: SMP DACR per-core issue\n");
	printf("  defect_dsb            — Defect #3: Missing dsb after DACR write\n");
	printf("  defect_dacr_clobber   — Defect #7: DACR clobbers other domains\n");
	printf("  defect_heap_overlap   — Defect #8: Heap overlap with wdog pool\n");
	printf("  defect_wd_recover     — Defect #11: wd_recover() missing protection\n");
	printf("  defect_abort_handler  — Defect #12: Data Abort handler guard\n");
	printf("  defect_boot_order     — Defect #13: Boot order issue\n");
	printf("  defect_section_size   — Defect #15: L1 entry cache coherency\n");
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
	printf("  wdog_write causes a fatal Data Abort if protection is enabled.  Run separately.\n");
	printf("  wdog (read-only) does NOT crash — safe to run anytime.\n");
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

	if (argc < 2) {
		mmu_protect_test_usage();
		return ERROR;
	}

	if (strcmp(argv[1], "wdog") == 0 || strcmp(argv[1], "wdog_write") == 0) {
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
#ifdef CONFIG_WDOG_MMU_SCENARIO_TEST
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
}
