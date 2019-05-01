# Introduce

Protocol Buffers (a.k.a. `protobuf`) is Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data. TizenRT features a port for `protobuf` version 3.5.1, with support for C++ language. 
You can find details of `protobuf` at https://github.com/google/protobuf.

Before you proceed with the rest of the documentation below, please note the following points:

## Note
1. **In case you have just recently installed `gRPC`, and wish to install `Protobuf` to work along with `gRPC`, please ignore this document, and refer to the installation instructions under [Installing Protobuf with gRPC](https://github.com/Samsung/TizenRT/blob/master/external/grpc/README.md#pre-requisites) instead.**

2. **In case you have installed `Protobuf` using the instructions in this document, and wish to additionally install `gRPC`, then please make sure you install the correct version of `protobuf` to use along with `gRPC`. It is not really necessary to un-install this version of protobuf, but you will need to maintain different protobuf versions at different paths, and establish the precedence in the $PATH variable. Therefore, to avoid complicating the build, it is recommended to un-install any previous version of `protobuf` and install the version that you need.**


The following sections detail the steps required to build `protobuf` on TizenRT, and to build and run TizenRT applications that use `protobuf`.

# Build `protobuf` 

`protobuf` takes in specifications for services and messages in a special `proto` file (with file extension `.proto`), and translates these specifications to actual C++ code that can be used by applications.
For the translation to happen, `protobuf` needs a protocol complier called `protoc` to compile the specification `.proto` file. Below are mentioned two approaches for installing `protoc` in your build environment (host OS):

## Installing `protoc` from source

`protoc` can be installed from source by using the link below:

- https://github.com/google/protobuf

## Installing `protoc` release from GitHub

The appropriate `protoc` release can be fetched and installed using the steps below:

- http://google.github.io/proto-lens/installing-protoc.html

 Having installed `protoc` in the build environment, the following steps are detailed for configuring TizenRT's `menuconfig` for building `protobuf`

## Build `protobuf` on TizenRT

Protocol Buffers in menuconfig should be selected under [`External Libraries > protocol buffer`].
Currently, `protobuf` configuration is supported only for applications that use `gRPC`(see `artik053/grpc` under `build/configs` directory). In the future, `protobuf` configurations will be extended to cover applications that
do not strictly require `gRPC`.

The following sections describes how to quickly verify the working of `protobuf` on TizenRT, and how to build a new application that uses `protobuf`.

# Quick Start

TizenRT provides a sample program for protobuf, named `addressbook_main` that serializes and deserializes user data.
`addressbook_main` program could be selected under below menuconfig

[`Application Configuration > Examples > Protocol Buffers example`]

Having selected the application as shown above, build it as follows:
```
$ make
```
 

# Add new application for protocol buffer

In general, steps for adding new application to TizenRT is explained in this link: https://github.com/Samsung/TizenRT/blob/master/apps/HowtoAddNewApp.md.
More specifically, applications using `protobuf` require the additional build options:
1. Application should point the header file path for protobuf, as shown:
```
CXXFLAGS  += -I$(TOPDIR)/../external/protobuf/src/
```

2. TizenRT doesn't support RTTI now so a `no rtti` option should be set accordingly, as shown.
```
CXXFLAGS  += -DGOOGLE_PROTOBUF_NO_RTTI -D__TizenRT__
```

Interested developers can refer to `example/protbuf/Makefile` for additional details.
