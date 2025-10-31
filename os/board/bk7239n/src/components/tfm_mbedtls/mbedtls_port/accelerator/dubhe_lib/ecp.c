/*
 *  Elliptic curves over GF(p): generic functions
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * References:
 *
 * SEC1 http://www.secg.org/index.php?action=secg,docs_secg
 * GECC = Guide to Elliptic Curve Cryptography - Hankerson, Menezes, Vanstone
 * FIPS 186-3 http://csrc.nist.gov/publications/fips/fips186-3/fips_186-3.pdf
 * RFC 4492 for the related TLS structures and constants
 * RFC 7748 for the Curve448 and Curve25519 curve definitions
 *
 * [Curve25519] http://cr.yp.to/ecdh/curve25519-20060209.pdf
 *
 * [2] CORON, Jean-S'ebastien. Resistance against differential power analysis
 *     for elliptic curve cryptosystems. In : Cryptographic Hardware and
 *     Embedded Systems. Springer Berlin Heidelberg, 1999. p. 292-302.
 *     <http://link.springer.com/chapter/10.1007/3-540-48059-5_25>
 *
 * [3] HEDABOU, Mustapha, PINEL, Pierre, et B'EN'ETEAU, Lucien. A comb method to
 *     render ECC resistant against Side Channel Attacks. IACR Cryptology
 *     ePrint Archive, 2004, vol. 2004, p. 342.
 *     <http://eprint.iacr.org/2004/342.pdf>
 */

#include "common.h"

/**
 * \brief Function level alternative implementation.
 *
 * The MBEDTLS_ECP_INTERNAL_ALT macro enables alternative implementations to
 * replace certain functions in this module. The alternative implementations are
 * typically hardware accelerators and need to activate the hardware before the
 * computation starts and deactivate it after it finishes. The
 * mbedtls_internal_ecp_init() and mbedtls_internal_ecp_free() functions serve
 * this purpose.
 *
 * To preserve the correct functionality the following conditions must hold:
 *
 * - The alternative implementation must be activated by
 *   mbedtls_internal_ecp_init() before any of the replaceable functions is
 *   called.
 * - mbedtls_internal_ecp_free() must \b only be called when the alternative
 *   implementation is activated.
 * - mbedtls_internal_ecp_init() must \b not be called when the alternative
 *   implementation is activated.
 * - Public functions must not return while the alternative implementation is
 *   activated.
 * - Replaceable functions are guarded by \c MBEDTLS_ECP_XXX_ALT macros and
 *   before calling them an \code if( mbedtls_internal_ecp_grp_capable( grp ) )
 *   \endcode ensures that the alternative implementation supports the current
 *   group.
 */
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
#endif

#if defined(MBEDTLS_ECP_C)

#include "mbedtls/ecp.h"
#include "mbedtls/threading.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"

#include "bn_mul.h"
#include "ecp_invasive.h"

#include <string.h>

#if !defined(MBEDTLS_ECP_ALT)

/* Parameter validation macros based on platform_util.h */
#define ECP_VALIDATE_RET( cond )    \
    MBEDTLS_INTERNAL_VALIDATE_RET( cond, MBEDTLS_ERR_ECP_BAD_INPUT_DATA )
#define ECP_VALIDATE( cond )        \
    MBEDTLS_INTERNAL_VALIDATE( cond )

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdlib.h>
#include <stdio.h>
#define mbedtls_printf     printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#include "ecp_internal_alt.h"

#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && \
    !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif

#if defined(MBEDTLS_SELF_TEST)
/*
 * Counts of point addition and doubling, and field multiplications.
 * Used to test resistance of point multiplication to simple timing attacks.
 */
static unsigned long add_count, dbl_count, mul_count;
#endif

#if defined(MBEDTLS_ECP_RESTARTABLE)
/*
 * Maximum number of "basic operations" to be done in a row.
 *
 * Default value 0 means that ECC operations will not yield.
 * Note that regardless of the value of ecp_max_ops, always at
 * least one step is performed before yielding.
 *
 * Setting ecp_max_ops=1 can be suitable for testing purposes
 * as it will interrupt computation at all possible points.
 */
static unsigned ecp_max_ops = 0;

/*
 * Set ecp_max_ops
 */
void mbedtls_ecp_set_max_ops( unsigned max_ops )
{
    ecp_max_ops = max_ops;
}

/*
 * Check if restart is enabled
 */
int mbedtls_ecp_restart_is_enabled( void )
{
    return( ecp_max_ops != 0 );
}

/*
 * Restart sub-context for ecp_mul_comb()
 */
struct mbedtls_ecp_restart_mul
{
    mbedtls_ecp_point R;    /* current intermediate result                  */
    size_t i;               /* current index in various loops, 0 outside    */
    mbedtls_ecp_point *T;   /* table for precomputed points                 */
    unsigned char T_size;   /* number of points in table T                  */
    enum {                  /* what were we doing last time we returned?    */
        ecp_rsm_init = 0,       /* nothing so far, dummy initial state      */
        ecp_rsm_pre_dbl,        /* precompute 2^n multiples                 */
        ecp_rsm_pre_norm_dbl,   /* normalize precomputed 2^n multiples      */
        ecp_rsm_pre_add,        /* precompute remaining points by adding    */
        ecp_rsm_pre_norm_add,   /* normalize all precomputed points         */
        ecp_rsm_comb_core,      /* ecp_mul_comb_core()                      */
        ecp_rsm_final_norm,     /* do the final normalization               */
    } state;
};

/*
 * Init restart_mul sub-context
 */
static void ecp_restart_rsm_init( mbedtls_ecp_restart_mul_ctx *ctx )
{
    mbedtls_ecp_point_init( &ctx->R );
    ctx->i = 0;
    ctx->T = NULL;
    ctx->T_size = 0;
    ctx->state = ecp_rsm_init;
}

/*
 * Free the components of a restart_mul sub-context
 */
static void ecp_restart_rsm_free( mbedtls_ecp_restart_mul_ctx *ctx )
{
    unsigned char i;

    if( ctx == NULL )
        return;

    mbedtls_ecp_point_free( &ctx->R );

    if( ctx->T != NULL )
    {
        for( i = 0; i < ctx->T_size; i++ )
            mbedtls_ecp_point_free( ctx->T + i );
        mbedtls_free( ctx->T );
    }

    ecp_restart_rsm_init( ctx );
}

/*
 * Restart context for ecp_muladd()
 */
struct mbedtls_ecp_restart_muladd
{
    mbedtls_ecp_point mP;       /* mP value                             */
    mbedtls_ecp_point R;        /* R intermediate result                */
    enum {                      /* what should we do next?              */
        ecp_rsma_mul1 = 0,      /* first multiplication                 */
        ecp_rsma_mul2,          /* second multiplication                */
        ecp_rsma_add,           /* addition                             */
        ecp_rsma_norm,          /* normalization                        */
    } state;
};

/*
 * Init restart_muladd sub-context
 */
static void ecp_restart_ma_init( mbedtls_ecp_restart_muladd_ctx *ctx )
{
    mbedtls_ecp_point_init( &ctx->mP );
    mbedtls_ecp_point_init( &ctx->R );
    ctx->state = ecp_rsma_mul1;
}

/*
 * Free the components of a restart_muladd sub-context
 */
static void ecp_restart_ma_free( mbedtls_ecp_restart_muladd_ctx *ctx )
{
    if( ctx == NULL )
        return;

    mbedtls_ecp_point_free( &ctx->mP );
    mbedtls_ecp_point_free( &ctx->R );

    ecp_restart_ma_init( ctx );
}

/*
 * Initialize a restart context
 */
void mbedtls_ecp_restart_init( mbedtls_ecp_restart_ctx *ctx )
{
    ECP_VALIDATE( ctx != NULL );
    ctx->ops_done = 0;
    ctx->depth = 0;
    ctx->rsm = NULL;
    ctx->ma = NULL;
}

/*
 * Free the components of a restart context
 */
void mbedtls_ecp_restart_free( mbedtls_ecp_restart_ctx *ctx )
{
    if( ctx == NULL )
        return;

    ecp_restart_rsm_free( ctx->rsm );
    mbedtls_free( ctx->rsm );

    ecp_restart_ma_free( ctx->ma );
    mbedtls_free( ctx->ma );

    mbedtls_ecp_restart_init( ctx );
}

/*
 * Check if we can do the next step
 */
int mbedtls_ecp_check_budget( const mbedtls_ecp_group *grp,
                              mbedtls_ecp_restart_ctx *rs_ctx,
                              unsigned ops )
{
    ECP_VALIDATE_RET( grp != NULL );

    if( rs_ctx != NULL && ecp_max_ops != 0 )
    {
        /* scale depending on curve size: the chosen reference is 256-bit,
         * and multiplication is quadratic. Round to the closest integer. */
        if( grp->pbits >= 512 )
            ops *= 4;
        else if( grp->pbits >= 384 )
            ops *= 2;

        /* Avoid infinite loops: always allow first step.
         * Because of that, however, it's not generally true
         * that ops_done <= ecp_max_ops, so the check
         * ops_done > ecp_max_ops below is mandatory. */
        if( ( rs_ctx->ops_done != 0 ) &&
            ( rs_ctx->ops_done > ecp_max_ops ||
              ops > ecp_max_ops - rs_ctx->ops_done ) )
        {
            return( MBEDTLS_ERR_ECP_IN_PROGRESS );
        }

        /* update running count */
        rs_ctx->ops_done += ops;
    }

    return( 0 );
}

/* Call this when entering a function that needs its own sub-context */
#define ECP_RS_ENTER( SUB )   do {                                      \
    /* reset ops count for this call if top-level */                    \
    if( rs_ctx != NULL && rs_ctx->depth++ == 0 )                        \
        rs_ctx->ops_done = 0;                                           \
                                                                        \
    /* set up our own sub-context if needed */                          \
    if( mbedtls_ecp_restart_is_enabled() &&                             \
        rs_ctx != NULL && rs_ctx->SUB == NULL )                         \
    {                                                                   \
        rs_ctx->SUB = mbedtls_calloc( 1, sizeof( *rs_ctx->SUB ) );      \
        if( rs_ctx->SUB == NULL )                                       \
            return( MBEDTLS_ERR_ECP_ALLOC_FAILED );                     \
                                                                        \
        ecp_restart_## SUB ##_init( rs_ctx->SUB );                      \
    }                                                                   \
} while( 0 )

