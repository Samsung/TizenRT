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


#include <stdio.h>
#include <string.h>
#include <os/os.h>
#include <os/mem.h>
#include "tfm_secure_api.h"
#include "mbedtls/ecp.h"
#include "tfm_ecp_nsc.h"

#define ECP_TAG "ECP"

#define ECP_LOGI(...) BK_LOGI(ECP_TAG, ##__VA_ARGS__)
#define ECP_LOGW(...) BK_LOGW(ECP_TAG, ##__VA_ARGS__)
#define ECP_LOGE(...) BK_LOGE(ECP_TAG, ##__VA_ARGS__)
#define ECP_LOGD(...) BK_LOGD(ECP_TAG, ##__VA_ARGS__)

#define ECP_DEBUG_LOG    (0)
#define ciL              (sizeof(mbedtls_mpi_uint)) 

/* Calling this API in tfm_s.bin to trigger the compiler to generate NSC symbols in tfm_s_veneers.o.
 **/
void psa_ecp_nsc_stub(void)
{
	return;
}

#define BYTES_TO_T_UINT_4( a, b, c, d )                       \
		( (mbedtls_mpi_uint) (a) <<  0 ) |						  \
		( (mbedtls_mpi_uint) (b) <<  8 ) |						  \
		( (mbedtls_mpi_uint) (c) << 16 ) |						  \
		( (mbedtls_mpi_uint) (d) << 24 )

#define BYTES_TO_T_UINT_2( a, b )                   \
		BYTES_TO_T_UINT_4( a, b, 0, 0 )

#define BYTES_TO_T_UINT_8( a, b, c, d, e, f, g, h ) \
		BYTES_TO_T_UINT_4( a, b, c, d ),				\
		BYTES_TO_T_UINT_4( e, f, g, h )

#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
static const mbedtls_mpi_uint secp192r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
};
#endif


#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
static const mbedtls_mpi_uint secp224r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
};
#endif
#endif /* CONFIG_FULL_SUPPLICANT */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
static const mbedtls_mpi_uint secp256r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
    BYTES_TO_T_UINT_8( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ),
    BYTES_TO_T_UINT_8( 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF ),
};
#endif

#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
static const mbedtls_mpi_uint secp384r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00 ),
    BYTES_TO_T_UINT_8( 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
};
#endif

#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
static const mbedtls_mpi_uint secp521r1_a[] = {
    BYTES_TO_T_UINT_8( 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF ),
    BYTES_TO_T_UINT_8( 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ),
};
#endif
#endif /* CONFIG_FULL_SUPPLICANT */
/*
 * Create an MPI from embedded constants
 * (assumes len is an exact multiple of sizeof mbedtls_mpi_uint)
 */
static inline void ecp_mpi_load( mbedtls_mpi *X, const mbedtls_mpi_uint *p, size_t len )
{
    X->MBEDTLS_PRIVATE(s) = 1;
    X->MBEDTLS_PRIVATE(n) = len / sizeof( mbedtls_mpi_uint );
    X->MBEDTLS_PRIVATE(p) = (mbedtls_mpi_uint *) p;
}

static int mbedtls_ecp_group_load_ex(mbedtls_ecp_group *grp, mbedtls_ecp_group_id id)
{
	int ret;

	ret = mbedtls_ecp_group_load(grp, id);

	if (!ret) {
		switch (id) {
#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP192R1:
			ecp_mpi_load(&grp->A, secp192r1_a, sizeof(secp192r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP192R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP224R1:
			ecp_mpi_load(&grp->A, secp224r1_a, sizeof(secp224r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP224R1_ENABLED */
#endif /* CONFIG_FULL_SUPPLICANT */

#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP256R1:
			ecp_mpi_load(&grp->A, secp256r1_a, sizeof(secp256r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP256R1_ENABLED */

#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP384R1:
			ecp_mpi_load(&grp->A, secp384r1_a, sizeof(secp384r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP384R1_ENABLED */

#ifdef CONFIG_FULL_SUPPLICANT
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
		case MBEDTLS_ECP_DP_SECP521R1:
			ecp_mpi_load(&grp->A, secp521r1_a, sizeof(secp521r1_a));
			break;
#endif /* MBEDTLS_ECP_DP_SECP521R1_ENABLED */
#endif /* CONFIG_FULL_SUPPLICANT */
		default:
			break;
		}
	}

	return ret;
}

extern int mbedtls_entropy_func( void *data, unsigned char *output, size_t len );
__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_mbedtls_ecp_mul_nsc(mbedtls_ecp_group_id id, ecp_binary_t *R, 
								mpi_binary_t *m, ecp_binary_t *P)
{
	int ret = 0;

	mbedtls_ecp_group ecp_g;
	mbedtls_ecp_point des_p;
	mbedtls_ecp_point res_p;
	mbedtls_mpi t;

	if( R == NULL || m == NULL || P == NULL )
	{
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	mbedtls_mpi_init(&t);
	mbedtls_ecp_point_init(&res_p);
	mbedtls_ecp_point_init(&des_p);
	mbedtls_ecp_group_init(&ecp_g);

	MBEDTLS_MPI_CHK(mbedtls_ecp_group_load_ex(&ecp_g, id));

	MBEDTLS_MPI_CHK(mbedtls_mpi_read_binary(&t, (const unsigned char *) m->buf, m->olen));

	MBEDTLS_MPI_CHK(mbedtls_ecp_point_read_binary(&ecp_g, &res_p, 
										(const unsigned char *)P->buf, P->olen));

	ret = mbedtls_ecp_mul( &ecp_g, &des_p, &t, &res_p, mbedtls_entropy_func, NULL );
	if(ret != 0)
		{
		goto cleanup;
	}

	MBEDTLS_MPI_CHK(mbedtls_ecp_point_write_binary(&ecp_g, &des_p , 0, &R->olen,
									R->buf, sizeof(R->buf)));

cleanup:
	mbedtls_mpi_free(&t);
	mbedtls_ecp_point_free(&res_p);
	mbedtls_ecp_point_free(&des_p);
	mbedtls_ecp_group_free(&ecp_g);

	return ret;
}

extern int ecp_mode_cal( mbedtls_ecp_group_id id, mbedtls_mpi *N );
__tfm_psa_secure_gateway_no_naked_attributes__  psa_status_t psa_ecp_mod_nsc( mbedtls_ecp_group_id id, mpi_binary_t *m )
{
	int ret = 0;

	mbedtls_mpi t;

	if( m == NULL )
	{
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	mbedtls_mpi_init( &t );

	MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &t, m->buf, m->olen ));

	ret = ecp_mode_cal( id, &t );

	if(ret != 0)
	{
		goto cleanup;
	}
	MBEDTLS_MPI_CHK(mbedtls_mpi_write_binary(&t, m->buf, m->olen));

cleanup:
	mbedtls_mpi_free(&t);

	return ret;
}

