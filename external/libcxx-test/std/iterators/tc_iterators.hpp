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

#ifndef __EXTERNAL_LIBCXX_TEST_TC_ITERATORS_H
#define __EXTERNAL_LIBCXX_TEST_TC_ITERATORS_H
int tc_libcxx_iterators_iterator_basic_iterator(void);
int tc_libcxx_iterators_iterator_operations_advance(void);
int tc_libcxx_iterators_iterator_operations_distance(void);
int tc_libcxx_iterators_iterator_operations_next(void);
int tc_libcxx_iterators_iterator_operations_prev(void);
int tc_libcxx_iterators_iterator_traits_const_pointer(void);
int tc_libcxx_iterators_iterator_traits_empty(void);
int tc_libcxx_iterators_iterator_traits_iterator(void);
int tc_libcxx_iterators_iterator_traits_pointer(void);
int tc_libcxx_iterators_std_iterator_tags_bidirectional_iterator_tag(void);
int tc_libcxx_iterators_std_iterator_tags_forward_iterator_tag(void);
int tc_libcxx_iterators_std_iterator_tags_input_iterator_tag(void);
int tc_libcxx_iterators_std_iterator_tags_output_iterator_tag(void);
int tc_libcxx_iterators_std_iterator_tags_random_access_iterator_tag(void);
int tc_libcxx_iterators_back_insert_iter_cons_container(void);
int tc_libcxx_iterators_back_insert_iter_op___post(void);
int tc_libcxx_iterators_back_insert_iter_op___pre(void);
int tc_libcxx_iterators_back_insert_iter_op__lv_value(void);
int tc_libcxx_iterators_back_insert_iter_op__rv_value(void);
int tc_libcxx_iterators_back_insert_iter_op_astrk_test(void);
int tc_libcxx_iterators_back_inserter_test(void);
int tc_libcxx_iterators_back_insert_iter_ops_nothing_to_do(void);
int tc_libcxx_iterators_front_insert_iter_cons_container(void);
int tc_libcxx_iterators_front_insert_iter_op___post(void);
int tc_libcxx_iterators_front_insert_iter_op___pre(void);
int tc_libcxx_iterators_front_insert_iter_op__lv_value(void);
int tc_libcxx_iterators_front_insert_iter_op__rv_value(void);
int tc_libcxx_iterators_front_insert_iter_op_astrk_test(void);
int tc_libcxx_iterators_front_inserter_test(void);
int tc_libcxx_iterators_front_insert_iter_ops_nothing_to_do(void);
int tc_libcxx_iterators_insert_iter_cons_test(void);
int tc_libcxx_iterators_insert_iter_op___post(void);
int tc_libcxx_iterators_insert_iter_op___pre(void);
int tc_libcxx_iterators_insert_iter_op__lv_value(void);
int tc_libcxx_iterators_insert_iter_op__rv_value(void);
int tc_libcxx_iterators_insert_iter_op_astrk_test(void);
int tc_libcxx_iterators_inserter_test(void);
int tc_libcxx_iterators_move_iter_nonmember_make_move_iterator(void);
int tc_libcxx_iterators_move_iter_nonmember_minus(void);
int tc_libcxx_iterators_move_iter_nonmember_plus(void);
int tc_libcxx_iterators_move_iter_op___difference_type(void);
int tc_libcxx_iterators_move_iter_op____difference_type(void);
int tc_libcxx_iterators_move_iter_op___difference_type(void);
int tc_libcxx_iterators_move_iter_op____difference_type(void);
int tc_libcxx_iterators_move_iter_op_comp_op_eq(void);
int tc_libcxx_iterators_move_iter_op_comp_op_gt(void);
int tc_libcxx_iterators_move_iter_op_comp_op_gte(void);
int tc_libcxx_iterators_move_iter_op_comp_op_lt(void);
int tc_libcxx_iterators_move_iter_op_comp_op_lte(void);
int tc_libcxx_iterators_move_iter_op_comp_op_neq(void);
int tc_libcxx_iterators_move_iter_op_const_convert(void);
int tc_libcxx_iterators_move_iter_op_const_default(void);
int tc_libcxx_iterators_move_iter_op_const_iter(void);
int tc_libcxx_iterators_move_iter_op_conv_tested_elsewhere(void);
int tc_libcxx_iterators_move_iter_op_decr_post(void);
int tc_libcxx_iterators_move_iter_op_decr_pre(void);
int tc_libcxx_iterators_move_iter_op_incr_post(void);
int tc_libcxx_iterators_move_iter_op_incr_pre(void);
int tc_libcxx_iterators_move_iter_op_index_difference_type(void);
int tc_libcxx_iterators_move_iter_op_ref_op_arrow(void);
int tc_libcxx_iterators_move_iter_op_star_op_star(void);
int tc_libcxx_iterators_move_iter_op__move_iterator(void);
int tc_libcxx_iterators_reverse_iter_cons_default(void);
int tc_libcxx_iterators_reverse_iter_cons_iter(void);
int tc_libcxx_iterators_reverse_iter_cons_reverse_iterator(void);
int tc_libcxx_iterators_reverse_iter_conv_tested_elsewhere(void);
int tc_libcxx_iterators_reverse_iter_op___test(void);
int tc_libcxx_iterators_reverse_iter_op___post(void);
int tc_libcxx_iterators_reverse_iter_op___pre(void);
int tc_libcxx_iterators_reverse_iter_op__difference_type(void);
int tc_libcxx_iterators_reverse_iter_op___difference_type(void);
int tc_libcxx_iterators_reverse_iter_op___post(void);
int tc_libcxx_iterators_reverse_iter_op___pre(void);
int tc_libcxx_iterators_reverse_iter_op__difference_type(void);
int tc_libcxx_iterators_reverse_iter_op___difference_type(void);
int tc_libcxx_iterators_reverse_iter_op_star_op_star(void);
int tc_libcxx_iterators_reverse_iter_op__reverse_iterator(void);
int tc_libcxx_iterators_reverse_iter_op___test(void);
int tc_libcxx_iterators_reverse_iter_opdiff_test(void);
int tc_libcxx_iterators_reverse_iter_opgt_test(void);
int tc_libcxx_iterators_reverse_iter_opgt__test(void);
int tc_libcxx_iterators_reverse_iter_opindex_difference_type(void);
int tc_libcxx_iterators_reverse_iter_oplt_test(void);
int tc_libcxx_iterators_reverse_iter_oplt__test(void);
int tc_libcxx_iterators_reverse_iter_opref_op_arrow(void);
int tc_libcxx_iterators_reverse_iter_opsum_difference_type(void);
int tc_libcxx_iterators_reverse_iter_requirements_nothing_to_do(void);
int tc_libcxx_iterators_istream_iterator_cons_copy(void);
int tc_libcxx_iterators_istream_iterator_cons_default(void);
int tc_libcxx_iterators_istream_iterator_cons_istream(void);
int tc_libcxx_iterators_istream_iterator_ops_arrow(void);
int tc_libcxx_iterators_istream_iterator_ops_dereference(void);
int tc_libcxx_iterators_istream_iterator_ops_equal(void);
int tc_libcxx_iterators_istream_iterator_ops_post_increment(void);
int tc_libcxx_iterators_istream_iterator_ops_pre_increment(void);
int tc_libcxx_iterators_istreambuf_iterator_cons_default(void);
int tc_libcxx_iterators_istreambuf_iterator_cons_istream(void);
int tc_libcxx_iterators_istreambuf_iterator_cons_proxy(void);
int tc_libcxx_iterators_istreambuf_iterator_cons_streambuf(void);
int tc_libcxx_iterators_istreambuf_iterator_equal_equal(void);
int tc_libcxx_iterators_istreambuf_iterator_op___not_equal(void);
int tc_libcxx_iterators_istreambuf_iterator_op___dereference(void);
int tc_libcxx_iterators_istreambuf_iterator_op___equal(void);
int tc_libcxx_iterators_istreambuf_iterator_op_astrk_post_increment(void);
int tc_libcxx_iterators_istreambuf_iterator_op_astrk_pre_increment(void);
int tc_libcxx_iterators_istreambuf_iterator_proxy_proxy(void);
int tc_libcxx_iterators_iterator_range_begin_array(void);
int tc_libcxx_iterators_iterator_range_begin_const(void);
int tc_libcxx_iterators_iterator_range_begin_non_const(void);
int tc_libcxx_iterators_iterator_range_end_array(void);
int tc_libcxx_iterators_iterator_range_end_const(void);
int tc_libcxx_iterators_iterator_range_end_non_const(void);
int tc_libcxx_iterators_ostream_iterator_cons_des_copy(void);
int tc_libcxx_iterators_ostream_iterator_cons_des_ostream(void);
int tc_libcxx_iterators_ostream_iterator_ops_assign_t(void);
int tc_libcxx_iterators_ostream_iterator_ops_dereference(void);
int tc_libcxx_iterators_ostream_iterator_ops_increment(void);
int tc_libcxx_iterators_ostreambuf_iter_cons_ostream(void);
int tc_libcxx_iterators_ostreambuf_iter_cons_streambuf(void);
int tc_libcxx_iterators_ostreambuf_iter_ops_assign_c(void);
int tc_libcxx_iterators_ostreambuf_iter_ops_deref(void);
int tc_libcxx_iterators_ostreambuf_iter_ops_failed(void);
int tc_libcxx_iterators_ostreambuf_iter_ops_increment(void);
#endif /* __EXTERNAL_LIBCXX_TEST_TC_ITERATORS_H */