/* Call this when leaving a function that needs its own sub-context */
#define ECP_RS_LEAVE( SUB )   do {                                      \
    /* clear our sub-context when not in progress (done or error) */    \
    if( rs_ctx != NULL && rs_ctx->SUB != NULL &&                        \
        ret != MBEDTLS_ERR_ECP_IN_PROGRESS )                            \
    {                                                                   \
        ecp_restart_## SUB ##_free( rs_ctx->SUB );                      \
        mbedtls_free( rs_ctx->SUB );                                    \
        rs_ctx->SUB = NULL;                                             \
    }                                                                   \
                                                                        \
    if( rs_ctx != NULL )                                                \
        rs_ctx->depth--;                                                \
} while( 0 )

#else /* MBEDTLS_ECP_RESTARTABLE */

#define ECP_RS_ENTER( sub )     (void) rs_ctx;
#define ECP_RS_LEAVE( sub )     (void) rs_ctx;

#endif /* MBEDTLS_ECP_RESTARTABLE */

/*
 * List of supported curves:
 *  - internal ID
 *  - TLS NamedCurve ID (RFC 4492 sec. 5.1.1, RFC 7071 sec. 2, RFC 8446 sec. 4.2.7)
 *  - size in bits
 *  - readable name
 *
 * Curves are listed in order: largest curves first, and for a given size,
 * fastest curves first.
 *
 * Reminder: update profiles in x509_crt.c and ssl_tls.c when adding a new curve!
 */
static const mbedtls_ecp_curve_info ecp_supported_curves[] =
{
    /*SM2 curve id, we refer to GMSSL ssl_curve_tbl in t1_trce.c*/
#if defined(MBEDTLS_ECP_DP_SM2P256V1_ENABLED)
    { MBEDTLS_ECP_DP_SM2P256V1,    30,     256,    "sm2p256v1"         },
#endif
#if defined(MBEDTLS_ECP_DP_SECP521R1_ENABLED)
    { MBEDTLS_ECP_DP_SECP521R1,    25,     521,    "secp521r1"         },
#endif
#if defined(MBEDTLS_ECP_DP_BP512R1_ENABLED)
    { MBEDTLS_ECP_DP_BP512R1,      28,     512,    "brainpoolP512r1"   },
#endif
#if defined(MBEDTLS_ECP_DP_SECP384R1_ENABLED)
    { MBEDTLS_ECP_DP_SECP384R1,    24,     384,    "secp384r1"         },
#endif
#if defined(MBEDTLS_ECP_DP_BP384R1_ENABLED)
    { MBEDTLS_ECP_DP_BP384R1,      27,     384,    "brainpoolP384r1"   },
#endif
#if defined(MBEDTLS_ECP_DP_SECP256R1_ENABLED)
    { MBEDTLS_ECP_DP_SECP256R1,    23,     256,    "secp256r1"         },
#endif
#if defined(MBEDTLS_ECP_DP_SECP256K1_ENABLED)
    { MBEDTLS_ECP_DP_SECP256K1,    22,     256,    "secp256k1"         },
#endif
#if defined(MBEDTLS_ECP_DP_BP256R1_ENABLED)
    { MBEDTLS_ECP_DP_BP256R1,      26,     256,    "brainpoolP256r1"   },
#endif
#if defined(MBEDTLS_ECP_DP_SECP224R1_ENABLED)
    { MBEDTLS_ECP_DP_SECP224R1,    21,     224,    "secp224r1"         },
#endif
#if defined(MBEDTLS_ECP_DP_SECP224K1_ENABLED)
    { MBEDTLS_ECP_DP_SECP224K1,    20,     224,    "secp224k1"         },
#endif
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
    { MBEDTLS_ECP_DP_SECP192R1,    19,     192,    "secp192r1"         },
#endif
#if defined(MBEDTLS_ECP_DP_SECP192K1_ENABLED)
    { MBEDTLS_ECP_DP_SECP192K1,    18,     192,    "secp192k1"         },
#endif
#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
    { MBEDTLS_ECP_DP_CURVE25519,   29,     256,    "x25519"            },
#endif
#if defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
    { MBEDTLS_ECP_DP_CURVE448,     30,     448,    "x448"              },
#endif
    { MBEDTLS_ECP_DP_NONE,          0,     0,      NULL                },
};

#define ECP_NB_CURVES   sizeof( ecp_supported_curves ) /    \
                        sizeof( ecp_supported_curves[0] )

static mbedtls_ecp_group_id ecp_supported_grp_id[ECP_NB_CURVES];

/*
 * List of supported curves and associated info
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_list( void )
{
    return( ecp_supported_curves );
}

/*
 * List of supported curves, group ID only
 */
const mbedtls_ecp_group_id *mbedtls_ecp_grp_id_list( void )
{
    static int init_done = 0;

    if( ! init_done )
    {
        size_t i = 0;
        const mbedtls_ecp_curve_info *curve_info;

        for( curve_info = mbedtls_ecp_curve_list();
             curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
             curve_info++ )
        {
            ecp_supported_grp_id[i++] = curve_info->grp_id;
        }
        ecp_supported_grp_id[i] = MBEDTLS_ECP_DP_NONE;

        init_done = 1;
    }

    return( ecp_supported_grp_id );
}

/*
 * Get the curve info for the internal identifier
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_grp_id( mbedtls_ecp_group_id grp_id )
{
    const mbedtls_ecp_curve_info *curve_info;

    for( curve_info = mbedtls_ecp_curve_list();
         curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
         curve_info++ )
    {
        if( curve_info->grp_id == grp_id )
            return( curve_info );
    }

    return( NULL );
}

/*
 * Get the curve info from the TLS identifier
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_tls_id( uint16_t tls_id )
{
    const mbedtls_ecp_curve_info *curve_info;

    for( curve_info = mbedtls_ecp_curve_list();
         curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
         curve_info++ )
    {
        if( curve_info->tls_id == tls_id )
            return( curve_info );
    }

    return( NULL );
}

/*
 * Get the curve info from the name
 */
const mbedtls_ecp_curve_info *mbedtls_ecp_curve_info_from_name( const char *name )
{
    const mbedtls_ecp_curve_info *curve_info;

    if( name == NULL )
        return( NULL );

    for( curve_info = mbedtls_ecp_curve_list();
         curve_info->grp_id != MBEDTLS_ECP_DP_NONE;
         curve_info++ )
    {
        if( strcmp( curve_info->name, name ) == 0 )
            return( curve_info );
    }

    return( NULL );
}

/*
 * Get the type of a curve
 */
mbedtls_ecp_curve_type mbedtls_ecp_get_type( const mbedtls_ecp_group *grp )
{
    if( grp->G.X.p == NULL )
        return( MBEDTLS_ECP_TYPE_NONE );

    if( grp->G.Y.p == NULL )
        return( MBEDTLS_ECP_TYPE_MONTGOMERY );
    else
        return( MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS );
}

/*
 * Initialize (the components of) a point
 */
void mbedtls_ecp_point_init( mbedtls_ecp_point *pt )
{
    ECP_VALIDATE( pt != NULL );

    mbedtls_mpi_init( &pt->X );
    mbedtls_mpi_init( &pt->Y );
    mbedtls_mpi_init( &pt->Z );
}

/*
 * Initialize (the components of) a group
 */
void mbedtls_ecp_group_init( mbedtls_ecp_group *grp )
{
    ECP_VALIDATE( grp != NULL );

    grp->id = MBEDTLS_ECP_DP_NONE;
    mbedtls_mpi_init( &grp->P );
    mbedtls_mpi_init( &grp->A );
    mbedtls_mpi_init( &grp->B );
    mbedtls_ecp_point_init( &grp->G );
    mbedtls_mpi_init( &grp->N );
    grp->pbits = 0;
    grp->nbits = 0;
    grp->h = 0;
    grp->modp = NULL;
    grp->t_pre = NULL;
    grp->t_post = NULL;
    grp->t_data = NULL;
    grp->T = NULL;
    grp->T_size = 0;
}

/*
 * Initialize (the components of) a key pair
 */
void mbedtls_ecp_keypair_init( mbedtls_ecp_keypair *key )
{
    ECP_VALIDATE( key != NULL );

    mbedtls_ecp_group_init( &key->grp );
    mbedtls_mpi_init( &key->d );
    mbedtls_ecp_point_init( &key->Q );
}

/*
 * Unallocate (the components of) a point
 */
void mbedtls_ecp_point_free( mbedtls_ecp_point *pt )
{
    if( pt == NULL )
        return;

    mbedtls_mpi_free( &( pt->X ) );
    mbedtls_mpi_free( &( pt->Y ) );
    mbedtls_mpi_free( &( pt->Z ) );
}

/*
 * Check that the comb table (grp->T) is static initialized.
 */
static int ecp_group_is_static_comb_table( const mbedtls_ecp_group *grp ) {
#if MBEDTLS_ECP_FIXED_POINT_OPTIM == 1
    return grp->T != NULL && grp->T_size == 0;
#else
    (void) grp;
    return 0;
#endif
}

/*
 * Unallocate (the components of) a group
 */
void mbedtls_ecp_group_free( mbedtls_ecp_group *grp )
{
    size_t i;

    if( grp == NULL )
        return;

    if( grp->h != 1 )
    {
        mbedtls_mpi_free( &grp->P );
        mbedtls_mpi_free( &grp->A );
        mbedtls_mpi_free( &grp->B );
        mbedtls_ecp_point_free( &grp->G );
        mbedtls_mpi_free( &grp->N );
    }
    else
    {
        mbedtls_mpi_free_handle( &grp->P );
        mbedtls_mpi_free_handle( &grp->A );
        mbedtls_mpi_free_handle( &grp->B );
        mbedtls_mpi_free_handle( &grp->G.X );
        mbedtls_mpi_free_handle( &grp->G.Y );
        mbedtls_mpi_free_handle( &grp->G.Z );
        mbedtls_mpi_free_handle( &grp->N );
    }

    if( !ecp_group_is_static_comb_table(grp) && grp->T != NULL )
    {
        for( i = 0; i < grp->T_size; i++ )
            mbedtls_ecp_point_free( &grp->T[i] );
        mbedtls_free( grp->T );
    }

    mbedtls_platform_zeroize( grp, sizeof( mbedtls_ecp_group ) );
}

/*
 * Unallocate (the components of) a key pair
 */
