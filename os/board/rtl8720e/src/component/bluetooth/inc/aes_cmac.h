/**
 * Copyright (c) 2015, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _AES_CMAC_H_
#define _AES_CMAC_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/*aes_cmac input and out are both BIG_ENDIAN,
for little endian usage, please use aes_cmac function instead.*/
void AES_CMAC(unsigned char *key, unsigned char *input,
			  int length, unsigned char *mac);

/**
   aes_cmac input and out are both LITTLE_ENDIAN
*/
void aes_cmac(uint8_t key[16], uint8_t *msg,
			  size_t msg_len, uint8_t res[16]);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _AES_CMAC_H_ */
