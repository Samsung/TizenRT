# User guide for Loadable Applications configuration on TizenRT

## CONTENTS

- [Overview of Loadable Apps](#overview-of-loadable-apps)
- [Assumptions and Limitations](#assumptions-and-limitations)
- [Steps to enable existing loadable apps configuration](#steps-to-enable-existing-loadable-apps-configuration)
- [Steps to add new loadable apps configuration](#steps-to-add-new-loadable-apps-configuration)

## Overview of Loadable Apps
- In case of Loadable Apps, in addition to a separate kernel binary, separate binaries are generated for each user space application. These binaries are stored in the flash & loaded to the RAM during runtime.
- Binary manager is a kernel thread to manage binaries and recover the faults in the system. The fault manager integrates into binary manager as recovery thread. It will identify the faulty app and take necessary action.
- The priority of binary manager is 203 and the binary manager loader is 200 whereas the priority of hpwork thread is 201. This is to ensure that hpwork must be running during the binary manager loader.
- This ensures the Real-time threads to be prioritized even during loading of binaries. The priorities in order are as follows:

```
	----------------------------------------
	MODULE				PRIORITY
	----------------------------------------
	BINARY_MANAGER_PRIORITY		  203
	CONFIG_SCHED_HPWORKPRIORITY	  201
	LOADINGTHD_PRIORITY		  200
	CONFIG_SCHED_LPWORKPRIORITY	  50
	----------------------------------------
```
- Loadable apps are needed for better fault management in the system. In case memory fault is detected in the system, the application could be reloaded without the need for the whole system to be rebooted (criticality & configuration dependent).
- There is an MPU based memory isolation & protection in place for each application. These apps are called loadable as they can be reloaded into their memory space even after fault detection.
- TizenRT provides 2 sample loadable apps called micom and wifi in TizenRT 3.0.
- Apart from these, the user can also add new loadable apps as per the requirement.

## Assumptions and Limitations

1. Protected build is enabled in the configuration (CONFIG_BUILD_PROTECTED=y).
The document [Protected Build Guide](ProtectedBuildGuide.md) provides the instructions for porting TizenRT protected build for a new chipset.
2. Flash and ram memory needs to be available to accommodate the applications.
3. The total ram requirement of an application needs to be fixed at build time.
4. Currently TizenRT supports only elf binary format.
5. Code in kernel space such as drivers are not allowed to use some APIs, which need to access memory in user space, as below:

 User APIs                      | Kernel APIs
--------------------------------|------------------------------------------
 malloc()                       | kmm_malloc()
 calloc()                       | kmm_calloc()
 zalloc()                       | kmm_zalloc()
 realloc()                      | kmm_realloc()
 memalign()                     | kmm_memalign()
 free()                         | kmm_free()
 pthread_create()               | kernel_thread()
 task_create()                  | kernel_thread()

## Steps to enable existing loadable apps configuration

### Configuration Settings

#### Essential Configurations

- The following configs should be enabled:

```
 ------------------------------------------------------------------------------------------------------
 CONFIG FLAG					VALUE		DESCRIPTION
 ------------------------------------------------------------------------------------------------------

 CONFIG_APP_BINARY_SEPARATION			y		App binary separation

 CONFIG_NUM_APPS				x		x = number of apps

 CONFIG_EXAMPLES_ELF				y		Loadable Apps test

 CONFIG_BINARY_MANAGER				y		Binary Manager
 CONFIG_BINMGR_RECOVERY				y		Binary Recovery Management (OPTIONAL)
 CONFIG_BINMGR_UPDATE				y		Binary Manager Update APIs (OPTIONAL)

 CONFIG_BINFMT_ENABLE				y		Binary Loader
 CONFIG_BINFMT_LOADABLE				y		Loadable binary format
 CONFIG_ELF					y		Enable the ELF Binary Format
 CONFIG_ELF_ALIGN_LOG2				2		Log2 Section Alignment
 CONFIG_ELF_STACKSIZE				2048		ELF Stack Size
 CONFIG_ELF_BUFFERSIZE				32		ELF I/O Buffer Size
 CONFIG_ELF_BUFFERINCR				32		ELF I/O Buffer Realloc Increment
 CONFIG_ELF_EXCLUDE_SYMBOLS			y		Excludes symbol information from ELF file
 CONFIG_ELF_CACHE_READ				n		ELF cache read support
 CONFIG_SYMTAB_ORDEREDBYNAME			n		Symbol Tables Ordered by Name
 CONFIG_OPTIMIZE_APP_RELOAD_TIME		y		Optimizations for application reload time
 ------------------------------------------------------------------------------------------------------

```

#### Optional configurations

These are not a part of the core loadable apps but can be optionally enabled to test some of the features.
Make sure that the maximum task size is increased to accommodate the extra tests enabled.

1. If MPU is enabled, MPU test can also be enabled as below:

```
CONFIG_EXAMPLES_MPU_TEST=y
CONFIG_MPUTEST_KERNEL_CODE_ADDR=0x60000000
CONFIG_MPUTEST_KERNEL_DATA_ADDR=0x20200000
CONFIG_MPUTEST_APP_ADDR=0x80200100
```
   In above scenario, the configs description is as below:

```
CONFIG_MPUTEST_KERNEL_CODE_ADDR		This is same as FLASH address		CONFIG_FLASH_START_ADDR
CONFIG_MPUTEST_KERNEL_DATA_ADDR		This is same as SRAM address		CONFIG_SRAM_START_ADDR
CONFIG_MPUTEST_APP_ADDR			Application sections' address		CONFIG_RAM_REGIONx_START
					(described below)
```

2. Other Loadable Apps test configs that could be enabled are as below:

```
#
# Enable Test Scenarios
#
CONFIG_EXAMPLES_MESSAGING_TEST=y
CONFIG_MESSAGING_TEST_REPETITION_NUM=1
CONFIG_EXAMPLES_RECOVERY_TEST=y
# CONFIG_ENABLE_RECOVERY_AGING_TEST is not set
# CONFIG_EXAMPLES_BINARY_UPDATE_TEST is not set
# CONFIG_EXAMPLES_MICOM_TIMER_TEST is not set
CONFIG_EXAMPLES_ELF_FULLYLINKED=y
```

### Memory Settings

ELF is stored in flash partition and loaded into ram at runtime and hence we need to reserve
both RAM & FLASH memories.

#### Flash Partition Settings

- Add a new flash partition for each application using the 3 configurations given below.
	1. The 'size' should be large enough to accommodate the binary size (CONFIG_FLASH_PART_SIZE).
	2. The 'type' should be of the type "bin" (CONFIG_FLASH_PART_TYPE).
	3. The 'name' can be the name of the application (CONFIG_FLASH_PART_NAME).

  In below scenario, the apps namely micom & wifi of sizes 1MB each are added. The two instances (wifi,wifi) are meant for FOTA update of binaries (if supported). One will contain the actual binary, whereas the other, the updated binary.

```
CONFIG_FLASH_PART_SIZE="2048,512,1024,1024,1024,1024,1024,128,128,"
CONFIG_FLASH_PART_TYPE="kernel,none,bin,bin,bin,bin,smartfs,ftl,config,"
CONFIG_FLASH_PART_NAME="kernel,app,micom,micom,wifi,wifi,userfs,ftl,config,"
```

#### RAM Memory Settings

- Text & data sections of loadable apps will be allocated from user RAM region.
- So find out the memory region large enough to load the appliacations.
- Add its address & size to CONFIG_RAM_REGIONx_START & CONFIG_RAM_REGIONx_SIZE (if not already present).

  In below scenario, we added a region which starts at 0x80200000 of size 30MB.

```
CONFIG_RAM_REGIONx_START="0x20200000,0x80200000"
CONFIG_RAM_REGIONx_SIZE="262144,31457280"
```

## Steps to add new loadable apps configuration

### Configuration Settings

- Follow the requirements as stated here ([Configuration Settings](#configuration-settings)) to enable all configurations related to loadable apps.

### Memory Settings

- Follow the requirements as stated here ([Memory Settings](#memory-settings)) for reserving flash & ram partitions for adding new applications.

### Code Changes Required

- Add required apps to "loadable_apps/loadable_sample/<app_name>".

#### Makefile

- Make sure to generate elf type binary for each newly added application.
  In <os/loadable_apps/loadable_sample/app/Makefile>,

```
ifeq ($(CONFIG_APP_BINARY_SEPARATION),y)

BIN = wifi
BIN_TYPE = ELF
BIN_VER = 20190412
DYNAMIC_RAM_SIZE = 512000
KERNEL_VER = 2.0
STACKSIZE = 8192
PRIORITY = 180

include $(TOPDIR)/$(LOADABLEDIR)/loadable.mk
endif # CONFIG_APP_BINARY_SEPARATION

```
Where,
- BIN_TYPE: Binary Type (should be elf).
- STACKSIZE: The first task will have a stack of this size.
- PRIORITY: The first (main) task of the app will be created with this priority.
- DYNAMIC_RAM_SIZE: This is the total amount of ram that will be available to the app. The developer must assess the max ram requirement for the app. During run time, if the memory requirement exceeds this value, then the app will fail. In that case the user might need to provide a higher value for this and rebuild the app and run it again.

These values will be used by the binary manager to start the first process of the application.

#### Make.defs

- Include the app folder in this as follows:

```
CONFIGURED += loadable_sample/<app>
```

#### Application code

- Below is the main signature for loadable apps where wifiapp being the non-critical app,
  calls preapp_start() to load tash.

```
extern int preapp_start(int argc, char **argv);

#ifdef CONFIG_APP_BINARY_SEPARATION
int main(int argc, char **argv)
#else
int wifiapp_main(int argc, char **argv)
#endif
{
.
.
.
#if defined(CONFIG_SYSTEM_PREAPP_INIT) && defined(CONFIG_APP_BINARY_SEPARATION)
        preapp_start(argc, argv);
#endif
```

- Add binary manager notification in the main function of the app that has been added.

```
#ifdef CONFIG_BINARY_MANAGER
        ret = binary_manager_notify_binary_started();
        if (ret < 0) {
                printf("MICOM notify 'START' state FAIL\n");
        }
#endif

```
