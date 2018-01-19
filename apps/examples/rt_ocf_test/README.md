# RT-OCF



## Overview

The RT-OCF project is to develop an open source software framework which is a lightweight device-to-device (D2D) framework based on Open Connectivity Foundation (OCF) specification for IoT devices. RT-OCF targets [TizenRT](https://github.com/Samsung/TizenRT), a lightweight RTOS-based platform to support low-end IoT devices, and provides functionalities such as device discovery, create/read/update/delete operations of device resources, resource observation, and so on. All target functionalities of RT-OCF are not yet completed. For example, provisioning to cloud considering security together is underdeveloped.
The Charter of the RT-OCF Open Source Project is to create, develop and maintain open source technologies. 

The architecture of RT-OCF is as follows.

<img src="./docs/overview.png">

And key modules of RT-OCF shown in the above architecture are summarized as follows.

| Module        | Functionality |
| ------------- |-------------|
| Resource      | Resource/Request/Observe definition |
| Messaging     | Coap and transport layers      |
| Security | Acl2/cred/doxm/pstat handler      |
| Utils | Memory and data structure APIs      |

### Module

RT-OCF Module is a unit of the source code that accomplishes a specific objective, a subsystem of RT-OCF. Modules are established by the development community on as-need basis. They are hosted on the RT-OCF infrastructure, follow the rules of community and of development, and follow the coding guidelines established by the RT-OCF community. 

Each Module is led by the Reviewer and Maintainer. New Module can be suggested by any member of the Community, but the final decision on hosting said project rests with the Steering Committee. Modules that are no longer active are archived and do not have assigned Reviewers and Maintainers.


## Setup Preparation

To run `RT-OCF`, you need to install the package below.

1. Move to 'external' directory.

```sh
$ cd external
```

2. Clone RT-OCF.

```sh
$ git clone https://github.com/Samsung/RT-OCF.git
```

3. Edit `external/Kconfig`.

```sh
$ vi ./Kconfig

Add "source "$EXTERNALDIR/RT-OCF/Kconfig" to at the end of 'Kconfig'
```

4. Set build configuration. 

```sh
$ cd ../os/tools
$ ./configure.sh artik053/rt-ocf
```

5. build TizenRT.

```sh
$ cd ../
$ make
```
6. Flash to target board

Use the following commands to install the output binaries after connecting an Artik053 board to your host PC.

```sh
$ make download ALL
```
