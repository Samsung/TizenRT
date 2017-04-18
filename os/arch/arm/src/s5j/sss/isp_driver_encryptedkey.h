/*!
 *  @file       isp_driver_encryptedkey.h
 *  @brief      Headerfile : isp driver for setting encryptedkey
 *  @author     jinsu.hyun
 *  @version    v0.00 : 2017.01.19 Init. release version
 */

#ifndef _ISP_DRIVER_ENCRYPTEDKEY_H_
#define _ISP_DRIVER_ENCRYPTEDKEY_H_

// ======================================
// Function
// ======================================

int isp_set_encryptedkey(unsigned char *key, unsigned int key_byte_len, unsigned int key_type, unsigned char *encryptedkey);
int isp_get_encryptedkey(unsigned int *object_id, unsigned int key_type, unsigned char *encryptedkey);
int isp_set_genkey_encryptedkey(unsigned int key_type, unsigned char *encryptedkey);

#endif							/* _ISP_DRIVER_ENCRYPTEDKEY_H_ */
