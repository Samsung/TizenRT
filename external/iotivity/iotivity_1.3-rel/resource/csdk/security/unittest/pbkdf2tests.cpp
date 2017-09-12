 /******************************************************************
  *
  * Copyright 2016 Microsoft Corporation
  *
  *
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************/

#include <gtest/gtest.h>
#include "pbkdf2.h"
#include <stdlib.h>
#include <stdint.h>
#include <cinttypes>

/*
 * Tests for the PBKDF2 implementation. 
 * There is a disabled test here that generates test vectors. Test vectors 
 * for PBKDF2-HMAC-SHA256 are not generally available, so we created our own to
 * ensure consistency when changing the PBKDF2 implementation (initially based 
 * on tiny DTLS, now moving to mbedTLS).
 */

static void print_buffer(const char* label, const uint8_t* buffer, size_t len)
{
    printf("%s:", label);

    if (len == 0)
    {
        printf(" NULL\n");
        return;
    }
    
    printf("{");
    for (size_t i = 0; i < len - 1; i++)
    {
        printf("0x%02X, ", buffer[i]);
    }
    printf("0x%02X}\n", buffer[len-1]);

}

static void print_vector(const char* label, 
    const unsigned char* passwd, size_t pLen,
    const uint8_t* salt, const size_t saltLen,
    const size_t iterations,
    const size_t keyLen, uint8_t* derivedKey)
{
    printf("%s\n", label);
    print_buffer("Password", passwd, pLen);
    print_buffer("Salt", salt, saltLen);
    printf("Iterations: %" PRIuPTR "\n", iterations);
    print_buffer("Derived Key", derivedKey, keyLen);
}

TEST(PBKDF2Tests, DISABLED_CreateTestVectors)
{
    unsigned char passwd[8] = { '1', '2', '3', '4', '5', '6', '7', '8' };
    uint8_t salt[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
    uint8_t derivedKey[16];

    /* Create and output test vector 1 */
    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        salt, sizeof(salt),
        PBKDF_ITERATIONS,
        sizeof(derivedKey), derivedKey);

    EXPECT_EQ(ret, 0);
    print_vector("PBKDF2 Test Vector 1:", passwd, sizeof(passwd), salt, sizeof(salt), PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    printf("\n");

    /* Create and output test vector 2 */
    ret = DeriveCryptoKeyFromPassword(passwd, 1, salt, 1, 1, 4, derivedKey);
    EXPECT_EQ(ret, 0);
    print_vector("PBKDF2 Test Vector 2:", passwd, 1, salt, 1, 1, 4, derivedKey);
    printf("\n");

    /* Create and output test vector 3 */
    uint8_t lgDerivedKey[65];   /* two SHA-256 blocks + one byte */
    ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd), salt, sizeof(salt), PBKDF_ITERATIONS, sizeof(lgDerivedKey), lgDerivedKey);
    EXPECT_EQ(ret, 0);
    print_vector("PBKDF2 Test Vector 3:", passwd, sizeof(passwd), salt, sizeof(salt), PBKDF_ITERATIONS, sizeof(lgDerivedKey), lgDerivedKey);
    printf("\n");

    /* Create and output test vector 4 */
    uint8_t lgSalt[48];
    memset(lgSalt, 0x42, sizeof(lgSalt));
    ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd), lgSalt, sizeof(lgSalt), PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    EXPECT_EQ(ret, 0);
    print_vector("PBKDF2 Test Vector 4:", passwd, sizeof(passwd), lgSalt, sizeof(lgSalt), PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    printf("\n");

    /* Create and output test vector 5 */
    ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd), NULL, 0, PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    EXPECT_EQ(ret, 0);
    print_vector("PBKDF2 Test Vector 5:", passwd, sizeof(passwd), NULL, 0, PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    printf("\n");

    /* Create and output test vector 6 */
    uint8_t lgPasswd[48];
    memset(lgPasswd, 0x42, sizeof(lgPasswd));
    ret = DeriveCryptoKeyFromPassword(lgPasswd, sizeof(lgPasswd), salt, sizeof(salt), PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    EXPECT_EQ(ret, 0);
    print_vector("PBKDF2 Test Vector 6:", lgPasswd, sizeof(lgPasswd), salt, sizeof(salt), PBKDF_ITERATIONS, sizeof(derivedKey), derivedKey);
    printf("\n");
}

TEST(PBKDF2Tests, TestVector1)
{
    unsigned char passwd[8] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
    uint8_t salt[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00 };
    size_t iterations = 1000;
    uint8_t derivedKey[16] = { 0xB2, 0x1A, 0xA9, 0x99, 0x87, 0x40, 0xC1, 0x9E, 0x55, 0xD7, 0xFD, 0xED, 0xB2, 0x5C, 0xCE, 0x75 };
    uint8_t reDerivedKey[16] = {};

    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        salt, sizeof(salt),
        iterations,
        sizeof(reDerivedKey), reDerivedKey);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(memcmp(derivedKey, reDerivedKey, sizeof(derivedKey)), 0);
}

