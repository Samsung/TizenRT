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

#include "tc_utilities.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <exception>

// Custom terminate handler for debugging crashes
void debug_terminate_handler() {
    printf("\n[CRITICAL] std::terminate() called!\n");
    printf("[DBG] This indicates an unhandled exception or explicit terminate call.\n");
    printf("[DBG] Check the last test that reported PASS for clues.\n");
    fflush(stdout);
    abort();
}

// Install the terminate handler at startup
struct TerminateHandlerInstaller {
    TerminateHandlerInstaller() {
        printf("[DBG] Installing custom terminate handler...\n");
        fflush(stdout);
        std::set_terminate(debug_terminate_handler);
    }
};
static TerminateHandlerInstaller g_terminate_handler_installer;

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

		/* pair.astuple */
		{ tc_utilities_utility_pairs_pair_astuple_get_const_rv(); }
		{ tc_utilities_utility_pairs_pair_astuple_pairs_by_type(); }

		/* pair.piecewise */
		{ tc_utilities_utility_pairs_pair_piecewise_piecewise_construct_t(); }

		/* pairs.pair - constructors */
		{ tc_utilities_utility_pairs_pairs_pair_ctor_default(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_move(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_U_V(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_piecewise_construct(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_copy_move_trivial(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_default_sfinae_LWG2367(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_move(); }
		{ tc_utilities_utility_pairs_pairs_pair_dtor(); }

		/* pairs.pair - assignment */
		{ tc_utilities_utility_pairs_pairs_pair_assign_pair(); }
		// { tc_utilities_utility_pairs_pairs_pair_assign_const_move_convert(); }  // Commented: const-correctness incompatibility with TizenRT libcxx
		{ tc_utilities_utility_pairs_pairs_pair_special_member_generation_test(); }

#if TEST_STD_VER >= 14
		/* C++14+ tests */
		// { tc_utilities_utility_pairs_pairs_pair_ctor_copy(); }   //commented out
		//{ tc_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03(); }    //commented out
#endif /* TEST_STD_VER >= 14 */

#if TEST_STD_VER >= 17
		/* C++17+ tests */
		{ tc_utilities_utility_pairs_pairs_pair_ctor_brace_init_P1951(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref_cxx03(); }
		{ tc_utilities_utility_pairs_pairs_general_nothing_to_do(); }
#endif /* TEST_STD_VER >= 17 */

#if TEST_STD_VER >= 20
		/* C++20+ tests */
		{ tc_utilities_utility_pairs_pairs_pair_ctor_brace_init(); }
		{ tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_ref(); }
		{ tc_utilities_utility_pairs_pairs_pair_assign_const_copy_pair(); }
		{ tc_utilities_utility_pairs_pairs_pair_assign_const_copy_convert(); }
		{ tc_utilities_utility_pairs_pairs_pair_assign_const_move_pair(); }
		{ tc_utilities_utility_pairs_pairs_pair_swap_member_const(); }
		{ tc_utilities_utility_pairs_pairs_spec_non_member_const_swap(); }
		{ tc_utilities_utility_pairs_pairs_pair_implicit_deduction_guides(); }
		{ tc_utilities_utility_pairs_pairs_pair_assign_pair_cxx03(); }
		{ tc_utilities_utility_pairs_pairs_spec_three_way_comparison(); }
#endif /* TEST_STD_VER >= 20 */
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

	void tc_utilities_optional()
	{
#if TEST_STD_VER >= 17
		printf("\nStarting [UTILITIES_OPTIONAL] Tests (C++17+)\n");
		fflush(stdout);
		
		/* optional.bad_optional_access */
		{ tc_utilities_optional_optional_bad_optional_access_default(); }
		{ tc_utilities_optional_optional_bad_optional_access_derive_pass(); }
		
		/* optional.comp_with_t */
#if 0 /* Commented out - compare_three_way.pass.cpp not found */
		{ tc_utilities_optional_optional_comp_with_t_compare_three_way(); }
#endif
		{ tc_utilities_optional_optional_comp_with_t_equal(); }
		{ tc_utilities_optional_optional_comp_with_t_greater_equal(); }
		{ tc_utilities_optional_optional_comp_with_t_greater(); }
		{ tc_utilities_optional_optional_comp_with_t_less_equal(); }
		{ tc_utilities_optional_optional_comp_with_t_less_than(); }
		{ tc_utilities_optional_optional_comp_with_t_not_equal(); }
		
		/* optional.hash */
		{ tc_utilities_optional_optional_hash_enabled_hash(); }
		{ tc_utilities_optional_optional_hash_hash(); }
		
		/* optional.monadic (C++23) */
#if TEST_STD_VER > 20
		{ tc_utilities_optional_optional_monadic_and_then_pass(); }
		{ tc_utilities_optional_optional_monadic_or_else_pass(); }
		{ tc_utilities_optional_optional_monadic_transform_pass(); }
#endif
		
		/* optional.nullops */
#if 0 /* Commented out - compare_three_way.pass.cpp not found */
		{ tc_utilities_optional_optional_nullops_compare_three_way(); }
#endif
		{ tc_utilities_optional_optional_nullops_equal(); }
		{ tc_utilities_optional_optional_nullops_greater_equal(); }
		{ tc_utilities_optional_optional_nullops_greater(); }
		{ tc_utilities_optional_optional_nullops_less_equal(); }
		{ tc_utilities_optional_optional_nullops_less_than(); }
		{ tc_utilities_optional_optional_nullops_not_equal(); }
		
		/* optional.nullopt */
#if 0 /* Commented out - compile_fail test, special handling needed */
		{ tc_utilities_optional_optional_nullopt_nullopt_t_compile_fail(); }
#endif
		{ tc_utilities_optional_optional_nullopt_nullopt_t(); }
		
		/* optional.object */
#if 0 /* Commented out - .verify.cpp files cause compile failures */
		{ tc_utilities_optional_optional_object_optional_requires_destructible_object_verify(); }
#endif
		{ tc_utilities_optional_optional_object_special_members(); }
		{ tc_utilities_optional_optional_object_triviality(); }
		{ tc_utilities_optional_optional_object_types(); }
		
		/* optional.object.assign */
		{ tc_utilities_optional_optional_object_optional_object_assign_assign_value(); }
		{ tc_utilities_optional_optional_object_optional_object_assign_const_optional_U(); }
		{ tc_utilities_optional_optional_object_optional_object_assign_copy(); }
		{ tc_utilities_optional_optional_object_optional_object_assign_emplace_initializer_list_pass(); }
#if 0 /* Commented out */
		{ tc_utilities_optional_optional_object_optional_object_assign_emplace(); }
#endif
#ifndef _LIBCPP_HAS_GCC10_BUGS
		{ tc_utilities_optional_optional_object_optional_object_assign_move(); }
#endif
		{ tc_utilities_optional_optional_object_optional_object_assign_nullopt_t(); }
		{ tc_utilities_optional_optional_object_optional_object_assign_optional_U_pass(); }
		
		/* optional.object.ctor */
		{ tc_utilities_optional_optional_object_optional_object_ctor_const_optional_U(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_const_T(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_copy(); }
#if 0 /* Commented out - .verify.cpp files cause compile failures */
		{ tc_utilities_optional_optional_object_optional_object_ctor_ctor_verify(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_deduct(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_deduct_verify(); }
#endif
		{ tc_utilities_optional_optional_object_optional_object_ctor_default(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_empty_in_place_t_does_not_clobber_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_explicit_const_optional_U_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_explicit_optional_U_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_in_place_t(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_initializer_list_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_move(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_nullopt_t(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_optional_U(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_rvalue_T(); }
		{ tc_utilities_optional_optional_object_optional_object_ctor_U(); }
		
		/* optional.object.dtor */
		{ tc_utilities_optional_optional_object_optional_object_dtor_dtor(); }
		
		/* optional.object.mod */
		{ tc_utilities_optional_optional_object_optional_object_mod_reset(); }
		
		/* optional.object.observe */
		{ tc_utilities_optional_optional_object_optional_object_observe_bool(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_dereference_const_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_dereference_const_rvalue_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_dereference(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_dereference_rvalue_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_has_value(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_op_arrow_const_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_op_arrow(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_value_const_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_value_const_rvalue_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_value_or_const_pass(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_value_or(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_value(); }
		{ tc_utilities_optional_optional_object_optional_object_observe_value_rvalue_pass(); }
		
		/* optional.object.swap */
		{ tc_utilities_optional_optional_object_optional_object_swap_swap(); }
		
		/* optional.relops */
#if 0 /* Commented out - compare_three_way.pass.cpp not found */
		{ tc_utilities_optional_optional_relops_compare_three_way(); }
#endif
		{ tc_utilities_optional_optional_relops_equal(); }
		{ tc_utilities_optional_optional_relops_greater_equal_pass(); }
		{ tc_utilities_optional_optional_relops_greater_than_pass(); }
		{ tc_utilities_optional_optional_relops_less_equal_pass(); }
		{ tc_utilities_optional_optional_relops_less_than(); }
		{ tc_utilities_optional_optional_relops_not_equal(); }
		
		/* optional.specalg */
		{ tc_utilities_optional_optional_specalg_make_optional_explicit_initializer_list_pass(); }
		{ tc_utilities_optional_optional_specalg_make_optional_explicit_pass(); }
		{ tc_utilities_optional_optional_specalg_make_optional(); }
		{ tc_utilities_optional_optional_specalg_swap(); }
		
		/* optional.syn */
#if 0 /* Commented out - .verify.cpp files cause compile failures */
		{ tc_utilities_optional_optional_syn_optional_in_place_t_verify(); }
		{ tc_utilities_optional_optional_syn_optional_nullopt_t_verify(); }
#endif
		{ tc_utilities_optional_optional_syn_optional_includes_initializer_list_pass(); }
		
		printf("\nCompleted [UTILITIES_OPTIONAL] Tests\n");
		fflush(stdout);
#endif /* TEST_STD_VER >= 17 */
	}


	void tc_utilities_variant()
	{
		/* variant.bad_variant_access */
		{ tc_utilities_variant_bad_variant_access(); }
		
		/* variant.general */
		{ tc_utilities_variant_general_nothing_to_do(); }
		
		/* variant.get */
		{ tc_utilities_variant_get_if_index(); }
		{ tc_utilities_variant_get_if_type(); }
		{ tc_utilities_variant_get_index(); }
		{ tc_utilities_variant_get_type(); }
		{ tc_utilities_variant_holds_alternative(); }
		
		/* variant.hash */
		{ tc_utilities_variant_hash_enabled(); }
		{ tc_utilities_variant_hash(); }
		
		/* variant.helpers */
		{ tc_utilities_variant_helper_alternative(); }
		{ tc_utilities_variant_helper_alternative_verify(); }
		{ tc_utilities_variant_helper_size(); }
		
		/* variant.monostate */
		{ tc_utilities_variant_monostate(); }
		
		/* variant.monostate.relops */
		{ tc_utilities_variant_monostate_relops(); }
		
		/* variant.relops */
		// Negative test case: verifies that code should NOT compile (not compiling = pass). 
		// { tc_utilities_variant_relops_bool_conv_verify(); }
		{ tc_utilities_variant_relops(); }
#if TEST_STD_VER > 17
		{ tc_utilities_variant_relops_three_way(); }
#endif
		
		/* variant.synopsis */
		{ tc_utilities_variant_synopsis_npos(); }
		
		/* variant.variant */
		{ tc_utilities_variant_implicit_ctad(); }
		// Negative test case: verifies that code should NOT compile (not compiling = pass).
		// { tc_utilities_variant_array_verify(); }
		// { tc_utilities_variant_empty_verify(); }
		// { tc_utilities_variant_reference_verify(); }
		// { tc_utilities_variant_void_verify(); }
		
		/* variant.variant.assign */
		{ tc_utilities_variant_assign_conv(); }
		//{ tc_utilities_variant_assign_copy(); }
		{ tc_utilities_variant_assign_copy_verify(); }
#ifndef _LIBCPP_HAS_GCC10_BUGS
		{ tc_utilities_variant_assign_move(); }
#endif
#if TEST_STD_VER >= 17
		// { tc_utilities_variant_assign_T(); }
#endif /* TEST_STD_VER >= 17 */
		
		/* variant.variant.ctor */
		{ tc_utilities_variant_ctor_conv(); }
		{ tc_utilities_variant_ctor_copy(); }
#ifndef _LIBCPP_HAS_GCC10_BUGS
		{ tc_utilities_variant_ctor_default(); }
#endif
		{ tc_utilities_variant_ctor_in_place_index_args(); }
		{ tc_utilities_variant_ctor_in_place_index_init_list_args(); }
		{ tc_utilities_variant_ctor_in_place_type_args(); }
		{ tc_utilities_variant_ctor_in_place_type_init_list_args(); }
		{ tc_utilities_variant_ctor_move(); }
		{ tc_utilities_variant_ctor_T(); }
		
		/* variant.variant.dtor */
		{ tc_utilities_variant_dtor(); }
		
		/* variant.variant.mod */
		{ tc_utilities_variant_mod_emplace_index_args(); }
		{ tc_utilities_variant_mod_emplace_index_init_list_args(); }
		{ tc_utilities_variant_mod_emplace_type_args(); }
		{ tc_utilities_variant_mod_emplace_type_init_list_args(); }
		
		/* variant.variant.status */
		{ tc_utilities_variant_status_index(); }
		{ tc_utilities_variant_status_valueless(); }
		
#ifndef _LIBCPP_HAS_GCC10_BUGS
		/* variant.variant.swap */
		{ tc_utilities_variant_swap(); }
#endif
		/* variant.visit */
		//{ tc_utilities_variant_visit(); } // Commented: causes board crash/reboot during test execution
		{ tc_utilities_variant_visit_robust_adl(); }
#if TEST_STD_VER >= 20
		{ tc_utilities_variant_visit_return_type(); }
#endif /* TEST_STD_VER >= 20 */
	}
	
#if TEST_STD_VER >= 17
	void tc_utilities_any()
	{
		printf("\nStarting [UTILITIES : ANY] Tests (C++17+)\n");
		fflush(stdout);
		
		/* any.class.any.assign */
		{ tc_any_assign_copy(); }
		{ tc_any_assign_move(); }
		{ tc_any_assign_value(); }
		
		/* any.class.any.cons */
		{ tc_any_cons_copy(); }
		{ tc_any_cons_default(); }
		{ tc_any_cons_in_place_type(); }
		{ tc_any_cons_move(); }
		{ tc_any_cons_value(); }
		
		/* any.class.any.modifiers */
		{ tc_any_emplace(); }
		{ tc_any_reset(); }
		{ tc_any_swap_member(); }
		
		/* any.class.any.observers */
		{ tc_any_has_value(); }
		{ tc_any_type(); }
		
		/* any.nonmembers */
		{ tc_any_make_any(); }
		{ tc_any_swap_nonmember(); }
		
		/* any.nonmembers.any.cast */
		{ tc_any_cast_pointer(); }
		{ tc_any_cast_reference(); }
		{ tc_any_cast_invalid_value_category_verify(); }
		{ tc_any_cast_const_correctness_verify(); }
		{ tc_any_cast_not_copy_constructible_verify(); }
		{ tc_any_cast_reference_types_verify(); }
		
		printf("\nCompleted [UTILITIES_ANY] Tests\n");
		fflush(stdout);
	}

#endif /* TEST_STD_VER >= 17 */

#if 0 /* Disabled - missing test function implementations */
	void tc_utilities_meta()
	{
		printf("\nStarting [UTILITIES_META] Tests (Type Traits)\n");
		fflush(stdout);
		
		/* meta.const.eval */
		{ tc_utilities_meta_meta_const_eval_is_constant_evaluated_pass(); }
		
		/* meta.help */
		{ tc_utilities_meta_meta_help_bool_constant_pass(); }
		{ tc_utilities_meta_meta_help_integral_constant_pass(); }
		
		/* meta.logical */
		{ tc_utilities_meta_meta_logical_negation_pass(); }
		
		/* meta.rel */
		{ tc_utilities_meta_meta_rel_is_base_of_union_pass(); }
		{ tc_utilities_meta_meta_rel_is_base_of_pass(); }
		{ tc_utilities_meta_meta_rel_is_convertible_fallback_pass(); }
		{ tc_utilities_meta_meta_rel_is_convertible_pass(); }
		{ tc_utilities_meta_meta_rel_is_invocable_pass(); }
		{ tc_utilities_meta_meta_rel_is_nothrow_convertible_pass(); }
		{ tc_utilities_meta_meta_rel_is_nothrow_invocable_pass(); }
		{ tc_utilities_meta_meta_rel_is_same_pass(); }
		
		/* meta.rqmts */
		{ tc_utilities_meta_meta_rqmts_nothing_to_do_pass(); }
		
		/* meta.trans.arr */
		{ tc_utilities_meta_meta_trans_arr_remove_all_extents_pass(); }
		{ tc_utilities_meta_meta_trans_arr_remove_extent_pass(); }
		
		/* meta.trans.cv */
		{ tc_utilities_meta_meta_trans_cv_add_const_pass(); }
		{ tc_utilities_meta_meta_trans_cv_add_cv_pass(); }
		{ tc_utilities_meta_meta_trans_cv_add_volatile_pass(); }
		{ tc_utilities_meta_meta_trans_cv_remove_const_pass(); }
		{ tc_utilities_meta_meta_trans_cv_remove_cv_pass(); }
		{ tc_utilities_meta_meta_trans_cv_remove_volatile_pass(); }
		
		/* meta.trans.other */
		{ tc_utilities_meta_meta_trans_other_aligned_storage_pass(); }
		{ tc_utilities_meta_meta_trans_other_aligned_union_pass(); }
		{ tc_utilities_meta_meta_trans_other_common_type_pass(); }
		{ tc_utilities_meta_meta_trans_other_conditional_pass(); }
		{ tc_utilities_meta_meta_trans_other_decay_pass(); }
		{ tc_utilities_meta_meta_trans_other_enable_if_pass(); }
		{ tc_utilities_meta_meta_trans_other_remove_cvref_pass(); }
		{ tc_utilities_meta_meta_trans_other_result_of_pass(); }
		{ tc_utilities_meta_meta_trans_other_result_of11_pass(); }
		{ tc_utilities_meta_meta_trans_other_type_identity_pass(); }
		{ tc_utilities_meta_meta_trans_other_underlying_type_pass(); }
		
		/* meta.trans.ptr */
		{ tc_utilities_meta_meta_trans_ptr_add_pointer_pass(); }
		{ tc_utilities_meta_meta_trans_ptr_remove_pointer_pass(); }
		
		/* meta.trans.ref */
		{ tc_utilities_meta_meta_trans_ref_add_lvalue_ref_pass(); }
		{ tc_utilities_meta_meta_trans_ref_add_rvalue_ref_pass(); }
		{ tc_utilities_meta_meta_trans_ref_remove_ref_pass(); }
		
		/* meta.trans.sign */
		{ tc_utilities_meta_meta_trans_sign_make_signed_pass(); }
		{ tc_utilities_meta_meta_trans_sign_make_unsigned_pass(); }
		
		/* meta.type.synop */
		{ tc_utilities_meta_meta_type_synop_nothing_to_do_pass(); }
		
		/* meta.unary.cat */
		{ tc_utilities_meta_meta_unary_cat_array_pass(); }
		{ tc_utilities_meta_meta_unary_cat_class_pass(); }
		{ tc_utilities_meta_meta_unary_cat_enum_pass(); }
		{ tc_utilities_meta_meta_unary_cat_floating_point_pass(); }
		{ tc_utilities_meta_meta_unary_cat_function_pass(); }
		{ tc_utilities_meta_meta_unary_cat_integral_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_array_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_class_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_enum_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_floating_point_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_function_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_integral_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_lvalue_reference_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_member_object_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_member_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_null_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_rvalue_reference_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_union_pass(); }
		{ tc_utilities_meta_meta_unary_cat_is_void_pass(); }
		{ tc_utilities_meta_meta_unary_cat_lvalue_ref_pass(); }
		{ tc_utilities_meta_meta_unary_cat_member_function_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_member_object_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_nullptr_pass(); }
		{ tc_utilities_meta_meta_unary_cat_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_cat_rvalue_ref_pass(); }
		{ tc_utilities_meta_meta_unary_cat_union_pass(); }
		{ tc_utilities_meta_meta_unary_cat_void_pass(); }
		
		/* meta.unary.comp */
		{ tc_utilities_meta_meta_unary_comp_arithmetic_pass(); }
		{ tc_utilities_meta_meta_unary_comp_bounded_array_pass(); }
		{ tc_utilities_meta_meta_unary_comp_compound_pass(); }
		{ tc_utilities_meta_meta_unary_comp_fundamental_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_arithmetic_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_bounded_array_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_compound_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_fundamental_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_member_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_object_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_reference_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_scalar_pass(); }
		{ tc_utilities_meta_meta_unary_comp_is_unbounded_array_pass(); }
		{ tc_utilities_meta_meta_unary_comp_lvalue_ref_pass(); }
		{ tc_utilities_meta_meta_unary_comp_member_function_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_comp_member_object_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_comp_pointer_pass(); }
		{ tc_utilities_meta_meta_unary_comp_rvalue_ref_pass(); }
		{ tc_utilities_meta_meta_unary_comp_union_pass(); }
		{ tc_utilities_meta_meta_unary_comp_void_pass(); }
		
		/* meta.unary.prop */
		{ tc_utilities_meta_meta_unary_prop_has_unique_object_representations_pass(); }
		{ tc_utilities_meta_meta_unary_prop_has_virtual_destructor_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_abstract_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_aggregate_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_const_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_copy_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_copy_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_default_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_destructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_empty_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_final_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_literal_type_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_move_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_move_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_copy_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_copy_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_default_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_destructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_move_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_move_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_swappable_with_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_nothrow_swappable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_pod_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_polymorphic_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_scoped_enum_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_signed_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_standard_layout_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_swappable_include_order_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_swappable_with_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_swappable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivial_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_copy_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_copy_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_copyable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_default_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_destructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_move_assignable_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_trivially_move_constructible_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_unsigned_pass(); }
		{ tc_utilities_meta_meta_unary_prop_is_volatile_pass(); }
		
		/* meta.unary.prop.query */
		{ tc_utilities_meta_meta_unary_prop_query_alignment_of_pass(); }
		{ tc_utilities_meta_meta_unary_prop_query_extent_pass(); }
		{ tc_utilities_meta_meta_unary_prop_query_rank_pass(); }
		{ tc_utilities_meta_meta_unary_prop_query_void_t_pass(); }
		
		printf("\nCompleted [UTILITIES_META] Tests\n");
		fflush(stdout);
	}

	void tc_utilities_utility()
	{
		printf("\nStarting [UTILITIES_UTILITY] Tests\n");
		fflush(stdout);
		
		/* utility.as_const */
		{ tc_utilities_utility_as_const_as_const_pass(); }
		
		/* utility.declval */
		{ tc_utilities_utility_declval_declval_pass(); }
		
		/* utility.exchange */
		{ tc_utilities_utility_exchange_exchange_pass(); }
		
		/* utility.forward */
		{ tc_utilities_utility_forward_forward_like_pass(); }
		{ tc_utilities_utility_forward_forward_pass(); }
		{ tc_utilities_utility_forward_move_if_noexcept_pass(); }
		{ tc_utilities_utility_forward_move_pass(); }
		
		/* utility.operators */
		{ tc_utilities_utility_operators_rel_ops_pass(); }
		
		/* utility.swap */
		{ tc_utilities_utility_swap_swap_array_pass(); }
		{ tc_utilities_utility_swap_swap_pass(); }
		
		/* utility.underlying */
		{ tc_utilities_utility_underlying_to_underlying_pass(); }
		
		/* utility.unreachable */
		{ tc_utilities_utility_unreachable_assert_unreachable_pass(); }
		{ tc_utilities_utility_unreachable_unreachable_pass(); }
		
		/* utility.synopsis */
		{ tc_utilities_utility_synopsis_pass(); }
		
		/* utility.inplace */
		{ tc_utilities_utility_inplace_inplace_pass(); }
		
		/* utility.intcmp (C++20) */
#if TEST_STD_VER >= 20
		{ tc_utilities_utility_intcmp_cmp_equal_pass(); }
		{ tc_utilities_utility_intcmp_cmp_greater_equal_pass(); }
		{ tc_utilities_utility_intcmp_cmp_greater_pass(); }
		{ tc_utilities_utility_intcmp_cmp_less_equal_pass(); }
		{ tc_utilities_utility_intcmp_cmp_less_pass(); }
		{ tc_utilities_utility_intcmp_cmp_not_equal_pass(); }
		{ tc_utilities_utility_intcmp_in_range_pass(); }
#endif
		
		printf("\nCompleted [UTILITIES_UTILITY] Tests\n");
		fflush(stdout);
	}
#endif /* Disabled - missing test function implementations */

	int tc_utilities_main(void)
	{
		printf("\nStarting [UTILITIES] Tests\n");
		fflush(stdout);

		tc_utilities_pair();
		usleep(1000);
		tc_utilities_smartptr();
		usleep(1000);

#if TEST_STD_VER >= 17
		tc_utilities_optional();
		usleep(1000);
		tc_utilities_variant();
		usleep(1000);
		tc_utilities_any();
		usleep(1000);
		//tc_utilities_meta();
		usleep(1000);
		//tc_utilities_utility();
		usleep(1000);
#endif /* TEST_STD_VER >= 17 */
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
#if TEST_STD_VER >= 17
		else if (strcmp(argv[1], "optional") == 0) {
			printf("\nRunning optional tests (C++17+)...\n");
			tc_utilities_optional();
		}
		else if (strcmp(argv[1], "variant") == 0) {
			printf("\nRunning variant tests (C++17+)...\n");
			tc_utilities_variant();
		}
		else if (strcmp(argv[1], "any") == 0) {
			printf("\nRunning any tests (C++17+)...\n");
			tc_utilities_any();
		}
		else if (strcmp(argv[1], "meta") == 0) {
			printf("\nRunning meta/type_traits tests...\n");
			//tc_utilities_meta();
		}
		else if (strcmp(argv[1], "utility") == 0) {
			printf("\nRunning utility tests...\n");
			//tc_utilities_utility();
		}
#endif /* TEST_STD_VER >= 17 */
		else {
			printf("\nUnknown utilities sub-category: %s\n", argv[1]);
			printf("Available: pair, smartptr");
#if 0 /* Disabled */
#if TEST_STD_VER >= 17
			printf(", optional, variant, any, meta, utility");
#endif /* TEST_STD_VER >= 17 */
#endif /* Disabled */
			printf("\n");
		}
	}
}
