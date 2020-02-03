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
PASS_COUNT = 0
FAIL_COUNT = 0

sys.stdout.flush()

def serial_write(port, send=""):
    print ("\n")

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
            print (('['+sys._getframe(3).f_code.co_name+']'+" %s" % (line.decode())), end='')
            ret1 = line.find(("unregistered").encode())
            ret2 = line.find(("] FAIL").encode())
            if (ret1 != -1) | (ret2 != -1):
                return False

        receive_cnt = len(receive)
        judge_cnt = 0
        for line in lines:
            for data in receive:
                ret = line.find(data.encode())
                if ret != -1:
                    judge_cnt = judge_cnt + 1
                    break

        if (receive_cnt <= judge_cnt):
            return True
        elif not lines:
            end_time = time.time()
            if (end_time - start_time >= limit_time):
                return False
            continue


def serial_way(usb_port, send, receive, limit_time):
    port = serial.Serial(usb_port, baudrate=115200, timeout=0.1)
    try:
        serial_write(port, send)
    except:
        time.sleep(5)
        serial_write(port, send)

    result = serial_read(port, receive, limit_time)
    return result


def execute_cmd(cmd, receive, port, limit_time):
    result = serial_way(port, cmd, receive, limit_time)
    return result

##### TC Function #####


def cmd_arastorage_utc():
    data = []
    data.append("End")
    result = execute_cmd("arastorage_utc", data, PORT, 10)
    return result


def cmd_arastorage_itc():
    data = []
    data.append("End")
    result = execute_cmd("arastorage_itc", data, PORT, 10)
    return result


def cmd_drivers_tc():
    data = []
    data.append("End")
    result = execute_cmd("drivers_tc", data, PORT, 10)
    return result


def cmd_filesystem_tc():
    data = []
    data.append("End")
    result = execute_cmd("filesystem_tc", data, PORT, 10)
    return result


def cmd_kernel_tc():
    data = []
    data.append("End")
    result = execute_cmd("kernel_tc", data, PORT, 15)
    return result


def cmd_network_tc():
    data = []
    data.append("End")
    result = execute_cmd("network_tc", data, PORT, 10)
    return result


def cmd_sysio_itc():
    data = []
    data.append("End")
    result = execute_cmd("sysio_itc", data, PORT, 10)
    return result


def cmd_sysio_utc():
    data = []
    data.append("End")
    result = execute_cmd("sysio_utc", data, PORT, 10)
    return result


def cmd_taskmgr_utc():
    data = []
    data.append("End")
    result = execute_cmd("taskmgr_utc", data, PORT, 10)
    return result


def cmd_taskmgr_itc():
    data = []
    data.append("End")
    result = execute_cmd("taskmgr_itc", data, PORT, 10)
    return result


def cmd_messaging_utc():
    data = []
    data.append("End")
    result = execute_cmd("messaging_utc", data, PORT, 10)
    return result


def cmd_security_utc():
    data = []
    data.append("End")
    result = execute_cmd("security_utc", data, PORT, 10)
    return result


def cmd_wifimgr_utc():
    data = []
    data.append("End")
    result = execute_cmd("wifimgr_utc", data, PORT, 10)
    return result


###### CMD Function #####
def cmd_cat():
    data = []
    data.append("test")
    execute_cmd("cd /mnt", "", PORT, 5)
    execute_cmd("echo test > cat_test", "", PORT, 5)
    result = execute_cmd("cat cat_test", data, PORT, 5)
    return result


def cmd_cd():
    result = execute_cmd("cd /mnt", "", PORT, 5)
    return result


def cmd_date():
    result = execute_cmd("cd date", "", PORT, 5)
    return result


def cmd_df():
    data = []
    data.append("Block  Number")

    result = execute_cmd("df", data, PORT, 5)
    return result


def cmd_echo():
    data = []
    data.append("echo_test")
    result = execute_cmd("echo echo_test", data, PORT, 5)
    return result


def cmd_free():
    data = []
    data.append("    total       used       free    largest")
    result = execute_cmd("free", data, PORT, 5)
    return result


def cmd_setenv():
    data = []
    data.append('env "test_set=test" is SET')
    result = execute_cmd("setenv test_set test", data, PORT, 5)
    return result


def cmd_getenv():
    data = []
    data.append("test_get=test")
    execute_cmd("setenv test_get test", "", PORT, 5)
    result = execute_cmd("getenv test_get", data, PORT, 5)
    return result


def cmd_unsetenv():
    data = []
    data.append('''env "test_unset" is UNSET''')
    execute_cmd("setenv test_unset test", "", PORT, 5)
    result = execute_cmd("unsetenv test_unset", data, PORT, 5)
    return result


def cmd_help():
    data = []
    data.append("TASH command list")
    data.append("--------------------")
    result = execute_cmd("help", data, PORT, 5)
    return result


