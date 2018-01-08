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
# File : dumpParce.py
# Description:
# Parser to parse the dump/log file using elf file and print CALL STACK.
# Gdb and NM tools are used to read the symbols

import re
import os
import string
import sys, time
import struct
import subprocess
from getopt import GetoptError, getopt as GetOpt

# Global variables
FP = 11
SP = 13
LR = 14
PC = 15
g_stext=0
g_etext=0

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
	def __init__(self, dump_file=None, elf=None, gdb_path=None, nm_path=None, readelf_path=None, log_file=None, debug=False):
		self.dump_file=dump_file		# dump file
		self.elf=elf				# Elf file
		self.gdb_path=gdb_path			# Path of GDB tool
		self.nm_path=nm_path			# Path of NM tool
		self.readelf_path=readelf_path		# Path of readelf tool
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
				print('Failed to open {0}'.format(elf))
				return None

		# Check for ramdump file existence, if exists open the dump file in binary format
		if self.dump_file is not None:
			self.dump_file_fd = open(dump_file, 'rb')
			if not self.dump_file_fd:
				print('Failed to open {0}'.format(dump_file))
				return None

			#Split the dump_file with delimiters _ and . to read the start and end addr
			# This is required to support any RAM dump size of any board
			temp = re.split(r'[_.]',dump_file)
			# Get the Start address and end address from dump file name
			self.ram_base_addr = int(temp[-3],16)
			self.ram_end_addr = int(temp[-2],16)
			print('self.ram_base_addr {0:x}'.format(self.ram_base_addr))
			print('self.ram_end_addr {0:x}'.format(self.ram_end_addr))

		# Read the elf header to get the offset of text section and ARM exidx section
		# These offsets will be used while creating ARM exidx table as well as while reading
		# the address from ELF file at a particular text address
		with os.popen(self.readelf_path + ' -S ' + elf) as readelf_fd:
			elfdata = readelf_fd.readlines()
			for line in elfdata:
				if '.text' in line:
					word = line.split()
					#word 5 contains the offset value
					self.text_offset = word[5]
					if debug:
						print line
						print self.text_offset

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

		print "Sorry. Given address does not present in stack lookup"
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
						print "stackSize :", self.stacksize
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
							print "{0:x}   {1:x}".format(self.stack_table[i][0],self.stack_table[i][1])
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
							print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'
							print ' '
							print 'stack size : ', self.stacksize
							print 'PC : ', hex(pc)
							print 'LR : ', hex(lr)
							print 'SP : ', hex(sp)
							print 'FP : ', hex(fp)
							print ' '
							print ' '

						# If the PC is not withing RAM range, it's a Prefetch issue
						# So, fill the PC with LR and help to get the call stack
						if ( pc < g_stext or pc > g_etext):
							print "It'S A PRE-FETCH ABORT @ PC", hex(pc)
							# Assign LR to PC to help constructing the stack
							pc = lr

					continue
				# Incase if log file already has this data, address to symbol mapping is
				# enough to get the call stack.
				if 'unwind_backtrace_with_fp:' in line:
					word = line.split(':')
					if word[1] == ' CallTrace_Start':
						got = 1
						print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'
						print 'Call Trace:'
						print ''
						continue
					if word[1] == ' CallTrace_End':
						got = 0
						print ''
						print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'
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
		print 'CALL STACK of Aborted task: '
		print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'
		print ''
		# Since Fp is enabled, use simple approach to get backtrace
		self.unwind_backtrace_with_framepointer(fp,sp,lr,pc,self.stacksize)
		print ''
		print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'


	# Function to setup the Address to Symbol mapping table from the ELF file ( tinyara in our case)
	def setup_symbol_table(self,tinyara_elf_file, debug=False):
		# Reading the tinyara elf and preparing the symbol map table
		with os.popen(self.nm_path + ' -n ' + tinyara_elf_file) as elf_file_fd_nm:
			symbols = elf_file_fd_nm.readlines()
			for line in symbols:
				s = line.split(' ')
				if len(s) == 3:
					if not '$' in s[2]:
						# s[0] contains address and s[2] contains Symbol name
						self.symbol_lookup_table.append((int(s[0], 16), s[2].rstrip()))

			# Printing the Created Symbol table
			if debug:
				print '~~~~~~~~~~~~~~~~~~~~~~~~ SYMBOL TABLE START ~~~~~~~~~~~~~~~~~~~~~'
				for line in self.symbol_lookup_table:
					print "{0:x} {1}".format(line[0], line[1])
				print '~~~~~~~~~~~~~~~~~~~~~~~~ SYMBOL TABLE END   ~~~~~~~~~~~~~~~~~~~~~'

	# Function to read the contents of given length from specific RAM/ELF address
	def read_address(self, addr, length, debug=False):
		# First check whether address falls within the code section, if so read from elf
		if (addr >= g_stext and addr <= g_etext):
			if debug:
				print('address {0:x} is in text range'.format(addr))
			# Checking in ELF file once for the offset at which we need to read the address
			offset = (addr - g_stext ) + int(self.text_offset, 16)
			if debug:
				print('Offset = {0:x}'.format(offset))
				print('Length = {0:x}'.format(length))
			self.elf_file_fd.seek(offset)
			a = self.elf_file_fd.read(length)
			return a
		# Since the given address does not belong to ELF section, read from DUMP
		else:
			# Calculate the OFFSET in the FILE by subtracting RAM start address
			offset = addr - self.ram_base_addr

		if debug:
			print('offset = {0:x}'.format(offset))
			print('length = {0:x}'.format(length))

		# If dump file is Passed, Read contents of address from DUMP file descriptor
		if self.log_file is None:
			self.dump_file_fd.seek(offset)
			a = self.dump_file_fd.read(length)
		else:
			print 'AM HERE, SOMETHING WRONG'
			# If only Log file is passed as i/p, Read the contents of address from ELF file
			self.elf_file_fd.seek(offset+self.text_offset) # offset got from elf header
			a = self.elf_file_fd.read(length)


	        return a

	# returns a tuple of the result by reading address from the "specified format string"
	# return None on failure
	def read_string(self, address, format_string, debug=False):
		addr = address

		s = self.read_address(addr, struct.calcsize(format_string), debug)
		if (s is None) or (s == ''):
			if debug and addr is not None:
				print('Failed to read address {0:x}'.format(addr))
			return None

		# Unpack the string with proper format and return to calle.
		return struct.unpack(format_string, s)

	# returns a word size (4 bytes = 32 bits) read from dump "<" means little endian format
	# I indicates word
	def read_word(self, address, debug=False):
		if debug:
			print('reading {0:x}'.format(address))
		s = self.read_string(address, '<I', debug)
		if s is None:
			if debug:
				print 'read_word s is None'
			return None
		else:
			return s[0]
	# returns a single Byte read from given dump address "<" means little endian format
	# H indicates Half word
	def read_halfword(self, address, debug=False):
		if debug:
			print('reading {0:x}'.format(address))
		s = self.read_string(address, '<H', debug)
		if s is None:
			if debug:
				print 'read_halfword s is None'
			return None
		else:
			return s[0]

	# returns a single Byte read from given dump address "<" means little endian format
	# B indicates Byte, It's useful while reading a String from dump
	def read_byte(self, address, debug=False):
		if debug:
			print('reading {0:x}'.format(address))
		s = self.read_string(address, '<B', debug)
		if s is None:
			if debug:
				print 'read_byte s is None'
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
				print "Address of symbol {0:x} {1}".format(self.symbol_lookup_table[i][0], symbol)
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
			print "fp:{0:x}  sp low:{1:x} high:{2:x}".format(frame.fp, frame.sp, high)

		# Check current frame pointer is within bounds of Stack*/
		if (fp < (low + 12) or fp + 4 >= high):
			print "End of stack frame"
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

			pstring = (extra_str + '[<{0:x}>] {1}+0x{2:x}'.format(frame.pc, symname, offset))

			if output_file:
				out_file.write(pstring + '\n')
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

