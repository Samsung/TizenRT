###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

ASRCS		=
CSRCS		=
CXXSRCS 	=

#UTC Main Source
CSRCS += utc_libcxx.c

#Add TC Common Header
CFLAGS += -I ../../apps/examples/testcase/
CXXFLAGS += -I ../../apps/examples/testcase/

#Add Support Headers
CXXFLAGS += -I. -I support/

#Add test llocator initialization code
CXXSRCS += support/test_allocator.cpp

ifeq ($(CONFIG_LIBCXX_UTC_ALGORITHMS),y)
#Algorithms Source
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_init_list_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_init_list_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_element_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_element_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_init_list_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_init_list.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_element.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_element.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_element.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max_element_comp.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/minmax_init_list.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/max.pass.cpp
CXXSRCS += std/algorithms/alg.sorting/alg.min.max/min_init_list.pass.cpp

#Algorithms TC Main
CXXSRCS += std/algorithms/tc_algorithms.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_UTILITIES),y)
#Utilities Source
CXXFLAGS += -I std/utilities/memory/
CXXSRCS += std/utilities/memory/smartptr_shared_common.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.const/shared_ptr_pointer.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.const/shared_ptr_Y_rv.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.const/shared_ptr_Y.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.const/shared_ptr.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.const/shared_ptr_rv.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.assign/auto_ptr_Y.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.assign/shared_ptr_Y_rv.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.assign/unique_ptr_Y.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.assign/shared_ptr_Y.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.assign/shared_ptr.pass.cpp
CXXSRCS += std/utilities/memory/util.smartptr/util.smartptr.shared/util.smartptr.shared.assign/shared_ptr_rv.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pair.astuple/tuple_element.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pair.astuple/get_rv.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pair.astuple/tuple_size.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pair.astuple/get_const.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pair.astuple/get_non_const.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pair.piecewise/piecewise_construct.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.spec/non_member_swap.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.spec/comparison.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.spec/make_pair.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/copy_ctor.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/piecewise.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/assign_rv_pair_U_V.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/assign_rv_pair.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/U_V.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/const_pair_U_V.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/const_first_const_second.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/assign_const_pair_U_V.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/swap.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/types.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/default.pass.cpp
CXXSRCS += std/utilities/utility/pairs/pairs.pair/rv_pair_U_V.pass.cpp

#Utilities TC Main
CXXSRCS += std/utilities/tc_utilities.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_DIAGNOSTICS),y)
#Diagnostics Sources
CXXSRCS += std/diagnostics/std.exceptions/out.of.range/out_of_range.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/overflow.error/overflow_error.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/runtime.error/runtime_error.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/invalid.argument/invalid_argument.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/domain.error/domain_error.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/length.error/length_error.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/range.error/range_error.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/underflow.error/underflow_error.pass.cpp
CXXSRCS += std/diagnostics/std.exceptions/logic.error/logic_error.pass.cpp

#Diagnostics TC Main
CXXSRCS += std/diagnostics/tc_diagnostics.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_STDBOOL),y)
#stdbool sources
CXXSRCS += std/depr/depr.c.headers/stdbool_h.pass.cpp
CXXSRCS += std/language.support/support.runtime/cstdbool.pass.cpp

#stdbool TC
CXXSRCS += std/depr/tc_depr.cpp
CXXSRCS += std/language.support/tc_language_support.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_THREAD),y)
#Thread Sources
CXXSRCS += std/thread/thread.mutex/thread.lock.algorithm/lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock.algorithm/try_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.mutex.requirements.mutex/thread.mutex.recursive/lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.mutex.requirements.mutex/thread.mutex.recursive/try_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.mutex.requirements.mutex/thread.mutex.recursive/default.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.mutex.requirements.mutex/thread.mutex.class/lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.mutex.requirements.mutex/thread.mutex.class/try_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.mutex.requirements.mutex/thread.mutex.class/default.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.recursive/lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.recursive/try_lock_for.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.recursive/try_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.recursive/default.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.recursive/try_lock_until.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.class/lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.class/try_lock_for.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.class/try_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.class/default.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.mutex.requirements/thread.timedmutex.requirements/thread.timedmutex.class/try_lock_until.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.once/thread.once.onceflag/default.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.once/thread.once.callonce/call_once.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.guard/adopt_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.guard/mutex.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.guard/types.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/mutex_try_to_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/move_assign.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/mutex_defer_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/mutex_time_point.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/mutex.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/move_ctor.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/mutex_duration.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/default.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.cons/mutex_adopt_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.locking/lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.locking/try_lock_for.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.locking/unlock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.locking/try_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.locking/try_lock_until.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.obs/owns_lock.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.obs/op_bool.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.obs/mutex.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.mod/member_swap.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.mod/release.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/thread.lock.unique.mod/nonmember_swap.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/thread.lock.unique/types.pass.cpp
CXXSRCS += std/thread/thread.mutex/thread.lock/types.pass.cpp

