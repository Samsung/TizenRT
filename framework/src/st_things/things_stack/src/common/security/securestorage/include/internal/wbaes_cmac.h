/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _WB_AES_CMAC_H
#define _WB_AES_CMAC_H

////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {


#endif	/* 
 */

#define WBCAMC_BUFFER_SIZE 512
#define AES_BLOCK_SIZE    16

/*!    \brief    generate WhiteBox AES CMAC code
 * \param    ctx                            [in]crypto parameter
 * \param    Key                            [in]user key
 * \param    KeyLen                        [in]byte-length of Key
 * \param    msg                            [in]data block
 * \param    msgLen                        [in]byte-length of Text
 * \param    output                        [out]generated MAC
 * \param    outputLen                    [out]byte-length of output
 * \return    CRYPTO_SUCCESS                if no error is occured
 */
int WBAES_CMAC(unsigned char *msg, unsigned int msg_len, unsigned char *output, unsigned int *output_len);



#ifdef __cplusplus
}
#endif	/* 
 */

#endif							// _CMAC_H
