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
# File : ramdumpParser.py
# Description:
# Parser to parse the dump/log file using elf file and print CALL STACK.
# Gdb and NM tools are used to read the symbols

from __future__ import print_function
import re
import os
import string
import sys, time
import struct
import subprocess
from getopt import GetoptError, getopt as GetOpt
from array import *

# Global variables
FP = 11
SP = 13
LR = 14
PC = 15

# Variable to check if board has kernel text in RAM region
have_ram_kernel_text = False

# Kernel text start & end addresses in FLASH & RAM regions
g_stext_flash = 0
g_etext_flash = 0
g_stext_ram = 0
g_etext_ram = 0
g_app_idx = 0 # To extract number of apps dynamically
g_assertpc = 0 # To extract the global PC value g_assertpc
g_stext_app = [0] * 10
g_etext_app = [0] * 10
app_name = []
crash_type_assert = False

BIN_PATH = '../../build/output/bin/'
CONFIG_PATH = '../../os/.config'
MAKEFILE_PATH = '../../os/Make.defs'
HEAPINFO_PATH = '../../os/include/tinyara/mm/heapinfo_internal.h'
BIN_ADDR_FXN = 'elf_show_all_bin_section_addr'
file_data = 'HeapInfo'

# Top level class to parse the dump and assert logs parsing feature
class dumpParser:

	# Sub class to hold the essential stack frame registers
	class Stackframe ():
		def __init__(self, fp, sp, lr,pc):
			self.fp = fp
			self.sp = sp
			self.lr = lr
			self.pc = pc

	# Init function for dumpParser Class
	def __init__(self, dump_file=None, elf=None, log_file=None, debug=False):
		self.dump_file=dump_file		# dump file
		self.elf=elf				# Elf file
		self.log_file=log_file			# Path of Log file passed as argument
		self.symbol_lookup_table = []		# Lookup table to store key(addr) value(symbol) from elf
		self.stack_table = []			# Lookup table created from stack contents of assert log file
		# These 2 addresses will be overwritten with ramdump file name
		self.ram_base_addr=0x4A0000		# Default RAM start address for ARTIK board
		self.ram_end_addr=0x6A0000		# Default RAM end address for ARTIK board
		self.stacksize=0 			# Stack size set to 0 and it gets updated dynamically while using

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

	# Function to Parse the i/p log file (which contains stackdump during assert) passed as part of -t argument ( No dump is supplied )
	def parse_file(self,log_file,debug=False):
		k = 1
		i = 0
		got = 0
		offset = 0
		extra_str = ''
		# Parse the contents based on tokens in log file.
		with open(log_file) as searchfile:
			for line in searchfile:
				# It's imp to get the stack size dynamically as it's diff for diff tasks
				if 'up_dumpstate:' in line:
					word = re.split(r'[:]',line)
					if word[1] == ' User stack':
						continue
					if word[1] == '   size':
						self.stacksize = int(word[2],16)
					if debug:
						print("stackSize :", self.stacksize)
					continue
				# Read the stack contents of aborted stack and Populate stack_table
				if 'up_stackdump:' in line:
					word = line.split(':')
					#print word[2]
					t = word[2].split( )
					if k == 1:
						self.stack_start_addr = int(word[1], 16)
						#print 'stack_start_addr: ', hex(self.stack_start_addr)
						k = 0
					for sub_word in t:
						self.stack_table.append((self.stack_start_addr,int(sub_word,16)))
						self.stack_start_addr = self.stack_start_addr + 4
						# Print the generated Stack table
						if debug:
							print("{0:x}   {1:x}".format(self.stack_table[i][0],self.stack_table[i][1]))
						i = i + 1
					continue
				# Read only critical ARM registers ( PC, LR , SP and FP )
				if 'up_registerdump:' in line:
					word = line.split(':')
					if word[1] == ' R0': # Observe the space before R0
						continue
					if word[1] == ' R8': # Observe the space before R8
						t = word[2].split()
						pc = int(t[-1],16) # converting string to base 16 (hex)
						lr = int(t[-2],16)
						sp = int(t[-3],16)
						fp = int(t[-5],16)
						if debug:
							print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')
							print(' ')
							print('stack size : ', self.stacksize)
							print('PC : ', hex(pc))
							print('LR : ', hex(lr))
							print('SP : ', hex(sp))
							print('FP : ', hex(fp))
							print(' ')
							print(' ')

						# If the PC is not within kernel text range, it's a Prefetch issue
						# So, fill the PC with LR and help to get the call stack
						if (not(pc >= g_stext_flash and pc <= g_etext_flash)):
							if (have_ram_kernel_text):
								if (not(pc >= g_stext_ram and pc <= g_etext_ram)):
									print("It'S A PRE-FETCH ABORT @ PC", hex(pc))
									# Assign LR to PC to help constructing the stack
									pc = lr
							else:
								print("It'S A PRE-FETCH ABORT @ PC", hex(pc))
								# Assign LR to PC to help constructing the stack
								pc = lr

					continue
				# In case if log file already has this data, address to symbol mapping is enough to get the call stack.
				if 'unwind_backtrace_with_fp:' in line:
					word = line.split(':')
					if word[1] == ' CallTrace_Start':
						got = 1
						print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')
						print('Call Trace:')
						print('')
						continue
					if word[1] == ' CallTrace_End':
						got = 0
						print('')
						print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')
					if got == 1:
