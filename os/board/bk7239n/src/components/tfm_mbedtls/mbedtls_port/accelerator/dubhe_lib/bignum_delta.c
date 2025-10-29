#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#include "common.h"

#if defined(MBEDTLS_BIGNUM_C)

#include "mbedtls/bignum.h"
#include "bn_mul.h"
#include "mbedtls/platform_util.h"
#include "mbedtls/error.h"
#include "constant_time_internal.h"

#include <limits.h>
#include <string.h>

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_printf     printf
#define mbedtls_calloc    calloc
#define mbedtls_free       free
#endif

#define ciL    (sizeof(mbedtls_mpi_uint))         /* chars in limb  */
#define biL    (ciL << 3)               /* bits  in limb  */
#define biH    (ciL << 2)               /* half limb size */

/*
 * Convert between bits/chars and number of limbs
 * Divide first in order to avoid potential overflows
 */
#define BITS_TO_LIMBS(i)  ( (i) / biL + ( (i) % biL != 0 ) )
#define CHARS_TO_LIMBS(i) ( (i) / ciL + ( (i) % ciL != 0 ) )

/* Get a specific byte, without range checks. */
#define GET_BYTE( X, i )                                \
    ( ( ( X )->p[( i ) / ciL] >> ( ( ( i ) % ciL ) * 8 ) ) & 0xff )

/* Resize X to have exactly n limbs and set it to 0. */
int mbedtls_mpi_resize_clear( mbedtls_mpi *X, size_t limbs )
{
    if( limbs == 0 )
    {
        mbedtls_mpi_free( X );
        return( 0 );
    }
    else if( X->n == limbs )
    {
        memset( X->p, 0, limbs * ciL );
        X->s = 1;
        return( 0 );
    }
    else
    {
        mbedtls_mpi_free( X );
        return( mbedtls_mpi_grow( X, limbs ) );
    }
}

/* Convert a big-endian byte array aligned to the size of mbedtls_mpi_uint
 * into the storage form used by mbedtls_mpi. */

static mbedtls_mpi_uint mpi_uint_bigendian_to_host_c( mbedtls_mpi_uint x )
{
    uint8_t i;
    unsigned char *x_ptr;
    mbedtls_mpi_uint tmp = 0;

    for( i = 0, x_ptr = (unsigned char*) &x; i < ciL; i++, x_ptr++ )
    {
        tmp <<= CHAR_BIT;
        tmp |= (mbedtls_mpi_uint) *x_ptr;
    }

    return( tmp );
}

static mbedtls_mpi_uint mpi_uint_bigendian_to_host( mbedtls_mpi_uint x )
{
#if defined(__BYTE_ORDER__)

/* Nothing to do on bigendian systems. */
#if ( __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ )
    return( x );
#endif /* __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ */

#if ( __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ )

/* For GCC and Clang, have builtins for byte swapping. */
#if defined(__GNUC__) && defined(__GNUC_PREREQ)
#if __GNUC_PREREQ(4,3)
#define have_bswap
#endif
#endif

#if defined(__clang__) && defined(__has_builtin)
#if __has_builtin(__builtin_bswap32)  &&                 \
    __has_builtin(__builtin_bswap64)
#define have_bswap
#endif
#endif

#if defined(have_bswap)
    /* The compiler is hopefully able to statically evaluate this! */
    switch( sizeof(mbedtls_mpi_uint) )
    {
        case 4:
            return( __builtin_bswap32(x) );
        case 8:
            return( __builtin_bswap64(x) );
    }
#endif
#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */
#endif /* __BYTE_ORDER__ */

    /* Fall back to C-based reordering if we don't know the byte order
     * or we couldn't use a compiler-specific builtin. */
    return( mpi_uint_bigendian_to_host_c( x ) );
}

static void mpi_bigendian_to_host( mbedtls_mpi_uint * const p, size_t limbs )
{
    mbedtls_mpi_uint *cur_limb_left;
    mbedtls_mpi_uint *cur_limb_right;
    if( limbs == 0 )
        return;

    /*
     * Traverse limbs and
     * - adapt byte-order in each limb
     * - swap the limbs themselves.
     * For that, simultaneously traverse the limbs from left to right
     * and from right to left, as long as the left index is not bigger
     * than the right index (it's not a problem if limbs is odd and the
     * indices coincide in the last iteration).
     */
    for( cur_limb_left = p, cur_limb_right = p + ( limbs - 1 );
         cur_limb_left <= cur_limb_right;
         cur_limb_left++, cur_limb_right-- )
    {
        mbedtls_mpi_uint tmp;
        /* Note that if cur_limb_left == cur_limb_right,
         * this code effectively swaps the bytes only once. */
        tmp             = mpi_uint_bigendian_to_host( *cur_limb_left  );
        *cur_limb_left  = mpi_uint_bigendian_to_host( *cur_limb_right );
        *cur_limb_right = tmp;
    }
}

