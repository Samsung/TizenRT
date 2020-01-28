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

import serial
import time
import sys

PORT = sys.argv[1]

sys.stdout.flush()

def serial_write(port, send=""):
    if port.isOpen() == False:
        try:
            port.open()
            print ("Port connect = ", port.isOpen())
        except FileNotFoundError:
            print ("FILE Not Found Error")
            time.sleep(2)
            port.open()
        except serial.serialutil.SerialException:
            print ("serial.serialutil.SerialException")
            time.sleep(2)
            port.open()

    if send.find("\n") != -1:
        port.write((send).encode())
    else:
        port.write((send+"\n").encode())
    time.sleep(0.1)


def serial_read(port, receive="", limit_time=1):
    sys.stdout.flush()
    if port.isOpen() == False:
        try:
            port.open()
            print ("Port connect = ", port.isOpen())
        except FileNotFoundError:
            print ("FILE Not Found Error")
            time.sleep(2)
            port.open()
        except serial.serialutil.SerialException:
            print ("serial.serialutil.SerialException")
            time.sleep(2)
            port.open()

        print ("Port connect = ", port.isOpen())

    start_time = time.time()
    while(True):
        # print ("\n FLUSHED...\n",)
        sys.stdout.flush()
        time.sleep(0.01)

        while(True):
            time.sleep(0.01)
            lines = port.readlines()
            if lines != None:
                break
            end_time = time.time()
            if (end_time - start_time >= limit_time):
                print ("[serial_read_error]Time over %s" % (limit_time))
                break

        if len(lines) != 0:
            start_time = time.time()

        for line in lines:
            print (" %s" % (line)),

        receive_cnt = len(receive)
        judge_cnt = 0
        for line in lines:
            for data in receive:
                ret = line.find(data)
                if ret != -1:
                    judge_cnt = judge_cnt + 1
                    break

        if (receive_cnt <= judge_cnt):
            # print ("\nPort is READ successfully\n")
            return True
        elif not lines:
            end_time = time.time()
            if (end_time - start_time >= limit_time):
                print ("end_tiem = %d, start_time = %d, limit_time = %d\n" % end_time, start_time, limit_time)
                return False
            continue

def serial_way(usb_port, send, receive, limit_time):
    port = serial.Serial(usb_port, baudrate=115200, timeout=0.1)
    try:
        serial_write(port, send)
    except:
        time.sleep(5)
        print ("\tserial_write " + send + " failed\n");

    result = serial_read(port, receive, limit_time)
    return result


def execute_cmd(cmd, receive, port, limit_time):
    serial_way(port, cmd, receive, limit_time)


def init():
    execute_cmd("\n", "", PORT, 5)

if (__name__ == '__main__'):
    data = []
    execute_cmd("DUMPFS", data, PORT, 30)
