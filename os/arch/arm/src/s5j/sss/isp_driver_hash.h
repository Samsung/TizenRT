/*!
 *  @file       isp_driver_hash.h
 *  @brief      Headerfile : isp driver for hash
 *  @author     jinsu.hyun
 *  @version    v0.01 : 2015.7.7
 *  @version    v0.10 : 2016.7.15 pre-beta release
 *  @version    v0.20 : 2016.7.22 support msg struct
 *  @version    v0.50 : 2016.8.13 Init. release version
 */


#ifndef ISP_DRIVER_HASH_H_
#define ISP_DRIVER_HASH_H_

#include "mb_cmd_hash.h"

// ======================================
// Function
// ======================================
int isp_hash(unsigned char * hash, struct sHASH_MSG * hash_msg, unsigned int object_id);

#endif /* ISP_DRIVER_HASH_H_ */
