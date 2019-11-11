# Porting Guide

## Contents
- [Policy & Caution](#policy--caution)  
- [Kernel & System](#kernel--system)  
- [File System](#file-system)  
- [Network](#network)  
- [Audio](#audio)  
- [Confirmation of Porting](#confirmation-of-porting)

## Policy & Caution
- Driver Implementation Recommendation  
Instead of hooking the default driver interface code (which is a part of SoC package) to the platform,  
it is advisable to include the drivers according the TizenRT platform driver model.

- New Application Inclusion  
It is a good practice to add the Kconfig entry. All application configurations and dependencies should be included properly.

- Use driver, framework and system call APIs  
Don't call architecture codes from application directly. When protected build enables, it will be blocked.  
Application must call TizenRT driver interface to talk to underlying driver.

- Maintain TizenRT Code Structure  
It is not advised if addition of peripherals, new interfaces, new open source code etc are done by aligning to default board package structure.  
Understanding the TizenRT code structure and adhering to the structure helps in better architecture design and portability.

## Kernel & System
1. [How to add new board](HowToAddnewBoard.md)
2. [How to use peripheral](HowToUsePeripheral.md)
3. [How to add static library](HowToAddStaticLibrary.md)
4. [How to configure memory](HowToConfigureMemory.md)
5. [How to use MPU](HowToUseMPU.md)

## File System
1. [How to use SmartFS](HowToUseSmartFS.md)

## Network
1. [How to port TizenRT on new WiFi chipset](HowToPortTizenRTOnWiFiChipset.md)
2. [How to use LWIP](HowToUseLWIP.md)

## Audio
[How to use audio](HowToUseAudio.md)

## Confirmation of Porting
1. [How to confirm Network](HowToConfirmNetworkPorting.md)

Will be updated
