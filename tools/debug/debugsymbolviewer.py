###########################################################################
#
# Copyright 2021 Samsung Electronics All Rights Reserved.
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

from __future__ import print_function
from math import log
import re

systemmap_path = '../../build/output/bin/System.map'		# System map file
log_file = '../dump_tool/log_file'				# Crash log file
symbol_lookup_table = []	# Lookup table created from contents of system map file
debug = False			# Variable to print debug logs
pc_value = 0x00000000		# Program counter value

# Kernel text start & end addresses
ram_text_start_addr = 0x00000000
ram_text_end_addr = 0x00000000
flash_text_start_addr = 0x00000000
flash_text_end_addr = 0x00000000

def bytes_needed(n):
    if n == 0:
        return 1
    return int(log(n, 256)) + 1

# Function to setup the Address to Symbol mapping table from the System Map file
def setup_symbol_table(sm_file):
	global flash_text_start_addr
	global flash_text_end_addr
	global ram_text_start_addr
	global ram_text_end_addr
	is_ram_text = False
	# Reading the System Map file and preparing the symbol map table
	with open(sm_file) as searchfile:
		for line in searchfile:
			s = line.split(' ', 2)
			if len(s) == 3:
				if not '$' in s[2]:
					# s[0] contains address and s[2] contains Symbol name
					symbol_lookup_table.append((int(s[0], 16), s[2].rstrip()))

					# Extract kernel text start & end addresses from the table
					symbol = s[2].rstrip('\n')
					if symbol == '_stext_flash':
						flash_text_start_addr = hex(int(s[0], 16))
					if symbol == '_etext_flash':
						flash_text_end_addr= hex(int(s[0], 16))
					if symbol == '_stext_ram':
						is_ram_text = True
						ram_text_start_addr = hex(int(s[0], 16))
					if symbol == '_etext_ram':
						ram_text_end_addr = hex(int(s[0], 16))

	if debug:
		# Printing the Created Symbol table
		print('~~~~~~~~~~~~~~~~~~~~~~~~ SYMBOL TABLE START ~~~~~~~~~~~~~~~~~~~~~')
		for line in symbol_lookup_table:
			print("{0:x} {1}".format(line[0], line[1]))
		print('~~~~~~~~~~~~~~~~~~~~~~~~SYMBOL TABLE END   ~~~~~~~~~~~~~~~~~~~~~')

		# Printing the kernel text address ranges in ram and flash areas
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~  KERNEL TEXT RANGE ~~~~~~~~~~~~~~~~~~~~~~~')
		print('flash_text_start_addr\t=\t', flash_text_start_addr)
		print('flash_text_end_addr\t=\t', flash_text_end_addr)
		if (is_ram_text):
			print('ram_text_start_addr\t=\t', ram_text_start_addr)
			print('ram_text_end_addr\t=\t', ram_text_end_addr)
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
		print('')


# Function to search the search_addr present in the System map file and print its corresponding symbol
def print_symbol(search_addr):
	addr = 0x00000000
	next_addr = 0x00000000
	l = 0
	h = len(symbol_lookup_table)
	mid = (l + h) / 2

	while (l <= h):
		addr = symbol_lookup_table[mid - 1][0]
		next_addr = symbol_lookup_table[mid][0]

		if (search_addr >= addr and search_addr <= next_addr):
			print("{0}\t {1} \t  {2}".format(hex(search_addr), hex(addr), symbol_lookup_table[mid - 1][1]))
			break

		if (search_addr < addr):
			h = mid - 1
		else:
			l = mid + 1

		mid = (l + h) / 2

	if (l > h):
		if debug:
			print('Symbol not found for address: {0} in system map file'.format(hex(search_addr)))


# Function to check if address lies in the kernel text address range
def is_kernel_text_address(address):

	# Check the address
	if (address >= ram_text_start_addr and address < ram_text_end_addr) or (address >= flash_text_start_addr and address < flash_text_end_addr):
		return True
	else:
		return False


# Function to Parse the i/p log file (which contains stackdump during assert)
def parse_file(log_file):
	stack_addr = 0x00000000
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
					print('\nPC_value\t Symbol_address\t  Symbol_name\tFile_name')
					print_symbol(pc_value)
				continue

			# Read the stack contents of aborted stack and check for valid addresses
			if 'up_stackdump:' in line:
				word = line.split(':')
				t = word[2].split( )
				for sub_word in t:
					# Check for valid address, else move to the next sub_word
					if (bytes_needed(int(sub_word,16)) != 4):
						continue

					stack_addr = int(sub_word,16)
					# Check if the stack address lies in kernel text address range
					if (is_kernel_text_address(hex(stack_addr))):
						#If yes, print it's corresponding symbol
						print_symbol(stack_addr)


#Execution starts here
if (__name__ == '__main__'):
    # init

    setup_symbol_table(systemmap_path)

    print('----------------------------------  DEBUG DISPLAY SYMBOLS -------------------------------')
    print('Dump_address\t Symbol_address\t  Symbol_name\tFile_name')
    parse_file(log_file)
    print('-----------------------------------------------------------------------------------------')

