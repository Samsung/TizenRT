# Compression driver

This driver APIs are used to access compression and decompression functions of compression module.
The ioctl function of the compress driver supports following commands and accordingly perform 
following tasks .

## 1. COMPIOC_COMPRESS cmd :

For this cmd, user needs to pass a struct compress_header pointer as arg to the ioctl and need to store
the members of the struct as below : <br />
input_size     ->  input size of the data <br />
input_buffer   ->  input data <br />
output_size    ->  should be same as input size because after compression, output size will be less than or equal to input size <br />
output_buffer  ->  should be allocated before passing to ioctl function 

The ioctl function will call the compression function and compressed data will be stored in 
output_buffer and output_size will get updated to the length of compressed data. 

## 2. COMPIOC_GET_COMP_TYPE cmd :

For this cmd, user can pass arg as NULL because there is no need of an argument. The ioctl will return 
the compression type.

## 3. COMPIOC_GET_COMP_NAME cmd :

For this cmd, user needs to pass a char pointer as arg that needs to be allocated memory with a size of
COMP_NAME_SIZE (compression.h). This should be allocated memory before passing to the ioctl.
The ioctl function will store the compression type name string in the char pointer.

## 4. COMPIOC_DECOMPRESS cmd :

For this cmd, user needs to pass a struct compress_header pointer as arg to the ioctl and need to store
the members of the struct as below : <br />
input_size      -> input size of compressed data <br />
input_buffer    -> compressed data <br />
output_buffer   -> should be allocated before passing to ioctl function <br />
output_size     -> three cases are possible for output size which is discussed below 

- ##### when output size passed is equal to actual decompressed data size : <br />
        This is the case where user knows the size of decompressed data. The ioctl function will call the
        decompress function and it will store the decompressed data in output_buffer and output_size will 
        remain same.
    
- ##### when output size passed is more than actual decompressed data size : <br />
        This is the case where user does not know the size of decompressed data and passes the output size 
        larger than required for output buffer. The ioctl function will call the decompress function and it will
        store the decompressed data in output buffer and update the output_size as the actual decompressed 
        data size .Here, output_size after the ioctl call will be less than before.

- ##### when output size passed is less than actual decompressed data size : <br />
        This is the case where user does not know the size of decompressed data and passes the output size
        smaller than required for output buffer. The ioctl call will return ENOMEM (out of memory) error 
        and it will print that output buffer allocated is not sufficient. 
        User need to update the output_size to a larger value and allocate memory to output_buffer 
        and call ioctl function again to get full decompressed data.

=================================================================================================
The below ioctls are not thread safe due to the limitation of underlying decompression framework.
As a result only one thread can perform the decompression operation at a time.
Each decompression operation consists of init -> get buffer size -> decompress -> deinit sequence.
When one thread is performing the decompression sequence, if other thread tries to start a new
decompression, then the INIT ioctl will return an EBUSY error.
The calling thread will need to retry after some time duration.

App shall use the ioctls as follows to perform the decompression.
==================================================================================================

## 5. COMPIOC_FCOMP_INIT cmd :

For this cmd, user needs to pass the file path as arg and this compress_init function initializes the
decompression framework in kernel. The ioctl will return OK on successful init otherwise error code.
If error, app should goto errout which calls COMPIOC_FCOMP_DEINIT cmd otherwise continue to next step.

## 6. COMPIOC_FCOMP_GET_BUFSIZE cmd :

For this cmd, user can pass arg as NULL because there is no need of an argument. The ioctl returns the buffer
size required to hold decompress data. User needs to allocate buffer of this size to get decompress data in it.
If error or size returned is less than equal to zero, app should goto errout which calls COMPIOC_FCOMP_DEINIT cmd
else continue to next step.

## 7. COMPIOC_FCOMP_DECOMPRESS cmd :

For this cmd, user needs to pass the buffer of size which got from COMPIOC_FCOMP_GET_BUFSIZE cmd and
compress_read() decompress the compressed file and store the data in that passed buffer. It returns the success
code when size of decompress data and actual size of data is same otherwise error code.
If error, app should goto errout which calls COMPIOC_FCOMP_DEINIT cmd otherwise continue to next step.

## 8. COMPIOC_FCOMP_DEINIT cmd :

For this cmd, user can pass arg as NULL because there is no need of an argument. It cleanup the kernel decompression
framework.
