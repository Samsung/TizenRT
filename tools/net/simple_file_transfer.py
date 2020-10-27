#!/usr/bin/env python3
############################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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
############################################################################

import socket
import os
import argparse
import logging
import sys

LOG = logging.getLogger(__name__)
PORT = 5555        # The port used by the server


def send_file(target_addr, send_file_name, target_file_name):

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((target_addr, PORT))
        direction = 1
        s.sendall(direction.to_bytes(4, byteorder='big'))
        s.sendall(len(target_file_name).to_bytes(4, byteorder='big'))
        s.sendall(target_file_name.encode())
        try:
            file_size = os.path.getsize(send_file_name)
            LOG.info("file size: %d", file_size)
            s.sendall(file_size.to_bytes(4, byteorder='big'))
        except os.error:
            LOG.exception("file is not exist")

        with open(send_file_name, 'r') as infile:
            data = infile.read()
            s.sendall(data.encode())
            LOG.info('File transfering is done')


def receive_file(target_addr, send_file_name, target_file_name):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((target_addr, PORT))
        direction = 0
        s.sendall(direction.to_bytes(4, byteorder='big'))
        s.sendall(len(target_file_name).to_bytes(4, byteorder='big'))
        s.sendall(target_file_name.encode())

        data = s.recv(4)
        file_size = socket.ntohl(int.from_bytes(data, sys.byteorder))
        LOG.info("file size: %d", file_size)
        with open(send_file_name, 'wb') as infile:
            data = s.recv(file_size)
            infile.write(data)
            LOG.info("File receiving is done")

    return


if __name__ == '__main__':
    logging.basicConfig(
        format="%(asctime)s.%(msecs)03d %(levelname)-8s %(message)s",
        level=logging.DEBUG,
        datefmt="%Y-%m-%d %H:%M:%S",
    )

    parser = argparse.ArgumentParser(
        description='Simple File Transfer -a\
        target_addr -s file to send -t file path of target')
    parser.add_argument('-r', action='store_true', help='reverse path')
    parser.add_argument('-a', help='address', required=True)
    parser.add_argument('-s', help='source file', required=True)
    parser.add_argument('-t', help='target path', required=True)

    args = parser.parse_args()
    LOG.info("Target address: %s", args.a)
    LOG.info("File name(source): %s", args.s)
    LOG.info("File name(target): %s", args.t)
    LOG.info("Direction: %s", args.r)

    if args.r:
        receive_file(target_addr=args.a, send_file_name=args.s,
                     target_file_name=args.t)
    else:
        send_file(target_addr=args.a, send_file_name=args.s,
                  target_file_name=args.t)
