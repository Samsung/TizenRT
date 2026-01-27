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
# File : trap.py
# Description: Tool to parse the assert/crash log and ram dump
# Gdb and NM tools are used to read the symbols

from __future__ import print_function
import re
import os
import string
import sys, time
from getopt import GetoptError, getopt as GetOpt
from dumpParser import dumpParser
from logParser import logParser

# Global variables
# Variable to check if board has kernel text in RAM region
have_ram_kernel_text = False

# Variable to check CONFIG_XIP_ELF enabled
xip_enabled = False

# variable to check architecture family. holds true if it is armv7-r or armv8-r or armv7-a
arch_family_v7r = False

BIN_PATH = '../../../build/output/bin/'
CONFIG_PATH = '../../../os/.config'
MAKEFILE_PATH = '../../../os/Make.defs'


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
	print('Note: Architecture other than arm v7-r don\'t support dump parsing')
	print('')
	print('syntax :')
	print('--------')
	print('python3 %s -r Filename_ramBaseAddr_ramEndAddr.bin' % sys.argv[0])
	print('')
	print('Below example if you give dump file as path: ')
	print('--------------------------------------------')
	print('python3 trap.py -r build/output/bin/ramdump_0x4a0000_0x6a0000.bin')
	print('')
	print('Below example if you give simple assert log file as path: ')
	print('---------------------------------------------------------')
	print('python3 trap.py -t log.txt ')
	print('')
	print('Below example if you give binary file other than default as path: ')
	print('---------------------------------------------------------')
	print('python3 trap.py -t log.txt -b ../../../os/vidisha/bin/ -c ../../../os/cfile -e tinayara.axf')
	print('')
	print('')
	print('Note:')
	print('--------')
	print('1) For getting call stack CONFIG_FRAME_POINTER flag should be enabled in your build')
	print('')
	print('python3 trap.py -r /build/bin/ramdump_0x4a0000_0x6a0000.bin')
	print('')
	print('')
	print('*************************************************************')

	sys.exit(1)

	# Print TRAP output Header information i.e. log_file, dump file, elf files, etc.
def print_trap_output_header(lparser):
	print('')
	print('')
	print('*************************************************************')
	print('log_file                    :', lparser.log_file)
	if (lparser.g_app_idx > 0):
		print('Number of binaries          : %d [kernel + apps]' % (lparser.g_app_idx + 1))
	else:
		print('Number of binary            : 1 [kernel]')
	print('"kernel" elf_file           :', lparser.elf)
	for app_idx in range(lparser.g_app_idx):
		print('"' + lparser.app_name[app_idx] + '"', 'elf_file            :', lparser.bin_path + lparser.app_name[app_idx] + '_dbg ')
	print('*************************************************************')
	print('')

def check_required_config(data):
	global have_ram_kernel_text
	global xip_enabled
	global arch_family_v7r

	if ('CONFIG_ARCH_FAMILY="armv7-r"' in data):
		arch_family_v7r = True

	# check if CONFIG_XIP_ELF is enabled
	if 'CONFIG_XIP_ELF=y' in data:
		xip_enabled = True

	if 'CONFIG_ARCH_HAVE_RAM_KERNEL_TEXT=y' in data:
		have_ram_kernel_text = True

	if not 'CONFIG_DEBUG_MM_HEAPINFO=y' in data:
			print('DEBUG_MM_HEAPINFO is not enable. Enable DEBUG_MM_HEAPINFO to see heap usage')

def check_path(log_file, dump_file, elf_path):
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

	# check either if log file or dump file, one of them exists.
	if not log_file and not dump_file:
		print('Usage error: Must specify one of the -t or -e options. Plz find below for proper usage')
		usage()

	exist_check_list = [
		("dump_file", dump_file),
		("log_file", log_file),
		("elf", elf)
	]

	#check if path exist for each required file.
	for name, path in exist_check_list:
		if path and not os.path.exists(path):
			print(f"{path} does not exist. Please provide the proper path for {name}...")
			sys.exit(1)
	return elf

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
	global have_ram_kernel_text
	global xip_enabled
	global arch_family_v7r

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

	elf = check_path(log_file, dump_file, elf_path)

	# Read config information
	fd = open(CONFIG_PATH, 'r')
	data = fd.read()
	fd.close()

	check_required_config(data)

	try:
		if log_file is not None:
			# Instance for assert log Parsing
			lparser = logParser(elf = elf, log_file = log_file, bin_path = BIN_PATH, config_path = CONFIG_PATH, xip_enabled = xip_enabled, have_ram_kernel_text =have_ram_kernel_text)

			# Format log file if timestamp is present at the start of each line
			lparser.format_log_file()

			# Get the number of application binaries, names, text address and sizes
			lparser.find_number_of_binaries()

			# Print TRAP output Header information i.e. log_file, dump file, elf files, etc.
			print_trap_output_header(lparser)

			# Function to parse the assert/crash log
			lparser.parse_log()

		elif arch_family_v7r:
			# Calling the Constructor with the initial set of arguments
			rParser = dumpParser(dump_file = dump_file, elf = elf, bin_path = BIN_PATH, config_path = CONFIG_PATH, have_ram_kernel_text = have_ram_kernel_text, debug=False)
			# If the log file not given, Consider parsing using full dump
			rParser.parse_ram_dump()

		else:
			# For architecture other than arm v7-r, we don't support dump parsing.
			usage()
			return None

	except Exception as e:
		print("ERROR:", e)


if __name__ == '__main__':
	main()
