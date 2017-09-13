/**
 * @file    SWAPv2.c
 * @brief    white box AES module for SSA(Samsung Security Architecture)
 * @author    Samsung WhiteBox AES Version 2 (SWAPv1 based)
 *                - Chul Lee(chuls.lee@samsung.com)
 *                - Dept : Software R&D Center / Cloud Computing Lab.
 *
 *            Samsung WhiteBox AES Version 1
 *                 - Jisoon Park(js00n.park@samsung.com)
 *                 - Dept: DMC R&D Center/Convergence Software Lab.
 *
 * @see        SWAPv2
 * @date    2012-10-05
 * @version    2.0
 * @par        Copyright:
 * Copyright 2012 by Samsung Electronics, Inc.,
 * This software is the confiedential and properietary information 
 * of Samsung Elctronics, Inc. ("Confidential Information"). You 
 * shall not disclose such Confidential Information and shall use 
 * it only in accordance with the term of the lice agreement 
 * you entered into with Samsung.
 */

////////////////////////////////////////////////////////////////////////////
// including header files
////////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <stdio.h>
#include "SWAPv2_defines.h"
#include "SWAPv2.h"

////////////////////////////////////////////////////////////////////////////
// function implementations
////////////////////////////////////////////////////////////////////////////
/*
 * @fn        static void SWAPv2_ProcNormal(SWAPv2_TBOX *pTBox, SWAPv2_PBOX *pPBox, unsigned int Nr, unsigned char* in, unsigned char* out, unsigned int isLittleEndian)
 * @brief    normal mode processing
 *
 * @param    [in] pTBox            T-Box
 * @param    [in] pPBox            P-Box
 * @param    [in] pWPBox            word(4byte) permutation box
 * @param    [in] pBPBox            bit permutation box
 * @param    [in] Nr                number of rounds
 * @param    [in] in                input bytes(16-byte length)
 * @param    [out] out            output bytes(16-byte length)
 * @param    [in] isLittleEndian    flag for little endian
 * @return    void
 */
static void SWAPv2_ProcNormal(SWAPv2_TBOX *pTBox, SWAPv2_PBOX *pPBox, SWAPv2_WORD_PBOX* pWPBox, SWAPv2_BIT_PBOX* pBPBox, unsigned int Nr, unsigned char* in, unsigned char* out, unsigned int isLittleEndian)
{
#ifdef _USE_ITERATION
    unsigned int i, j;
#endif
    unsigned int idx = 0;
    unsigned int idx16;
    unsigned char idx_wps[16];
    unsigned char state[16];    //state
    SWAPv2_WORD* itmdt[16];        //intermediate value


    //3. Process initial permutation
#ifdef _USE_ITERATION
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < 8; j++)
        {
            idx_wps[(pPBox[0][i][j]) & 0x0f] = state[(pPBox[0][i][j]) & 0x0f] = in[i * 8 + j];
        }
    }
#else
        idx_wps[(pPBox[0][0][0]) & 0x0f] = state[(pPBox[0][0][0]) & 0x0f] = in[ 0];
        idx_wps[(pPBox[0][0][1]) & 0x0f] = state[(pPBox[0][0][1]) & 0x0f] = in[ 1];
        idx_wps[(pPBox[0][0][2]) & 0x0f] = state[(pPBox[0][0][2]) & 0x0f] = in[ 2];
        idx_wps[(pPBox[0][0][3]) & 0x0f] = state[(pPBox[0][0][3]) & 0x0f] = in[ 3];
        idx_wps[(pPBox[0][0][4]) & 0x0f] = state[(pPBox[0][0][4]) & 0x0f] = in[ 4];
        idx_wps[(pPBox[0][0][5]) & 0x0f] = state[(pPBox[0][0][5]) & 0x0f] = in[ 5];
        idx_wps[(pPBox[0][0][6]) & 0x0f] = state[(pPBox[0][0][6]) & 0x0f] = in[ 6];
        idx_wps[(pPBox[0][0][7]) & 0x0f] = state[(pPBox[0][0][7]) & 0x0f] = in[ 7];
        idx_wps[(pPBox[0][1][0]) & 0x0f] = state[(pPBox[0][1][0]) & 0x0f] = in[ 8];
        idx_wps[(pPBox[0][1][1]) & 0x0f] = state[(pPBox[0][1][1]) & 0x0f] = in[ 9];
        idx_wps[(pPBox[0][1][2]) & 0x0f] = state[(pPBox[0][1][2]) & 0x0f] = in[10];
        idx_wps[(pPBox[0][1][3]) & 0x0f] = state[(pPBox[0][1][3]) & 0x0f] = in[11];
        idx_wps[(pPBox[0][1][4]) & 0x0f] = state[(pPBox[0][1][4]) & 0x0f] = in[12];
        idx_wps[(pPBox[0][1][5]) & 0x0f] = state[(pPBox[0][1][5]) & 0x0f] = in[13];
        idx_wps[(pPBox[0][1][6]) & 0x0f] = state[(pPBox[0][1][6]) & 0x0f] = in[14];
        idx_wps[(pPBox[0][1][7]) & 0x0f] = state[(pPBox[0][1][7]) & 0x0f] = in[15];

