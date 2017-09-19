
Trouble shooting
================

When using OpenOCD on Mac OS X and you encounter an error similar to


```
Error: libusb_claim_interface() failed with LIBUSB_ERROR_ACCESS
Error: unable to open ftdi device with vid 0403, pid 6010, description '*' and serial '*'
```


Make sure you are running make flash with sudo and that the system ftdi driver is unloaded:

From OpenOCD documentation ( source ):

```
If you're using a USB adapter and have a driver kext matched to it,
you will need to unload it prior to running OpenOCD. E.g. with Apple
driver (OS X 10.9 or later) for FTDI run:
  sudo kextunload -b com.apple.driver.AppleUSBFTDI
for FTDI vendor driver use:
  sudo kextunload FTDIUSBSerialDriver.kext
```


**The steps to avoid driver conflict were performed first:**

first, check the System Extensions (*.kext) on your Mac.

```
   kextstat | grep FTDI
```


You need to unload all of the FTDI driver(s) you have identified.

```
   sudo kextunload -b com.apple.driver.AppleUSBFTDI
                      (or)
   sudo kextunload -b com.FTDI.driver.FTDIUSBSerialDriver
```


You can see the following in 'kextutil' menual page.

```
     -p personality, -personality personality
              Send only the named personalities from the kext to the kernel.
              Repeat for each personality desired, or use the -interactive
              option to have kextutil ask for each personality.
```


now, try to below command:

```
   sudo kextutil -b com.apple.driver.AppleUSBFTDI -p AppleUSBEFTDI-6010-1
```


**I think you can run the Openocd without any problems if you have done the above.**