#						word.line.split(':')
						word = re.split(r'[:><\[\]]',line)
						r = self.symbol_table_lookup(int(word[3],16))
						if r is None:
							symname = 'UNKNOWN'
							offset = 0x0
						else:
							symname, offset = r

						pstring = (extra_str + '[<{0:x}>] {1}+0x{2:x}'.format(int(word[3],16), symname, offset))
						print(pstring)
					continue
		print('CALL STACK of Aborted task:')
		print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')
		print('')
		# Since Fp is enabled, use simple approach to get backtrace
		self.unwind_backtrace_with_framepointer(fp,sp,lr,pc,self.stacksize)
		print('')
		print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')


	# Function to setup the Address to Symbol mapping table from the ELF file ( tinyara in our case)
	def setup_symbol_table(self,tinyara_elf_file, debug=False):
		# Reading the tinyara elf and preparing the symbol map table
		with os.popen('arm-none-eabi-nm -n ' + tinyara_elf_file) as elf_file_fd_nm:
			symbols = elf_file_fd_nm.readlines()
			for line in symbols:
				s = line.split(' ')
				if len(s) == 3:
					if not '$' in s[2]:
						# s[0] contains address and s[2] contains Symbol name
						self.symbol_lookup_table.append((int(s[0], 16), s[2].rstrip()))

			# Printing the Created Symbol table
			if debug:
				print('~~~~~~~~~~~~~~~~~~~~~~~~ SYMBOL TABLE START ~~~~~~~~~~~~~~~~~~~~~')
				for line in self.symbol_lookup_table:
					print("{0:x} {1}".format(line[0], line[1]))
				print('~~~~~~~~~~~~~~~~~~~~~~~~ SYMBOL TABLE END   ~~~~~~~~~~~~~~~~~~~~~')

	# Function to read the contents of given length from specific RAM/ELF address
	def read_address(self, addr, length, debug=False):
		global have_ram_kernel_text

		# If the given address does not belong to ELF section, read from DUMP
		# Calculate the OFFSET in the FILE by subtracting RAM start address
		offset = addr - self.ram_base_addr

		# Also check whether address falls within the code section, if so read from elf
		if (have_ram_kernel_text):
			if (addr >= g_stext_flash and addr <= g_etext_flash) or (addr >= g_stext_ram and addr <= g_etext_ram):
				# Checking in ELF file once for the offset at which we need to read the address
				offset = (addr - g_stext_ram) + int(self.text_offset, 16)
				if debug:
					print(('address {0:x} is in text range'.format(addr)))
				self.elf_file_fd.seek(offset)
				a = self.elf_file_fd.read(length)
				return a
		elif not(have_ram_kernel_text):
			if (addr >= g_stext_flash and addr <= g_etext_flash):
				# Checking in ELF file once for the offset at which we need to read the address
				offset = (addr - g_stext_flash) + int(self.text_offset, 16)
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
	def read_string(self, address, format_string, debug=False):
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
	def read_word(self, address, debug=False):
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

	# Function to return the address of a Symbol from mapping table
	def get_address_of_symbol(self, symbol, debug=False):
		i = 0
		while (self.symbol_lookup_table[i][1] != symbol and i <= len(self.symbol_lookup_table)):
			i = i+1
		if (i > len(self.symbol_lookup_table)):
			return None
		else:
			if debug:
				print("Address of symbol {0:x} {1}".format(self.symbol_lookup_table[i][0], symbol))
			return self.symbol_lookup_table[i][0]

	# Function to search for a address from "Address to Symbol" mapping table
	# Apply Generic binary search
	def symbol_table_lookup(self, addr, symbol_size=0):
		if (addr is None):
			return ('(Invalid address)', 0x0)

		low = 0
		high = len(self.symbol_lookup_table)
		mid = (low + high) >> 1
		premid = 0
		# Check whether given address is within the 2 neighbour address range or not ?
		while(not(addr >= self.symbol_lookup_table[mid][0] and addr < self.symbol_lookup_table[mid + 1][0])):

			if(addr < self.symbol_lookup_table[mid][0]):
				high = mid - 1

			if(addr > self.symbol_lookup_table[mid][0]):
				low = mid + 1

			mid = (high + low) >> 1

			if(mid == premid):
				return None
			if(mid + 1) >= len(self.symbol_lookup_table) or mid < 0:
				return None

			premid = mid

		if symbol_size == 0:
			return (self.symbol_lookup_table[mid][1], addr - self.symbol_lookup_table[mid][0])
		else:
			return (self.symbol_lookup_table[mid][1], self.symbol_lookup_table[mid + 1][0] - self.symbol_lookup_table[mid][0])


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

			# Read the Symbol name and offset for a specific PC address from symbol table
			r = self.symbol_table_lookup(frame.pc)
			if r is None:
				symname = 'UNKNOWN'
				offset = 0x0
			else:
				symname, offset = r # Update both symbol name and offset

			address = '{0:x}'.format(frame.pc)
			cmd = ['arm-none-eabi-addr2line -e', self.elf, address]
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

