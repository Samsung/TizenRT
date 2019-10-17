# IMXRT1050-EVK

## Contents
> [Information](#information)  
> [Environment Set-up](#environment-set-up)  
> [How to program a binary](#how-to-program-a-binary)  

## Information
Please refer NXP Semiconductors [homepage](https://www.nxp.com/support/developer-resources/software-development-tools/i.mx-developer-resources/i.mx-rt1050-evaluation-kit:MIMXRT1050-EVK).

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
