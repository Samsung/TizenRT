#!/usr/bin/env python
###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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
import socket
import subprocess
from struct import unpack
from optparse import OptionParser


class RtLog:
    ErrorModule = {
        0: "Wi-Fi Manager",
        1: "WPA Supplicant",
        2: "lwIP Core",
        3: "lwIP Socket",
        4: "None",
    }
    function = ''
    file_path = ''
    time = 0
    task_addr = 0
    err_code = 0
    module_id = 4
    peer_id = ''

    def __init__(self, function, file_path, time, task_addr, err_code,
                 module_id, peer_id):
        self.function, self.file_path, self.time = function, file_path, time
        self.task_addr, self.err_code = task_addr, err_code
        self.module_id, self.peer_id = module_id, peer_id

    def print_log(self):
        log = str(self.time) + ' ' + self.peer_id + ' ' + self.file_path + \
              ' ' + self.function + ' ' + str(hex(self.task_addr)) + ' ' + \
              self.ErrorModule[self.module_id] + ':' + str(self.err_code)
        print(log)


class RtLogBuilder:
    function = ''
    file_path = ''
    time = 0
    task_addr = 0
    err_code = 0
    module_id = 4
    peer_id = '0.0.0.0'

    def __init__(self):
        pass

    def set_function(self, function):
        self.function = function
        return self

    def set_filepath(self, file_path):
        self.file_path = file_path
        return self

    def set_time(self, time):
        self.time = time
        return self

    def set_taskaddr(self, task_addr):
        self.task_addr = task_addr
        return self

    def set_errcode(self, err_code):
        self.err_code = err_code
        return self

    def set_moduleid(self, module_id):
        self.module_id = module_id
        return self

    def set_peerid(self, peer_id):
        self.peer_id = peer_id
        return self

    def build(self):
        log_item = RtLog(self.function, self.file_path, self.time,
                         self.task_addr, self.err_code, self.module_id,
                         self.peer_id)
        return log_item


class RtLogStorage:
    log_storage = []

    def __init__(self, mgr):
        self.mgr = mgr

    def addItem(self, item):
        self.log_storage.append(item)
        item.print_log()

        return

    def print_logs(self):
        for item in self.log_storage:
            item.print_log()


class RtLogParser:
    path = './tinyara'
    tool = 'arm-none-eabi-addr2line'
    tool_opt = '-fCe'

    def __init__(self, binary_path, manager):
        self.path, self.manager = binary_path, manager
        print("Binary path: " + self.path)
        print("default path is " + RtLogParser.path + " set path is " +
              self.path)
        print("Tool: " + self.tool + " with option: " + self.tool_opt)

    def parse(self, data, addr):
        sec, = unpack('<I', data[0:4])
        usec, = unpack('<I', data[4:8])
        pc, = unpack('<I', data[8:12])
        task_addr, = unpack('<I', data[12:16])
        err_code, = unpack('<h', data[16:18])
        module_id, = unpack('<h', data[18:20])
        # print(self.path)
        command = self.tool + ' ' + self.tool_opt + ' ' + str(self.path) +\
            ' ' + str(hex(pc))
        # print(command)
        result = subprocess.check_output(command, shell=True)
        # print(result)
        pc_info = result.decode('utf-8').split('\n')
        time = sec + usec / 1000000.0
        builder = RtLogBuilder()
        log = builder.set_time(time).set_filepath(pc_info[0])\
            .set_function(pc_info[1]).set_moduleid(module_id)\
            .set_taskaddr(task_addr).set_errcode(err_code).set_peerid(addr)\
            .build()

        cmd = InsertCommand(self.manager)
        self.manager.set_command(cmd, log)

        return


class Command:
    def __init__(self, mgr):
        self.mgr = mgr
        return

    def execute(self, *arg):
        pass


class InsertCommand(Command):
    def execute(self, *arg):
        self.insert_item(arg[0])
        return

    def insert_item(self, *arg):
        self.mgr.storage.addItem(arg[0])
        return


