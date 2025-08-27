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
