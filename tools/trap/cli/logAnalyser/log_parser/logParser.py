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
# File : logParser.py
# Description: It contains a logParser class which has APIs for parsing assert/crash log by using elfs.
# Gdb and NM tools are used to read the symbols

from __future__ import print_function
import re
import os
import string
import sys, time
from array import *
import utils as utils
import logAnalyser.logUtils as logUtils
import logAnalyser.log_parser.heapNode as heapNode
import logAnalyser.log_parser.assertInfo as assertInfo
import logAnalyser.log_parser.crashPoint as crashPoint
import logAnalyser.log_parser.tcbInfo as tcbInfo

# Global variables
assertion_details = "Assertion details\n"
stack_details = "Asserted task's stack details\n"
register_dump = "Asserted task's register dump\n"
BIN_ADDR_FXN = "Loading location information\n"
tcb_info = "Asserted task's TCB info"

partition_string = "==========================================================="


class logParser:

	# Init function for logParser Class
	def __init__(self, elf = None, log_file = None, bin_path = None, config_path= None, xip_enabled = False, have_ram_kernel_text = False):
		self.elf = elf				         # Elf file
		self.log_file = log_file			 # Path of Log file passed as argument

		self.g_app_idx = 0                        # To extract number of apps dynamically
		self.g_assertpc = 0                       # To extract the global PC value g_assertpc
		self.g_stext_app = [0] * 10
		self.g_etext_app = [0] * 10
		self.app_name = []
		self.read_all_elf = False
		self.crash_type_assert = False
		self.task_state = dict()

		# Kernel text start & end addresses in FLASH & RAM regions
		self.g_stext_flash = 0
		self.g_etext_flash = 0
		self.g_stext_ram = 0
		self.g_etext_ram = 0

		self.bin_path = bin_path
		self.config_path = config_path
		self.xip_enabled = xip_enabled
		self.have_ram_kernel_text = have_ram_kernel_text

		logUtils.convert_stateno_statemsg(self)

	# API to Parse the call stack from input log file and to print stack values
	def parse_call_stack(self):
		stack_addr = 0x00000000
		stack_val = 0x00000000
		current_line = ""
			
		print('Stack_address\t Symbol_address\t Symbol location  {: <45}  File_name'.format("Symbol_name"))

		# Parse the contents based on tokens in log file.
		with open(self.log_file) as searchfile:
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
						t = word[2].split()
						# Check for corruption of PC value in logs
						if (len(word[2].split()) != 8):
							print('\nAssert logs are corrupted, and we are not able to determine the value of PC.\n')
							continue
					continue

				if 'stack:' in line:
					word = line.split(':')
					print(word[1])
				if 'stack dump:' in line:
					word = line.split(':')
					print(word[1])

				# Read the stack contents of aborted stack and check for valid addresses
				if current_line == stack_details and 'up_stackdump' in line:
					word = line.split(':')
					t = word[2].split()
					stack_addr = int(word[1], 16)
					for sub_word in t:
						stack_addr = stack_addr + 4
						# Check for valid address, else move to the next sub_word
						if (sub_word == 'xxxxxxxx'):
							continue
						if (utils.bytes_needed(int(sub_word,16)) != 4):
							continue

						stack_val = int(sub_word,16)
						# Check if the stack address lies in kernel text address range
						if (logUtils.is_kernel_text_address(self, hex(stack_val))):
							#If yes, print it's corresponding symbol
							utils.print_symbol(stack_addr, stack_val, 0, self.bin_path, self.app_name)
						else:
							if self.read_all_elf:
								if not self.xip_enabled:
									stack_val = stack_val - int(self.g_stext_app[is_app_symbol - 1])
								for i in range(self.g_app_idx):
									utils.print_symbol(stack_addr, stack_val, i + 1, self.bin_path, self.app_name)
							else:
								# Check if the stack address lies in application text address range
								is_app_symbol = logUtils.is_app_text_address(self, hex(stack_val))	# app index in case of application symbol
								if (is_app_symbol):
									#If yes, print it's corresponding symbol
									if not self.xip_enabled:
										stack_val = stack_val - int(self.g_stext_app[is_app_symbol - 1])
									utils.print_symbol(stack_addr, stack_val, is_app_symbol, self.bin_path, self.app_name)

	# Function to Parse the input log file (which contains wrong stackdump during assert)
	def print_wrong_sp(self):
		stack_addr = 0x00000000
		format_print = True

		# Parse the contents based on tokens in log file.
		with open(self.log_file) as searchfile:
			for line in searchfile:
				# Read the stack contents of aborted stack and check for valid addresses
				if 'Wrong Stack pointer' in line:
					word = line.split(':')
					t = word[2].split()
					s = word[1].split()
					stack_addr = int(s[-1], 16)
					for sub_word in t:
						stack_addr = stack_addr + 4
						# Check for valid address, else move to the next sub_word
						if (sub_word == 'xxxxxxxx'):
							continue
						if (utils.bytes_needed(int(sub_word,16)) != 4):
							continue

						stack_val = int(sub_word,16)
						# Check if the stack address lies in kernel text address range
						if (logUtils.is_kernel_text_address(self, hex(stack_val))):
							if (format_print):
								print('\t- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:')
								print('Stack_address\t Symbol_address\t Symbol location  {: <45}  File_name'.format("Symbol_name"))
								format_print = False
							#If yes, print it's corresponding symbol
							utils.print_symbol(stack_addr, stack_val, 0, self.bin_path, self.app_name)
						# Check if the stack address lies in application text address range
						is_app_symbol = logUtils.is_app_text_address(self, hex(stack_val))	# app index in case of application symbol
						if (is_app_symbol):
							if (format_print):
								print('\t- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:')
								print('Stack_address\t Symbol_address\t Symbol location  {: <45}  File_name'.format("Symbol_name"))
								format_print = False
							#If yes, print it's corresponding symbol
							if not self.xip_enabled:
								stack_val = stack_val - int(self.g_stext_app[is_app_symbol - 1])
							utils.print_symbol(stack_addr, stack_val, is_app_symbol, self.bin_path, self.app_name)

	# API to parse heap region information and check heap corruption details
	def parse_heap_info(self):
		current_line = ""
		print('\nh. Heap Region information:\n')

		with open(self.log_file) as searchfile:
			heap_corr = 0
			for line in searchfile:
				# Check if there is heap corruption or not
				if 'Heap corruption detected' in line:
						heap_corr = 1
			if (heap_corr == 1):
				print('\t!!!! HEAP CORRUPTION DETECTED !!!!\n\n')
			else:
				print('\t!!!! NO HEAP CORRUPTION DETECTED !!!!\n\n')

		with open(self.log_file) as searchfile:
		# Parse the contents based on tokens in log file for heap corruption
			for line in searchfile:
				# Print the heap corruption data (if any)
				if 'Checking kernel heap for corruption' in line:
					print("Checking kernel heap for corruption")
					line = next(searchfile)
					line = next(searchfile)
					if heapNode.parseCorruptHeapInfo(line, self.g_app_idx, self.g_stext_app, self.g_etext_app,self.app_name, self.elf, self.bin_path, self.xip_enabled, searchfile) == False:
						print("No Kernel heap corruption detected.\n")

				if 'Checking app heap for corruption' in line:
					print("Checking application heap for corruption")
					line = next(searchfile)
					line = next(searchfile)
					if heapNode.parseCorruptHeapInfo(line, self.g_app_idx, self.g_stext_app, self.g_etext_app,self.app_name, self.elf, self.bin_path, self.xip_enabled, searchfile) == False:
						print("No app heap corruption detected.\n")

	# API to print all the runnning TCB in the system
	def print_task_list(self):
		#Parse content of file for displaying tasks
		with open(self.log_file) as searchfile:
			for line in searchfile:
				if 'Stack overflow error has occurred' in line:
					print("\n!! Stack overflow error has occurred !!")
				if 'List of all tasks in the system' in line:
					print("\nList of all tasks in the system:\n")
					line = next(searchfile)
					line = next(searchfile)
					print(line[0:], end = "")
					line = next(searchfile)
					while line != "No more lines" and '==================' not in line:
						if '----------------------------' in line:
							print(line[0:], end = "")
							line = next(searchfile, "No more lines")
							continue
						current_line = line.replace("|", " ").replace("/", " ").replace("\n", " ").split(" ")
						current_line = list(filter(None, current_line))
						state = current_line[len(current_line) - 1]
						line = line.replace("         " + state, self.task_state[state])
						print(line[0:], end = "")
						line = next(searchfile, "No more lines")

	# Function to Parse the i/p log file in case of app crashes to corresponding app display debug symbols
	def parse_log(self):

		os.system("nm --defined-only -n -C " + self.bin_path + "tinyara.axf > " + self.bin_path + "Kernel.map")
		utils.setup_symbol_table(self.bin_path + "Kernel.map", 0)

		for idx in range(self.g_app_idx):
			os.system("nm --defined-only -n -C " + self.bin_path + self.app_name[idx] + "_dbg > " + self.bin_path + self.app_name[idx] + ".map")
			utils.setup_symbol_table(self.bin_path + self.app_name[idx] + ".map", idx + 1)

		# Find crash Point
		crashPoint.find_crash_point(self)

		# Find point of assertion and assertion details
		assertInfo.parse_assert_info(self)

		# It displays the debug symbols corresponding to all the addresses in the kernel and application text address range
		self.parse_call_stack()

		# Parse heap region information
		self.parse_heap_info()

		# Parse asserted tcb information
		tcbInfo.parse_tcb_info(self)

		print('\nx. Miscellaneous information:')

		# It displays the debug symbols corresponding to all the wrong sp addresses (if any)
		self.print_wrong_sp()

		# print current running task List
		self.print_task_list()

