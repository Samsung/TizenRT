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

#include "hal_config.h"
#include "otp_hw.h"
#include "otp_hal.h"

typedef void (*otp_dump_fn_t)(void);
typedef struct {
	uint32_t start;
	uint32_t end;
	otp_dump_fn_t fn;
} otp_reg_fn_map_t;

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x0 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x0 << 2)));
	SOC_LOGD("	shfwen: %8x\r\n", r->shfwen);
	SOC_LOGD("	shfren: %8x\r\n", r->shfren);
	SOC_LOGD("	puflck: %8x\r\n", r->puflck);
	SOC_LOGD("	tmlck: %8x\r\n", r->tmlck);
	SOC_LOGD("	enroll: %8x\r\n", r->enroll);
	SOC_LOGD("	otplck: %8x\r\n", r->otplck);
	SOC_LOGD("	reserved_24_27: %8x\r\n", r->reserved_24_27);
	SOC_LOGD("	pgmprt: %8x\r\n", r->pgmprt);
}

static void otp_dump_rsv_1_3(void)
{
	for (uint32_t idx = 0; idx < 3; idx++) {
		SOC_LOGD("rsv_1_3: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x1 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x4 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x4 << 2)));
	SOC_LOGD("	rolck_cde_000: %8x\r\n", r->rolck_cde_000);
	SOC_LOGD("	rolck_cde_001: %8x\r\n", r->rolck_cde_001);
	SOC_LOGD("	rolck_cde_002: %8x\r\n", r->rolck_cde_002);
	SOC_LOGD("	rolck_cde_003: %8x\r\n", r->rolck_cde_003);
	SOC_LOGD("	rolck_cde_004: %8x\r\n", r->rolck_cde_004);
	SOC_LOGD("	rolck_cde_005: %8x\r\n", r->rolck_cde_005);
	SOC_LOGD("	rolck_cde_006: %8x\r\n", r->rolck_cde_006);
	SOC_LOGD("	rolck_cde_007: %8x\r\n", r->rolck_cde_007);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x5 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x5 << 2)));
	SOC_LOGD("	rolck_cde_008: %8x\r\n", r->rolck_cde_008);
	SOC_LOGD("	rolck_cde_009: %8x\r\n", r->rolck_cde_009);
	SOC_LOGD("	rolck_cde_010: %8x\r\n", r->rolck_cde_010);
	SOC_LOGD("	rolck_cde_011: %8x\r\n", r->rolck_cde_011);
	SOC_LOGD("	rolck_cde_012: %8x\r\n", r->rolck_cde_012);
	SOC_LOGD("	rolck_cde_013: %8x\r\n", r->rolck_cde_013);
	SOC_LOGD("	rolck_cde_014: %8x\r\n", r->rolck_cde_014);
	SOC_LOGD("	rolck_cde_015: %8x\r\n", r->rolck_cde_015);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x6 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x6 << 2)));
	SOC_LOGD("	rolck_cde_016: %8x\r\n", r->rolck_cde_016);
	SOC_LOGD("	rolck_cde_017: %8x\r\n", r->rolck_cde_017);
	SOC_LOGD("	rolck_cde_018: %8x\r\n", r->rolck_cde_018);
	SOC_LOGD("	rolck_cde_019: %8x\r\n", r->rolck_cde_019);
	SOC_LOGD("	rolck_cde_020: %8x\r\n", r->rolck_cde_020);
	SOC_LOGD("	rolck_cde_021: %8x\r\n", r->rolck_cde_021);
	SOC_LOGD("	rolck_cde_022: %8x\r\n", r->rolck_cde_022);
	SOC_LOGD("	rolck_cde_023: %8x\r\n", r->rolck_cde_023);
}

