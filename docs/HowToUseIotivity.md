# How to use IoTivity

IoTivity is an open source software framework enabling seamless device-to-device connectivity to address the emerging needs of the Internet of Things.  
More detailed information, please find it at [README](../external/iotivity/iotivity_1.2-rel/README.md) and [IoTivity Site](https://iotivity.org).  

## To use the IoTivity feature

### 1. with a build configuration of IoTivity

Please find it at [README](../external/iotivity/README.md).  

### 2. with another build configuration

Set the Iotivity feature through menuconfig.  
```
~/$ cd $TIZENRT_BASEDIR
~/TIZENRT_BASEDIR$ cd os
~/TIZENRT_BASEDIR/os$ make menuconfig
```
In menuconfig, select the 'enable / disable iotivity stack'.  
```
    -+-External Libraries
          |
          +-enable / disable iotivity stack
```

## To build IoTivity sample app - simple server

Please find it at [README](../apps/examples/iotivity_simpleserver/README.md).


