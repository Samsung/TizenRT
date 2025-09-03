## /*
##  * Copyright (c) 2025 Syntiant Corp.  All rights reserved.
##  * Contact at http://www.syntiant.com
##  *
##  * This software is available to you under a choice of one of two licenses.
##  * You may choose to be licensed under the terms of the GNU General Public
##  * License (GPL) Version 2, available from the file LICENSE in the main
##  * directory of this source tree, or the OpenIB.org BSD license below.  Any
##  * code involving Linux software will require selection of the GNU General
##  * Public License (GPL) Version 2.
##  *
##  * OPENIB.ORG BSD LICENSE
##  *
##  * Redistribution and use in source and binary forms, with or without
##  * modification, are permitted provided that the following conditions are met:
##  *
##  * 1. Redistributions of source code must retain the above copyright notice,
##  * this list of conditions and the following disclaimer.
##  *
##  * 2. Redistributions in binary form must reproduce the above copyright
##  * notice, this list of conditions and the following disclaimer in the
##  * documentation and/or other materials provided with the distribution.
##  *
##  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
##  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
##  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
##  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
##  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
##  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
##  * DEALINGS IN THE SOFTWARE.
## */

import socket
import datetime
import signal
import sys

# Create a TCP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

# Bind the socket to a specific address and port
server_address = ('', 12345)
server_socket.bind(server_address)

# Listen for incoming connections
server_socket.listen()

def sigHandler(signo, frame):
    server_socket.close()
    sys.exit(0)


signal.signal(signal.SIGINT, sigHandler)

print('Server listening on port 12345')

i = 1
# Accept connections
while True:
    connection, client_address = server_socket.accept()
    print('Connected to:', client_address)

    # Open a file for writing
    now = datetime.datetime.now()
    filename = now.strftime("record_%Y-%m-%d_%H-%M-%S.bin")
    with open(filename, 'wb') as file:
        total_len = 0
        while True:
            data = connection.recv(8192)
            if not data:
                print("No data")
                break
            total_len += len(data)
            print("Chunk {} => {}  (total: {})".format(i, len(data), total_len))
            i += 1
            
            file.write(data)
    print(f"Wrote {filename}")

    connection.close()
    print('Connection closed')
