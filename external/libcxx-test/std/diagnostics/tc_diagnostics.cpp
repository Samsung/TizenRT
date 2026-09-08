#include "tc_diagnostics.hpp"
#include <stdio.h>

extern "C" {
#include <tc_common.h>
}

extern "C" int tc_diagnostics_main(void)
{
	printf("\nStarting [DIAGNOSTICS] Tests\n");
	fflush(stdout);
	tc_libcxx_diagnostics_stdexcept_domain_error();
	tc_libcxx_diagnostics_stdexcept_invalid_argument();
	tc_libcxx_diagnostics_stdexcept_length_error();
	tc_libcxx_diagnostics_stdexcept_logic_error();
	tc_libcxx_diagnostics_stdexcept_out_of_range();
	tc_libcxx_diagnostics_stdexcept_overflow_error();
	tc_libcxx_diagnostics_stdexcept_range_error();
	tc_libcxx_diagnostics_stdexcept_runtime_error();
	tc_libcxx_diagnostics_stdexcept_underflow_error();
	return 0;
}
