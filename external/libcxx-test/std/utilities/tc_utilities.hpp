/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#ifndef __EXTERNAL_LIBCXX_TEST_TC_UTILITIES_H
#define __EXTERNAL_LIBCXX_TEST_TC_UTILITIES_H
int tc_libcxx_utilities_util_smartptr_shared_assign_auto_ptr_Y(void);
int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr(void);
int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y(void);
int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y_rv(void);
int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_rv(void);
int tc_libcxx_utilities_util_smartptr_shared_assign_unique_ptr_Y(void);
int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr(void);
int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y(void);
int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y_rv(void);
int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_pointer(void);
int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_rv(void);
int tc_libcxx_utilities_pair_astuple_get_const(void);
int tc_libcxx_utilities_pair_astuple_get_non_const(void);
int tc_libcxx_utilities_pair_astuple_get_rv(void);
int tc_libcxx_utilities_pair_astuple_tuple_element(void);
int tc_libcxx_utilities_pair_astuple_tuple_size(void);
int tc_libcxx_utilities_pair_piecewise_piecewise_construct(void);
int tc_libcxx_utilities_pairs_pair_U_V(void);
int tc_libcxx_utilities_pairs_pair_assign_const_pair_U_V(void);
int tc_libcxx_utilities_pairs_pair_assign_rv_pair(void);
int tc_libcxx_utilities_pairs_pair_assign_rv_pair_U_V(void);
int tc_libcxx_utilities_pairs_pair_const_first_const_second(void);
int tc_libcxx_utilities_pairs_pair_const_pair_U_V(void);
int tc_libcxx_utilities_pairs_pair_copy_ctor(void);
int tc_libcxx_utilities_pairs_pair_default(void);
int tc_libcxx_utilities_pairs_pair_piecewise(void);
int tc_libcxx_utilities_pairs_pair_rv_pair_U_V(void);
int tc_libcxx_utilities_pairs_pair_swap(void);
int tc_libcxx_utilities_pairs_pair_types(void);
int tc_libcxx_utilities_pairs_spec_comparison(void);
int tc_libcxx_utilities_pairs_spec_make_pair(void);
int tc_libcxx_utilities_pairs_spec_non_member_swap(void);
#endif /* __EXTERNAL_LIBCXX_TEST_TC_UTILITIES_H */