#endif

    //4. Process each rounds
    do
    {
        idx16 = (idx << 4);

#ifdef _USE_ITERATION
        for(i = 0; i < 16; i++)
        {
            itmdt[i] = (SWAPv2_WORD*)pTBox[ idx16 +  i ][ state[i] >> 4 ][ state[i] &  0xf ];
        }
#else
        itmdt[ 0] = (SWAPv2_WORD*)pTBox[idx16 +  0][state[ 0] >> 4][state[ 0] &  0xf];
        itmdt[ 1] = (SWAPv2_WORD*)pTBox[idx16 +  1][state[ 1] >> 4][state[ 1] &  0xf];
        itmdt[ 2] = (SWAPv2_WORD*)pTBox[idx16 +  2][state[ 2] >> 4][state[ 2] &  0xf];
        itmdt[ 3] = (SWAPv2_WORD*)pTBox[idx16 +  3][state[ 3] >> 4][state[ 3] &  0xf];
        itmdt[ 4] = (SWAPv2_WORD*)pTBox[idx16 +  4][state[ 4] >> 4][state[ 4] &  0xf];
        itmdt[ 5] = (SWAPv2_WORD*)pTBox[idx16 +  5][state[ 5] >> 4][state[ 5] &  0xf];
        itmdt[ 6] = (SWAPv2_WORD*)pTBox[idx16 +  6][state[ 6] >> 4][state[ 6] &  0xf];
        itmdt[ 7] = (SWAPv2_WORD*)pTBox[idx16 +  7][state[ 7] >> 4][state[ 7] &  0xf];
        itmdt[ 8] = (SWAPv2_WORD*)pTBox[idx16 +  8][state[ 8] >> 4][state[ 8] &  0xf];
        itmdt[ 9] = (SWAPv2_WORD*)pTBox[idx16 +  9][state[ 9] >> 4][state[ 9] &  0xf];
        itmdt[10] = (SWAPv2_WORD*)pTBox[idx16 + 10][state[10] >> 4][state[10] &  0xf];
        itmdt[11] = (SWAPv2_WORD*)pTBox[idx16 + 11][state[11] >> 4][state[11] &  0xf];
        itmdt[12] = (SWAPv2_WORD*)pTBox[idx16 + 12][state[12] >> 4][state[12] &  0xf];
        itmdt[13] = (SWAPv2_WORD*)pTBox[idx16 + 13][state[13] >> 4][state[13] &  0xf];
        itmdt[14] = (SWAPv2_WORD*)pTBox[idx16 + 14][state[14] >> 4][state[14] &  0xf];
        itmdt[15] = (SWAPv2_WORD*)pTBox[idx16 + 15][state[15] >> 4][state[15] &  0xf];
#endif
        idx++;


#ifdef _USE_ITERATION
        for(i = 0; i < 4; i++)
        {
            for(j = 0; j < 4; j++)
            {
                state[ pPBox[idx][i][j] & 0x0f ] = (*itmdt[ pPBox[idx][i][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][i][4] & 0x0f ] &  0xf) % 4 ]][j] ]
                                                 ^ (*itmdt[ pPBox[idx][i][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][i][5] & 0x0f ] &  0xf) % 4 ]][j] ]
                                                 ^ (*itmdt[ pPBox[idx][i][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][i][6] & 0x0f ] &  0xf) % 4 ]][j] ]
                                                 ^ (*itmdt[ pPBox[idx][i][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][i][7] & 0x0f ] &  0xf) % 4 ]][j] ];
            }
        }

        for(i = 0; i < 16; i++)
        {
             idx_wps[i] = state[i];
        }