#Thread TC Main
CXXSRCS += std/thread/tc_thread.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_ITERATORS),y)
#Iterators Source
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.opref/op_arrow.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op++/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op++/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.conv/tested_elsewhere.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.opgt/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.oplt=/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op-/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.opindex/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.opdiff/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op+=/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op!=/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.opgt=/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op=/reverse_iterator.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op+/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op==/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op-=/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op--/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op--/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.cons/iter.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.cons/reverse_iterator.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.cons/default.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.oplt/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.op.star/op_star.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.ops/reverse.iter.opsum/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/reverse.iterators/reverse.iter.requirements/nothing_to_do.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.inserter/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.insert.iter.op++/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.insert.iter.op++/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.insert.iter.op_astrk/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.insert.iter.op=/lv_value.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.insert.iter.op=/rv_value.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/nothing_to_do.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/front.insert.iter.ops/front.insert.iter.cons/container.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.insert.iter.op++/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.insert.iter.op++/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.insert.iter.cons/container.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.inserter/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/nothing_to_do.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.insert.iter.op=/lv_value.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.insert.iter.op=/rv_value.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/back.insert.iter.ops/back.insert.iter.op_astrk/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/insert.iter.cons/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/inserter/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/insert.iter.op=/lv_value.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/insert.iter.op=/rv_value.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/insert.iter.op_astrk/test.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/insert.iter.op++/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/insert.iterators/insert.iter.ops/insert.iter.op++/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.comp/op_lt.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.comp/op_neq.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.comp/op_gt.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.comp/op_lte.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.comp/op_gte.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.comp/op_eq.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op=/move_iterator.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.nonmember/make_move_iterator.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.nonmember/plus.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.nonmember/minus.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.decr/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.decr/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.index/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.-/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.const/iter.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.const/convert.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.const/default.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.incr/post.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.incr/pre.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.ref/op_arrow.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.conv/tested_elsewhere.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.+/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.star/op_star.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.+=/difference_type.pass.cpp
CXXSRCS += std/iterators/predef.iterators/move.iterators/move.iter.ops/move.iter.op.-=/difference_type.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.operations/prev.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.operations/advance.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.operations/distance.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.operations/next.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.traits/empty.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.traits/const_pointer.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.traits/iterator.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.traits/pointer.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/iterator.basic/iterator.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/std.iterator.tags/bidirectional_iterator_tag.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/std.iterator.tags/random_access_iterator_tag.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/std.iterator.tags/input_iterator_tag.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/std.iterator.tags/forward_iterator_tag.pass.cpp
CXXSRCS += std/iterators/iterator.primitives/std.iterator.tags/output_iterator_tag.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostreambuf.iterator/ostreambuf.iter.cons/streambuf.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostreambuf.iterator/ostreambuf.iter.cons/ostream.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostreambuf.iterator/ostreambuf.iter.ops/increment.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostreambuf.iterator/ostreambuf.iter.ops/failed.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostreambuf.iterator/ostreambuf.iter.ops/assign_c.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostreambuf.iterator/ostreambuf.iter.ops/deref.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_op++/dereference.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_op==/equal.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_op_astrk/post_increment.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_op_astrk/pre_increment.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator.cons/streambuf.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator.cons/proxy.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator.cons/istream.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator.cons/default.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_op!=/not_equal.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_equal/equal.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istreambuf.iterator/istreambuf.iterator_proxy/proxy.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.ops/post_increment.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.ops/arrow.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.ops/pre_increment.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.ops/equal.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.ops/dereference.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.cons/copy.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.cons/istream.pass.cpp
CXXSRCS += std/iterators/stream.iterators/istream.iterator/istream.iterator.cons/default.pass.cpp
CXXSRCS += std/iterators/stream.iterators/iterator.range/end_array.pass.cpp
CXXSRCS += std/iterators/stream.iterators/iterator.range/begin_const.pass.cpp
CXXSRCS += std/iterators/stream.iterators/iterator.range/begin_non_const.pass.cpp
CXXSRCS += std/iterators/stream.iterators/iterator.range/end_non_const.pass.cpp
CXXSRCS += std/iterators/stream.iterators/iterator.range/end_const.pass.cpp
CXXSRCS += std/iterators/stream.iterators/iterator.range/begin_array.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostream.iterator/ostream.iterator.cons.des/copy.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostream.iterator/ostream.iterator.cons.des/ostream.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostream.iterator/ostream.iterator.ops/increment.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostream.iterator/ostream.iterator.ops/assign_t.pass.cpp
CXXSRCS += std/iterators/stream.iterators/ostream.iterator/ostream.iterator.ops/dereference.pass.cpp

