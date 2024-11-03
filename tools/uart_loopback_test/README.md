# UART Loopback test

This test expands the existing UART loopback test by adding stress through communication between a PC and a chip


## Requirements

### Install python package
```
$ pip3 install pyserial
```

### Enable uart_loopback example
This tool is used with `apps/examples/uart_loopback`

Enable *CONFIG_EXAMPLES_UART_LOOPBACK* on menuconfig.

```
Application Configuration -> Examples -> [*] UART loopback test application
```

## How to use uart_loopback_test
```
$ sudo python3 tools/uart_loopback_test/uart_loopback_test.py
============================================
  Select Port
============================================
  "1. /dev/ttyS1"
  "2. /dev/ttyS0"
  "3. /dev/ttyUSB3"
  "4. /dev/ttyUSB2"
  "5. /dev/ttyUSB1"
  "6. /dev/ttyUSB0"
============================================
5
============================================
        START uart_loopback test

Port    : "/dev/ttyUSB1"
Baudrate: 115200 bps
============================================
1 b'1234567890abcdefghijklmnopqrstuvwxyz'
2 b'1234567890abcdefghijklmnopqrstuvwxyz
3 b'1234567890abcdefghijklmnopqrstuvwxyz
4 b'1234567890abcdefghijklmnopqrstuvwxyz
```

## Test Option

### 1. delay 

you can add delay of response as below in `uart_loopback_test.py`

```
delay = 0.1    # 100ms (delay off : 0)
```

### 2. Fixed response value
you can ensure that even if incorrect data is received, a fixed correct data will be responded.
```
test_len = 37
test_str = "1234567890abcdefghijklmnopqrstuvwxyz\0"

fixde_response = True
```