def format_output(res, string):
     r = res.split('\n')
     print('\t- symbol addr {0}       : {1}'.format(string, r[0]))
     print('\t- function name {0}     : {1}'.format(string, r[1]))
     print('\t- file {0}              : {1}'.format(string, r[2]))

def print_crash_type(string):
    global crash_type_assert
    if 'up_memfault' in string:
        print('\n2. Crash type               : memory fault')
    elif 'up_busfault' in string:
        print('\n2. Crash type               : bus fault')
    elif 'up_usagefault' in string:
        print('\n2. Crash type               : usage fault')
    elif 'up_hardfault' in string:
        print('\n2. Crash type               : hard fault')
    elif 'Assertion failed at file' in string:
        crash_type_assert = True
        print('\n2. Crash type               : code assertion by code ASSERT or PANIC')
    else:
        print('\n2. Crash type               : etc')
    if (crash_type_assert == True):
        print('\n3. Crash point\n\t- ', string)
    else:
        print('   Crash log\n\t-', string)

# Function to get the number of application binaries, names, text address and sizes
def find_number_of_binaries(log_file):

    global g_app_idx
    global g_stext_app
    global g_etext_app

    # Parse the contents based on tokens in log file.
    with open(log_file) as searchfile:
        for line in searchfile:
            # Get the number of applications loaded
            if BIN_ADDR_FXN in line:
                g_app_idx = g_app_idx + 1

    app_idx = 0
    g_stext_app = array('i', range(0, g_app_idx))
    g_etext_app = array('i', range(0, g_app_idx))
    with open(log_file) as searchfile:
        for line in searchfile:
            # Read the app text address and size
            if BIN_ADDR_FXN in line:
               word = line.split(':')
               t = word[2].split(',') # word[2] is the App Start Text address
               w = word[1].split(' ')
               # w[1] denotes string '[<app_name>]'
               start_idx = int(w[1].find('[')) + 1
               end_idx = int(w[1].find(']'))
               app_name.append(w[1][start_idx:end_idx])
               g_stext_app[app_idx] = int(t[0], 16)
               g_etext_app[app_idx] = g_stext_app[app_idx] + int(word[3], 10) # word[3] is text_size
               app_idx = app_idx + 1