#Iterators TC Main
CXXSRCS += std/iterators/tc_iterators.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_STRINGS),y)
#String Sources
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/not_eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eq.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/to_char_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/lt.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/types.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.wchar.t/assign2.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/not_eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/copy.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eq.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/find.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/move.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/length.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/to_char_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/lt.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/compare.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign3.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/types.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char/assign2.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/not_eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/copy.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eq.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/find.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/move.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/length.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/to_char_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/lt.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/compare.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign3.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/types.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char16_t/assign2.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/not_eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/copy.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eq.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/find.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/eof.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/move.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_int_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/length.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/to_char_type.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/lt.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/compare.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign3.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/types.pass.cpp
CXXSRCS += std/strings/char.traits/char.traits.specializations/char.traits.specializations.char32_t/assign2.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op!=/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op!=/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op!=/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op+/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op+/char_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op+/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op+/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_op+/string_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_oplt=/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_oplt=/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_oplt=/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_oplt/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_oplt/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_oplt/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_opgt=/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_opgt=/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_opgt=/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_opgt/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_opgt/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_opgt/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_operator==/string_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_operator==/string_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string_operator==/pointer_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string.io/get_line_rv.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string.io/get_line_delim_rv.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string.special/swap_noexcept.pass.cpp
CXXSRCS += std/strings/basic.string/string.nonmembers/string.special/swap.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_substr/substr.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.of/string_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.of/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.of/char_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.not.of/string_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.not.of/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.not.of/char_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_compare/string.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_compare/pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find/string_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find/char_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_rfind/string_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_rfind/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_rfind/char_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.not.of/string_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.not.of/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.not.of/char_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.of/string_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.of/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.of/char_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string.accessors/get_allocator.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string.accessors/c_str.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string.accessors/data.pass.cpp
CXXSRCS += std/strings/basic.string/string.access/back.pass.cpp
CXXSRCS += std/strings/basic.string/string.access/index.pass.cpp
CXXSRCS += std/strings/basic.string/string.access/at.pass.cpp
CXXSRCS += std/strings/basic.string/string.access/front.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/rbegin.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/crend.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/rend.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/cbegin.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/begin.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/end.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/crbegin.pass.cpp
CXXSRCS += std/strings/basic.string/string.iterators/cend.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/empty.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/clear.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/resize_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/resize_size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/capacity.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/length.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/size.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/reserve.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/shrink_to_fit.pass.cpp
CXXSRCS += std/strings/basic.string/string.capacity/max_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/iter_size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/iter_initializer_list.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/size_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/iter_iter_iter.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/size_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/iter_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_swap/swap.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/string.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/rv_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/initializer_list.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/iterator.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/string.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/initializer_list.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/iterator.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/push_back.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_erase/iter.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_erase/pop_back.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_erase/iter_iter.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/iter_iter_string.pass.cpp
ifeq ($(CONFIG_LIBCXX_UTC_STRINGS_LARGESIZE),y)
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/size_pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.of/pointer_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.not.of/pointer_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find/pointer_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_rfind/pointer_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.last.not.of/pointer_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_find.first.of/pointer_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_assign/string_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/size_string_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_append/string_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_erase/size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_compare/size_size_string_size_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_compare/size_size_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_compare/size_size_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.ops/string_compare/size_size_pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_insert/size_size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/size_size_string_size_size.pass.cpp #VM exhaust : compilation failed : large size
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/size_size_string.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/size_size_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/size_size_pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/size_size_size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/iter_iter_iter_iter.pass.cpp
endif
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/iter_iter_size_char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/iter_iter_initializer_list.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/iter_iter_pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_replace/iter_iter_pointer_size.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_op_plus_equal/string.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_op_plus_equal/initializer_list.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_op_plus_equal/pointer.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_op_plus_equal/char.pass.cpp
CXXSRCS += std/strings/basic.string/string.modifiers/string_copy/copy.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/pointer_size_alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/move_assignment.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/size_char_alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/move_assign_noexcept.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/copy.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/iter_alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/default_noexcept.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/dtor_noexcept.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/substr.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/move.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/copy_alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/copy_assignment.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/move_noexcept.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/pointer_alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/move_alloc.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/pointer_assignment.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/char_assignment.pass.cpp
CXXSRCS += std/strings/basic.string/string.cons/initializer_list_assignment.pass.cpp
CXXSRCS += std/strings/basic.string/types.pass.cpp
CXXSRCS += std/strings/c.strings/cctype.pass.cpp
CXXSRCS += std/strings/basic.string.hash/strings.pass.cpp