def usage():
	print '*************************************************************'
	print '\nUsage: %s -e ELF_FILE -r DUMP_FILE [OPTIONS]' % sys.argv[0]
	print '                  OR'
	print 'Usage: %s -e ELF_FILE -t LOG_FILE [OPTIONS]\n' % sys.argv[0]
	print 'Following options are available'
	print '\t-e, --elf                       tinyara elf file along with path'
	print '\t-r, --dump_file                 RAM/FLASH dump_file along with path'
	print '\t-t, --log_file                  Enter Logfile which contains stackdump during assert'
	print '\t-G, --gdb_path                  Enter gdb tool path'
	print '\t-N, --nm_path                   Enter nm tool path'
	print '\t-E, --readelf_path              Enter readelf tool path'
	print '\t-h, --help                      Show help'
	print ''
	print 'syntax :'
	print '--------'
	print 'python %s -e <Tinyara elf path> -r Filename_ramBaseAddr_ramEndAddr.bin -G <Gdb path> -N < NM path> ' % sys.argv[0]
	print ''
	print 'I assume, gdb and nm tool exist in your linux machine like /usr/bin/gdb and /usr/bin/nm, so hard coded this path inside script'
	print ''
	print 'Below example if you give dump file as path: '
	print '--------------------------------------------'
	print 'python dumpParser.py -e build/output/bin/tinyara -r build/output/bin/ramdump_0x4a0000_0x6a0000.bin'
	print ''
	print 'Below example if you give simple assert log file as path: '
	print '---------------------------------------------------------'
	print 'python dumpParser.py -e build/output/bin/tinyara -r log.txt '
	print ''
	print ''
	print 'Note:'
	print '--------'
	print '1) For getting call stack CONFIG_FRAME_POINTER flag should be enabled in your build'
	print ''
	print 'If you do not have gdb and nm path set, please pass the path as below'
	print ''
	print 'python dumpParser.py -r /build/bin/ramdump_0x4a0000_0x6a0000.bin -e build/bin/tinyara -G <your_gdb_path> -N <your_nm_path>'
	print ''
	print ''
	print '*************************************************************'

	sys.exit(1)

