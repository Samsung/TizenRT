#!/usr/bin/env python
###########################################################################
#
# Copyright 2024 Samsung Electronics All Rights Reserved.
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
# File : tcbInfo.py
# Description: Contains functions for parsing TCB (Task Control Block) information from log files.
#              This module extracts and displays detailed information about the asserted task including
#              its state, syscall information, and thread flags. It uses addr2line to resolve addresses
#              to symbol names for both kernel and application text sections.
#
#              The parse_tcb_info function is the main API that:
#              - Parses the asserted task's TCB information from the crash log
#              - Decodes task state values to human-readable format
#              - Resolves syscall addresses to function names
#              - Interprets TCB flags to show thread type, scheduling policy, and various states

from __future__ import print_function
import os
import logAnalyser.logUtils as logUtils

tcb_info = "Asserted task's TCB info"

partition_string = "==========================================================="

TCB_FLAG_TTYPE_SHIFT      = (0)	                         # Bits 0-1: thread type 
TCB_FLAG_TTYPE_MASK       = (3 << TCB_FLAG_TTYPE_SHIFT)
TCB_FLAG_TTYPE_TASK       = (0 << TCB_FLAG_TTYPE_SHIFT)	 # Normal user task 
TCB_FLAG_TTYPE_PTHREAD    = (1 << TCB_FLAG_TTYPE_SHIFT)	 # User pthread 
TCB_FLAG_TTYPE_KERNEL     = (2 << TCB_FLAG_TTYPE_SHIFT)	 # Kernel thread 
TCB_FLAG_NONCANCELABLE    = (1 << 2)                     # Bit 2: Pthread is non-cancelable 
TCB_FLAG_CANCEL_DEFERRED  = (1 << 3)                     # Bit 3: Deferred (vs asynch) cancellation type 
TCB_FLAG_CANCEL_PENDING   = (1 << 4)                     # Bit 4: Pthread cancel is pending 
TCB_FLAG_POLICY_SHIFT     = (5)                          # Bit 5-6: Scheduling policy 
TCB_FLAG_POLICY_MASK      = (3 << TCB_FLAG_POLICY_SHIFT)
TCB_FLAG_SCHED_FIFO       = (0 << TCB_FLAG_POLICY_SHIFT) # FIFO scheding policy 
TCB_FLAG_ROUND_ROBIN      = (1 << TCB_FLAG_POLICY_SHIFT) # Round robin scheding policy 
TCB_FLAG_SCHED_SPORADIC   = (2 << TCB_FLAG_POLICY_SHIFT) # Sporadic scheding policy 
TCB_FLAG_SCHED_OTHER      = (3 << TCB_FLAG_POLICY_SHIFT) # Other scheding policy 
TCB_FLAG_CPU_LOCKED       = (1 << 7)                     # Bit 7: Locked to this CPU 
TCB_FLAG_EXIT_PROCESSING  = (1 << 8)                     # Bit 8: Exitting 
TCB_FLAG_SYSCALL          = (1 << 10)                    # Bit 9: In a system call 


# API to parse asserted tcb information
def parse_tcb_info(parser):
    current_line = ""
    print("\nt. Asserted TCB info:\n")
    with open(parser.log_file) as searchfile:
        for line in searchfile:
            if partition_string in line:
                line = next(searchfile)
                current_line = line
                line = next(searchfile)
                continue
            if tcb_info in current_line:
                while '==================' not in line:
                    data = line.replace("|", " ").replace("/", " ").replace("\n", " ").split(":")[2]
                    data = str(list(filter(None, data.split(" ")))[0])
                    if "State" in line:
                        line = line.replace(" " + data, parser.task_state[data.split()[0]])
                    elif "Syscall" in line:
                        app_idx = logUtils.is_app_text_address(parser, data)
                        if app_idx:
                            if parser.xip_enabled:
                                addr = data
                            else:
                                addr = data - int(hex(parser.g_stext_app[app_idx - 1]), 16)
                            f = os.popen('arm-none-eabi-addr2line -f -e ' + parser.bin_path + parser.app_name[app_idx - 1] + '_dbg ' + hex(int(addr, 16)))
                            result = f.read()
                            if '??' not in result and '$d' not in result:
                                line = line.replace(data, result.replace("\n"," ").split()[0])
                        elif logUtils.is_kernel_text_address(parser, data):
                            f = os.popen('arm-none-eabi-addr2line -f -e ' + parser.elf + ' ' + hex(int(data, 16)))
                            result = f.read()
                            if '??' not in result and '$d' not in result:
                                line = line.replace(data, result.replace("\n"," ").split()[0])
                    elif "Flags" in line:
                        flag = int(data)
                        line = line.replace("\n","")
                        if (TCB_FLAG_TTYPE_TASK & flag):
                            line += "\t - User Task"
                        elif TCB_FLAG_TTYPE_PTHREAD & flag:
                            line += "\t - User Pthread"
                        elif TCB_FLAG_TTYPE_KERNEL & flag:
                            line += "\t - Kernel Thread"
                        if TCB_FLAG_NONCANCELABLE & flag:
                            line += "\t - Non-cancellable Pthread"
                        if TCB_FLAG_CANCEL_DEFERRED & flag:
                            line += "\t - Cancel Deffered"
                        if TCB_FLAG_CANCEL_PENDING & flag:
                            line += "\t - Cancel Pending"
                        if TCB_FLAG_ROUND_ROBIN & flag:
                            line += "\t - Round Robin"
                        elif TCB_FLAG_SCHED_FIFO & flag:
                            line += "\t - FIFO"
                        elif TCB_FLAG_SCHED_SPORADIC & flag:
                            line += "\t - Sporadic"
                        if TCB_FLAG_CPU_LOCKED & flag:
                            line += "\t - CPU Locked"
                        if TCB_FLAG_EXIT_PROCESSING & flag:
                            line += "\t - Exiting"
                        if TCB_FLAG_SYSCALL & flag:
                            line += "\t - In a System Call"
                        line += "\n"
                    print(' '.join(line.split(":", 1)[1:]), end = "")
                    line = next(searchfile)
                break
