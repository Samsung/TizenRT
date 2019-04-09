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

## How to program a binary
After building TizenRT, execute below command at $TIZENRT_BASEDIR/os folder.  
See [[Getting the sources]](https://github.com/Samsung/TizenRT#getting-the-sources) for how to set *TIZENRT_BASEDIR*.

After building TizenRT, execute below command at *os* folder. 
Please check the connection between IMXRT1020-EVK and PC 

```bash
make download
```
