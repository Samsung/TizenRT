/**
 * \file    wbaes_cmac.c
 * \brief    WhiteBox AES CMAC implementation.
 *
 * - Copyright : Samsung Electronics CO.LTD.,
 *
 * \internal
 * Author : Chul Lee
 * Dept : Security/Cloud Computing Lab/Convergence Solution Team/SW Center
 * Creation date : 2012/08/27
 */

////////////////////////////////////////////////////////////////////////////
// Include Header Files
////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ssDefines.h"

#include "SWAPv2.h"
#include "SWAPv2_defines.h"

#include "wbaes_cmac.h"
#include "wbaes_cmacDefines.h"



/* For CMAC Calculation */
static unsigned char Rb[16] = 
{ 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
};

static unsigned char ZERO[16] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/*****************************/
/**** Function Prototypes ****/
/*****************************/

static void XOR128(unsigned char *a, unsigned char *b, unsigned char *out)
{
    int i;
    for (i=0;i<16; i++)
    {
        out[i] = a[i] ^ b[i];
    }
}

static void LShiftOnebit(unsigned char *input,unsigned char *output)
{
    int         i;
    unsigned char overflow = 0;

    for ( i=15; i>=0; i-- ) {
        output[i] = input[i] << 1;
        output[i] |= overflow;
        overflow = (input[i] & 0x80)?1:0;
    }
    return;
}

static void GenerateSubKey(unsigned char *K1, unsigned char *K2)
{
    unsigned char L[16];
    unsigned char Z[16];
    unsigned char tmp[16];
    int i;

    for (i = 0; i < 16; i++) Z[i] = 0;

    SWAPv2_Process(WB_Table, TABLE_LENGTH, Z, L);

    if(( L[0] & 0x80 ) == 0) 
    {
        LShiftOnebit(L, K1);
    }
    else
    {
        LShiftOnebit(L, tmp);
        XOR128(tmp, Rb, K1);
    }

    if(( K1[0] & 0x80 ) == 0)
    {
        LShiftOnebit(K1, K2);
    } 
    else 
    {
        LShiftOnebit(K1, tmp);
        XOR128(tmp, Rb, K2);
    }
    return;
}



static void Padding(unsigned char *lastb, unsigned char *pad, int length)
{
    int j;

    /* original last block */
    for(j = 0; j < 16; j++)
    {
        if (j < length)
        {
            pad[j] = lastb[j];
        }
        else if(j == length)
        {
            pad[j] = 0x80;
        } 
        else
        {
            pad[j] = 0x00;
        }
    }
}

static void WBAES_CMAC_internal(unsigned char *input, int length, unsigned char *mac )
{
    unsigned char       X[16],Y[16], M_last[16], padded[16];
    unsigned char       K1[16], K2[16];
    int         n, i, flag;
    GenerateSubKey(K1, K2);

    n = (length + 15) / 16;       /* n is number of rounds */

    if(n == 0)
    {
        n = 1;
        flag = 0;
    }
    else
    {
        if(( length % 16 ) == 0)
        { /* last block is a complete block */
            flag = 1;
        }
        else 
        { /* last block is not complete block */
            flag = 0;
        }
    }

    if(flag) 
    { /* last block is complete block */
        XOR128(&input[ 16 * (n - 1) ], K1, M_last);
    }
    else
    {
        Padding(&input[ 16 * (n - 1) ], padded, length % 16);
        XOR128(padded, K2, M_last);
    }

    for(i = 0; i < 16; i++) 
    {
        X[i] = 0;
    }
    for(i = 0; i < n - 1; i++)
    {
        XOR128(X, &input[16 * i], Y); /* Y := Mi (+) X  */
        SWAPv2_Process(WB_Table, TABLE_LENGTH, Y, X);
    }

    XOR128(X, M_last, Y);
    SWAPv2_Process(WB_Table, TABLE_LENGTH, Y, X);

    for(i = 0; i < 16; i++)
    {
        mac[i] = X[i];
    }
}


/*!    \brief    generate WhiteBox AES CMAC code
 * \param    ctx                            [in]crypto parameter
 * \param    msg                            [in]data block
 * \param    msgLen                        [in]byte-length of Text
 * \param    output                        [out]generated MAC
 * \param    outputLen                    [out]byte-length of output
 * \return    CRYPTO_SUCCESS                if no error is occured
 */
int WBAES_CMAC(unsigned char *msg, unsigned int msgLen, unsigned char *output, unsigned int *outputLen)
{
    WBAES_CMAC_internal(msg, msgLen, output);

    if(outputLen != NULL) *outputLen = AES_BLOCK_SIZE;

    return SS_SUCCESS;
}
    

/***************************** End of File *****************************/    
