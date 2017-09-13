/**
 * \file    ssAPI.h
 * \brief    Header file of Secure storage API
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Chul Lee
 * Dept : Security/Cloud Computing Lab/Convergence Solution Team/SW Center
 * Creation date : 2012/08/27
 */


#ifndef _SECURE_STORAGE_HANDLER_H_
#define _SECURE_STORAGE_HANDLER_H_

#include <stdio.h>
#include <stdint.h>
#include "ssDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * API to initialize secure storage
 *
 * @return 0 is success, otherwise error.
 */
int InitSecureStorage();

/**
 * API to deinitialize secure storage
 *
 * @return 0 is success, otherwise error.
 */
int DeinitSecureStorage();


/** Persistent storage open handler with secure storage. */
FILE* secure_fopen(const char *path, const char *mode);

/** Persistent storage read handler with secure storage.*/
size_t secure_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

/** Persistent storage write handler with secure storage.*/
size_t secure_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/** Persistent storage close handler with secure storage. */
int secure_fclose(FILE *fp);


#ifdef __cplusplus
}
#endif

#endif
