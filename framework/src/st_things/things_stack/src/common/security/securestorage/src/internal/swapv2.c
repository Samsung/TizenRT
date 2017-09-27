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

////////////////////////////////////////////////////////////////////////////
// including header files
////////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <stdio.h>
#include "swapv2_defines.h"
#include "swapv2.h"

////////////////////////////////////////////////////////////////////////////
// function implementations
////////////////////////////////////////////////////////////////////////////
/*
 * @fn        static void SWAPV2_ProcNormal(SWAPV2_TBOX *p_tbox, SWAPV2_PBOX *p_pbox, unsigned int nr, unsigned char* in, unsigned char* out, unsigned int is_little_endian)
 * @brief    normal mode processing
 *
 * @param    [in] p_tbox            T-Box
 * @param    [in] p_pbox            P-Box
 * @param    [in] p_wpbox            word(4byte) permutation box
 * @param    [in] p_bpbox            bit permutation box
 * @param    [in] nr                number of rounds
 * @param    [in] in                input bytes(16-byte length)
 * @param    [out] out            output bytes(16-byte length)
 * @param    [in] is_little_endian    flag for little endian
 * @return    void
 */
//static void SWAPV2_ProcNormal(SWAPV2_TBOX *p_tbox, SWAPV2_PBOX *p_pbox, SWAPV2_WORD_PBOX *p_wpbox, SWAPV2_BIT_PBOX *p_bpbox, unsigned int nr, unsigned char *in, unsigned char *out, unsigned int is_little_endian)
//{
//#ifdef _USE_ITERATION
//  unsigned int i, j;
//#endif
//  unsigned int idx = 0;
//  unsigned int idx16;
//  unsigned char idx_wps[16];
//  unsigned char state[16];    //state
//  SWAPV2_WORD *itmdt[16];     //intermediate value
//
//  //3. Process initial permutation
//#ifdef _USE_ITERATION
//  for (i = 0; i < 2; i++) {
//      for (j = 0; j < 8; j++) {
//          idx_wps[(p_pbox[0][i][j]) & 0x0f] = state[(p_pbox[0][i][j]) & 0x0f] = in[i * 8 + j];
//      }
//  }
//#else
//  idx_wps[(p_pbox[0][0][0]) & 0x0f] = state[(p_pbox[0][0][0]) & 0x0f] = in[0];
//  idx_wps[(p_pbox[0][0][1]) & 0x0f] = state[(p_pbox[0][0][1]) & 0x0f] = in[1];
//  idx_wps[(p_pbox[0][0][2]) & 0x0f] = state[(p_pbox[0][0][2]) & 0x0f] = in[2];
//  idx_wps[(p_pbox[0][0][3]) & 0x0f] = state[(p_pbox[0][0][3]) & 0x0f] = in[3];
//  idx_wps[(p_pbox[0][0][4]) & 0x0f] = state[(p_pbox[0][0][4]) & 0x0f] = in[4];
//  idx_wps[(p_pbox[0][0][5]) & 0x0f] = state[(p_pbox[0][0][5]) & 0x0f] = in[5];
//  idx_wps[(p_pbox[0][0][6]) & 0x0f] = state[(p_pbox[0][0][6]) & 0x0f] = in[6];
//  idx_wps[(p_pbox[0][0][7]) & 0x0f] = state[(p_pbox[0][0][7]) & 0x0f] = in[7];
//  idx_wps[(p_pbox[0][1][0]) & 0x0f] = state[(p_pbox[0][1][0]) & 0x0f] = in[8];
//  idx_wps[(p_pbox[0][1][1]) & 0x0f] = state[(p_pbox[0][1][1]) & 0x0f] = in[9];
//  idx_wps[(p_pbox[0][1][2]) & 0x0f] = state[(p_pbox[0][1][2]) & 0x0f] = in[10];
//  idx_wps[(p_pbox[0][1][3]) & 0x0f] = state[(p_pbox[0][1][3]) & 0x0f] = in[11];
//  idx_wps[(p_pbox[0][1][4]) & 0x0f] = state[(p_pbox[0][1][4]) & 0x0f] = in[12];
//  idx_wps[(p_pbox[0][1][5]) & 0x0f] = state[(p_pbox[0][1][5]) & 0x0f] = in[13];
//  idx_wps[(p_pbox[0][1][6]) & 0x0f] = state[(p_pbox[0][1][6]) & 0x0f] = in[14];
//  idx_wps[(p_pbox[0][1][7]) & 0x0f] = state[(p_pbox[0][1][7]) & 0x0f] = in[15];
//
//#endif
//
//  //4. Process each rounds
//  do {
//      idx16 = (idx << 4);
//
//#ifdef _USE_ITERATION
//      for (i = 0; i < 16; i++) {
//          itmdt[i] = (SWAPV2_WORD *) p_tbox[idx16 + i][state[i] >> 4][state[i] & 0xf];
//      }
//#else
//      itmdt[0] = (SWAPV2_WORD *) p_tbox[idx16 + 0][state[0] >> 4][state[0] & 0xf];
//      itmdt[1] = (SWAPV2_WORD *) p_tbox[idx16 + 1][state[1] >> 4][state[1] & 0xf];
//      itmdt[2] = (SWAPV2_WORD *) p_tbox[idx16 + 2][state[2] >> 4][state[2] & 0xf];
//      itmdt[3] = (SWAPV2_WORD *) p_tbox[idx16 + 3][state[3] >> 4][state[3] & 0xf];
//      itmdt[4] = (SWAPV2_WORD *) p_tbox[idx16 + 4][state[4] >> 4][state[4] & 0xf];
//      itmdt[5] = (SWAPV2_WORD *) p_tbox[idx16 + 5][state[5] >> 4][state[5] & 0xf];
//      itmdt[6] = (SWAPV2_WORD *) p_tbox[idx16 + 6][state[6] >> 4][state[6] & 0xf];
//      itmdt[7] = (SWAPV2_WORD *) p_tbox[idx16 + 7][state[7] >> 4][state[7] & 0xf];
//      itmdt[8] = (SWAPV2_WORD *) p_tbox[idx16 + 8][state[8] >> 4][state[8] & 0xf];
//      itmdt[9] = (SWAPV2_WORD *) p_tbox[idx16 + 9][state[9] >> 4][state[9] & 0xf];
//      itmdt[10] = (SWAPV2_WORD *) p_tbox[idx16 + 10][state[10] >> 4][state[10] & 0xf];
//      itmdt[11] = (SWAPV2_WORD *) p_tbox[idx16 + 11][state[11] >> 4][state[11] & 0xf];
//      itmdt[12] = (SWAPV2_WORD *) p_tbox[idx16 + 12][state[12] >> 4][state[12] & 0xf];
//      itmdt[13] = (SWAPV2_WORD *) p_tbox[idx16 + 13][state[13] >> 4][state[13] & 0xf];
//      itmdt[14] = (SWAPV2_WORD *) p_tbox[idx16 + 14][state[14] >> 4][state[14] & 0xf];
//      itmdt[15] = (SWAPV2_WORD *) p_tbox[idx16 + 15][state[15] >> 4][state[15] & 0xf];
//#endif
//      idx++;
//
//#ifdef _USE_ITERATION
//      for (i = 0; i < 4; i++) {
//          for (j = 0; j < 4; j++) {
//              state[p_pbox[idx][i][j] & 0x0f] = (*itmdt[p_pbox[idx][i][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][i][4] & 0x0f] & 0xf) % 4]][j]]
//                                               ^ (*itmdt[p_pbox[idx][i][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][i][5] & 0x0f] & 0xf) % 4]][j]]
//                                               ^ (*itmdt[p_pbox[idx][i][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][i][6] & 0x0f] & 0xf) % 4]][j]]
//                                               ^ (*itmdt[p_pbox[idx][i][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][i][7] & 0x0f] & 0xf) % 4]][j]];
//          }
//      }
//
//      for (i = 0; i < 16; i++) {
//          idx_wps[i] = state[i];
//      }
//#else
//      state[p_pbox[idx][0][0] & 0x0f] = (*itmdt[p_pbox[idx][0][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][4] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][0][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][5] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][0][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][6] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][0][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][7] & 0x0f] & 0xf) % 4]][0]];
//      state[p_pbox[idx][0][1] & 0x0f] = (*itmdt[p_pbox[idx][0][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][4] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][0][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][5] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][0][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][6] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][0][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][7] & 0x0f] & 0xf) % 4]][1]];
//      state[p_pbox[idx][0][2] & 0x0f] = (*itmdt[p_pbox[idx][0][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][4] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][0][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][5] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][0][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][6] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][0][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][7] & 0x0f] & 0xf) % 4]][2]];
//      state[p_pbox[idx][0][3] & 0x0f] = (*itmdt[p_pbox[idx][0][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][4] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][0][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][5] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][0][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][6] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][0][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][0][7] & 0x0f] & 0xf) % 4]][3]];
//
//      state[p_pbox[idx][1][0] & 0x0f] = (*itmdt[p_pbox[idx][1][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][4] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][1][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][5] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][1][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][6] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][1][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][7] & 0x0f] & 0xf) % 4]][0]];
//      state[p_pbox[idx][1][1] & 0x0f] = (*itmdt[p_pbox[idx][1][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][4] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][1][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][5] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][1][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][6] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][1][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][7] & 0x0f] & 0xf) % 4]][1]];
//      state[p_pbox[idx][1][2] & 0x0f] = (*itmdt[p_pbox[idx][1][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][4] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][1][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][5] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][1][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][6] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][1][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][7] & 0x0f] & 0xf) % 4]][2]];
//      state[p_pbox[idx][1][3] & 0x0f] = (*itmdt[p_pbox[idx][1][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][4] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][1][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][5] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][1][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][6] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][1][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][1][7] & 0x0f] & 0xf) % 4]][3]];
//
//      state[p_pbox[idx][2][0] & 0x0f] = (*itmdt[p_pbox[idx][2][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][4] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][2][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][5] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][2][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][6] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][2][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][7] & 0x0f] & 0xf) % 4]][0]];
//      state[p_pbox[idx][2][1] & 0x0f] = (*itmdt[p_pbox[idx][2][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][4] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][2][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][5] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][2][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][6] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][2][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][7] & 0x0f] & 0xf) % 4]][1]];
//      state[p_pbox[idx][2][2] & 0x0f] = (*itmdt[p_pbox[idx][2][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][4] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][2][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][5] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][2][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][6] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][2][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][7] & 0x0f] & 0xf) % 4]][2]];
//      state[p_pbox[idx][2][3] & 0x0f] = (*itmdt[p_pbox[idx][2][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][4] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][2][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][5] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][2][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][6] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][2][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][2][7] & 0x0f] & 0xf) % 4]][3]];
//
//      state[p_pbox[idx][3][0] & 0x0f] = (*itmdt[p_pbox[idx][3][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][4] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][3][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][5] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][3][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][6] & 0x0f] & 0xf) % 4]][0]]
//                                       ^ (*itmdt[p_pbox[idx][3][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][7] & 0x0f] & 0xf) % 4]][0]];
//      state[p_pbox[idx][3][1] & 0x0f] = (*itmdt[p_pbox[idx][3][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][4] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][3][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][5] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][3][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][6] & 0x0f] & 0xf) % 4]][1]]
//                                       ^ (*itmdt[p_pbox[idx][3][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][7] & 0x0f] & 0xf) % 4]][1]];
//      state[p_pbox[idx][3][2] & 0x0f] = (*itmdt[p_pbox[idx][3][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][4] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][3][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][5] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][3][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][6] & 0x0f] & 0xf) % 4]][2]]
//                                       ^ (*itmdt[p_pbox[idx][3][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][7] & 0x0f] & 0xf) % 4]][2]];
//      state[p_pbox[idx][3][3] & 0x0f] = (*itmdt[p_pbox[idx][3][4] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][4] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][3][5] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][5] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][3][6] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][6] & 0x0f] & 0xf) % 4]][3]]
//                                       ^ (*itmdt[p_pbox[idx][3][7] & 0x0f])[byte_pMatrix[(*p_wpbox)[idx - 1][(idx_wps[p_pbox[idx][3][7] & 0x0f] & 0xf) % 4]][3]];
//
//      idx_wps[0] = state[0];
//      idx_wps[1] = state[1];
//      idx_wps[2] = state[2];
//      idx_wps[3] = state[3];
//      idx_wps[4] = state[4];
//      idx_wps[5] = state[5];
//      idx_wps[6] = state[6];
//      idx_wps[7] = state[7];
//      idx_wps[8] = state[8];
//      idx_wps[9] = state[9];
//      idx_wps[10] = state[10];
//      idx_wps[11] = state[11];
//      idx_wps[12] = state[12];
//      idx_wps[13] = state[13];
//      idx_wps[14] = state[14];
//      idx_wps[15] = state[15];
//
//#endif
//  } while (idx < (nr - 1));
//
//  idx16 = (idx << 4);
//
//#ifdef _USE_ITERATION
//  for (i = 0; i < 2; i++) {
//      for (j = 0; j < 8; j++) {
//          out[p_pbox[nr][i][j] & 0x0f] = p_tbox[idx16 + (i * 8 + j)][state[(i * 8 + j)] >> 4][state[(i * 8 + j)] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[(i * 8 + j)] & 0x0f) % 4]][0]];
//      }
//  }
//#else
//  //5. Process final round
//  //make output
//  out[p_pbox[nr][0][0] & 0x0f] = p_tbox[idx16 + 0][state[0] >> 4][state[0] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[0] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][1] & 0x0f] = p_tbox[idx16 + 1][state[1] >> 4][state[1] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[1] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][2] & 0x0f] = p_tbox[idx16 + 2][state[2] >> 4][state[2] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[2] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][3] & 0x0f] = p_tbox[idx16 + 3][state[3] >> 4][state[3] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[3] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][4] & 0x0f] = p_tbox[idx16 + 4][state[4] >> 4][state[4] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[4] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][5] & 0x0f] = p_tbox[idx16 + 5][state[5] >> 4][state[5] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[5] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][6] & 0x0f] = p_tbox[idx16 + 6][state[6] >> 4][state[6] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[6] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][0][7] & 0x0f] = p_tbox[idx16 + 7][state[7] >> 4][state[7] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[7] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][0] & 0x0f] = p_tbox[idx16 + 8][state[8] >> 4][state[8] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[8] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][1] & 0x0f] = p_tbox[idx16 + 9][state[9] >> 4][state[9] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[9] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][2] & 0x0f] = p_tbox[idx16 + 10][state[10] >> 4][state[10] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[10] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][3] & 0x0f] = p_tbox[idx16 + 11][state[11] >> 4][state[11] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[11] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][4] & 0x0f] = p_tbox[idx16 + 12][state[12] >> 4][state[12] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[12] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][5] & 0x0f] = p_tbox[idx16 + 13][state[13] >> 4][state[13] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[13] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][6] & 0x0f] = p_tbox[idx16 + 14][state[14] >> 4][state[14] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[14] & 0x0f) % 4]][0]];
//  out[p_pbox[nr][1][7] & 0x0f] = p_tbox[idx16 + 15][state[15] >> 4][state[15] & 0x0f][byte_pMatrix[(*p_wpbox)[idx][(state[15] & 0x0f) % 4]][0]];
//#endif
//
//  return;
//}

