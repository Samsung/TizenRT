/*!
 *  @file       isp_driver_flash.h
 *  @brief      Headerfile : isp driver for flash memory
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef DRIVER_FLASH_H_
#define DRIVER_FLASH_H_

#ifndef SSS_POR
#define SSS_POR
#endif

#if defined(SSS_POR)
int SFS_Secure_Write_POR(const char *file_name, unsigned char *src_addr, unsigned int byte_len);
int SFS_Secure_Read_POR(const char *file_name, unsigned char *dst_addr, unsigned int byte_len);

#else
int SFS_Secure_Read(const char *file_name, unsigned int dst_addr, unsigned int *byte_len);
int SFS_Secure_Write(const char *file_name, unsigned int dst_addr, unsigned int byte_len);

#endif /* SSS_POR */

#endif /* DRIVER_FLASH_H_ */