def cmd_kill():
    result = execute_cmd("kill -1 5", "", PORT, 5)
    return result


def cmd_ls():
    data = []
    data.append("/:")
    data.append("TASH>>")
    execute_cmd("cd /", "", PORT, 5)
    result = execute_cmd("ls", data, PORT, 5)
    return result


def cmd_mkdir():
    data = []
    data.append("/mkdir_test:")
    execute_cmd("cd /", "", PORT, 5)
    execute_cmd("mkdir mkdir_test", "", PORT, 5)
    result = execute_cmd("ls /mkdir_test", data, PORT, 5)
    return result


def cmd_ps():
    data = []
    data.append("PID | PRIO | FLAG |  TYPE   | NP |  STATUS  | NAME")
    data.append("------|------|------|---------|----|----------|----------")
    result = execute_cmd("ps", data, PORT, 5)
    return result


def cmd_pwd():
    data = []
    data.append("/mnt")
    data.append("TASH>>")
    execute_cmd("cd /mnt", "", PORT, 5)
    result = execute_cmd("pwd", data, PORT, 5)
    return result


def cmd_rm():
    data = []
    data.append("/mnt/rm_test deleted")
    execute_cmd("cd /mnt", "", PORT, 5)
    execute_cmd("echo rm > rm_test", "", PORT, 5)
    result = execute_cmd("rm rm_test", data, PORT, 5)
    return result


def cmd_rmdir():
    data = []
    data.append("failed")
    execute_cmd("cd /", "", PORT, 5)
    execute_cmd("mkdir rmdir_test", "", PORT, 5)
    execute_cmd("rmdir rmdir_test", "", PORT, 5)
    result = execute_cmd("ls /rmdir_test", data, PORT, 5)
    return result


def cmd_sleep():
    data = []
    data.append("TASH>>")
    result = execute_cmd("sleep 0", data, PORT, 5)
    return result


###### CMD List #####
CMD_LIST = OrderedDict()
##### TC Function #####

# CMD_LIST['arastorage_utc'] = cmd_arastorage_utc
# CMD_LIST['arastorage_itc'] = cmd_arastorage_itc
# CMD_LIST['drivers_tc'] = cmd_drivers_tc
# CMD_LIST['filesystem_tc'] = cmd_filesystem_tc
# CMD_LIST['kernel_tc'] = cmd_kernel_tc
# CMD_LIST['network_tc'] = cmd_network_tc
# CMD_LIST['sysio_itc'] = cmd_sysio_itc
# CMD_LIST['sysio_utc'] = cmd_sysio_utc
# CMD_LIST['taskmgr_utc'] = cmd_taskmgr_utc
# CMD_LIST['taskmgr_itc'] = cmd_taskmgr_itc
# CMD_LIST['messaging_utc'] = cmd_messaging_utc
# CMD_LIST['security_utc'] = cmd_security_utc
# CMD_LIST['wifimgr_utc'] = cmd_wifimgr_utc

###### CMD Function #####

CMD_LIST['cat'] = cmd_cat
CMD_LIST['cd'] = cmd_cd
CMD_LIST['date'] = cmd_date
CMD_LIST['df'] = cmd_df
CMD_LIST['echo'] = cmd_echo
CMD_LIST['free'] = cmd_free
CMD_LIST['setenv'] = cmd_setenv
CMD_LIST['getenv'] = cmd_getenv
CMD_LIST['unsetenv'] = cmd_unsetenv
CMD_LIST['help'] = cmd_help
CMD_LIST['kill'] = cmd_kill
CMD_LIST['ls'] = cmd_ls
CMD_LIST['mkdir'] = cmd_mkdir
CMD_LIST['ps'] = cmd_ps
CMD_LIST['pwd'] = cmd_pwd
CMD_LIST['rm'] = cmd_rm
CMD_LIST['rmdir'] = cmd_rmdir
CMD_LIST['sleep'] = cmd_sleep


def init():
    execute_cmd("\n", "", PORT, 5)


if (__name__ == '__main__'):
    init()
    result_list = OrderedDict()
    for key, tc in CMD_LIST.items():
        result = CMD_LIST[key]()
        if result == False:
            result_list[key] = ('\n['+(CMD_LIST[key].__name__)+']' + " : FAIL")
            FAIL_COUNT = FAIL_COUNT + 1
        else:
            result_list[key] = ('\n['+(CMD_LIST[key].__name__)+']' + " : PASS")
            PASS_COUNT = PASS_COUNT + 1

        print (result_list[key])

    print ("\n\n########## Auto Test Result ##########")
    for key, log in result_list.items():
        print (result_list[key], end='')

    print ("\n\n########## Auto Test End [PASS : %d, FAIL : %d] ##########" % (
        PASS_COUNT, FAIL_COUNT))
