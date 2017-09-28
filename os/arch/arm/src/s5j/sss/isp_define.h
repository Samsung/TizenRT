/*!
 *  @file       isp_define.h
 *  @brief      Headerfile : definitions for ISP
 *  @author     jinsu.hyun
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v0.90 : 2016.9.03 2nd.  release version
 *
 */

#ifndef ISP_DEFINE_H
#define ISP_DEFINE_H_

//! ********** Constant Variables **********
#define MAX_KEY_WSIZE 17
#define MAX_SIGNATURE_WSIZE 17

//! ********** Index Variables **********

// Hash Function

#define SHA1                (0x1)
#define SHA2                (0x2)
#define SHA3                (0x3)

#define HASH160             (0x1)
#define HASH224             (0x2)
#define HASH256             (0x3)
#define HASH384             (0x4)
#define HASH512             (0x5)
#define HMAC                (0x1)

#define SHA1_160            (((SHA1 << 4) | (HASH160)) << 8)  //0x00001100
#define SHA2_224            (((SHA2 << 4) | (HASH224)) << 8)  //0x00002200
#define SHA2_256            (((SHA2 << 4) | (HASH256)) << 8)  //0x00002300
#define SHA2_384            (((SHA2 << 4) | (HASH384)) << 8)  //0x00002400
#define SHA2_512            (((SHA2 << 4) | (HASH512)) << 8)  //0x00002500
#define SHA3_224            (((SHA3 << 4) | (HASH224)) << 8)  //0x00003200
#define SHA3_256            (((SHA3 << 4) | (HASH256)) << 8)  //0x00003300
#define SHA3_384            (((SHA3 << 4) | (HASH384)) << 8)  //0x00003400
#define SHA3_512            (((SHA3 << 4) | (HASH512)) << 8)  //0x00003500

#define HMAC_SHA1_160       ((((SHA1 << 4) | (HASH160)) << 8) | (HMAC) << 16)  //0x00011100
#define HMAC_SHA2_256       ((((SHA2 << 4) | (HASH256)) << 8) | (HMAC) << 16)  //0x00012300
#define HMAC_SHA2_384       ((((SHA2 << 4) | (HASH384)) << 8) | (HMAC) << 16)  //0x00012400
#define HMAC_SHA2_512       ((((SHA2 << 4) | (HASH512)) << 8) | (HMAC) << 16)  //0x00012500

// ECC Algorithm ID
#define ECDSA               (0x1)
#define ECDH                (0x2)
#define PKGEN               (0x3)

// ECC Curve ID
#define ECC192              (0x1)
#define ECC224              (0x2)
#define ECC256              (0x3)
#define ECC384              (0x4)
#define ECC521              (0x5)

#define ECDSA_192           ((ECDSA << 4) | (ECC192))  //0x11
#define ECDSA_224           ((ECDSA << 4) | (ECC224))  //0x12
#define ECDSA_256           ((ECDSA << 4) | (ECC256))  //0x13
#define ECDSA_384           ((ECDSA << 4) | (ECC384))  //0x14
#define ECDSA_521           ((ECDSA << 4) | (ECC521))  //0x15

#define ECDH_192            ((ECDH << 4) | (ECC192))   //0x21
#define ECDH_224            ((ECDH << 4) | (ECC224))   //0x22
#define ECDH_256            ((ECDH << 4) | (ECC256))   //0x23
#define ECDH_384            ((ECDH << 4) | (ECC384))   //0x24
#define ECDH_521            ((ECDH << 4) | (ECC521))   //0x25

#define GET_ECC_CURVE(OID)  ((OID) & 0x07)
#define GET_ECC_ALG(OID)    ((OID >> 4) & 0x0F)

#define GET_HASH_SIZE(OID)  ((OID >> 8) & 0x07)
#define GET_HASH_ALG(OID)   ((OID >> 12) & 0x0F)

#define Is_HMAC_ALG(OID)    ((OID >> 16) & 0x0F)

#define Is_ECC_BP(OID)      (OID & 0x40)

#define RSP_FAIL            (0xF1)
#define RSP_SUCCESS         (0xA1)

#endif							/*ISP_DEFINE_H_ */