static void otp_dump_rsv_7_7(void)
{
	for (uint32_t idx = 0; idx < 1; idx++) {
		SOC_LOGD("rsv_7_7: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x7 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x8 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8 << 2)));
	SOC_LOGD("	secr_ctl_otp_0: %8x\r\n", r->secr_ctl_otp_0);
	SOC_LOGD("	secr_ctl_otp_1: %8x\r\n", r->secr_ctl_otp_1);
	SOC_LOGD("	secr_ctl_otp_2: %8x\r\n", r->secr_ctl_otp_2);
	SOC_LOGD("	secr_ctl_otp_3: %8x\r\n", r->secr_ctl_otp_3);
	SOC_LOGD("	secr_ctl_puf_0: %8x\r\n", r->secr_ctl_puf_0);
	SOC_LOGD("	secr_ctl_puf_1: %8x\r\n", r->secr_ctl_puf_1);
	SOC_LOGD("	secr_ctl_puf_2: %8x\r\n", r->secr_ctl_puf_2);
	SOC_LOGD("	secr_ctl_puf_3: %8x\r\n", r->secr_ctl_puf_3);
	SOC_LOGD("	secr_zeroize: %8x\r\n", r->secr_zeroize);
	SOC_LOGD("	reserved_27_27: %8x\r\n", r->reserved_27_27);
	SOC_LOGD("	secr_prot_en: %8x\r\n", r->secr_prot_en);
}

static void otp_dump_rsv_9_b(void)
{
	for (uint32_t idx = 0; idx < 3; idx++) {
		SOC_LOGD("rsv_9_b: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x9 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xc << 2)));
}

static void otp_dump_rsv_d_d(void)
{
	for (uint32_t idx = 0; idx < 1; idx++) {
		SOC_LOGD("rsv_d_d: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xd + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xe << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xf << 2)));
}

static void otp_dump_rsv_10_1b(void)
{
	for (uint32_t idx = 0; idx < 12; idx++) {
		SOC_LOGD("rsv_10_1b: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x10 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x1c << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x1c << 2)));
	SOC_LOGD("	puf_lckwd_000: %8x\r\n", r->puf_lckwd_000);
	SOC_LOGD("	puf_lckwd_001: %8x\r\n", r->puf_lckwd_001);
	SOC_LOGD("	puf_lckwd_002: %8x\r\n", r->puf_lckwd_002);
	SOC_LOGD("	puf_lckwd_003: %8x\r\n", r->puf_lckwd_003);
	SOC_LOGD("	puf_lckwd_004: %8x\r\n", r->puf_lckwd_004);
	SOC_LOGD("	puf_lckwd_005: %8x\r\n", r->puf_lckwd_005);
	SOC_LOGD("	puf_lckwd_006: %8x\r\n", r->puf_lckwd_006);
	SOC_LOGD("	puf_lckwd_007: %8x\r\n", r->puf_lckwd_007);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x1d << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x1d << 2)));
	SOC_LOGD("	puf_lckwd_008: %8x\r\n", r->puf_lckwd_008);
	SOC_LOGD("	puf_lckwd_009: %8x\r\n", r->puf_lckwd_009);
	SOC_LOGD("	puf_lckwd_010: %8x\r\n", r->puf_lckwd_010);
	SOC_LOGD("	puf_lckwd_011: %8x\r\n", r->puf_lckwd_011);
	SOC_LOGD("	puf_lckwd_012: %8x\r\n", r->puf_lckwd_012);
	SOC_LOGD("	puf_lckwd_013: %8x\r\n", r->puf_lckwd_013);
	SOC_LOGD("	puf_lckwd_014: %8x\r\n", r->puf_lckwd_014);
	SOC_LOGD("	puf_lckwd_015: %8x\r\n", r->puf_lckwd_015);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x1e << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x1e << 2)));
	SOC_LOGD("	puf_lckwd_016: %8x\r\n", r->puf_lckwd_016);
	SOC_LOGD("	puf_lckwd_017: %8x\r\n", r->puf_lckwd_017);
	SOC_LOGD("	puf_lckwd_018: %8x\r\n", r->puf_lckwd_018);
	SOC_LOGD("	puf_lckwd_019: %8x\r\n", r->puf_lckwd_019);
	SOC_LOGD("	puf_lckwd_020: %8x\r\n", r->puf_lckwd_020);
	SOC_LOGD("	puf_lckwd_021: %8x\r\n", r->puf_lckwd_021);
	SOC_LOGD("	puf_lckwd_022: %8x\r\n", r->puf_lckwd_022);
	SOC_LOGD("	puf_lckwd_023: %8x\r\n", r->puf_lckwd_023);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x1f << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x1f << 2)));
	SOC_LOGD("	puf_lckwd_024: %8x\r\n", r->puf_lckwd_024);
	SOC_LOGD("	puf_lckwd_025: %8x\r\n", r->puf_lckwd_025);
	SOC_LOGD("	puf_lckwd_026: %8x\r\n", r->puf_lckwd_026);
	SOC_LOGD("	puf_lckwd_027: %8x\r\n", r->puf_lckwd_027);
	SOC_LOGD("	puf_lckwd_028: %8x\r\n", r->puf_lckwd_028);
	SOC_LOGD("	puf_lckwd_029: %8x\r\n", r->puf_lckwd_029);
	SOC_LOGD("	puf_lckwd_030: %8x\r\n", r->puf_lckwd_030);
	SOC_LOGD("	puf_lckwd_031: %8x\r\n", r->puf_lckwd_031);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x20 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x20 << 2)));
	SOC_LOGD("	puf_lckwd_000: %8x\r\n", r->puf_lckwd_000);
	SOC_LOGD("	puf_lckwd_001: %8x\r\n", r->puf_lckwd_001);
	SOC_LOGD("	puf_lckwd_002: %8x\r\n", r->puf_lckwd_002);
	SOC_LOGD("	puf_lckwd_003: %8x\r\n", r->puf_lckwd_003);
	SOC_LOGD("	puf_lckwd_004: %8x\r\n", r->puf_lckwd_004);
	SOC_LOGD("	puf_lckwd_005: %8x\r\n", r->puf_lckwd_005);
	SOC_LOGD("	puf_lckwd_006: %8x\r\n", r->puf_lckwd_006);
	SOC_LOGD("	puf_lckwd_007: %8x\r\n", r->puf_lckwd_007);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x21 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x21 << 2)));
	SOC_LOGD("	puf_lckwd_008: %8x\r\n", r->puf_lckwd_008);
	SOC_LOGD("	puf_lckwd_009: %8x\r\n", r->puf_lckwd_009);
	SOC_LOGD("	puf_lckwd_010: %8x\r\n", r->puf_lckwd_010);
	SOC_LOGD("	puf_lckwd_011: %8x\r\n", r->puf_lckwd_011);
	SOC_LOGD("	puf_lckwd_012: %8x\r\n", r->puf_lckwd_012);
	SOC_LOGD("	puf_lckwd_013: %8x\r\n", r->puf_lckwd_013);
	SOC_LOGD("	puf_lckwd_014: %8x\r\n", r->puf_lckwd_014);
	SOC_LOGD("	puf_lckwd_015: %8x\r\n", r->puf_lckwd_015);
}

