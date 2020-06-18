# IMXRT1020-EVK

## Contents
> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [How to program a binary](#how-to-program-a-binary)  

## Information

Please refer NXP Semiconductors [homepage](https://www.nxp.com/support/developer-resources/software-development-tools/i.mx-developer-resources/i.mx-rt1020-evaluation-kit:MIMXRT1020-EVK).

## Environment Set-up
This section covers board-specific environment set-up.  
Please set TizenRT common environment, [quick start](https://github.com/Samsung/TizenRT#quick-start), first before doing below.

### Add USB device rules

This is an optional environment.  
But as the device is connected through USB, some operation like programming of binary can't be worked without this configuration.

The *dbuild.sh* script with the *menu* option supports the *u. USBrule* option to add it.

1. Select the *d. Download* option after building TizenRT
```sh
======================================================
  "Select build Option"
======================================================
  "1. Build with Current Configurations"
  "2. Re-configure"
  "3. Menuconfig"
  "4. Build Clean"
  "5. Build Dist-Clean"
  "6. Build SmartFS Image"
  "d. Download"
  "x. Exit"
======================================================
d
```
2. Select the *u. USBrule* option
```sh
==================================================
  "Select download option"
==================================================
  "1. ALL"
  "2. KERNEL"
  "3. APP"
  "4. MICOM"
  "5. WIFI"
  "6. USERFS"
  "8. ERASE KERNEL"
  "9. ERASE APP"
  "10. ERASE MICOM"
  "11. ERASE WIFI"
  "12. ERASE USERFS"
  "13. ERASE ALL"
  "u. USBrule"
  "x. Exit"
==================================================
u
```

>**NOTE**
>It requires root permission.

## How to program a binary
After building TizenRT, execute below command at $TIZENRT_BASEDIR/os folder.  
See [[Getting the sources]](https://github.com/Samsung/TizenRT#getting-the-sources) for how to set *TIZENRT_BASEDIR*.

After building TizenRT, execute below command at *os* folder.

By manual commands:
```bash
./dbuild.sh download PARTITION

i.e. os$ ./dbuild.sh download ALL
     os$ ./dbuild.sh download kernel wifi
```
When you command download with unavailable PARTITION name, it shows HELP message with available partition names.

By an interactive tool:
```bash
./dbuild.sh menu

======================================================
  "Select build Option"
======================================================
  "1. Build with Current Configurations"
  "2. Re-configure"
  "3. Menuconfig"
  "4. Build Clean"
  "5. Build Dist-Clean"
  "d. Download"
  "x. Exit"
======================================================
d

==================================================
  "Select download option"
==================================================
  "1. ALL"
  "2. OS"
  "x. Exit"
==================================================
1
```
