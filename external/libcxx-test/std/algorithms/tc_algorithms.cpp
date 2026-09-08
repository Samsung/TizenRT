//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "libcxx_tc_common.h"
#include "tc_algorithms.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef TEST_STD_VER
#if __cplusplus >= 202302L
#define TEST_STD_VER 23
#elif __cplusplus >= 202002L
#define TEST_STD_VER 20
#elif __cplusplus >= 201703L
#define TEST_STD_VER 17
#elif __cplusplus >= 201402L
#define TEST_STD_VER 14
#elif __cplusplus >= 201103L
#define TEST_STD_VER 11
#else
#define TEST_STD_VER 98
#endif
#endif

extern "C"
{
	void tc_algorithms_mod()
	{
		printf("\nStarting [ALGORITHMS_MOD] Tests\n");
		fflush(stdout);
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_copy_copy(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_copy_copy_backward(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_copy_copy_if(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_copy_copy_n(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_fill_fill(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_fill_fill_n(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_generate_generate(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_generate_generate_n(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_move_move(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_move_move_backward(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_is_partitioned(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_partition(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_partition_copy(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_partition_point(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_stable_partition(); }
#if TEST_STD_VER > 14
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_random_sample_sample(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_random_sample_sample_stable(); }
#endif
		// Note: random_shuffle removed - std::random_shuffle was removed in C++17
		// Note: random_shuffle_rand removed - std::random_shuffle was removed in C++17
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_random_shuffle_random_shuffle_urng(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_remove_remove(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_remove_remove_copy(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_remove_remove_copy_if(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_remove_remove_if(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_replace_replace(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_replace_replace_copy(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_replace_replace_copy_if(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_replace_replace_if(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_reverse_reverse(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_reverse_reverse_copy(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_rotate_rotate(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_rotate_rotate_copy(); }
		// Note: shift_left removed - std::shift_left is C++23
		// Note: shift_right removed - std::shift_right is C++23
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_swap_iter_swap(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_transform_binary_transform(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_transform_unary_transform(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_unique_unique(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_unique_unique_copy(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_unique_unique_copy_pred(); }
		{ tc_libcxx_algorithms_alg_modifying_operations_alg_unique_unique_pred(); }
	}

	void tc_algorithms_NonMod()
	{
		printf("\nStarting [ALGORITHMS_NONMOD] Tests\n");
		fflush(stdout);
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_adjacent_find_adjacent_find(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_adjacent_find_adjacent_find_pred(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_all_of_all_of(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_any_of_any_of(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_count_count(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_count_count_if(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_equal_equal(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_equal_equal_pred(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_find(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_find_if(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_find_if_not(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_end_find_end(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_end_find_end_pred(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_first_of_find_first_of(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_find_first_of_find_first_of_pred(); }
#if TEST_STD_VER > 14
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_foreach_for_each_n(); }
#endif
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_foreach_test(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_is_permutation_is_permutation(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_is_permutation_is_permutation_pred(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_none_of_none_of(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_search_search(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_search_search_n(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_search_search_n_pred(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_alg_search_search_pred(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_mismatch_mismatch(); }
		{ tc_libcxx_algorithms_alg_nonmodifying_mismatch_mismatch_pred(); }
	}

	void tc_algorithms_Sort_BinarySearch()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_binary_search_binary_search(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_binary_search_binary_search_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_equal_range_equal_range(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_equal_range_equal_range_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_lower_bound_lower_bound(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_lower_bound_lower_bound_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_upper_bound_upper_bound(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_binary_search_upper_bound_upper_bound_comp(); }		
	}

	void tc_algorithms_Sort_Clamp()
	{
#if TEST_STD_VER > 14
		{ tc_libcxx_algorithms_alg_sorting_alg_clamp_clamp_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_clamp_clamp(); }
#endif
	}

	void tc_algorithms_Sort_HeapOperations()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap_until(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap_until_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_make_heap_make_heap(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_make_heap_make_heap_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_pop_heap_pop_heap(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_pop_heap_pop_heap_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_push_heap_push_heap(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_push_heap_push_heap_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_sort_heap_sort_heap(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_heap_operations_sort_heap_sort_heap_comp(); }		
	}

	void tc_algorithms_Sort_LexComparison()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_lex_comparison_lexicographical_compare(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_lex_comparison_lexicographical_compare_comp(); }
		// Note: lexicographical_compare_three_way removed - uses C++20 features
		// Note: lexicographical_compare_three_way_comp removed - uses C++20 features
	}

	void tc_algorithms_Sort_Merge()
	{
		//{ tc_libcxx_algorithms_alg_sorting_alg_merge_inplace_merge(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_merge_inplace_merge_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_merge_merge(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_merge_merge_comp(); }
	}

	void tc_algorithms_Sort_MinMax()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_max(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_max_comp(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_min_max_max_element(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_min_max_max_element_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_max_init_list(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_max_init_list_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_min(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_comp(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_element(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_element_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_init_list(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_init_list_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_min_comp(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_min_max_min_element(); }
		//{ tc_libcxx_algorithms_alg_sorting_alg_min_max_min_element_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_min_init_list(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_min_max_min_init_list_comp(); }
	}

	void tc_algorithms_Sort_NthElement()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_nth_element_nth_element(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_nth_element_nth_element_comp(); }
	}

	void tc_algorithms_Sort_PermutationGenerators()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_permutation_generators_next_permutation(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_permutation_generators_next_permutation_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_permutation_generators_prev_permutation(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_permutation_generators_prev_permutation_comp(); }
	}

	void tc_algorithms_Sort_SetOperations()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_includes_includes(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_includes_includes_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_difference_set_difference(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_difference_set_difference_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_intersection_set_intersection(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_intersection_set_intersection_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_symmetric_difference_set_symmetric_difference(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_symmetric_difference_set_symmetric_difference_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_union_set_union(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_union_set_union_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_set_operations_set_union_set_union_move(); }
	}

	void tc_algorithms_Sort_Sort()
	{
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_until(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_until_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_partial_sort_partial_sort(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_partial_sort_partial_sort_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_partial_sort_copy_partial_sort_copy(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_partial_sort_copy_partial_sort_copy_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort_constexpr(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort_constexpr_comp(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_stable_sort_stable_sort(); }
		{ tc_libcxx_algorithms_alg_sorting_alg_sort_stable_sort_stable_sort_comp(); }
	}

	void tc_algorithms_Sort()
	{
		printf("\nStarting [ALGORITHMS_SORT] Tests\n");
		fflush(stdout);
		
		tc_algorithms_Sort_BinarySearch();
		usleep(1000); 
		tc_algorithms_Sort_Clamp();
		usleep(1000); 
		tc_algorithms_Sort_HeapOperations();
		usleep(1000); 
		tc_algorithms_Sort_LexComparison();
		usleep(1000); 
		tc_algorithms_Sort_Merge();
		usleep(1000); 
		tc_algorithms_Sort_MinMax();
		usleep(1000); 
		tc_algorithms_Sort_NthElement();
		usleep(1000); 
		tc_algorithms_Sort_PermutationGenerators();
		usleep(1000); 
		tc_algorithms_Sort_SetOperations();
		usleep(1000); 
		tc_algorithms_Sort_Sort();
		usleep(1000); 
		
		printf("\nCompleted [ALGORITHMS_SORT] Tests\n");
		fflush(stdout);
	}


	int tc_algorithms_main(void)
	{
		printf("\nStarting [ALGORITHMS] Tests\n");
		fflush(stdout);
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS
		{ tc_libcxx_algorithms_robust_against_adl_on_new(); }
		usleep(1000); 
#endif

#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS_MOD
		tc_algorithms_mod();
		usleep(1000); 
#endif

#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS_NONMOD
		tc_algorithms_NonMod();
		usleep(1000); 
#endif

#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS_SORT
		tc_algorithms_Sort();
		usleep(1000); 
#endif

		return 0;
	}

	// SORT sub-category handler (called from tc_algorithms_menu)
	static void tc_algorithms_sort_handler(int argc, char *argv[])
	{
		// No sub-argument: run all sort tests
		if (argc <= 1) {
			tc_algorithms_Sort();
			return;
		}

		if (strcmp(argv[1], "binarysearch") == 0) {
			printf("\nRunning BinarySearch tests...\n");
			tc_algorithms_Sort_BinarySearch();
		}
		else if (strcmp(argv[1], "clamp") == 0) {
			printf("\nRunning Clamp tests...\n");
			tc_algorithms_Sort_Clamp();
		}
		else if (strcmp(argv[1], "heap") == 0) {
			printf("\nRunning HeapOperations tests...\n");
			tc_algorithms_Sort_HeapOperations();
		}
		else if (strcmp(argv[1], "lexcompare") == 0) {
			printf("\nRunning LexComparison tests...\n");
			tc_algorithms_Sort_LexComparison();
		}
		else if (strcmp(argv[1], "merge") == 0) {
			printf("\nRunning Merge tests...\n");
			tc_algorithms_Sort_Merge();
		}
		else if (strcmp(argv[1], "minmax") == 0) {
			printf("\nRunning MinMax tests...\n");
			tc_algorithms_Sort_MinMax();
		}
		else if (strcmp(argv[1], "nthelement") == 0) {
			printf("\nRunning NthElement tests...\n");
			tc_algorithms_Sort_NthElement();
		}
		else if (strcmp(argv[1], "permutation") == 0) {
			printf("\nRunning PermutationGenerators tests...\n");
			tc_algorithms_Sort_PermutationGenerators();
		}
		else if (strcmp(argv[1], "setops") == 0) {
			printf("\nRunning SetOperations tests...\n");
			tc_algorithms_Sort_SetOperations();
		}
		else if (strcmp(argv[1], "sort") == 0) {
			printf("\nRunning Sort tests...\n");
			tc_algorithms_Sort_Sort();
		}
		else {
			printf("\nUnknown sort sub-category: %s\n", argv[1]);
			printf("Available: binarysearch, clamp, heap, lexcompare, merge, minmax, nthelement, permutation, setops, sort\n");
		}
	}

	// Menu function for ALGORITHMS sub-tests (called from utc_libcxx.c)
	void tc_algorithms_menu(int argc, char *argv[])
	{
		// No arguments: run all algorithms tests
		if (argc <= 1) {
			tc_algorithms_main();
			return;
		}

		if (strcmp(argv[1], "mod") == 0) {
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS_MOD
			printf("\nRunning alg.modifying tests...\n");
			tc_algorithms_mod();
#else
			printf("\nALGORITHMS_MOD tests not enabled in config\n");
#endif
		}
		else if (strcmp(argv[1], "nonmod") == 0) {
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS_NONMOD
			printf("\nRunning alg.nonmodifying tests...\n");
			tc_algorithms_NonMod();
#else
			printf("\nALGORITHMS_NONMOD tests not enabled in config\n");
#endif
		}
		else if (strcmp(argv[1], "sort") == 0) {
#ifdef CONFIG_LIBCXX_UTC_ALGORITHMS_SORT
			tc_algorithms_sort_handler(argc - 1, &argv[1]);
#else
			printf("\nALGORITHMS_SORT tests not enabled in config\n");
#endif
		}
		else {
			printf("\nUnknown algorithms sub-category: %s\n", argv[1]);
			printf("Available: mod, nonmod, sort\n");
		}
	}
}
