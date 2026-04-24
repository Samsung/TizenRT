#include "tc_diagnostics.hpp"

extern "C" {
#include <tc_common.h>
}

extern "C" int tc_diagnostics_main(void)
{
	tc_libcxx_diagnostics_stdexcept_domain_error();
	tc_libcxx_diagnostics_stdexcept_invalid_argument();
	tc_libcxx_diagnostics_stdexcept_length_error();
	tc_libcxx_diagnostics_stdexcept_logic_error();
	tc_libcxx_diagnostics_stdexcept_out_of_range();
	tc_libcxx_diagnostics_stdexcept_overflow_error();
	tc_libcxx_diagnostics_stdexcept_range_error();
	tc_libcxx_diagnostics_stdexcept_runtime_error();
	tc_libcxx_diagnostics_stdexcept_underflow_error();
}
