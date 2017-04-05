/*!
 *  @file       isp_driver_system.h
 *  @brief      Headerfile : isp driver for system 
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 */

#ifndef DRIVER_ISP_DRIVER_SYSTEM_H_
#define DRIVER_ISP_DRIVER_SYSTEM_H_

#include "mb_cmd_system.h"

// ======================================
// Function
// ======================================
int isp_get_status(void);
int isp_get_info(unsigned int *version);
int isp_clear(unsigned int type);

#endif /* DRIVER_ISP_DRIVER_SYSTEM_H_ */
