# Manual Setup Build Environment
## Getting the toolchain

Install the OS specific toolchain. Supported OS Type's are "linux" and "mac".  
Get the build in binaries and libraries, [gcc-arm-none-eabi-6-2017-q1-update-*OS Type*.tar.bz2](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads/6-2017-q1-update)  
Untar the gcc-arm-none-eabi-6-2017-q1-update-*OS Type*.tar.bz2 and export the path like

```bash
tar xvjf gcc-arm-none-eabi-6-2017-q1-update-[OS Type].tar.bz2
export PATH=<Your Toolchain PATH>:$PATH
```
Be aware that recommended toolchain is fully working on 64bits machine.

## Getting the source code

```bash
git clone https://github.com/Samsung/TizenRT.git
cd TizenRT
TIZENRT_BASEDIR="$PWD"
```

## How to Build

Configure the build from *$TIZENRT_BASEDIR/os/tools* directory
```bash
cd os/tools
./configure.sh <board>/<configuration_set>
```
The configuration file is named *defconfig*,  
and resides under the relative path \<board\>/\<configuration_set\> rooted at *build/configs*.  
To check the different \<board\>/\<configuration_set\> combinations supported, type below:
```bash
./configure.sh --help
```

After configuring above, configuration can be modified through *make menuconfig* from *$TIZENRT_BASEDIR/os*.
```bash
cd ..
make menuconfig
```

Refer [kconfig-frontend installation](docs/HowtoInstallKconfigFrontend.md) to use *menuconfig*.

Finally, initiate build by make from *$TIZENRT_BASEDIR/os*.
```bash
make
```

Built binaries are in *$TIZENRT_BASEDIR/build/output/bin*.

See [Clean commands](HowtoClean.md) to clean built files.  
See [Trouble Shooting](TroubleShooting.md) to resolve any issue on TizenRT usages.

## Configuration Sets

To build a TizenRT application, use the default configuration files named *defconfig* under *build/configs/\<board\>/\<configuration_set\>* folder.  
To customize your application with specific configuration settings, using the menuconfig tool is recommended at *os* folder as shown:
```bash
make menuconfig
```
Please keep in mind that we are actively working on board configurations, and will be posting our updates on the README files under each config.
