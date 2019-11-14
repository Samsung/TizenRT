#!/usr/bin/env python
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
import sys
import re
import os
from sets import Set
from optparse import OptionParser

parser = OptionParser()
parser.add_option("-f", "--file", dest="infilename", help="map FILE needed to be parsed", metavar="INPUT_FILE")
parser.add_option("-o", "--output",dest="output", help="Output written to this file. Default is stdout.", metavar = "OUTPUT_FILE")
parser.add_option("-a", "--all", action="store_true", dest="all", help="Print all information.", default = False)
parser.add_option("-s", "--size", action="store_true", dest="totsize", help="Print total size of data segments.", default=False)
parser.add_option("-l", "--libsize", action="store_true", dest="libsize", help="Print Library Sizes.", default = False)
parser.add_option("-d", "--details", action="store_true", dest="details", help="Print details of Object Sizes.",default = False)
#Example: memstats_xtensa.py <option {-a / -s / -l / -d}> -f tinyara.map -o memory.txt.
(options, args) = parser.parse_args()
if not options.infilename:
        parser.print_help()
        sys.exit(1)

if (not options.all) and (not options.totsize) and (not options.libsize) and (not options.details):
        parser.print_help()
        sys.exit(1)

infile = open(options.infilename,'r')
if options.output:
        sys.stdout = open(options.output,'w')


lib_text_size = 0
lib_data_size = 0
lib_bss_size = 0
total = 0;
seglist = [".text", ".data",".bss"]
TEXT_INDEX = 0
DATA_INDEX = 1
BSS_INDEX = 2

LIB_PATH="../build/output/libraries"
LIB_INFO_PATH = 'temp_libinfo'

print "######################################"
print "##          Library Sizes           ##"
print "######################################"

dirs=os.listdir(LIB_PATH)
for seg in seglist:
    print "\t"+seg,
print "\t Total"

for libname in dirs:
    if re.search("lib",libname) != None:
        lib_text_size = 0
        lib_data_size = 0   
        lib_bss_size = 0
        total = 0
        file_path="../build/output/libraries/"+libname
        os.system('size ' + file_path + ' > ' + LIB_INFO_PATH)
       	fp = open(LIB_INFO_PATH, 'rb')
    	fp.readline()
        for array in fp.readlines():
    	    fp.close()
    	    size_array = array.split()
            lib_text_size += int(size_array[TEXT_INDEX])
   	    lib_data_size += int(size_array[DATA_INDEX])
   	    lib_bss_size +=  int(size_array[BSS_INDEX])
	    total = lib_text_size + lib_data_size + lib_bss_size   
        print "\t"+str(lib_text_size), "\t"+str(lib_data_size), "\t"+str(lib_bss_size)+"\t"+str(total)+"\t"+libname 
    else:
        continue

