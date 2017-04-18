/*!
 *  @file       isp_driver_rng.h
 *  @brief      Headerfile : isp driver for rng
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef ISP_DRIVER_RNG_H_
#define ISP_DRIVER_RNG_H_

#include "mb_cmd_rng.h"

// ======================================
// Function
// ======================================
int isp_generate_random(unsigned int *random, unsigned int word_len);

#endif							/* ISP_DRIVER_RNG_H_ */
