# AmebaD J-Link GDB Debugger

### Preparations

1. Prepare J-Link with firmware upgraded to V10.
2. Install JLinkGDBServer.
3. Set 'environment variable' for JLinkGDBServer if not done so.

=============================

### Connection

Jlink     AmebaD
VCC   ->  3.3v
GND   ->  GND
SWDIO ->  DATA
SWCLK ->  CLK

=============================

### How to use?

1. Build TizenRT and make sure 'tinyara.axf' is generated.
2. Run 'jlink_debug.sh' in this directory.
3. Enter gdb debug command in the 'GDB Client' window.

=============================


