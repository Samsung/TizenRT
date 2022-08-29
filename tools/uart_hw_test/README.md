# UART HW test tool

This tool can be used to perform the uart HW test.
The tool performs the following tests.

UART Tx byte gap time test
1. The target prints log on the console continuously
2. The target sends data on the selected uart
3. The ubuntu app checks tx packet gap using time api

UART Rx loss check test
1. The target prints log on the console continuously
2. The ubuntu app sends data on the selected uart
3. The target verify rx data for data loss

When the tool is run, the Rx and Tx tests are performed for below scenarios:
Scenario 1: Perform FS operations in background and run the uart rx / tx test
Scenario 2: Print console logs continuously in a background thread and run the uart rx / tx test
Scenario 3: Perform iperf test in a background thread and run the uart rx / tx test

Each of the above tests are performed for the following baud rates.
115200, 38400, 19200, 9600, 4800


### How to USE
1. Find the UART port to be tested. For example, on the RTK8721 board, we will perform uart test for the uart1.
2. Connect the uart (Rx, Tx, Gnd) lines from the test device to the Linux PC using Serial-USB converter.
3. Enable CONFIG_UART_HW_TEST config. Build and download the binary.
4. After boot up, run the uart_hw_test command at the tash prompt
5. Input the port number on the device and on the PC. In our example, the uart1 appears as /dev/ttyS2 on the device. On the PC, the Serial-USB converter appears as /dev/ttyUSB2. Hence the port number is 2 in both the cases.
6. Now run the uart test tool on the PC (./uart_test.sh) and input the port number.
7. The tests are performed for all the scenarios mentioned above and for all the preset baud rates
8. The results are printed on the PC shell and indicate if each test is PASS or FAIL. If it is FAIL, then more details are printed.
9. In case of tx fail, the result shows the number of times the tx delay exceeds the spec and also what was the max delay in milliseconds.
10. In case of rx fail, the result shows the number of times the received character does not match the expected value.


NOTE!!!!
It is MANDATORY to first run the uart_hw_test tash command and enter the port number.
After this run the ./uart_test.sh test app on linux and enter the port number.
If this sequence is not followed, then the result is not proper.


Below is a sample set of results:
```
Enter UART port number : 1
Opened UART port : /dev/ttyUSB1
######################## UART Test Start #############################
######################################################################
Test Scenario 0: UART test only
######################################################################
[BAUD 115200 Tx] PASS
[BAUD 115200 Rx] PASS
[BAUD  38400 Tx] PASS
[BAUD  38400 Rx] PASS
[BAUD  19200 Tx] PASS
[BAUD  19200 Rx] PASS
[BAUD   9600 Tx] PASS
[BAUD   9600 Rx] PASS
[BAUD   4800 Tx] PASS
[BAUD   4800 Rx] PASS

######################################################################
Test Scenario 1: UART test with background FS ops
######################################################################
[BAUD 115200 Tx] PASS
[BAUD 115200 Rx] PASS
[BAUD  38400 Tx] PASS
[BAUD  38400 Rx] PASS
[BAUD  19200 Tx] PASS
[BAUD  19200 Rx] PASS
[BAUD   9600 Tx] PASS
[BAUD   9600 Rx] PASS
[BAUD   4800 Tx] PASS
[BAUD   4800 Rx] PASS

######################################################################
Test Scenario 2: UART test with background console logs
######################################################################
[BAUD 115200 Tx] PASS
[BAUD 115200 Rx] PASS
[BAUD  38400 Tx] PASS
[BAUD  38400 Rx] PASS
[BAUD  19200 Tx] PASS
[BAUD  19200 Rx] PASS
[BAUD   9600 Tx] PASS
[BAUD   9600 Rx] PASS
[BAUD   4800 Tx] PASS
[BAUD   4800 Rx] PASS

######################################################################
Test Scenario 3: UART test with background iperf
######################################################################
[BAUD 115200 Tx] PASS
[BAUD 115200 Rx] PASS
[BAUD  38400 Tx] PASS
[BAUD  38400 Rx] PASS
[BAUD  19200 Tx] PASS
[BAUD  19200 Rx] PASS
[BAUD   9600 Tx] PASS
[BAUD   9600 Rx] PASS
[BAUD   4800 Tx] PASS
[BAUD   4800 Rx] PASS
######################################################################
################## UART Test End [PASS: 40, FAIL: 0] #################
```
