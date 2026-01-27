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
import heapNode

# Global variables
assertion_details = "Assertion details\n"
stack_details = "Asserted task's stack details\n"
register_dump = "Asserted task's register dump\n"
BIN_ADDR_FXN = "Loading location information\n"
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

		self.convert_stateno_statemsg()

	def format_output(self,res , string):
		r = res.split('\n')
		print('\t- symbol addr {0}       : {1}'.format(string, r[0]))
		print('\t- function name {0}     : {1}'.format(string, r[1]))
		print('\t- file {0}              : {1}'.format(string, r[2]))

	def print_crash_type(self, string):
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
			self.crash_type_assert = True
			print('\n2. Crash type               : code assertion by code ASSERT or PANIC')
		else:
			print('\n2. Crash type               : etc')
		if (self.crash_type_assert == True):
			print('\n3. Crash point\n\t-', string.split(': ',1)[1])
		else:
			print('   Crash log\n\t-', string)

	# Function to check if address lies in the application's text address range
	def is_app_text_address(self, address):
		idx = 0
		# Check the application text address range
		for idx in range(self.g_app_idx):
			if (address >= hex(self.g_stext_app[idx]) and address < hex(self.g_etext_app[idx])):
				return (idx + 1)
		if (idx == self.g_app_idx):
			return False

	# Function to check if address lies in the kernel text address range
	def is_kernel_text_address(self, address):

		# Check the kernel text address range
		if (address >= hex(self.g_stext_ram) and address < hex(self.g_etext_ram)) or (address >= hex(self.g_stext_flash) and address < hex(self.g_etext_flash)):
			return True
		else:
			return False

	# Function to get the number of application binaries, names, text address and sizes
	def find_number_of_binaries(self):

		current_line = ""
		# Parse the contents based on tokens in log file.
		with open(self.log_file) as searchfile:
			for line in searchfile:
				if partition_string in line:
					line = next(searchfile)
					current_line = line
					line = next(searchfile)
					continue
				# Get the number of applications loaded
				if BIN_ADDR_FXN == current_line:
					self.g_app_idx = self.g_app_idx + 1

		app_idx = 0
		self.g_stext_app = array('i', range(0, self.g_app_idx))
		self.g_etext_app = array('i', range(0, self.g_app_idx))
		with open(self.log_file) as searchfile:
			for line in searchfile:
				if partition_string in line:
					line = next(searchfile)
					current_line = line
					line = next(searchfile)
					continue
				# Read the app text address and size
				if BIN_ADDR_FXN == current_line:
					word = line.split(':')
					t = word[2].split(',') # word[2] is the App Start Text address
					w = word[1].split(' ')
					# w[1] denotes string '[<app_name>]'
					start_idx = int(w[1].find('[')) + 1
					end_idx = int(w[1].find(']'))
					self.app_name.append(w[1][start_idx:end_idx])
					self.g_stext_app[app_idx] = int(t[0], 16)
					self.g_etext_app[app_idx] = self.g_stext_app[app_idx] + int(word[3], 10) # word[3] is text_size
					app_idx = app_idx + 1

		if app_idx == 0:
			app_count = 0
			app_names = []

			debug_files = [
				f for f in os.listdir(self.bin_path)
				if f.endswith("_dbg") and os.path.isfile(os.path.join(self.bin_path, f))
			]

			for f in debug_files:
				app_names.append(f.split("_")[0])
				self.app_name.append(app_names[app_count])
				app_count = app_count + 1

			if app_count > 0:
				self.read_all_elf = True
				self.g_app_idx = app_count
				self.g_stext_app = array('i', range(0, self.g_app_idx))
				self.g_etext_app = array('i', range(0, self.g_app_idx))

				for app_idx in range(app_count):
					#setting start and end address with minimum and maximum possible values for now when address is not available
					self.g_stext_app[app_idx] = int("0x00000000", 16)
					self.g_etext_app[app_idx] = int("0x77777777", 16)
			else:
				print("\nNo debug files found for common and app binaries\n")

	# API to find crash binary, crash point and crash type from assert log
	def find_crash_point(self):
		pc_value = 0
		lr_value = 0
		is_app_crash = 0
		is_kernel_crash = 0
		assertline = ""
		current_line = ""

		self.g_stext_flash = utils.get_address_of_symbol("_stext_flash")
		self.g_etext_flash = utils.get_address_of_symbol("_etext_flash")
		if (self.have_ram_kernel_text):
			self.g_stext_ram = utils.get_address_of_symbol("_stext_ram")
			self.g_etext_ram = utils.get_address_of_symbol("_etext_ram")

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
					self.g_assertpc = int(word[2].strip(),16)

		print('-----------------------------------------------------------------------------------------')
		address1 = hex(lr_value)
		address2 = hex(pc_value)
		result = 0
		# Check for lr & pc values in application text address range
		if (pc_value != 00000000):
			for app_idx in range(self.g_app_idx):
				if (address1 >= hex(self.g_stext_app[app_idx]) and address1 < hex(self.g_etext_app[app_idx])):
					if self.xip_enabled:
						addr = lr_value
					else:
						addr = lr_value - int(hex(self.g_stext_app[app_idx]), 16)
					f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr))
					result = f.read()
					if '??' not in result and '$d' not in result:
						is_app_crash = 1
						print('\n1. Crash Binary             : {0}'.format(self.app_name[app_idx]))
						self.print_crash_type(assertline)
						if (self.crash_type_assert == False):
							print('3. Crash point (PC or LR)')
							print('\n\t[ Caller - return address (LR) - of the function which has caused the crash ]')
							self.format_output(result, "")
				if (address2 >= hex(self.g_stext_app[app_idx]) and address2 < hex(self.g_etext_app[app_idx])):
					if self.xip_enabled:
						addr = pc_value
					else:
						addr = pc_value - int(hex(self.g_stext_app[app_idx]), 16)
					f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr))
					result = f.read()
					if '??' not in result and '$d' not in result:
						if (not is_app_crash):
							print('\n1. Crash Binary             : {0}'.format(self.app_name[app_idx]))
							self.print_crash_type(assertline)
							if (self.crash_type_assert == False):
								print('3. Crash point (PC or LR)')
						is_app_crash = 1
						if (self.crash_type_assert == False):
							print('\n\t[ Current location (PC) of assert ]')
							self.format_output(result, "")
					if ((addr - 4) > 0x0):
						f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr - 4))
						result1 = f.read()
						if '??' not in result1 and '$d' not in result1:
							if (self.crash_type_assert == False):
								print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
								self.format_output(result1, "of (pc - 4)")
					if ((addr - 8) > 0x0):
						f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr - 8))
						result2 = f.read()
						if '??' not in result2 and '$d' not in result2:
							if (self.crash_type_assert == False):
								print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
								self.format_output(result2, "of (pc - 8)")

		# Scenario when up_registerdump() is not present in dump logs, use g_assertpc value to give crash point
		else:
			address1 = hex(self.g_assertpc)
			for app_idx in range(self.g_app_idx):
				if (address1 >= hex(self.g_stext_app[app_idx]) and address1 < hex(self.g_etext_app[app_idx])):
					if self.xip_enabled:
						addr = self.g_assertpc
					else:
						addr = self.g_assertpc - int(hex(self.g_stext_app[app_idx]), 16)
					f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr))
					result = f.read()
					if '??' not in result and '$d' not in result:
						is_app_crash = 1
						print('\n1. Crash Binary             : {0}'.format(self.app_name[app_idx]))
						self.print_crash_type(assertline)
						if (self.crash_type_assert == False):
							print('3. Crash point (PC or LR)')
							print('\n\t[ Current location (PC) of assert ]')
							self.format_output(result, "")
					if ((addr - 4) > 0x0):
						f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr - 4))
						result1 = f.read()
						if '??' not in result1 and '$d' not in result1:
							if (self.crash_type_assert == False):
								print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
								self.format_output(result1, "of (pc - 4)")
					if ((addr - 8) > 0x0):
						f = os.popen('arm-none-eabi-addr2line -a -f -e ' + self.bin_path + self.app_name[app_idx] + '_dbg ' + hex(addr - 8))
						result2 = f.read()
						if '??' not in result2 and '$d' not in result2:
							if (self.crash_type_assert == False):
								print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
								self.format_output(result2, "of (pc - 8)")

		# Check for lr & pc values in kernel text address range
		if (not is_app_crash) and (pc_value != 00000000):
			address1 = hex(lr_value)
			if (address1 >= hex(self.g_stext_flash) and address1 < hex(self.g_etext_flash)) or (address1 >= hex(self.g_stext_ram) and address1 < hex(self.g_etext_ram)):
				# If yes, print the crash point using addr2line
				f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(lr_value))
				result = f.read()
				if '??' not in result and '$d' not in result:
					is_kernel_crash = 1
					print('1. Crash Binary             : kernel')
					self.print_crash_type(assertline)
					if (self.crash_type_assert == False):
						print('3. Crash point (PC or LR)')
						print('\n\t[ Caller - return address (LR) - of the function which has caused the crash ]')
						self.format_output(result, "")
			if (address2 >= hex(self.g_stext_flash) and address2 < hex(self.g_etext_flash)) or (address2 >= hex(self.g_stext_ram) and address2 < hex(self.g_etext_ram)):
				f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(pc_value))
				result = f.read()
				if '??' not in result and '$d' not in result:
					if (not is_kernel_crash):
						print('1. Crash Binary             : kernel')
						self.print_crash_type(assertline)
						if (self.crash_type_assert == False):
							print('3. Crash point (PC or LR)')
					is_kernel_crash = 1
					if (self.crash_type_assert == False):
						print('\n\t[ Current location (PC) of assert ]')
						self.format_output(result, "")
				if ((pc_value - 4) > 0x0):
					f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(pc_value - 4))
					result1 = f.read()
					if '??' not in result1 and '$d' not in result1:
						if (self.crash_type_assert == False):
							print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
							self.format_output(result1, "of (pc - 4)")
				if ((pc_value - 8) > 0x0):
					f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(pc_value - 8))
					result2 = f.read()
					if '??' not in result2 and '$d' not in result2:
						if (self.crash_type_assert == False):
							print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
							self.format_output(result2, "of (pc - 8)")

		# Scenario when up_registerdump() is not present in dump logs, use g_assertpc value to give crash point
		if (pc_value == 00000000 and self.g_assertpc):
			address1 = hex(self.g_assertpc)
			if (address1 >= hex(self.g_stext_flash) and address1 < hex(self.g_etext_flash)) or (address1 >= hex(self.g_stext_ram) and address1 < hex(self.g_etext_ram)):
				# If yes, print the crash point using addr2line
				f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(self.g_assertpc))
				result = f.read()
				if '??' not in result and '$d' not in result:
					is_kernel_crash = 1
					print('1. Crash Binary             : kernel')
					self.print_crash_type(assertline)
					if (self.crash_type_assert == False):
						print('3. Crash point (PC or LR)')
						print('\n\t[ Current location (PC) of assert ]')
						self.format_output(result, "")
				if ((self.g_assertpc - 4) > 0x0):
					f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(self.g_assertpc - 4))
					result1 = f.read()
					if '??' not in result1 and '$d' not in result1:
						if (self.crash_type_assert == False):
							print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
							self.format_output(result1, "of (pc - 4)")
				if ((self.g_assertpc - 8) > 0x0):
					f = os.popen('arm-none-eabi-addr2line -a -f -e' + self.elf + ' ' + hex(self.g_assertpc - 8))
					result2 = f.read()
					if '??' not in result2 and '$d' not in result2:
						if (self.crash_type_assert == False):
							print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
							self.format_output(result2, "of (pc - 8)")

		if (not is_app_crash) and (not is_kernel_crash):
			print('1. Crash Binary             : NA')
			self.print_crash_type(assertline)
			if (self.crash_type_assert == False):
				print('3. Crash point (PC or LR)')
				# Parse the contents based on tokens in log file.
				with open(self.log_file) as searchfile:
					for line in searchfile:
						# If PC value is invalid, show invalid PC
						if 'PC value might be invalid' in line:
							print('\tPC value might be invalid.')
				print('\t-  PC & LR values not in any text range! No probable crash point detected.')

	# API to find point of assertion
	def parse_assert_info(self):
		is_interrupt_mode = 0
		current_line = ""

		with open(self.log_file) as searchfile:
			for line in searchfile:
				if 'ERROR: Stack pointer is not within any of the allocated stack' in line:
					word = line.split(':')
					print('\n\t-', word[1], ':', word[2])

		print('\n4. Code asserted in:')

		# Parse the contents based on tokens in log file to determine point of assertion in details
		with open(self.log_file) as searchfile:
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
		with open(self.log_file) as searchfile:
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
					utils.print_interrupt_handler_data(self.log_file, self.bin_path, self.app_name)

		with open(self.log_file) as searchfile:
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
		with open(self.log_file) as searchfile:
			mm_fail = 0
			for line in searchfile:
				# Print the mm allocation failure data during crash (if crashed during mm allocation)
				if ('mm_manage_alloc_fail:' in line) and (mm_fail == 0):
					print('\nMemory allocation failure logs are as below:')
					mm_fail = 1
				if (mm_fail == 1):
					print(line)

		# Parse the contents based on tokens in log file.
		with open(self.log_file) as searchfile:
			for line in searchfile:
				if partition_string in line:
					line = next(searchfile)
					current_line = line
					line = next(searchfile)
					continue
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
					utils.print_running_work_function(self.log_file, self.bin_path, self.app_name)

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
						if (self.is_kernel_text_address(hex(stack_val))):
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
								is_app_symbol = self.is_app_text_address(hex(stack_val))	# app index in case of application symbol
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
						if (self.is_kernel_text_address(hex(stack_val))):
							if (format_print):
								print('\t- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:')
								print('Stack_address\t Symbol_address\t Symbol location  {: <45}  File_name'.format("Symbol_name"))
								format_print = False
							#If yes, print it's corresponding symbol
							utils.print_symbol(stack_addr, stack_val, 0, self.bin_path, self.app_name)
						# Check if the stack address lies in application text address range
						is_app_symbol = self.is_app_text_address(hex(stack_val))	# app index in case of application symbol
						if (is_app_symbol):
							if (format_print):
								print('\t- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:')
								print('Stack_address\t Symbol_address\t Symbol location  {: <45}  File_name'.format("Symbol_name"))
								format_print = False
							#If yes, print it's corresponding symbol
							if not self.xip_enabled:
								stack_val = stack_val - int(self.g_stext_app[is_app_symbol - 1])
							utils.print_symbol(stack_addr, stack_val, is_app_symbol, self.bin_path, self.app_name)

	# API to parse asserted tcb information
	def parse_tcb_info(self):
		current_line = ""
		print("\nt. Asserted TCB info:\n")
		with open(self.log_file) as searchfile:
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
							line = line.replace(" " + data, self.task_state[data.split()[0]])
						elif "Syscall" in line:
							app_idx = self.is_app_text_address(data)
							if app_idx:
								if self.xip_enabled:
									addr = data
								else:
									addr = data - int(hex(self.g_stext_app[app_idx - 1]), 16)
								f = os.popen('arm-none-eabi-addr2line -f -e ' + self.bin_path + self.app_name[app_idx - 1] + '_dbg ' + hex(int(addr, 16)))
								result = f.read()
								if '??' not in result and '$d' not in result:
									line = line.replace(data, result.replace("\n"," ").split()[0])
							elif self.is_kernel_text_address(data):
								f = os.popen('arm-none-eabi-addr2line -f -e ' + self.elf + ' ' + hex(int(data, 16)))
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

	# API to parse TCB state to corresponding TCB state msg
	def convert_stateno_statemsg(self):
		# Convert task state number to corresponding task state message
		config_smp = False
		config_disable_signals = False
		config_disable_mqueue = False
		config_paging = False
		with open(self.config_path) as configfile:
			for line in configfile:
				if "CONFIG_SMP=y" in line:
					config_smp = True
				elif "CONFIG_DISABLE_SIGNALS=y" in line:
					config_disable_signals = True
				elif "CONFIG_DISABLE_MQUEUE=y" in line:
					config_disable_mqueue = True
				elif "CONFIG_PAGING=y" in line:
					config_paging = True
		state_no = 0
		self.task_state[str(state_no)] = " Invalid"
		state_no+=1
		self.task_state[str(state_no)] = " Pending preemption unlock"
		state_no+=1
		self.task_state[str(state_no)] = " Wait to scheduling (Ready)"
		state_no+=1
		self.task_state[str(state_no)] = " Assigned to CPU (Ready)"
		state_no+=1
		self.task_state[str(state_no)] = " Running"
		state_no+=1
		self.task_state[str(state_no)] = " Inactive"
		state_no+=1
		self.task_state[str(state_no)] = " Wait Semaphore"
		state_no+=1
		self.task_state[str(state_no)] = " Wait FIN"
		state_no+=1
		self.task_state[str(state_no)] = " Wait Signal"
		state_no+=1
		self.task_state[str(state_no)] = " Wait MQ Receive (MQ Empty)"
		state_no+=1
		self.task_state[str(state_no)] = " Wait MQ Send (MQ Full)"
		state_no+=1
		self.task_state[str(state_no)] = " Wait Page Fill"
		state_no+=1

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
		self.find_crash_point()

		# Find point of assertion and assertion details
		self.parse_assert_info()

		# It displays the debug symbols corresponding to all the addresses in the kernel and application text address range
		self.parse_call_stack()

		# Parse heap region information
		self.parse_heap_info()

		# Parse asserted tcb information
		self.parse_tcb_info()

		print('\nx. Miscellaneous information:')

		# It displays the debug symbols corresponding to all the wrong sp addresses (if any)
		self.print_wrong_sp()

		# print current running task List
		self.print_task_list()

	# Function to format logs and delete the timestamp (supported formats-|xxxxxxxxx| and [xxxxxxxxx]) if it consists of timestamp at the start of each log line
	def format_log_file(self):

		current_line = ""  # Initialize current_line

		# Delete unwanted logs (if any) and timestamp at the start of each line
		with open(self.log_file, "r") as f:
			data = f.readlines()
		with open(self.log_file, "w") as f:
			data = iter(data)
			for line in data:
				delete_idx = 0
				# Timestamp present if line starts with '|'
				if line[0] == '|' or line[0] == '[':
					for idx in range(1, len(line)):
						if '|' == line[idx] or ']' == line[idx]:
							delete_idx = idx + 1
							break
				if line[delete_idx] == ' ':	# Check for trailing white spaces
					delete_idx = delete_idx + 1
				line = line[delete_idx:]
				f.write(line)

		# Check for invalid format after above formatting
		with open(self.log_file, "r") as f:
			data = f.readlines()
			data = iter(data)
			for line in data:
				if partition_string in line:
					line = next(data)
					current_line = line
					line = next(data)
					continue
				if 'Assertion failed at file:' in line and current_line == assertion_details:
					word = line.split()
					if word[1] != 'Assertion':
						for idx in range(0, len(line)):
							if 'A' == line[idx]:
								delete_idx = idx
								break
						correctFormatString = line[delete_idx:]
						print ("\n\t- Below log format is not supported in TRAP")
						print ('\t\t-{0}\t- Instead, supported log format in TRAP is as follows:'.format(line))
						print ("\t\t-{0} {1}\n\tKindly modify the log file as per accepted format.\n".format(word[word.index('Assertion')-1], correctFormatString))
						sys.exit(1)
