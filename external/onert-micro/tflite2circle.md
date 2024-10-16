# circle schema

You can find the latest circle schema at https://github.com/Samsung/ONE/blob/master/nnpackage/schema/circle_schema.fbs

# How to convert tflite to circle

## Install one-compiler package

Please follow the instruction in https://pages.github.sec.samsung.net/AIP/ONE_User_Guide/latest/html/getting_started/setup_linux.html

## Convert tflite to circle

Use the following command in shell:

```
onecc import tflite -i <input tflite path> -o <output circle path>
```

# How to convert circle model into C array

You can use the same instruction for tflite model conversion : https://www.tensorflow.org/lite/microcontrollers/build_convert#convert_to_a_c_array
