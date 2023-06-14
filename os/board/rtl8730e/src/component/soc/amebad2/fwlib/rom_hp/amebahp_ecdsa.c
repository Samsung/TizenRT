#include "ameba_soc.h"
#include "mbedtls/ecp.h"

/*
 * Domain parameters for secp192r1
 */
HAL_ROM_DATA_SECTION
static u32 SECP192R1_P[8]  = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192R1_Gx[8]  = {0x82FF1012, 0xF4FF0AFD, 0x43A18800, 0x7CBF20EB, 0xB03090F6, 0x188DA80E, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192R1_Gy[8]	= {0x1e794811, 0x73f977a1, 0x6b24cdd5, 0x631011ed, 0xffc8da78, 0x07192b95, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192R1_N[8]	= {0xb4d22831, 0x146bc9b1, 0x99def836, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192R1_A[8]	= {0xfffffffc, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000};

/*
 * Domain parameters for secp224r1
 */

HAL_ROM_DATA_SECTION
static u32 SECP224R1_P[8]  = {0x00000001, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224R1_Gx[8]	= {0x115c1d21, 0x343280d6, 0x56c21122, 0x4a03c1d3, 0x321390b9, 0x6bb4bf7f, 0xb70e0cbd, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224R1_Gy[8]	= {0x85007e34, 0x44d58199, 0x5a074764, 0xcd4375a0, 0x4c22dfe6, 0xb5f723fb, 0xbd376388, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224R1_N[8]	= {0x5c5c2a3d, 0x13dd2945, 0xe0b8f03e, 0xffff16a2, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224R1_A[8]	= {0xfffffffe, 0xffffffff, 0xffffffff, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000};

/*
 * Domain parameters for secp192k1
 */
HAL_ROM_DATA_SECTION
static u32 SECP192K1_P[8]  = {0xffffee37, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192K1_Gx[8]	= {0xeae06c7d, 0x1da5d1b1, 0x80b7f434, 0x26b07d02, 0xc057e9ae, 0xdb4ff10e, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192K1_Gy[8]	= {0xd95e2f9d, 0x4082aa88, 0x15be8634, 0x844163d0, 0x9c5628a7, 0x9b2f2f6d, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192K1_N[8]	= {0x74defd8d, 0x0f69466a, 0x26f2fc17, 0xfffffffe, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP192K1_A[8]	= {0x00, 0x00};

/*
 * Domain parameters for secp224k1
 */
HAL_ROM_DATA_SECTION
static u32 SECP224K1_P[8]  = {0xffffe56d, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224K1_Gx[8]	= {0xb6b7a45c, 0x0f7e650e, 0xe47075a9, 0x69a467e9, 0x30fc28a1, 0x4df099df, 0xa1455b33, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224K1_Gy[8]	= {0x556d61a5, 0xe2ca4bdb, 0xc0b0bd59, 0xf7e319f7, 0x82cafbd6, 0x7fba3442, 0x7e089fed, 0x00000000};

HAL_ROM_DATA_SECTION
static u32 SECP224K1_N[8]	= {0x769fb1f7, 0xcaf0a971, 0xd2ec6184, 0x0001dce8, 0x00000000, 0x00000000, 0x00000000, 0x00000001};

HAL_ROM_DATA_SECTION
static u32 SECP224K1_A[8]	= {0x00, 0x00};

/*
 * Domain parameters for brainpoolP256r1 (RFC 5639 3.4)
 */
HAL_ROM_DATA_SECTION
static u32 BRAINPOOLP256R1_P[8] = {0x1f6e5377, 0x2013481d, 0xd5262028, 0x6e3bf623, 0x9d838d72, 0x3e660a90, 0xa1eea9bc, 0xa9fb57db};

HAL_ROM_DATA_SECTION
static u32 BRAINPOOLP256R1_Gx[8] = {0x9ace3262, 0x3a4453bd, 0xe3bd23c2, 0xb9de27e1, 0xfc81b7af, 0x2c4b482f, 0xcb7e57cb, 0x8bd2aeb9};

HAL_ROM_DATA_SECTION
static u32 BRAINPOOLP256R1_Gy[8] = {0x2f046997, 0x5c1d54c7, 0x2ded8e54, 0xc2774513, 0x14611dc9, 0x97f8461a, 0xc3dac4fd, 0x547ef835};

HAL_ROM_DATA_SECTION
static u32 BRAINPOOLP256R1_N[8]	= {0x974856a7, 0x901e0e82, 0xb561a6f7, 0x8c397aa3, 0x9d838d71, 0x3e660a90, 0xa1eea9bc, 0xa9fb57db};

HAL_ROM_DATA_SECTION
static u32 BRAINPOOLP256R1_A[8]	= {0xf330b5d9, 0xe94a4b44, 0x26dc5c6c, 0xfb8055c1, 0x417affe7, 0xeef67530, 0xfc2c3057, 0x7d5a0975};

HAL_ROM_DATA_SECTION
static u32 SECP256K1_P[8]  = {0xfffffc2f, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};

HAL_ROM_DATA_SECTION
static u32 SECP256K1_N[8]	= {0xd0364141, 0xbfd25e8c, 0xaf48a03b, 0xbaaedce6, 0xfffffffe, 0xffffffff, 0xffffffff, 0xffffffff};

HAL_ROM_DATA_SECTION
static u32 SECP256R1_N[8]	= {0xfc632551, 0xf3b9cac2, 0xa7179e84, 0xbce6faad, 0xffffffff, 0xffffffff, 0x00000000, 0xffffffff};

HAL_ROM_DATA_SECTION
static u32 curve25519_N[8]	= {0x5cf5d3ed, 0x5812631a, 0xa2f79cd6, 0x14def9de, 0x00000000, 0x00000000, 0x00000000, 0x10000000};

/**
  * @brief  buf sub: d = d - s
  * @param  n: size of buf s.
  * @retval None
  */
__weak HAL_ROM_TEXT_SECTION
void ECDSA_BufSub(u32 n, u32 *s, u32 *d)
{
	u32 i;
	char c, z;
	u32 size;

	for (size = n; size > 0; size --)
		if (s[size - 1] != 0) {
			break;
		}

	for (i = c = 0; i < size; i++, s++, d++) {
		z = (*d <  c);
		*d -=  c;
		c = (*d < *s) + z;
		*d -= *s;
	}

	while (c != 0) {
		z = (*d < c);
		*d -= c;
		c = z;
		i++;
		d++;
	}

}

/**
  * @brief  Compare hash buf and N
  * @param  X: hash value buf.
  * @param  sizeofx: hash buf size.
  * @param  N: curve n buf.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_BufCmp(u32 *X, u32 sizeofx, u32 *Y)
{
	int i, j;

	for (i = sizeofx; i > 0; i--)
		if (X[i - 1] != 0) {
			break;
		}

	for (j = 8; j > 0; j--)
		if (Y[j - 1] != 0) {
			break;
		}

	if (i == 0 && j == 0) {
		return (0);
	}

	if (i > j) {
		return (1);
	}
	if (j > i) {
		return (-1);
	}

	for (; i > 0; i --) {
		if (X[i - 1] > Y[i - 1]) {
			return (1);
		}
		if (X[i - 1] < Y[i - 1]) {
			return (-1);
		}
	}
	return (0);
}

/**
  * @brief  modify hash buf for sign or verify
  * @param  curve: curve info.
  * @param  x: output hash buf.
  * @param  buf: input hash buf.
  * @param  blen: input hash buf len.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_HashBuf_Setting(ecdsa_ecp_group *curve, u32 *x, const u8 *buf, u32 blen)
{
	u32 n_size = (curve->nbits + 7) / 8;
	u32 use_size = blen > n_size ? n_size : blen;
	u32 n, i, temp_size, bufsize;
	u32 shift_size, v0, v1;
	u32 r0 = 0, r1;

	for (n = 0; n < use_size; n++) {
		if (buf[n] != 0) {
			break;
		}
	}
	temp_size = use_size - n;

	i = temp_size % 4 > 0 ? 1 : 0;
	bufsize = temp_size / 4 + i;

	u32 hashtemp[bufsize];
	_memset((char *)x, 0x0, 32);
	_memset(hashtemp, 0x0, bufsize * 4);

	for (n = use_size, i = 0; n > 0; n--, i++) {
		hashtemp[i / 4] |= buf[n - 1] << ((i % 4) << 3);
	}

	if (use_size * 8 > curve->nbits) {
		shift_size = use_size * 8 - curve->nbits;
		v0 = shift_size / 32;
		v1 = shift_size & 31;

		/* shift by 8 bits */
		if (v0 > 0) {
			for (i = 0; i < bufsize - v0; i++) {
				hashtemp[i] = hashtemp[i + v0];
			}

			for (; i < bufsize; i++) {
				hashtemp[i] = 0;
			}
		}

		/* shift by bits */
		if (v1 > 0) {
			for (i = bufsize; i > 0; i --) {
				r1 = hashtemp[i - 1] << (32 - v1);
				hashtemp[i - 1] >>= v1;
				hashtemp[i - 1] |= r0;
				r0 = r1;
			}
		}

	}

	/* compare x and n */
	if (ECDSA_BufCmp(hashtemp, bufsize, curve->N) >= 0) {
		ECDSA_BufSub(bufsize, curve->N, hashtemp);
	}

	_memcpy(x, hashtemp, bufsize * 4);
	return 0;
}

/**
  * @brief  set para in ecdsa reg by the curve id
  * @param  curve: curve info.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_CurveSel(ECDSA_TypeDef *ECDSA, ecdsa_ecp_group *curve)
{
	ECDSA->ECR2 &= (~ECDSA_MASK_CURVE_SEL);

	switch (curve->curve_id) {
	case ECDSA_ECP_SECP256K1:
		curve->nbits = 256;
		curve->curve_bit = 256;
		curve->N = SECP256K1_N;
		break;
	case ECDSA_ECP_SECP256R1:
		curve->nbits = 256;
		curve->curve_bit = 256;
		curve->N = SECP256R1_N;
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_P256);
		break;
	case ECDSA_ECP_CURVE25519:
		curve->nbits = 256;
		curve->curve_bit = 256;
		curve->N = curve25519_N;
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_CURVE25519);
		break;
	case ECDSA_ECP_SECP192K1:
		curve->nbits = 192;
		curve->curve_bit = 192;
		curve->N = SECP192K1_N;
		_memcpy((char *) & (ECDSA->ENG_GX_PSx[0]), SECP192K1_Gx, 32);
		_memcpy((char *) & (ECDSA->ENG_GY_PSx[0]), SECP192K1_Gy, 32);
		_memcpy((char *) & (ECDSA->COR_A_Px[0]), SECP192K1_A, 32);
		_memcpy((char *) & (ECDSA->PRIME_Px[0]), SECP192K1_P, 32);
		_memcpy((char *) & (ECDSA->ORDER_N_Px[0]), SECP192K1_N, 32);
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_OTHERS);
		break;
	case ECDSA_ECP_SECP224K1:
		curve->nbits = 225;
		curve->curve_bit = 224;
		curve->N = SECP224K1_N;
		_memcpy((char *) & (ECDSA->ENG_GX_PSx[0]), SECP224K1_Gx, 32);
		_memcpy((char *) & (ECDSA->ENG_GY_PSx[0]), SECP224K1_Gy, 32);
		_memcpy((char *) & (ECDSA->COR_A_Px[0]), SECP224K1_A, 32);
		_memcpy((char *) & (ECDSA->PRIME_Px[0]), SECP224K1_P, 32);
		_memcpy((char *) & (ECDSA->ORDER_N_Px[0]), SECP224K1_N, 32);
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_OTHERS);
		break;
	case ECDSA_ECP_SECP192R1:
		curve->nbits = 192;
		curve->curve_bit = 192;
		curve->N = SECP192R1_N;
		_memcpy((char *) & (ECDSA->ENG_GX_PSx[0]), SECP192R1_Gx, 32);
		_memcpy((char *) & (ECDSA->ENG_GY_PSx[0]), SECP192R1_Gy, 32);
		_memcpy((char *) & (ECDSA->COR_A_Px[0]), SECP192R1_A, 32);
		_memcpy((char *) & (ECDSA->PRIME_Px[0]), SECP192R1_P, 32);
		_memcpy((char *) & (ECDSA->ORDER_N_Px[0]), SECP192R1_N, 32);
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_OTHERS);
		break;
	case ECDSA_ECP_SECP224R1:
		curve->nbits = 224;
		curve->curve_bit = 224;
		curve->N = SECP224R1_N;
		_memcpy((char *) & (ECDSA->ENG_GX_PSx[0]), SECP224R1_Gx, 32);
		_memcpy((char *) & (ECDSA->ENG_GY_PSx[0]), SECP224R1_Gy, 32);
		_memcpy((char *) & (ECDSA->COR_A_Px[0]), SECP224R1_A, 32);
		_memcpy((char *) & (ECDSA->PRIME_Px[0]), SECP224R1_P, 32);
		_memcpy((char *) & (ECDSA->ORDER_N_Px[0]), SECP224R1_N, 32);
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_OTHERS);
		break;
	case ECDSA_ECP_BP256R1:
		curve->nbits = 256;
		curve->curve_bit = 256;
		curve->N = BRAINPOOLP256R1_N;
		_memcpy((char *) & (ECDSA->ENG_GX_PSx[0]), BRAINPOOLP256R1_Gx, 32);
		_memcpy((char *) & (ECDSA->ENG_GY_PSx[0]), BRAINPOOLP256R1_Gy, 32);
		_memcpy((char *) & (ECDSA->COR_A_Px[0]), BRAINPOOLP256R1_A, 32);
		_memcpy((char *) & (ECDSA->PRIME_Px[0]), BRAINPOOLP256R1_P, 32);
		_memcpy((char *) & (ECDSA->ORDER_N_Px[0]), BRAINPOOLP256R1_N, 32);
		ECDSA->ECR2 |= ECDSA_CURVE_SEL(ECDSA_ENG_OTHERS);
		break;
	default:
		return ECDSA_ERR_CURVE_NOT_SUPPORTED;
		break;
	}

	for (int i = 0; i < 8; i++) {
		ECDSA->ENG_X_Px[i] = 0;
		ECDSA->ENG_Y_Px[i] = 0;
		ECDSA->ENG_VERIFY_R_Px[i] = 0;
		ECDSA->ENG_VERIFY_S_Px[i] = 0;
		ECDSA->ENG_H_Px[i] = 0;
	}

	return 0;
}

__weak HAL_ROM_TEXT_SECTION
void ECDSA_EngRst(ECDSA_TypeDef *ECDSA)
{
	ECDSA->ECR1 |= ECDSA_BIT_REG_ENG_CLK_ENABLE | ECDSA_BIT_REG_ENG_RST;
	ECDSA->ECR1 &= (~ECDSA_BIT_REG_ENG_RST);
}

/**
  * @brief  enable the ecdsa engine or not
  * @param  status: ENABLE/DISABLE.
  * @retval NONE
  */
__weak HAL_ROM_TEXT_SECTION
void ECDSA_Cmd(ECDSA_TypeDef *ECDSA, u8 NewState)
{
	if (NewState != DISABLE) {
		/* Enable the peripheral */
		ECDSA->ECR1 |= ECDSA_BIT_REG_ENG_CLK_ENABLE;
		ECDSA->EISR |= ECDSA_ENG_START(1);
	} else {
		/* Disable the peripheral */
		ECDSA->ECR1 &= (~ECDSA_BIT_REG_ENG_CLK_ENABLE);
		ECDSA->EISR &= (~ECDSA_MASK_ENG_START);
	}
}

/**
  * @brief  generation key pair for target curve id and private key
  * @param  curve_id: the target curve.
  * @param  d: private key buf.
  * @param  Qx: output buf for x-coordinate of public key.
  * @param  Qy: output buf for y-coordinate of public key.
  * @param  OTPKey: 0 for not use OTP key while 1 for key 0 and 2 for key 1.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_KeyGen(ECDSA_TypeDef *ECDSA, u32 curve_id, u8 *d, u8 *Qx, u8 *Qy, u32 OTPKey)
{
	int i, ret = -1;
	u32 len;
	u32 ECDSA_TimeOut = 20000000;
	ecdsa_ecp_group curve;
	curve.curve_id = curve_id;

	ret = ECDSA_CurveSel(ECDSA, &curve);

	if (ret != 0) {
		return ret;
	}

	len = curve.nbits / 8;

	/* mode select , scalar multiplication mode */
	ECDSA->ECR2 &= (~ECDSA_MASK_REG_MODE_SEL);
	ECDSA->ECR2 |= ECDSA_REG_MODE_SEL(ECDSA_ENG_MODE2);
	ECDSA->ECR2 &= (~ECDSA_MASK_PRI_KEY_SEL);

	/* pri key */
	if (OTPKey > 2) {
		return ECDSA_ERR_OTP_KEYNUM;
	} else if (OTPKey != 0) {
		ECDSA->ECR2 |= ECDSA_PRI_KEY_SEL(OTPKey);
	} else {
		/* pri key */
		_memcpy((char *) & (ECDSA->ENG_H_Px[0]), d, len);
	}

	/* set base point, need? check */
	for (i = 0; i < 8; i++) {
		ECDSA->ENG_X_Px[i] = ECDSA->ENG_GX_PSx[i];
		ECDSA->ENG_Y_Px[i] = ECDSA->ENG_GY_PSx[i];
	}

	ECDSA_Cmd(ECDSA, ENABLE);

	while ((ECDSA->EISR & ECDSA_BIT_FINISH_INT) == 0) {
		ECDSA_TimeOut--;
		if (ECDSA_TimeOut == 0) {
			return ECDSA_ERR_KEYGEN_TIMEOUT;
		}
	}

	_memcpy(Qx, (char *) & (ECDSA->ENG_XO_Px[0]), len);
	_memcpy(Qy, (char *) & (ECDSA->ENG_YO_Px[0]), len);

	return 0;
}

/**
  * @brief  generation sign for target curve id and private key and hash
  * @param  curve_id: the target curve.
  * @param  d: private key buf.
  * @param  k: random num for sign.
  * @param  hbuf: input hash value buf.
  * @param  hlen: len of input hash value buf.
  * @param  Qx: output buf for x-coordinate of public key.
  * @param  Qy: output buf for y-coordinate of public key.
  * @param  r: output buf for sign r.
  * @param  s: output buf for sign s.
  * @param  OTPKey: 0 for not use OTP key while 1 for key 0 and 2 for key 1.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_Sign(ECDSA_TypeDef *ECDSA, u32 curve_id, u8 *d, u8 *k, const u8 *hbuf, u32 hlen, u8 *Qx, u8 *Qy, u8 *r, u8 *s, u32 OTPKey)
{
	u32 len;
	u32 e[8];
	u32 ECDSA_TimeOut = 20000000;
	ecdsa_ecp_group curve;
	int ret = -1;

	curve.curve_id = curve_id;
	ret = ECDSA_CurveSel(ECDSA, &curve);

	if (ret != 0) {
		return ret;
	}

	len = curve.nbits / 8;

	ECDSA_HashBuf_Setting(&curve, e, hbuf, hlen);

	/* mode select, sign mode */
	ECDSA->ECR2 &= (~ECDSA_MASK_REG_MODE_SEL);
	ECDSA->ECR2 |= ECDSA_REG_MODE_SEL(ECDSA_ENG_MODE1);
	ECDSA->ECR2 &= (~ECDSA_MASK_PRI_KEY_SEL);

	if (OTPKey > 2) {
		return ECDSA_ERR_OTP_KEYNUM;
	} else if (OTPKey != 0) {
		ECDSA->ECR2 |= ECDSA_PRI_KEY_SEL(OTPKey);
	} else {
		/* pri key */
		_memcpy((char *) & (ECDSA->ENG_X_Px[0]), d, len);
	}

	/*pay attention to key buf, input buf must no smalller than len, paddig 0 */
	_memcpy((char *) & (ECDSA->ENG_X_Px[0]), d, len);
	_memcpy((char *) & (ECDSA->ENG_Y_Px[0]), k, len);
	_memcpy((char *) & (ECDSA->ENG_H_Px[0]), e, 32);

	ECDSA_Cmd(ECDSA, ENABLE);

	ECDSA->EISR |= ECDSA_BIT_HASH_OK;

	while ((ECDSA->EISR & ECDSA_BIT_FINISH_INT) == 0) {
		ECDSA_TimeOut--;
		if (ECDSA_TimeOut == 0) {
			return ECDSA_ERR_SIGN_TIMEOUT;
		} else if ((ECDSA->EISR & (ECDSA_MASK_SIGN_ERR | ECDSA_BIT_SHARE_KEY_ERR | ECDSA_BIT_INFINITY_ERR)) != 0) {
			return ECDSA_ERR_SIGN_ERROR;
		}
	}

	if (Qx != NULL) {
		_memcpy(Qx, (char *) & (ECDSA->ENG_XO_Px[0]), len);
	}
	if (Qy != NULL) {
		_memcpy(Qy, (char *) & (ECDSA->ENG_YO_Px[0]), len);
	}
	_memcpy(r, (char *) & (ECDSA->ENG_SIGN_R_Sx[0]), len);
	_memcpy(s, (char *) & (ECDSA->ENG_SIGN_S_Sx[0]), len);

	return 0;
}

/**
  * @brief  Verify sign for target curve id and public key and hash
  * @param  curve_id: the target curve.
  * @param  hbuf: input hash value buf.
  * @param  hlen: len of input hash value buf.
  * @param  Qx: input buf for x-coordinate of public key.
  * @param  Qy: input buf for y-coordinate of public key.
  * @param  r: input buf for sign r.
  * @param  s: input buf for sign s.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_Verify(ECDSA_TypeDef *ECDSA, u32 curve_id, const u8 *hbuf, u32 hlen, u8 *Qx, u8 *Qy, u8 *r, u8 *s)
{
	u32 len;
	u32 e[8];
	u32 ECDSA_TimeOut = 20000000;
	int ret = -1;

	ecdsa_ecp_group curve;

	curve.curve_id = curve_id;
	ret = ECDSA_CurveSel(ECDSA, &curve);

	if (ret != 0) {
		return ret;
	}

	len = curve.nbits / 8;

	/* mode select */
	ECDSA->ECR2 &= (~ECDSA_MASK_REG_MODE_SEL);
	ECDSA->ECR2 |= ECDSA_REG_MODE_SEL(ECDSA_ENG_MODE0);

	ECDSA_HashBuf_Setting(&curve, e, hbuf, hlen);

	/*pay attention to key and sign buf, input buf must no smalller than len, paddig 0 */
	_memcpy((char *) & (ECDSA->ENG_X_Px[0]), Qx, len);
	_memcpy((char *) & (ECDSA->ENG_Y_Px[0]), Qy, len);
	_memcpy((char *) & (ECDSA->ENG_VERIFY_R_Px[0]), r, len);
	_memcpy((char *) & (ECDSA->ENG_VERIFY_S_Px[0]), s, len);
	_memcpy((char *) & (ECDSA->ENG_H_Px[0]), e, 32);

	ECDSA_Cmd(ECDSA, ENABLE);

	ECDSA->EISR |= ECDSA_BIT_HASH_OK;

	while ((ECDSA->EISR & ECDSA_BIT_FINISH_INT) == 0) {
		ECDSA_TimeOut--;
		if (ECDSA_TimeOut == 0) {
			return ECDSA_ERR_VERIFY_TIMEOUT;
		} else if ((ECDSA->EISR & (ECDSA_MASK_VERIFY_ERR | ECDSA_BIT_SHARE_KEY_ERR | ECDSA_BIT_INFINITY_ERR)) != 0) {
			return ECDSA_ERR_VERIFY_ERROR;
		}
	}

	if ((ECDSA->EISR & ECDSA_BIT_VERIFY_PASS) == 0) {
		return ECDSA_ERR_VERIFY_FAIL;
	} else {
		return 0;
	}
}

/**
  * @brief  ECDH for for target curve id, z = Q *d
  * @param  curve_id: the target curve.
  * @param  zx: output buf for x-coordinate of public key.
  * @param  zy: output buf for y-coordinate of public key, can be null if y-coordinate is not needed.
  * @param  Q: input buf for pubkey.
  * @param  d: input buf for private key.
  * @retval 0 success
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_ECDH_Compute_Shared(ECDSA_TypeDef *ECDSA, u32 curve_id, u8 *zx, u8 *zy, u8 *Qx, u8 *Qy, u8 *d, u32 OTPKey)
{
	u32 len;
	u32 ECDSA_TimeOut = 20000000;
	int ret = -1;

	ecdsa_ecp_group curve;

	curve.curve_id = curve_id;
	ret = ECDSA_CurveSel(ECDSA, &curve);
	if (ret != 0) {
		return ret;
	}
	len = curve.nbits / 8;

	/* mode select */
	ECDSA->ECR2 &= (~ECDSA_MASK_REG_MODE_SEL);
	ECDSA->ECR2 |= ECDSA_REG_MODE_SEL(ECDSA_ENG_MODE2);
	ECDSA->ECR2 &= (~ECDSA_MASK_PRI_KEY_SEL);

	/*pay attention to key and sign buf, input buf must no smalller than len, paddig 0 */
	_memcpy((char *) & (ECDSA->ENG_X_Px[0]), Qx, len);
	_memcpy((char *) & (ECDSA->ENG_Y_Px[0]), Qy, len);

	if (OTPKey > 2) {
		return ECDSA_ERR_OTP_KEYNUM;
	} else if (OTPKey != 0) {
		ECDSA->ECR2 |= ECDSA_PRI_KEY_SEL(OTPKey);
	} else {
		/* pri key */
		_memcpy((char *) & (ECDSA->ENG_H_Px[0]), d, len);
	}

	ECDSA_Cmd(ECDSA, ENABLE);

	while ((ECDSA->EISR & ECDSA_BIT_FINISH_INT) == 0) {
		ECDSA_TimeOut--;
		if (ECDSA_TimeOut == 0) {
			return ECDSA_ERR_VERIFY_TIMEOUT;
		} else if ((ECDSA->EISR & (ECDSA_MASK_VERIFY_ERR | ECDSA_BIT_SHARE_KEY_ERR | ECDSA_BIT_INFINITY_ERR)) != 0) {
			return ECDSA_ERR_VERIFY_ERROR;
		}
	}

	_memcpy(zx, (char *) & (ECDSA->ENG_XO_Px[0]), len);
	if (zy != NULL) {
		_memcpy(zy, (char *) & (ECDSA->ENG_YO_Px[0]), len);
	}

	return 0;
}

/**
  * @brief R = (X mode Y) mod p, p is the para of 256k1
  * @param  X: input buf, should be 256 bit, must < P.
  * @param  Y: input buf, should be 256 bit, must < P.
  * @param  mode: can be one of ECDSA_BASIC_FUNCTION_definitions .
  * @param  R: output buf, should be 256 bit.
  * @retval 0 success
  * @note: Div mode is for Modular Inverse Algorithm, X < Y is needed.
  */
__weak HAL_ROM_TEXT_SECTION
int ECDSA_Basic_Func(ECDSA_TypeDef *ECDSA,  u32 X[8], u32 Y[8], u8 R[32], int mode)
{
	u32 ECDSA_TimeOut = 20000000;
	int ret = -1;

	ecdsa_ecp_group curve;

	if (ECDSA_BufCmp(X, 8, SECP256K1_P) >= 0 || ECDSA_BufCmp(Y, 8, SECP256K1_P) >= 0) {
		return ECDSA_ERR_BASIC_FUNC_ERROR_PARA;
	}
	/* default 256k1 */
	curve.curve_id = MBEDTLS_ECP_DP_SECP256K1;
	ret = ECDSA_CurveSel(ECDSA, &curve);

	if (ret != 0) {
		return ret;
	}

	/* choose basic function */
	ECDSA->ECR2 &= (~ECDSA_MASK_REG_MODE_SEL);
	ECDSA->ECR2 |= ECDSA_REG_MODE_SEL(ECDSA_ENG_MODE3);

	/* choose mode */
	ECDSA->ECR2 &= (~ECDSA_MASK_REG_FUNC_SEL);
	ECDSA->ECR2 |= ECDSA_REG_FUNC_SEL(mode);

	/*pay attention to input buf X and Y, should be 256 bits, paddig 0 if need */
	_memcpy((char *) & (ECDSA->ENG_X_Px[0]), X, 32);
	_memcpy((char *) & (ECDSA->ENG_Y_Px[0]), Y, 32);

	ECDSA_Cmd(ECDSA, ENABLE);

	while ((ECDSA->EISR & ECDSA_BIT_FINISH_INT) == 0) {
		ECDSA_TimeOut--;
		if (ECDSA_TimeOut == 0) {
			return ECDSA_ERR_BASIC_FUNC_TIMEOUT;
		} else if ((ECDSA->EISR & (ECDSA_MASK_VERIFY_ERR | ECDSA_BIT_SHARE_KEY_ERR | ECDSA_BIT_INFINITY_ERR)) != 0) {
			return ECDSA_ERR_BASIC_FUNC_ERROR;
		}
	}

	_memcpy(R, (char *) & (ECDSA->ENG_XO_Px[0]), 32);
	return 0;
}
