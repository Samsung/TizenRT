#!/usr/bin/env python3
# coding: utf8
#
# control appc agent

import serial
import threading
import socket
import argparse

UDP_PORT_IN = 9004
udp_server_address = ('',UDP_PORT_IN) #udp server
udp_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
udp_socket.bind(udp_server_address)
udp_client = None
debug = True

# uart timeout for different messages
UART_TIMEOUT = 0.1

# parse commandline arguments
def parse_args():
    description = '''Control APPC Agent.'''
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('-d', '--device',
                        default='/dev/ttyUSB1',
                        help="Uart device, default /dev/ttyUSB1")
    parser.add_argument('-b', '--baudrate', type=int,
                        default=1500000,
                        help="burn uart baudrate, defaults to 1500000")
    args = parser.parse_args()

    return args

# parse args
args = parse_args()

# Init Uart
ser = serial.Serial(args.device, 115200, timeout=UART_TIMEOUT)

# Global rx buffer from target, will be sent to UDP server
serial_rx_buf = b''

def serial_thread_handler():
    # receive target's response
    global serial_rx_buf
    global udp_client
    while True:
        data = ser.read(1024)
        if data:
            serial_rx_buf += data
            if debug:
                print("Target  ->   Agent:\t", data)
        else:
            # 500ms timeout, assume all data are received
            # print("serial_rx_buf: ", serial_rx_buf)
            # print("udp_client: ", udp_client)
            if len(serial_rx_buf) > 0 and udp_client:
                if debug:
                    print('Agent   ->   Server:\t', serial_rx_buf)
                udp_socket.sendto(serial_rx_buf, udp_client)
                serial_rx_buf = b''

def udp_thread_handler():
    # received udp request and forward to target by serial
    global udp_client
    while True:
        data, client = udp_socket.recvfrom(4096)
        udp_client = client
        if debug:
            print("Server  ->   Agent:\t", data)
        ser.write(data)

# create thread
serial_thread = threading.Thread(target=serial_thread_handler)
udp_thread = threading.Thread(target=udp_thread_handler)

# create thread
serial_thread.start()
udp_thread.start()