def main():
	dump_file = None
	log_file = None
	elf = None
	framePointer = 0
	stackPointer = 0
	programCounter = 0
	stackSize = 0
	gdb_path='/usr/bin/gdb'
	nm_path='/usr/bin/nm'
	readelf_path='/usr/bin/readelf'

	try:
		opts, args = GetOpt(sys.argv[1:],'r:e:G:N:d:t:g:h', ['dump_file=', 'elf=','gdb_path=','nm_path=','readelf_path=','log_file=','help'])
	except GetoptError, e:
		print ' '
		print ' '
		print '*************************************************************'
		print 'Usage error:', e
		usage()

	for opt, arg in opts:
		if opt in ('-r', '--dump_file'):
			dump_file = arg
		elif opt in ('-e', '--elf'):
			elf = arg
		elif opt in ('-G', '--gdb_path'):
			gdb_path = arg
		elif opt in ('-N', '--nm_path'):
			nm_path = arg
		elif opt in ('-E', '--readelf_path'):
			readelf_path = arg
		elif opt in ('-t', '--log_file'):
			log_file = arg
		elif opt in ('-h', '--help'):
			usage()

	print ''
	print ''
	print '*************************************************************'
	print 'dump_file         :', dump_file
	print 'log_file          :', log_file
	print 'Tinyara_elf_file  :', elf
	print '*************************************************************'
	print ''


	if not elf :
		print 'Usage error: Must specify -e option, please find below for proper usage'
		usage()

	if log_file is not None:
		if not os.path.exists(log_file):
			print ('{0} does not exist. Please provide the proper path for log_file...'.format(log_file))
			sys.exit(1)

	if dump_file is not None:
		if not os.path.exists(dump_file):
			print ('{0} does not exist. Plz provide proper path for dump_file...'.format(dump_file))
			sys.exit(1)

	if not log_file and not dump_file:
		print 'Usage error: Must specify one of the -t or -e options. Plz find below for proper usage'
		usage()

	if not os.path.exists(elf):
		print ('{0} does not exist. Cannot proceed without System.map Exiting...'.format(elf))
		sys.exit(1)

	if not os.path.exists(gdb_path):
		print ('{0} does not exist. Cannot proceed without GDB Tool Exiting...'.format(gdb_path))
		sys.exit(1)

	if not os.access(gdb_path, os.X_OK):
		print("!!! No execute permissions on gdb path {0}".format(gdb_path))
		print("!!! Please check the path settings")
		print("!!! If this tool is being run from a shared location, contact the maintainer")
		sys.exit(1)

	if not os.path.exists(nm_path):
		print ('{0} does not exist. Cannot proceed without NM Tool Exiting...'.format(nm_path))
		sys.exit(1)

	if not os.access(nm_path, os.X_OK):
		print("!!! No execute permissions on gdb path {0}".format(nm_path))
		print("!!! Please check the path settings")
		print("!!! If this tool is being run from a shared location, contact the maintainer")
		sys.exit(1)

	if not os.path.exists(readelf_path):
		print ('{0} does not exist. Cannot proceed without readelf Tool Exiting...'.format(readelf_path))
		sys.exit(1)

	if not os.access(readelf_path, os.X_OK):
		print("!!! No execute permissions on readelf path {0}".format(readelf_path))
		print("!!! Please check the path settings")
		print("!!! If this tool is being run from a shared location, contact the maintainer")
		sys.exit(1)


	try:
		# Calling the Constructor with the initial set of arguments
		rParser = dumpParser(dump_file=dump_file,elf=elf,gdb_path=gdb_path,nm_path=nm_path,readelf_path=readelf_path,log_file=log_file, debug=False)

		# Setup the Symbol table lookup with given System.map file
		rParser.setup_symbol_table(elf,debug=False)

		# Find offset
		global g_stext
		g_stext = rParser.get_address_of_symbol("_stext")
		global g_etext
		g_etext = rParser.get_address_of_symbol("_etext")

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
				print 'current_regs is not NULL'
			else:
				cpu_ctxt_regs_pointer = rParser.get_address_of_symbol("cpu_ctxt_regs")
				if cpu_ctxt_regs_pointer is not None:
					ctxt_regs = cpu_ctxt_regs_pointer

		if ctxt_regs is None:
			print 'System is not crashed'
			return None

		framePointer = rParser.read_word(ctxt_regs+(4* FP))
		stackPointer = rParser.read_word(ctxt_regs+(4* SP))
		linkRegister = rParser.read_word(ctxt_regs+(4*LR))
		programCounter = rParser.read_word(ctxt_regs+(4*PC))
		# There are spl case where PC can be invalid, So assigning LR to PC
		if ( programCounter < g_stext or programCounter > g_etext):
			# This is possible for a prefetch abort. so am taking care by assigning LR to PC
			print "It's a Prefetch abort at Addr : ", hex(programCounter)
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

		print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'
		print ''
		print "Board Crashed at :"
		print "PC: [0x{0:x}] {1}+0x{2:x}]\"".format(programCounter,temp1_symname,temp1_offset)
		print "LR: [0x{0:x}] {1}+0x{2:x}]\"".format(linkRegister,temp2_symname,temp2_offset)
		print ' '
		print "FP: 0x{0:x} and SP: 0x{1:x}".format(framePointer, stackPointer)
		print ' '
		print 'CALL STACK of Aborted task: '
		print '*******************************************************************'

		rParser.unwind_backtrace_with_framepointer(framePointer,stackPointer,linkRegister,programCounter,stackSize)
		print ''
		print ''
		print '&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&'


	except Exception, e:
		print "ERROR:", e


if __name__ == '__main__':
	main()