# Function to Parse the i/p log file in case of app crashes to corresponding app display debug symbols
def find_crash_point(log_file, elf):

    global g_app_idx
    global g_assertpc
    global g_stext_app
    global g_etext_app
    global crash_type_assert
    app_idx = 0
    pc_value = 0
    lr_value = 0
    is_app_crash = 0
    is_kernel_crash = 0
    is_interrupt_mode = 0
    assertline = ""

    # Parse the contents based on tokens in log file.
    with open(log_file) as searchfile:
        for line in searchfile:
            if 'up_registerdump:' in line:
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
            if 'up_assert: Assertion failed at file:' in line:
                assertline = line
            if 'Assert location (PC) :' in line:
                word = line.split(':')
                #word[2] contains the g_assertpc value
                g_assertpc = int(word[2].strip(),16)

    print('-----------------------------------------------------------------------------------------')
    address1 = hex(lr_value)
    address2 = hex(pc_value)
    result = 0

    # Check for lr & pc values in application text address range
    if (pc_value != 00000000):
        for app_idx in range(g_app_idx):
            os.system("nm --defined-only -l --numeric-sort " + BIN_PATH + app_name[app_idx] + "_dbg > " + BIN_PATH + app_name[app_idx] + ".map")
            if (address1 >= hex(g_stext_app[app_idx]) and address1 < hex(g_etext_app[app_idx])):
                addr = lr_value - int(hex(g_stext_app[app_idx]), 16)
                f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr))
                result = f.read()
                if '??' not in result and '$d' not in result:
                    is_app_crash = 1
                    print('\n1. Crash Binary           : {0}'.format(app_name[app_idx]))
                    print_crash_type(assertline)
                    if (crash_type_assert == False):
                        print('3. Crash point (PC or LR)')
                        print('\n\t[ Caller - return address (LR) - of the function which has caused the crash ]')
                        format_output(result, "")
            if (address2 >= hex(g_stext_app[app_idx]) and address2 < hex(g_etext_app[app_idx])):
                addr = pc_value - int(hex(g_stext_app[app_idx]), 16)
                f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr))
                result = f.read()
                if '??' not in result and '$d' not in result:
                    if (not is_app_crash):
                        print('\n1. Crash Binary           : {0}'.format(app_name[app_idx]))
                        print_crash_type(assertline)
                        if (crash_type_assert == False):
                            print('3. Crash point (PC or LR)')
                    is_app_crash = 1
                    if (crash_type_assert == False):
                        print('\n\t[ Current location (PC) of assert ]')
                        format_output(result, "")
                if ((addr - 4) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr - 4))
                    result1 = f.read()
                    if '??' not in result1 and '$d' not in result1:
                        if (crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            format_output(result1, "of (pc - 4)")
                if ((addr - 8) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr - 8))
                    result2 = f.read()
                    if '??' not in result2 and '$d' not in result2:
                        if (crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            format_output(result2, "of (pc - 8)")

    # Scenario when up_registerdump() is not present in dump logs, use g_assertpc value to give crash point
    else:
        address1 = hex(g_assertpc)
        for app_idx in range(g_app_idx):
            os.system("nm --defined-only -l --numeric-sort " + BIN_PATH + app_name[app_idx] + "_dbg > " + BIN_PATH + app_name[app_idx] + ".map")
            if (address1 >= hex(g_stext_app[app_idx]) and address1 < hex(g_etext_app[app_idx])):
                addr = g_assertpc - int(hex(g_stext_app[app_idx]), 16)
                f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr))
                result = f.read()
                if '??' not in result and '$d' not in result:
                    is_app_crash = 1
                    print('\n1. Crash Binary           : {0}'.format(app_name[app_idx]))
                    print_crash_type(assertline)
                    if (crash_type_assert == False):
                        print('3. Crash point (PC or LR)')
                        print('\n\t[ Current location (PC) of assert ]')
                        format_output(result, "")
                if ((addr - 4) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr - 4))
                    result1 = f.read()
                    if '??' not in result1 and '$d' not in result1:
                        if (crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            format_output(result1, "of (pc - 4)")
                if ((addr - 8) > 0x0):
                    f = os.popen('arm-none-eabi-addr2line -a -f -e ' + BIN_PATH + app_name[app_idx] + '_dbg ' + hex(addr - 8))
                    result2 = f.read()
                    if '??' not in result2 and '$d' not in result2:
                        if (crash_type_assert == False):
                            print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                            format_output(result2, "of (pc - 8)")

    # Check for lr & pc values in kernel text address range
    if (not is_app_crash) and (pc_value != 00000000):
        address1 = hex(lr_value)
        if (address1 >= hex(g_stext_flash) and address1 < hex(g_etext_flash)) or (address1 >= hex(g_stext_ram) and address1 < hex(g_etext_ram)):
            # If yes, print the crash point using addr2line
            f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(lr_value))
            result = f.read()
            if '??' not in result and '$d' not in result:
                is_kernel_crash = 1
                print('1. Crash Binary             : kernel')
                print_crash_type(assertline)
                if (crash_type_assert == False):
                    print('3. Crash point (PC or LR)')
                    print('\n\t[ Caller - return address (LR) - of the function which has caused the crash ]')
                    format_output(result, "")
        if (address2 >= hex(g_stext_flash) and address2 < hex(g_etext_flash)) or (address2 >= hex(g_stext_ram) and address2 < hex(g_etext_ram)):
            f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(pc_value))
            result = f.read()
            if '??' not in result and '$d' not in result:
                if (not is_kernel_crash):
                    print('1. Crash Binary             : kernel')
                    print_crash_type(assertline)
                    if (crash_type_assert == False):
                        print('3. Crash point (PC or LR)')
                is_kernel_crash = 1
                if (crash_type_assert == False):
                    print('\n\t[ Current location (PC) of assert ]')
                    format_output(result, "")
            if ((pc_value - 4) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(pc_value - 4))
                result1 = f.read()
                if '??' not in result1 and '$d' not in result1:
                    if (crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        format_output(result1, "of (pc - 4)")
            if ((pc_value - 8) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(pc_value - 8))
                result2 = f.read()
                if '??' not in result2 and '$d' not in result2:
                    if (crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        format_output(result2, "of (pc - 8)")

    # Scenario when up_registerdump() is not present in dump logs, use g_assertpc value to give crash point
    if (pc_value == 00000000 and g_assertpc):
        address1 = hex(g_assertpc)
        if (address1 >= hex(g_stext_flash) and address1 < hex(g_etext_flash)) or (address1 >= hex(g_stext_ram) and address1 < hex(g_etext_ram)):
            # If yes, print the crash point using addr2line
            f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(g_assertpc))
            result = f.read()
            if '??' not in result and '$d' not in result:
                is_kernel_crash = 1
                print('1. Crash Binary             : kernel')
                print_crash_type(assertline)
                if (crash_type_assert == False):
                    print('3. Crash point (PC or LR)')
                    print('\n\t[ Current location (PC) of assert ]')
                    format_output(result, "")
            if ((g_assertpc - 4) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(g_assertpc - 4))
                result1 = f.read()
                if '??' not in result1 and '$d' not in result1:
                    if (crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        format_output(result1, "of (pc - 4)")
            if ((g_assertpc - 8) > 0x0):
                f = os.popen('arm-none-eabi-addr2line -a -f -e' + elf + ' ' + hex(g_assertpc - 8))
                result2 = f.read()
                if '??' not in result2 and '$d' not in result2:
                    if (crash_type_assert == False):
                        print('\n\t[ Exact crash point might be -4 or -8 bytes from the PC ]')
                        format_output(result2, "of (pc - 8)")

    if (not is_app_crash) and (not is_kernel_crash):
        print('1. Crash Binary             : NA')
        print_crash_type(assertline)
        if (crash_type_assert == False):
            print('3. Crash point (PC or LR)')
            # Parse the contents based on tokens in log file.
            with open(log_file) as searchfile:
                for line in searchfile:
                    # If PC value is invalid, show invalid PC
                    if 'PC value might be invalid' in line:
                        print('\tPC value might be invalid.')
            print('\t-  PC & LR values not in any text range! No probable crash point detected.')

    with open(log_file) as searchfile:
        for line in searchfile:
            if 'ERROR: Stack pointer is not within any of the allocated stack' in line:
                word = line.split(':')
                print('\n\t-', word[1], ':', word[2])

    # Parse the contents based on tokens in log file to determine point of assertion in details
    with open(log_file) as searchfile:
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
    with open(log_file) as searchfile:
        for line in searchfile:
            # Print the interrupt data during crash (if crashed during interrupt context)
            if 'IRQ num:' in line:
                word = line.split(' ')
                # Last word[-1] contains the interrupt number
                irq_num = word[-1]
                print('\n\t- Interrupt number\t\t:',irq_num)
            if 'Code asserted in IRQ state!' in line:
                is_interrupt_mode = 1
                # It displays the interrupt handler information corresponding to the Interrupt
                # The last argument to debugsymbolviewer specifies whether or not to check for interrupt handler address (4)
                print("\n4. Assertion Data during interrupt mode:\n")
                print('- Interrupt handler at addr\t\tSymbol_name')
                os.system("python3 ../debug/debugsymbolviewer.py " + log_file + " " + str(g_app_idx) + " 4")
    with open(log_file) as searchfile:
        for line in searchfile:
            if 'Nested IRQ stack:' in line:
                is_interrupt_mode = 2
                print("- IRQ Stack information:\n")
            if (is_interrupt_mode >= 2 and is_interrupt_mode < 9):
                is_interrupt_mode = is_interrupt_mode + 1
                print("\033[F", line)

    if (is_interrupt_mode):
        print('\n5. Call stack of last run thread')
    else:
        print('\n4. Call stack of last run thread')

    # Parse the contents based on tokens in log file for memory allocation failure data
    with open(log_file) as searchfile:
        mm_fail = 0
        for line in searchfile:
            # Print the mm allocation failure data during crash (if crashed during mm allocation)
            if ('mm_manage_alloc_fail:' in line) and (mm_fail == 0):
                print('\nMemory allocation failure logs are as below:')
                mm_fail = 1
            if (mm_fail == 1):
                print(line)

    # Parse the contents based on tokens in log file.
    with open(log_file) as searchfile:
        for line in searchfile:
            # Print the current stack pointer value
            if 'up_dumpstate: sp:' in line:
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
                # The last argument to debugsymbolviewer specifies whether or not to check for current running work function (3)
                print('\nCurrent running work function\t\tFile_name')
                os.system("python3 ../debug/debugsymbolviewer.py " + log_file + " " + str(g_app_idx) + " 3")

    # It displays the debug symbols corresponding to all the addresses in the kernel and application text address range
    print('\nStack_address\t Symbol_address\t Symbol location  Symbol_name\t\tFile_name')
    os.system("python3 ../debug/debugsymbolviewer.py " + log_file + " " + str(g_app_idx) + " 0")

    print('\nx. Miscellaneous information:')

    with open(log_file) as searchfile:
    # Parse the contents based on tokens in log file for heap corruption
        heap_corr = 0
        for line in searchfile:
            # Print the heap corruption data (if any)
            if 'mm_check_heap_corruption:' in line:
                if (heap_corr == 0):
                    print("\n## Heap Corruption Data:\n")
                    heap_corr = 1
                print("\033[F", line)
            if 'dump_node:' in line:
                print("\033[F", line)
    with open(log_file) as searchfile:
        for line in searchfile:
            if 'allocated by code at addr' in line:
                print("## Code Location of possible corrupted node allocation:")
                # It displays the debug symbols corresponding to the corrupted heap node
                # The last argument to debugsymbolviewer specifies whether or not to check for corrupted heap node addresses (2)
                print('Allocated by code at addr\t\tFile_name')
                os.system("python3 ../debug/debugsymbolviewer.py " + log_file + " " + str(g_app_idx) + " 2")
                break

    # It displays the debug symbols corresponding to all the wrong sp addresses (if any)
    # The last argument to debugsymbolviewer specifies whether or not to check for wrong stack pointer addresses (1)
    os.system("python3 ../debug/debugsymbolviewer.py " + log_file + " " + str(g_app_idx) + " 1")
    print('-----------------------------------------------------------------------------------------')

# Function to format logs and delete the timestamp (format-[xxxxxxxxx]) if it consists of timestamp at the start of each log line
def format_log_file(log_file):

	# Delete unwanted logs (if any) and timestamp at the start of each line
	with open(log_file, "r") as f:
		data = f.readlines()
	with open(log_file, "w") as f:
		assertinlog = False #Truncate logs only if assert has occured. For other type of crashes, no need to truncate
		trunc = True # False if log line is to be retained, True otherwise
		for line in data:
			if 'up_assert: ' in line:
				assertinlog = True
			if assertinlog:
				# Truncate logs after first crash dump: up_assert
				if ('up_assert: Assertion failed at file:' in line) and (trunc == False):
					break
				# Truncate logs before first crash dump: up_assert
				if 'up_assert: Assertion failed at file:' in line:
					trunc = False
				if trunc:
					# Do not write line and move to the next line
					continue

			delete_idx = 0
			# Timestamp present if line starts with '['
			if line[0] == '[':
				for idx in range(0, len(line)):
					if ']' == line[idx]:
						delete_idx = idx + 1
						break
			if line[delete_idx] == ' ':	# Check for trailing white spaces
				delete_idx = delete_idx + 1
			line = line[delete_idx:]
			f.write(line)

	# Check for invalid format after above formatting
	with open(log_file, "r") as f:
		data = f.readlines()
		for line in data:
			if 'up_assert:' in line:
				if line[0] != 'u':
					for idx in range(0, len(line)):
						if 'u' == line[idx]:
							delete_idx = idx
							break
					correctFormatString = line[delete_idx:]
					print ("\n\t- Below log format is not supported in TRAP")
					print ('\t\t-{0}\t- Instead, supported log format in TRAP is as follows:'.format(line))
					print ("\t\t-{0}\n\tKindly modify the log file as per accepted format.\n".format(correctFormatString))
					sys.exit(1)

def usage():
	print('*************************************************************')
	print('\nUsage: %s -r DUMP_FILE [OPTIONS]' % sys.argv[0])
	print('                  OR')
	print('Usage: %s -t LOG_FILE [OPTIONS]\n' % sys.argv[0])
	print('Following options are available')
	print('\t-r, --dump_file                 RAM/FLASH dump_file along with path')
	print('\t-t, --log_file                  Enter Logfile which contains stackdump during assert')
	print('\t-b, --bin_path                  Enter binary folder path other than default binary')
	print('\t-c, --config_Path               Enter configuration file path for above binary')
	print('\t-e, --elf_path                  Enter kernel elf file name')
	print('\t-h, --help                      Show help')
	print('')
	print('syntax :')
	print('--------')
	print('python3 %s -r Filename_ramBaseAddr_ramEndAddr.bin' % sys.argv[0])
	print('Below example if you give dump file as path: ')
	print('--------------------------------------------')
	print('python3 ramdumpParser.py -r build/output/bin/ramdump_0x4a0000_0x6a0000.bin')
	print('')
	print('Below example if you give simple assert log file as path: ')
	print('---------------------------------------------------------')
	print('python3 ramdumpParser.py -r log.txt ')
	print('')
	print('Below example if you give binary file other than default as path: ')
	print('---------------------------------------------------------')
	print('python3 ramdumpParser.py -r log.txt -b ../../os/vidisha/bin/ -c ../../os/cfile -e tinayara.axf')
	print('')
	print('')
	print('Note:')
	print('--------')
	print('1) For getting call stack CONFIG_FRAME_POINTER flag should be enabled in your build')
	print('')
	print('python3 ramdumpParser.py -r /build/bin/ramdump_0x4a0000_0x6a0000.bin')
	print('')
	print('')
	print('*************************************************************')

	sys.exit(1)

def main():
	global BIN_PATH
	global CONFIG_PATH
	dump_file = None
	log_file = None
	elf = None
	elf_path = None
	framePointer = 0
	stackPointer = 0
	programCounter = 0
	stackSize = 0
	have_ram_kernel_text = False

	try:
            opts, args = GetOpt(sys.argv[1:],'r:t:b:c:e:h', ['dump_file=','log_file=','bin_path=','config_path=','elf_path=','help'])
	except GetoptError as e:
		print(' ')
		print(' ')
		print('*************************************************************')
		print('Usage error:', e)
		usage()

	for opt, arg in opts:
		if opt in ('-r', '--dump_file'):
			dump_file = arg
		elif opt in ('-t', '--log_file'):
			log_file = arg
		elif opt in ('-b', '--bin_path'):
			bin_path = arg
			BIN_PATH = bin_path
		elif opt in ('-c', '--config_path'):
			config_path = arg
			CONFIG_PATH = config_path
		elif opt in ('-e', '--elf_ext'):
			elf_path = arg
		elif opt in ('-h', '--help'):
			usage()

	if not elf_path:
		# Read tinyara extension from Make.defs
		with open(MAKEFILE_PATH, 'r') as fd:
			lines = fd.readlines()
			for line in lines:
				if 'EXEEXT =' in line:
					ext = (line.split("=")[1].strip())
		elf = (BIN_PATH + 'tinyara' + ext)
	else:
		elf = BIN_PATH + elf_path

	if not log_file and not dump_file:
		print('Usage error: Must specify one of the -t or -e options. Plz find below for proper usage')
		usage()

	exist_check_list = [
		("dump_file", dump_file),
		("log_file", log_file),
		("elf", elf)
	]

	for name, path in exist_check_list:
		if path and not os.path.exists(path):
			print(f"{path} does not exist. Please provide the proper path for {name}...")
			sys.exit(1)

	# Format log file if timestamp is present at the start of each line
	format_log_file(log_file)

	# Get the number of application binaries, names, text address and sizes
	find_number_of_binaries(log_file)

	print('')
	print('')
	print('*************************************************************')
	print('dump_file                   :', dump_file)
	print('log_file                    :', log_file)
	if (g_app_idx > 0):
		print('Number of binaries          : %d [kernel + apps]' % (g_app_idx + 1))
	else:
		print('Number of binary            : 1 [kernel]')
	print('"kernel" elf_file           :', elf)
	for app_idx in range(g_app_idx):
		print('"app%d" elf_file             :' % (app_idx + 1), BIN_PATH + app_name[app_idx] + '_dbg ')
	print('*************************************************************')
	print('')

	# Read config information
	fd = open(CONFIG_PATH, 'r')
	data = fd.read()
	fd.close()

	try:
		if 'CONFIG_ARCH_HAVE_RAM_KERNEL_TEXT=y' in data:
			have_ram_kernel_text = True

		# Calling the Constructor with the initial set of arguments
		rParser = dumpParser(dump_file=dump_file,elf=elf,log_file=log_file, debug=False)


		# Setup the Symbol table lookup with given System.map file
		rParser.setup_symbol_table(elf,debug=False)

		# Find offset
		global g_stext_flash
		g_stext_flash = rParser.get_address_of_symbol("_stext_flash")
		global g_etext_flash
		g_etext_flash = rParser.get_address_of_symbol("_etext_flash")
		if (have_ram_kernel_text):
			global g_stext_ram
			g_stext_ram = rParser.get_address_of_symbol("_stext_ram")
			global g_etext_ram
			g_etext_ram = rParser.get_address_of_symbol("_etext_ram")

		# Find the point of crash in the kernel, application or common binaries
		find_crash_point(log_file, elf)

		# Get ARM arch family
		if ('CONFIG_ARCH_FAMILY="armv8-m"' in data) or ('CONFIG_ARCH_FAMILY="armv7-m"' in data):
			#If architecture is cortex M, then run return without further script execution
			return None

		if not 'CONFIG_DEBUG_MM_HEAPINFO=y' in data:
			print('DEBUG_MM_HEAPINFO is not enable. Enable DEBUG_MM_HEAPINFO to see heap usage')
			return

		# If the log file is given, then parse that log file only and exit
		if log_file is not None:
			rParser.parse_file(log_file,debug=False)
			return None

		# If the log file not given, Consider full dump


		# Unwind the Callstack by obtaining FP, SP and PC from the proper global variable
		# task_ctxt_regs is used if SEC_DEBUG is enabled , else current_regs is used
		current_regs_pointer = rParser.get_address_of_symbol("current_regs")
		if current_regs_pointer is not None:
			ctxt_regs = rParser.read_word(current_regs_pointer)
			if ctxt_regs != 0x0:
				print('current_regs is not NULL')
			else:
				cpu_ctxt_regs_pointer = rParser.get_address_of_symbol("cpu_ctxt_regs")
				if cpu_ctxt_regs_pointer is not None:
					ctxt_regs = cpu_ctxt_regs_pointer

		if ctxt_regs is None:
			print('System is not crashed')
			return None

		framePointer = rParser.read_word(ctxt_regs+(4* FP))
		stackPointer = rParser.read_word(ctxt_regs+(4* SP))
		linkRegister = rParser.read_word(ctxt_regs+(4*LR))
		programCounter = rParser.read_word(ctxt_regs+(4*PC))
		# There are spl case where PC can be invalid, So assigning LR to PC
		if (not(programCounter >= g_stext_flash and programCounter <= g_etext_flash)):
			if (have_ram_kernel_text):
				if (not(programCounter >= g_stext_ram and programCounter <= g_etext_ram)):
					# This is possible for a prefetch abort. so am taking care by assigning LR to PC
					print("It's a Prefetch abort at Addr : ", hex(programCounter))
					programCounter = linkRegister
			else:
				# This is possible for a prefetch abort. so am taking care by assigning LR to PC
				print("It's a Prefetch abort at Addr : ", hex(programCounter))
				programCounter = linkRegister


		# Explicitly getting the PC and LR symbol names for display purpose
		temp1 = rParser.symbol_table_lookup(programCounter)
		if temp1 is None:
			temp1_symname = 'UNKNOWN'
			temp1_offset = 0x0
		else:
			temp1_symname, temp1_offset = temp1

		temp2 = rParser.symbol_table_lookup(linkRegister)
		if temp2 is None:
			temp2_symname = 'UNKNOWN'
			temp2_offset = 0x0
		else:
			temp2_symname, temp2_offset = temp2

		# Reading the Current Task from g_readytorun list, usually head is the current
		# Since its a pointer, first am getting pointer address and then reading the
		# contents of that Pointer to get the proper task address
		current_task_pointer = rParser.get_address_of_symbol('g_readytorun')
		current_task_addr = rParser.read_word(current_task_pointer)
		# Reading the stack size value from tbe tcb_s struct
		stackSize = rParser.read_word(current_task_addr+0x2c)

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

		rParser.unwind_backtrace_with_framepointer(framePointer,stackPointer,linkRegister,programCounter,stackSize)
		print('')
		print('')
		print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&')


		g_kmmheap = rParser.get_address_of_symbol("g_kmmheap")

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

		if 'CONFIG_MM_SMALL=y' in data:
			MM_ALLOC_BIT = 0x8000
		else:
			MM_ALLOC_BIT = 0x80000000

		start_heap = rParser.read_word(g_kmmheap + HEAP_START_POINT)
		end_heap = rParser.read_word(g_kmmheap + HEAP_START_POINT + 4)

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
			size = rParser.read_word(point)
			preceding = rParser.read_word(point + 4)
			owner = rParser.read_word(point + 8)
			pid = rParser.read_halfword(point + 12)
			if preceding & MM_ALLOC_BIT :
				fd_popen = subprocess.Popen(['arm-none-eabi-addr2line -e', elf, hex(owner)], stdout=subprocess.PIPE).stdout
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

		heap_size = rParser.read_word(g_kmmheap + HEAP_SIZE_POINT)
		print('HEAP SIZE        : ', heap_size)

		peack_alloc_size = rParser.read_word(g_kmmheap + PEAK_ALLOC_SIZE_POINT)
		print('PEAK ALLOC SIZE  : ', peack_alloc_size)

		total_alloc_size = rParser.read_word(g_kmmheap + TOTAL_ALLOC_SIZE_POINT)
		print('TOTAL ALLOC SIZE : ', total_alloc_size)
		print('FREE SIZE        : ', heap_size - total_alloc_size)
		print('')
		print('***********************************************************')
		print('  PID  | STACK SIZE |  CUR ALLOC SIZE   | PEAK ALLOC SIZE |')
		print('-------|------------|-------------------|-----------------|')
		INVALID_PROCESS_ID = 0xFFFFFFFF

		alloc_list = ALLOC_LIST_POINT + g_kmmheap
		for i in range(0, max_tasks):
			pid = rParser.read_word(alloc_list)
			if pid != INVALID_PROCESS_ID :
				# This information depends on the heapinfo_tcb_info_t
				cur_alloc = rParser.read_word(alloc_list + 4)
				peak_alloc = rParser.read_word(alloc_list + 8)
				print('{:^7}|'.format(pid), '{:>7}'.format(stack_size[i]), '   |', '{:>13}'.format(cur_alloc), '    |', '{:>13}'.format(peak_alloc), '  |')
			# next alloc list
			alloc_list += SIZE_OF_HEAPINFO_TCB_INFO

	except Exception as e:
		print("ERROR:", e)


if __name__ == '__main__':
	main()
