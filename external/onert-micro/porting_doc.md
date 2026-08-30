# Porting Documentation OneRT Micro Library
Parent repository: https://github.com/Samsung/ONE

## OneRT Micro 2.0 

Reference : https://github.ecodesamsung.com/TizenRT/AIFW/pull/114/commits/45af6078a988437ad872d87e843eec68dc28fa0a

Files excluded
1. dir - onert-micro/luci-interpreter/ (Not required now)

<hr>

## OneRT Micro

Commit: 0f3365a0f9271136c722fb03a41d71555e2e8370

Files excluded (all just unnecessary):
1. dir - src/loader/nodes
2. dir - pal/linux
3. All MemoryManagers from src/memory_managers except SimpleMemoryManager
4. All CMakefiles
5. All kernels from src/kernels except listed in pal/mcu/KernelsToBuild.lst

Some notes:
1. To avoid building error with expm1 math function, Elu kernel is disabled as in pal/mcu/KernelsToBuild.lst as in Makefile
2. When we use both tflite and onert-micro:
    If you use the external directory as it is presented in the repository (for commit 0f3365a0f9271136c722fb03a41d71555e2e8370),
    an error occurs, since the compiler tries to use the path to the flatbuffer from the tflite directory.
    To avoid this, we change the path `#include "flatbuffer /..."` everywhere to `#include "third_party/flatbuffer /..."`
    and specify the corresponding path in the Makefile. This change affects only the third_party folder.
3. pal/cmsisnn folder is removed since it is disabled and produces AVAS error
