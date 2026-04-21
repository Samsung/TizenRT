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

#ifndef __EXTERNAL_LIBCXX_TEST_STD_ALGORITHMS_TC_ALGORITHMS_HPP
#define __EXTERNAL_LIBCXX_TEST_STD_ALGORITHMS_TC_ALGORITHMS_HPP

// alg.c.library
int tc_algorithms_alg_c_library_tested_elsewhere(void);

// alg.modifying.operations/alg.copy
int tc_algorithms_alg_modifying_operations_alg_copy_copy(void);
int tc_algorithms_alg_modifying_operations_alg_copy_copy_backward(void);
int tc_algorithms_alg_modifying_operations_alg_copy_copy_if(void);
int tc_algorithms_alg_modifying_operations_alg_copy_copy_n(void);

// alg.modifying.operations/alg.fill
int tc_algorithms_alg_modifying_operations_alg_fill_fill(void);
int tc_algorithms_alg_modifying_operations_alg_fill_fill_n(void);

// alg.modifying.operations/alg.generate
int tc_algorithms_alg_modifying_operations_alg_generate_generate(void);
int tc_algorithms_alg_modifying_operations_alg_generate_generate_n(void);

// alg.modifying.operations/alg.move
int tc_algorithms_alg_modifying_operations_alg_move_contiguous_trivial_optimization(void);
int tc_algorithms_alg_modifying_operations_alg_move_move(void);
int tc_algorithms_alg_modifying_operations_alg_move_move_backward(void);

// alg.modifying.operations/alg.partitions
int tc_algorithms_alg_modifying_operations_alg_partitions_is_partitioned(void);
int tc_algorithms_alg_modifying_operations_alg_partitions_partition(void);
int tc_algorithms_alg_modifying_operations_alg_partitions_partition_copy(void);
int tc_algorithms_alg_modifying_operations_alg_partitions_partition_point(void);
int tc_algorithms_alg_modifying_operations_alg_partitions_stable_partition(void);

// alg.modifying.operations/alg.random.sample
int tc_algorithms_alg_modifying_operations_alg_random_sample_sample(void);
int tc_algorithms_alg_modifying_operations_alg_random_sample_sample_stable(void);

// alg.modifying.operations/alg.remove
int tc_algorithms_alg_modifying_operations_alg_remove_remove(void);
int tc_algorithms_alg_modifying_operations_alg_remove_remove_copy(void);
int tc_algorithms_alg_modifying_operations_alg_remove_remove_copy_if(void);
int tc_algorithms_alg_modifying_operations_alg_remove_remove_if(void);

// alg.modifying.operations/alg.replace
int tc_algorithms_alg_modifying_operations_alg_replace_replace(void);
int tc_algorithms_alg_modifying_operations_alg_replace_replace_copy(void);
int tc_algorithms_alg_modifying_operations_alg_replace_replace_copy_if(void);
int tc_algorithms_alg_modifying_operations_alg_replace_replace_if(void);

// alg.modifying.operations/alg.reverse
int tc_algorithms_alg_modifying_operations_alg_reverse_reverse(void);
int tc_algorithms_alg_modifying_operations_alg_reverse_reverse_copy(void);

// alg.modifying.operations/alg.rotate
int tc_algorithms_alg_modifying_operations_alg_rotate_rotate(void);
int tc_algorithms_alg_modifying_operations_alg_rotate_rotate_copy(void);

// alg.modifying.operations/alg.swap
int tc_algorithms_alg_modifying_operations_alg_swap_iter_swap(void);
int tc_algorithms_alg_modifying_operations_alg_swap_swap_ranges(void);

// alg.modifying.operations/alg.transform
int tc_algorithms_alg_modifying_operations_alg_transform_binary_transform(void);
int tc_algorithms_alg_modifying_operations_alg_transform_unary_transform(void);

// alg.modifying.operations/alg.unique
int tc_algorithms_alg_modifying_operations_alg_unique_unique(void);
int tc_algorithms_alg_modifying_operations_alg_unique_unique_copy(void);
int tc_algorithms_alg_modifying_operations_alg_unique_unique_copy_pred(void);
int tc_algorithms_alg_modifying_operations_alg_unique_unique_pred(void);

// alg.nonmodifying/alg.adjacent.find
int tc_algorithms_alg_nonmodifying_alg_adjacent_find_adjacent_find(void);
int tc_algorithms_alg_nonmodifying_alg_adjacent_find_adjacent_find_pred(void);

// alg.nonmodifying/alg.all_of
int tc_algorithms_alg_nonmodifying_alg_all_of_all_of(void);

// alg.nonmodifying/alg.any_of
int tc_algorithms_alg_nonmodifying_alg_any_of_any_of(void);

// alg.nonmodifying/alg.count
int tc_algorithms_alg_nonmodifying_alg_count_count(void);
int tc_algorithms_alg_nonmodifying_alg_count_count_if(void);