class ParseCommand(Command):
    def parse_item(self, *arg):
        self.mgr.parser.parse(arg[0], arg[1])
        return

    def execute(self, *arg):
        self.parse_item(*arg)
        return


class RtLogMgr:
    def __init__(self):
        pass

    def set_parser(self, parser):
        self.parser = parser
        return

    def set_storage(self, storage):
        self.storage = storage
        return

    def set_command(self, cmd, *arg):
        cmd.execute(*arg)
        return


class RtLogServer:

    parser = None
    address = ''
    port = 0

    def __init__(self, addr, port, mgr):
        self.address = addr
        self.port = port
        self.mgr = mgr

        return

    def __enter__(self):
        try:
            self.server_socket = socket.socket(socket.AF_INET,
                                               socket.SOCK_STREAM)
        except socket.error as msg:
            print(msg)
            return

        try:
            self.server_socket.setsockopt(socket.SOL_SOCKET,
                                          socket.SO_REUSEADDR, 1)
            self.server_socket.bind((self.address, self.port))
            self.server_socket.listen(10)
        except socket.error as msg:
            self.server_socket.close()
            print(msg)
            return

        return self

    def __exit__(self, type, value, traceback):
        self.server_socket.close()
        return

    def receive_data_from_server(self):
        while True:
            try:
                client_socket, addr = self.server_socket.accept()
                print("Connected by " + addr[0] + ":" + str(addr[1]))
            except socket.error as msg:
                print(msg)
                return

            try:
                while True:
                    data = client_socket.recv(128)
                    if len(data) == 0:
                        print("Connection is closed")
                        client_socket.close()
                        break

                    # print("receive data:" + str(len(data)))
                    my_byte = bytearray(data)
                    for index in range(0, len(data), 20):
                        # self.parser.parse(my_byte[index:index+20], addr[0])
                        cmd = ParseCommand(self.mgr)
                        self.mgr.set_command(cmd, my_byte[index:index+20],
                                             addr[0])
            except socket.error as msg:
                print(msg)
                client_socket.close()
                return

        return my_byte


class RtLogServerBuilder:
    port = 9098
    addr = ''
    parser = None

    def __init__(self):
        pass

    def set_port(self, port):
        self.port = port
        return self

    def set_addr(self, address):
        self.addr = address
        return self

    def set_parser(self, parser):
        self.parser = parser
        return self

    def set_manager(self, manager):
        self.mgr = manager
        return self

    def build(self):
        rt_server = RtLogServer(self.addr, self.port, self.mgr)
        return rt_server


PORT = 9098
IP_ADDR = "INADDR_ANY"


def main(argv=None):
    port = PORT
    addr = IP_ADDR
    use = 'Usage: %prog [options] binary_path'

    parser = OptionParser(usage = use)

    parser.add_option("-p", "--port",
                      dest = "port",
                      metavar="PORT",
                      default = False,
                      action = "store",
                      type = 'int',
                      help = "set listen port")
    parser.add_option("-a", "--addr",
                      dest = "addr",
                      metavar='ADDRESS',
                      default = False,
                      action = "store",
                      help = "set binding ip address(IPv4 only)")

    options, args = parser.parse_args()

    if len(sys.argv) == 1 or len(args) is not 1:
        parser.error('Invalid input')
        return

    log_manager = RtLogMgr()
    server_builder = RtLogServerBuilder()
    server_builder.set_parser(parser)
    server_builder.set_manager(log_manager)

    if options.port:
        port = options.port
        server_builder.set_port(port)
    if options.addr:
        addr = options.addr
        server_builder.set_addr(addr)

    print("Port number is " + str(port))
    print("Binding address is " + addr)
    print("Binary path is " + args[0])

    with server_builder.build() as log_server:
        parser = RtLogParser(args[0], log_manager)
        storage = RtLogStorage(log_manager)
        log_manager.set_parser(parser)
        log_manager.set_storage(storage)

        log_server.receive_data_from_server()

    return


if __name__ == "__main__":
    sys.exit(main())