static void otp_dump_rsv_22_3d(void)
{
	for (uint32_t idx = 0; idx < 28; idx++) {
		SOC_LOGD("rsv_22_3d: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x22 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x3e << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x3e << 2)));
	SOC_LOGD("	puf_lckwd_240: %8x\r\n", r->puf_lckwd_240);
	SOC_LOGD("	puf_lckwd_241: %8x\r\n", r->puf_lckwd_241);
	SOC_LOGD("	puf_lckwd_242: %8x\r\n", r->puf_lckwd_242);
	SOC_LOGD("	puf_lckwd_243: %8x\r\n", r->puf_lckwd_243);
	SOC_LOGD("	puf_lckwd_244: %8x\r\n", r->puf_lckwd_244);
	SOC_LOGD("	puf_lckwd_245: %8x\r\n", r->puf_lckwd_245);
	SOC_LOGD("	puf_lckwd_246: %8x\r\n", r->puf_lckwd_246);
	SOC_LOGD("	puf_lckwd_247: %8x\r\n", r->puf_lckwd_247);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0x3f << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x3f << 2)));
	SOC_LOGD("	puf_lckwd_248: %8x\r\n", r->puf_lckwd_248);
	SOC_LOGD("	puf_lckwd_249: %8x\r\n", r->puf_lckwd_249);
	SOC_LOGD("	puf_lckwd_250: %8x\r\n", r->puf_lckwd_250);
	SOC_LOGD("	puf_lckwd_251: %8x\r\n", r->puf_lckwd_251);
	SOC_LOGD("	puf_lckwd_252: %8x\r\n", r->puf_lckwd_252);
	SOC_LOGD("	puf_lckwd_253: %8x\r\n", r->puf_lckwd_253);
	SOC_LOGD("	puf_lckwd_254: %8x\r\n", r->puf_lckwd_254);
	SOC_LOGD("	puf_lckwd_255: %8x\r\n", r->puf_lckwd_255);
}

