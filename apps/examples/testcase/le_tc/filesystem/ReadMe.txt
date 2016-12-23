Readme -> fs_sample
~~~~~~~~~~~~~~~~~~~~

This is a sample application to perform File System Operations.

Steps:
1. Initiate File System
2. Perform File I/O Operation
3. Unload File System

We can erase Flash wheneven required.

List of APIs:
~~~~~~~~~~~~~
1. To Initiate File System:
int fs_initiate(char* block_device, char* fs_name)
Example:
    fs_initiate("/dev/smart1", "smartfs")

2. To Erase Flash:
int fs_erase(char* block_device)
Example:
    fs_erase("/dev/smart1")

3. To Unload FS from RAM:
int fs_clean(char* block_device)
Example:
    fs_clean("/dev/smart1")