#include "tc_utilities.hpp"

extern "C" {
#include <tc_common.h>
}

extern "C" int tc_utilities_main(void)
{
	tc_libcxx_utilities_pair_astuple_get_const();
	tc_libcxx_utilities_pair_astuple_get_non_const();
	tc_libcxx_utilities_pair_astuple_get_rv();
	tc_libcxx_utilities_pair_astuple_tuple_element();
	tc_libcxx_utilities_pair_astuple_tuple_size();
	tc_libcxx_utilities_pair_piecewise_piecewise_construct();
	tc_libcxx_utilities_pairs_pair_assign_const_pair_U_V();
	tc_libcxx_utilities_pairs_pair_assign_rv_pair();
	tc_libcxx_utilities_pairs_pair_assign_rv_pair_U_V();
	tc_libcxx_utilities_pairs_pair_const_first_const_second();
	tc_libcxx_utilities_pairs_pair_const_pair_U_V();
	tc_libcxx_utilities_pairs_pair_copy_ctor();
	tc_libcxx_utilities_pairs_pair_default();
	tc_libcxx_utilities_pairs_pair_piecewise();
	tc_libcxx_utilities_pairs_pair_rv_pair_U_V();
	tc_libcxx_utilities_pairs_pair_swap();
	tc_libcxx_utilities_pairs_pair_types();
	tc_libcxx_utilities_pairs_pair_U_V();
	tc_libcxx_utilities_pairs_spec_comparison();
	tc_libcxx_utilities_pairs_spec_make_pair();
	tc_libcxx_utilities_pairs_spec_non_member_swap();
	tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr();
	tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_rv();
	tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y();
	tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y_rv();
	tc_libcxx_utilities_util_smartptr_shared_assign_unique_ptr_Y();
	tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr();
	tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_pointer();
	tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_rv();
	tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y();
	tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y_rv();
}
