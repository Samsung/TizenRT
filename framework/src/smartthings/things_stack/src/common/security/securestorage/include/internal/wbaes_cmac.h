/**
 * \file    wbaes_cmac.h
 * \brief    WhiteBox AES CMAC difinitions.
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Chul Lee
 * Dept : Security/Cloud Computing Lab/Convergence Solution Team/SW Center
 * Creation date : 2012/08/27
 */

#ifndef _WB_AES_CMAC_H
#define _WB_AES_CMAC_H

////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
//#include "CC_API.h"


////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

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
int WBAES_CMAC(unsigned char *msg, unsigned int msgLen, unsigned char *output, unsigned int *outputLen);

#ifdef __cplusplus
}
#endif

#endif // _CMAC_H

/***************************** End of File *****************************/
