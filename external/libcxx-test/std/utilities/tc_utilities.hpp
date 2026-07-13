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

#ifndef TC_UTILITIES_HPP
#define TC_UTILITIES_HPP

#include <tinyara/config.h>
#include "libcxx_tc_common.h"

extern int tc_libcxx_utilities_pair_astuple_get_const(void);
extern int tc_libcxx_utilities_pair_astuple_get_non_const(void);
extern int tc_libcxx_utilities_pair_astuple_get_rv(void);
extern int tc_libcxx_utilities_pair_astuple_tuple_element(void);
extern int tc_libcxx_utilities_pair_astuple_tuple_size(void);
extern int tc_libcxx_utilities_pair_piecewise_piecewise_construct(void);
extern int tc_libcxx_utilities_pairs_pair_assign_const_pair_U_V(void);
extern int tc_libcxx_utilities_pairs_pair_assign_rv_pair(void);
extern int tc_libcxx_utilities_pairs_pair_assign_rv_pair_U_V(void);
extern int tc_libcxx_utilities_pairs_pair_const_first_const_second(void);
extern int tc_libcxx_utilities_pairs_pair_const_pair_U_V(void);
extern int tc_libcxx_utilities_pairs_pair_copy_ctor(void);
extern int tc_libcxx_utilities_pairs_pair_default(void);
extern int tc_libcxx_utilities_pairs_pair_piecewise(void);
extern int tc_libcxx_utilities_pairs_pair_rv_pair_U_V(void);
extern int tc_libcxx_utilities_pairs_pair_swap(void);
extern int tc_libcxx_utilities_pairs_pair_types(void);
extern int tc_libcxx_utilities_pairs_pair_U_V(void);
extern int tc_libcxx_utilities_pairs_spec_comparison(void);
extern int tc_libcxx_utilities_pairs_spec_make_pair(void);
extern int tc_libcxx_utilities_pairs_spec_non_member_swap(void);

/* ============================================================================
 * ADDITIONAL PAIR TESTS (C++11+)
 * ============================================================================ */
/* pair.astuple */
extern int tc_utilities_utility_pairs_pair_astuple_get_const_rv(void);
extern int tc_utilities_utility_pairs_pair_astuple_pairs_by_type(void);

/* pair.piecewise */
extern int tc_utilities_utility_pairs_pair_piecewise_piecewise_construct_t(void);

