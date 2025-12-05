// Copyright 2022-2025 Beken
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

#include "psa/crypto.h"
#include "psa/crypto_sizes.h"
#include "psa/crypto_values.h"
#include "mbedtls/bignum.h"
#include "tfm_dh_nsc.h"
#include "tfm_secure_api.h"
#include "psa_crypto_random_impl.h"
#include <os/mem.h>
#include <string.h>

extern unsigned char *P_TEST;
extern unsigned char *G_TEST;
extern size_t P_TEST_size;
extern size_t G_TEST_size;

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_dh_nsc_stub(void)
{
	return;
}

__tfm_psa_secure_gateway_no_naked_attributes__  int psa_dh_import_p_g(dh_type_t *dh_param)
{

    if (dh_param == NULL || dh_param->P == NULL || dh_param->G == NULL) {
        printf("Invalid DH parameter\r\n");
        return PSA_ERROR_INVALID_ARGUMENT;
    }

    P_TEST = os_malloc(dh_param->P_size);
    if (P_TEST == NULL) {
        printf("malloc P failed\r\n");
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }
    P_TEST_size = dh_param->P_size;

    os_memset(P_TEST, 0, dh_param->P_size);
    os_memcpy(P_TEST, dh_param->P, dh_param->P_size);

    G_TEST = os_malloc(dh_param->G_size);
    if (G_TEST == NULL) {
        printf("malloc G failed\r\n");
        os_free(P_TEST);
        return PSA_ERROR_INSUFFICIENT_MEMORY;
    }
    G_TEST_size = dh_param->G_size;

    os_memset(G_TEST, 0, dh_param->G_size);
    os_memcpy(G_TEST, dh_param->G, dh_param->G_size);

	return PSA_SUCCESS;
}

__tfm_psa_secure_gateway_no_naked_attributes__ int psa_dh_mem_free(void)
{
    if (P_TEST != NULL) {
        os_free(P_TEST);
        P_TEST = NULL;
        P_TEST_size = 0;
    }

    if (G_TEST != NULL) {
        os_free(G_TEST);
        G_TEST = NULL;
        G_TEST_size = 0;
    }

    return PSA_SUCCESS;
}