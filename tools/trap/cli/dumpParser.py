#!/usr/bin/env python
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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
# File : dumpParser.py
# Description: It contains a dumpParser class which has APIs mainly for parsing RAM/Flash dump.
# Gdb and NM tools are used to read the symbols

from __future__ import print_function
import re
import os
import string
import sys, time
import struct
import subprocess
import utils as utils

stack_details = "Asserted task's stack details\n"
register_dump = "Asserted task's register dump\n"

HEAPINFO_PATH = '../../../os/include/tinyara/mm/heapinfo_internal.h'
file_data = 'HeapInfo'


# Top level class to parse the RAM/FLASH dump
class dumpParser:

	# Sub class to hold the essential stack frame registers
	class Stackframe ():
		def __init__(self, fp, sp, lr,pc):
			self.fp = fp
			self.sp = sp
			self.lr = lr
			self.pc = pc

	# Init function for dumpParser Class
	def __init__(self, dump_file = None, elf = None, bin_path = None, config_path = None, have_ram_kernel_text = False, debug = False):
		self.dump_file = dump_file		# dump file
		self.elf = elf				# Elf file
		self.bin_path = bin_path
		self.stack_table = []			# Lookup table created from stack contents of assert log file

		# These 2 addresses will be overwritten with ramdump file name
		self.ram_base_addr = 0x4A0000		# Default RAM start address for ARTIK board
		self.ram_end_addr = 0x6A0000		# Default RAM end address for ARTIK board
		self.stacksize = 0 			# Stack size set to 0 and it gets updated dynamically while using
		self.have_ram_kernel_text = have_ram_kernel_text # Variable to check if board has kernel text in RAM region

		utils.setup_symbol_table(bin_path + "System.map", 0)

		# Kernel text start & end addresses in FLASH & RAM regions
		self.g_stext_flash = utils.get_address_of_symbol("_stext_flash")
		self.g_etext_flash = utils.get_address_of_symbol("_etext_flash")
		self.g_stext_ram = 0
		self.g_etext_ram = 0
		if (have_ram_kernel_text):
			self.g_stext_ram = utils.get_address_of_symbol("_stext_ram")
			self.g_etext_ram = utils.get_address_of_symbol("_etext_ram")


		# Check for ELF file existence, if exists open the ELF (tinyara) file in binary format
		# As well as open using NM utility so that, we can read the symbols easily
		if self.elf is not None:
			self.elf_file_fd = open(elf, 'rb')
			if not self.elf_file_fd:
				print(('Failed to open {0}'.format(elf)))
				return None

		# Check for ramdump file existence, if exists open the dump file in binary format
		if self.dump_file is not None:
			self.dump_file_fd = open(dump_file, 'rb')
			if not self.dump_file_fd:
				print(('Failed to open {0}'.format(dump_file)))
				return None

			#Split the dump_file with delimiters _ and . to read the start and end addr
			# This is required to support any RAM dump size of any board
			temp = re.split(r'[_.]',dump_file)
			# Get the Start address and end address from dump file name
			self.ram_base_addr = int(temp[-3],16)
			self.ram_end_addr = int(temp[-2],16)
			print(('self.ram_base_addr {0:x}'.format(self.ram_base_addr)))
			print(('self.ram_end_addr {0:x}'.format(self.ram_end_addr)))

		# Read the elf header to get the offset of text section and ARM exidx section
		# These offsets will be used while creating ARM exidx table as well as while reading
		# the address from ELF file at a particular text address
		with os.popen('arm-none-eabi-readelf -S ' + elf) as readelf_fd:
			elfdata = readelf_fd.readlines()
			for line in elfdata:
				if '.text' in line:
					word = line.split()
					#word 5 contains the offset value
					self.text_offset = word[5]
					if debug:
						print(line)
						print(self.text_offset)

	def __del__(self):
		self.elf_file_fd.close() # Close the elf file instance

	# Function to search the instructions present in stack address.
	def stack_lookup(self, addr):
		if(addr is None):
			return ('(Invalid address)', 0x0)
		low = 0
		high = len(self.stack_table)
		mid = (low + high) >> 1
		premid = 0

		while(low <= high):

			if(addr == self.stack_table[mid][0]):
				return self.stack_table[mid][1]

			if(addr < self.stack_table[mid][0]):
				high = mid - 1

			else:
				low = mid + 1


			mid = (high + low) >> 1

		print("Sorry. Given address does not present in stack lookup")
		return None


	# Function to read the contents of given length from specific RAM/ELF address
	def read_address(self, addr, length, debug=False):

		# If the given address does not belong to ELF section, read from DUMP
		# Calculate the OFFSET in the FILE by subtracting RAM start address
		offset = addr - self.ram_base_addr

		# Also check whether address falls within the code section, if so read from elf
		if (self.have_ram_kernel_text):
			if (addr >= self.g_stext_flash and addr <= self.g_etext_flash) or (addr >= self.g_stext_ram and addr <= self.g_etext_ram):
				# Checking in ELF file once for the offset at which we need to read the address
				offset = (addr - self.g_stext_ram) + int(self.text_offset, 16)
				if debug:
					print(('address {0:x} is in text range'.format(addr)))
				self.elf_file_fd.seek(offset)
				a = self.elf_file_fd.read(length)
				return a
		elif not(self.have_ram_kernel_text):
			if (addr >= self.g_stext_flash and addr <= self.g_etext_flash):
				# Checking in ELF file once for the offset at which we need to read the address
				offset = (addr - self.g_stext_flash) + int(self.text_offset, 16)
				if debug:
					print(('address {0:x} is in text range'.format(addr)))
				self.elf_file_fd.seek(offset)
				a = self.elf_file_fd.read(length)
				return a

		if debug:
			print('offset = {0:x}'.format(offset))
			print('length = {0:x}'.format(length))

		# If dump file is Passed, Read contents of address from DUMP file descriptor
		if self.log_file is None:
			self.dump_file_fd.seek(offset)
			a = self.dump_file_fd.read(length)
		else:
			print('AM HERE, SOMETHING WRONG')
			# If only Log file is passed as i/p, Read the contents of address from ELF file
			self.elf_file_fd.seek(offset+self.text_offset) # offset got from elf header
			a = self.elf_file_fd.read(length)
		return a

	# Returns a tuple of the result by reading the address from the "specified format string"
	# In case of failure, return None
	def read_string(self, address, format_string, debug = False):
		addr = address

		s = self.read_address(addr, struct.calcsize(format_string), debug)
		if (s is None) or (s == ''):
			if debug and addr is not None:
				print(('Failed to read address {0:x}'.format(addr)))
			return None

		# Unpack the string with proper format and return to caller
		return struct.unpack(format_string, s)

	# Returns a word size (4 bytes = 32 bits) read from dump "<" means little endian format
	# I indicates word
	def read_word(self, address, debug = False):
		if debug:
			print('reading {0:x}'.format(address))
		s = self.read_string(address, '<I', debug)
		if s is None:
			if debug:
				print('read_word s is None')
			return None
		else:
			return s[0]
        # Returns a single Byte read from given dump address "<" means little endian format
	# H indicates Half word
	def read_halfword(self, address, debug=False):
		if debug:
			print('reading {0:x}'.format(address))
		s = self.read_string(address, '<h', debug)
		if s is None:
			if debug:
				print('read_halfword s is None')
			return None
		else:
			return s[0]

	# Returns a single Byte read from given dump address "<" means little endian format
	# B indicates Byte, It's useful while reading a String from dump
	def read_byte(self, address, debug=False):
		if debug:
			print('reading {0:x}'.format(address))
		s = self.read_string(address, '<B', debug)
		if s is None:
			if debug:
				print('read_byte s is None')
			return None
		else:
			return s[0]

	# Function to unwind the FRAME with generic method of using Frame pointer
	def unwind_frame_using_framepointer(self, frame, debug=False):
		high = 0
		fp = frame.fp

		low = (frame.sp)		# Get the Stack pointer
		mask = (self.stacksize) - 1	# Prepare Mask for stacksize for ex.stack:0x8000, mask=0x7FFF

		high = (low + mask) & (~mask)   # Get the end of stack with alignment.ALIGN(low, THREAD_SIZE)

		# Verify the Addresses passed
		if debug:
			print("fp:{0:x}  sp low:{1:x} high:{2:x}".format(frame.fp, frame.sp, high))

		# Check current frame pointer is within bounds of Stack*/
		if (fp < (low + 12) or fp + 4 >= high):
			print("End of stack frame")
			return -1

		# If the dump is given, Read contents of address from the DUMP file
		# by using read_word API
		if self.log_file is None:
			fp_is_at = self.read_word(frame.fp - 12)
			sp_is_at = self.read_word(frame.fp - 8)
			pc_is_at = self.read_word(frame.fp - 4)
		else:
			# If only log file is given, Read contents of addr from the populated stack_table
			fp_is_at = self.stack_lookup(frame.fp - 12)
			sp_is_at = self.stack_lookup(frame.fp - 8)
			pc_is_at = self.stack_lookup(frame.fp - 4)

		frame.fp = fp_is_at
		frame.sp = sp_is_at
		frame.pc = pc_is_at
		return 0

	# Function to walk the stack frame when framepointer is enabled
	def find_stackframe_using_framepointer(self, frame, extra_str='', output_file=None):
		while True:

			offset = 0
			# If PC is Null, prefetch abort or Something wrong, Break
			if frame.pc is None:
				break
			if(frame.pc == 0):
				break

			r = utils.symbol_table_lookup(frame.pc)
			if r is None:
				symname = 'UNKNOWN'
				offset = 0x0
			else:
				symname, offset = r # Update both symbol name and offset

			address = '{0:x}'.format(frame.pc)
			cmd = ['arm-none-eabi-addr2line','-e', self.elf, address]
			fd_popen = subprocess.Popen(cmd, stdout=subprocess.PIPE).stdout
			data = fd_popen.read().decode()
			temp = data.split('/')[-1]
			line = temp.split(':')[-1].rstrip()
			name = temp[:-len(line)-2]
			pstring = (extra_str + '[<{0:x}>] {1}+0x{2:x} [Line {3} of {4}]'.format(frame.pc, symname, offset, line, name))
			if output_file:
				output_file.write(pstring + '\n')
			else:
				print(pstring)

			# Unwind the complete Stack with the help of Frame pointer register
			ret = self.unwind_frame_using_framepointer(frame)
			if ret < 0:
				break

	# Function to backtrace the stack in Generic method, which uses frame pointer register
	# First function to get called to unwind
	def unwind_backtrace_with_framepointer(self, fp, sp, lr, pc, ss):
		frame = self.Stackframe(fp,sp,lr,pc)
		frame.fp = fp
		frame.sp = sp
		frame.lr = lr
		frame.pc = pc
		self.stacksize = ss
		self.find_stackframe_using_framepointer(frame)

	# Function to parse ram dump if log file is not given.
	def parse_ram_dump(self):

		FP = 11
		SP = 13
		LR = 14
		PC = 15

		# Read config information
		fd = open(self.config_path, 'r')
		data = fd.read()
		fd.close()

		# Unwind the Callstack by obtaining FP, SP and PC from the proper global variable
		# task_ctxt_regs is used if SEC_DEBUG is enabled , else current_regs is used
		current_regs_pointer = utils.get_address_of_symbol("current_regs")
		if current_regs_pointer is not None:
			ctxt_regs = self.read_word(current_regs_pointer)
			if ctxt_regs != 0x0:
				print('current_regs is not NULL')
			else:
				cpu_ctxt_regs_pointer = utils.get_address_of_symbol("cpu_ctxt_regs")
				if cpu_ctxt_regs_pointer is not None:
					ctxt_regs = cpu_ctxt_regs_pointer

		if ctxt_regs is None:
			print('System is not crashed')
			return None

		framePointer = self.read_word(ctxt_regs+(4* FP))
		stackPointer = self.read_word(ctxt_regs+(4* SP))
		linkRegister = self.read_word(ctxt_regs+(4*LR))
		programCounter = self.read_word(ctxt_regs+(4*PC))
		# There are spl case where PC can be invalid, So assigning LR to PC
		if (not(programCounter >= self.g_stext_flash and programCounter <= self.g_etext_flash)):
			if (self.have_ram_kernel_text):
				if (not(programCounter >= self.g_stext_ram and programCounter <= self.g_etext_ram)):
					# This is possible for a prefetch abort. so am taking care by assigning LR to PC
					print("It's a Prefetch abort at Addr : ", hex(programCounter))
					programCounter = linkRegister
			else:
				# This is possible for a prefetch abort. so am taking care by assigning LR to PC
				print("It's a Prefetch abort at Addr : ", hex(programCounter))
				programCounter = linkRegister


		# Explicitly getting the PC and LR symbol names for display purpose
		temp1 = utils.symbol_table_lookup(programCounter)
		if temp1 is None:
			temp1_symname = 'UNKNOWN'
			temp1_offset = 0x0
		else:
			temp1_symname, temp1_offset = temp1

		temp2 = utils.symbol_table_lookup(linkRegister)
		if temp2 is None:
			temp2_symname = 'UNKNOWN'
			temp2_offset = 0x0
		else:
			temp2_symname, temp2_offset = temp2

		# Reading the Current Task from g_readytorun list, usually head is the current
		# Since its a pointer, first am getting pointer address and then reading the
		# contents of that Pointer to get the proper task address
		current_task_pointer = utils.get_address_of_symbol('g_readytorun')
		current_task_addr = self.read_word(current_task_pointer)
		# Reading the stack size value from tbe tcb_s struct
		stackSize = self.read_word(current_task_addr+0x2c)

		print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')
		print('')
		print("Board Crashed at :")
		print("PC: [0x{0:x}] {1}+0x{2:x}]\"".format(programCounter,temp1_symname,temp1_offset))
		print("LR: [0x{0:x}] {1}+0x{2:x}]\"".format(linkRegister,temp2_symname,temp2_offset))
		print(' ')
		print("FP: 0x{0:x} and SP: 0x{1:x}".format(framePointer, stackPointer))
		print(' ')
		print('CALL STACK of Aborted task: ')
		print('*******************************************************************')

		self.unwind_backtrace_with_framepointer(framePointer,stackPointer,linkRegister,programCounter,stackSize)
		print('')
		print('')
		print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')


		g_kmmheap = utils.get_address_of_symbol("g_kmmheap")

		# This information depends on the mm_heap_s structure

		SIZE_OF_MM_SEM = 8
		SIZE_OF_MM_HOLDER = 4
		SIZE_OF_MM_COUNTS_HELD = 4

		HEAP_SIZE_POINT = SIZE_OF_MM_SEM + SIZE_OF_MM_HOLDER + SIZE_OF_MM_COUNTS_HELD
		SIZE_OF_MM_HEAPSIZE = 4

		PEAK_ALLOC_SIZE_POINT = HEAP_SIZE_POINT + SIZE_OF_MM_HEAPSIZE
		SIZE_OF_PEAK_ALLOC_SIZE = 4

		TOTAL_ALLOC_SIZE_POINT = PEAK_ALLOC_SIZE_POINT + SIZE_OF_PEAK_ALLOC_SIZE
		SIZE_OF_TOTAL_ALLOC_SIZE = 4

		SIZE_OF_MAX_GROUP = 4
		SIZE_OF_HEAPINFO_TCB_INFO = 16

		# Read HEAPINFO_PATH to procure HEAPINFO_USER_GROUP_NUM value

		heapinfo_user_group_num = 0
		with open(HEAPINFO_PATH) as searchfile:
			for line in searchfile:
				if 'HEAPINFO_USER_GROUP_DELIM_NUM' in line:
					word = line.split(' ')
					# word[2] contains the value
					heapinfo_user_group_delim_num = int(word[2], 16)
					heapinfo_user_group_num = heapinfo_user_group_delim_num + 1
					break

		TOTAL_HEAPINFO_TCB_INFO_POINT = SIZE_OF_TOTAL_ALLOC_SIZE + SIZE_OF_MAX_GROUP + (SIZE_OF_HEAPINFO_TCB_INFO * heapinfo_user_group_num)
		ALLOC_LIST_POINT = TOTAL_ALLOC_SIZE_POINT + TOTAL_HEAPINFO_TCB_INFO_POINT

		SIZE_OF_ALLOC_NODE = 16

		max_tasks = 0
		# Get MAX_TASKS num
		if 'CONFIG_MAX_TASKS=' in data:
			index = data.find('CONFIG_MAX_TASKS=')
			index += len('CONFIG_MAX_TASKS=')
			while data[index] != '\n' :
				max_tasks *= 10
				max_tasks += int(data[index])
				index += 1

		HEAP_START_POINT = (ALLOC_LIST_POINT + (max_tasks * SIZE_OF_HEAPINFO_TCB_INFO))

		MM_ALLOC_BIT = 0x80000000

		start_heap = self.read_word(g_kmmheap + HEAP_START_POINT)
		end_heap = self.read_word(g_kmmheap + HEAP_START_POINT + 4)

		print('')
		print('')
		print('Details of Heap Usages (Size in Bytes)')
		print('')
		print('start heap address : ', hex(start_heap))
		print('end heap address   : ', hex(end_heap))
		print('')
		point = start_heap + SIZE_OF_ALLOC_NODE

		stack_size = [0 for i in range(max_tasks)]

		idle_stack_size = 0
		if 'CONFIG_IDLETHREAD_STACKSIZE=' in data:
			index = data.find('CONFIG_IDLETHREAD_STACKSIZE=')
			index += len('CONFIG_IDLETHREAD_STACKSIZE=')
			while data[index] != '\n' :
				idle_stack_size *= 10
				idle_stack_size += int(data[index])
				index += 1
		stack_size[0] = idle_stack_size
		print('******************************************************************')
		print('  MemAddr |   Size   | Status |  Pid  |           Owner           ')
		print('----------|----------|--------|-------|---------------------------')
		f = open(file_data, 'w')
		while point < end_heap:
			size = self.read_word(point)
			preceding = self.read_word(point + 4)
			owner = self.read_word(point + 8)
			pid = self.read_halfword(point + 12)
			if preceding & MM_ALLOC_BIT :
				fd_popen = subprocess.Popen(['arm-none-eabi-addr2line','-e', elf, hex(owner)], stdout=subprocess.PIPE).stdout
				data = fd_popen.read().decode()
				fd_popen.close()
				if pid >= 0:
					print('{:^10}|'.format(hex(point)), '{:>6}'.format(size), '  |', '{:^7}|'.format('Alloc'), '{:^6}|'.format(pid), data[13:], end=' ')
					f.write(str(size) + ' 0 ' + str(hex(point)) + ' ' + str(pid) + ' ' + data[14:])
				else: # If pid is less than 0, it is the stack size of (-pid)
					stack_size[(-pid) & (max_tasks - 1)] = size
					print('{:^10}|'.format(hex(point)), '{:>6}'.format(size), '  |', '{:^7}|'.format('Alloc'), '{:^6}|'.format(-pid), data[13:], end=' ')
					f.write(str(size) + ' 1 ' + str(hex(point)) + ' ' + str(-pid) + ' ' + data[14:])
			else:
				print('{:^10}|'.format(hex(point)), '{:>6}'.format(size), '  |', '{:^7}|'.format('Free'), '{:6}|'.format(""))
				f.write(str(size) +' 2 ' + str(hex(point)))
			# next node
			point = point + size

		f.close()
		print('')
		print('***********************************************************')
		print('       Summary of Heap Usages (Size in Bytes)')
		print('***********************************************************')

		heap_size = self.read_word(g_kmmheap + HEAP_SIZE_POINT)
		print('HEAP SIZE        : ', heap_size)

		peack_alloc_size = self.read_word(g_kmmheap + PEAK_ALLOC_SIZE_POINT)
		print('PEAK ALLOC SIZE  : ', peack_alloc_size)

		total_alloc_size = self.read_word(g_kmmheap + TOTAL_ALLOC_SIZE_POINT)
		print('TOTAL ALLOC SIZE : ', total_alloc_size)
		print('FREE SIZE        : ', heap_size - total_alloc_size)
		print('')
		print('***********************************************************')
		print('  PID  | STACK SIZE |  CUR ALLOC SIZE   | PEAK ALLOC SIZE |')
		print('-------|------------|-------------------|-----------------|')
		INVALID_PROCESS_ID = 0xFFFFFFFF

		alloc_list = ALLOC_LIST_POINT + g_kmmheap
		for i in range(0, max_tasks):
			pid = self.read_word(alloc_list)
			if pid != INVALID_PROCESS_ID :
				# This information depends on the heapinfo_tcb_info_t
				cur_alloc = self.read_word(alloc_list + 4)
				peak_alloc = self.read_word(alloc_list + 8)
				print('{:^7}|'.format(pid), '{:>7}'.format(stack_size[i]), '   |', '{:>13}'.format(cur_alloc), '    |', '{:>13}'.format(peak_alloc), '  |')
			# next alloc list
			alloc_list += SIZE_OF_HEAPINFO_TCB_INFO
