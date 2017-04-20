/*!
 *  @file       isp_driver_secure_storage.h
 *  @brief      Headerfile : isp driver for secure storage
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.60 : 2016.12.13 Add for secure_storage_map function
 */

#ifndef ISP_DRIVER_SECURE_STORAGE_H_
#define ISP_DRIVER_SECURE_STORAGE_H_

// ======================================
// Struct
// ======================================
//! @struct SECURE_STORAGE_MAP
//! @brief struct of secure storage parameter
struct SECURE_STORAGE_MAP {
	unsigned int offset;
	unsigned int index_max;
	unsigned int key_byte_len_max;
	unsigned int slot_byte_len;
	unsigned int type;
};

#define KEY_PATH_BASE								"/mnt"
//#define KEY_PATH_BASE                             "/mnt/sss"

#define SECURE_STORAGE_TYPE_FACTORYKEY_KEY			(0x00)
#define SECURE_STORAGE_TYPE_FACTORYKEY_DATA			(0x01)

#define SECURE_STORAGE_TYPE_KEY_AES 				(0x0A)
#define SECURE_STORAGE_TYPE_KEY_HMAC 				(0x0B)
#define SECURE_STORAGE_TYPE_KEY_RSA 				(0x0C)
#define SECURE_STORAGE_TYPE_KEY_DH 					(0x0D)
#define SECURE_STORAGE_TYPE_KEY_ECC 				(0x0E)
#define SECURE_STORAGE_TYPE_DATA 					(0x10)
#define SECURE_STORAGE_TYPE_CERT 					(0x11)

#define FACTORYKEY_ARTIK_PSK 						(0x80000100)
#define FACTORYKEY_ARTIK_DEVICE						(0x00010120)
#define FACTORYKEY_ARTIK_CERT						(0x00010122)
#define FACTORYKEY_DA_CA							(0x00081110)
#define FACTORYKEY_DA_DEVICE						(0x00081210)
#define FACTORYKEY_DA_PBKEY							(0x00081211)
#define FACTORYKEY_IOTIVITY_ECC						(0x00011120)
#define FACTORYKEY_IOTIVITY_ECC_CERT 				(0x00011122)
#define FACTORYKEY_IOTIVITY_SUB_CA_CERT				(0x00011222)

// ======================================
// Function
// ======================================
int read_secure_storage(unsigned int type, unsigned int index, unsigned int data_offset, unsigned int src_addr, unsigned int byte_len);
int write_secure_storage(unsigned int type, unsigned int index, unsigned int data_offset, unsigned int src_addr, unsigned int byte_len);
int secure_storage_map(unsigned int type, struct SECURE_STORAGE_MAP *map);

#endif							/* ISP_DRIVER_SECURE_STORAGE_H_ */
