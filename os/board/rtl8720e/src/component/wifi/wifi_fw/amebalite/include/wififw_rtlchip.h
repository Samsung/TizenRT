#ifndef _RTLCHIP_H_
#define _RTLCHIP_H_
/*
*****************************************

        Chip Selection

Field    FOUNDRY     RTLCHIP               CHIPCUT
BITS   [17:16]         [15:4]                 [3:0]


*****************************************
*/
//#define RTL8721D_CHIP               0x21D0
#define RTL8720E_CHIP               0x30A0
#define RTLCHIP_MASK                0xFFF0

/* Cut */
#define CHIPCUT_TEST                0x0
#define CHIPCUT_A                   0x1
#define CHIPCUT_B                   0x2
#define CHIPCUT_C                   0x3
#define CHIPCUT_D                   0x4
#define CHIPCUT_E                   0x5

#define CHIPCUT_MASK                0xF

/* Chip + Cut (Signature) */
#define CHIP_SIGNATURE_MASK         0xFFFF

/* Foundry */
#define CHIP_FOUNDRY_SHIFT          16
#define CHIP_FOUNDRY_MASK           (0x3<<CHIP_FOUNDRY_SHIFT)

#define CHIP_FOUNDRY_TSMC           (0x0<<CHIP_FOUNDRY_SHIFT)
#define CHIP_FOUNDRY_UMC            (0x1<<CHIP_FOUNDRY_SHIFT)
#define CHIP_FOUNDRY_SMIC           (0x2<<CHIP_FOUNDRY_SHIFT)

/* Macro for Chip */
#define IS_CHIP_8720E(CurSel)       ( ((CurSel) & RTLCHIP_MASK) == RTL8720E_CHIP )

#define IS_CHIP_RF_N(CurSel)        (( ((CurSel) & RTLCHIP_MASK) == RTL8188E_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTLWISOC_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8192E_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8723B_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8703B_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8188F_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8710B_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8192F_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8721D_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8720E_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8723D_CHIP ))

#define IS_CHIP_RF_AC(CurSel)       (( ((CurSel) & RTLCHIP_MASK) == RTL8195_CHIP )\
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8821_CHIP ) \
                                    || ( ((CurSel) & RTLCHIP_MASK) == RTL8881_CHIP ) )

#define GET_CHIP_ID(CurSel)         ( (CurSel) & RTLCHIP_MASK )

/* Macro for Cut */
#define IS_CUT_TEST(CurSel)         ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_TEST )
#define IS_CUT_A(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_A )
#define IS_CUT_B(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_B )
#define IS_CUT_C(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_C )
#define IS_CUT_D(CurSel)            ( ((CurSel) & CHIPCUT_MASK) == CHIPCUT_D )
#define GET_CUT(CurSel)             ((CurSel) & CHIPCUT_MASK)

/* Macro for Chip + Cut */
#define GET_SIGNATURE(CurSel)       ((CurSel) & CHIP_SIGNATURE_MASK)

/* Macro for Foundry */
#define GET_FOUNDRY(CurSel)         ((CurSel) & CHIP_FOUNDRY_MASK)

/* 3 Final Combination (Chip Select) */
/* RTL8720E */
#define RTL8720E_TEST_CHIP_S             (RTL8720E_CHIP  | CHIPCUT_TEST | CHIP_FOUNDRY_SMIC)
#define RTL8720E_A_CUT_S                 (RTL8720E_CHIP  | CHIPCUT_A    | CHIP_FOUNDRY_SMIC)
#define RTL8720E_B_CUT_S                 (RTL8720E_CHIP  | CHIPCUT_B    | CHIP_FOUNDRY_SMIC)

#endif  /* #ifndef _RTLCHIP_H_ */
