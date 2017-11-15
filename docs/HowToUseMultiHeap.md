# How to use(add) Multi-heap Regions

There are two cases to use multi-heap regions.  
1. continuous physical RAM but want to split regions  
2. separated physical RAMs but want to use for heap  

Modify *[Kconfig](#Kconfig)* and *[TizenRT configuration file](#tizenrt-configuration-file)*.  

## Kconfig

Add **select ARCH_HAVE_HEAPx** belong to config ARCH_ARM in os/arch/<ARCH_NAME>/src/<CHIP_NAME>/Kconfig.
```
config ARCH_CHIP_<CHIP_NAME>
	select ARCH_HAVE_HEAPx
```
This will enable HEAPx_BASE and HEAPx_SIZE configs in os/mm/Kconfig.

## TizenRT configuration file

Set base addresses and sizes for new heap regions through menuconfig.
```
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
Set *CONFIG_HEAPx_BASE* and *CONFIG_HEAPx_SIZE* based on new heap region information.
```
Memory Management -> Set List of start address for additional heap region
Memory Management -> Set List of size for additional heap region
```

TizenRT will add additional heap region automatically.