static void otp_dump_rsv_40_7f(void)
{
	for (uint32_t idx = 0; idx < 64; idx++) {
		SOC_LOGD("rsv_40_7f: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x40 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x80 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x81 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x82 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x83 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x84 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x85 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x86 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x87 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x88 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x89 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8a << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8b << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8c << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8d << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8e << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x8f << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x90 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x91 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x92 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x93 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x94 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x95 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x96 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x97 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x98 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x99 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x9a << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x9b << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x9c << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x9d << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x9e << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x9f << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xa0 << 2)));
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0xa1 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xa1 << 2)));
	SOC_LOGD("	cde_psmsk_1: %8x\r\n", r->cde_psmsk_1);
	SOC_LOGD("	reserved_16_31: %8x\r\n", r->reserved_16_31);
}

static void otp_dump_rsv_a2_a7(void)
{
	for (uint32_t idx = 0; idx < 6; idx++) {
		SOC_LOGD("rsv_a2_a7: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xa2 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xa8 << 2)));
}

static void otp_dump_rsv_a9_ab(void)
{
	for (uint32_t idx = 0; idx < 3; idx++) {
		SOC_LOGD("rsv_a9_ab: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xa9 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0xac << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xac << 2)));
	SOC_LOGD("	intrpt_st: %8x\r\n", r->intrpt_st);
	SOC_LOGD("	reserved_10_15: %8x\r\n", r->reserved_10_15);
	SOC_LOGD("	intrpt_en: %8x\r\n", r->intrpt_en);
	SOC_LOGD("	reserved_26_31: %8x\r\n", r->reserved_26_31);
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xad << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xae << 2)));
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0xaf << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xaf << 2)));
	SOC_LOGD("	puf_psmsk: %8x\r\n", r->puf_psmsk);
	SOC_LOGD("	reserved_8_23: %8x\r\n", r->reserved_8_23);
	SOC_LOGD("	lck_psmsk: %8x\r\n", r->lck_psmsk);
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb0 << 2)));
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0xb1 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb1 << 2)));
	SOC_LOGD("	busy: %8x\r\n", r->busy);
	SOC_LOGD("	error: %8x\r\n", r->error);
	SOC_LOGD("	warning: %8x\r\n", r->warning);
	SOC_LOGD("	wrong: %8x\r\n", r->wrong);
	SOC_LOGD("	forbid: %8x\r\n", r->forbid);
	SOC_LOGD("	reserved_5_7: %8x\r\n", r->reserved_5_7);
	SOC_LOGD("	failcnt: %8x\r\n", r->failcnt);
	SOC_LOGD("	reserved_12_31: %8x\r\n", r->reserved_12_31);
}

