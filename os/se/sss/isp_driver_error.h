/*!
 *  @file       isp_driver_error.h
 *  @brief      Headerfile : isp driver for error messages
 *  @author     jinsu.hyun
 *  @version    v0.01 : 2015.7.7
 *  @version    v0.10 : 2016.7.15 pre-beta release
 *  @version    v0.50 : 2016.8.13 Init. release version
 *  @version    v1.00 : 2016.9.27 release version v1.0
 */

#ifndef ISP_DRIVER_ERROR_H_
#define ISP_DRIVER_ERROR_H_

// AES
#define ERROR_AES_INVALID_BLOCK_LEN                         (0x000133D1)
#define ERROR_AES_INVALID_RSP_BLOCK_LEN                     (0x000233D1)
#define ERROR_AES_INVALID_MSG_LEN                           (0x000333D1)
#define ERROR_AES_INVALID_MODE                              (0x000433D1)
#define ERROR_AES_INVALID_KEY_LEN                           (0x000533D1)
#define ERROR_AES_INVALID_AAD_LEN                           (0x000633D1)
#define ERROR_AES_INVALID_IV_LEN                            (0x000733D1)
#define ERROR_AES_INVALID_INDEX                             (0x000833D1)
#define ERROR_AES_INVALID_TAG_LEN                           (0x000933D1)

// Hash
#define ERROR_HASH_INVALID_MODE                             (0x000131D1)
#define ERROR_HASH_INVALID_BLOCK_LEN                        (0x000231D1)
#define ERROR_HASH_INVALID_MSG_LEN                          (0x000331D1)

// HMAC
#define ERROR_HMAC_INVALID_MODE                             (0x000132D1)
#define ERROR_HMAC_INVALID_KEY_LEN                          (0x000232D1)
#define ERROR_HMAC_INVALID_BLOCK_LEN                        (0x000332D1)
#define ERROR_HMAC_INVALID_INDEX                            (0x000432D1)
#define ERROR_HMAC_INVALID_RSP_BLOCK_LEN                    (0x000532D1)

// DH
#define ERROR_DH_INVALID_PRIME_LEN                          (0x000125D1)
#define ERROR_DH_INVALID_PUBKEY_LEN                         (0x000225D1)
#define ERROR_DH_INVALID_GENERATOR_LEN                      (0x000325D1)
#define ERROR_DH_INVALID_PRIME                              (0x000425D1)
#define ERROR_DH_INVALID_PUBKEY                             (0x000525D1)
#define ERROR_DH_INVALID_GENERATOR                          (0x000625D1)

// ECDSA
#define ERROR_ECDSA_INVALID_MSG_LEN                         (0x000111D1)
#define ERROR_ECDSA_INVALID_SIGNATURE_LEN                   (0x000311D1)
#define ERROR_ECDH_INVALID_PUBKEY_LEN                       (0x000411D1)
#define ERROR_ECDH_INVALID_PUBKEY                           (0x000511D1)

// RSA
#define ERROR_RSA_INVALID_CIPHER_LEN                        (0x000151D1)
#define ERROR_RSA_INVALID_MSG_LEN                           (0x000251D1)
#define ERROR_RSA_INVALID_SIGN_LEN                          (0x000351D1)
#define ERROR_RSA_INVALID_PUKEY                             (0x000551D1)
#define ERROR_RSA_INVALID_PAD_SELECTION                     (0x000451D1)

// RNG
#define ERROR_RNG_INVALID_RANDOM_REQUEST                    (0x000161D1)

// Common
#define ERROR_SSKeyID_InputID_MISSMATCH                     (0x000171D1)
#define ERROR_INVALID_OID                                   (0x000271D1)

// Secure Storage
#define ERROR_SSTORAGE_INVALID_SLOT_INDEX                   (0x00F1A1D1)
#define ERROR_SSTORAGE_INVALID_DATA_LEN                     (0x00F2A1D1)
#define ERROR_SSTORAGE_INVALID_TYPE                         (0x00F3A1D1)
#define ERROR_SSTORAGE_DATA_INVALID_DATA_LEN                (0x0002A1D1)
#define ERROR_SSTORAGE_CERT_INVALID_DATA_LEN                (0x0003A2D1)
#define ERROR_SSTORAGE_KEY_INVALID_DATA_LEN                 (0x0004A3D1)
#define ERROR_SSTORAGE_KEY_INVALID_KEY_LEN                  (0x0005A3D1)
#define ERROR_SSTORAGE_KEY_INVALID_KEY_TYPE                 (0x0006A3D1)
#define ERROR_SSTORAGE_FACTORYKEY_PBKEY_INVALID_DATA_LEN    (0x0007A2D1)
#define ERROR_SSTORAGE_QSPI_WRITE                           (0x0008A2D1)
#define ERROR_SSTORAGE_SFS_FOPEN                            (0x0009A2D1)
#define ERROR_SSTORAGE_SFS_FSEEK                            (0x000AA2D1)
#define ERROR_SSTORAGE_SFS_FREAD                            (0x000BA2D1)
#define ERROR_SSTORAGE_SFS_FWRITE                           (0x000CA2D1)
// System Function
#define ERROR_SYSTEM_INVALID_DATA_LEN                       (0x000201D1)
#define ERROR_SYSTEM_MAILBOX_BUSY                           (0x000100D1)

