// Copyright 2022-2023 Beken
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

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#pragma once

#ifdef __cplusplus
extern "C" {
#endif


typedef volatile union {
	struct {
		uint32_t shfwen           :  4; /**<bit[0 : 3] 0: shuffle write disabled 1: shuffle write enabled*/
		uint32_t shfren           :  4; /**<bit[4 : 7] 0: shuffle read disabled 1: shuffle read enabled*/
		uint32_t puflck           :  4; /**<bit[8 : 11] 0: PUF Lock disabled 1: PUF Lock enabled*/
		uint32_t tmlck            :  4; /**<bit[12 : 15] 0: Test Mode Lock disabled 1: Test Mode Lock enabled*/
		uint32_t enroll           :  4; /**<bit[16 : 19] 0: enrollment not done 1: enrollment done*/
		uint32_t otplck           :  4; /**<bit[20 : 23] 0: OTP Lock disabled 1: OTP Lock enabled*/
		uint32_t reserved_24_27   :  4; /**<bit[24 : 27] */
		uint32_t pgmprt           :  4; /**<bit[28 : 31] 0: program protect disabled 1: program protect enabled*/
	};
	uint32_t v;
} otp_module_t;


typedef volatile union {
	struct {
		uint32_t rolck_cde_000    :  4; /**<bit[0 : 3] otp2_000~031 permission*/ 
		uint32_t rolck_cde_001    :  4; /**<bit[4 : 7] otp2_032~063 permission*/
		uint32_t rolck_cde_002    :  4; /**<bit[8 : 11] otp2_064~095 permission*/
		uint32_t rolck_cde_003    :  4; /**<bit[12 : 15] otp2_096~127 permission*/
		uint32_t rolck_cde_004    :  4; /**<bit[16 : 19] otp2_128~159 permission*/
		uint32_t rolck_cde_005    :  4; /**<bit[20 : 23] otp2_160~191 permission*/
		uint32_t rolck_cde_006    :  4; /**<bit[24 : 27] otp2_192~223 permission*/
		uint32_t rolck_cde_007    :  4; /**<bit[28 : 31] otp2_224~255 permission*/
	};
	uint32_t v;
} rolck_cde_t; /*total otp2 000~767*/


typedef volatile union {
	struct {
		uint32_t secr_ctl_otp_0   :  3; /**<bit[0 : 2] secure flag for otp_000~otp_063(to define secure range)*/
		uint32_t secr_ctl_otp_1   :  3; /**<bit[3 : 5] secure flag for otp_064~otp_127(to define secure range)*/
		uint32_t secr_ctl_otp_2   :  3; /**<bit[6 : 8] secure flag for otp_128~otp_191(to define secure range)*/
		uint32_t secr_ctl_otp_3   :  3; /**<bit[9 : 11] secure flag for otp_192~otp_256(to define secure range)*/
		uint32_t secr_ctl_puf_0   :  3; /**<bit[12 : 14] secure flag for puf_000~puf_007(to define secure range)*/
		uint32_t secr_ctl_puf_1   :  3; /**<bit[15 : 17] secure flag for puf_008~puf_015(to define secure range)*/
		uint32_t secr_ctl_puf_2   :  3; /**<bit[18 : 20] secure flag for puf_016~puf_023(to define secure range)*/
		uint32_t secr_ctl_puf_3   :  3; /**<bit[21 : 23] secure flag for puf_024~puf_031(to define secure range)*/
		uint32_t secr_zeroize     :  3; /**<bit[24 : 26] secure zeroize flag*/
		uint32_t reserved_27_27   :  1; /**<bit[27 : 27] */
		uint32_t secr_prot_en     :  4; /**<bit[28 : 31] secure protection flag*/
	};
	uint32_t v;
} secr_ctrl_t;


typedef volatile union {
	struct {
		uint32_t zeroized_puf     : 32; /**<bit[0 : 31] [1:0] puf_000~puf_007 
														[3:2] puf_008~puf_015 
														[5:4] puf_016~puf_023 
														[7:6] puf_024~puf_031 
														11: zeroized values 
														others: puf values*/
	};
	uint32_t v;
} zeroized_puf_t;


typedef volatile union {
	struct {
		uint32_t zeroized_otp    : 32; /**<bit[0 : 31] [1: 0] otp_000~otp_007 
														[ 3: 2] otp_008~otp_015 
														[ 5: 4] otp_016~otp_023 
														[ 7: 6] otp_024~otp_031 
														......
														[31:29] otp_124~otp_127 
														11: zeroized values 
														others: otp values*/
	};
	uint32_t v;
} zeroized_otp_t;


typedef volatile union {
	struct {
		uint32_t puf_lckwd_000    :  4; /**<bit[0 : 3] puf_000 RO/NA permission*/
		uint32_t puf_lckwd_001    :  4; /**<bit[4 : 7] puf_001 RO/NA permission*/
		uint32_t puf_lckwd_002    :  4; /**<bit[8 : 11] puf_002 RO/NA permission*/
		uint32_t puf_lckwd_003    :  4; /**<bit[12 : 15] puf_003 RO/NA permission*/
		uint32_t puf_lckwd_004    :  4; /**<bit[16 : 19] puf_004 RO/NA permission*/
		uint32_t puf_lckwd_005    :  4; /**<bit[20 : 23] puf_005 RO/NA permission*/
		uint32_t puf_lckwd_006    :  4; /**<bit[24 : 27] puf_006 RO/NA permission*/
		uint32_t puf_lckwd_007    :  4; /**<bit[28 : 31] puf_007 RO/NA permission*/
	};
	uint32_t v;
} puf_lckwd_t; /*total puf 000~031*/


typedef volatile union {
	struct {
		uint32_t otp_lckwd_000    :  4; /**<bit[0 : 3] otp_000 RD/WR permission*/
		uint32_t otp_lckwd_001    :  4; /**<bit[4 : 7] otp_001 RD/WR permission*/
		uint32_t otp_lckwd_002    :  4; /**<bit[8 : 11] otp_002 RD/WR permission*/
		uint32_t otp_lckwd_003    :  4; /**<bit[12 : 15] otp_003 RD/WR permission*/
		uint32_t otp_lckwd_004    :  4; /**<bit[16 : 19] otp_004 RD/WR permission*/
		uint32_t otp_lckwd_005    :  4; /**<bit[20 : 23] otp_005 RD/WR permission*/
		uint32_t otp_lckwd_006    :  4; /**<bit[24 : 27] otp_006 RD/WR permission*/
		uint32_t otp_lckwd_007    :  4; /**<bit[28 : 31] otp_007 RD/WR permission*/
	};
	uint32_t v;
} otp_lckwd_t; /*total otp 000~255*/


typedef volatile union {
	struct {
		uint32_t ptr_000          : 32; /**<bit[0 : 31] Test Mode-Test Row*/   
	};
	uint32_t v;
} ptr_t; /*total ptr 000~016*/



typedef volatile union {
	struct {
		uint32_t ptc_000          : 32; /**<bit[0 : 31] Test Mode-Test Column(only LSB is valid)*/
	};
	uint32_t v;
} ptc_t; /*total ptc 000~016*/


typedef volatile union {
	struct {
		uint32_t cde_psmsk_0      : 32; /**<bit[0 : 31] [ 1: 0]: otp2_000-otp2_031
										[ 3: 2]: otp2_032-otp2_063
										...........
										[31:30]: otp2_480-otp2_511
										b00: OTP2 code segment can be accessed
										others: OTP2 code segment is post-masked, permission becomes NA*/
	};
	uint32_t v;
} cde_psmask_t;


typedef volatile union {
	struct {
		uint32_t random_value     : 32; /**<bit[0 : 31] */
	};
	uint32_t v;
} random_value_t;


typedef volatile union {
	struct {
		uint32_t intrpt_st        : 10; /**<bit[0 : 9] 		[0] 1: problem with power supply
															[1] 1: conflict on output data
															[2] 1: input parity check fail
															[3] 1: metal shielding fault
															[4] 1: access permission error
															[5] 1: parity error of FSM
															[6] 1: parity error of critical reg
															[7] reserved
															[8] 1: RNG health check fail
															[9] 1: RNG internal clock is turned off*/
		uint32_t reserved_10_15   :  6; /**<bit[10 : 15] */
		uint32_t intrpt_en        : 10; /**<bit[16 : 25] */
		uint32_t reserved_26_31   :  6; /**<bit[26 : 31] */
	};
	uint32_t v;
} intrpt_t;


typedef volatile union {
	struct {
		uint32_t otp_psmsk_0      : 32; /**<bit[0 : 31] post masking flags for otp_000~otp_127
										[ 1: 0]: otp_000-otp_007
										[ 3: 2]: otp_008-otp_015
										..............
										[31:30]: otp_120-otp_127
										b00: OTP code segment can be accessed
										others: OTP code segment is post-masked, permission becomes NA*/
	};
	uint32_t v;
} otp_psmask_t;


typedef volatile union {
	struct {
		uint32_t puf_psmsk        :  8; /**<bit [0: 7] post masking flags for puf_000~puf_031
												[1: 0]: puf_000-puf_007
												[3: 2]: puf_008-puf_015
												[5: 4]: puf_016-puf_023
												[7: 6]: puf_024-puf_031
												b00: PUF code segment can be accessed
												others: PUF code segment is post-masked, permission becomes NA*/
		uint32_t reserved_8_23    : 16; /**<bit[8 : 23] */
		uint32_t lck_psmsk        :  8; /**<bit[24 : 31]change access policy (from RW to RO) of the 
														cde_psmsk_0, cde_psmsk_1, the otp_psmsk_0, otp_psmsk_1 and puf_psmsk*/
	};
	uint32_t v;
} puf_psmask_t;


typedef volatile union {
	struct {
		uint32_t version          : 32; /**<bit[0 : 31] version=PM30D6 (0x504d30d6)*/
	};
	uint32_t v;
} version_t;


typedef volatile union {
	struct {
		uint32_t busy             :  1; /**<bit[0 : 0] 1: PTM is busy*/
		uint32_t error            :  1; /**<bit[1 : 1] 1: PTM is error*/
		uint32_t warning          :  1; /**<bit[2 : 2] 1: PTM is re-run or not initialized*/
		uint32_t wrong            :  1; /**<bit[3 : 3] 1: PTM data is not correctly set*/
		uint32_t forbid           :  1; /**<bit[4 : 4] 1: PTM is locked*/
		uint32_t reserved_5_7     :  3; /**<bit[5 : 7] */
		uint32_t failcnt          :  4; /**<bit[8 : 11] number of failed OTP cells*/
		uint32_t reserved_12_31   : 20; /**<bit[12 : 31] */
	};
	uint32_t v;
} status_t;


typedef volatile union {
	struct {
		uint32_t pdstb            :  1; /**<bit[0 : 0] 0: Hard Macro sleep 1: Hard Macro active*/
		uint32_t clkosc_en        :  1; /**<bit[1 : 1] 0: disable internal PUFtrng clock 1: enable internal PUFtrng clock*/
		uint32_t reserved_2_3     :  2; /**<bit[2 : 3] (please set all 0)*/
		uint32_t rd_mode          :  2; /**<bit[4 : 5] 0: set rd_mode=RD 1: set rd_mode=OFF_MG_RD 2: set rd_mode=ON_MG_RD*/
		uint32_t rd_to_sel        :  2; /**<bit[6 : 7] wait timeout to active mode 0: 10us 1: 5us 2: 2us 3: immediately*/
		uint32_t fre_cont         :  1; /**<bit[8 : 8] 0: disable raw entropy test mode 1: enable raw entropy test mode*/
		uint32_t reserved_9_10    :  2; /**<bit[9 : 10] */
		uint32_t ht_rd            :  1; /**<bit[11 : 11] 1: Enable HighTemp Margin Read OFF_MG_RD -> OFF_MG_RD_HT ON_MG_RD -> ON_MG_RD_HT*/
		uint32_t reserved_12_23   : 12; /**<bit[12 : 23] */
		uint32_t ptc_page         :  8; /**<bit[24 : 31] page of test column (PTC) (ptc_page=0~7)*/
	};
	uint32_t v;
} otp_hard_t;


typedef volatile union {
	struct {
		uint32_t auto_repair      : 32; /**<bit[0 : 31] if tmlck=unlocked 0xe9: do auto repair*/
	};
	uint32_t v;
} auto_repair_t;


typedef volatile union {
	struct {
		uint32_t ini_off_chk      : 32; /**<bit[0 : 31] if tmlck=unlocked, 0x5a: Initial OFF check (OFF_MG_RD) 0x83: initial off check (OFF_MG_RD_HT)*/
	};
	uint32_t v;
} ini_off_chk_t;


typedef volatile union {
	struct {
		uint32_t puf_qty_chk      : 32; /**<bit[0 : 31] 0x00: do PUF quality check 0xdc: do PUF health check*/
	};
	uint32_t v;
} puf_qty_chk_t;


typedef volatile union {
	struct {
		uint32_t puf_enroll       : 32; /**<bit[0 : 31] if tmlck=unlocked and puflck=unlocked 0xa7: do PUF Enrollment*/
	};
	uint32_t v;
} puf_enroll_t;


typedef volatile union {
	struct {
		uint32_t puf_zeroize      : 32; /**<bit[0 : 31] if puflck=unlocked  0x4b: zeroize puf_000~puf_007 
																			0xad: zeroize puf_008~puf_015 
																			0xd2: zeroize puf_016~puf_023 
																			0x34: zeroize puf_024~puf_031*/
	};
	uint32_t v;
} puf_zeroize_t;


typedef volatile union {
	struct {
		uint32_t set_flag         : 32; /**<bit[0 : 31] if tmlck=unlocked   0xb6: program the pgmprt flag 
																			0x71: program the tmlck flag 
																			0x2c: program the puflck flag 
																			0x65: program the otplck flag 
																			0x99: program the shfren flag 
																			0xc2: program the shfwen flag*/
	};
	uint32_t v;
} set_flag_t;


typedef volatile union {
	struct {
		uint32_t otp_zeroize      : 32; /**<bit[0 : 31] if otplck=unlocked  0x80: zeroize otp_000~otp_007 
																			0x81: zeroize otp_008~otp_015 
																			......
																			0x9f: zeroize otp_248~otp_255*/
	};
	uint32_t v;
} otp_zeroize_t;


typedef volatile union {
	struct {
		uint32_t puf_000          : 32; /**<bit[0 : 31] for UID*/
	};
	uint32_t v;
} puf_t;


typedef volatile union {
	struct {
		uint32_t otp_000          : 32; /**<bit[0 : 31] OTP location 000*/
	};
	uint32_t v;
} otp_t;

typedef volatile union {
	struct {
		uint32_t otp2_000         : 32; /**<bit[0 : 31] OTP2 location 000*/
	};
	uint32_t v;
} otp2_t;


typedef volatile struct {
	volatile otp_module_t    module_flag;                  /*reg 0x0*/
	volatile uint32_t        reserved0[3];                 /*reg 0x1-0x3*/
	volatile rolck_cde_t     rolck[3];                     /*reg 0x4-0x6*/
	volatile uint32_t        reserved1;                    /*reg 0x7*/
	volatile secr_ctrl_t     sectrl;                       /*reg 0x8*/
	volatile uint32_t        reserved2[3];                 /*reg 0x9-0xb*/
	volatile zeroized_puf_t  zeroized_puf;                 /*reg 0xc*/
	volatile uint32_t        reserved3;                    /*reg 0xd*/
	volatile zeroized_otp_t  zeroized_otp[2];              /*reg 0xe-0xf*/
	volatile uint32_t        reserved4[12];                /*reg 0x10-0x1b*/
	volatile puf_lckwd_t     puf_lckwd[4];                 /*reg 0x1c-0x1f*/
	volatile otp_lckwd_t     otp_lckwd[32];                /*reg 0x20-0x3f*/
	volatile uint32_t        reserved5[64];                /*reg 0x40-0x7f*/
	volatile ptr_t           test_row[16];                 /*reg 0x80-0x8f*/
	volatile ptc_t           test_column[16];              /*reg 0x90-0x9f*/
	volatile cde_psmask_t    cde_mask[2];                  /*reg 0xA0-0xA1*/
	volatile uint32_t        reserved6[6];                 /*reg 0xA2-0xA7*/
	volatile random_value_t  random_value;                 /*reg 0xA8*/
	volatile uint32_t        reserved7[3];                 /*reg 0xA9-0xAB*/
	volatile intrpt_t        intrpt;                       /*reg 0xAC*/
	volatile otp_psmask_t    otp_mask[2];                  /*reg 0xAD-0xAE*/
	volatile puf_psmask_t    puf_mask;                     /*reg 0xAF*/
	volatile version_t       version;                      /*reg 0xB0*/
	volatile status_t        status;                       /*reg 0xB1*/
	volatile otp_hard_t      hardware;                     /*reg 0xB2*/
	volatile uint32_t        reserved8;                    /*reg 0xB3*/
	volatile auto_repair_t   auto_repair;                  /*reg 0xB4*/
	volatile ini_off_chk_t   ini_off_check;                /*reg 0xB5*/
	volatile uint32_t        reserved9[2];                 /*reg 0xB6-0xB7*/
	volatile puf_qty_chk_t   puf_qty_chk;                  /*reg 0xB8*/
	volatile puf_enroll_t    puf_enroll;                   /*reg 0xB9*/
	volatile puf_zeroize_t   puf_zeroize;                  /*reg 0xBA*/
	volatile set_flag_t      set_flag;                     /*reg 0xBB*/
	volatile otp_zeroize_t   otp_zeroize;                  /*reg 0xBC*/
	volatile uint32_t        reserved10[3];                 /*reg 0xBD-0xBF*/
	volatile puf_t           puf[64];                      /*reg 0xC0-0xFF*/
	volatile otp_t           otp[256];                     /*reg 0x100-0x1FF*/
} otp_hw_t;

typedef volatile struct {
	volatile otp2_t          otp2[767];                     /*reg 0x000-0x2FF*/
} otp2_hw_t;

#ifdef __cplusplus
}
#endif
