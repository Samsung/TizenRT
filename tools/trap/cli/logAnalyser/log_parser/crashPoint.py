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
# File : crashPoint.py
# Description: Contains functions for finding crash point and crash type from log files

from __future__ import print_function
import os
import logAnalyser.logUtils as logUtils
import utils


assertion_details = "Assertion details\n"
register_dump = "Asserted task's register dump\n"

partition_string = "==========================================================="


# API to find crash binary, crash point and crash type from assert log
def find_crash_point(parser):
    pc_value = 0
    lr_value = 0
    is_app_crash = 0
    is_kernel_crash = 0
    assertline = ""
    current_line = ""

    parser.g_stext_flash = utils.get_address_of_symbol("_stext_flash")
    parser.g_etext_flash = utils.get_address_of_symbol("_etext_flash")
    if (parser.have_ram_kernel_text):
        parser.g_stext_ram = utils.get_address_of_symbol("_stext_ram")
        parser.g_etext_ram = utils.get_address_of_symbol("_etext_ram")

    # Parse the contents based on tokens in log file.
    with open(parser.log_file) as searchfile:
        for line in searchfile:
            if partition_string in line:
                line = next(searchfile)
                current_line = line
                line = next(searchfile)
                continue
            if current_line == register_dump:
                word = line.split(':')

                # word[1] contains the register name i.e R0 or R8
                reg = word[1].split()

                if reg[0] == 'R8':
                    t = word[2].split( )

                    # Check for corruption of PC value in logs
                    if (len(word[2].split( )) != 8):
                        print('\nAssert logs are corrupted, and we are not able to determine the value of PC.\n')
                        continue

                    # Last subword of word[2] contains the PC value
                    pc_value = int(t[-1],16)
                    lr_value = int(t[-2],16)
                    continue

            # Get the assert location PC value
            if current_line == assertion_details and "Assertion failed" in line:
                assertline = line
            if 'Assert location (PC) :' in line:
                word = line.split(':')
                #word[2] contains the g_assertpc value
                parser.g_assertpc = int(word[2].strip(),16)
                
    print('-----------------------------------------------------------------------------------------')
    address1 = hex(lr_value)
    address2 = hex(pc_value)
    result = 0
    # Check for lr & pc values in application text address range
    if (pc_value != 00000000):
        for app_idx in range(parser.g_app_idx):
            if (address1 >= hex(parser.g_stext_app[app_idx]) and address1 < hex(parser.g_etext_app[app_idx])):
                if parser.xip_enabled:
                    addr = lr_value
                else:
                    addr = lr_value - int(hex(parser.g_stext_app[app_idx]), 16)
                f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr))
                result = f.read()
                if '??' not in result and '$d' not in result:
                    is_app_crash = 1
                    print('\n1. Crash Binary             : {0}'.format(parser.app_name[app_idx]))
                    logUtils.print_crash_type(parser, assertline)
                    if (parser.crash_type_assert == False):
                        print('3. Crash point (PC or LR)')
                        print('\n\t[ Caller - return address (LR) - of the function which has caused the crash ]')
                        logUtils.format_output(result, "")
            if (address2 >= hex(parser.g_stext_app[app_idx]) and address2 < hex(parser.g_etext_app[app_idx])):
                if parser.xip_enabled:
                    addr = pc_value
                else:
                    addr = pc_value - int(hex(parser.g_stext_app[app_idx]), 16)
                f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr))
                result = f.read()
                if '??' not in result and '$d' not in result:
                    if (not is_app_crash):
                        print('\n1. Crash Binary             : {0}'.format(parser.app_name[app_idx]))
                        logUtils.print_crash_type(parser, assertline)
                        if (parser.crash_type_assert == False):
                            print('3. Crash point (PC or LR)')
                    is_app_crash = 1
                    if (parser.crash_type_assert == False):
                        print('\n\t[ Current location (PC) of assert ]')
                        logUtils.format_output(result, "")
                if ((addr - 4) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr - 4))
                    result1 = f.read()
                    if '??' not in result1 and '$d' not in result1:
                        if (parser.crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            logUtils.format_output(result1, "of (pc - 4)")
                if ((addr - 8) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr - 8))
                    result2 = f.read()
                    if '??' not in result2 and '$d' not in result2:
                        if (parser.crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            logUtils.format_output(result2, "of (pc - 8)")

    # Scenario when up_registerdump() is not present in dump logs, use g_assertpc value to give crash point
    else:
        address1 = hex(parser.g_assertpc)
        for app_idx in range(parser.g_app_idx):
            if (address1 >= hex(parser.g_stext_app[app_idx]) and address1 < hex(parser.g_etext_app[app_idx])):
                if parser.xip_enabled:
                    addr = parser.g_assertpc
                else:
                    addr = parser.g_assertpc - int(hex(parser.g_stext_app[app_idx]), 16)
                f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr))
                result = f.read()
                if '??' not in result and '$d' not in result:
                    is_app_crash = 1
                    print('\n1. Crash Binary             : {0}'.format(parser.app_name[app_idx]))
                    logUtils.print_crash_type(parser, assertline)
                    if (parser.crash_type_assert == False):
                        print('3. Crash point (PC or LR)')
                        print('\n\t[ Current location (PC) of assert ]')
                        logUtils.format_output(result, "")
                if ((addr - 4) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr - 4))
                    result1 = f.read()
                    if '??' not in result1 and '$d' not in result1:
                        if (parser.crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            logUtils.format_output(result1, "of (pc - 4)")
                if ((addr - 8) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + parser.bin_path + parser.app_name[app_idx] + '_dbg ' + hex(addr - 8))
                    result2 = f.read()
                    if '??' not in result2 and '$d' not in result2:
                        if (parser.crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            logUtils.format_output(result2, "of (pc - 8)")

    # Check for lr & pc values in kernel text address range
    if (not is_app_crash) and (pc_value != 00000000):
        address1 = hex(lr_value)
        if (address1 >= hex(parser.g_stext_flash) and address1 < hex(parser.g_etext_flash)) or (address1 >= hex(parser.g_stext_ram) and address1 < hex(parser.g_etext_ram)):
            # If yes, print the crash point using addr2line
            f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(lr_value))
            result = f.read()
            if '??' not in result and '$d' not in result:
                is_kernel_crash = 1
                print('1. Crash Binary             : kernel')
                logUtils.print_crash_type(parser, assertline)
                if (parser.crash_type_assert == False):
                    print('3. Crash point (PC or LR)')
                    print('\n\t[ Caller - return address (LR) - of the function which has caused the crash ]')
                    logUtils.format_output(result, "")
        if (address2 >= hex(parser.g_stext_flash) and address2 < hex(parser.g_etext_flash)) or (address2 >= hex(parser.g_stext_ram) and address2 < hex(parser.g_etext_ram)):
            f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(pc_value))
            result = f.read()
            if '??' not in result and '$d' not in result:
                if (not is_kernel_crash):
                    print('1. Crash Binary             : kernel')
                    logUtils.print_crash_type(parser, assertline)
                    if (parser.crash_type_assert == False):
                        print('3. Crash point (PC or LR)')
                is_kernel_crash = 1
                if (parser.crash_type_assert == False):
                    print('\n\t[ Current location (PC) of assert ]')
                    logUtils.format_output(result, "")
            if ((pc_value - 4) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(pc_value - 4))
                result1 = f.read()
                if '??' not in result1 and '$d' not in result1:
                    if (parser.crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        logUtils.format_output(result1, "of (pc - 4)")
            if ((pc_value - 8) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(pc_value - 8))
                result2 = f.read()
                if '??' not in result2 and '$d' not in result2:
                    if (parser.crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        logUtils.format_output(result2, "of (pc - 8)")

    # Scenario when up_registerdump() is not present in dump logs, use g_assertpc value to give crash point
    if (pc_value == 00000000 and parser.g_assertpc):
        address1 = hex(parser.g_assertpc)
        if (address1 >= hex(parser.g_stext_flash) and address1 < hex(parser.g_etext_flash)) or (address1 >= hex(parser.g_stext_ram) and address1 < hex(parser.g_etext_ram)):
            # If yes, print the crash point using addr2line
            f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(parser.g_assertpc))
            result = f.read()
            if '??' not in result and '$d' not in result:
                is_kernel_crash = 1
                print('1. Crash Binary             : kernel')
                logUtils.print_crash_type(parser, assertline)
                if (parser.crash_type_assert == False):
                    print('3. Crash point (PC or LR)')
                    print('\n\t[ Current location (PC) of assert ]')
                    logUtils.format_output(result, "")
            if ((parser.g_assertpc - 4) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(parser.g_assertpc - 4))
                result1 = f.read()
                if '??' not in result1 and '$d' not in result1:
                    if (parser.crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        logUtils.format_output(result1, "of (pc - 4)")
            if ((parser.g_assertpc - 8) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + parser.elf + ' ' + hex(parser.g_assertpc - 8))
                result2 = f.read()
                if '??' not in result2 and '$d' not in result2:
                    if (parser.crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        logUtils.format_output(result2, "of (pc - 8)")

    if (not is_app_crash) and (not is_kernel_crash):
        print('1. Crash Binary             : NA')
        logUtils.print_crash_type(parser, assertline)
        if (parser.crash_type_assert == False):
            print('3. Crash point (PC or LR)')
            # Parse the contents based on tokens in log file.
            with open(parser.log_file) as searchfile:
                for line in searchfile:
                    # If PC value is invalid, show invalid PC
                    if 'PC value might be invalid' in line:
                        print('\tPC value might be invalid.')
            print('\t-  PC & LR values not in any text range! No probable crash point detected.')
