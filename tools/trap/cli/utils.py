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
# File : heapNode.py
# Description: It act as utility for dumpParser and logParser module

from __future__ import print_function
from math import log
import os
import sys
import subprocess

ksymbol_lookup_table = []		# Lookup table created from contents of system map file for kernel symbols
asymbol_lookup_table = [[]] * 10	# Lookup table created from contents of appX map files for application symbols
debug = False				# Variable to print debug logs


def bytes_needed(n):
	if n == 0:
		return 1
	return int(log(n, 256)) + 1

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

# Function to search the search_addr present in the Address to Symbol mapping table and print its corresponding symbol and related information
def print_symbol(stack_addr, search_addr, is_app_symbol, bin_path, app_name):
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
			if (stack_addr == 0x00000000):
				if (is_app_symbol):
									loc = asymbol_lookup_table[is_app_symbol][mid - 1][1]
									filen = loc.split('	')
									print("\t  {:8}\t\t\t\t{}".format(hex(search_addr), filen[-1]))
				else:
									loc = ksymbol_lookup_table[mid - 1][1]
									filen = loc.split('	')
									print("\t  {:8}\t\t\t\t{}".format(hex(search_addr), filen[-1]))
				break
			else:
				if (is_app_symbol):
					sname = (asymbol_lookup_table[is_app_symbol][mid - 1][1])
					fname = subprocess.getoutput("arm-none-eabi-addr2line -e " + bin_path + app_name[is_app_symbol - 1] + "_dbg " + hex(search_addr))
					if not ('??:' in fname):
						print("{:8}\t {:8}\t {} binary    {: <45}  {}".format(hex(stack_addr), hex(search_addr), app_name[is_app_symbol - 1],  (sname[:20] + "..." + sname[-20:]) if len(sname) > 40 else sname, "File not found" if "*?" in fname else fname))
				else:
					sname = (ksymbol_lookup_table[mid - 1][1])
					fname = subprocess.getoutput("arm-none-eabi-addr2line -e " + bin_path + "tinyara.axf " + hex(search_addr))
					print("{:8}\t {:8}\t kernel binary    {: <45}  {}".format(hex(stack_addr), hex(search_addr), (sname[:20] + "..." + sname[-20:]) if len(sname) > 40 else sname , "File not found" if ":?" in fname else fname))
				break

		if (search_addr < addr):
			h = mid - 1
		else:
			l = mid + 1

		mid = (l + h) // 2

	if (l > h):
		if debug:
			print('Symbol not found for address: {0} in map file'.format(hex(search_addr)))

# Function to Parse the input log file (which contains heap corruption data) and to print owner of corrupted node
def print_heap_corruption_data(log_file, bin_path, app_name):
	# Parse the contents based on tokens in log file.
	with open(log_file) as searchfile:
		for line in searchfile:
			if 'allocated by code at addr' in line:
							word = line.split(' ')
							node = int(word[-1], 16)
							print_symbol(0x00000000, node, 0, bin_path, app_name)
							print_symbol(0x00000000, node, 1, bin_path, app_name)

# Function to Parse the input log file (which contains current running work function) and to print its owner
def print_running_work_function(log_file, bin_path, app_name):
	# Parse the contents based on tokens in log file.
	with open(log_file) as searchfile:
		for line in searchfile:
			if 'Running work function is' in line:
							word = line.split(' ')
							wf = word[-1]
							node = int(wf[:-2], 16)
							print_symbol(0x00000000, node, 0, bin_path, app_name)
							print_symbol(0x00000000, node, 1, bin_path, app_name)

# Function to Parse the input log file (which contains interrupt handler data) and to print its owner
def print_interrupt_handler_data(log_file, bin_path, app_name):
	# Parse the contents based on tokens in log file.
	with open(log_file) as searchfile:
		for line in searchfile:
			if 'IRQ handler' in line:
							word = line.split(' ')
							node = int(word[-1], 16)
							print_symbol(0x00000000, node, 0, bin_path, app_name)
							print_symbol(0x00000000, node, 1, bin_path, app_name)

# Function to return the address of a Symbol from mapping table
def get_address_of_symbol(symbol, debug=False):
	i = 0
	while (ksymbol_lookup_table[i][1] != symbol and i <= len(ksymbol_lookup_table)):
		i = i+1
	if (i > len(ksymbol_lookup_table)):
		return None
	else:
		if debug:
			print("Address of symbol {0:x} {1}".format(ksymbol_lookup_table[i][0], symbol))
		return ksymbol_lookup_table[i][0]

# Function to search for a address from "Address to Symbol" mapping table
# Apply Generic binary search
def symbol_table_lookup(addr, symbol_size=0):
	if (addr is None):
		return ('(Invalid address)', 0x0)

	low = 0
	high = len(ksymbol_lookup_table)
	mid = (low + high) >> 1
	premid = 0
	# Check whether given address is within the 2 neighbour address range or not ?
	while(not(addr >= ksymbol_lookup_table[mid][0] and addr < ksymbol_lookup_table[mid + 1][0])):

		if(addr < ksymbol_lookup_table[mid][0]):
			high = mid - 1

		if(addr > ksymbol_lookup_table[mid][0]):
			low = mid + 1

		mid = (high + low) >> 1

		if(mid == premid):
			return None
		if(mid + 1) >= len(ksymbol_lookup_table) or mid < 0:
			return None

		premid = mid

	if symbol_size == 0:
		return (ksymbol_lookup_table[mid][1], addr - ksymbol_lookup_table[mid][0])
	else:
		return (ksymbol_lookup_table[mid][1], ksymbol_lookup_table[mid + 1][0] - ksymbol_lookup_table[mid][0])
