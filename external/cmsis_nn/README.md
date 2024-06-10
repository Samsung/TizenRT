# CMSIS NN
CMSIS NN software library is a collection of efficient neural network kernels developed to maximize the
performance and minimize the memory footprint of neural networks on Arm Cortex-M processors.
## About
Work In Progress to move CMSIS-NN repository from https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/NN to 
https://github.com/ARM-software/CMSIS-NN. https://github.com/ARM-software/CMSIS_5/issues/1564

## Supported Framework
The library follows the [int8](https://www.tensorflow.org/lite/performance/quantization_spec) and int16 quantization specification of TensorFlow Lite for Microcontrollers.

## Branches and Tags
There is a single branch called 'main'. 
Tags are created during a release. Two releases are planned to be done in a year. The releases can be found
[here](https://github.com/ARM-software/CMSIS-NN/releases) .

## Current Operator Support
In general optimizations are written for an architecture feature. This falls into one of the following categories.
Based on feature flags for a processor or architecture provided to the compiler, the right implementation is picked.
### Pure C
 There is always a pure C implementation for an operator. This is used for processors like Arm Cortex-M0 or Cortex-M3.
### DSP Extension
Processors with DSP extension uses Single Instruction Multiple Data(SIMD) instructions for optimization. Examples of 
processors here are Cortex-M4 or a Cortex-M33 configured with optional DSP extension. 

### MVE Extension
Processors with Arm Helium Technology use the Arm M-profile Vector Extension(MVE) instructions for optimization.
Examples are Cortex-M55 or Cortex-M85 configured with MVE.

 
| Operator        |   C <br> int8 |C<br>int16 | DSP<br>int8 |DSP<br>int16 | MVE<br>int8 |MVE<br>int16
|-----------------|--------|------------------|---------------|-------|-------------|-------|
| Conv2D          | Yes    | Yes              | Yes           | Yes   | Yes         | Yes   |
| DepthwiseConv2D | Yes    | Yes              | Yes           | Yes   | Yes         | Yes   |
| Fully Connected | Yes    | Yes              | Yes           | Yes   | Yes         | Yes   |
| Add             | Yes    | Yes              | Yes           | Yes   | Yes         | No    |
| Mul             | Yes    | Yes              | Yes           | Yes   | Yes         | No    |
| MaxPooling      | Yes    | Yes              | Yes           | Yes   | Yes         | Yes   |
| AvgPooling      | Yes    | Yes              | Yes           | Yes   | Yes         | Yes   |
| Softmax         | Yes    | Yes              | Yes           | Yes   | No          | No   |
| LSTM            | No     | No               | No            | No    | No          | No   |


## Contribution Guideline
First, a thank you for the contribution. Here are some guidelines and good to know information to get started.

### Coding Guideline
By default, follow the style used in the file. You'll soon start noticing a pattern like
* Variable and function names are lower case with an underscore separator.
* Hungarian notation is not used. Well, almost.
* If the variable names don't convey the action, then add comments.

### New Files
One function per file is followed in most places. In those cases, the file name must match the function name. Connect
the function to an appropriate Doxygen group as well.

### Doxygen
Function prototypes must have a detailed comment header in Doxygen format. You can execute the doxygen document generation
script in the Doxygen folder to check that no errors are introduced.

### Unit Tests
For any new features and bug fixes, new unit tests are needed. Improvements have to be veriifed by unit tests. If you do
not have the means to execute the tests, you can still make the PR and comment that you need help in completing/executing
the unit tests.

### Version & Date
Each File has a version number and a date field that must be updated when making any change to that file. The versioning
follows Semantic Versioning 2.0.0 format. 

## Building CMSIS-NN as a library
It is recommended to use toolchain files from [Arm Ethos-U Core Platform](https://review.mlplatform.org/admin/repos/ml/ethos-u/ethos-u-core-platform) project. These are supporting TARGET_CPU, which is a required argument. Note that if not specifying TARGET_CPU, these toolchains will set some default. The format must be TARGET_CPU=cortex-mXX, see examples below.
Furthermore CMSIS-NN has a dependency to [CMSIS](https://github.com/ARM-software/CMSIS_5) project.
Here is an example:

```
cd </path/to/CMSIS_NN>
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=</path/to/ethos-u-core-platform>/cmake/toolchain/arm-none-eabi-gcc.cmake -DTARGET_CPU=cortex-m55 -DCMSIS_PATH=</path/to/CMSIS>
make
```

Some more examples:

```
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/ethos-u-core-platform/cmake/toolchain/arm-none-eabi-gcc.cmake -DTARGET_CPU=cortex-m55 -DCMSIS_PATH=</path/to/CMSIS>
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/ethos-u-core-platform/cmake/toolchain/arm-none-eabi-gcc.cmake -DTARGET_CPU=cortex-m7 -DCMSIS_PATH=</path/to/CMSIS>
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/ethos-u-core-platform/cmake/toolchain/armclang.cmake -DTARGET_CPU=cortex-m3 -DCMSIS_PATH=</path/to/CMSIS>
```

### Compiler Options
Default optimization level is set at Ofast. Please change according to project needs. Just bear in mind this can impact
performance. With only optimization level -O0, *ARM_MATH_AUTOVECTORIZE* needs to be defined for processors with Helium 
Technology.

The compiler option *'-fomit-frame-pointer'* is enabled by default at -O and higher. When no optimization level is specified,
you may need to specify '-fomit-frame-pointer'.

The compiler option *'-fno-builtin'* does not utilize optimized implementations of e.g. memcpy and memset, which are heavily used by CMSIS-NN. It can significantly downgrade performance. So this should be avoided. The compiler option *'-ffreestanding'* should also be avoided as it enables '-fno-builtin' implicitly.

## Support / Contact

For any questions or to reach the CMSIS-NN team, please create a new issue in https://github.com/ARM-software/CMSIS-NN/issues