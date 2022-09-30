# Amebalite J-Link GDB Debugger

### Preparations

1. Prepare J-Link and Download JLinkGDBServer from the [link](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) and install as below. 
```
$ sudo dpkg -i JLink_Linux_V780b_x86_64.deb
```

2. Upgrade the firmware to V10 and verify through the message of 'GDB Server' window like below.


### Connection

| J-Link | AmebaD |
|--------|--------|
|   VCC  |  3.3v  |
|   GND  |  GND   |
|  SWDIO |  DATA  |
|  SWCLK |  CLK   |

Refer to J-Link interface information [link](https://www.segger.com/products/debug-probes/j-link/technology/interface-description/).

### How to use?

1. Build TizenRT and make sure 'tinyara.axf' is generated in *build/output/bin* folder.
2. Connect module board with Putty to download TizenRT image and press reset button.
3. Connect Segger JLink in environment.
4. Run 'jlink_debug.sh' from *build/tools/amebalite/gnu_utility* folder.
5. Enter gdb debug command in the 'GDB Client' window.