static void otp_dump_(void)
{
	otp__t *r = (otp__t *)(SOC_OTP_REG_BASE + (0xb2 << 2));

	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb2 << 2)));
	SOC_LOGD("	pdstb: %8x\r\n", r->pdstb);
	SOC_LOGD("	clkosc_en: %8x\r\n", r->clkosc_en);
	SOC_LOGD("	reserved_2_3: %8x\r\n", r->reserved_2_3);
	SOC_LOGD("	rd_mode: %8x\r\n", r->rd_mode);
	SOC_LOGD("	rd_to_sel: %8x\r\n", r->rd_to_sel);
	SOC_LOGD("	fre_cont: %8x\r\n", r->fre_cont);
	SOC_LOGD("	reserved_9_10: %8x\r\n", r->reserved_9_10);
	SOC_LOGD("	ht_rd: %8x\r\n", r->ht_rd);
	SOC_LOGD("	reserved_12_23: %8x\r\n", r->reserved_12_23);
	SOC_LOGD("	ptc_page: %8x\r\n", r->ptc_page);
}

static void otp_dump_rsv_b3_b3(void)
{
	for (uint32_t idx = 0; idx < 1; idx++) {
		SOC_LOGD("rsv_b3_b3: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xb3 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb4 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb5 << 2)));
}

static void otp_dump_rsv_b6_b7(void)
{
	for (uint32_t idx = 0; idx < 2; idx++) {
		SOC_LOGD("rsv_b6_b7: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xb6 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb8 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xb9 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xba << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xbb << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xbc << 2)));
}

static void otp_dump_rsv_bd_bf(void)
{
	for (uint32_t idx = 0; idx < 3; idx++) {
		SOC_LOGD("rsv_bd_bf: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xbd + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xc0 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xc1 << 2)));
}

static void otp_dump_rsv_c2_de(void)
{
	for (uint32_t idx = 0; idx < 29; idx++) {
		SOC_LOGD("rsv_c2_de: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xc2 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xdf << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xe0 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xe1 << 2)));
}

static void otp_dump_rsv_e2_fe(void)
{
	for (uint32_t idx = 0; idx < 29; idx++) {
		SOC_LOGD("rsv_e2_fe: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0xe2 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0xff << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x100 << 2)));
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x101 << 2)));
}

static void otp_dump_rsv_102_1fe(void)
{
	for (uint32_t idx = 0; idx < 253; idx++) {
		SOC_LOGD("rsv_102_1fe: %8x\r\n", REG_READ(SOC_OTP_REG_BASE + ((0x102 + idx) << 2)));
	}
}

static void otp_dump_(void)
{
	SOC_LOGD(": %8x\r\n", REG_READ(SOC_OTP_REG_BASE + (0x1ff << 2)));
}

static otp_reg_fn_map_t s_fn[] =
{
	{0x0, 0x0, otp_dump_},
	{0x1, 0x4, otp_dump_rsv_1_3},
	{0x4, 0x4, otp_dump_},
	{0x5, 0x5, otp_dump_},
	{0x6, 0x6, otp_dump_},
	{0x7, 0x8, otp_dump_rsv_7_7},
	{0x8, 0x8, otp_dump_},
	{0x9, 0xc, otp_dump_rsv_9_b},
	{0xc, 0xc, otp_dump_},
	{0xd, 0xe, otp_dump_rsv_d_d},
	{0xe, 0xe, otp_dump_},
	{0xf, 0xf, otp_dump_},
	{0x10, 0x1c, otp_dump_rsv_10_1b},
	{0x1c, 0x1c, otp_dump_},
	{0x1d, 0x1d, otp_dump_},
	{0x1e, 0x1e, otp_dump_},
	{0x1f, 0x1f, otp_dump_},
	{0x20, 0x20, otp_dump_},
	{0x21, 0x21, otp_dump_},
	{0x22, 0x3e, otp_dump_rsv_22_3d},
	{0x3e, 0x3e, otp_dump_},
	{0x3f, 0x3f, otp_dump_},
	{0x40, 0x80, otp_dump_rsv_40_7f},
	{0x80, 0x80, otp_dump_},
	{0x81, 0x81, otp_dump_},
	{0x82, 0x82, otp_dump_},
	{0x83, 0x83, otp_dump_},
	{0x84, 0x84, otp_dump_},
	{0x85, 0x85, otp_dump_},
	{0x86, 0x86, otp_dump_},
	{0x87, 0x87, otp_dump_},
	{0x88, 0x88, otp_dump_},
	{0x89, 0x89, otp_dump_},
	{0x8a, 0x8a, otp_dump_},
	{0x8b, 0x8b, otp_dump_},
	{0x8c, 0x8c, otp_dump_},
	{0x8d, 0x8d, otp_dump_},
	{0x8e, 0x8e, otp_dump_},
	{0x8f, 0x8f, otp_dump_},
	{0x90, 0x90, otp_dump_},
	{0x91, 0x91, otp_dump_},
	{0x92, 0x92, otp_dump_},
	{0x93, 0x93, otp_dump_},
	{0x94, 0x94, otp_dump_},
	{0x95, 0x95, otp_dump_},
	{0x96, 0x96, otp_dump_},
	{0x97, 0x97, otp_dump_},
	{0x98, 0x98, otp_dump_},
	{0x99, 0x99, otp_dump_},
	{0x9a, 0x9a, otp_dump_},
	{0x9b, 0x9b, otp_dump_},
	{0x9c, 0x9c, otp_dump_},
	{0x9d, 0x9d, otp_dump_},
	{0x9e, 0x9e, otp_dump_},
	{0x9f, 0x9f, otp_dump_},
	{0xa0, 0xa0, otp_dump_},
	{0xa1, 0xa1, otp_dump_},
	{0xa2, 0xa8, otp_dump_rsv_a2_a7},
	{0xa8, 0xa8, otp_dump_},
	{0xa9, 0xac, otp_dump_rsv_a9_ab},
	{0xac, 0xac, otp_dump_},
	{0xad, 0xad, otp_dump_},
	{0xae, 0xae, otp_dump_},
	{0xaf, 0xaf, otp_dump_},
	{0xb0, 0xb0, otp_dump_},
	{0xb1, 0xb1, otp_dump_},
	{0xb2, 0xb2, otp_dump_},
	{0xb3, 0xb4, otp_dump_rsv_b3_b3},
	{0xb4, 0xb4, otp_dump_},
	{0xb5, 0xb5, otp_dump_},
	{0xb6, 0xb8, otp_dump_rsv_b6_b7},
	{0xb8, 0xb8, otp_dump_},
	{0xb9, 0xb9, otp_dump_},
	{0xba, 0xba, otp_dump_},
	{0xbb, 0xbb, otp_dump_},
	{0xbc, 0xbc, otp_dump_},
	{0xbd, 0xc0, otp_dump_rsv_bd_bf},
	{0xc0, 0xc0, otp_dump_},
	{0xc1, 0xc1, otp_dump_},
	{0xc2, 0xdf, otp_dump_rsv_c2_de},
	{0xdf, 0xdf, otp_dump_},
	{0xe0, 0xe0, otp_dump_},
	{0xe1, 0xe1, otp_dump_},
	{0xe2, 0xff, otp_dump_rsv_e2_fe},
	{0xff, 0xff, otp_dump_},
	{0x100, 0x100, otp_dump_},
	{0x101, 0x101, otp_dump_},
	{0x102, 0x1ff, otp_dump_rsv_102_1fe},
	{0x1ff, 0x1ff, otp_dump_},
	{-1, -1, 0}
};

void otp_struct_dump(uint32_t start, uint32_t end)
{
	uint32_t dump_fn_cnt = sizeof(s_fn)/sizeof(s_fn[0]) - 1;

	for (uint32_t idx = 0; idx < dump_fn_cnt; idx++) {
		if ((start <= s_fn[idx].start) && (end >= s_fn[idx].end)) {
			s_fn[idx].fn();
		}
	}
}
