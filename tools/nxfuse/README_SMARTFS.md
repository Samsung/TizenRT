# How to make smartfs user image for TizenRT

## Contents
> [How to Enable SmartFS user image](#how-to-enable-smartfs-user-image)  
> [APPENDIX](#appendix)  

## How to Enable SmartFS user image
Enable below configurations as followings

### Configurations
SMARTFS file system(CONFIG_FS_SMARTFS) needs to be enabled in order to use SMARTFS image

```
-> File Systems
	->SMART file system (=y)
```

CRC flag is must

```
->File Systems
	-> Memory Technology Device (MTD) Support
		-> SMART Device options
			->  Enable "Enable Sector CRC error detection" (=y)
```

### SmartFS Partition size

Developer needs to align below 4 varibles to create a perfect image in file
$TIZENRT_BASEDIR/tools/nxfuse/mksmartfsimg.sh

```
blkcount
blksize
pagesize
erasesize
```

#### How to set the blkcount & blksize variables?
Below menuconfig path would fetch the smartfs *partition size in KB's*
Developer need to map the index of partition size w.r.t "Flash partition type list"

```
->Board Selection
	->Enable partition support on FLASH
		(2048,256,256,256,512,512, << 2048 >>) Flash partition size list (in KBytes)
		(kernel,none,bin,bin,bin,bin,<< smartfs >>,) Flash partition type list
		(kernel,app,micom,micom,wifi,wifi,<< userfs >>,) Flash partition name list
```

Developer can modify this "2048" to suit their requirements
blkcount multiplied by blksize should be equal to SMARTFS partition size

```
For Ex:
	If partition size is "2048", then blksize=512 & blkcount=4096 [(512 X 4096)/1024] = 2048
	If partition size is "1024", then blksize=512 & blkcount=2048 [(512 X 2048)/1024] = 1024
```

#### How to set the pagesize & erasesize variables?
For pagesize & erasesize, Its depend on chipset and FLASH memory.
So it may vary for each chipset and products.
we need to refer the FLASH memory spec file shared by vendors or request vendor share this info

### Where to place user contents?
Place the user files to be loaded in path, "$TIZENRT_BASEDIR/tools/fs/contents-smartfs/<CONFIG_ARCH_BOARD>/base-files/" (CONFIG_ARCH_BOARD--> refer os/.config)

```
For Ex:
	If board name is "CONFIG_ARCH_BOARD=imxrt1020-evk",
	then the user base-files path would be "$TIZENRT_BASEDIR/tools/fs/contents-smartfs/imxrt1020-evk/base-files/"
```

### Command to create smartfs image

```bash
$TIZENRT_BASEDIR/os# ./dbuild.sh smartfs
```

### Binarypath
The final image of bin file named "<CONFIG_ARCH_BOARD>_smartfs.bin" would be in "$TIZENRT_BASEDIR/build/output/bin/" path

```
For Ex:
	If board name is "CONFIG_ARCH_BOARD=imxrt1020-evk",
	then the bin file would be named as "$TIZENRT_BASEDIR/build/output/bin/imxrt1020-evk_smartfs.bin"
```

### Command to flash smartfs image to device

```bash
$TIZENRT_BASEDIR/os# ./dbuild.sh download userfs
```

## APPENDIX

### About NxFuse tool

A Linux FUSE implmentation that enables native mounting of TizenRT filesystems
in Linux.  Currently the implementation supports SmartFS.  The
Some of the Linux utilities depend on this capability (overwriting an existing file
with new content for instance) and may fail.

To build nxfuse, you must first install the libfuse-dev and fuse library as root

```bash
sudo apt-get install libfuse-dev
sudo apt-get install fuse

sudo yum install libfuse-dev
sudo yum install fuse
```

Then just type 'make' within the smartfuse directory.  This project does
not contain an autoconf configuration script.  It was developed under
Ubuntu and so it may need slight modifications for other distributions.

To compile correctly, all of the the TizenRT FS and support code that has
been copied from the TizenRT source directory depends on a valid config.h file
to define the system configuration.  The make system automatically copies
this config file from the Tinyara directory $(TINYARADIR) of a pre-configured
NuttX build into the nxfuse/include/nuttx directory.  If the TINYARADIR variable
is not define, the Makefile will use the default value of "../../os",
which makes the assumption that the NuttX tools repo is located at the same
directory level as the main "nuttx" source repo.

To enable / disable SmartFS features (wear leveling, CRC, etc.) in the
nxfuse implementation, run the 'make menuconfig' routine from within the nuttx
directory and select the features required.  Then run 'make context' within
the nuttx directory.  The newly generated tinyara/include/tinyara/config.h file
will be copied to the tools/nxfuse/include/tinyara directory when the nxfuse is
next built.


### How to use nxfuse

To use nxfuse, invoke it directly from within the nxfuse directory or run the
'make install' command as root (or sudo) to install it in /usr/bin.  The
invocation format for nxfuse is:

```bash
nxfuse [options] [-t fstype] [fuse_options] mount_dir data_source
```

The mount_dir must already exist in the Linux filesystem as a normal directory.
For details about the options, see the man page (automatically installed by the
'make install' target).  To view the man page without installing it, simply
use the command 'man -l man/nxfuse.1'.  The default options for nxfuse are:

   fstype:       smartfs
   erasesize:    Set by CONFIG_FILEMTD_ERASESIZE in the config file
   pagesize:     Set by CONFIG_FILEMTD_BLOCKSIZE in the config file
   logsectsize:  Set by CONFIG_MTD_SMART_SECTOR_SIZE in config file

An example invocation might be:

```bash
./nxfuse -t smartfs /tmp/fuse /tmp/smartfs_data_file.bin
```

For a list of fuse_options that are supported, add the -h option to the command
line as follows:

```bash
./nxfuse -h /tmp/fuse /tmp/smartfs_data_file.bin
```

When using nxfuse with a block device, such as /dev/sdb, etc. the nxfuse
command will likely need to be executed with 'sudo' access.  Doing this will
cause the resuling mount (e.g. /tmp/fuse) to be inaccesable to anyone but
root.  To get around this, the fuse_option "-o allow_other" can be used on the
nxfuse command line.  This will cause the mounted directory to be setup
with "rwxrwxrwx" permissions, allowing any user to access the mount.  As an
example:

```bash
mkdir /tmp/fuse
sudo losetup /dev/loop0 /tmp/smartfs_data_file.bin
sudo ./nxfuse -o allow_other -t smartfs /tmp/fuse /dev/loop0
ls -l /tmp/fuse
```

To unmount a FUSE device, use the fusermount command as follows:

```bash
   fusermount -u /tmp/fuse
```

### Formatting and Reformatting

The nxfuse command also has the ability to create (or re-create) a new
fiilesystem within the data file (a built-in 'mkfs').  This is done by
specifying the -m option on the command line (-m for mkfs).  When invoked
with the -m option, the mount_point argument should be ommited, such as:

```bash
./nxfuse -t smartfs -m /tmp/emptyfile
```

If the target file already contains a valid filesystem, then nxfuse will
report an error and request the addition of the -c option for confirmation
of the re-format.  After the format is complete, the newly created filesystem
is not mounted ... mounting must be performed as detailed above as a
secondary step.
