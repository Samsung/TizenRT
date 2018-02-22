# How to develop things using SmartThings Things SDK based TizenRT

SmartThings Things(ST-Things) SDK is lightweight OCF development framework based TizenRT.  
It provides APIs to develop OCF devices and supports Samsung Connect Services provided by Samsung mobile devices to easily connect Samsung Cloud.

## Prerequisites
### 1. To use TizenRT
Please refer to each details.  

- Toolchain installation [[Details]](../README.md#getting-the-toolchain)  
- Board specific prerequisites [[Details]](../README.md#supported-board--emulator)  
- IoTivity prerequisite installation [[Details]](../external/iotivity/README.md#prerequisites)  
- ROMFS prerequisite installation [[Details]](../tools/fs/README_ROMFS.md#pre-condition)  


### 2. To use Samsung Cloud
A device developed using ST-Things SDK based TizenRT can connect to Samsung Cloud.  
In order to work with the cloud, it is necessary to sign up Samsung Account at [SmartThings Developers](https://smartthings.developer.samsung.com/) site.  
Then, follow the steps.  
1. Get your MNID - Manufacturer ID - on [My Page](https://smartthings.developer.samsung.com/partner/dashboard)  
2. Create a cloud-connected device [[Details]](https://smartthings.developer.samsung.com/develop/workspace/ide/create-a-cloud-connected-device.html)  
3. Generate the Certificate Signing Request and issue a new Signing key for the device certificate [[Details]](https://smartthings.developer.samsung.com/develop/workspace/ide/about-common-tools.html)  
4. Add `certificate.pem` and `privateKey.der` files to `$TIZENRT_BASEDIR/tools/fs/contents/`   
5. Edit Json file to match the following: JSON  
   ```  
    "certificate": "certificate.pem",    
    "privateKey": "privateKey.der"  
   ```  

To develop App(Samsung Connect) UI and functionality, use the SmartThings SDK. Please refer to [SmartThings SDK](https://smartthings.developer.samsung.com/develop/workspace/sdk/overview.html) site.

## Getting Started
## 1. with Tizen Studio for RT
The Tizen Studio for RT is an IDE that provides a lightweight RTOS (real-time operating system)-based application development environment that helps you develop, build, flash, and debug applications in Ubuntu.  
Select OS as **Ubuntu** and download Tizen Studio for RT IDE at [HERE](https://developer.tizen.org/development/tizen-studio/download).  
Tizen Studio supports ST-Things SDK since v2.0.  
Please refer to [README](HowToDevelopThingsWithTizenStudioForRT.md).

## 2. without the IDE
Please find it at [README](../apps/examples/st_things/README.md) to build st_things sample app.  

## API Reference
Please find ST-Things APIs at [README](API_Reference/README.md).  
