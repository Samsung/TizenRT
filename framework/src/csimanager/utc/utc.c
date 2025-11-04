/*
 * utc.c
 *
 * Created on: 2021.08.18
 *     Author: taehwan
 */

#include <check.h>
#include <stdlib.h>
#include <system/system_log.h>
#include "dacm_interface/da_log_color.h"

#include "utc_config.h"
#include "utc_mocks.h"

#define TCNAME "csifw"

extern void utc_csimanager(TCase *tc);
extern void utc_csinetworkmonitor(TCase *tc);

static void setup(void)
{
	mock_common_init();
}

static void teardown(void)
{
	mock_common_deinit();
}

Suite *my_suite(void)
{
	system_log_level_set_all(SYSTEM_LOG_LEVEL_DEBUG);
	Suite *s = suite_create(TCNAME);
	TCase *tc = tcase_create(TCNAME);

	utc_csimanager(tc);
	utc_csinetworkmonitor(tc);

	tcase_add_checked_fixture(tc, setup, teardown);
	tcase_set_timeout(tc, 10);
	suite_add_tcase(s, tc);
	return s;
}

int main(void)
{
	int number_failed, number_run;
	Suite *s = my_suite();
	SRunner *sr = srunner_create(s);
	UTC_CONFIG_SET_DEFAULT_RUNNER(sr);
	srunner_run_all(sr, CK_ENV);
	number_failed = srunner_ntests_failed(sr);
	number_run = srunner_ntests_run(sr);
	srunner_free(sr);

	PRINT_RESULT(number_failed, number_run);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
