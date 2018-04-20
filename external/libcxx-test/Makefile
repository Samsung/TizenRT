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
