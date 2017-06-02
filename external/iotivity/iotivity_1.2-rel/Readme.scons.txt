== Quick guide: build and run IoTivity projects on Ubuntu ==

1. Build
    Go to the top directory of 'iotivity' project(Note: should always run 'scons'
    command in this directory)

    Install build tools:
      $ sudo apt-get install git-core scons ssh build-essential g++ doxygen valgrind

    Install external libraries:
      $ sudo apt-get install libboost-dev libboost-program-options-dev libboost-thread-dev uuid-dev libssl-dev libtool libglib2.0-dev

    Build release binaries:
      $ scons
(Note: C sdk requires tinycbor. Please follow the instruction in the build
message to install tinycbor)

    Build debug binaries:
      $scons RELEASE=false

    Help:
      $ scons -h

    Clear:
      $ scons -c

2. Run the samples
      $ export LD_LIBRARY_PATH=<iotivity>/out/linux/x86_64/release
      Run the c++ samples in <iotivity>/out/linux/x86_64/release/resource/examples
      Run the c samples in <iotivity>/out/linux/x86_64/release/resource/csdk/stack/samples/linux/SimpleClientServer
('<iotivity>' is the path to 'iotivity' project. If your device is x86, arm,
or arm64, please change 'x86_64' to the proper arch)

3. To build and test IoTivity with Security enabled (required for certification)
follow the instructions found in:
  <iotivity>/resource/csdk/security/README-building-and-running-secure-IoTivity-stack.txt

== How to build IoTivity projects ==

IoTivity includes a series of projects. You can find all these projects here:
    https://gerrit.iotivity.org/gerrit/#/admin/projects/

You can build IoTivity project on Linux / Windows / MAC OSX for various OS(
Linux, Tizen, Android, Arduino, Windows, MAC OSX, IOS ...).
The output of the build is in:
  <top directory of the project>/out/<target_os>/<target_arch>/<build_type>/
e.g.
	iotivity/out/android/armeabi-v7a/release/.

This document takes 'iotivity' project as example, the way to build other
projects is almost the same.

=== IoTivity project build tool scons ===

Scons is a cross-platform build tool, its usage is quite similar to GNU make.
To build a project, you just require to run following command at the directory
where a SConstruct file exists(SConstruct is the entrance of scons build, it's
equivalent to Makefile of 'make') :
      $ scons [options] [target]

In additional, usually the scons build script of a project provides useful help
information(include build options). To see the help information:
      $ scons [options] -h

Note: If no value is specified for an option, the default value will be used.
The change of options value may impact the help information and the behavior
of the building.

Generally, it's required to specify the target OS and target ARCH, that's to say
tell Scons which OS and which ARCH you'd like build this project for. By default,
the target OS and ARCH is the same as the host.

Some more options may be required, please care the 'error' notification when build.
For help about how to set an option, please run:
     $ scons TARGET_OS=xxx TARGET_ARCH=yyy [XXX=zzz ...] -h


=== Prerequites ===

* 1. Scons

Please refer to the following page to install scons:
   http://www.scons.org/doc/production/HTML/scons-user.html#chap-build-install
(Note: on Windows, install Python 2.x before installing scons)

* 2. IDE/SDK Prerequites
To build for some OS (Android / Arduino / IOS ...), an IDE/SDK may be required,
please go to the relative page to download and install the required IDE/SDK.

Android:
To build for Android, Andorid NDK and SDK are required.
  Android NDK: http://developer.android.com/tools/sdk/ndk/index.html
  Android SDK: http://developer.android.com/sdk/index.html
(Note: as in some IoTivity projects, C++11 features are used, recommend Android
 NDK >= r10)

Arduino:
To build for Arduino, Arduino IDE is required.
  Arduino IDE: http://arduino.cc/en/Main/Software
  (Note: recommend install Arduino IDE >=1.5.8)

Arduino builds are dependent on latest Time library. Download it from here:
    http://www.pjrc.com/teensy/td_libs_Time.html
and extract to <arduino_ide_root>/libraries/

(Note: If you are using Arduino IDE 1.5.8 BETA on Windows, it may pop up some
dll isn't found. please copy relative dll from the IDE directory to
C:\Windows\SysWOW64. IDE 1.5.7 doesn't have this issue. Other version IDE isn't
tested.)

Apple:
To build for Mac OSX or IOS, Xcode is required.
  Xcode: https://developer.apple.com/xcode/downloads/

Java:
To build the Java code, JDK is required.
  JDK: http://www.oracle.com/technetwork/java/javase/downloads/index.html