void mbedtls_ecp_keypair_free( mbedtls_ecp_keypair *key )
{
    if( key == NULL )
        return;

    mbedtls_ecp_group_free( &key->grp );
    mbedtls_mpi_free( &key->d );
    mbedtls_ecp_point_free( &key->Q );
}

/*
 * Copy the contents of a point
 */
int mbedtls_ecp_copy( mbedtls_ecp_point *P, const mbedtls_ecp_point *Q )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECP_VALIDATE_RET( P != NULL );
    ECP_VALIDATE_RET( Q != NULL );

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &P->X, &Q->X ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &P->Y, &Q->Y ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &P->Z, &Q->Z ) );

cleanup:
    return( ret );
}

/*
 * Copy the contents of a group object
 */
int mbedtls_ecp_group_copy( mbedtls_ecp_group *dst, const mbedtls_ecp_group *src )
{
    ECP_VALIDATE_RET( dst != NULL );
    ECP_VALIDATE_RET( src != NULL );

    return( mbedtls_ecp_group_load( dst, src->id ) );
}

/*
 * Set point to zero
 */
int mbedtls_ecp_set_zero( mbedtls_ecp_point *pt )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECP_VALIDATE_RET( pt != NULL );

    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &pt->X , 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &pt->Y , 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &pt->Z , 0 ) );

cleanup:
    return( ret );
}

/*
 * Tell if a point is zero
 */
int mbedtls_ecp_is_zero( mbedtls_ecp_point *pt )
{
    ECP_VALIDATE_RET( pt != NULL );

    return( mbedtls_mpi_cmp_int( &pt->Z, 0 ) == 0 );
}

/*
 * Compare two points lazily
 */
int mbedtls_ecp_point_cmp( const mbedtls_ecp_point *P,
                           const mbedtls_ecp_point *Q )
{
    ECP_VALIDATE_RET( P != NULL );
    ECP_VALIDATE_RET( Q != NULL );

    if( mbedtls_mpi_cmp_mpi( &P->X, &Q->X ) == 0 &&
        mbedtls_mpi_cmp_mpi( &P->Y, &Q->Y ) == 0 &&
        mbedtls_mpi_cmp_mpi( &P->Z, &Q->Z ) == 0 )
    {
        return( 0 );
    }

    return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
}

/*
 * Import a non-zero point from ASCII strings
 */
int mbedtls_ecp_point_read_string( mbedtls_ecp_point *P, int radix,
                           const char *x, const char *y )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECP_VALIDATE_RET( P != NULL );
    ECP_VALIDATE_RET( x != NULL );
    ECP_VALIDATE_RET( y != NULL );

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &P->X, radix, x ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( &P->Y, radix, y ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &P->Z, 1 ) );

cleanup:
    return( ret );
}

/*
 * Export a point into unsigned binary data (SEC1 2.3.3 and RFC7748)
 */
int mbedtls_ecp_point_write_binary( const mbedtls_ecp_group *grp,
                                    const mbedtls_ecp_point *P,
                                    int format, size_t *olen,
                                    unsigned char *buf, size_t buflen )
{
    int ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    size_t plen;
    ECP_VALIDATE_RET( grp  != NULL );
    ECP_VALIDATE_RET( P    != NULL );
    ECP_VALIDATE_RET( olen != NULL );
    ECP_VALIDATE_RET( buf  != NULL );
    ECP_VALIDATE_RET( format == MBEDTLS_ECP_PF_UNCOMPRESSED ||
                      format == MBEDTLS_ECP_PF_COMPRESSED );

    plen = mbedtls_mpi_size( &grp->P );

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    (void) format; /* Montgomery curves always use the same point format */
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
    {
        *olen = plen;
        if( buflen < *olen )
            return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary_le( &P->X, buf, plen ) );
    }
#endif
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
    {
        /*
         * Common case: P == 0
         */
        if( mbedtls_mpi_cmp_int( &P->Z, 0 ) == 0 )
        {
            if( buflen < 1 )
                return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

            buf[0] = 0x00;
            *olen = 1;

            return( 0 );
        }

        if( format == MBEDTLS_ECP_PF_UNCOMPRESSED )
        {
            *olen = 2 * plen + 1;

            if( buflen < *olen )
                return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

            buf[0] = 0x04;
            MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &P->X, buf + 1, plen ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &P->Y, buf + 1 + plen, plen ) );
        }
        else if( format == MBEDTLS_ECP_PF_COMPRESSED )
        {
            *olen = plen + 1;

            if( buflen < *olen )
                return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

            buf[0] = 0x02 + mbedtls_mpi_get_bit( &P->Y, 0 );
            MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &P->X, buf + 1, plen ) );
        }
    }
#endif

cleanup:
    return( ret );
}

/*
 * Import a point from unsigned binary data (SEC1 2.3.4 and RFC7748)
 */
int mbedtls_ecp_point_read_binary( const mbedtls_ecp_group *grp,
                                   mbedtls_ecp_point *pt,
                                   const unsigned char *buf, size_t ilen )
{
    int ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;
    size_t plen;
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( pt  != NULL );
    ECP_VALIDATE_RET( buf != NULL );

    if( ilen < 1 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    plen = mbedtls_mpi_size( &grp->P );

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
    {
        if( plen != ilen )
            return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

        MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary_le( &pt->X, buf, plen ) );
        mbedtls_mpi_free( &pt->Y );

        if( grp->id == MBEDTLS_ECP_DP_CURVE25519 )
            /* Set most significant bit to 0 as prescribed in RFC7748 §5 */
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( &pt->X, plen * 8 - 1, 0 ) );

        MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &pt->Z, 1 ) );
    }
#endif
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
    {
        if( buf[0] == 0x00 )
        {
            if( ilen == 1 )
                return( mbedtls_ecp_set_zero( pt ) );
            else
                return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
        }

        if( buf[0] != 0x04 )
            return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );

        if( ilen != 2 * plen + 1 )
            return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

        MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &pt->X, buf + 1, plen ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &pt->Y,
                                                  buf + 1 + plen, plen ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &pt->Z, 1 ) );
    }
#endif

cleanup:
    return( ret );
}

/*
 * Import a point from a TLS ECPoint record (RFC 4492)
 *      struct {
 *          opaque point <1..2^8-1>;
 *      } ECPoint;
 */
int mbedtls_ecp_tls_read_point( const mbedtls_ecp_group *grp,
                                mbedtls_ecp_point *pt,
                                const unsigned char **buf, size_t buf_len )
{
    unsigned char data_len;
    const unsigned char *buf_start;
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( pt  != NULL );
    ECP_VALIDATE_RET( buf != NULL );
    ECP_VALIDATE_RET( *buf != NULL );

    /*
     * We must have at least two bytes (1 for length, at least one for data)
     */
    if( buf_len < 2 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    data_len = *(*buf)++;
    if( data_len < 1 || data_len > buf_len - 1 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /*
     * Save buffer start for read_binary and update buf
     */
    buf_start = *buf;
    *buf += data_len;

    return( mbedtls_ecp_point_read_binary( grp, pt, buf_start, data_len ) );
}

/*
 * Export a point as a TLS ECPoint record (RFC 4492)
 *      struct {
 *          opaque point <1..2^8-1>;
 *      } ECPoint;
 */
int mbedtls_ecp_tls_write_point( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt,
                         int format, size_t *olen,
                         unsigned char *buf, size_t blen )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECP_VALIDATE_RET( grp  != NULL );
    ECP_VALIDATE_RET( pt   != NULL );
    ECP_VALIDATE_RET( olen != NULL );
    ECP_VALIDATE_RET( buf  != NULL );
    ECP_VALIDATE_RET( format == MBEDTLS_ECP_PF_UNCOMPRESSED ||
                      format == MBEDTLS_ECP_PF_COMPRESSED );

    /*
     * buffer length must be at least one, for our length byte
     */
    if( blen < 1 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    if( ( ret = mbedtls_ecp_point_write_binary( grp, pt, format,
                    olen, buf + 1, blen - 1) ) != 0 )
        return( ret );

    /*
     * write length to the first byte and update total length
     */
    buf[0] = (unsigned char) *olen;
    ++*olen;

    return( 0 );
}

/*
 * Set a group from an ECParameters record (RFC 4492)
 */
int mbedtls_ecp_tls_read_group( mbedtls_ecp_group *grp,
                                const unsigned char **buf, size_t len )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_ecp_group_id grp_id;
    ECP_VALIDATE_RET( grp  != NULL );
    ECP_VALIDATE_RET( buf  != NULL );
    ECP_VALIDATE_RET( *buf != NULL );

    if( ( ret = mbedtls_ecp_tls_read_group_id( &grp_id, buf, len ) ) != 0 )
        return( ret );

    return( mbedtls_ecp_group_load( grp, grp_id ) );
}

/*
 * Read a group id from an ECParameters record (RFC 4492) and convert it to
 * mbedtls_ecp_group_id.
 */
int mbedtls_ecp_tls_read_group_id( mbedtls_ecp_group_id *grp,
                                   const unsigned char **buf, size_t len )
{
    uint16_t tls_id;
    const mbedtls_ecp_curve_info *curve_info;
    ECP_VALIDATE_RET( grp  != NULL );
    ECP_VALIDATE_RET( buf  != NULL );
    ECP_VALIDATE_RET( *buf != NULL );

    /*
     * We expect at least three bytes (see below)
     */
    if( len < 3 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /*
     * First byte is curve_type; only named_curve is handled
     */
    if( *(*buf)++ != MBEDTLS_ECP_TLS_NAMED_CURVE )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /*
     * Next two bytes are the namedcurve value
     */
    tls_id = *(*buf)++;
    tls_id <<= 8;
    tls_id |= *(*buf)++;

    if( ( curve_info = mbedtls_ecp_curve_info_from_tls_id( tls_id ) ) == NULL )
        return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );

    *grp = curve_info->grp_id;

    return( 0 );
}

/*
 * Write the ECParameters record corresponding to a group (RFC 4492)
 */
int mbedtls_ecp_tls_write_group( const mbedtls_ecp_group *grp, size_t *olen,
                         unsigned char *buf, size_t blen )
{
    const mbedtls_ecp_curve_info *curve_info;
    ECP_VALIDATE_RET( grp  != NULL );
    ECP_VALIDATE_RET( buf  != NULL );
    ECP_VALIDATE_RET( olen != NULL );

    if( ( curve_info = mbedtls_ecp_curve_info_from_grp_id( grp->id ) ) == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /*
     * We are going to write 3 bytes (see below)
     */
    *olen = 3;
    if( blen < *olen )
        return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

    /*
     * First byte is curve_type, always named_curve
     */
    *buf++ = MBEDTLS_ECP_TLS_NAMED_CURVE;

    /*
     * Next two bytes are the namedcurve value
     */
    MBEDTLS_PUT_UINT16_BE( curve_info->tls_id, buf, 0 );

    return( 0 );
}

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
/*
 * Wrapper around fast quasi-modp functions, with fall-back to mbedtls_mpi_mod_mpi.
 * See the documentation of struct mbedtls_ecp_group.
 *
 * This function is in the critial loop for mbedtls_ecp_mul, so pay attention to perf.
 */
static int ecp_modp( mbedtls_mpi *N, const mbedtls_ecp_group *grp )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if( grp->modp == NULL )
        return( mbedtls_mpi_mod_mpi( N, N, &grp->P ) );

    /* N->s < 0 is a much faster test, which fails only if N is 0 */
    if( ( N->s < 0 && mbedtls_mpi_cmp_int( N, 0 ) != 0 ) ||
        mbedtls_mpi_bitlen( N ) > 2 * grp->pbits )
    {
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    }

    MBEDTLS_MPI_CHK( grp->modp( N ) );

    /* N->s < 0 is a much faster test, which fails only if N is 0 */
    while( N->s < 0 && mbedtls_mpi_cmp_int( N, 0 ) != 0 )
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( N, N, &grp->P ) );

    while( mbedtls_mpi_cmp_mpi( N, &grp->P ) >= 0 )
        /* we known P, N and the result are positive */
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_abs( N, N, &grp->P ) );

