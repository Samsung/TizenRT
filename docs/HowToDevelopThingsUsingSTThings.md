# How to develop things using ST Things based TizenRT

ST Things (SmartThings Things) is lightweight OCF development framework based TizenRT.  
It provides APIs to develop OCF devices and supports SmartThings Services provided by mobile devices to easily connect SmartThings Cloud.

## Prerequisites
### 1. To use TizenRT
Please refer to each details.  

- Toolchain installation [[Details]](../README.md#getting-the-toolchain)  
- Board specific prerequisites [[Details]](../README.md#supported-board--emulator)  
- IoTivity prerequisite installation [[Details]](../external/iotivity/README.md#prerequisites)  
- ROMFS prerequisite installation [[Details]](../tools/fs/README_ROMFS.md#pre-condition)  


### 2. To use SmartThings Cloud
A device developed using ST Things based TizenRT can connect to SmartThings Cloud.  
In order to work with the cloud, it is necessary to sign up Samsung Account at [SmartThings Developers](https://smartthings.developer.samsung.com/) site.  
Then, follow the steps.  
1. Get your MNID - Manufacturer ID - on [My Page](https://smartthings.developer.samsung.com/partner/dashboard)  
2. Create a cloud-connected device [[Details]](https://smartthings.developer.samsung.com/develop/getting-started/cloud-to-cloud.html)  
3. Generate the Certificate Signing Request and issue a new Signing key for the device certificate [[Details]](https://smartthings.developer.samsung.com/develop/workspace/general-tools/certificate-signing-request.html)  
4. Add `certificate.pem` and `privateKey.der` files to `$TIZENRT_BASEDIR/tools/fs/contents/`  
5. Edit Json file to match the following: JSON  
   ```  
    "certificate": "certificate.pem",    
    "privateKey": "privateKey.der"  
   ```  

To develop App(SmartThings) UI and functionality, use the SmartThings SDK. Please refer to [SmartThings SDK](https://smartthings.developer.samsung.com/develop/workspace/general-tools/sdk.html) site.  
The device definition is stored in a JSON configuration file that you need to provide [[Device Definition]](https://developer.tizen.org/development/iot-preview/iot-apis/things-sdk-api/device-definition)   

## Getting Started
## 1. with Tizen Studio for RT
The Tizen Studio for RT is an IDE that provides a lightweight RTOS (real-time operating system)-based application development environment that helps you develop, build, flash, and debug applications in Ubuntu.  
Select OS as **Ubuntu** and download Tizen Studio for RT IDE at [HERE](https://developer.tizen.org/development/tizen-studio/download).  
Tizen Studio for RT supports ST Things since v2.0.  
Please refer to [README](HowToDevelopThingsWithTizenStudioForRT.md).

## 2. without the IDE
Please find it at [README](../apps/examples/st_things/README.md) to build ST Things sample app.  

## API Reference
Please find ST Things APIs at [README](API_Reference/README.md).  
