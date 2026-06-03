#include "tc_iterators.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern "C" {
#include <tc_common.h>
}

extern "C" {

	void tc_iterators_primitives()
	{
		{ tc_libcxx_iterators_iterator_basic_iterator(); }
		{ tc_libcxx_iterators_iterator_traits_const_pointer(); }
		{ tc_libcxx_iterators_iterator_traits_empty(); }
		{ tc_libcxx_iterators_iterator_traits_iterator(); }
		{ tc_libcxx_iterators_iterator_traits_pointer(); }
		{ tc_libcxx_iterators_std_iterator_tags_bidirectional_iterator_tag(); }
		{ tc_libcxx_iterators_std_iterator_tags_forward_iterator_tag(); }
		{ tc_libcxx_iterators_std_iterator_tags_input_iterator_tag(); }
		{ tc_libcxx_iterators_std_iterator_tags_output_iterator_tag(); }
		{ tc_libcxx_iterators_std_iterator_tags_random_access_iterator_tag(); }
	}

	void tc_iterators_operations()
	{
		{ tc_libcxx_iterators_iterator_operations_advance(); }
		{ tc_libcxx_iterators_iterator_operations_distance(); }
		{ tc_libcxx_iterators_iterator_operations_next(); }
		{ tc_libcxx_iterators_iterator_operations_prev(); }
		{ tc_libcxx_iterators_iterator_range_begin_array(); }
		{ tc_libcxx_iterators_iterator_range_begin_const(); }
		{ tc_libcxx_iterators_iterator_range_begin_non_const(); }
		{ tc_libcxx_iterators_iterator_range_end_array(); }
		{ tc_libcxx_iterators_iterator_range_end_const(); }
		{ tc_libcxx_iterators_iterator_range_end_non_const(); }
	}

	void tc_iterators_adaptors()
	{
		// back_insert_iterator
		{ tc_libcxx_iterators_back_insert_iter_cons_container(); }
		{ tc_libcxx_iterators_back_insert_iter_op___post(); }
		{ tc_libcxx_iterators_back_insert_iter_op___pre(); }
		{ tc_libcxx_iterators_back_insert_iter_op__lv_value(); }
		{ tc_libcxx_iterators_back_insert_iter_op__rv_value(); }
		{ tc_libcxx_iterators_back_insert_iter_op_astrk_test(); }
		{ tc_libcxx_iterators_back_insert_iter_ops_nothing_to_do(); }
		{ tc_libcxx_iterators_back_inserter_test(); }
		// front_insert_iterator
		{ tc_libcxx_iterators_front_insert_iter_cons_container(); }
		{ tc_libcxx_iterators_front_insert_iter_op___post(); }
		{ tc_libcxx_iterators_front_insert_iter_op___pre(); }
		{ tc_libcxx_iterators_front_insert_iter_op__lv_value(); }
		{ tc_libcxx_iterators_front_insert_iter_op__rv_value(); }
		{ tc_libcxx_iterators_front_insert_iter_op_astrk_test(); }
		{ tc_libcxx_iterators_front_insert_iter_ops_nothing_to_do(); }
		{ tc_libcxx_iterators_front_inserter_test(); }
		// insert_iterator
		{ tc_libcxx_iterators_insert_iter_cons_test(); }
		{ tc_libcxx_iterators_insert_iter_op___post(); }
		{ tc_libcxx_iterators_insert_iter_op___pre(); }
		{ tc_libcxx_iterators_insert_iter_op__lv_value(); }
		{ tc_libcxx_iterators_insert_iter_op__rv_value(); }
		{ tc_libcxx_iterators_insert_iter_op_astrk_test(); }
		{ tc_libcxx_iterators_inserter_test(); }
		// reverse_iterator
		{ tc_libcxx_iterators_reverse_iter_cons_default(); }
		{ tc_libcxx_iterators_reverse_iter_cons_iter(); }
		{ tc_libcxx_iterators_reverse_iter_cons_reverse_iterator(); }
		{ tc_libcxx_iterators_reverse_iter_conv_tested_elsewhere(); }
		{ tc_libcxx_iterators_reverse_iter_op___difference_type(); }
		{ tc_libcxx_iterators_reverse_iter_op___post(); }
		{ tc_libcxx_iterators_reverse_iter_op___pre(); }
		{ tc_libcxx_iterators_reverse_iter_op___test(); }
		{ tc_libcxx_iterators_reverse_iter_op__difference_type(); }
		{ tc_libcxx_iterators_reverse_iter_op__reverse_iterator(); }
		{ tc_libcxx_iterators_reverse_iter_op_star_op_star(); }
		{ tc_libcxx_iterators_reverse_iter_opdiff_test(); }
		{ tc_libcxx_iterators_reverse_iter_opgt__test(); }
		{ tc_libcxx_iterators_reverse_iter_opgt_test(); }
		{ tc_libcxx_iterators_reverse_iter_opindex_difference_type(); }
		{ tc_libcxx_iterators_reverse_iter_oplt__test(); }
		{ tc_libcxx_iterators_reverse_iter_oplt_test(); }
		{ tc_libcxx_iterators_reverse_iter_opref_op_arrow(); }
		{ tc_libcxx_iterators_reverse_iter_opsum_difference_type(); }
		{ tc_libcxx_iterators_reverse_iter_requirements_nothing_to_do(); }
		// move_iterator
		{ tc_libcxx_iterators_move_iter_nonmember_make_move_iterator(); }
		{ tc_libcxx_iterators_move_iter_nonmember_minus(); }
		{ tc_libcxx_iterators_move_iter_nonmember_plus(); }
		{ tc_libcxx_iterators_move_iter_op____difference_type(); }
		{ tc_libcxx_iterators_move_iter_op___difference_type(); }
		{ tc_libcxx_iterators_move_iter_op__move_iterator(); }
		{ tc_libcxx_iterators_move_iter_op_comp_op_eq(); }
		{ tc_libcxx_iterators_move_iter_op_comp_op_gt(); }
		{ tc_libcxx_iterators_move_iter_op_comp_op_gte(); }
		{ tc_libcxx_iterators_move_iter_op_comp_op_lt(); }
		{ tc_libcxx_iterators_move_iter_op_comp_op_lte(); }
		{ tc_libcxx_iterators_move_iter_op_comp_op_neq(); }
		{ tc_libcxx_iterators_move_iter_op_const_convert(); }
		{ tc_libcxx_iterators_move_iter_op_const_default(); }
		{ tc_libcxx_iterators_move_iter_op_const_iter(); }
		{ tc_libcxx_iterators_move_iter_op_conv_tested_elsewhere(); }
		{ tc_libcxx_iterators_move_iter_op_decr_post(); }
		{ tc_libcxx_iterators_move_iter_op_decr_pre(); }
		{ tc_libcxx_iterators_move_iter_op_incr_post(); }
		{ tc_libcxx_iterators_move_iter_op_incr_pre(); }
		{ tc_libcxx_iterators_move_iter_op_index_difference_type(); }
		{ tc_libcxx_iterators_move_iter_op_ref_op_arrow(); }
		{ tc_libcxx_iterators_move_iter_op_star_op_star(); }
	}

	void tc_iterators_stream()
	{
#ifdef CONFIG_LIBCXX_LOCALE
		// istream_iterator
		{ tc_libcxx_iterators_istream_iterator_cons_copy(); }
		{ tc_libcxx_iterators_istream_iterator_cons_default(); }
		{ tc_libcxx_iterators_istream_iterator_cons_istream(); }
		{ tc_libcxx_iterators_istream_iterator_ops_arrow(); }
		{ tc_libcxx_iterators_istream_iterator_ops_dereference(); }
		{ tc_libcxx_iterators_istream_iterator_ops_equal(); }
		{ tc_libcxx_iterators_istream_iterator_ops_post_increment(); }
		{ tc_libcxx_iterators_istream_iterator_ops_pre_increment(); }
		// istreambuf_iterator
		{ tc_libcxx_iterators_istreambuf_iterator_cons_default(); }
		{ tc_libcxx_iterators_istreambuf_iterator_cons_istream(); }
		{ tc_libcxx_iterators_istreambuf_iterator_cons_proxy(); }
		{ tc_libcxx_iterators_istreambuf_iterator_cons_streambuf(); }
		{ tc_libcxx_iterators_istreambuf_iterator_equal_equal(); }
		{ tc_libcxx_iterators_istreambuf_iterator_op___dereference(); }
		{ tc_libcxx_iterators_istreambuf_iterator_op___equal(); }
		{ tc_libcxx_iterators_istreambuf_iterator_op___not_equal(); }
		{ tc_libcxx_iterators_istreambuf_iterator_op_astrk_post_increment(); }
		{ tc_libcxx_iterators_istreambuf_iterator_op_astrk_pre_increment(); }
		{ tc_libcxx_iterators_istreambuf_iterator_proxy_proxy(); }
		// ostream_iterator
		{ tc_libcxx_iterators_ostream_iterator_cons_des_copy(); }
		{ tc_libcxx_iterators_ostream_iterator_cons_des_ostream(); }
		{ tc_libcxx_iterators_ostream_iterator_ops_assign_t(); }
		{ tc_libcxx_iterators_ostream_iterator_ops_dereference(); }
		{ tc_libcxx_iterators_ostream_iterator_ops_increment(); }
		// ostreambuf_iterator
		{ tc_libcxx_iterators_ostreambuf_iter_cons_ostream(); }
		{ tc_libcxx_iterators_ostreambuf_iter_cons_streambuf(); }
		{ tc_libcxx_iterators_ostreambuf_iter_ops_assign_c(); }
		{ tc_libcxx_iterators_ostreambuf_iter_ops_deref(); }
		{ tc_libcxx_iterators_ostreambuf_iter_ops_failed(); }
		{ tc_libcxx_iterators_ostreambuf_iter_ops_increment(); }
#endif
	}

	int tc_iterators_main(void)
	{
		printf("\nStarting [ITERATORS] Tests\n");
		fflush(stdout);

		tc_iterators_primitives();
		usleep(1000);
		tc_iterators_operations();
		usleep(1000);
		tc_iterators_adaptors();
		usleep(1000);
		tc_iterators_stream();
		usleep(1000);

		return 0;
	}

	// Menu function for ITERATORS sub-tests (called from utc_libcxx.c)
	void tc_iterators_menu(int argc, char *argv[])
	{
		// No arguments: run all iterators tests
		if (argc <= 1) {
			tc_iterators_main();
			return;
		}

		if (strcmp(argv[1], "primitives") == 0) {
			printf("\nRunning iterator.primitives tests...\n");
			tc_iterators_primitives();
		}
		else if (strcmp(argv[1], "operations") == 0) {
			printf("\nRunning iterator.operations tests...\n");
			tc_iterators_operations();
		}
		else if (strcmp(argv[1], "adaptors") == 0) {
			printf("\nRunning iterator.adaptors tests...\n");
			tc_iterators_adaptors();
		}
		else if (strcmp(argv[1], "stream") == 0) {
#ifdef CONFIG_LIBCXX_LOCALE
			printf("\nRunning stream.iterators tests...\n");
			tc_iterators_stream();
#else
			printf("\nstream.iterators tests require CONFIG_LIBCXX_LOCALE\n");
#endif
		}
		else {
			printf("\nUnknown iterators sub-category: %s\n", argv[1]);
			printf("Available: primitives, operations, adaptors, stream\n");
		}
	}
}