cleanup:
    return( ret );
}

/*
 * Fast mod-p functions expect their argument to be in the 0..p^2 range.
 *
 * In order to guarantee that, we need to ensure that operands of
 * mbedtls_mpi_mul_mpi are in the 0..p range. So, after each operation we will
 * bring the result back to this range.
 *
 * The following macros are shortcuts for doing that.
 */

/*
 * Reduce a mbedtls_mpi mod p in-place, general case, to use after mbedtls_mpi_mul_mpi
 */
#if defined(MBEDTLS_SELF_TEST)
#define INC_MUL_COUNT   mul_count++;
#else
#define INC_MUL_COUNT
#endif

#define MOD_MUL( N )                                                    \
    do                                                                  \
    {                                                                   \
        MBEDTLS_MPI_CHK( ecp_modp( &(N), grp ) );                       \
        INC_MUL_COUNT                                                   \
    } while( 0 )

/*static inline int mbedtls_mpi_mul_mod( const mbedtls_ecp_group *grp,
                                       mbedtls_mpi *X,
                                       const mbedtls_mpi *A,
                                       const mbedtls_mpi *B )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( X, A, B ) );
    MOD_MUL( *X );
cleanup:
    return( ret );
}*/
#endif

/*
 * Reduce a mbedtls_mpi mod p in-place, to use after mbedtls_mpi_sub_mpi
 * N->s < 0 is a very fast test, which fails only if N is 0
 */
#define MOD_SUB( N )                                                    \
    while( (N).s < 0 && mbedtls_mpi_cmp_int( &(N), 0 ) != 0 )           \
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &(N), &(N), &grp->P ) )

#if ( defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED) && \
      !( defined(MBEDTLS_ECP_NO_FALLBACK) && \
         defined(MBEDTLS_ECP_DOUBLE_JAC_ALT) && \
         defined(MBEDTLS_ECP_ADD_MIXED_ALT) ) ) || \
    ( defined(MBEDTLS_ECP_MONTGOMERY_ENABLED) && \
      !( defined(MBEDTLS_ECP_NO_FALLBACK) && \
         defined(MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT) ) )
static inline int mbedtls_mpi_sub_mod( const mbedtls_ecp_group *grp,
                                       mbedtls_mpi *X,
                                       const mbedtls_mpi *A,
                                       const mbedtls_mpi *B )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( X, A, B ) );
    MOD_SUB( *X );
cleanup:
    return( ret );
}
#endif /* All functions referencing mbedtls_mpi_sub_mod() are alt-implemented without fallback */

/*
 * Reduce a mbedtls_mpi mod p in-place, to use after mbedtls_mpi_add_mpi and mbedtls_mpi_mul_int.
 * We known P, N and the result are positive, so sub_abs is correct, and
 * a bit faster.
 */
#define MOD_ADD( N )                                                    \
    while( mbedtls_mpi_cmp_mpi( &(N), &grp->P ) >= 0 )                  \
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_abs( &(N), &(N), &grp->P ) )

static inline int mbedtls_mpi_add_mod( const mbedtls_ecp_group *grp,
                                       mbedtls_mpi *X,
                                       const mbedtls_mpi *A,
                                       const mbedtls_mpi *B )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( X, A, B ) );
    MOD_ADD( *X );
cleanup:
    return( ret );
}

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED) && \
    !( defined(MBEDTLS_ECP_NO_FALLBACK) && \
       defined(MBEDTLS_ECP_DOUBLE_JAC_ALT) && \
       defined(MBEDTLS_ECP_ADD_MIXED_ALT) )
static inline int mbedtls_mpi_shift_l_mod( const mbedtls_ecp_group *grp,
                                           mbedtls_mpi *X,
                                           size_t count )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( X, count ) );
    MOD_ADD( *X );
cleanup:
    return( ret );
}
#endif /* All functions referencing mbedtls_mpi_shift_l_mod() are alt-implemented without fallback */

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
/*
 * For Montgomery curves, we do all the internal arithmetic in projective
 * coordinates. Import/export of points uses only the x coordinates, which is
 * internaly represented as X / Z.
 *
 * For scalar multiplication, we'll use a Montgomery ladder.
 */

/*
 * Normalize Montgomery x/z coordinates: X = X/Z, Z = 1
 * Cost: 1M + 1I
 */
static int ecp_normalize_mxz( const mbedtls_ecp_group *grp, mbedtls_ecp_point *P )
{
#if defined(MBEDTLS_ECP_NORMALIZE_MXZ_ALT)
    if( mbedtls_internal_ecp_grp_capable( grp ) )
        return( mbedtls_internal_ecp_normalize_mxz( grp, P ) );
#endif /* MBEDTLS_ECP_NORMALIZE_MXZ_ALT */

#if defined(MBEDTLS_ECP_NO_FALLBACK) && defined(MBEDTLS_ECP_NORMALIZE_MXZ_ALT)
    return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );
#else
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    MBEDTLS_MPI_CHK( mbedtls_mpi_inv_mod( &P->Z, &P->Z, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &P->X, &P->X, &P->Z, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &P->Z, 1 ) );

cleanup:
    return( ret );
#endif /* !defined(MBEDTLS_ECP_NO_FALLBACK) || !defined(MBEDTLS_ECP_NORMALIZE_MXZ_ALT) */
}

/*
 * Randomize projective x/z coordinates:
 * (X, Z) -> (l X, l Z) for random l
 * This is sort of the reverse operation of ecp_normalize_mxz().
 *
 * This countermeasure was first suggested in [2].
 * Cost: 2M
 */
static int ecp_randomize_mxz( const mbedtls_ecp_group *grp, mbedtls_ecp_point *P,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
#if defined(MBEDTLS_ECP_RANDOMIZE_MXZ_ALT)
    if( mbedtls_internal_ecp_grp_capable( grp ) )
        return( mbedtls_internal_ecp_randomize_mxz( grp, P, f_rng, p_rng ) );
#endif /* MBEDTLS_ECP_RANDOMIZE_MXZ_ALT */

#if defined(MBEDTLS_ECP_NO_FALLBACK) && defined(MBEDTLS_ECP_RANDOMIZE_MXZ_ALT)
    return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );
#else
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi l;
    mbedtls_mpi_init( &l );

    /* Generate l such that 1 < l < p */
    MBEDTLS_MPI_CHK( mbedtls_mpi_random( &l, 2, &grp->P, f_rng, p_rng ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &P->X, &P->X, &l, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &P->Z, &P->Z, &l, &grp->P ) );

cleanup:
    mbedtls_mpi_free( &l );

    if( ret == MBEDTLS_ERR_MPI_NOT_ACCEPTABLE )
        ret = MBEDTLS_ERR_ECP_RANDOM_FAILED;
    return( ret );
#endif /* !defined(MBEDTLS_ECP_NO_FALLBACK) || !defined(MBEDTLS_ECP_RANDOMIZE_MXZ_ALT) */
}

/*
 * Double-and-add: R = 2P, S = P + Q, with d = X(P - Q),
 * for Montgomery curves in x/z coordinates.
 *
 * http://www.hyperelliptic.org/EFD/g1p/auto-code/montgom/xz/ladder/mladd-1987-m.op3
 * with
 * d =  X1
 * P = (X2, Z2)
 * Q = (X3, Z3)
 * R = (X4, Z4)
 * S = (X5, Z5)
 * and eliminating temporary variables tO, ..., t4.
 *
 * Cost: 5M + 4S
 */
static int ecp_double_add_mxz( const mbedtls_ecp_group *grp,
                               mbedtls_ecp_point *R, mbedtls_ecp_point *S,
                               const mbedtls_ecp_point *P, const mbedtls_ecp_point *Q,
                               const mbedtls_mpi *d )
{
#if defined(MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT)
    if( mbedtls_internal_ecp_grp_capable( grp ) )
        return( mbedtls_internal_ecp_double_add_mxz( grp, R, S, P, Q, d ) );
#endif /* MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT */

#if defined(MBEDTLS_ECP_NO_FALLBACK) && defined(MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT)
    return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );
