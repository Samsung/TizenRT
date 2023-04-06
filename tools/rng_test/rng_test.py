###########################################################################
#
# Copyright 2023 Samsung Electronics All Rights Reserved.
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

from operator import eq

import serial
import time
import threading
import sys

# ------------- Global Variables -------------
port_num = sys.argv[1]          # port number (user input)
baudrate = int(sys.argv[2])     # baudrate (user input)

iteration = int(sys.argv[3])    # Number of iteration (user input)
iter_cnt = 0                    # Current number of iteration

is_exit = False                  # True if exit the script
total_results = {}              # Dictionary for storing total results
rand_nums = []                  # list for saving generated random number


def rxfromUART(port):
    global is_exit

    prev = ''
    while (is_exit == False):
        time.sleep(0.01)

        line = port.readline().decode()
        if (prev.find("Random : ") != -1):
            rand_nums.append(line)
        #sys.stdout.write("%s" % (line))
        prev = line


def txtoUART(port):
    global is_exit
    global iter_cnt

    cmd = ''
    while (is_exit == False):
        time.sleep(0.1)

        # Reboot
        cmd = 'reboot\n'
        port.write(cmd.encode())
        time.sleep(5)

        cmd = '\n'
        for i in range(5):
            port.write(cmd.encode())
        
        # Generate random number by using TASH command (security_api)
        cmd = 'security_api\n'
        port.write(cmd.encode())
        time.sleep(1)

        print('Test {0} : {1}'.format(iter_cnt + 1, rand_nums[iter_cnt]))

        iter_cnt += 1
        if (iter_cnt == iteration):
            is_exit = True
            break


if (__name__ == "__main__"):
    print('\n------------- Start RNG Duplication Test -------------\n')

    # Connect to UART port
    PORT = serial.Serial(port_num, baudrate, timeout=0.1)

    # Start RX, TX thread
    rxthread = threading.Thread(target=rxfromUART, args=(PORT,))
    rxthread.daemon = True
    rxthread.start()

    txthread = threading.Thread(target=txtoUART, args=(PORT,))
    txthread.daemon = True
    txthread.start()
    
    while (is_exit == False):
        time.sleep(0.1)


    # Terminate threads
    if (is_exit == True):
        txthread.join()
        rxthread.join()
    
    print('------------- End iteration -------------\n')

    # Count duplicated cases
    for index, num in enumerate(rand_nums):
        if num in total_results:
            total_results[num].append(index + 1)
        else:
            total_results[num] = [index + 1]

    cases = 0       # number of duplicate cases
    duplicated = {} # Store duplicate cases
    for num, indices in total_results.items():
        if len(indices) > 1:
            cases += 1
            duplicated[num] = indices

    # Show results
    print('Total duplicated cases : {0} / {1}\n'.format(cases, iteration))

    if cases == 0:
        print('No duplicated cases found\n')
    else:
        print('----------!!! Duplication occurred !!!----------\n')
        for num, indices in duplicated.items():
            for index in indices:
                print('Test {0} : {1}'.format(index, num))
            print('----------------------------------------')

        
    print('\n------------- End RNG Duplication Test -------------\n')
