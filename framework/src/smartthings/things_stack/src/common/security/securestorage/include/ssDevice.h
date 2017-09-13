/**
 * \file    ssDevice.h
 * \brief    reading for device unique value
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Chul Lee
 * Dept : Security/Cloud Computing Lab/Convergence Solution Team/SW Center
 * Creation date : 2012/08/27
 */


#ifndef _SECURE_STORAGE_DEVICE_H_
#define _SECURE_STORAGE_DEVICE_H_

// #ifdef __cplusplus
// extern "C" {
// #endif

/**
 * Callback to read device unique ID.
 *
 * @param    pIdBuf            [out] ID buffer
 * @param   pIdBufSize      [in] byte length of pIdBuf size
 * @param    pIdOutLen        [out] Length of CPU ID
 * @return    SS_SUCCESS        if no error is occured
 */
typedef int (*GetDeviceUniqueID)(unsigned char* pIdBuf, size_t pIdBufSize, unsigned int* pIdOutLen);

/**
 * Set callback implementation to read device unique ID.
 * It will be invoked when generate master key of secure storage
 *
 * @param getIdCB callback implementation to read device unique ID.
 */
extern void ssSetDeviceUniqueIDCallback(GetDeviceUniqueID getIdCB);

/**
 * @brief    API to read device unique ID
 *
 * @param    pId                [out] Device unique ID
 * @param   pIdBufSize      [in] byte length of pId size
 * @param    pIdLen            [out] Length of ID
 * @return    SS_SUCCESS        if no error is occured
 */
extern int ssGetDeviceID(unsigned char* pId, size_t nIdBufSize, unsigned int* pIdLen);

// #ifdef __cplusplus
// }
// #endif

#endif