// alg.nonmodifying/alg.equal
int tc_algorithms_alg_nonmodifying_alg_equal_equal(void);
int tc_algorithms_alg_nonmodifying_alg_equal_equal_pred(void);

// alg.nonmodifying/alg.find
int tc_algorithms_alg_nonmodifying_alg_find_find(void);
int tc_algorithms_alg_nonmodifying_alg_find_find_if(void);
int tc_algorithms_alg_nonmodifying_alg_find_find_if_not(void);

// alg.nonmodifying/alg.find.end
int tc_algorithms_alg_nonmodifying_alg_find_end_find_end(void);
int tc_algorithms_alg_nonmodifying_alg_find_end_find_end_pred(void);

// alg.nonmodifying/alg.find.first.of
int tc_algorithms_alg_nonmodifying_alg_find_first_of_find_first_of(void);
int tc_algorithms_alg_nonmodifying_alg_find_first_of_find_first_of_pred(void);

// alg.nonmodifying/alg.foreach
int tc_algorithms_alg_nonmodifying_alg_foreach_for_each_n(void);
int tc_algorithms_alg_nonmodifying_alg_foreach_test(void);

// alg.nonmodifying/alg.is_permutation
int tc_algorithms_alg_nonmodifying_alg_is_permutation_is_permutation(void);
int tc_algorithms_alg_nonmodifying_alg_is_permutation_is_permutation_pred(void);

// alg.nonmodifying/alg.none_of
int tc_algorithms_alg_nonmodifying_alg_none_of_none_of(void);

// alg.nonmodifying/alg.search
int tc_algorithms_alg_nonmodifying_alg_search_search(void);
int tc_algorithms_alg_nonmodifying_alg_search_search_n(void);
int tc_algorithms_alg_nonmodifying_alg_search_search_n_pred(void);
int tc_algorithms_alg_nonmodifying_alg_search_search_pred(void);

// alg.nonmodifying/mismatch
int tc_algorithms_alg_nonmodifying_mismatch_mismatch(void);
int tc_algorithms_alg_nonmodifying_mismatch_mismatch_pred(void);

// alg.sorting/alg.binary.search/binary.search
int tc_algorithms_alg_sorting_alg_binary_search_binary_search_binary_search(void);
int tc_algorithms_alg_sorting_alg_binary_search_binary_search_binary_search_comp(void);

// alg.sorting/alg.binary.search/equal.range
int tc_algorithms_alg_sorting_alg_binary_search_equal_range_equal_range(void);
int tc_algorithms_alg_sorting_alg_binary_search_equal_range_equal_range_comp(void);

// alg.sorting/alg.binary.search/lower.bound
int tc_algorithms_alg_sorting_alg_binary_search_lower_bound_lower_bound(void);
int tc_algorithms_alg_sorting_alg_binary_search_lower_bound_lower_bound_comp(void);

// alg.sorting/alg.binary.search/upper.bound
int tc_algorithms_alg_sorting_alg_binary_search_upper_bound_upper_bound(void);
int tc_algorithms_alg_sorting_alg_binary_search_upper_bound_upper_bound_comp(void);

// alg.sorting/alg.clamp
int tc_algorithms_alg_sorting_alg_clamp_clamp_comp(void);
int tc_algorithms_alg_sorting_alg_clamp_clamp(void);

// alg.sorting/alg.heap.operations/is.heap
int tc_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap(void);
int tc_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap_comp(void);
int tc_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap_until(void);
int tc_algorithms_alg_sorting_alg_heap_operations_is_heap_is_heap_until_comp(void);

// alg.sorting/alg.heap.operations/make.heap
int tc_algorithms_alg_sorting_alg_heap_operations_make_heap_make_heap(void);
int tc_algorithms_alg_sorting_alg_heap_operations_make_heap_make_heap_comp(void);

// alg.sorting/alg.heap.operations/pop.heap
int tc_algorithms_alg_sorting_alg_heap_operations_pop_heap_pop_heap(void);
int tc_algorithms_alg_sorting_alg_heap_operations_pop_heap_pop_heap_comp(void);

// alg.sorting/alg.heap.operations/push.heap
int tc_algorithms_alg_sorting_alg_heap_operations_push_heap_push_heap(void);
int tc_algorithms_alg_sorting_alg_heap_operations_push_heap_push_heap_comp(void);

// alg.sorting/alg.heap.operations/sort.heap
int tc_algorithms_alg_sorting_alg_heap_operations_sort_heap_complexity(void);
int tc_algorithms_alg_sorting_alg_heap_operations_sort_heap_sort_heap(void);
int tc_algorithms_alg_sorting_alg_heap_operations_sort_heap_sort_heap_comp(void);

// alg.sorting/alg.lex.comparison
int tc_algorithms_alg_sorting_alg_lex_comparison_lexicographical_compare(void);
int tc_algorithms_alg_sorting_alg_lex_comparison_lexicographical_compare_comp(void);

