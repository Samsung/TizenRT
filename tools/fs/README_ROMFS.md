
# Contents for ROMFS

### Prerequisites
```bash
sudo apt-get install genromfs
```
### Steps
1. Enable ROMFS config through menuconfig at *os* folder.
```bash
cd $TIZENRT_BASEDIR
make menuconfig
```

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

