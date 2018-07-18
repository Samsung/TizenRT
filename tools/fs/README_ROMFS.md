
# Contents for ROMFS

### Prerequisites
Install genromfs  
On Linux, It can be directly installed from package manager using below command
```bash
sudo apt-get install genromfs
```
Otherwise, Install genromfs from source.

1. Getting the sources
```bash
git clone https://github.com/chexum/genromfs.git
```
2. Switch to genromfs 0.5.2 (released) version
```bash
cd genromfs
git checkout 0.5.2 -b 0.5.2
```
3. Patch genromfs with below change
```
diff --git a/Makefile b/Makefile
index d278efc..72a4c5c 100644
--- a/Makefile
+++ b/Makefile
@@ -16,7 +16,7 @@ FILES = COPYING NEWS ChangeLog Makefile \
  readme-kernel-patch genrommkdev romfs.txt \
  checkdist

-prefix = /usr
+prefix = /usr/local
 bindir = $(prefix)/bin
 mandir = $(prefix)/man
```
4. Build and Install genromfs
```bash
make
sudo make install
```

### Steps
1. Enable ROMFS config through menuconfig at *os* folder.
```bash
cd $TIZENRT_BASEDIR/os
make menuconfig
```
See [[Getting the sources]](https://github.com/Samsung/TizenRT#getting-the-sources) for how to set *TIZENRT_BASEDIR*.

Select menu.
```bash
File Systems -> ROMFS to y
```

2. Put your contents at *tools/fs/contents* folder.

After these two steps, board specific steps should be executed.

ARTIK053 [[Details]](../../build/configs/artik053/README.md#romfs)  
ARTIK053S [[Details]](../../build/configs/artik053s/README.md#romfs)  
ARTIK055S [[Details]](../../build/configs/artik055s/README.md#romfs)  
SIDK_S5JT200 [[Details]](../../build/configs/sidk_s5jt200/README.md#romfs)

