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
from array import *
import os
import sys

bin_path = '../../build/output/bin/'	# Output bin path
ksymbol_lookup_table = []		# Lookup table created from contents of system map file for kernel symbols
asymbol_lookup_table = [[]] * 10	# Lookup table created from contents of appX map files for application symbols
debug = False				# Variable to print debug logs
pc_value = 0x00000000			# Program counter value
format_print = True

# Kernel text start & end addresses
ram_text_start_addr = 0x00000000
ram_text_end_addr = 0x00000000
flash_text_start_addr = 0x00000000
flash_text_end_addr = 0x00000000
g_stext_app = [0] * 10
g_etext_app = [0] * 10
app_name = []

def bytes_needed(n):
	if n == 0:
		return 1
	return int(log(n, 256)) + 1

# Function to get the application binary names, text address and sizes
def find_app_text_range(log_file):
	global g_stext_app
	global g_etext_app
	g_stext_app = array('i', range(0, app_idx))
	g_etext_app = array('i', range(0, app_idx))

	idx = 0
	with open(log_file) as searchfile:
		for line in searchfile:
			# Read the app text address and size
			if 'elf_show_all_bin_addr:' in line:
				word = line.split(':')
				t = word[2].split(',') # word[2] is the App Start Text address
				w = word[1].split(' ')
				# w[1] denotes string '[<app_name>]'
				start_idx = int(w[1].find('[')) + 1
				end_idx = int(w[1].find(']'))
				app_name.append(w[1][start_idx:end_idx])
				g_stext_app[idx] = int(t[0], 16)
				g_etext_app[idx] = g_stext_app[idx] + int(word[3], 10) # word[3] is text_size
				idx = idx + 1

	if debug:
		# Printing the kernel text address ranges in ram and flash areas
		for idx in range(app_idx):
			print('~~~~~~~~~~~~~~~~~~~ APPLICATION TEXT ADDRESS RANGE ~~~~~~~~~~~~~~~~~~~~')
			print('g_stext_app[{}]\t=\t{:8}'.format(idx, hex(g_stext_app[idx])))
			print('g_etext_app[{}]\t=\t{:8}'.format(idx, hex(g_etext_app[idx])))
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
		print('')

# Function to setup the kernel text addresses
def find_kernel_text_range(log_file):
	global flash_text_start_addr
	global flash_text_end_addr
	global ram_text_start_addr
	global ram_text_end_addr
	is_ram_text = False

	# Reading the System Map file and preparing the symbol map table
	with open(log_file) as searchfile:
		for line in searchfile:
			s = line.split(' ', 2)
			if len(s) == 3:
				if not '$' in s[2]:
					# s[0] contains address and s[2] contains Symbol name
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
		# Printing the kernel text address ranges in ram and flash areas
		print('~~~~~~~~~~~~~~~~~~~~~ KERNEL TEXT ADDRESS RANGE ~~~~~~~~~~~~~~~~~~~~~~')
		print('flash_text_start_addr\t=\t', flash_text_start_addr)
		print('flash_text_end_addr\t=\t', flash_text_end_addr)
		if (is_ram_text):
			print('ram_text_start_addr\t=\t', ram_text_start_addr)
			print('ram_text_end_addr\t=\t', ram_text_end_addr)
		print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
		print('')

# Function to setup Address to Symbol mapping table from the System Map and appX map files for kernel and applications
def setup_symbol_table(map_file, is_app):
	if debug:
		print('setup_symbol_table : '+ map_file, is_app)
	# Reading the System Map file and preparing the symbol map table
	with open(map_file) as searchfile:
		for line in searchfile:
			s = line.split(' ', 2)
			if len(s) == 3:
				if not '$' in s[2]:
					# s[0] contains address and s[2] contains Symbol name
					if(is_app):
						asymbol_lookup_table[is_app].append((int(s[0], 16), s[2].rstrip()))
					else:
						ksymbol_lookup_table.append((int(s[0], 16), s[2].rstrip()))
	if debug:
		# Printing the Created Symbol table
		print('\n~~~~~~~~~~~~~~~~~~~~~~~~ SYMBOL TABLE START ~~~~~~~~~~~~~~~~~~~~~')
		if(is_app):
			for line in asymbol_lookup_table[is_app]:
				print("{0:x} {1}".format(line[0], line[1]))
		else:
			for line in ksymbol_lookup_table:
				print("{0:x} {1}".format(line[0], line[1]))
		print('~~~~~~~~~~~~~~~~~~~~~~~~SYMBOL TABLE END   ~~~~~~~~~~~~~~~~~~~~~')