#else
        state[ pPBox[idx][0][0] & 0x0f ] = (*itmdt[ pPBox[idx][0][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][4] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][0][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][5] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][0][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][6] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][0][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][7] & 0x0f ] &  0xf) % 4 ]][0] ];
        state[ pPBox[idx][0][1] & 0x0f ] = (*itmdt[ pPBox[idx][0][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][4] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][0][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][5] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][0][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][6] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][0][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][7] & 0x0f ] &  0xf) % 4 ]][1] ];
        state[ pPBox[idx][0][2] & 0x0f ] = (*itmdt[ pPBox[idx][0][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][4] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][0][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][5] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][0][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][6] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][0][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][7] & 0x0f ] &  0xf) % 4 ]][2] ];
        state[ pPBox[idx][0][3] & 0x0f ] = (*itmdt[ pPBox[idx][0][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][4] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][0][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][5] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][0][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][6] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][0][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][0][7] & 0x0f ] &  0xf) % 4 ]][3] ];

        state[ pPBox[idx][1][0] & 0x0f ] = (*itmdt[ pPBox[idx][1][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][4] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][1][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][5] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][1][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][6] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][1][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][7] & 0x0f ] &  0xf) % 4 ]][0] ];
        state[ pPBox[idx][1][1] & 0x0f ] = (*itmdt[ pPBox[idx][1][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][4] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][1][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][5] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][1][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][6] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][1][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][7] & 0x0f ] &  0xf) % 4 ]][1] ];
        state[ pPBox[idx][1][2] & 0x0f ] = (*itmdt[ pPBox[idx][1][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][4] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][1][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][5] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][1][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][6] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][1][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][7] & 0x0f ] &  0xf) % 4 ]][2] ];
        state[ pPBox[idx][1][3] & 0x0f ] = (*itmdt[ pPBox[idx][1][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][4] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][1][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][5] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][1][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][6] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][1][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][1][7] & 0x0f ] &  0xf) % 4 ]][3] ];

        state[ pPBox[idx][2][0] & 0x0f ] = (*itmdt[ pPBox[idx][2][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][4] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][2][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][5] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][2][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][6] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][2][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][7] & 0x0f ] &  0xf) % 4 ]][0] ];
        state[ pPBox[idx][2][1] & 0x0f ] = (*itmdt[ pPBox[idx][2][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][4] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][2][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][5] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][2][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][6] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][2][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][7] & 0x0f ] &  0xf) % 4 ]][1] ];
        state[ pPBox[idx][2][2] & 0x0f ] = (*itmdt[ pPBox[idx][2][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][4] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][2][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][5] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][2][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][6] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][2][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][7] & 0x0f ] &  0xf) % 4 ]][2] ];
        state[ pPBox[idx][2][3] & 0x0f ] = (*itmdt[ pPBox[idx][2][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][4] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][2][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][5] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][2][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][6] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][2][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][2][7] & 0x0f ] &  0xf) % 4 ]][3] ];

        state[ pPBox[idx][3][0] & 0x0f ] = (*itmdt[ pPBox[idx][3][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][4] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][3][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][5] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][3][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][6] & 0x0f ] &  0xf) % 4 ]][0] ]
                                         ^ (*itmdt[ pPBox[idx][3][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][7] & 0x0f ] &  0xf) % 4 ]][0] ];
        state[ pPBox[idx][3][1] & 0x0f ] = (*itmdt[ pPBox[idx][3][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][4] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][3][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][5] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][3][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][6] & 0x0f ] &  0xf) % 4 ]][1] ]
                                         ^ (*itmdt[ pPBox[idx][3][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][7] & 0x0f ] &  0xf) % 4 ]][1] ];
        state[ pPBox[idx][3][2] & 0x0f ] = (*itmdt[ pPBox[idx][3][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][4] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][3][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][5] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][3][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][6] & 0x0f ] &  0xf) % 4 ]][2] ]
                                         ^ (*itmdt[ pPBox[idx][3][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][7] & 0x0f ] &  0xf) % 4 ]][2] ];
        state[ pPBox[idx][3][3] & 0x0f ] = (*itmdt[ pPBox[idx][3][4] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][4] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][3][5] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][5] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][3][6] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][6] & 0x0f ] &  0xf) % 4 ]][3] ]
                                         ^ (*itmdt[ pPBox[idx][3][7] & 0x0f ])[ byte_pMatrix[(*pWPBox)[idx - 1][ (idx_wps[ pPBox[idx][3][7] & 0x0f ] &  0xf) % 4 ]][3] ];

         idx_wps[ 0] = state[ 0];
        idx_wps[ 1] = state[ 1];
        idx_wps[ 2] = state[ 2];
        idx_wps[ 3] = state[ 3];
        idx_wps[ 4] = state[ 4];
        idx_wps[ 5] = state[ 5];
        idx_wps[ 6] = state[ 6];
        idx_wps[ 7] = state[ 7];
        idx_wps[ 8] = state[ 8];
        idx_wps[ 9] = state[ 9];
        idx_wps[10] = state[10];
        idx_wps[11] = state[11];
        idx_wps[12] = state[12];
        idx_wps[13] = state[13];
        idx_wps[14] = state[14];
        idx_wps[15] = state[15];

