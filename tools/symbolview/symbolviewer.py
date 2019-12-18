###########################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
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

from collections import OrderedDict
from operator import eq

import subprocess
import serial
import threading
import time
import sys

isExit = False

def rxfromUART(port):
    global isExit
    isStackInfo = False
    length = len("unwind_backtrace_with_fp: [<")

    while(isExit == False):
        time.sleep(0.01)

        lines = port.readlines()

        if lines == None:
            continue

        for line in lines:

            if line.find("unwind_backtrace_with_fp: Call Stack:") != -1:
                isStackInfo = True
            elif line.find("unwind_frame_with_fp: End of Callstack") != -1:
                isStackInfo = False

            if isStackInfo == True:
                if line.find("unwind_backtrace_with_fp: [<") != -1:
                             #unwind_backtrace_with_fp: [<
                    addr = line[length: length + 10]

                    proc = subprocess.Popen(["addr2line" ,
                                    "-e" ,
                                    "../../build/output/bin/tinyara",
                                    addr], stdout=subprocess.PIPE)
                    ouput, err = proc.communicate()
                    time.sleep(0.01)

                    sys.stdout.write( "%s \t: %s" % ((line),  ouput.decode('utf-8')))

            else:
                sys.stdout.write( "%s" % (line))

def txtoUART(port, send):
    if send.find("\n") != -1:
        PORT.write((send).encode())
    else:
        PORT.write((send+"\n").encode())

def wait2commnad(port):
    global isExit
    cmd = ""
    while(isExit == False):
        time.sleep(0.1)
        cmd = sys.stdin.readline()
        cmd.strip()
        if eq(cmd, "exit\n"):
            isExit = True
            break
        txtoUART(port, cmd)


if (__name__ == '__main__'):
    # init

    PORT = serial.Serial(sys.argv[1], baudrate=115200, timeout=0.1)

    rxthread = threading.Thread(target=rxfromUART,args=(PORT,))
    rxthread.daemon = True
    rxthread.start()

    txthread = threading.Thread(target=wait2commnad, args=(PORT,))
    txthread.daemon = True
    txthread.start()

    while(isExit == False):
        time.sleep(0.1)

    if (isExit == True):
        txthread.join()
        rxthread.join()