#Strings TC Main
CXXSRCS += std/strings/tc_strings.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_CONTAINERS),y)

ifeq ($(CONFIG_LIBCXX_UTC_CONTAINERS_MAP),y)

CXXSRCS += std/containers/associative/map/map.access/at.pass.cpp
CXXSRCS += std/containers/associative/map/map.access/empty.pass.cpp
CXXSRCS += std/containers/associative/map/map.access/index_key.pass.cpp
CXXSRCS += std/containers/associative/map/map.access/index_rv_key.pass.cpp
CXXSRCS += std/containers/associative/map/map.access/iterator.pass.cpp
CXXSRCS += std/containers/associative/map/map.access/max_size.pass.cpp
CXXSRCS += std/containers/associative/map/map.access/size.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/alloc.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/compare.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/compare_alloc.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/copy.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/copy_alloc.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/default.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/default_noexcept.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/default_recursive.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/initializer_list.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/initializer_list_compare.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/initializer_list_compare_alloc.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/iter_iter.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/iter_iter_comp.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/iter_iter_comp_alloc.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/move.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/move_alloc.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/move_assign.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/associative/map/map.cons/move_noexcept.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/clear.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/emplace.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/emplace_hint.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/erase_iter.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/erase_iter_iter.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/erase_key.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/insert_cv.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/insert_iter_cv.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/insert_iter_iter.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/insert_iter_rv.pass.cpp
CXXSRCS += std/containers/associative/map/map.modifiers/insert_rv.pass.cpp
CXXSRCS += std/containers/associative/map/map.ops/count.pass.cpp
CXXSRCS += std/containers/associative/map/map.special/swap_noexcept.pass.cpp
CXXSRCS += std/containers/associative/map/types.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/alloc.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/assign_initializer_list.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/compare.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/compare_alloc.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/copy.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/copy_alloc.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/copy_assign.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/default.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/default_noexcept.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/initializer_list.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/initializer_list_compare.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/initializer_list_compare_alloc.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/iter_iter.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/iter_iter_comp.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/iter_iter_comp_alloc.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/move_alloc.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/move_assign.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.cons/move_noexcept.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/clear.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/emplace.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/emplace_hint.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/erase_iter.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/erase_iter_iter.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/erase_key.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/insert_cv.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/insert_initializer_list.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/insert_iter_iter.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.modifiers/insert_iter_rv.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.ops/count.pass.cpp
CXXSRCS += std/containers/associative/multimap/multimap.special/swap_noexcept.pass.cpp
CXXSRCS += std/containers/associative/multimap/size.pass.cpp
CXXSRCS += std/containers/associative/multimap/types.pass.cpp
CXXSRCS += std/containers/unord/unord.map/types.pass.cpp
CXXSRCS += std/containers/unord/unord.map/find_const.pass.cpp
CXXSRCS += std/containers/unord/unord.map/iterators.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.swap/swap_non_member.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.swap/swap_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/copy_alloc.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/range_size_hash.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/assign_init.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/range_size.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/range.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/init_size_hash_equal.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/size_hash.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/move.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/assign_copy.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/move_alloc.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/init_size_hash.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/move_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/copy.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/init.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/size.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/range_size_hash_equal.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/default_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/init_size_hash_equal_allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/size_hash_equal.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/range_size_hash_equal_allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/assign_move.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/init_size.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/size_hash_equal_allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.cnstr/default.pass.cpp
CXXSRCS += std/containers/unord/unord.map/eq.pass.cpp
CXXSRCS += std/containers/unord/unord.map/bucket.pass.cpp
CXXSRCS += std/containers/unord/unord.map/equal_range_const.pass.cpp
CXXSRCS += std/containers/unord/unord.map/find_non_const.pass.cpp
CXXSRCS += std/containers/unord/unord.map/max_load_factor.pass.cpp
CXXSRCS += std/containers/unord/unord.map/swap_member.pass.cpp
CXXSRCS += std/containers/unord/unord.map/count.pass.cpp
CXXSRCS += std/containers/unord/unord.map/reserve.pass.cpp
CXXSRCS += std/containers/unord/unord.map/bucket_size.pass.cpp
CXXSRCS += std/containers/unord/unord.map/max_bucket_count.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.elem/at.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.elem/index.pass.cpp
CXXSRCS += std/containers/unord/unord.map/load_factor.pass.cpp
CXXSRCS += std/containers/unord/unord.map/bucket_count.pass.cpp
CXXSRCS += std/containers/unord/unord.map/rehash.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/insert_const_lvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/insert_hint_rvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/insert_init.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/emplace_hint.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/insert_rvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/erase_key.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/erase_range.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/emplace.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/erase_const_iter.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/clear.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/insert_range.pass.cpp
CXXSRCS += std/containers/unord/unord.map/unord.map.modifiers/insert_hint_const_lvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.map/max_size.pass.cpp
CXXSRCS += std/containers/unord/unord.map/local_iterators.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/types.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/find_const.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.swap/swap_non_member.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.swap/swap_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/iterators.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/eq.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/copy_alloc.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/range_size_hash.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/assign_init.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/range_size.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/range.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/init_size_hash_equal.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/assign_copy.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/size_hash.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/move.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/move_alloc.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/init_size_hash.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/move_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/copy.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/init.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/size.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/range_size_hash_equal.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/default_noexcept.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/init_size_hash_equal_allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/size_hash_equal.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/range_size_hash_equal_allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/assign_move.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/init_size.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/size_hash_equal_allocator.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.cnstr/default.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/bucket.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/equal_range_const.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/find_non_const.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/max_load_factor.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/swap_member.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/count.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/reserve.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/bucket_size.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/max_bucket_count.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/load_factor.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/equal_range_non_const.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/bucket_count.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/rehash.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/max_size.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/insert_const_lvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/insert_hint_rvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/insert_init.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/emplace_hint.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/insert_rvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/erase_key.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/erase_range.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/emplace.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/erase_const_iter.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/clear.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/insert_range.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/unord.multimap.modifiers/insert_hint_const_lvalue.pass.cpp
CXXSRCS += std/containers/unord/unord.multimap/local_iterators.pass.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_CONTAINERS_VECTOR),y)
#Containers(Vectors)
CXXSRCS += std/containers/sequences/vector/types.pass.cpp
CXXSRCS += std/containers/sequences/vector/iterators.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.capacity/resize_size.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.capacity/shrink_to_fit.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.capacity/swap.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.capacity/capacity.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.capacity/reserve.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.capacity/resize_size_value.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.data/data_const.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.data/data.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/construct_default.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/copy_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/initializer_list_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/move.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/assign_copy.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/move_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/assign_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/move_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/construct_size_value.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/copy.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/op_equal_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/default_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/assign_move.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/construct_size_value_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.cons/construct_size.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/emplace_back.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/push_back_rvalue.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/push_back.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/erase_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/insert_iter_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/insert_iter_rvalue.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/emplace.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/insert_iter_value.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/insert_iter_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/erase_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.modifiers/emplace_extra.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.special/swap.pass.cpp
CXXSRCS += std/containers/sequences/vector/vector.special/swap_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/resize_size.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/construct_default.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/shrink_to_fit.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/copy_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/types.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/swap.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/iterators.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/push_back.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/erase_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/initializer_list_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/insert_iter_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/construct_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/insert_iter_value.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/insert_iter_size_value.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/move.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/assign_copy.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/capacity.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/move_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/assign_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/move_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/insert_iter_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/reserve.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/construct_size_value.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/copy.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/construct_iter_iter_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/op_equal_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/default_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/assign_move.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/erase_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/swap_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/construct_size_value_alloc.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/resize_size_value.pass.cpp
CXXSRCS += std/containers/sequences/vector.bool/construct_size.pass.cpp
endif