// alg.sorting/alg.merge
int tc_algorithms_alg_sorting_alg_merge_inplace_merge(void);
int tc_algorithms_alg_sorting_alg_merge_inplace_merge_comp(void);
int tc_algorithms_alg_sorting_alg_merge_merge(void);
int tc_algorithms_alg_sorting_alg_merge_merge_comp(void);

// alg.sorting/alg.min.max
int tc_algorithms_alg_sorting_alg_min_max_max(void);
int tc_algorithms_alg_sorting_alg_min_max_max_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_max_element(void);
int tc_algorithms_alg_sorting_alg_min_max_max_element_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_max_init_list(void);
int tc_algorithms_alg_sorting_alg_min_max_max_init_list_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_min(void);
int tc_algorithms_alg_sorting_alg_min_max_minmax(void);
int tc_algorithms_alg_sorting_alg_min_max_minmax_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_minmax_element(void);
int tc_algorithms_alg_sorting_alg_min_max_minmax_element_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_minmax_init_list(void);
int tc_algorithms_alg_sorting_alg_min_max_minmax_init_list_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_min_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_min_element(void);
int tc_algorithms_alg_sorting_alg_min_max_min_element_comp(void);
int tc_algorithms_alg_sorting_alg_min_max_min_init_list(void);
int tc_algorithms_alg_sorting_alg_min_max_min_init_list_comp(void);

// alg.sorting/alg.nth.element
int tc_algorithms_alg_sorting_alg_nth_element_nth_element(void);
int tc_algorithms_alg_sorting_alg_nth_element_nth_element_comp(void);

// alg.sorting/alg.set.operations/includes
int tc_algorithms_alg_sorting_alg_set_operations_includes_includes(void);
int tc_algorithms_alg_sorting_alg_set_operations_includes_includes_comp(void);

// alg.sorting/alg.set.operations/set.difference
int tc_algorithms_alg_sorting_alg_set_operations_set_difference_set_difference(void);
int tc_algorithms_alg_sorting_alg_set_operations_set_difference_set_difference_comp(void);

// alg.sorting/alg.set.operations/set.intersection
int tc_algorithms_alg_sorting_alg_set_operations_set_intersection_set_intersection(void);
int tc_algorithms_alg_sorting_alg_set_operations_set_intersection_set_intersection_comp(void);

// alg.sorting/alg.set.operations/set.symmetric.difference
int tc_algorithms_alg_sorting_alg_set_operations_set_symmetric_difference_set_symmetric_difference(void);
int tc_algorithms_alg_sorting_alg_set_operations_set_symmetric_difference_set_symmetric_difference_comp(void);

// alg.sorting/alg.set.operations/set.union
int tc_algorithms_alg_sorting_alg_set_operations_set_union_set_union(void);
int tc_algorithms_alg_sorting_alg_set_operations_set_union_set_union_comp(void);
int tc_algorithms_alg_sorting_alg_set_operations_set_union_set_union_move(void);

// alg.sorting/alg.sort/is.sorted
int tc_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted(void);
int tc_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_comp(void);
int tc_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_until(void);
int tc_algorithms_alg_sorting_alg_sort_is_sorted_is_sorted_until_comp(void);

// alg.sorting/alg.sort/partial.sort
int tc_algorithms_alg_sorting_alg_sort_partial_sort_partial_sort(void);
int tc_algorithms_alg_sorting_alg_sort_partial_sort_partial_sort_comp(void);

// alg.sorting/alg.sort/partial.sort.copy
int tc_algorithms_alg_sorting_alg_sort_partial_sort_copy_partial_sort_copy(void);
int tc_algorithms_alg_sorting_alg_sort_partial_sort_copy_partial_sort_copy_comp(void);

// alg.sorting/alg.sort/sort
int tc_algorithms_alg_sorting_alg_sort_sort_sort(void);
int tc_algorithms_alg_sorting_alg_sort_sort_sort_comp(void);
int tc_algorithms_alg_sorting_alg_sort_sort_sort_constexpr(void);
int tc_algorithms_alg_sorting_alg_sort_sort_sort_constexpr_comp(void);

// alg.sorting/alg.sort/stable.sort
int tc_algorithms_alg_sorting_alg_sort_stable_sort_stable_sort(void);
int tc_algorithms_alg_sorting_alg_sort_stable_sort_stable_sort_comp(void);

// algorithms.general
int tc_algorithms_algorithms_general_nothing_to_do(void);

// misc
int tc_algorithms_robust_against_adl_on_new(void);
int tc_algorithms_robust_against_proxy_iterators_lifetime_bugs(void);
int tc_algorithms_robust_re_difference_type_compile(void);

#endif /* __EXTERNAL_LIBCXX_TEST_STD_ALGORITHMS_TC_ALGORITHMS_HPP */