// Copyright 2020-2023 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __ETH_MAC_TYPES_H_
#define __ETH_MAC_TYPES_H_

/** @addtogroup Exported_types
  * @{
  */
typedef enum
{
  RESET = 0,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum
{
  SUCCESS = 0,
  ERROR = !SUCCESS
} ErrorStatus;

#define HAL_MAX_DELAY      0xFFFFFFFFU

/**
  * @}
  */

#define UNUSED(x) ((void)(x))

#ifndef   __ALIGNED
  #define __ALIGNED(x)                           __attribute__((aligned(x)))
#endif

#ifndef   __WEAK
  #define __WEAK                                 __attribute__((weak))
#endif

/** @addtogroup Exported_macros
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))

/* Use of CMSIS compiler intrinsics for register exclusive access */
/* Atomic 32-bit register access macro to set one or several bits */
#define ATOMIC_SET_BIT(REG, BIT)                             \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) | (BIT);       \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 32-bit register access macro to clear one or several bits */
#define ATOMIC_CLEAR_BIT(REG, BIT)                           \
  do {                                                       \
    uint32_t val;                                            \
    do {                                                     \
      val = __LDREXW((__IO uint32_t *)&(REG)) & ~(BIT);      \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 32-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFY_REG(REG, CLEARMSK, SETMASK)                          \
  do {                                                                     \
    uint32_t val;                                                          \
    do {                                                                   \
      val = (__LDREXW((__IO uint32_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXW(val,(__IO uint32_t *)&(REG))) != 0U);               \
  } while(0)

/* Atomic 16-bit register access macro to set one or several bits */
#define ATOMIC_SETH_BIT(REG, BIT)                            \
  do {                                                       \
    uint16_t val;                                            \
    do {                                                     \
      val = __LDREXH((__IO uint16_t *)&(REG)) | (BIT);       \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 16-bit register access macro to clear one or several bits */
#define ATOMIC_CLEARH_BIT(REG, BIT)                          \
  do {                                                       \
    uint16_t val;                                            \
    do {                                                     \
      val = __LDREXH((__IO uint16_t *)&(REG)) & ~(BIT);      \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U); \
  } while(0)

/* Atomic 16-bit register access macro to clear and set one or several bits */
#define ATOMIC_MODIFYH_REG(REG, CLEARMSK, SETMASK)                         \
  do {                                                                     \
    uint16_t val;                                                          \
    do {                                                                   \
      val = (__LDREXH((__IO uint16_t *)&(REG)) & ~(CLEARMSK)) | (SETMASK); \
    } while ((__STREXH(val,(__IO uint16_t *)&(REG))) != 0U);               \
  } while(0)

/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  HAL_OK       = 0x00,
  HAL_ERROR    = 0x01,
  HAL_BUSY     = 0x02,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;

#ifndef __IO
#define     __IO    volatile             /*!< Defines 'read / write' permissions */
#endif


/**
  * @brief Ethernet MAC
  */
typedef struct
{                                        /* Offset to Base Addr */
  __IO uint32_t MACCR;
  __IO uint32_t MACECR;
  __IO uint32_t MACPFR;
  __IO uint32_t MACWTR;
  __IO uint32_t MACHT0R;
  __IO uint32_t MACHT1R;
  uint32_t      RESERVED1[14];
  __IO uint32_t MACVTR;
  uint32_t      RESERVED2;
  __IO uint32_t MACVHTR;
  uint32_t      RESERVED3;
  __IO uint32_t MACVIR;
  __IO uint32_t MACIVIR;
  uint32_t      RESERVED4[2];
  __IO uint32_t MACTFCR;
  uint32_t      RESERVED5[7];
  __IO uint32_t MACRFCR;
  uint32_t      RESERVED6[7];
  __IO uint32_t MACISR;
  __IO uint32_t MACIER;
  __IO uint32_t MACRXTXSR;
  uint32_t      RESERVED7;
  __IO uint32_t MACPCSR;
  __IO uint32_t MACRWKPFR;
  uint32_t      RESERVED8[2];
  __IO uint32_t MACLCSR;
  __IO uint32_t MACLTCR;
  __IO uint32_t MACLETR;
  __IO uint32_t MAC1USTCR;
  uint32_t      RESERVED9[12];
  __IO uint32_t MACVR;
  __IO uint32_t MACDR;
  uint32_t      RESERVED10;
  __IO uint32_t MACHWF0R;
  __IO uint32_t MACHWF1R;
  __IO uint32_t MACHWF2R;
  uint32_t      RESERVED11[54];
  __IO uint32_t MACMDIOAR;
  __IO uint32_t MACMDIODR;
  uint32_t      RESERVED12[2];
  __IO uint32_t MACARPAR;
  uint32_t      RESERVED13[59];
  __IO uint32_t MACA0HR;
  __IO uint32_t MACA0LR;
  __IO uint32_t MACA1HR;
  __IO uint32_t MACA1LR;
  __IO uint32_t MACA2HR;
  __IO uint32_t MACA2LR;
  __IO uint32_t MACA3HR;
  __IO uint32_t MACA3LR;
  uint32_t      RESERVED14[248];
  __IO uint32_t MMCCR;
  __IO uint32_t MMCRIR;
  __IO uint32_t MMCTIR;
  __IO uint32_t MMCRIMR;
  __IO uint32_t MMCTIMR;
  uint32_t      RESERVED15[14];
  __IO uint32_t MMCTSCGPR;
  __IO uint32_t MMCTMCGPR;
  uint32_t      RESERVED16[5];
  __IO uint32_t MMCTPCGR;
  uint32_t      RESERVED17[10];
  __IO uint32_t MMCRCRCEPR;
  __IO uint32_t MMCRAEPR;
  uint32_t      RESERVED18[10];
  __IO uint32_t MMCRUPGR;
  uint32_t      RESERVED19[9];
  __IO uint32_t MMCTLPIMSTR;
  __IO uint32_t MMCTLPITCR;
  __IO uint32_t MMCRLPIMSTR;
  __IO uint32_t MMCRLPITCR;
  uint32_t      RESERVED20[65];
  __IO uint32_t MACL3L4C0R;
  __IO uint32_t MACL4A0R;
  uint32_t      RESERVED21[2];
  __IO uint32_t MACL3A0R0R;
  __IO uint32_t MACL3A1R0R;
  __IO uint32_t MACL3A2R0R;
  __IO uint32_t MACL3A3R0R;
  uint32_t      RESERVED22[4];
  __IO uint32_t MACL3L4C1R;
  __IO uint32_t MACL4A1R;
  uint32_t      RESERVED23[2];
  __IO uint32_t MACL3A0R1R;
  __IO uint32_t MACL3A1R1R;
  __IO uint32_t MACL3A2R1R;
  __IO uint32_t MACL3A3R1R;
  uint32_t      RESERVED24[108];
  __IO uint32_t MACTSCR;
  __IO uint32_t MACSSIR;
  __IO uint32_t MACSTSR;
  __IO uint32_t MACSTNR;
  __IO uint32_t MACSTSUR;
  __IO uint32_t MACSTNUR;
  __IO uint32_t MACTSAR;
  uint32_t      RESERVED25;
  __IO uint32_t MACTSSR;
  uint32_t      RESERVED26[3];
  __IO uint32_t MACTTSSNR;
  __IO uint32_t MACTTSSSR;
  uint32_t      RESERVED27[2];
  __IO uint32_t MACACR;
  uint32_t      RESERVED28;
  __IO uint32_t MACATSNR;
  __IO uint32_t MACATSSR;
  __IO uint32_t MACTSIACR;
  __IO uint32_t MACTSEACR;
  __IO uint32_t MACTSICNR;
  __IO uint32_t MACTSECNR;
  uint32_t      RESERVED29[4];
  __IO uint32_t MACPPSCR;
  uint32_t      RESERVED30[3];
  __IO uint32_t MACPPSTTSR;
  __IO uint32_t MACPPSTTNR;
  __IO uint32_t MACPPSIR;
  __IO uint32_t MACPPSWR;
  uint32_t      RESERVED31[12];
  __IO uint32_t MACPOCR;
  __IO uint32_t MACSPI0R;
  __IO uint32_t MACSPI1R;
  __IO uint32_t MACSPI2R;
  __IO uint32_t MACLMIR;
  uint32_t      RESERVED32[11];
  __IO uint32_t MTLOMR;
  uint32_t      RESERVED33[7];
  __IO uint32_t MTLISR;
  uint32_t      RESERVED34[55];
  __IO uint32_t MTLTQOMR;
  __IO uint32_t MTLTQUR;
  __IO uint32_t MTLTQDR;
  uint32_t      RESERVED35[8];
  __IO uint32_t MTLQICSR;
  __IO uint32_t MTLRQOMR;
  __IO uint32_t MTLRQMPOCR;
  __IO uint32_t MTLRQDR;
  uint32_t      RESERVED36[177];
  __IO uint32_t DMAMR;                   /* 0x400 */
  __IO uint32_t DMASBMR;
  __IO uint32_t DMAISR;
  __IO uint32_t DMADSR;
  uint32_t      RESERVED37[60];
  __IO uint32_t DMACCR;                  /* 0x440 */
  __IO uint32_t DMACTCR;
  __IO uint32_t DMACRCR;
  uint32_t      RESERVED38[2];
  __IO uint32_t DMACTDLAR;               /* 0x445 */
  uint32_t      RESERVED39;
  __IO uint32_t DMACRDLAR;
  __IO uint32_t DMACTDTPR;
  uint32_t      RESERVED40;
  __IO uint32_t DMACRDTPR;
  __IO uint32_t DMACTDRLR;
  __IO uint32_t DMACRDRLR;
  __IO uint32_t DMACIER;
  __IO uint32_t DMACRIWTR;
  __IO uint32_t DMACSFCSR;
  uint32_t      RESERVED41;
  __IO uint32_t DMACCATDR;
  uint32_t      RESERVED42;
  __IO uint32_t DMACCARDR;
  uint32_t      RESERVED43;
  __IO uint32_t DMACCATBR;
  uint32_t      RESERVED44;
  __IO uint32_t DMACCARBR;
  __IO uint32_t DMACSR;
  uint32_t      RESERVED45[2];
  __IO uint32_t DMACMFCR;
}ETH_TypeDef;

#endif /* __ETH_MAC_TYPES_H_ */
