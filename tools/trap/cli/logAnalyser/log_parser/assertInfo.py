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
# File : assertInfo.py
# Description: Contains functions for parsing assertion information from log files

from __future__ import print_function
import utils


stack_details = "Asserted task's stack details\n"

partition_string = "==========================================================="


# API to find point of assertion
def parse_assert_info(parser):
    is_interrupt_mode = 0
    current_line = ""

    with open(parser.log_file) as searchfile:
        for line in searchfile:
            if 'ERROR: Stack pointer is not within any of the allocated stack' in line:
                word = line.split(':')
                print('\n\t-', word[1], ':', word[2])

    print('\n4. Code asserted in:')

    # Parse the contents based on tokens in log file to determine point of assertion in details
    with open(parser.log_file) as searchfile:
        found_type = 0
        for line in searchfile:
            if 'Code asserted in nested IRQ state!' in line:
                found_type = 1
                print('\n\t- Code asserted in Nested IRQ state.')
                break
            elif 'Code asserted in IRQ state!' in line:
                found_type = 1
                print('\n\t- Code asserted in IRQ state.')
                break
            elif 'Running work function is' in line:
                found_type = 1
                print('\n\t- Code asserted in workqueue.')
                break
        if (found_type == 0):
                print('\n\t- Code asserted in normal thread.')

    # Parse the contents based on tokens in log file for assert during interrupt context
    with open(parser.log_file) as searchfile:
        for line in searchfile:
            # Print the interrupt data during crash (if crashed during interrupt context)
            if 'IRQ num:' in line:
                word = line.split(' ')
                # Last word[-1] contains the interrupt number
                irq_num = word[-1]
                print('\n\t- Interrupt number\t\t\t:',irq_num)
            if 'Code asserted in IRQ state!' in line:
                is_interrupt_mode = 1
                # It displays the interrupt handler information corresponding to the Interrupt
                print("\n5. Assertion Data during interrupt mode:\n")
                print('\t- Interrupt handler at addr\t\tSymbol_name')
                utils.print_interrupt_handler_data(parser.log_file, parser.bin_path, parser.app_name)

    with open(parser.log_file) as searchfile:
        for line in searchfile:
            if 'Nested IRQ stack:' in line:
                is_interrupt_mode = 2
                print("- IRQ Stack information:\n")
            if (is_interrupt_mode >= 2 and is_interrupt_mode < 9):
                is_interrupt_mode = is_interrupt_mode + 1
                print("\033[F", line)

    if (is_interrupt_mode):
        print('\n6. Call stack of last run thread')
    else:
        print('\n5. Call stack of last run thread')

    # Parse the contents based on tokens in log file for memory allocation failure data
    with open(parser.log_file) as searchfile:
        mm_fail = 0
        for line in searchfile:
            # Print the mm allocation failure data during crash (if crashed during mm allocation)
            if ('mm_manage_alloc_fail:' in line) and (mm_fail == 0):
                print('\nMemory allocation failure logs are as below:')
                mm_fail = 1
            if (mm_fail == 1):
                print(line)

    # Parse the contents based on tokens in log file.
    with open(parser.log_file) as searchfile:
        for line in searchfile:
            
            # Print the current stack pointer value
            if current_line == stack_details and ' sp:' in line:
                word = line.split(':')
                # word[2] contains the current stack pointer
                stack_curr = int(word[2], 16)
                print("\n\t- Current stack pointer:\t\t", hex(stack_curr))
            # Print the current running work function
            if 'Running work function is' in line:
                word = line.split(' ')
                # Last word[-1] contains the current running work function
                wf = word[-1]
                curr_worker = int(wf[:-2], 16)
                print("\t- Current running work function is:\t", hex(curr_worker))
                # It displays the symbol corresponding to the current running work function
                print('\nCurrent running work function\t\tFile_name')
                utils.print_running_work_function(parser.log_file, parser.bin_path, parser.app_name)

