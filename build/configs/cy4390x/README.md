# CY4390X

## Contents
> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [How to program a binary](#how-to-program-a-binary)  

## Information

Please refer Cypress [homepage](http://www.cypress.com/documentation/datasheets/cyw43907-wiced-ieee-80211-abgn-soc-embedded-applications-processor).

## Environment Set-up

### Install FTDI driver
```bash
sudo apt-get install libftdi-dev
```

### Register Cypress USB device rule
1. Make a file named 99-\<anyname\>.rules.
2. Add below contents at above file.
```bash
SUBSYSTEMS=="usb",ATTRS{idVendor}=="0a5c",ATTRS{idProduct}=="43fa",MODE="0666" RUN+="/sbin/modprobe ftdi_sio" RUN+="/bin/sh -c 'echo 0a5c 43fa > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'"
```
3. Place the above file in /etc/udev/rules.d folder with sudo permission.
4. Reboot your system.

## How to program a binary

**Note**: This is not available now because programming tools are not opened yet.  
To download programming tools, the link will be guided.

After building TizenRT, execute below command at *os* folder.  

```bash
make download [Programming Option]
```

For example, *ALL* option makes complete set of binaries programmed.
```bash
make download ALL
```
See programming options list which is pre-defined at [.flashSpec.xml](.flashSpec.xml).
