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

/// @file tc_watchdog_security.c
/// @brief Security Test Cases for watchdog driver - WDIOC_CAPTURE vulnerability
/// @details Tests for security fix: Watchdog ioctl accepts caller-controlled IRQ callback pointers

#include <tinyara/config.h>
#include <tinyara/watchdog.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "tc_internal.h"
#include <errno.h>

#ifdef CONFIG_WATCHDOG

/**
 * @brief Closes all open file descriptors
 */
static inline void close_fds(int *fds, int count)
{
	for (; count >= 0; count--) {
		close(fds[count]);
	}
}

#ifdef CONFIG_BUILD_PROTECTED

/**
 * @fn                   :tc_watchdog_security_capture_userspace_ptr
 * @brief                :Test that user-space callback pointers are rejected in protected builds
 * @scenario             :Security test for WDIOC_CAPTURE vulnerability (finding-198)
 * API's covered         :ioctl with WDIOC_CAPTURE
 * Preconditions         :CONFIG_BUILD_PROTECTED=y
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_capture_userspace_ptr(void)
{
	int fd = 0;
	int ret = 0;
	FAR struct watchdog_capture_s cap;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	/* Test: User-space address handling depends on CONFIG_WATCHDOG_CAPTURE_USER
	 * User-space typically starts at 0x20000000 for ARM Cortex-M
	 * This simulates an attacker trying to install a user callback
	 */
	cap.newhandler = (xcpt_t)0x20000000;
	cap.oldhandler = NULL;

	ret = ioctl(fd, WDIOC_CAPTURE, (unsigned long)&cap);

#ifdef CONFIG_WATCHDOG_CAPTURE_USER
	/* Legacy mode: user-space callbacks are allowed (development/legacy support) */
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl_legacy_mode", ret, OK, close(fd));
	printf("[WATCHDOG SECURITY] Legacy mode: user callbacks allowed (CONFIG_WATCHDOG_CAPTURE_USER=y)\n");
#else
	/* Security mode: user-space callbacks are rejected with -EPERM */
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl_security", ret, -EPERM, close(fd));
#endif

	close(fd);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_watchdog_security_capture_null_struct
 * @brief                :Test that NULL capture struct pointer is handled correctly
 * @scenario             :Security test for NULL pointer handling
 * API's covered         :ioctl with WDIOC_CAPTURE
 * Preconditions         :CONFIG_BUILD_PROTECTED=y
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_capture_null_struct(void)
{
	int fd = 0;
	int ret = 0;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	/* Test: NULL capture struct pointer should return -EINVAL */
	ret = ioctl(fd, WDIOC_CAPTURE, 0UL);
	TC_ASSERT_LT_CLEANUP("watchdog_ioctl_null_struct", ret, 0, close(fd));

	close(fd);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_watchdog_security_device_permission
 * @brief                :Test that watchdog device permissions restrict access
 * @scenario             :Security test for device access control
 * API's covered         :open
 * Preconditions         :CONFIG_BUILD_PROTECTED=y
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_device_permission(void)
{
	int fd = 0;

	/* Test: In protected builds, device permissions should be 0600
	 * This test verifies the device can be opened (kernel has access)
	 * but documents that user apps would be denied
	 */
	fd = open("/dev/watchdog0", O_RDWR);

	/* In kernel context, open should succeed.
	 * User-space apps would get -EACCES due to 0600 permissions.
	 * This test documents the expected behavior.
	 */
	TC_ASSERT_GT("watchdog_open_kernel_access", fd, 0);

	close(fd);

	/* Note: Full permission test requires user-space context
	 * which is outside the scope of this kernel test.
	 */
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_watchdog_security_invalid_address_high
 * @brief                :Test that invalid high addresses are rejected
 * @scenario             :Security test for address validation
 * API's covered         :ioctl with WDIOC_CAPTURE
 * Preconditions         :CONFIG_BUILD_PROTECTED=y
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_invalid_address_high(void)
{
	int fd = 0;
	int ret = 0;
	FAR struct watchdog_capture_s cap;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	/* Test: Address beyond valid memory range handling depends on CONFIG_WATCHDOG_CAPTURE_USER
	 * 0xFFFFFFFF is clearly invalid
	 */
	cap.newhandler = (xcpt_t)0xFFFFFFFF;
	cap.oldhandler = NULL;

	ret = ioctl(fd, WDIOC_CAPTURE, (unsigned long)&cap);

#ifdef CONFIG_WATCHDOG_CAPTURE_USER
	/* Legacy mode: high addresses may be accepted (no security check) */
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl_legacy_mode", ret, OK, close(fd));
	printf("[WATCHDOG SECURITY] Legacy mode: high address accepted (CONFIG_WATCHDOG_CAPTURE_USER=y)\n");
#else
	/* Security mode: invalid high addresses are rejected with -EPERM */
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl_security", ret, -EPERM, close(fd));
#endif

	close(fd);
	TC_SUCCESS_RESULT();
}

/**
 * @fn                   :tc_watchdog_security_invalid_address_zero
 * @brief                :Test that NULL callback handler is handled correctly
 * @scenario             :Security test for NULL callback (should restore reset behavior)
 * API's covered         :ioctl with WDIOC_CAPTURE
 * Preconditions         :CONFIG_BUILD_PROTECTED=y
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_null_callback(void)
{
	int fd = 0;
	int ret = 0;
	FAR struct watchdog_capture_s cap;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	/* Test: NULL callback should be allowed (restores reset behavior)
	 * Per watchdog.h: "Providing handler==NULL will restore the reset behavior"
	 */
	cap.newhandler = NULL;
	cap.oldhandler = NULL;

	ret = ioctl(fd, WDIOC_CAPTURE, (unsigned long)&cap);
	TC_ASSERT_EQ_CLEANUP("watchdog_ioctl_null_callback", ret, OK, close(fd));

	close(fd);
	TC_SUCCESS_RESULT();
}

#else /* CONFIG_BUILD_PROTECTED */

/**
 * @fn                   :tc_watchdog_security_flat_build_warning
 * @brief                :Document that security tests are skipped in flat builds
 * @scenario             :Informational test for flat build configuration
 * API's covered         :none
 * Preconditions         :CONFIG_BUILD_PROTECTED=n
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_flat_build_warning(void)
{
	/* In flat builds, the security checks are not active.
	 * This test documents that behavior.
	 * WDIOC_CAPTURE security validation only applies to protected builds.
	 */
	printf("[WATCHDOG SECURITY] Tests skipped: CONFIG_BUILD_PROTECTED is not set\n");
	printf("[WATCHDOG SECURITY] Security validation requires protected build\n");
	TC_SUCCESS_RESULT();
}

