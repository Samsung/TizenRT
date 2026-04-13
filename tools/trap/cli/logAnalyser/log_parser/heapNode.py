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
# Description: It contains heap node class which helps in parsing kernel and user heap corruption from log file

from __future__ import print_function
import os

closing_log_line = "##########################################################################################################################################"


class heap_node:
    def __init__(self, node_info, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled):
        #Split node data using "," and parse info
        node_info = node_info.split(",")
        addr_at = node_info[0].index("addr =") + len("addr =")
        type_at = node_info[1].index("type =") + len("type =")
        size_at = node_info[2].index("size =") + len("size =")
        prec_add_at = node_info[3].index("preceding size =") + len("preceding size =")

        owner_pid_info = node_info[4].split("(")
        owner_pid_at = owner_pid_info[0].index("owner pid =") + len("owner pid =")
        isStack = 'X'
        sind = owner_pid_info[1].find('stack')
        pid_str = '('+str(owner_pid_info[1])
        if (sind !=-1):
            isStack ='O'
            pid_str = '('+str(owner_pid_info[1][0:sind])+')'

        alloca_at = node_info[5].index("allocated by code at addr = ")+len("allocated by code at addr = ")

        self.addr = node_info[0][addr_at:].replace(" ","")

        self.isAllocated = "O"
        if node_info[1][type_at:].replace(" ","") == "F" or node_info[1][type_at:].replace(" ","") == "f":
            self.isAllocated = "X"

        self.size = node_info[2][size_at:].replace(" ","")
        self.preceding_size = node_info[3][prec_add_at:].replace(" ","")

        self.ownerpid = str(abs(int(owner_pid_info[0][owner_pid_at:].replace(" ",""))))+" "+pid_str      
        self.stacknode = isStack
        self.allocAt =  node_info[5][alloca_at:].replace(" ","")

        isKernel = True
        addrcmd = ""
        address = int(self.allocAt, 16)

        for app_idx in range(g_app_idx):
            #check if address falls in app
            if (address >= int(hex(g_stext_app[app_idx]),16) and address < int(hex(g_etext_app[app_idx]),16)):
                isKernel = False
                if xip_enabled:
                    addr = address
                else:
                    addr = address - int(hex(g_stext_app[app_idx]), 16)
                addrcmd = 'arm-none-eabi-addr2line -e ' + bin_path + app_name[app_idx] + '_dbg ' + hex(addr) 

        #if address is in kernel
        if isKernel == True:
            addrcmd = "arm-none-eabi-addr2line -e {file} {trans_address}".format(file= elf,trans_address=hex(address))

        self.filename = os.popen(addrcmd).read()

    def print_info(self):
        print("\tAddress        : ", self.addr)
        print("\tAllocated node : ", self.isAllocated)
        print("\tSize           : ", self.size)
        print("\tPreceding size : ", self.preceding_size)
        print("\tOwner pid      : ", self.ownerpid)
        print("\tStack node     : ", self.stacknode)
        print("\tAllocated by   : ", self.allocAt + " (" + self.filename[ :-1] +")")

def parseCorruptHeapInfo(line, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled, searchfile):
    heap_corrupted =  False
    while line != "No more lines" and 'Heap start =' in line:
        #Read start and end address from heap
        heap_start_at =  line.index("Heap start = ") + len("Heap start = ")
        start_info = line[heap_start_at:].split(' ')
        heap_start_add = start_info[0]
        heap_end_add = start_info[-1][:-1]
        line = next(searchfile, "No more lines")
        print("Checking corruption ({} - {}) : ".format(heap_start_add,heap_end_add), end="")

        #Skip unnecessary info
        if ('#######' in line) and line != closing_log_line:
            line = next(searchfile, "No more lines")
        else:
            print("Heap NOT corrupted")

        #Check if heap is corrupted
        if 'Heap corruption detected in mm_heapstart node' in line:
            heap_corrupted = True
            print("Heap corruption detected in heap start node")
            while 'HEAP START NODE' not in line:
                line = next(searchfile)
            node_info = ""
            while ('HEAP START NODE' in line):
                node_info+= line[:-1] + ","
                line =next(searchfile)
            print("  Heap Start Node : ")
            start_heap_node = heap_node(node_info)
            start_heap_node.print_info()
 
        elif 'Heap corruption detected.' in line:
            heap_corrupted = True
            print("Heap corrupted")
            line = next(searchfile)

            while 'Forward traversal of heap' in line:
                line = next(searchfile)
                if ('==========' in line):
                    line =next(searchfile)

                    #Parse previous, current and next node
                    node_info = ""
                    while ('PREV NODE' in line):
                        node_info+= line[:-1] + ","
                        line =next(searchfile)
                    print("  Previous Node : ")
                    prev_heap_node = heap_node(node_info, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled)
                    prev_heap_node.print_info()

                    node_info = ""
                    while ('CORRUPT NODE' in line):
                        node_info+= line[:-1] + ","
                        line =next(searchfile)
                    curr_heap_node = heap_node(node_info, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled)

                    while 'CORRUPT NODE' not in line and 'HEAP END NODE'  not in line:
                        line = next(searchfile)

                    if 'CORRUPT NODE' not in line:
                        print(" Corrupt Node : ")
                        curr_heap_node.print_info()

                    if ('CORRUPT NODE' in line):
                        node_info = ""
                        while ('CORRUPT NODE' in line):
                            node_info+= line[:-1] + ","
                            line =next(searchfile)
                        curr_heap_node_2 = heap_node(node_info, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled)

                        if(curr_heap_node.addr != curr_heap_node_2.addr):
                            print(" Corrupt Node 1 : ")
                            curr_heap_node.print_info()
                            print(" Corrupt Node 2: ")
                            curr_heap_node_2.print_info()
                        else:
                            print(" Corrupt Node : ")
                            curr_heap_node.print_info()

                        node_info = ""
                        while ('PREV NODE' in line):
                            node_info+= line[:-1] + ","
                            line =next(searchfile)
                        print("  Next Node : ")
                        next_heap_node = heap_node(node_info, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled)						
                        next_heap_node.print_info()

                    elif ('HEAP END NODE' in line):
                        node_info = ""
                        while ('HEAP END NODE' in line):
                            node_info+= line[:-1] + ","
                            line =next(searchfile)
                        print("  Next Node : ")
                        next_heap_node = heap_node(node_info, g_app_idx, g_stext_app, g_etext_app, app_name, elf, bin_path, xip_enabled)						
                        next_heap_node.print_info()
            print()
    return heap_corrupted