ifeq ($(CONFIG_LIBCXX_UTC_CONTAINERS_LIST),y)
#Containers(List)
CXXSRCS += std/containers/sequences/list/types.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/pop_back.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/emplace_back.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/push_back_rvalue.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/push_back.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/erase_iter.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/insert_iter_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/insert_iter_rvalue.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/emplace.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/insert_iter_value.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/insert_iter_size_value.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/emplace_front.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/insert_iter_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/clear.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/pop_front.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/erase_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/push_front.pass.cpp
CXXSRCS += std/containers/sequences/list/list.modifiers/push_front_rvalue.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/copy_alloc.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/initializer_list_alloc.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/size_value_alloc.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/move.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/default_stack_alloc.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/assign_copy.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/move_alloc.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/assign_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/move_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/copy.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/op_equal_initializer_list.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/default_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/assign_move.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/size_type.pass.cpp
CXXSRCS += std/containers/sequences/list/list.cons/default.pass.cpp
CXXSRCS += std/containers/sequences/list/list.special/swap.pass.cpp
CXXSRCS += std/containers/sequences/list/list.special/swap_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/list/iterators.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/remove_if.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/splice_pos_list_iter_iter.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/unique.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/sort.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/unique_pred.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/merge.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/splice_pos_list.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/splice_pos_list_iter.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/sort_comp.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/remove.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/reverse.pass.cpp
CXXSRCS += std/containers/sequences/list/list.ops/merge_comp.pass.cpp
CXXSRCS += std/containers/sequences/list/list.capacity/resize_size.pass.cpp
CXXSRCS += std/containers/sequences/list/list.capacity/resize_size_value.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/types.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/copy_alloc.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/assign_range.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/move_assign_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/size_value_alloc.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/range_alloc.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/assign_init.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/range.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/move.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/assign_copy.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/dtor_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/move_alloc.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/move_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/copy.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/init.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/size.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/default_noexcept.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/assign_move.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/alloc.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/assign_op_init.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/default.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/assign_size_value.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/size_value.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.cons/init_alloc.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/erase_after_one.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/resize_size.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/insert_after_rv.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/insert_after_size_value.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/emplace_front.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/insert_after_const.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/insert_after_range.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/insert_after_init.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/clear.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/erase_after_many.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/pop_front.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/push_front_const.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/push_front_rv.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/resize_size_value.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.modifiers/emplace_after.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.access/front.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/remove_if.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/unique.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/sort.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/unique_pred.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/merge.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/splice_after_one.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/sort_pred.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/remove.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/reverse.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/merge_pred.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/splice_after_range.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.ops/splice_after_flist.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.iter/before_begin.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.iter/iterators.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.spec/member_swap.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.spec/non_member_swap.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.spec/relational.pass.cpp
CXXSRCS += std/containers/sequences/forwardlist/forwardlist.spec/swap_noexcept.pass.cpp
endif

#Containers TC Main
CXXSRCS += std/containers/tc_containers.cpp
endif

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:.cpp=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\libexternal$(LIBEXT)
else
  BIN		= ../libexternal$(LIBEXT)
endif
endif

DEPPATH	= --dep-path .

# Common build

VPATH		=

all: .built
.PHONY: depend clean distclean chkcxx

chkcxx:
ifneq ($(CONFIG_HAVE_CXX),y)
	@echo ""
	@echo "In order to use this example, you toolchain must support must"
	@echo ""
	@echo "  (1) Explicitly select CONFIG_HAVE_CXX to build in C++ support"
	@echo "  (2) Define CXX, CXXFLAGS, and COMPILEXX in the Make.defs file"
	@echo "      of the configuration that you are using."
	@echo ""
	@exit 1
endif

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(MAINOBJ): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

.built: chkcxx $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	find . -name "*.o" -exec rm -rf {} \; # Tobe fixed

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
