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
# File : log_preprocessor.py
# Description: Contains preprocessing functions for log files including
# format validation and binary discovery

from __future__ import print_function
import os
import sys
from array import *

# Global variables
assertion_details = "Assertion details\n"
BIN_ADDR_FXN = "Loading location information\n"


partition_string = "==========================================================="


# Function to format logs and delete the timestamp (supported formats-|xxxxxxxxx| and [xxxxxxxxx]) if it consists of timestamp at the start of each log line
def format_log_file(log_file):

    current_line = ""  # Initialize current_line

    # Delete unwanted logs (if any) and timestamp at the start of each line
    with open(log_file, "r") as f:
        data = f.readlines()
    with open(log_file, "w") as f:
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
    with open(log_file, "r") as f:
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

# Function to get the number of application binaries, names, text address and sizes
def find_number_of_binaries(lparser):

    current_line = ""
    # Parse the contents based on tokens in log file.
    with open(lparser.log_file) as searchfile:
        for line in searchfile:
            if partition_string in line:
                line = next(searchfile)
                current_line = line
                line = next(searchfile)
                continue
            # Get the number of applications loaded
            if BIN_ADDR_FXN == current_line:
                lparser.g_app_idx = lparser.g_app_idx + 1

    app_idx = 0
    lparser.g_stext_app = array('i', range(0, lparser.g_app_idx))
    lparser.g_etext_app = array('i', range(0, lparser.g_app_idx))
    with open(lparser.log_file) as searchfile:
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
                lparser.app_name.append(w[1][start_idx:end_idx])
                lparser.g_stext_app[app_idx] = int(t[0], 16)
                lparser.g_etext_app[app_idx] = lparser.g_stext_app[app_idx] + int(word[3], 10) # word[3] is text_size
                app_idx = app_idx + 1

    if app_idx == 0:
        app_count = 0
        app_names = []

        debug_files = [
            f for f in os.listdir(lparser.bin_path)
            if f.endswith("_dbg") and os.path.isfile(os.path.join(lparser.bin_path, f))
        ]

        for f in debug_files:
            app_names.append(f.split("_")[0])
            lparser.app_name.append(app_names[app_count])
            app_count = app_count + 1

        if app_count > 0:
            lparser.read_all_elf = True
            lparser.g_app_idx = app_count
            lparser.g_stext_app = array('i', range(0, lparser.g_app_idx))
            lparser.g_etext_app = array('i', range(0, lparser.g_app_idx))

            for app_idx in range(app_count):
                #setting start and end address with minimum and maximum possible values for now when address is not available
                lparser.g_stext_app[app_idx] = int("0x00000000", 16)
                lparser.g_etext_app[app_idx] = int("0x77777777", 16)
        else:
            print("\nNo debug files found for common and app binaries\n")
    
def preprocessLogFile(lparser):
    # Format log file if timestamp is present at the start of each line
	format_log_file(lparser.log_file)

    # Get the number of application binaries, names, text address and sizes
	find_number_of_binaries(lparser)