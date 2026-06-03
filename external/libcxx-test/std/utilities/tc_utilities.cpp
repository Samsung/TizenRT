#include "tc_utilities.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern "C" {
#include <tc_common.h>
}

extern "C" {

	void tc_utilities_pair()
	{
		{ tc_libcxx_utilities_pair_astuple_get_const(); }
		{ tc_libcxx_utilities_pair_astuple_get_non_const(); }
		{ tc_libcxx_utilities_pair_astuple_get_rv(); }
		{ tc_libcxx_utilities_pair_astuple_tuple_element(); }
		{ tc_libcxx_utilities_pair_astuple_tuple_size(); }
		{ tc_libcxx_utilities_pair_piecewise_piecewise_construct(); }
		{ tc_libcxx_utilities_pairs_pair_assign_const_pair_U_V(); }
		{ tc_libcxx_utilities_pairs_pair_assign_rv_pair(); }
		{ tc_libcxx_utilities_pairs_pair_assign_rv_pair_U_V(); }
		{ tc_libcxx_utilities_pairs_pair_const_first_const_second(); }
		{ tc_libcxx_utilities_pairs_pair_const_pair_U_V(); }
		{ tc_libcxx_utilities_pairs_pair_copy_ctor(); }
		{ tc_libcxx_utilities_pairs_pair_default(); }
		{ tc_libcxx_utilities_pairs_pair_piecewise(); }
		{ tc_libcxx_utilities_pairs_pair_rv_pair_U_V(); }
		{ tc_libcxx_utilities_pairs_pair_swap(); }
		{ tc_libcxx_utilities_pairs_pair_types(); }
		{ tc_libcxx_utilities_pairs_pair_U_V(); }
		{ tc_libcxx_utilities_pairs_spec_comparison(); }
		{ tc_libcxx_utilities_pairs_spec_make_pair(); }
		{ tc_libcxx_utilities_pairs_spec_non_member_swap(); }
	}

	void tc_utilities_smartptr()
	{
		{ tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr(); }
		{ tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_rv(); }
		{ tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y(); }
		{ tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y_rv(); }
		{ tc_libcxx_utilities_util_smartptr_shared_assign_unique_ptr_Y(); }
		{ tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr(); }
		{ tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_pointer(); }
		{ tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_rv(); }
		{ tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y(); }
		{ tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y_rv(); }
	}

	int tc_utilities_main(void)
	{
		printf("\nStarting [UTILITIES] Tests\n");
		fflush(stdout);

		tc_utilities_pair();
		usleep(1000);
		tc_utilities_smartptr();
		usleep(1000);

		return 0;
	}

	// Menu function for UTILITIES sub-tests (called from utc_libcxx.c)
	void tc_utilities_menu(int argc, char *argv[])
	{
		// No arguments: run all utilities tests
		if (argc <= 1) {
			tc_utilities_main();
			return;
		}

		if (strcmp(argv[1], "pair") == 0) {
			printf("\nRunning pair tests...\n");
			tc_utilities_pair();
		}
		else if (strcmp(argv[1], "smartptr") == 0) {
			printf("\nRunning smartptr tests...\n");
			tc_utilities_smartptr();
		}
		else {
			printf("\nUnknown utilities sub-category: %s\n", argv[1]);
			printf("Available: pair, smartptr\n");
		}
	}
}