// Error from FW
#define FW_ERROR_ISP_INVALID_FUNCID00                       (0x801000F1)
#define FW_ERROR_ISP_INVALID_FUNCID01                       (0x801100F1)
#define FW_ERROR_ISP_INVALID_FUNCID02                       (0x801200F1)
#define FW_ERROR_ISP_INVALID_FUNCID03                       (0x801300F1)
#define FW_ERROR_ISP_INVALID_DATASIZE                       (0x801400F1)
#define FW_ERROR_ISP_FW_BODYSIZE                            (0x801500F1)
#define FW_ERROR_ISP_FW_ROLLBACK_CNT                        (0x801600F1)
#define FW_ERROR_ISP_FW_INVALID_PUBLICKEY                   (0x801700F1)
#define FW_ERROR_ISP_RESTORE_INTEGRITY_FAIL                 (0x801800F1)
#define FW_ERROR_ISP_IP_BUSY                                (0x801900F1)
#define FW_ERROR_ISP_SRAM_CMD_NOT_SUPPORTED                 (0x801A00F1)

#define FW_ERROR_INVALID_FUNCTION                           (0x000100f1)
#define FW_ERROR_FW_VERIFY                                  (0x001400f1)
#define FW_ERROR_RESTORE_FAIL                               (0x001800f1)
#define FW_ERROR_IP_BUSY                                    (0x001900f1)
#define FW_ERROR_INVALID_OID                                (0x003000f1)
#define FW_ERROR_INVALID_INPUT                              (0x003400f1)
#define FW_ERROR_INPUT_SETTING                              (0x004000f1)
#define FW_ERROR_PRNG                                       (0x005000f1)
#define FW_FAIL_INVALID_SIGNATURE                           (0x006000f1)
#define FW_FAIL_INFINITY_POINT                              (0x006100f1)
#define FW_FAIL_NOT_ON_ECC_CURVE                            (0x006200f1)

// SRAM error
#define FW_ERROR_INVALID_EXEC_ORDER                         (0x80AF00F1)
#define FW_ERROR_OVER_VALID_RSA_MSGLEN                      (0x803500F1)
#define FW_ERROR_INVALID_RSA_MODLEN                         (0x803600F1)
#define FW_ERROR_Input_Public_is_not_odd                    (0x803700F1)
#define FW_ERROR_OVER_VALID_RSA_Saltlen                     (0x803800F1)
#define FW_ERROR_NO_PUKEY                                   (0x803900F1)
#define FW_FAIL_OVER_MR_TRIALS                              (0x805600F1)
#define FW_FAIL_OVER_DH_RETRIALS                            (0x805600F1)
#define FW_FAIL_OVER_ECC_RETRIALS                           (0x805600F1)
#define FW_FAIL_NO_OUTPUT_KEY                               (0x805700F1)
#define FW_FAIL_OVER_GEN_RETRIALS                           (0x805800F1)
#define FW_ERROR_INVALID_RSASIGNATURE_0xBC                  (0x806400F1)
#define FW_ERROR_INVALID_RSASIGNATURE_lsb                   (0x806500F1)
#define FW_ERROR_INVALID_RSANONZERO_PS                      (0x806600F1)
#define FW_ERROR_INVALID_RSADB_SPLITTER                     (0x806700F1)
#define FW_ERROR_INVALID_SIGNATURE_BLEN                     (0x806800F1)
#define FW_ERROR_INVALID_CIPHER                             (0x806900F1)
#define FW_ERROR_INVALID_SEQUENCE                           (0x80ff00F1)
#define FW_ERROR_DER2INT_PARSE                              (0x80fe00F1)
#define FW_ERROR_AES_INVALID_KEYSEL                         (0x80D000F1)
#define FW_ERROR_AES_INVALID_KEY_LEN                        (0x80D100F1)
#define FW_ERROR_AES_INVALID_DIR_MODE                       (0x80D200F1)
#define FW_ERROR_AES_INVLIAD_SWAP                           (0x80D300F1)
#define FW_ERROR_AES_INVALID_MODE                           (0x80D400F1)
#define FW_ERROR_AES_INVALID_TAG                            (0x80D500F1)
#define FW_ERROR_AES_INVALID_BLOCK_LEN                      (0x80D600F1)
#define FW_ERROR_AES_KM_BUSY                                (0x80D700F1)
#define FW_ERROR_AES_KM_INIT                                (0x80D800F1)
#define FW_ERROR_SSTORAGE_DATA_INVALID_SLOT_INDEX           (0x80E100F1)
#define FW_ERROR_SSTORAGE_CERT_INVALID_SLOT_INDEX           (0x80E200F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_KEY_LEN               (0x80E300F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_MODE                  (0x80E400F1)
#define FW_ERROR_SSTORAGE_INVALID_DATA_TYPE                 (0x80E500F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_KEYTYPE               (0x80E600F1)
#define FW_ERROR_SSTORAGE_KEY_INVALID_SLOT_INDEX            (0x80E700F1)
#define FW_ERROR_SSTORAGE_FACTORYKEY_INVALID_HMAC           (0x80E800F1)
#define FW_ERROR_SSTORAGE_FACTORYKEY_INVALID_ENCODING       (0x80E900F1)
#define FW_ERROR_SSTORAGE_FACTORYKEY_INVALID_KEYTYPE        (0x80EA00F1)
#define FW_ERROR_RNG_INVALID_LEN                            (0x80f100F1)
#define FW_ERROR_KEYGEN_INVALID_KEYLEN                      (0x80f200F1)
#define FW_ERROR_PKA_IP_BUSY                                (0x80B100F1)
#define FW_ERROR_HASH_IP_BUSY                               (0x80B200F1)
#define FW_ERROR_PRNG_IP_BUSY                               (0x80B300F1)

#endif							/* ISP_DRIVER_ERROR_H_ */