/* pairs.pair - constructors (C++11+) */
extern int tc_utilities_utility_pairs_pairs_pair_ctor_default(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_move(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_U_V(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_piecewise_construct(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_copy_move_trivial(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_default_sfinae_LWG2367(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_move(void);
extern int tc_utilities_utility_pairs_pairs_pair_dtor(void);

/* pairs.pair - assignment (C++11+) */
extern int tc_utilities_utility_pairs_pairs_pair_assign_pair(void);
// extern int tc_utilities_utility_pairs_pairs_pair_assign_const_move_convert(void);  // Commented: const-correctness incompatibility with TizenRT libcxx
extern int tc_utilities_utility_pairs_pairs_pair_special_member_generation_test(void);

/* ============================================================================
 * ADDITIONAL PAIR TESTS (C++14+)
 * ============================================================================ */
#if TEST_STD_VER >= 14
extern int tc_utilities_utility_pairs_pairs_pair_ctor_copy(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_const_first_const_second_cxx03(void);
#endif /* TEST_STD_VER >= 14 */

/* ============================================================================
 * ADDITIONAL PAIR TESTS (C++17+)
 * ============================================================================ */
#if TEST_STD_VER >= 17
extern int tc_utilities_utility_pairs_pairs_pair_ctor_brace_init_P1951(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_const_ref_cxx03(void);
extern int tc_utilities_utility_pairs_pairs_general_nothing_to_do(void);
#endif /* TEST_STD_VER >= 17 */

/* ============================================================================
 * ADDITIONAL PAIR TESTS (C++20+)
 * ============================================================================ */
#if TEST_STD_VER >= 20
extern int tc_utilities_utility_pairs_pairs_pair_ctor_brace_init(void);
extern int tc_utilities_utility_pairs_pairs_pair_ctor_pair_U_V_ref(void);
extern int tc_utilities_utility_pairs_pairs_pair_assign_const_copy_pair(void);
extern int tc_utilities_utility_pairs_pairs_pair_assign_const_copy_convert(void);
extern int tc_utilities_utility_pairs_pairs_pair_assign_const_move_pair(void);
extern int tc_utilities_utility_pairs_pairs_pair_swap_member_const(void);
extern int tc_utilities_utility_pairs_pairs_spec_non_member_const_swap(void);
extern int tc_utilities_utility_pairs_pairs_pair_implicit_deduction_guides(void);
extern int tc_utilities_utility_pairs_pairs_pair_assign_pair_cxx03(void);
extern int tc_utilities_utility_pairs_pairs_spec_three_way_comparison(void);
#endif /* TEST_STD_VER >= 20 */

extern int tc_libcxx_utilities_util_smartptr_shared_assign_auto_ptr_Y(void);
extern int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr(void);
extern int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_rv(void);
extern int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y(void);
extern int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y_rv(void);
extern int tc_libcxx_utilities_util_smartptr_shared_assign_unique_ptr_Y(void);
extern int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr(void);
extern int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_pointer(void);
extern int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_rv(void);
extern int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y(void);
extern int tc_libcxx_utilities_util_smartptr_shared_const_shared_ptr_Y_rv(void);

/* ============================================================================
 * OPTIONAL TESTS (C++17+)
 * ============================================================================ */
#if TEST_STD_VER >= 17
/* optional.bad_optional_access */
extern int tc_utilities_optional_optional_bad_optional_access_default(void);
extern int tc_utilities_optional_optional_bad_optional_access_derive_pass(void);

/* optional.comp_with_t */
#if 0 /* Commented out - compare_three_way.pass.cpp not found in ported files */
extern int tc_utilities_optional_optional_comp_with_t_compare_three_way(void);
#endif
extern int tc_utilities_optional_optional_comp_with_t_equal(void);
extern int tc_utilities_optional_optional_comp_with_t_greater_equal(void);
extern int tc_utilities_optional_optional_comp_with_t_greater(void);
extern int tc_utilities_optional_optional_comp_with_t_less_equal(void);
extern int tc_utilities_optional_optional_comp_with_t_less_than(void);
extern int tc_utilities_optional_optional_comp_with_t_not_equal(void);

/* optional.hash */
extern int tc_utilities_optional_optional_hash_enabled_hash(void);
extern int tc_utilities_optional_optional_hash_hash(void);

/* optional.monadic (C++23) */
#if TEST_STD_VER > 20
extern int tc_utilities_optional_optional_monadic_and_then_pass(void);
extern int tc_utilities_optional_optional_monadic_or_else_pass(void);
extern int tc_utilities_optional_optional_monadic_transform_pass(void);
#endif

/* optional.nullops */
#if 0 /* Commented out - compare_three_way.pass.cpp not found in ported files */
extern int tc_utilities_optional_optional_nullops_compare_three_way(void);
#endif
extern int tc_utilities_optional_optional_nullops_equal(void);
extern int tc_utilities_optional_optional_nullops_greater_equal(void);
extern int tc_utilities_optional_optional_nullops_greater(void);
extern int tc_utilities_optional_optional_nullops_less_equal(void);
extern int tc_utilities_optional_optional_nullops_less_than(void);
extern int tc_utilities_optional_optional_nullops_not_equal(void);

/* optional.nullopt */
extern int tc_utilities_optional_optional_nullopt_nullopt_t(void);
#if 0 /* Commented out - compile_fail test, special handling needed */
extern int tc_utilities_optional_optional_nullopt_nullopt_t_compile_fail(void);
#endif

/* optional.object */
#if 0 /* Commented out - .verify.cpp files cause compile failures due to static_assert on invalid types */
extern int tc_utilities_optional_optional_object_optional_requires_destructible_object_verify(void);
#endif
extern int tc_utilities_optional_optional_object_special_members(void);
extern int tc_utilities_optional_optional_object_triviality(void);
extern int tc_utilities_optional_optional_object_types(void);

/* optional.object.assign */
extern int tc_utilities_optional_optional_object_optional_object_assign_assign_value(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_const_optional_U(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_copy(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_emplace_initializer_list_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_emplace(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_move(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_nullopt_t(void);
extern int tc_utilities_optional_optional_object_optional_object_assign_optional_U_pass(void);

/* optional.object.ctor */
extern int tc_utilities_optional_optional_object_optional_object_ctor_const_optional_U(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_const_T(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_copy(void);
#if 0 /* Commented out - .verify.cpp files cause compile failures */
extern int tc_utilities_optional_optional_object_optional_object_ctor_ctor_verify(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_deduct(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_deduct_verify(void);
#endif
extern int tc_utilities_optional_optional_object_optional_object_ctor_default(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_empty_in_place_t_does_not_clobber_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_explicit_const_optional_U_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_explicit_optional_U_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_in_place_t(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_initializer_list_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_move(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_nullopt_t(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_optional_U(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_rvalue_T(void);
extern int tc_utilities_optional_optional_object_optional_object_ctor_U(void);

/* optional.object.dtor */
extern int tc_utilities_optional_optional_object_optional_object_dtor_dtor(void);

/* optional.object.mod */
extern int tc_utilities_optional_optional_object_optional_object_mod_reset(void);

/* optional.object.observe */
extern int tc_utilities_optional_optional_object_optional_object_observe_bool(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_dereference_const_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_dereference_const_rvalue_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_dereference(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_dereference_rvalue_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_has_value(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_op_arrow_const_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_op_arrow(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_value_const_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_value_const_rvalue_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_value_or_const_pass(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_value_or(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_value(void);
extern int tc_utilities_optional_optional_object_optional_object_observe_value_rvalue_pass(void);

/* optional.object.swap */
extern int tc_utilities_optional_optional_object_optional_object_swap_swap(void);

/* optional.relops */
#if 0 /* Commented out - compare_three_way.pass.cpp not found in ported files */
extern int tc_utilities_optional_optional_relops_compare_three_way(void);
#endif
extern int tc_utilities_optional_optional_relops_equal(void);
extern int tc_utilities_optional_optional_relops_greater_equal_pass(void);
extern int tc_utilities_optional_optional_relops_greater_than_pass(void);
extern int tc_utilities_optional_optional_relops_less_equal_pass(void);
extern int tc_utilities_optional_optional_relops_less_than(void);
extern int tc_utilities_optional_optional_relops_not_equal(void);

/* optional.specalg */
extern int tc_utilities_optional_optional_specalg_make_optional_explicit_initializer_list_pass(void);
extern int tc_utilities_optional_optional_specalg_make_optional_explicit_pass(void);
extern int tc_utilities_optional_optional_specalg_make_optional(void);
extern int tc_utilities_optional_optional_specalg_swap(void);

/* optional.syn */
#if 0 /* Commented out - .verify.cpp files cause compile failures */
extern int tc_utilities_optional_optional_syn_optional_in_place_t_verify(void);
extern int tc_utilities_optional_optional_syn_optional_nullopt_t_verify(void);
#endif
extern int tc_utilities_optional_optional_syn_optional_includes_initializer_list_pass(void);
#endif /* TEST_STD_VER >= 17 */

/* ============================================================================
 * VARIANT TESTS (C++17+)
 * ============================================================================ */
#if TEST_STD_VER >= 17
/* variant.bad_variant_access */
extern int tc_utilities_variant_bad_variant_access(void);

/* variant.general */
extern int tc_utilities_variant_general_nothing_to_do(void);

/* variant.get */
extern int tc_utilities_variant_get_if_index(void);
extern int tc_utilities_variant_get_if_type(void);
extern int tc_utilities_variant_get_index(void);
extern int tc_utilities_variant_get_type(void);
extern int tc_utilities_variant_holds_alternative(void);

/* variant.hash */
extern int tc_utilities_variant_hash_enabled(void);
extern int tc_utilities_variant_hash(void);

/* variant.helpers */
extern int tc_utilities_variant_helper_alternative(void);
// extern int tc_utilities_variant_helper_alternative_verify(void);  // .verify.cpp - compile-time test
extern int tc_utilities_variant_helper_size(void);

/* variant.monostate */
extern int tc_utilities_variant_monostate(void);

/* variant.monostate.relops */
extern int tc_utilities_variant_monostate_relops(void);

/* variant.relops */
// extern int tc_utilities_variant_relops_bool_conv_verify(void);  // .verify.cpp - compile-time test
extern int tc_utilities_variant_relops(void);
#if TEST_STD_VER > 17
extern int tc_utilities_variant_relops_three_way(void);
#endif

/* variant.synopsis */
extern int tc_utilities_variant_synopsis_npos(void);

/* variant.variant */
extern int tc_utilities_variant_implicit_ctad(void);
// extern int tc_utilities_variant_array_verify(void);  // .verify.cpp - compile-time test
// extern int tc_utilities_variant_empty_verify(void);  // .verify.cpp - compile-time test
// extern int tc_utilities_variant_reference_verify(void);  // .verify.cpp - compile-time test
// extern int tc_utilities_variant_void_verify(void);  // .verify.cpp - compile-time test

/* variant.variant.assign */
extern int tc_utilities_variant_assign_conv(void);
extern int tc_utilities_variant_assign_copy(void);
// extern int tc_utilities_variant_assign_copy_verify(void);  // .verify.cpp - compile-time test
extern int tc_utilities_variant_assign_move(void);
extern int tc_utilities_variant_assign_T(void);

/* variant.variant.ctor */
extern int tc_utilities_variant_ctor_conv(void);
extern int tc_utilities_variant_ctor_copy(void);
extern int tc_utilities_variant_ctor_default(void);
extern int tc_utilities_variant_ctor_in_place_index_args(void);
extern int tc_utilities_variant_ctor_in_place_index_init_list_args(void);
extern int tc_utilities_variant_ctor_in_place_type_args(void);
extern int tc_utilities_variant_ctor_in_place_type_init_list_args(void);
extern int tc_utilities_variant_ctor_move(void);
extern int tc_utilities_variant_ctor_T(void);

/* variant.variant.dtor */
extern int tc_utilities_variant_dtor(void);

/* variant.variant.mod */
extern int tc_utilities_variant_mod_emplace_index_args(void);
extern int tc_utilities_variant_mod_emplace_index_init_list_args(void);
extern int tc_utilities_variant_mod_emplace_type_args(void);
extern int tc_utilities_variant_mod_emplace_type_init_list_args(void);

/* variant.variant.status */
extern int tc_utilities_variant_status_index(void);
extern int tc_utilities_variant_status_valueless(void);

/* variant.variant.swap */
extern int tc_utilities_variant_swap(void);

/* variant.visit */
extern int tc_utilities_variant_visit_robust_adl(void);
extern int tc_utilities_variant_visit_return_type(void);
extern int tc_utilities_variant_visit(void);
#endif /* TEST_STD_VER >= 17 */

/* ============================================================================
 * ANY TESTS (C++17+) - Commented out due to missing implementations
 * ============================================================================ */
#if 0 /* Disabled - missing test function implementations */
#if TEST_STD_VER >= 17
/* any.class.any.assign */
extern int tc_utilities_any_any_class_any_assign_copy_pass(void);
extern int tc_utilities_any_any_class_any_assign_move_pass(void);
extern int tc_utilities_any_any_class_any_assign_value_pass(void);

/* any.class.any.cons */
extern int tc_utilities_any_any_class_any_cons_copy_pass(void);
extern int tc_utilities_any_any_class_any_cons_default_pass(void);
extern int tc_utilities_any_any_class_any_cons_in_place_type_pass(void);
extern int tc_utilities_any_any_class_any_cons_move_pass(void);
extern int tc_utilities_any_any_class_any_cons_value_pass(void);

/* any.class.any.modifiers */
extern int tc_utilities_any_any_class_any_modifiers_emplace_pass(void);
extern int tc_utilities_any_any_class_any_modifiers_reset_pass(void);
extern int tc_utilities_any_any_class_any_modifiers_swap_pass(void);

/* any.class.any.observers */
extern int tc_utilities_any_any_class_any_observers_has_value_pass(void);
extern int tc_utilities_any_any_class_any_observers_type_pass(void);

/* any.nonmembers */
extern int tc_utilities_any_any_nonmembers_make_any_pass(void);
extern int tc_utilities_any_any_nonmembers_swap_pass(void);

/* any.nonmembers.any.cast */
extern int tc_utilities_any_any_nonmembers_any_cast_any_cast_pointer_pass(void);
extern int tc_utilities_any_any_nonmembers_any_cast_any_cast_reference_pass(void);
extern int tc_utilities_any_any_nonmembers_any_cast_any_cast_request_invalid_value_category_verify(void);
extern int tc_utilities_any_any_nonmembers_any_cast_const_correctness_verify(void);
extern int tc_utilities_any_any_nonmembers_any_cast_not_copy_constructible_verify(void);
extern int tc_utilities_any_any_nonmembers_any_cast_reference_types_verify(void);
#endif /* TEST_STD_VER >= 17 */
#endif /* Disabled - missing test function implementations */

/* ============================================================================
 * META TESTS (Type Traits) - Commented out due to missing implementations
 * ============================================================================ */
#if 0 /* Disabled - missing test function implementations */
/* meta.const.eval */
extern int tc_utilities_meta_meta_const_eval_is_constant_evaluated_pass(void);

/* meta.help */
extern int tc_utilities_meta_meta_help_bool_constant_pass(void);
extern int tc_utilities_meta_meta_help_integral_constant_pass(void);

/* meta.logical */
extern int tc_utilities_meta_meta_logical_negation_pass(void);

/* meta.rel */
extern int tc_utilities_meta_meta_rel_is_base_of_union_pass(void);
extern int tc_utilities_meta_meta_rel_is_base_of_pass(void);
extern int tc_utilities_meta_meta_rel_is_convertible_fallback_pass(void);
extern int tc_utilities_meta_meta_rel_is_convertible_pass(void);
extern int tc_utilities_meta_meta_rel_is_invocable_pass(void);
extern int tc_utilities_meta_meta_rel_is_nothrow_convertible_pass(void);
extern int tc_utilities_meta_meta_rel_is_nothrow_invocable_pass(void);
extern int tc_utilities_meta_meta_rel_is_same_pass(void);

/* meta.rqmts */
extern int tc_utilities_meta_meta_rqmts_nothing_to_do_pass(void);

/* meta.trans.arr */
extern int tc_utilities_meta_meta_trans_arr_remove_all_extents_pass(void);
extern int tc_utilities_meta_meta_trans_arr_remove_extent_pass(void);

/* meta.trans.cv */
extern int tc_utilities_meta_meta_trans_cv_add_const_pass(void);
extern int tc_utilities_meta_meta_trans_cv_add_cv_pass(void);
extern int tc_utilities_meta_meta_trans_cv_add_volatile_pass(void);
extern int tc_utilities_meta_meta_trans_cv_remove_const_pass(void);
extern int tc_utilities_meta_meta_trans_cv_remove_cv_pass(void);
extern int tc_utilities_meta_meta_trans_cv_remove_volatile_pass(void);

/* meta.trans.other */
extern int tc_utilities_meta_meta_trans_other_aligned_storage_pass(void);
extern int tc_utilities_meta_meta_trans_other_aligned_union_pass(void);
extern int tc_utilities_meta_meta_trans_other_common_type_pass(void);
extern int tc_utilities_meta_meta_trans_other_conditional_pass(void);
extern int tc_utilities_meta_meta_trans_other_decay_pass(void);
extern int tc_utilities_meta_meta_trans_other_enable_if_pass(void);
extern int tc_utilities_meta_meta_trans_other_remove_cvref_pass(void);
extern int tc_utilities_meta_meta_trans_other_result_of_pass(void);
extern int tc_utilities_meta_meta_trans_other_result_of11_pass(void);
extern int tc_utilities_meta_meta_trans_other_type_identity_pass(void);
extern int tc_utilities_meta_meta_trans_other_underlying_type_pass(void);

/* meta.trans.ptr */
extern int tc_utilities_meta_meta_trans_ptr_add_pointer_pass(void);
extern int tc_utilities_meta_meta_trans_ptr_remove_pointer_pass(void);

/* meta.trans.ref */
extern int tc_utilities_meta_meta_trans_ref_add_lvalue_ref_pass(void);
extern int tc_utilities_meta_meta_trans_ref_add_rvalue_ref_pass(void);
extern int tc_utilities_meta_meta_trans_ref_remove_ref_pass(void);

/* meta.trans.sign */
extern int tc_utilities_meta_meta_trans_sign_make_signed_pass(void);
extern int tc_utilities_meta_meta_trans_sign_make_unsigned_pass(void);

/* meta.type.synop */
extern int tc_utilities_meta_meta_type_synop_nothing_to_do_pass(void);

/* meta.unary.cat */
extern int tc_utilities_meta_meta_unary_cat_array_pass(void);
extern int tc_utilities_meta_meta_unary_cat_class_pass(void);
extern int tc_utilities_meta_meta_unary_cat_enum_pass(void);
extern int tc_utilities_meta_meta_unary_cat_floating_point_pass(void);
extern int tc_utilities_meta_meta_unary_cat_function_pass(void);
extern int tc_utilities_meta_meta_unary_cat_integral_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_array_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_class_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_enum_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_floating_point_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_function_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_integral_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_lvalue_reference_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_member_object_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_member_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_null_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_rvalue_reference_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_union_pass(void);
extern int tc_utilities_meta_meta_unary_cat_is_void_pass(void);
extern int tc_utilities_meta_meta_unary_cat_lvalue_ref_pass(void);
extern int tc_utilities_meta_meta_unary_cat_member_function_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_member_object_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_nullptr_pass(void);
extern int tc_utilities_meta_meta_unary_cat_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_cat_rvalue_ref_pass(void);
extern int tc_utilities_meta_meta_unary_cat_union_pass(void);
extern int tc_utilities_meta_meta_unary_cat_void_pass(void);

/* meta.unary.comp */
extern int tc_utilities_meta_meta_unary_comp_arithmetic_pass(void);
extern int tc_utilities_meta_meta_unary_comp_bounded_array_pass(void);
extern int tc_utilities_meta_meta_unary_comp_compound_pass(void);
extern int tc_utilities_meta_meta_unary_comp_fundamental_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_arithmetic_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_bounded_array_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_compound_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_fundamental_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_member_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_object_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_reference_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_scalar_pass(void);
extern int tc_utilities_meta_meta_unary_comp_is_unbounded_array_pass(void);
extern int tc_utilities_meta_meta_unary_comp_lvalue_ref_pass(void);
extern int tc_utilities_meta_meta_unary_comp_member_function_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_comp_member_object_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_comp_pointer_pass(void);
extern int tc_utilities_meta_meta_unary_comp_rvalue_ref_pass(void);
extern int tc_utilities_meta_meta_unary_comp_union_pass(void);
extern int tc_utilities_meta_meta_unary_comp_void_pass(void);

/* meta.unary.prop */
extern int tc_utilities_meta_meta_unary_prop_has_unique_object_representations_pass(void);
extern int tc_utilities_meta_meta_unary_prop_has_virtual_destructor_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_abstract_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_aggregate_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_const_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_copy_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_copy_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_default_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_destructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_empty_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_final_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_literal_type_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_move_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_move_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_copy_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_copy_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_default_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_destructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_move_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_move_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_swappable_with_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_nothrow_swappable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_pod_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_polymorphic_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_scoped_enum_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_signed_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_standard_layout_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_swappable_include_order_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_swappable_with_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_swappable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivial_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_copy_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_copy_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_copyable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_default_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_destructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_move_assignable_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_trivially_move_constructible_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_unsigned_pass(void);
extern int tc_utilities_meta_meta_unary_prop_is_volatile_pass(void);

/* meta.unary.prop.query */
extern int tc_utilities_meta_meta_unary_prop_query_alignment_of_pass(void);
extern int tc_utilities_meta_meta_unary_prop_query_extent_pass(void);
extern int tc_utilities_meta_meta_unary_prop_query_rank_pass(void);
extern int tc_utilities_meta_meta_unary_prop_query_void_t_pass(void);
#endif /* Disabled - missing test function implementations */

/* ============================================================================
 * UTILITY TESTS - Commented out due to missing implementations
 * ============================================================================ */
#if 0 /* Disabled - missing test function implementations */
/* utility.as_const */
extern int tc_utilities_utility_as_const_as_const_pass(void);

/* utility.declval */
extern int tc_utilities_utility_declval_declval_pass(void);

/* utility.exchange */
extern int tc_utilities_utility_exchange_exchange_pass(void);

/* utility.forward */
extern int tc_utilities_utility_forward_forward_like_pass(void);
extern int tc_utilities_utility_forward_forward_pass(void);
extern int tc_utilities_utility_forward_move_if_noexcept_pass(void);
extern int tc_utilities_utility_forward_move_pass(void);

/* utility.operators */
extern int tc_utilities_utility_operators_rel_ops_pass(void);

/* utility.swap */
extern int tc_utilities_utility_swap_swap_array_pass(void);
extern int tc_utilities_utility_swap_swap_pass(void);

/* utility.underlying */
extern int tc_utilities_utility_underlying_to_underlying_pass(void);

/* utility.unreachable */
extern int tc_utilities_utility_unreachable_assert_unreachable_pass(void);
extern int tc_utilities_utility_unreachable_unreachable_pass(void);

/* utility.synopsis */
extern int tc_utilities_utility_synopsis_pass(void);

/* utility.inplace */
extern int tc_utilities_utility_inplace_inplace_pass(void);

/* utility.intcmp (C++20) */
#if TEST_STD_VER >= 20
extern int tc_utilities_utility_intcmp_cmp_equal_pass(void);
extern int tc_utilities_utility_intcmp_cmp_greater_equal_pass(void);
extern int tc_utilities_utility_intcmp_cmp_greater_pass(void);
extern int tc_utilities_utility_intcmp_cmp_less_equal_pass(void);
extern int tc_utilities_utility_intcmp_cmp_less_pass(void);
extern int tc_utilities_utility_intcmp_cmp_not_equal_pass(void);
extern int tc_utilities_utility_intcmp_in_range_pass(void);
#endif
#endif /* Disabled - missing test function implementations */

#endif /* TC_UTILITIES_HPP */
