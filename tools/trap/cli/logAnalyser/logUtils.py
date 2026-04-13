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
# File : utils.py
# Description: Contains utility functions for log parsing including
# formatting, crash type detection, address validation, and state mapping

from __future__ import print_function



def format_output(res, string):
    r = res.split('\n')
    print('\t- symbol addr {0}       : {1}'.format(string, r[0]))
    print('\t- function name {0}     : {1}'.format(string, r[1]))
    print('\t- file {0}              : {1}'.format(string, r[2]))

def print_crash_type(parser, string):
    if 'up_memfault' in string:
        print('\n2. Crash type               : memory fault')
    elif 'up_busfault' in string:
        print('\n2. Crash type               : bus fault')
    elif 'up_usagefault' in string:
        print('\n2. Crash type               : usage fault')
    elif 'up_hardfault' in string:
        print('\n2. Crash type               : hard fault')
    elif 'dataabort' in string:
        print('\n2. Crash type               : data abort')
    elif 'prefetchabort' in string:
        print('\n2. Crash type               : prefetch abort')
    elif 'undefinedinsn' in string:
        print('\n2. Crash type               : undefined instruction abort')
    elif 'Assertion failed' in string:
        parser.crash_type_assert = True
        print('\n2. Crash type               : code assertion by code ASSERT or PANIC')
    else:
        print('\n2. Crash type               : etc')
    if (parser.crash_type_assert == True):
        print('\n3. Crash point\n\t-', string.split(': ',1)[1])
    else:
        print('   Crash log\n\t-', string)

def is_app_text_address(parser, address):
	idx = 0
	# Check the application text address range
	for idx in range(parser.g_app_idx):
		if (address >= hex(parser.g_stext_app[idx]) and address < hex(parser.g_etext_app[idx])):
			return (idx + 1)
	if (idx == parser.g_app_idx):
		return False


# Function to check if address lies in the kernel text address range
def is_kernel_text_address(parser, address):

    # Check the kernel text address range
    if (address >= hex(parser.g_stext_ram) and address < hex(parser.g_etext_ram)) or (address >= hex(parser.g_stext_flash) and address < hex(parser.g_etext_flash)):
        return True
    else:
        return False


def convert_stateno_statemsg(parser):
	# Parse configuration to determine enabled features
	config_smp = False
	config_disable_signals = False
	config_disable_mqueue = False
	config_paging = False
	
	# Read configuration file
	with open(parser.config_path) as configfile:
		for line in configfile:
			if "CONFIG_SMP=y" in line:
				config_smp = True
			elif "CONFIG_DISABLE_SIGNALS=y" in line:
				config_disable_signals = True
			elif "CONFIG_DISABLE_MQUEUE=y" in line:
				config_disable_mqueue = True
			elif "CONFIG_PAGING=y" in line:
				config_paging = True
	
	# Build task state mapping
	state_no = 0
	parser.task_state[str(state_no)] = " Invalid"
	state_no+=1
	parser.task_state[str(state_no)] = " Pending preemption unlock"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait to scheduling (Ready)"
	state_no+=1
	parser.task_state[str(state_no)] = " Assigned to CPU (Ready)"
	state_no+=1
	parser.task_state[str(state_no)] = " Running"
	state_no+=1
	parser.task_state[str(state_no)] = " Inactive"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait Semaphore"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait FIN"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait Signal"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait MQ Receive (MQ Empty)"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait MQ Send (MQ Full)"
	state_no+=1
	parser.task_state[str(state_no)] = " Wait Page Fill"
	state_no+=1