#else
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi A, AA, B, BB, E, C, D, DA, CB;

    mbedtls_mpi_init( &A ); mbedtls_mpi_init( &AA ); mbedtls_mpi_init( &B );
    mbedtls_mpi_init( &BB ); mbedtls_mpi_init( &E ); mbedtls_mpi_init( &C );
    mbedtls_mpi_init( &D ); mbedtls_mpi_init( &DA ); mbedtls_mpi_init( &CB );

    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mod( grp, &A,    &P->X,   &P->Z ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &AA, &A,    &A,      &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mod( grp, &B,    &P->X,   &P->Z ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &BB, &B,    &B,      &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mod( grp, &E,    &AA,     &BB   ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mod( grp, &C,    &Q->X,   &Q->Z ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mod( grp, &D,    &Q->X,   &Q->Z ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &DA, &D,    &A,      &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &CB, &C,    &B,      &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mod( grp, &S->X, &DA,     &CB   ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &S->X, &S->X,   &S->X, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mod( grp, &S->Z, &DA,     &CB   ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &S->Z, &S->Z,   &S->Z, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &S->Z, d,       &S->Z, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &R->X, &AA,     &BB, &grp->P   ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &R->Z, &grp->A, &E, &grp->P    ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mod( grp, &R->Z, &BB,     &R->Z ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &R->Z, &E,      &R->Z, &grp->P ) ); 

cleanup:
    mbedtls_mpi_free( &A ); mbedtls_mpi_free( &AA ); mbedtls_mpi_free( &B );
    mbedtls_mpi_free( &BB ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &C );
    mbedtls_mpi_free( &D ); mbedtls_mpi_free( &DA ); mbedtls_mpi_free( &CB );

    return( ret );
#endif /* !defined(MBEDTLS_ECP_NO_FALLBACK) || !defined(MBEDTLS_ECP_DOUBLE_ADD_MXZ_ALT) */
}

/*
 * Multiplication with Montgomery ladder in x/z coordinates,
 * for curves in Montgomery form
 */
static int ecp_mul_mxz( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                        const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t i;
    unsigned char b;
    mbedtls_ecp_point RP;
    mbedtls_mpi PX;
    mbedtls_ecp_point_init( &RP ); mbedtls_mpi_init( &PX );

    if( f_rng == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    /* Save PX and read from P before writing to R, in case P == R */
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &PX, &P->X ) );
    MBEDTLS_MPI_CHK( mbedtls_ecp_copy( &RP, P ) );

    /* Set R to zero in modified x/z coordinates */
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &R->X, 1 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &R->Z, 0 ) );
    mbedtls_mpi_free( &R->Y );

    /* RP.X might be sligtly larger than P, so reduce it */
    MOD_ADD( RP.X );

    /* Randomize coordinates of the starting point */
    MBEDTLS_MPI_CHK( ecp_randomize_mxz( grp, &RP, f_rng, p_rng ) );

    /* Loop invariant: R = result so far, RP = R + P */
    i = mbedtls_mpi_bitlen( m ); /* one past the (zero-based) most significant bit */
    while( i-- > 0 )
    {
        b = mbedtls_mpi_get_bit( m, i );
        /*
         *  if (b) R = 2R + P else R = 2R,
         * which is:
         *  if (b) double_add( RP, R, RP, R )
         *  else   double_add( R, RP, R, RP )
         * but using safe conditional swaps to avoid leaks
         */
        MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_swap( &R->X, &RP.X, b ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_swap( &R->Z, &RP.Z, b ) );
        MBEDTLS_MPI_CHK( ecp_double_add_mxz( grp, R, &RP, R, &RP, &PX ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_swap( &R->X, &RP.X, b ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_swap( &R->Z, &RP.Z, b ) );
    }

    /*
     * Knowledge of the projective coordinates may leak the last few bits of the
     * scalar [1], and since our MPI implementation isn't constant-flow,
     * inversion (used for coordinate normalization) may leak the full value
     * of its input via side-channels [2].
     *
     * [1] https://eprint.iacr.org/2003/191
     * [2] https://eprint.iacr.org/2020/055
     *
     * Avoid the leak by randomizing coordinates before we normalize them.
     */
    MBEDTLS_MPI_CHK( ecp_randomize_mxz( grp, R, f_rng, p_rng ) );
    MBEDTLS_MPI_CHK( ecp_normalize_mxz( grp, R ) );

cleanup:
    mbedtls_ecp_point_free( &RP ); mbedtls_mpi_free( &PX );

    return( ret );
}

#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

#if defined(MBEDTLS_ENBALE_SCALAR_MUL_COUNTERMEASURE)

static int ecp_randomize_mul( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                         const mbedtls_mpi *d, const mbedtls_ecp_point *Q,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
    int ret;
    mbedtls_mpi l, ll;
    mbedtls_ecp_point P;
    size_t p_size;
    int count = 0;

    mbedtls_ecp_point_init( &P );
    mbedtls_mpi_init( &l ); mbedtls_mpi_init( &ll );

    p_size = ( grp->pbits + 7 ) / 8;

    /* Generate l such that 1 < l < p */
    do
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( &l, p_size, f_rng, p_rng ) );

        while( mbedtls_mpi_cmp_mpi( &l, &grp->P ) >= 0 )
            MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( &l, 1 ) );

        if( count++ > 10 )
            return( MBEDTLS_ERR_ECP_RANDOM_FAILED );
    }
    while( mbedtls_mpi_cmp_int( &l, 1 ) <= 0 );

    MBEDTLS_MPI_CHK( mbedtls_ecp_copy( &P, Q ) );
    /* Z = l * Z */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &P.Z,   &P.Z,     &l, &grp->P ) );
    /* X = l^2 * X */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &ll,      &l,         &l, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &P.X,   &P.X,     &ll, &grp->P) );
    /* Y = l^3 * Y */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &ll,      &ll,        &l, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &P.Y,   &P.Y,     &ll, &grp->P ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_ecp_no_naf( &R->X, &R->Y, &R->Z, d, &P.X,
                                                 &P.Y, &P.Z, &grp->A, &grp->P ) );
cleanup:
    mbedtls_mpi_free( &l ); mbedtls_mpi_free( &ll );
    mbedtls_ecp_point_free( &P );
    return( ret );
}
#endif

/*
 * Restartable multiplication R = m * P
 *
 * This internal function can be called without an RNG in case where we know
 * the inputs are not sensitive.
 */
static int ecp_mul_restartable_internal( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
             mbedtls_ecp_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
    char is_grp_capable = 0;
#endif

#if defined(MBEDTLS_ECP_RESTARTABLE)
    /* reset ops count for this call if top-level */
    if( rs_ctx != NULL && rs_ctx->depth++ == 0 )
        rs_ctx->ops_done = 0;
#else
    (void) rs_ctx;
#endif

#if defined(MBEDTLS_ECP_INTERNAL_ALT)
    if( ( is_grp_capable = mbedtls_internal_ecp_grp_capable( grp ) ) )
        MBEDTLS_MPI_CHK( mbedtls_internal_ecp_init( grp ) );
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
    /* skip argument check when restarting */
    if( rs_ctx == NULL || rs_ctx->rsm == NULL )
#endif
    {
        /* check_privkey is free */
        MBEDTLS_ECP_BUDGET( MBEDTLS_ECP_OPS_CHK );

        /* Common sanity checks */
        MBEDTLS_MPI_CHK( mbedtls_ecp_check_privkey( grp, m ) );
        MBEDTLS_MPI_CHK( mbedtls_ecp_check_pubkey( grp, P ) );
    }

    ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
        MBEDTLS_MPI_CHK( ecp_mul_mxz( grp, R, m, P, f_rng, p_rng ) );
#endif
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
    {
#if defined(MBEDTLS_ENBALE_SCALAR_MUL_COUNTERMEASURE)
        if(f_rng != NULL)
        {
            MBEDTLS_MPI_CHK( ecp_randomize_mul( grp, R, m, P, f_rng, p_rng ) );
        }
        else
#endif
        {
            MBEDTLS_MPI_CHK( mbedtls_mpi_mul_ecp( &R->X, &R->Y, &R->Z, m, &P->X,
                                                  &P->Y, &P->Z, &grp->A, &grp->P ) );
        }

        MBEDTLS_MPI_CHK( mbedtls_mpi_jac2affine( &R->X, &R->Y, &R->Z, &grp->P ) );
    }
#endif

cleanup:

#if defined(MBEDTLS_ECP_INTERNAL_ALT)
    if( is_grp_capable )
        mbedtls_internal_ecp_free( grp );
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL )
        rs_ctx->depth--;
#endif

    return( ret );
}

/*
 * Restartable multiplication R = m * P
 */
int mbedtls_ecp_mul_restartable( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng,
             mbedtls_ecp_restart_ctx *rs_ctx )
{
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( R   != NULL );
    ECP_VALIDATE_RET( m   != NULL );
    ECP_VALIDATE_RET( P   != NULL );

    if( f_rng == NULL )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    return( ecp_mul_restartable_internal( grp, R, m, P, f_rng, p_rng, rs_ctx ) );
}

/*
 * Multiplication R = m * P
 */
int mbedtls_ecp_mul( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( R   != NULL );
    ECP_VALIDATE_RET( m   != NULL );
    ECP_VALIDATE_RET( P   != NULL );
    return( mbedtls_ecp_mul_restartable( grp, R, m, P, f_rng, p_rng, NULL ) );
}

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
/*
 * Check that an affine point is valid as a public key,
 * short weierstrass curves (SEC1 3.2.3.1)
 */
static int ecp_check_pubkey_sw( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_mpi YY, RHS;

    /* pt coordinates must be normalized for our checks */
    if( mbedtls_mpi_cmp_int( &pt->X, 0 ) < 0 ||
        mbedtls_mpi_cmp_int( &pt->Y, 0 ) < 0 ||
        mbedtls_mpi_cmp_mpi( &pt->X, &grp->P ) >= 0 ||
        mbedtls_mpi_cmp_mpi( &pt->Y, &grp->P ) >= 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    mbedtls_mpi_init( &YY ); mbedtls_mpi_init( &RHS );

    /*
     * YY = Y^2
     * RHS = X (X^2 + A) + B = X^3 + A X + B
     */
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &YY,  &pt->Y,   &pt->Y, &grp->P  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &RHS, &pt->X,   &pt->X, &grp->P  ) );

    /* Special case for A = -3 */
    if( grp->A.p == NULL )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &RHS, &RHS, 3       ) );  MOD_SUB( RHS );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mod( grp, &RHS, &RHS, &grp->A ) );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mod( &RHS, &RHS,     &pt->X, &grp->P  ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mod( grp, &RHS, &RHS,     &grp->B ) );

    if( mbedtls_mpi_cmp_mpi( &YY, &RHS ) != 0 )
        ret = MBEDTLS_ERR_ECP_INVALID_KEY;

cleanup:

    mbedtls_mpi_free( &YY ); mbedtls_mpi_free( &RHS );

    return( ret );
}
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
/*
 * R = m * P with shortcuts for m == 0, m == 1 and m == -1
 * NOT constant-time - ONLY for short Weierstrass!
 */
