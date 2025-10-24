# AmebaD J-Link GDB Debugger

### Preparations

1. Prepare J-Link and Download JLinkGDBServer from the [link](https://www.segger.com/downloads/jlink/#J-LinkSoftwareAndDocumentationPack) and install as below.  
```
$ sudo dpkg -i JLink_Linux_V686g_x86_64.deb
```

2. Upgrade the firmware to V10 and verify through the message of 'GDB Server' window like below.  
```
Firmware : J-Link V10 compiled Jan 7 2020 16:51:47
```

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
2. Run 'jlink_debug.sh' from *build/tools/amebad/gnu_utility* folder.
3. Enter gdb debug command in the 'GDB Client' window.