(If the project doesn't include Java code or you wouldn't like build the
Java codes, this isn't required)

(Note: for convenience, suggest add the IDE/SDK path in environment variable,
so you don't need to add it in command line each time. The build script will
guide you to do that.)

Tizen:
To build for tizen platform tinycbor library is needed.
Please download tinycbor if it is not present in extlibs/tinycbor folder
by doing the following:
	$ git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor


* 3. External libraries
IoTivity project depends on some external libraries, such as boost ...
During building, the existence of external library will be checked, if it doesn't
exist, the build script will try to download, unpack and build the library or
notify user to install it.

Downloading and unpacking may fail due to network problem or required unpacking
tool isn't installed. An message will be displayed, please follow the message
to skip it.


=== Build IoTivity project ===

Linux:
 * Possible values for <TARGET_TRANSPORT> are: ALL, IP, BLE

1. Go to root directory
    $ cd <top directory of the project>
    $ sudo apt-get install libboost-dev libboost-thread-dev libssl-dev libtool

2. Execute following command(s) to start build based on transport selection required

    -> Building for all transports :
    $ scons TARGET_OS=linux TARGET_TRANSPORT=ALL

    -> Building for a specific transport :
    $ scons TARGET_OS=linux TARGET_TRANSPORT=IP

    -> Building for multiple transports :
    $ scons TARGET_OS=linux TARGET_TRANSPORT=IP,BLE TARGET_ARCH=xxx

    -> Clean Build (all transports) :
    $ scons TARGET_OS=linux TARGET_TRANSPORT=ALL -c (for clean)

Android:
 * Possible values for <TARGET_TRANSPORT> are: ALL, IP, BT, BLE
 * Possible values for <TARGET_ARCH> are: x86, armeabi, armeabi-v7a, armeabi-v7a-hard
   (To see all of its allowed value, please execute command 'scons TARGET_OS=android -Q -h')

1. Go to root directory
    $ cd <top directory of the project>

2. Execute following command(s) to start build based on transport selection required

    -> Building for all transports :
    $ scons TARGET_OS=android TARGET_TRANSPORT=ALL TARGET_ARCH=xxx

    -> Building for a specific transport :
    $ scons TARGET_OS=android TARGET_TRANSPORT=IP TARGET_ARCH=xxx

    -> Building for multiple transports :
    $ scons TARGET_OS=android TARGET_TRANSPORT=IP,BT,BLE TARGET_ARCH=xxx

    -> Clean Build (all transports) :
    $ scons TARGET_OS=android TARGET_TRANSPORT=ALL -c (for clean)

Tizen:
 * Possible values for <TARGET_TRANSPORT> are: ALL, IP, BT, BLE

 1. Go to root directory
    $ cd <top directory of the project>

 2. Execute following command(s) to start build based on transport selection required

    -> Building for all transports :
    $ scons TARGET_OS=tizen TARGET_TRANSPORT=ALL

    -> Building for a specific transport :
    $ scons TARGET_OS=tizen TARGET_TRANSPORT=IP

    -> Building for multiple transports :
    $ scons TARGET_OS=tizen TARGET_TRANSPORT=IP,BT,BLE TARGET_ARCH=xxx

    -> Clean Build (all transports) :
    $ scons TARGET_OS=tizen TARGET_TRANSPORT=ALL -c (for clean)

(we provide the spec file required by gbs tool at toools/tizen directory.
gbs is default build tool for Tizen platform, we can refer the following
wiki to setup Tizen development environment:
https://source.tizen.org/documentation/developer-guide/getting-started-guide)

Arduino:
 * Possible values for <TARGET_TRANSPORT> are: IP, BLE
 * Possible values for <TARGET_ARCH> are: arv, arm
    (arv: arduino due, arm: arduino mega)
 * Possible values for <BOARD> are: arduino_due_x_dbg, arduino_due_x, mega
 * Possible values for <SHIELD> are: ETH, WIFI
    (It is required in the case of the arduino due.)

 1. Go to root directory
    $ cd <top directory of the project>
    $ sudo apt-get install dos2unix

 2. Execute following command(s) to start build based on transport selection required

    -> Building for a specific transport :
    $ scons TARGET_OS=arduino TARGET_ARCH=xxx BOARD=yyy SHIELD=zzz

(To get allowed values run: scons TARGET_OS=arduino TARGET_ARCH=xxx SHIELD=zzz -h.
You may see a option 'CPU' in the output of above command line, that's due to
some boards have different processors, to specify the processor, add 'CPU=zzz'
in the command line. If no 'CPU' option exists, that means the board only
support one kind of processor, it's unnecessary to specify it)

Mac OSX:
 * Possible values for <SYS_VERSION> are: OSX version, e.g. 10.9

 1. Go to root directory
    $ cd <top directory of the project>

 2. Execute following command(s) to start build based on transport selection required

    -> Building for a specific transport :
    $ scons SYS_VERSION=yyy

IOS:
 * Possible values for <TARGET_ARCH> are: i386, x86_64, armv7, armv7s, arm64
 * Possible values for <SYS_VERSION> are: IOS version, e.g. 7.0

 1. Go to root directory
    $ cd <top directory of the project>

 2. Execute following command(s) to start build based on transport selection required

    -> Building for a specific transport :
    $ scons TARGET_OS=ios TARGET_ARCH=xxx SYS_VERSION=yyy

Windows:
 * Possible values for <TARGET_ARCH> are: amd64

For convenience to build projects supported on Windows a batch file (run.bat) is provided
to run many build combinations with TARGET_OS to 'windows'.

1. Go to root directory
    $ cd <top directory of the project>
2. To clean before building:
      $ run clean
3. To build debug amd64 binaries:
      $ run build
See run.bat for more example usage parameters

* Additional options
 * VERBOSE=true or false (Show compilation)
 * RELEASE=true or false (Build for release?)
 * LOGGING=true or false (Enable stack logging)
 * SECURED=1 or 0 (Build with DTLS)
 * TEST=1 or 0 (Run unit tests)
 * BUILD_SAMPLE=ON or OFF (Build with sample)
 * ROUTING=GW or EP (Enable routing)
 * WITH_TCP=true or false (Enable CoAP over TCP Transport, arduino is not supported)
 * WITH_RA=true or false (Build with Remote Access module)
 * RD_MODE=CLIENT or SERVER (Build including Resource Directory)
 * SIMULATOR=true or false (Build with simulator module)
 * Possible values for <WITH_MQ> are: PUB,SUB,BROKER (Build including Message Queue)
   -> PUB : publisher, SUB : subscriber, BROKER : MQ broker(not supported yet)

Note:
1) for convenience, a script (auto_build.sh) is provided to run possible build
at once. Following is the usage:

To build:
     $ auto_build.sh <path-to-android-ndk>
To clean:
     $ auto_build.sh -c