static int mbedtls_ecp_mul_shortcuts( mbedtls_ecp_group *grp,
                                      mbedtls_ecp_point *R,
                                      const mbedtls_mpi *m,
                                      const mbedtls_ecp_point *P,
                                      mbedtls_ecp_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;

    if( mbedtls_mpi_cmp_int( m, 0 ) == 0 )
    {
        MBEDTLS_MPI_CHK( mbedtls_ecp_set_zero( R ) );
    }
    else if( mbedtls_mpi_cmp_int( m, 1 ) == 0 )
    {
        MBEDTLS_MPI_CHK( mbedtls_ecp_copy( R, P ) );
    }
    else if( mbedtls_mpi_cmp_int( m, -1 ) == 0 )
    {
        MBEDTLS_MPI_CHK( mbedtls_ecp_copy( R, P ) );
        if( mbedtls_mpi_cmp_int( &R->Y, 0 ) != 0 )
            MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &R->Y, &grp->P, &R->Y ) );
    }
    else
    {
        MBEDTLS_MPI_CHK( ecp_mul_restartable_internal( grp, R, m, P,
                                                       NULL, NULL, rs_ctx ) );
    }

cleanup:
    return( ret );
}

/*
 * Restartable linear combination
 * NOT constant-time
 */
int mbedtls_ecp_muladd_restartable(
             mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             const mbedtls_mpi *n, const mbedtls_ecp_point *Q,
             mbedtls_ecp_restart_ctx *rs_ctx )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_ecp_point mP;
    mbedtls_ecp_point *pmP = &mP;
    mbedtls_ecp_point *pR = R;
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
    char is_grp_capable = 0;
#endif
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( R   != NULL );
    ECP_VALIDATE_RET( m   != NULL );
    ECP_VALIDATE_RET( P   != NULL );
    ECP_VALIDATE_RET( n   != NULL );
    ECP_VALIDATE_RET( Q   != NULL );

    if( mbedtls_ecp_get_type( grp ) != MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
        return( MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE );

    mbedtls_ecp_point_init( &mP );

    ECP_RS_ENTER( ma );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ma != NULL )
    {
        /* redirect intermediate results to restart context */
        pmP = &rs_ctx->ma->mP;
        pR  = &rs_ctx->ma->R;

        /* jump to next operation */
        if( rs_ctx->ma->state == ecp_rsma_mul2 )
            goto mul2;
        if( rs_ctx->ma->state == ecp_rsma_add )
            goto add;
        if( rs_ctx->ma->state == ecp_rsma_norm )
            goto norm;
    }
#endif /* MBEDTLS_ECP_RESTARTABLE */

    MBEDTLS_MPI_CHK( mbedtls_ecp_mul_shortcuts( grp, pmP, m, P, rs_ctx ) );
#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ma != NULL )
        rs_ctx->ma->state = ecp_rsma_mul2;

mul2:
#endif
    MBEDTLS_MPI_CHK( mbedtls_ecp_mul_shortcuts( grp, pR,  n, Q, rs_ctx ) );

#if defined(MBEDTLS_ECP_INTERNAL_ALT)
    if( ( is_grp_capable = mbedtls_internal_ecp_grp_capable( grp ) ) )
        MBEDTLS_MPI_CHK( mbedtls_internal_ecp_init( grp ) );
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ma != NULL )
        rs_ctx->ma->state = ecp_rsma_add;

add:
#endif
    MBEDTLS_ECP_BUDGET( MBEDTLS_ECP_OPS_ADD );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_ecp ( &R->X, &R->Y, &R->Z, &R->X,
                                           &R->Y, &R->Z, &mP.X, &mP.Y,
                                           &mP.Z, &grp->P ) );
#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ma != NULL )
        rs_ctx->ma->state = ecp_rsma_norm;

norm:
#endif
    MBEDTLS_ECP_BUDGET( MBEDTLS_ECP_OPS_INV );
    MBEDTLS_MPI_CHK( mbedtls_mpi_jac2affine( &R->X, &R->Y, &R->Z, &grp->P ) );

#if defined(MBEDTLS_ECP_RESTARTABLE)
    if( rs_ctx != NULL && rs_ctx->ma != NULL )
        MBEDTLS_MPI_CHK( mbedtls_ecp_copy( R, pR ) );
#endif

cleanup:
#if defined(MBEDTLS_ECP_INTERNAL_ALT)
    if( is_grp_capable )
        mbedtls_internal_ecp_free( grp );
#endif /* MBEDTLS_ECP_INTERNAL_ALT */

    mbedtls_ecp_point_free( &mP );

    ECP_RS_LEAVE( ma );

    return( ret );
}

/*
 * Linear combination
 * NOT constant-time
 */
int mbedtls_ecp_muladd( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
             const mbedtls_mpi *m, const mbedtls_ecp_point *P,
             const mbedtls_mpi *n, const mbedtls_ecp_point *Q )
{
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( R   != NULL );
    ECP_VALIDATE_RET( m   != NULL );
    ECP_VALIDATE_RET( P   != NULL );
    ECP_VALIDATE_RET( n   != NULL );
    ECP_VALIDATE_RET( Q   != NULL );
    return( mbedtls_ecp_muladd_restartable( grp, R, m, P, n, Q, NULL ) );
}
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
#define ECP_MPI_INIT(s, n, p) {s, (n), (mbedtls_mpi_uint *)(p)}
#define ECP_MPI_INIT_ARRAY(x)   \
    ECP_MPI_INIT(1, sizeof(x) / sizeof(mbedtls_mpi_uint), x)
/*
 * Constants for the two points other than 0, 1, -1 (mod p) in
 * https://cr.yp.to/ecdh.html#validate
 * See ecp_check_pubkey_x25519().
 */
static const mbedtls_mpi_uint x25519_bad_point_1[] = {
    MBEDTLS_BYTES_TO_T_UINT_8( 0xe0, 0xeb, 0x7a, 0x7c, 0x3b, 0x41, 0xb8, 0xae ),
    MBEDTLS_BYTES_TO_T_UINT_8( 0x16, 0x56, 0xe3, 0xfa, 0xf1, 0x9f, 0xc4, 0x6a ),
    MBEDTLS_BYTES_TO_T_UINT_8( 0xda, 0x09, 0x8d, 0xeb, 0x9c, 0x32, 0xb1, 0xfd ),
    MBEDTLS_BYTES_TO_T_UINT_8( 0x86, 0x62, 0x05, 0x16, 0x5f, 0x49, 0xb8, 0x00 ),
};
static const mbedtls_mpi_uint x25519_bad_point_2[] = {
    MBEDTLS_BYTES_TO_T_UINT_8( 0x5f, 0x9c, 0x95, 0xbc, 0xa3, 0x50, 0x8c, 0x24 ),
    MBEDTLS_BYTES_TO_T_UINT_8( 0xb1, 0xd0, 0xb1, 0x55, 0x9c, 0x83, 0xef, 0x5b ),
    MBEDTLS_BYTES_TO_T_UINT_8( 0x04, 0x44, 0x5c, 0xc4, 0x58, 0x1c, 0x8e, 0x86 ),
    MBEDTLS_BYTES_TO_T_UINT_8( 0xd8, 0x22, 0x4e, 0xdd, 0xd0, 0x9f, 0x11, 0x57 ),
};
static const mbedtls_mpi ecp_x25519_bad_point_1 = ECP_MPI_INIT_ARRAY(
        x25519_bad_point_1 );
static const mbedtls_mpi ecp_x25519_bad_point_2 = ECP_MPI_INIT_ARRAY(
        x25519_bad_point_2 );
#endif /* MBEDTLS_ECP_DP_CURVE25519_ENABLED */

/*
 * Check that the input point is not one of the low-order points.
 * This is recommended by the "May the Fourth" paper:
 * https://eprint.iacr.org/2017/806.pdf
 * Those points are never sent by an honest peer.
 */
static int ecp_check_bad_points_mx( const mbedtls_mpi *X, const mbedtls_mpi *P,
                                    const mbedtls_ecp_group_id grp_id )
{
    int ret;
    mbedtls_mpi XmP;

    mbedtls_mpi_init( &XmP );

    /* Reduce X mod P so that we only need to check values less than P.
     * We know X < 2^256 so we can proceed by subtraction. */
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &XmP, X ) );
    while( mbedtls_mpi_cmp_mpi( &XmP, P ) >= 0 )
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &XmP, &XmP, P ) );

    /* Check against the known bad values that are less than P. For Curve448
     * these are 0, 1 and -1. For Curve25519 we check the values less than P
     * from the following list: https://cr.yp.to/ecdh.html#validate */
    if( mbedtls_mpi_cmp_int( &XmP, 1 ) <= 0 ) /* takes care of 0 and 1 */
    {
        ret = MBEDTLS_ERR_ECP_INVALID_KEY;
        goto cleanup;
    }

#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
    if( grp_id == MBEDTLS_ECP_DP_CURVE25519 )
    {
        if( mbedtls_mpi_cmp_mpi( &XmP, &ecp_x25519_bad_point_1 ) == 0 )
        {
            ret = MBEDTLS_ERR_ECP_INVALID_KEY;
            goto cleanup;
        }

        if( mbedtls_mpi_cmp_mpi( &XmP, &ecp_x25519_bad_point_2 ) == 0 )
        {
            ret = MBEDTLS_ERR_ECP_INVALID_KEY;
            goto cleanup;
        }
    }
#else
    (void) grp_id;
#endif

    /* Final check: check if XmP + 1 is P (final because it changes XmP!) */
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_int( &XmP, &XmP, 1 ) );
    if( mbedtls_mpi_cmp_mpi( &XmP, P ) == 0 )
    {
        ret = MBEDTLS_ERR_ECP_INVALID_KEY;
        goto cleanup;
    }

    ret = 0;

cleanup:
    mbedtls_mpi_free( &XmP );

    return( ret );
}

/*
 * Check validity of a public key for Montgomery curves with x-only schemes
 */