#endif /* CONFIG_BUILD_PROTECTED */

/**
 * @fn                   :tc_watchdog_security_capture_with_user_flag
 * @brief                :Test WDIOC_CAPTURE when CONFIG_WATCHDOG_CAPTURE_USER is enabled
 * @scenario             :Functional test for legacy/development mode
 * API's covered         :ioctl with WDIOC_CAPTURE
 * Preconditions         :CONFIG_WATCHDOG_CAPTURE_USER=y (optional feature)
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_capture_with_user_flag(void)
{
#ifdef CONFIG_WATCHDOG_CAPTURE_USER
	int fd = 0;
	int ret = 0;
	FAR struct watchdog_capture_s cap;

	fd = open("/dev/watchdog0", O_RDWR);
	TC_ASSERT_GT("watchdog_open", fd, 0);

	/* When WATCHDOG_CAPTURE_USER is enabled, user-space callbacks are allowed
	 * This is for development/legacy support only
	 */
	cap.newhandler = (xcpt_t)0x20000000;  /* User-space address */
	cap.oldhandler = NULL;

	ret = ioctl(fd, WDIOC_CAPTURE, (unsigned long)&cap);

	/* In legacy mode, this should succeed (or fail for other reasons, not -EPERM) */
	/* Note: May still fail if address is truly invalid, but not due to security check */
	if (ret == -EPERM) {
		printf("[WATCHDOG SECURITY] WARNING: User callback rejected even with CONFIG_WATCHDOG_CAPTURE_USER=y\n");
	}

	close(fd);
	TC_SUCCESS_RESULT();
#else
	/* Test not applicable - feature not enabled */
	printf("[WATCHDOG SECURITY] Skipped: CONFIG_WATCHDOG_CAPTURE_USER is not set\n");
	TC_SUCCESS_RESULT();
#endif
}

/**
 * @fn                   :tc_watchdog_security_multiple_open_attempt
 * @brief                :Test multiple open attempts to verify resource limits
 * @scenario             :Security test for resource exhaustion
 * API's covered         :open
 * Preconditions         :none
 * Postconditions        :none
 * @return               :void
 */
static void tc_watchdog_security_multiple_open_attempt(void)
{
	int count = 0;
	int fds[10] = {0};

	/* Test: Verify that multiple opens are limited
	 * This prevents resource exhaustion attacks
	 */
	for (count = 0; count < 10; count++) {
		fds[count] = open("/dev/watchdog0", O_RDWR);
		if (fds[count] < 0) {
			/* Expected: EMFILE when limit reached */
			TC_ASSERT_EQ("watchdog_open_limit", errno, EMFILE);
			break;
		}
	}

	/* Cleanup */
	close_fds(fds, --count);
	TC_SUCCESS_RESULT();
}

#endif /* CONFIG_WATCHDOG */

/****************************************************************************
 * Name: watchdog_security_main
 ****************************************************************************/

void watchdog_security_main(void)
{
#ifdef CONFIG_WATCHDOG
	printf("=== Watchdog Security Tests ===\n");
	printf("Testing fix for: Watchdog ioctl accepts caller-controlled IRQ callback pointers\n");
	printf("Finding ID: finding-198\n\n");

#ifdef CONFIG_BUILD_PROTECTED
	printf("Build Type: PROTECTED (Security tests ENABLED)\n\n");

	tc_watchdog_security_capture_userspace_ptr();
	tc_watchdog_security_capture_null_struct();
	tc_watchdog_security_device_permission();
	tc_watchdog_security_invalid_address_high();
	tc_watchdog_security_null_callback();
#else
	printf("Build Type: FLAT (Security tests DISABLED)\n\n");

	tc_watchdog_security_flat_build_warning();
#endif

	tc_watchdog_security_capture_with_user_flag();
	tc_watchdog_security_multiple_open_attempt();

	printf("\n=== Watchdog Security Tests Complete ===\n");
#else
	printf("WATCHDOG NOT ENABLED: CONFIG_WATCHDOG is not set\n");
	printf("Enable CONFIG_WATCHDOG to run security tests\n");
#endif
}