# Function to search the search_addr present in the Address to Symbol mapping table and print its corresponding symbol
def print_symbol(stack_addr, search_addr, is_app_symbol):
	addr = 0x00000000
	next_addr = 0x00000000
	l = 0
	if (is_app_symbol):
		h = len(asymbol_lookup_table[is_app_symbol]) - 1
	else:
		h = len(ksymbol_lookup_table) - 1
	mid = (l + h) // 2

	while (l <= h):
		if (is_app_symbol):
			addr = asymbol_lookup_table[is_app_symbol][mid - 1][0]
			next_addr = asymbol_lookup_table[is_app_symbol][mid][0]
		else:
			addr = ksymbol_lookup_table[mid - 1][0]
			next_addr = ksymbol_lookup_table[mid][0]

		if (search_addr >= addr and search_addr <= next_addr):
			if (is_app_symbol):
				print("{:8}\t {:8}\t app{}    \t  {}".format(hex(stack_addr), hex(addr), is_app_symbol, asymbol_lookup_table[is_app_symbol][mid - 1][1]))
			else:
				print("{:8}\t {:8}\t kernel  \t  {}".format(hex(stack_addr), hex(addr), ksymbol_lookup_table[mid - 1][1]))
			break

		if (search_addr < addr):
			h = mid - 1
		else:
			l = mid + 1

		mid = (l + h) // 2

	if (l > h):
		if debug:
			print('Symbol not found for address: {0} in map file'.format(hex(search_addr)))


# Function to check if address lies in the applications' text address range
def is_app_text_address(address):
	idx = 0
	# Check the application text address range
	for idx in range(app_idx):
		if (address >= hex(g_stext_app[idx]) and address < hex(g_etext_app[idx])):
			return (idx + 1)
	if (idx == app_idx):
		return False

# Function to check if address lies in the kernel text address range
def is_kernel_text_address(address):

	# Check the kernel text address range
	if (address >= ram_text_start_addr and address < ram_text_end_addr) or (address >= flash_text_start_addr and address < flash_text_end_addr):
		return True
	else:
		return False


# Function to Parse the i/p log file (which contains stackdump during assert) and to print stack values
def print_stack(log_file):
	stack_addr = 0x00000000
	stack_val = 0x00000000
	# Parse the contents based on tokens in log file.
	with open(log_file) as searchfile:
		for line in searchfile:
			if 'up_registerdump:' in line:
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

			# Read the stack contents of aborted stack and check for valid addresses
			if 'up_stackdump:' in line:
				word = line.split(':')
				t = word[2].split()
				stack_addr = int(word[1], 16)
				for sub_word in t:
					stack_addr = stack_addr + 4
					# Check for valid address, else move to the next sub_word
					if (sub_word == 'xxxxxxxx'):
						continue
					if (bytes_needed(int(sub_word,16)) != 4):
						continue

					stack_val = int(sub_word,16)
					# Check if the stack address lies in kernel text address range
					if (is_kernel_text_address(hex(stack_val))):
						#If yes, print it's corresponding symbol
						print_symbol(stack_addr, stack_val, 0)
					# Check if the stack address lies in application text address range
					is_app_symbol = is_app_text_address(hex(stack_val))	# app index in case of application symbol
					if (is_app_symbol):
						#If yes, print it's corresponding symbol
						stack_val = stack_val - int(g_stext_app[is_app_symbol - 1])
						print_symbol(stack_addr, stack_val, is_app_symbol)


# Function to Parse the i/p log file (which contains wrong stackdump during assert)
def print_wrong_sp(log_file):
	stack_addr = 0x00000000
	format_print = True
	# Parse the contents based on tokens in log file.
	with open(log_file) as searchfile:
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
					if (bytes_needed(int(sub_word,16)) != 4):
						continue

					stack_val = int(sub_word,16)
					# Check if the stack address lies in kernel text address range
					if (is_kernel_text_address(hex(stack_val))):
						if (format_print):
							print('\t- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:')
							print('Stack_address\t Symbol_address\t Symbol location  Symbol_name\t\tFile_name')
							format_print = False
						#If yes, print it's corresponding symbol
						print_symbol(stack_addr, stack_val, 0)
					# Check if the stack address lies in application text address range
					is_app_symbol = is_app_text_address(hex(stack_val))	# app index in case of application symbol
					if (is_app_symbol):
						if (format_print):
							print('\t- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:')
							print('Stack_address\t Symbol_address\t Symbol location  Symbol_name\t\tFile_name')
							format_print = False
						#If yes, print it's corresponding symbol
						stack_val = stack_val - int(g_stext_app[is_app_symbol - 1])
						print_symbol(stack_addr, stack_val, is_app_symbol)


#Execution starts here
if (__name__ == '__main__'):

	# init
	log_file = sys.argv[1]		# Log file containing crash logs
	app_idx = int(sys.argv[2])		# Number of applications (g_app_idx)
	is_wrong_sp = int(sys.argv[3])	# 1 if wrong SP information needs to be printed, 0 if no wrong sp in logs

	if debug:
		print("Log file : " + log_file)
		print("Number of applications : ", app_idx)
		print("is_wrong_sp : ", is_wrong_sp)

	find_kernel_text_range(bin_path + "System.map")
	if (app_idx > 0):
		find_app_text_range(log_file)

	setup_symbol_table(bin_path + "System.map", 0)
	for idx in range(app_idx):
		os.system("nm --defined-only -l --numeric-sort " + bin_path + app_name[idx] + "_dbg > " + bin_path + app_name[idx] + ".map")
		setup_symbol_table(bin_path + app_name[idx] + ".map", idx + 1)

	if (is_wrong_sp):
		print_wrong_sp(log_file)
	else:
		print_stack(log_file)