static int ecp_check_pubkey_mx( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt )
{
    /* [Curve25519 p. 5] Just check X is the correct number of bytes */
    /* Allow any public value, if it's too big then we'll just reduce it mod p
     * (RFC 7748 sec. 5 para. 3). */
    if( mbedtls_mpi_size( &pt->X ) > ( grp->nbits + 7 ) / 8 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    /* Implicit in all standards (as they don't consider negative numbers):
     * X must be non-negative. This is normally ensured by the way it's
     * encoded for transmission, but let's be extra sure. */
    if( mbedtls_mpi_cmp_int( &pt->X, 0 ) < 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    return( ecp_check_bad_points_mx( &pt->X, &grp->P, grp->id ) );
}
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

/*
 * Check that a point is valid as a public key
 */
int mbedtls_ecp_check_pubkey( const mbedtls_ecp_group *grp,
                              const mbedtls_ecp_point *pt )
{
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( pt  != NULL );

    /* Must use affine coordinates */
    if( mbedtls_mpi_cmp_int( &pt->Z, 1 ) != 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
        return( ecp_check_pubkey_mx( grp, pt ) );
#endif
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
        return( ecp_check_pubkey_sw( grp, pt ) );
#endif
    return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
}

/*
 * Check that an mbedtls_mpi is valid as a private key
 */
int mbedtls_ecp_check_privkey( const mbedtls_ecp_group *grp,
                               const mbedtls_mpi *d )
{
    ECP_VALIDATE_RET( grp != NULL );
    ECP_VALIDATE_RET( d   != NULL );

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
    {
        /* see RFC 7748 sec. 5 para. 5 */
        if( mbedtls_mpi_get_bit( d, 0 ) != 0 ||
            mbedtls_mpi_get_bit( d, 1 ) != 0 ||
            mbedtls_mpi_bitlen( d ) - 1 != grp->nbits ) /* mbedtls_mpi_bitlen is one-based! */
            return( MBEDTLS_ERR_ECP_INVALID_KEY );

        /* see [Curve25519] page 5 */
        if( grp->nbits == 254 && mbedtls_mpi_get_bit( d, 2 ) != 0 )
            return( MBEDTLS_ERR_ECP_INVALID_KEY );

        return( 0 );
    }
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
    {
        /* see SEC1 3.2 */
        if( mbedtls_mpi_cmp_int( d, 1 ) < 0 ||
            mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
            return( MBEDTLS_ERR_ECP_INVALID_KEY );
        else
            return( 0 );
    }
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

    return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
}

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
MBEDTLS_STATIC_TESTABLE
int mbedtls_ecp_gen_privkey_mx( size_t high_bit,
                                mbedtls_mpi *d,
                                int (*f_rng)(void *, unsigned char *, size_t),
                                void *p_rng )
{
    int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
    size_t n_random_bytes = high_bit / 8 + 1;

    /* [Curve25519] page 5 */
    /* Generate a (high_bit+1)-bit random number by generating just enough
     * random bytes, then shifting out extra bits from the top (necessary
     * when (high_bit+1) is not a multiple of 8). */
    MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( d, n_random_bytes,
                                              f_rng, p_rng ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( d, 8 * n_random_bytes - high_bit - 1 ) );

    MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, high_bit, 1 ) );

    /* Make sure the last two bits are unset for Curve448, three bits for
       Curve25519 */
    MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, 0, 0 ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, 1, 0 ) );
    if( high_bit == 254 )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, 2, 0 ) );
    }

cleanup:
    return( ret );
}
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
static int mbedtls_ecp_gen_privkey_sw(
    const mbedtls_mpi *N, mbedtls_mpi *d,
    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = mbedtls_mpi_random( d, 1, N, f_rng, p_rng );
    switch( ret )
    {
        case MBEDTLS_ERR_MPI_NOT_ACCEPTABLE:
            return( MBEDTLS_ERR_ECP_RANDOM_FAILED );
        default:
            return( ret );
    }
}
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

/*
 * Generate a private key
 */
int mbedtls_ecp_gen_privkey( const mbedtls_ecp_group *grp,
                     mbedtls_mpi *d,
                     int (*f_rng)(void *, unsigned char *, size_t),
                     void *p_rng )
{
    ECP_VALIDATE_RET( grp   != NULL );
    ECP_VALIDATE_RET( d     != NULL );
    ECP_VALIDATE_RET( f_rng != NULL );

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
        return( mbedtls_ecp_gen_privkey_mx( grp->nbits, d, f_rng, p_rng ) );
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
        return( mbedtls_ecp_gen_privkey_sw( &grp->N, d, f_rng, p_rng ) );
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

    return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
}

/*
 * Generate a keypair with configurable base point
 */
int mbedtls_ecp_gen_keypair_base( mbedtls_ecp_group *grp,
                     const mbedtls_ecp_point *G,
                     mbedtls_mpi *d, mbedtls_ecp_point *Q,
                     int (*f_rng)(void *, unsigned char *, size_t),
                     void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECP_VALIDATE_RET( grp   != NULL );
    ECP_VALIDATE_RET( d     != NULL );
    ECP_VALIDATE_RET( G     != NULL );
    ECP_VALIDATE_RET( Q     != NULL );
    ECP_VALIDATE_RET( f_rng != NULL );

    MBEDTLS_MPI_CHK( mbedtls_ecp_gen_privkey( grp, d, f_rng, p_rng ) );
    MBEDTLS_MPI_CHK( mbedtls_ecp_mul( grp, Q, d, G, f_rng, p_rng ) );

cleanup:
    return( ret );
}

/*
 * Generate key pair, wrapper for conventional base point
 */
int mbedtls_ecp_gen_keypair( mbedtls_ecp_group *grp,
                             mbedtls_mpi *d, mbedtls_ecp_point *Q,
                             int (*f_rng)(void *, unsigned char *, size_t),
                             void *p_rng )
{
    ECP_VALIDATE_RET( grp   != NULL );
    ECP_VALIDATE_RET( d     != NULL );
    ECP_VALIDATE_RET( Q     != NULL );
    ECP_VALIDATE_RET( f_rng != NULL );

    return( mbedtls_ecp_gen_keypair_base( grp, &grp->G, d, Q, f_rng, p_rng ) );
}

/*
 * Generate a keypair, prettier wrapper
 */
int mbedtls_ecp_gen_key( mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    ECP_VALIDATE_RET( key   != NULL );
    ECP_VALIDATE_RET( f_rng != NULL );

    if( ( ret = mbedtls_ecp_group_load( &key->grp, grp_id ) ) != 0 )
        return( ret );

    return( mbedtls_ecp_gen_keypair( &key->grp, &key->d, &key->Q, f_rng, p_rng ) );
}

#define ECP_CURVE25519_KEY_SIZE 32
#define ECP_CURVE448_KEY_SIZE   56
/*
 * Read a private key.
 */
int mbedtls_ecp_read_key( mbedtls_ecp_group_id grp_id, mbedtls_ecp_keypair *key,
                          const unsigned char *buf, size_t buflen )
{
    int ret = 0;

    ECP_VALIDATE_RET( key  != NULL );
    ECP_VALIDATE_RET( buf  != NULL );

    if( ( ret = mbedtls_ecp_group_load( &key->grp, grp_id ) ) != 0 )
        return( ret );

    ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( &key->grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
    {
        /*
         * Mask the key as mandated by RFC7748 for Curve25519 and Curve448.
         */
        if( grp_id == MBEDTLS_ECP_DP_CURVE25519 )
        {
            if( buflen != ECP_CURVE25519_KEY_SIZE )
                return( MBEDTLS_ERR_ECP_INVALID_KEY );

            MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary_le( &key->d, buf, buflen ) );

            /* Set the three least significant bits to 0 */
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( &key->d, 0, 0 ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( &key->d, 1, 0 ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( &key->d, 2, 0 ) );

            /* Set the most significant bit to 0 */
            MBEDTLS_MPI_CHK(
                    mbedtls_mpi_set_bit( &key->d,
                                         ECP_CURVE25519_KEY_SIZE * 8 - 1, 0 )
                    );

            /* Set the second most significant bit to 1 */
            MBEDTLS_MPI_CHK(
                    mbedtls_mpi_set_bit( &key->d,
                                         ECP_CURVE25519_KEY_SIZE * 8 - 2, 1 )
                    );
        }
        else if( grp_id == MBEDTLS_ECP_DP_CURVE448 )
        {
            if( buflen != ECP_CURVE448_KEY_SIZE )
                return( MBEDTLS_ERR_ECP_INVALID_KEY );

            MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary_le( &key->d, buf, buflen ) );

            /* Set the two least significant bits to 0 */
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( &key->d, 0, 0 ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( &key->d, 1, 0 ) );

            /* Set the most significant bit to 1 */
            MBEDTLS_MPI_CHK(
                    mbedtls_mpi_set_bit( &key->d,
                                         ECP_CURVE448_KEY_SIZE * 8 - 1, 1 )
                    );
        }
    }

#endif
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( &key->grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_read_binary( &key->d, buf, buflen ) );

        MBEDTLS_MPI_CHK( mbedtls_ecp_check_privkey( &key->grp, &key->d ) );
    }

#endif
cleanup:

    if( ret != 0 )
        mbedtls_mpi_free( &key->d );

    return( ret );
}

/*
 * Write a private key.
 */
int mbedtls_ecp_write_key( mbedtls_ecp_keypair *key,
                           unsigned char *buf, size_t buflen )
{
    int ret = MBEDTLS_ERR_ECP_FEATURE_UNAVAILABLE;

    ECP_VALIDATE_RET( key != NULL );
    ECP_VALIDATE_RET( buf != NULL );

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( mbedtls_ecp_get_type( &key->grp ) == MBEDTLS_ECP_TYPE_MONTGOMERY )
    {
        if( key->grp.id == MBEDTLS_ECP_DP_CURVE25519 )
        {
            if( buflen < ECP_CURVE25519_KEY_SIZE )
                return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );

        }
        else if( key->grp.id == MBEDTLS_ECP_DP_CURVE448 )
        {
            if( buflen < ECP_CURVE448_KEY_SIZE )
                return( MBEDTLS_ERR_ECP_BUFFER_TOO_SMALL );
        }
        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary_le( &key->d, buf, buflen ) );
    }
#endif
#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    if( mbedtls_ecp_get_type( &key->grp ) == MBEDTLS_ECP_TYPE_SHORT_WEIERSTRASS )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_write_binary( &key->d, buf, buflen ) );
    }

#endif
cleanup:

    return( ret );
}


/*
 * Check a public-private key pair
 */
