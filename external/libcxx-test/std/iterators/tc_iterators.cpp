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

#include "libcxx_tc_common.h"
#include "tc_iterators.hpp"

extern "C"
{
	int tc_iterators_main(void)
	{
		tc_libcxx_iterators_iterator_basic_iterator();
		tc_libcxx_iterators_iterator_operations_advance();
		tc_libcxx_iterators_iterator_operations_distance();
		tc_libcxx_iterators_iterator_operations_next();
		tc_libcxx_iterators_iterator_operations_prev();
		tc_libcxx_iterators_iterator_traits_const_pointer();
		tc_libcxx_iterators_iterator_traits_empty();
		tc_libcxx_iterators_iterator_traits_iterator();
		tc_libcxx_iterators_iterator_traits_pointer();
		tc_libcxx_iterators_std_iterator_tags_bidirectional_iterator_tag();
		tc_libcxx_iterators_std_iterator_tags_forward_iterator_tag();
		tc_libcxx_iterators_std_iterator_tags_input_iterator_tag();
		tc_libcxx_iterators_std_iterator_tags_output_iterator_tag();
		tc_libcxx_iterators_std_iterator_tags_random_access_iterator_tag();
		tc_libcxx_iterators_back_insert_iter_cons_container();
		tc_libcxx_iterators_back_insert_iter_op___post();
		tc_libcxx_iterators_back_insert_iter_op___pre();
		tc_libcxx_iterators_back_insert_iter_op__lv_value();
		tc_libcxx_iterators_back_insert_iter_op__rv_value();
		tc_libcxx_iterators_back_insert_iter_op_astrk_test();
		tc_libcxx_iterators_back_inserter_test();
		tc_libcxx_iterators_back_insert_iter_ops_nothing_to_do();
		tc_libcxx_iterators_front_insert_iter_cons_container();
		tc_libcxx_iterators_front_insert_iter_op___post();
		tc_libcxx_iterators_front_insert_iter_op___pre();
		tc_libcxx_iterators_front_insert_iter_op__lv_value();
		tc_libcxx_iterators_front_insert_iter_op__rv_value();
		tc_libcxx_iterators_front_insert_iter_op_astrk_test();
		tc_libcxx_iterators_front_inserter_test();
		tc_libcxx_iterators_front_insert_iter_ops_nothing_to_do();
		tc_libcxx_iterators_insert_iter_cons_test();
		tc_libcxx_iterators_insert_iter_op___post();
		tc_libcxx_iterators_insert_iter_op___pre();
		tc_libcxx_iterators_insert_iter_op__lv_value();
		tc_libcxx_iterators_insert_iter_op__rv_value();
		tc_libcxx_iterators_insert_iter_op_astrk_test();
		tc_libcxx_iterators_inserter_test();
		tc_libcxx_iterators_move_iter_nonmember_make_move_iterator();
		tc_libcxx_iterators_move_iter_nonmember_minus();
		tc_libcxx_iterators_move_iter_nonmember_plus();
		tc_libcxx_iterators_move_iter_op___difference_type();
		tc_libcxx_iterators_move_iter_op____difference_type();
		tc_libcxx_iterators_move_iter_op___difference_type();
		tc_libcxx_iterators_move_iter_op____difference_type();
		tc_libcxx_iterators_move_iter_op_comp_op_eq();
		tc_libcxx_iterators_move_iter_op_comp_op_gt();
		tc_libcxx_iterators_move_iter_op_comp_op_gte();
		tc_libcxx_iterators_move_iter_op_comp_op_lt();
		tc_libcxx_iterators_move_iter_op_comp_op_lte();
		tc_libcxx_iterators_move_iter_op_comp_op_neq();
		tc_libcxx_iterators_move_iter_op_const_convert();
		tc_libcxx_iterators_move_iter_op_const_default();
		tc_libcxx_iterators_move_iter_op_const_iter();
		tc_libcxx_iterators_move_iter_op_conv_tested_elsewhere();
		tc_libcxx_iterators_move_iter_op_decr_post();
		tc_libcxx_iterators_move_iter_op_decr_pre();
		tc_libcxx_iterators_move_iter_op_incr_post();
		tc_libcxx_iterators_move_iter_op_incr_pre();
		tc_libcxx_iterators_move_iter_op_index_difference_type();
		tc_libcxx_iterators_move_iter_op_ref_op_arrow();
		tc_libcxx_iterators_move_iter_op_star_op_star();
		tc_libcxx_iterators_move_iter_op__move_iterator();
		tc_libcxx_iterators_reverse_iter_cons_default();
		tc_libcxx_iterators_reverse_iter_cons_iter();
		tc_libcxx_iterators_reverse_iter_cons_reverse_iterator();
		tc_libcxx_iterators_reverse_iter_conv_tested_elsewhere();
		tc_libcxx_iterators_reverse_iter_op___test();
		tc_libcxx_iterators_reverse_iter_op___post();
		tc_libcxx_iterators_reverse_iter_op___pre();
		tc_libcxx_iterators_reverse_iter_op__difference_type();
		tc_libcxx_iterators_reverse_iter_op___difference_type();
		tc_libcxx_iterators_reverse_iter_op___post();
		tc_libcxx_iterators_reverse_iter_op___pre();
		tc_libcxx_iterators_reverse_iter_op__difference_type();
		tc_libcxx_iterators_reverse_iter_op___difference_type();
		tc_libcxx_iterators_reverse_iter_op_star_op_star();
		tc_libcxx_iterators_reverse_iter_op__reverse_iterator();
		tc_libcxx_iterators_reverse_iter_op___test();
		tc_libcxx_iterators_reverse_iter_opdiff_test();
		tc_libcxx_iterators_reverse_iter_opgt_test();
		tc_libcxx_iterators_reverse_iter_opgt__test();
		tc_libcxx_iterators_reverse_iter_opindex_difference_type();
		tc_libcxx_iterators_reverse_iter_oplt_test();
		tc_libcxx_iterators_reverse_iter_oplt__test();
		tc_libcxx_iterators_reverse_iter_opref_op_arrow();
		tc_libcxx_iterators_reverse_iter_opsum_difference_type();
		tc_libcxx_iterators_reverse_iter_requirements_nothing_to_do();
		tc_libcxx_iterators_istream_iterator_cons_copy();
		tc_libcxx_iterators_istream_iterator_cons_default();
		tc_libcxx_iterators_istream_iterator_cons_istream();
		tc_libcxx_iterators_istream_iterator_ops_arrow();
		tc_libcxx_iterators_istream_iterator_ops_dereference();
		tc_libcxx_iterators_istream_iterator_ops_equal();
		tc_libcxx_iterators_istream_iterator_ops_post_increment();
		tc_libcxx_iterators_istream_iterator_ops_pre_increment();
		tc_libcxx_iterators_istreambuf_iterator_cons_default();
		tc_libcxx_iterators_istreambuf_iterator_cons_istream();
		tc_libcxx_iterators_istreambuf_iterator_cons_proxy();
		tc_libcxx_iterators_istreambuf_iterator_cons_streambuf();
		tc_libcxx_iterators_istreambuf_iterator_equal_equal();
		tc_libcxx_iterators_istreambuf_iterator_op___not_equal();
		tc_libcxx_iterators_istreambuf_iterator_op___dereference();
		tc_libcxx_iterators_istreambuf_iterator_op___equal();
		tc_libcxx_iterators_istreambuf_iterator_op_astrk_post_increment();
		tc_libcxx_iterators_istreambuf_iterator_op_astrk_pre_increment();
		tc_libcxx_iterators_istreambuf_iterator_proxy_proxy();
		tc_libcxx_iterators_iterator_range_begin_array();
		tc_libcxx_iterators_iterator_range_begin_const();
		tc_libcxx_iterators_iterator_range_begin_non_const();
		tc_libcxx_iterators_iterator_range_end_array();
		tc_libcxx_iterators_iterator_range_end_const();
		tc_libcxx_iterators_iterator_range_end_non_const();
		tc_libcxx_iterators_ostream_iterator_cons_des_copy();
		tc_libcxx_iterators_ostream_iterator_cons_des_ostream();
		tc_libcxx_iterators_ostream_iterator_ops_assign_t();
		tc_libcxx_iterators_ostream_iterator_ops_dereference();
		tc_libcxx_iterators_ostream_iterator_ops_increment();
		tc_libcxx_iterators_ostreambuf_iter_cons_ostream();
		tc_libcxx_iterators_ostreambuf_iter_cons_streambuf();
		tc_libcxx_iterators_ostreambuf_iter_ops_assign_c();
		tc_libcxx_iterators_ostreambuf_iter_ops_deref();
		tc_libcxx_iterators_ostreambuf_iter_ops_failed();
		tc_libcxx_iterators_ostreambuf_iter_ops_increment();
		return 0;
	}
}