#endif
    } while(idx < (Nr - 1));

    idx16 = (idx << 4);

#ifdef _USE_ITERATION
    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < 8; j++)
        {
            out[pPBox[Nr][i][j] & 0x0f] = pTBox[ idx16 +  (i * 8 + j) ][ state[(i * 8 + j)] >> 4 ][ state[(i * 8 + j)] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[(i * 8 + j)] & 0x0f) % 4]][0] ];
        }
    }
#else
    //5. Process final round
    //make output
    out[pPBox[Nr][0][0] & 0x0f] = pTBox[ idx16 +  0 ][ state[ 0] >> 4 ][ state[ 0] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 0] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][1] & 0x0f] = pTBox[ idx16 +  1 ][ state[ 1] >> 4 ][ state[ 1] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 1] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][2] & 0x0f] = pTBox[ idx16 +  2 ][ state[ 2] >> 4 ][ state[ 2] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 2] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][3] & 0x0f] = pTBox[ idx16 +  3 ][ state[ 3] >> 4 ][ state[ 3] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 3] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][4] & 0x0f] = pTBox[ idx16 +  4 ][ state[ 4] >> 4 ][ state[ 4] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 4] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][5] & 0x0f] = pTBox[ idx16 +  5 ][ state[ 5] >> 4 ][ state[ 5] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 5] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][6] & 0x0f] = pTBox[ idx16 +  6 ][ state[ 6] >> 4 ][ state[ 6] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 6] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][0][7] & 0x0f] = pTBox[ idx16 +  7 ][ state[ 7] >> 4 ][ state[ 7] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 7] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][0] & 0x0f] = pTBox[ idx16 +  8 ][ state[ 8] >> 4 ][ state[ 8] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 8] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][1] & 0x0f] = pTBox[ idx16 +  9 ][ state[ 9] >> 4 ][ state[ 9] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[ 9] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][2] & 0x0f] = pTBox[ idx16 + 10 ][ state[10] >> 4 ][ state[10] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[10] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][3] & 0x0f] = pTBox[ idx16 + 11 ][ state[11] >> 4 ][ state[11] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[11] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][4] & 0x0f] = pTBox[ idx16 + 12 ][ state[12] >> 4 ][ state[12] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[12] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][5] & 0x0f] = pTBox[ idx16 + 13 ][ state[13] >> 4 ][ state[13] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[13] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][6] & 0x0f] = pTBox[ idx16 + 14 ][ state[14] >> 4 ][ state[14] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[14] & 0x0f) % 4]][0] ];
    out[pPBox[Nr][1][7] & 0x0f] = pTBox[ idx16 + 15 ][ state[15] >> 4 ][ state[15] & 0x0f ][ byte_pMatrix[(*pWPBox)[idx][(state[15] & 0x0f) % 4]][0] ];
#endif

    return;
}