int mbedtls_ecp_check_pub_priv(
        const mbedtls_ecp_keypair *pub, const mbedtls_ecp_keypair *prv,
        int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_ecp_point Q;
    mbedtls_ecp_group grp;
    ECP_VALIDATE_RET( pub != NULL );
    ECP_VALIDATE_RET( prv != NULL );

    if( pub->grp.id == MBEDTLS_ECP_DP_NONE ||
        pub->grp.id != prv->grp.id ||
        mbedtls_mpi_cmp_mpi( &pub->Q.X, &prv->Q.X ) ||
        mbedtls_mpi_cmp_mpi( &pub->Q.Y, &prv->Q.Y ) ||
        mbedtls_mpi_cmp_mpi( &pub->Q.Z, &prv->Q.Z ) )
    {
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
    }

    mbedtls_ecp_point_init( &Q );
    mbedtls_ecp_group_init( &grp );

    /* mbedtls_ecp_mul() needs a non-const group... */
    mbedtls_ecp_group_copy( &grp, &prv->grp );

    /* Also checks d is valid */
    MBEDTLS_MPI_CHK( mbedtls_ecp_mul( &grp, &Q, &prv->d, &prv->grp.G, f_rng, p_rng ) );

    if( mbedtls_mpi_cmp_mpi( &Q.X, &prv->Q.X ) ||
        mbedtls_mpi_cmp_mpi( &Q.Y, &prv->Q.Y ) ||
        mbedtls_mpi_cmp_mpi( &Q.Z, &prv->Q.Z ) )
    {
        ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
        goto cleanup;
    }

cleanup:
    mbedtls_ecp_point_free( &Q );
    mbedtls_ecp_group_free( &grp );

    return( ret );
}

#if defined(MBEDTLS_SELF_TEST)

/*
 * PRNG for test - !!!INSECURE NEVER USE IN PRODUCTION!!!
 *
 * This is the linear congruential generator from numerical recipes,
 * except we only use the low byte as the output. See
 * https://en.wikipedia.org/wiki/Linear_congruential_generator#Parameters_in_common_use
 */
static int self_test_rng( void *ctx, unsigned char *out, size_t len )
{
    static uint32_t state = 42;

    (void) ctx;

    for( size_t i = 0; i < len; i++ )
    {
        state = state * 1664525u + 1013904223u;
        out[i] = (unsigned char) state;
    }

    return( 0 );
}

/* Adjust the exponent to be a valid private point for the specified curve.
 * This is sometimes necessary because we use a single set of exponents
 * for all curves but the validity of values depends on the curve. */
static int self_test_adjust_exponent( const mbedtls_ecp_group *grp,
                                      mbedtls_mpi *m )
{
    int ret = 0;
    switch( grp->id )
    {
        /* If Curve25519 is available, then that's what we use for the
         * Montgomery test, so we don't need the adjustment code. */
#if ! defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
#if defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
        case MBEDTLS_ECP_DP_CURVE448:
            /* Move highest bit from 254 to N-1. Setting bit N-1 is
             * necessary to enforce the highest-bit-set constraint. */
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( m, 254, 0 ) );
            MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( m, grp->nbits, 1 ) );
            /* Copy second-highest bit from 253 to N-2. This is not
             * necessary but improves the test variety a bit. */
            MBEDTLS_MPI_CHK(
                mbedtls_mpi_set_bit( m, grp->nbits - 1,
                                     mbedtls_mpi_get_bit( m, 253 ) ) );
            break;
#endif
#endif /* ! defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED) */
        default:
            /* Non-Montgomery curves and Curve25519 need no adjustment. */
            (void) grp;
            (void) m;
            goto cleanup;
    }
cleanup:
    return( ret );
}

/* Calculate R = m.P for each m in exponents. Check that the number of
 * basic operations doesn't depend on the value of m. */
static int self_test_point( int verbose,
                            mbedtls_ecp_group *grp,
                            mbedtls_ecp_point *R,
                            mbedtls_mpi *m,
                            const mbedtls_ecp_point *P,
                            const char *const *exponents,
                            size_t n_exponents )
{
    int ret = 0;
    size_t i = 0;
    unsigned long add_c_prev, dbl_c_prev, mul_c_prev;
    add_count = 0;
    dbl_count = 0;
    mul_count = 0;

    MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( m, 16, exponents[0] ) );
    MBEDTLS_MPI_CHK( self_test_adjust_exponent( grp, m ) );
    MBEDTLS_MPI_CHK( mbedtls_ecp_mul( grp, R, m, P, self_test_rng, NULL ) );

    for( i = 1; i < n_exponents; i++ )
    {
        add_c_prev = add_count;
        dbl_c_prev = dbl_count;
        mul_c_prev = mul_count;
        add_count = 0;
        dbl_count = 0;
        mul_count = 0;

        MBEDTLS_MPI_CHK( mbedtls_mpi_read_string( m, 16, exponents[i] ) );
        MBEDTLS_MPI_CHK( self_test_adjust_exponent( grp, m ) );
        MBEDTLS_MPI_CHK( mbedtls_ecp_mul( grp, R, m, P, self_test_rng, NULL ) );

        if( add_count != add_c_prev ||
            dbl_count != dbl_c_prev ||
            mul_count != mul_c_prev )
        {
            ret = 1;
            break;
        }
    }

cleanup:
    if( verbose != 0 )
    {
        if( ret != 0 )
            mbedtls_printf( "failed (%u)\n", (unsigned int) i );
        else
            mbedtls_printf( "passed\n" );
    }
    return( ret );
}

/*
 * Checkup routine
 */
int mbedtls_ecp_self_test( int verbose )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    mbedtls_ecp_group grp;
    mbedtls_ecp_point R, P;
    mbedtls_mpi m;

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    /* Exponents especially adapted for secp192k1, which has the lowest
     * order n of all supported curves (secp192r1 is in a slightly larger
     * field but the order of its base point is slightly smaller). */
    const char *sw_exponents[] =
    {
        "000000000000000000000000000000000000000000000001", /* one */
        "FFFFFFFFFFFFFFFFFFFFFFFE26F2FC170F69466A74DEFD8C", /* n - 1 */
        "5EA6F389A38B8BC81E767753B15AA5569E1782E30ABE7D25", /* random */
        "400000000000000000000000000000000000000000000000", /* one and zeros */
        "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF", /* all ones */
        "555555555555555555555555555555555555555555555555", /* 101010... */
    };
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */
#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    const char *m_exponents[] =
    {
        /* Valid private values for Curve25519. In a build with Curve448
         * but not Curve25519, they will be adjusted in
         * self_test_adjust_exponent(). */
        "4000000000000000000000000000000000000000000000000000000000000000",
        "5C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C30",
        "5715ECCE24583F7A7023C24164390586842E816D7280A49EF6DF4EAE6B280BF8",
        "41A2B017516F6D254E1F002BCCBADD54BE30F8CEC737A0E912B4963B6BA74460",
        "5555555555555555555555555555555555555555555555555555555555555550",
        "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8",
    };
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

    mbedtls_ecp_group_init( &grp );
    mbedtls_ecp_point_init( &R );
    mbedtls_ecp_point_init( &P );
    mbedtls_mpi_init( &m );

#if defined(MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED)
    /* Use secp192r1 if available, or any available curve */
#if defined(MBEDTLS_ECP_DP_SECP192R1_ENABLED)
    MBEDTLS_MPI_CHK( mbedtls_ecp_group_load( &grp, MBEDTLS_ECP_DP_SECP192R1 ) );
#else
    MBEDTLS_MPI_CHK( mbedtls_ecp_group_load( &grp, mbedtls_ecp_curve_list()->grp_id ) );
#endif

    if( verbose != 0 )
        mbedtls_printf( "  ECP SW test #1 (constant op_count, base point G): " );
    /* Do a dummy multiplication first to trigger precomputation */
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &m, 2 ) );
    MBEDTLS_MPI_CHK( mbedtls_ecp_mul( &grp, &P, &m, &grp.G, self_test_rng, NULL ) );
    ret = self_test_point( verbose,
                           &grp, &R, &m, &grp.G,
                           sw_exponents,
                           sizeof( sw_exponents ) / sizeof( sw_exponents[0] ));
    if( ret != 0 )
        goto cleanup;

    if( verbose != 0 )
        mbedtls_printf( "  ECP SW test #2 (constant op_count, other point): " );
    /* We computed P = 2G last time, use it */
    ret = self_test_point( verbose,
                           &grp, &R, &m, &P,
                           sw_exponents,
                           sizeof( sw_exponents ) / sizeof( sw_exponents[0] ));
    if( ret != 0 )
        goto cleanup;

    mbedtls_ecp_group_free( &grp );
    mbedtls_ecp_point_free( &R );
#endif /* MBEDTLS_ECP_SHORT_WEIERSTRASS_ENABLED */

#if defined(MBEDTLS_ECP_MONTGOMERY_ENABLED)
    if( verbose != 0 )
        mbedtls_printf( "  ECP Montgomery test (constant op_count): " );
#if defined(MBEDTLS_ECP_DP_CURVE25519_ENABLED)
    MBEDTLS_MPI_CHK( mbedtls_ecp_group_load( &grp, MBEDTLS_ECP_DP_CURVE25519 ) );
#elif defined(MBEDTLS_ECP_DP_CURVE448_ENABLED)
    MBEDTLS_MPI_CHK( mbedtls_ecp_group_load( &grp, MBEDTLS_ECP_DP_CURVE448 ) );
#else
#error "MBEDTLS_ECP_MONTGOMERY_ENABLED is defined, but no curve is supported for self-test"
#endif
    ret = self_test_point( verbose,
                           &grp, &R, &m, &grp.G,
                           m_exponents,
                           sizeof( m_exponents ) / sizeof( m_exponents[0] ));
    if( ret != 0 )
        goto cleanup;
#endif /* MBEDTLS_ECP_MONTGOMERY_ENABLED */

cleanup:

    if( ret < 0 && verbose != 0 )
        mbedtls_printf( "Unexpected error, return code = %08X\n", (unsigned int) ret );

    mbedtls_ecp_group_free( &grp );
    mbedtls_ecp_point_free( &R );
    mbedtls_ecp_point_free( &P );
    mbedtls_mpi_free( &m );

    if( verbose != 0 )
        mbedtls_printf( "\n" );

    return( ret );
}

#endif /* MBEDTLS_SELF_TEST */

#endif /* !MBEDTLS_ECP_ALT */

#endif /* MBEDTLS_ECP_C */
