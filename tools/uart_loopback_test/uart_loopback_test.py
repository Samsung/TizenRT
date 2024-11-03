from serial import Serial
from time import sleep

import serial.tools.list_ports as sp

################### Test Option #########################
baudrate = 115200
delay = 0.1    # 100ms (delay off : 0)

test_len = 37
test_str = "1234567890abcdefghijklmnopqrstuvwxyz\0"

fixde_response = True
#########################################################

print("============================================")
print("  Select Port")
print("============================================")
port_list = []
port_n = 1
for sp_port in sp.comports():
	print("  \"{}. {}\"".format(port_n, sp_port.device))
	port_list.append(sp_port.device)
	port_n += 1
print("============================================")
idx = int(input(""))
port = port_list[idx - 1]
print("============================================")
print("        START uart_loopback test")
print("")
print("Port    : \"{}\"".format(port))
print("Baudrate: {} bps".format(baudrate))
print("============================================")

ser = Serial(port, baudrate)
test_count = 0
while True:
    if ser.readable():
        res = ser.read(test_len)
        test_count += 1
        print(test_count, res)
        if delay > 0:
            sleep(delay)

        if fixde_response:
            ser.write(test_str.encode('utf-8'))
        else:
            ser.write(res)