/**
 * @fn        SWAPv2_RETURN_VAL SWAPv2_Process(unsigned char *Table, unsigned int TableLen, unsigned char* in, unsigned char* out, unsigned int OptimizLev)
 * @brief    processing encryption or decryption according to the table
 *
 * @param    [in] Table        look-up table for SWAPv2
 * @param    [in] TableLen    byte-length of look-up table
 * @param    [in] in            input bytes(16-byte length)
 * @param    [out] out        output bytes(16-byte length)
 * @return    SWAPv2_RETURN_VAL
 */
SWAPv2_RETURN_VAL SWAPv2_Process(unsigned char *Table, unsigned int TableLen, unsigned char* in, unsigned char* out)
{
    unsigned int Nr;
    unsigned int TBoxSize;
    unsigned int PBoxSize;
    unsigned int WPBoxSize;

    SWAPv2_TBOX *pTBox = NULL;
    SWAPv2_PBOX *pPBox = NULL;
    SWAPv2_WORD_PBOX* pWPBox = NULL;
    SWAPv2_BIT_PBOX* pBPBox = NULL;

    unsigned char isLittleEndian;

    {
        //get little endian flag
        unsigned int EndianTestVector = 1;
        isLittleEndian = *(unsigned char*)(&EndianTestVector);
    }

    //1. Parameter checking
    if ((Table == NULL) || (in == NULL) || (out == NULL))
    {
        return SWAPv2_RETURN_INVALID_PARAMETER;                //invalid parameter
    }

#ifdef SIMPLE_AES_NR
    Nr = SIMPLE_AES_NR;
#else
    //2. Get Nr(Number of Rounds)
    switch(TableLen)
    {
        case (SWAPv2_T_TABLE_ROUND_SIZE * SWAPv2_128_ROUNDS) + (SWAPv2_P_TABLE_ROUND_SIZE * (SWAPv2_128_ROUNDS + 1)) + (SWAPv2_WP_TABLE_ROUND_SIZE * SWAPv2_128_ROUNDS) + (SWAPv2_BP_TABLE_ROUND_SIZE * SWAPv2_128_ROUNDS):
            //AES128
            Nr = SWAPv2_128_ROUNDS;
            break;
        case (SWAPv2_T_TABLE_ROUND_SIZE * SWAPv2_192_ROUNDS) + (SWAPv2_P_TABLE_ROUND_SIZE * (SWAPv2_192_ROUNDS + 1)) + (SWAPv2_WP_TABLE_ROUND_SIZE * SWAPv2_192_ROUNDS) + (SWAPv2_BP_TABLE_ROUND_SIZE * SWAPv2_192_ROUNDS):
            //AES192
            Nr = SWAPv2_192_ROUNDS;
            break;
        case (SWAPv2_T_TABLE_ROUND_SIZE * SWAPv2_256_ROUNDS) + (SWAPv2_P_TABLE_ROUND_SIZE * (SWAPv2_256_ROUNDS + 1)) + (SWAPv2_WP_TABLE_ROUND_SIZE * SWAPv2_256_ROUNDS) + (SWAPv2_BP_TABLE_ROUND_SIZE * SWAPv2_256_ROUNDS):
            //AES256
            Nr = SWAPv2_256_ROUNDS;
            break;
        default :
            return SWAPv2_RETURN_INVALID_TBLSIZE;                //invalid parameter
    }
#endif

    //Get pointer of P-Box
    pTBox = (SWAPv2_TBOX*)Table;
    TBoxSize = SWAPv2_T_TABLE_ROUND_SIZE * Nr;

    pPBox = (SWAPv2_PBOX*)((unsigned char*)pTBox + TBoxSize);
    PBoxSize = SWAPv2_P_TABLE_ROUND_SIZE * (Nr + 1);
    WPBoxSize = SWAPv2_WP_TABLE_ROUND_SIZE * (Nr);

    pWPBox = (SWAPv2_WORD_PBOX*)(((unsigned char*)pTBox) + TBoxSize + PBoxSize);
    pBPBox = (SWAPv2_BIT_PBOX*)(((unsigned char*)pTBox) + TBoxSize + PBoxSize + WPBoxSize);

    SWAPv2_ProcNormal(pTBox, pPBox, pWPBox, pBPBox, Nr, in, out, isLittleEndian);
    
    return SWAPv2_RETURN_SUCCESS;
}

////////////////////////// End of File //////////////////////////