/**
 * @fn        SWAPV2_RETURN_VAL SWAPV2_Process(unsigned char *table, unsigned int table_len, unsigned char* in, unsigned char* out, unsigned int OptimizLev)
 * @brief    processing encryption or decryption according to the table
 *
 * @param    [in] table        look-up table for SWAPV2
 * @param    [in] table_len    byte-length of look-up table
 * @param    [in] in            input bytes(16-byte length)
 * @param    [out] out        output bytes(16-byte length)
 * @return    SWAPV2_RETURN_VAL
 */
SWAPV2_RETURN_VAL swapv2_process(unsigned char *table, unsigned int table_len, unsigned char *in, unsigned char *out)
{
	unsigned int nr;
	unsigned int tbox_size;
	unsigned int pbox_size;
	unsigned int wpbox_size;

	SWAPV2_TBOX *p_tbox = NULL;
	SWAPV2_PBOX *p_pbox = NULL;
	SWAPV2_WORD_PBOX *p_wpbox = NULL;
	SWAPV2_BIT_PBOX *p_bpbox = NULL;

	unsigned char is_little_endian;

	{
		//get little endian flag
		unsigned int endian_test_vector = 1;
		is_little_endian = *(unsigned char *)(&endian_test_vector);
	}

	//1. Parameter checking
	if ((table == NULL) || (in == NULL) || (out == NULL)) {
		return SWAPV2_RETURN_INVALID_PARAMETER;	//invalid parameter
	}
#ifdef SIMPLE_AES_NR
	nr = SIMPLE_AES_NR;
#else
	//2. Get nr(Number of Rounds)
	switch (table_len) {
	case (SWAPV2_T_TABLE_ROUND_SIZE * SWAPV2_128_ROUNDS) + (SWAPV2_P_TABLE_ROUND_SIZE * (SWAPV2_128_ROUNDS + 1)) + (SWAPV2_WP_TABLE_ROUND_SIZE * SWAPV2_128_ROUNDS) + (SWAPV2_BP_TABLE_ROUND_SIZE * SWAPV2_128_ROUNDS):
		//AES128
		nr = SWAPV2_128_ROUNDS;
		break;
	case (SWAPV2_T_TABLE_ROUND_SIZE * SWAPV2_192_ROUNDS) + (SWAPV2_P_TABLE_ROUND_SIZE * (SWAPV2_192_ROUNDS + 1)) + (SWAPV2_WP_TABLE_ROUND_SIZE * SWAPV2_192_ROUNDS) + (SWAPV2_BP_TABLE_ROUND_SIZE * SWAPV2_192_ROUNDS):
		//AES192
		nr = SWAPV2_192_ROUNDS;
		break;
	case (SWAPV2_T_TABLE_ROUND_SIZE * SWAPV2_256_ROUNDS) + (SWAPV2_P_TABLE_ROUND_SIZE * (SWAPV2_256_ROUNDS + 1)) + (SWAPV2_WP_TABLE_ROUND_SIZE * SWAPV2_256_ROUNDS) + (SWAPV2_BP_TABLE_ROUND_SIZE * SWAPV2_256_ROUNDS):
		//AES256
		nr = SWAPV2_256_ROUNDS;
		break;
	default:
		return SWAPV2_RETURN_INVALID_TBLSIZE;	//invalid parameter
	}
#endif

	//Get pointer of P-Box
	p_tbox = (SWAPV2_TBOX *) table;
	tbox_size = SWAPV2_T_TABLE_ROUND_SIZE * nr;

	p_pbox = (SWAPV2_PBOX *)((unsigned char *)p_tbox + tbox_size);
	pbox_size = SWAPV2_P_TABLE_ROUND_SIZE * (nr + 1);
	wpbox_size = SWAPV2_WP_TABLE_ROUND_SIZE * (nr);

	p_wpbox = (SWAPV2_WORD_PBOX *)(((unsigned char *)p_tbox) + tbox_size + pbox_size);
	p_bpbox = (SWAPV2_BIT_PBOX *)(((unsigned char *)p_tbox) + tbox_size + pbox_size + wpbox_size);

	SWAPV2_ProcNormal(p_tbox, p_pbox, p_wpbox, p_bpbox, nr, in, out, is_little_endian);

	return SWAPV2_RETURN_SUCCESS;
}

////////////////////////// End of File //////////////////////////