/* Fill X with n_bytes random bytes.
 * X must already have room for those bytes.
 * The ordering of the bytes returned from the RNG is suitable for
 * deterministic ECDSA (see RFC 6979 §3.3 and mbedtls_mpi_random()).
 * The size and sign of X are unchanged.
 * n_bytes must not be 0.
 */
static int mpi_fill_random_internal(
    mbedtls_mpi *X, size_t n_bytes,
    int (*f_rng)(void *, unsigned char *, size_t), void *p_rng )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    const size_t limbs = CHARS_TO_LIMBS( n_bytes );
    const size_t overhead = ( limbs * ciL ) - n_bytes;

    if( X->n < limbs )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    memset( X->p, 0, overhead );
    memset( (unsigned char *) X->p + limbs * ciL, 0, ( X->n - limbs ) * ciL );
    MBEDTLS_MPI_CHK( f_rng( p_rng, (unsigned char *) X->p + overhead, n_bytes ) );
    mpi_bigendian_to_host( X->p, limbs );

cleanup:
    return( ret );
}

int mbedtls_mpi_random_3_0( mbedtls_mpi *X,
                        mbedtls_mpi_sint min,
                        const mbedtls_mpi *N,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng )
{
    int ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    int count;
    unsigned lt_lower = 1, lt_upper = 0;
    size_t n_bits = mbedtls_mpi_bitlen( N );
    size_t n_bytes = ( n_bits + 7 ) / 8;
    mbedtls_mpi lower_bound;

    if( min < 0 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );
    if( mbedtls_mpi_cmp_int( N, min ) <= 0 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    /*
    * When min == 0, each try has at worst a probability 1/2 of failing
    * (the msb has a probability 1/2 of being 0, and then the result will
    * be < N), so after 30 tries failure probability is a most 2**(-30).
    *
    * When N is just below a power of 2, as is the case when generating
    * a random scalar on most elliptic curves, 1 try is enough with
    * overwhelming probability. When N is just above a power of 2,
    * as when generating a random scalar on secp224k1, each try has
    * a probability of failing that is almost 1/2.
    *
    * The probabilities are almost the same if min is nonzero but negligible
    * compared to N. This is always the case when N is crypto-sized, but
    * it's convenient to support small N for testing purposes. When N
    * is small, use a higher repeat count, otherwise the probability of
    * failure is macroscopic.
    */
    count = ( n_bytes > 4 ? 30 : 250 );

    mbedtls_mpi_init( &lower_bound );

    /* Ensure that target MPI has exactly the same number of limbs
    * as the upper bound, even if the upper bound has leading zeros.
    * This is necessary for the mbedtls_mpi_lt_mpi_ct() check. */
    MBEDTLS_MPI_CHK( mbedtls_mpi_resize_clear( X, N->n ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_grow( &lower_bound, N->n ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_lset( &lower_bound, min ) );

    /*
    * Match the procedure given in RFC 6979 §3.3 (deterministic ECDSA)
    * when f_rng is a suitably parametrized instance of HMAC_DRBG:
    * - use the same byte ordering;
    * - keep the leftmost n_bits bits of the generated octet string;
    * - try until result is in the desired range.
    * This also avoids any bias, which is especially important for ECDSA.
    */
    do
    {
        MBEDTLS_MPI_CHK( mpi_fill_random_internal( X, n_bytes, f_rng, p_rng ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( X, 8 * n_bytes - n_bits ) );

        if( --count == 0 )
        {
            ret = MBEDTLS_ERR_MPI_NOT_ACCEPTABLE;
            goto cleanup;
        }

        MBEDTLS_MPI_CHK( mbedtls_mpi_lt_mpi_ct( X, &lower_bound, &lt_lower ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_lt_mpi_ct( X, N, &lt_upper ) );
    }
    while( lt_lower != 0 || lt_upper == 0 );

cleanup:
    mbedtls_mpi_free( &lower_bound );
    return( ret );
}

int mbedtls_mpi_random( mbedtls_mpi *X,
                        mbedtls_mpi_sint min,
                        const mbedtls_mpi *N,
                        int (*f_rng)(void *, unsigned char *, size_t),
                        void *p_rng )
{
    int ret = MBEDTLS_ERR_MPI_BAD_INPUT_DATA;
    int count = 0;
    size_t n_bits = mbedtls_mpi_bitlen( N );
    size_t n_bytes = ( n_bits + 7 ) / 8;

    if( min < 0 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );
    if( mbedtls_mpi_cmp_int( N, min ) <= 0 )
        return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

    /*
     * Match the procedure given in RFC 6979 (deterministic ECDSA):
     * - use the same byte ordering;
     * - keep the leftmost nbits bits of the generated octet string;
     * - try until result is in the desired range.
     * This also avoids any biais, which is especially important for ECDSA.
     */
    do
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( X, n_bytes, f_rng, p_rng ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( X, 8 * n_bytes - n_bytes ) );

        /*
         * Each try has at worst a probability 1/2 of failing (the msb has
         * a probability 1/2 of being 0, and then the result will be < N),
         * so after 30 tries failure probability is a most 2**(-30).
         *
         * For most curves, 1 try is enough with overwhelming probability,
         * since N starts with a lot of 1s in binary, but some curves
         * such as secp224k1 are actually very close to the worst case.
         */
        if( ++count > 30 )
            return( MBEDTLS_ERR_MPI_NOT_ACCEPTABLE );
    }
    while( mbedtls_mpi_cmp_int( X, 1 ) < 0 ||
           mbedtls_mpi_cmp_mpi( X, N ) >= 0 );

cleanup:
    return ( ret );
}

/*
 * Import X from unsigned binary data, little endian
 */
int mbedtls_mpi_read_binary_le( mbedtls_mpi *X,
                                const unsigned char *buf, size_t buflen )
{
    int ret = MBEDTLS_ERR_ERROR_CORRUPTION_DETECTED;
    size_t i;
    size_t const limbs = CHARS_TO_LIMBS( buflen );

    /* Ensure that target MPI has exactly the necessary number of limbs */
    MBEDTLS_MPI_CHK( mbedtls_mpi_resize_clear( X, limbs ) );

    for( i = 0; i < buflen; i++ )
        X->p[i / ciL] |= ((mbedtls_mpi_uint) buf[i]) << ((i % ciL) << 3);

cleanup:

    /*
     * This function is also used to import keys. However, wiping the buffers
     * upon failure is not necessary because failure only can happen before any
     * input is copied.
     */
    return( ret );
}

/*
 * Export X into unsigned binary data, little endian
 */
int mbedtls_mpi_write_binary_le( const mbedtls_mpi *X,
                                 unsigned char *buf, size_t buflen )
{
    size_t stored_bytes = X->n * ciL;
    size_t bytes_to_copy;
    size_t i;

    if( stored_bytes < buflen )
    {
        bytes_to_copy = stored_bytes;
    }
    else
    {
        bytes_to_copy = buflen;

        /* The output buffer is smaller than the allocated size of X.
         * However X may fit if its leading bytes are zero. */
        for( i = bytes_to_copy; i < stored_bytes; i++ )
        {
            if( GET_BYTE( X, i ) != 0 )
                return( MBEDTLS_ERR_MPI_BUFFER_TOO_SMALL );
        }
    }

    for( i = 0; i < bytes_to_copy; i++ )
        buf[i] = GET_BYTE( X, i );

    if( stored_bytes < buflen )
    {
        /* Write trailing 0 bytes */
        memset( buf + stored_bytes, 0, buflen - stored_bytes );
    }

    return( 0 );
}

/*
 * Pseudo-primality test: small factors, then Miller-Rabin
 */
int mbedtls_mpi_is_prime_ext( const mbedtls_mpi *X, int rounds,
                              int (*f_rng)(void *, unsigned char *, size_t),
                              void *p_rng )
{
    /*rounds is fixed to PRIME_JUDGE_MAX_TMP_REG_NUM by DUBHE*/
    return mbedtls_mpi_is_prime(X, f_rng, p_rng);
}
#endif