TEST(PBKDF2Tests, TestVector2)
{
    unsigned char passwd[1] = { 0x31 };
    uint8_t salt[1] = { 0x01 };
    size_t iterations = 1;
    uint8_t derivedKey[4] = { 0x20, 0xA3, 0x2A, 0x13 };
    uint8_t reDerivedKey[4] = {};

    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        salt, sizeof(salt),
        iterations,
        sizeof(reDerivedKey), reDerivedKey);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(memcmp(derivedKey, reDerivedKey, sizeof(derivedKey)), 0);
}


TEST(PBKDF2Tests, TestVector3)
{
    unsigned char passwd[8] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
    uint8_t salt[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00 };
    size_t iterations = 1000;
    uint8_t derivedKey[65] = { 0xB2, 0x1A, 0xA9, 0x99, 0x87, 0x40, 0xC1, 0x9E, 0x55, 0xD7, 0xFD, 0xED, 0xB2, 0x5C, 0xCE, 0x75, 0x2E, 0xA1, 0xA0, 0x43, 0x71, 0x0F, 0xE7, 0xD0, 0xF4, 0xF6, 0x79, 0x01, 0xB5, 0x47, 0x72, 0x20, 0xC0, 0x14, 0x25, 0x5E, 0x4B, 0xA7, 0x40, 0xEC, 0x6A, 0x29, 0x55, 0x0B, 0xDC, 0x2C, 0x10, 0x87, 0xD3, 0xE7, 0x4F, 0xF7, 0xD8, 0x55, 0x64, 0x95, 0xA8, 0x63, 0x6E, 0x64, 0x0E, 0x63, 0xE1, 0x8E, 0xC9 };
    uint8_t reDerivedKey[65] = {};

    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        salt, sizeof(salt),
        iterations,
        sizeof(reDerivedKey), reDerivedKey);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(memcmp(derivedKey, reDerivedKey, sizeof(derivedKey)), 0);
}

TEST(PBKDF2Tests, TestVector4)
{
    unsigned char passwd[8] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
    uint8_t salt[48] = { 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42 };
    size_t iterations = 1000;
    uint8_t derivedKey[16] = { 0x14, 0x01, 0x5B, 0x8F, 0x23, 0x00, 0x57, 0x4F, 0x48, 0x7B, 0x0A, 0x85, 0x79, 0xFD, 0x3C, 0x31 };
    uint8_t reDerivedKey[16] = {};

    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        salt, sizeof(salt),
        iterations,
        sizeof(reDerivedKey), reDerivedKey);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(memcmp(derivedKey, reDerivedKey, sizeof(derivedKey)), 0);
}

TEST(PBKDF2Tests, TestVector5)
{
    unsigned char passwd[8] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
    size_t iterations = 1000;
    uint8_t derivedKey[16] = { 0x8D, 0x9B, 0x3E, 0x30, 0xED, 0x2C, 0x67, 0x8A, 0x58, 0x1A, 0x2D, 0x40, 0x7A, 0x47, 0xB4, 0x05 };
    uint8_t reDerivedKey[16] = {};

    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        NULL, 0,
        iterations,
        sizeof(reDerivedKey), reDerivedKey);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(memcmp(derivedKey, reDerivedKey, sizeof(derivedKey)), 0);
}

TEST(PBKDF2Tests, TestVector6)
{
    unsigned char passwd[48] = { 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42 };
    uint8_t salt[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x00 };
    size_t iterations = 1000;
    uint8_t derivedKey[16] = { 0x03, 0xB4, 0x80, 0xEE, 0x1B, 0x2D, 0x55, 0xA8, 0x90, 0x88, 0xB5, 0xCE, 0x9F, 0x1C, 0x4E, 0x40 };
    uint8_t reDerivedKey[sizeof(derivedKey)] = {};

    int ret = DeriveCryptoKeyFromPassword(passwd, sizeof(passwd),
        salt, sizeof(salt),
        iterations,
        sizeof(reDerivedKey), reDerivedKey);

    EXPECT_EQ(ret, 0);
    EXPECT_EQ(memcmp(derivedKey, reDerivedKey, sizeof(derivedKey)), 0);
}