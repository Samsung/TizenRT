/**
 * @if copyright_display
 *      Copyright (C), 2018-2021, Arm Technology (China) Co., Ltd.
 *      All rights reserved
 *
 *      The content of this file or document is CONFIDENTIAL and PROPRIETARY
 *      to Arm Technology (China) Co., Ltd. It is subject to the terms of a
 *      License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *      restricting among other things, the use, reproduction, distribution
 *      and transfer.  Each of the embodiments, including this information and
 *      any derivative work shall retain this copyright notice.
 * @endif
 */

#include "dubhe_otp.h"
#include "pal_time.h"
#include "pal_log.h"
#include "dubhe_event.h"
#include "string.h"

#define ACCESS_OVERFLOW(a, b, _size) ( ( ( a ) + ( b ) < ( a ) ) ?             \
                                       1 : ( ( a ) + ( b ) < ( b ) ) ?         \
                                       1 : ( ( a ) + ( b ) > ( _size ) ) ?     \
                                       1 : 0 )

#define DBH_OTP_LOCK() do{ \
    dubhe_mutex_lock( DBH_OTP_MUTEX ); \
}while (0)

#define DBH_OTP_UNLOCK() do{ \
    dubhe_mutex_unlock( DBH_OTP_MUTEX ); \
}while (0)

#define IS_ALIGNED( x, align )                                                 \
    ( !( ( (unsigned long) ( x ) ) & ( (unsigned long) ( align ) - 1 ) ) )

#define DBH_LIFE_CYCLE_MASK     (0x07)

#define DBH_OTP_INIT_DONE() (DBH_READ_REGISTER( OTP_MGR, OTP_SET )  & 0x01)

#define DBH_OTP_PUF_READY() (DBH_READ_REGISTER( OTP_MGR, OTP_UPDATE_STAT ) & 0x04)

#define IS_SHADOW_REGISTER_REGION(region) ( OTP_INFO_MODEL_ID == region ?     \
                                        1 : OTP_INFO_MODEL_KEY == region ?    \
                                        1 : OTP_INFO_DEV_ID == region ?       \
                                        1 : OTP_INFO_DEV_ROOT_KEY == region ? \
                                        1 : OTP_INFO_LIFE_CYCLE == region ?   \
                                        1 : OTP_INFO_LOCK_CTRL == region ?    \
                                        1 : 0                                 \
                                        )
#define BIT( n ) ( 1UL << ( n ) )
#define BIT_MASK( V, n ) ( (V) &BIT( n ) )
#define SUB_REGION_b_SIZE        (128)
#define B_b_SIZE                 (8)
#define SUB_REGION_B_SIZE        (SUB_REGION_b_SIZE / B_b_SIZE)

#define SECTION_MATCH(_locks, _offset, _len)                                    \
        do{                                                                     \
            int _i;                                                             \
            int _start_block;                                                   \
            int _end_block;                                                     \
            _start_block = ALIGN_DOWN( (_offset), SUB_REGION_B_SIZE ) /         \
                           SUB_REGION_B_SIZE;                                   \
            _end_block = ALIGN_UP( (_offset) + (_len), SUB_REGION_B_SIZE ) /    \
                           SUB_REGION_B_SIZE;                                   \
            if(_end_block == _start_block){                                     \
                _end_block += 1;                                                \
            }                                                                   \
            for( _i = _start_block; _i < _end_block; _i++ ){                    \
                if( BIT_MASK(_locks, _i ) ){                                    \
                    b_lock = true;                                              \
                    break;                                                      \
                }                                                               \
            }                                                                   \
        }while (0)

static int _arm_ce_read_otp( arm_ce_dev_lcs_t lcs,
                            arm_ce_otp_info_t type,
                            void *output,
                            size_t size,
                            size_t offset );


static void _DBH_READ_OTP_INFO( size_t offset, size_t size, void *output )
{
    size_t read_size = 0;
    size_t copy_size = 0;
    size_t r_off = 0;
    size_t r_off_off = 0;
    volatile uint32_t _tmp;
    uint8_t *p_tmp = (uint8_t *)&_tmp;

    r_off = ALIGN_DOWN(offset, DBH_OTP_WORD_SIZE);
    r_off_off = offset % DBH_OTP_WORD_SIZE;
    while ( read_size < size ) {
        _tmp = DBH_READ_REGISTER2( ( DBH_BASE_OTP_SPACE ) +  r_off );
    #if defined( CFG_DBH_OTP_WR_ZERO )
        _tmp = = ~_tmp;
    #endif
        if ( 0 == read_size ) {
            copy_size = DBH_OTP_WORD_SIZE - r_off_off;
        } else {
            copy_size = DBH_OTP_WORD_SIZE;
            r_off_off = 0;
        }
        copy_size = ( copy_size > ( size - read_size ) ) ?
                            ( size - read_size ) : copy_size;
        memcpy((uint8_t *)output + read_size,
                    (uint8_t *)p_tmp+r_off_off, copy_size);
        read_size += copy_size;
        r_off += DBH_OTP_WORD_SIZE;
    }
}

#if defined(DUBHE_SECURE)
/* Write OTP information */
#if defined( CFG_DBH_OTP_WR_ONE )
static void _DBH_WRITE_OTP_INFO( size_t offset,
                                        size_t size,
                                        const void *input )
{
    size_t write_size = 0;
    size_t wcopy_size = 0;
    size_t w_off = ALIGN_DOWN( offset, DBH_OTP_WORD_SIZE );
    size_t w_off_off = ( offset % DBH_OTP_WORD_SIZE );
    volatile uint32_t value = 0;
    uint8_t *p_input = (uint8_t *)input;

    while ( write_size < size ) {
        if ( 0 == write_size ) {
            wcopy_size = DBH_OTP_WORD_SIZE - w_off_off;
        } else {
            w_off_off = 0;
            wcopy_size = DBH_OTP_WORD_SIZE;
        }
        wcopy_size = ( wcopy_size > ( size - write_size ) ) ?
                            ( size - write_size ) : wcopy_size;
        value = DBH_READ_REGISTER2( ( DBH_BASE_OTP_SPACE ) +  w_off );
        OR_BLOCK((uint8_t *)&value, p_input+write_size,
                    w_off_off, wcopy_size);
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR_ADDR, w_off );
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR_DATA, value );
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR, 0x1 );
        write_size += wcopy_size;
        w_off += DBH_OTP_WORD_SIZE;
    }
}

#elif defined( CFG_DBH_OTP_WR_ZERO )
static void _DBH_WRITE_OTP_INFO( size_t offset,
                                        size_t size,
                                        const void *input )
{
    size_t write_size = 0;
    size_t wcopy_size = 0;
    size_t w_off = ALIGN_DOWN( offset, DBH_OTP_WORD_SIZE );
    size_t w_off_off = ( offset % DBH_OTP_WORD_SIZE );
    volatile uint32_t value = 0;

    while ( write_size < size ) {
        _tmp = 0;
        if ( 0 == write_size ) {
            wcopy_size = DBH_OTP_WORD_SIZE - w_off_off;
        } else {
            w_off_off = 0;
            wcopy_size = DBH_OTP_WORD_SIZE;
        }
        wcopy_size = ( wcopy_size > ( size - write_size ) ) ?
                            ( size - write_size ) : wcopy_size;
        value = DBH_READ_REGISTER2( ( DBH_BASE_OTP_SPACE ) +  w_off );
        value = ~value;
        OR_BLOCK((uint8_t *)&value, p_input+write_size, w_off_off, wcopy_size);
        value = ~value;
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR_ADDR, w_off );
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR_DATA, value );
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR, 0x1 );
        write_size += wcopy_size;
        w_off += DBH_OTP_WORD_SIZE;
    }
}
#else
static void _DBH_WRITE_OTP_INFO( size_t offset,
                                        size_t size,
                                        const void *input )
{
    size_t write_size = 0;
    size_t wcopy_size = 0;
    size_t w_off = ALIGN_DOWN( offset, DBH_OTP_WORD_SIZE );
    size_t w_off_off = ( offset % DBH_OTP_WORD_SIZE );
    volatile uint32_t value = 0;
    uint8_t *p_input =(uint8_t *)input;

    while ( write_size < size ) {
        _tmp = 0;
        if ( 0 == write_size ) {
            wcopy_size = DBH_OTP_WORD_SIZE - w_off_off;
        } else {
            w_off_off = 0;
            wcopy_size = DBH_OTP_WORD_SIZE;
        }
        wcopy_size = ( wcopy_size > ( size - write_size ) ) ?
                            ( size - write_size ) : wcopy_size;
        memcpy((unsigned char *)&_tmp +w_off_off,
                    (uint8_t *)input, wcopy_size);
        if (DBH_OTP_WORD_SIZE != wcopy_size ) {
            value = DBH_READ_REGISTER2( ( DBH_BASE_OTP_SPACE ) +  w_off );
            OR_BLOCK((uint8_t *)&value, p_input+write_size,
                     w_off_off, wcopy_size);
        } else {
            memcpy((uint8_t *)&value, p_input+write_size, wcopy_size);
        }

        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR_ADDR, w_off );
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR_DATA, value );
        DBH_WRITE_REGISTER( OTP_MGR, OTP_WR, 0x1 );
        write_size += wcopy_size;
        w_off += DBH_OTP_WORD_SIZE;
    }
}
#endif

static int arm_ce_otp_wait_write_done( void )
{
    uint8_t result;
    uint32_t value;

    for ( ;; ) {
        value  = DBH_READ_REGISTER( OTP_MGR, OTP_UPDATE_STAT );
        result = value & 0x1;
        if ( result != 0 )
            pal_udelay( 2000 );
        else
            break;
    }

    if ( ( ( value >> 0x1 ) & 0x1 ) == 0x1 )
        return ( DBH_OTP_UPDATE_FAIL );
    else
        return ( DBH_OTP_UPDATE_SUCCESS );
}

static void arm_ce_otp_wait_init_done( void )
{
    uint8_t result;
    volatile uint32_t value;

    for ( ;; ) {
        value  = DBH_READ_REGISTER( OTP_MGR, OTP_SET );
        result = value & 0x1;
        if ( result != 1 )
            pal_udelay( 2000 );
        else
            break;
    }
}
#endif

static arm_ce_dev_lcs_t _arm_ce_get_lcs( void )
{
    arm_ce_dev_lcs_t lcs;
    uint32_t output = 0;

    _DBH_READ_OTP_INFO( DBH_OTP_LCS_OFFSET, DBH_OTP_LCS_SIZE, &output );
    lcs = ( arm_ce_dev_lcs_t )( output & DBH_LIFE_CYCLE_MASK );
    if ( ( lcs == DBH_DEV_LCS_DR ) || ( lcs == DBH_DEV_LCS_CM )
         || ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DM ) )
        return ( lcs );
    else
        return ( DBH_DEV_LCS_INVALID );
}

static int _arm_ce_otp_get_lock( arm_ce_otp_lock_t *lock )
{
    int ret = 0;
    arm_ce_dev_lcs_t lcs;

    if ( NULL == lock ) {
        ret = DBH_OTP_PARAM_INVALID_ERR;
        goto exit;
    }

    lcs = _arm_ce_get_lcs( );

    if ( lcs == DBH_DEV_LCS_INVALID ) {
        ret = ( DBH_OTP_LCS_ERR );
        goto exit;
    }
    ret = _arm_ce_read_otp( lcs, OTP_INFO_LOCK_CTRL, &lock->value, sizeof( arm_ce_otp_lock_t ), 0 );
exit:
    return ret;
}


static bool _is_region_lock(arm_ce_otp_info_t region, size_t offset, size_t len)
{
    bool b_lock = false;
    arm_ce_otp_lock_t lock;
    _arm_ce_otp_get_lock(&lock);
    switch(region){
        case OTP_INFO_USR_NON_SEC_REGION:
            SECTION_MATCH(lock.locks.usr_nonsec_region, offset, len);
            break;
        case OTP_INFO_USR_SEC_REGION:
            SECTION_MATCH(lock.locks.usr_sec_region, offset, len);
            break;
        case OTP_INFO_MODEL_ID:
            b_lock = (lock.locks.model_id == 1) ? true : false;
            break;
        case OTP_INFO_MODEL_KEY:
            b_lock = (lock.locks.model_key == 1) ? true : false;
            break;
        case OTP_INFO_DEV_ROOT_KEY:
            b_lock = (lock.locks.device_root_key == 1) ? true : false;
            break;
        case OTP_INFO_DEV_ID:
            b_lock = (lock.locks.device_id == 1) ? true : false;
            break;
        case OTP_INFO_SEC_BT_PK_HASH:
            b_lock = (lock.locks.secure_boot_pk_hash == 1) ? true : false;
            break;
        case OTP_INFO_SEC_DBG_PK_HASH:
            b_lock = (lock.locks.secure_dbg_pk_hash == 1) ? true : false;
            break;
        default:
            break;
    }
    return b_lock;
}

#if defined( DBH_OTP_PUF_SUPPORT )
#if defined(DUBHE_SECURE)
static int arm_ce_otp_power_up( void )
{
    volatile uint32_t value     = 0;
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#endif
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    value     = DBH_READ_REGISTER( OTP_MGR, OTP_SET );
    /* Set PENVDD2_VDD2 to 1*/
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PENVDD2, value, 0x1 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    pal_udelay( 1 );
    /* Set PDSTB to 1 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PDSTB, value, 0x1 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    pal_udelay(2);
    /* Set PRESETn to 1 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PRESETN, value, 0x1 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    return 0;
}
#endif

int arm_ce_otp_power_down( void )
{
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value;
    value     = DBH_READ_REGISTER( OTP_MGR, OTP_SET );
    /* Set PRESETn to 0 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PRESETN, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    /* Set PDSTB to 0 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PDSTB, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    /* Set PENVDD2_VDD2 to 0 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PENVDD2, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    return 0;
#endif
}

int arm_ce_otp_deep_standby( void )
{
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value;
#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    value     = DBH_READ_REGISTER( OTP_MGR, OTP_SET );

    /*set PRESETn to 0*/

    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PRESETN, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    pal_udelay(100);
    /* Set PDSTB to 0 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PDSTB, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    while (DBH_OTP_PUF_READY())
    {
        pal_udelay(2);
    }

#if defined( DUBHE_FOR_RUNTIME )
    /* release command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return 0;
#endif
}

bool arm_ce_otp_puf_ready( void )
{
    return (DBH_OTP_PUF_READY())?true:false;
}

static int _is_enrollment_done( void )
{
    arm_ce_otp_lock_t lock;
    memset( &lock, 0x00, sizeof( lock ) );
    _arm_ce_otp_get_lock( &lock );
    return lock.locks.device_root_key;
}

int arm_ce_do_puf_quality_check( void )
{
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value = 0x00;
    uint32_t origin = 0;
    uint8_t dev_key[DBH_OTP_DEV_ROOT_KEY_SIZE] = {0};
    int ret = 0;
    arm_ce_dev_lcs_t lcs;
#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    if ( !DBH_OTP_PUF_READY() )
    {
        PAL_LOG_ERR("PUF not ready yet\n");
        ret = DBH_OTP_PERMISSION_DENY;
        goto _exit;
    }
    /* back up OTP_SET here, need to recover later */
    value     = DBH_READ_REGISTER( OTP_MGR, OTP_SET );
    origin    = value;
    /* be careful here can't use direct read,
       because PTA & PAPUF is still under quality
       check stage, if try to will lead to
       unpredictable consequence,such as reboot...
       OTP_SET will be reset after ROOT KEY is read */
    DBH_OTP_SET_SHADOW_READ();
    lcs = _arm_ce_get_lcs();
    switch(lcs) {
    case DBH_DEV_LCS_DD:
    case DBH_DEV_LCS_DR:
        ret = DBH_OTP_LCS_ERR;
        goto clean_up;
    default:
        break;
    }

    /* Set PTA[1:0] to 10, PAPUF to 1 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PTA, value, 0x2 );
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PAPUF, value, 0x1 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );

    /* !!!Direct read root key this time for quality check */
    DBH_OTP_SET_DIRECT_READ();
    _DBH_READ_OTP_INFO( DBH_OTP_DEV_ROOT_KEY_OFFSET,
            DBH_OTP_DEV_ROOT_KEY_SIZE, dev_key );
    /* check first world least sinificant bit
     * 0x1 indicates quality check success,
     * 0x0 failed, please refer OTP spec */
    if ( ( dev_key[3] & 0x1 ) == 0x0 ) {
        ret = ( DBH_OTP_QUALITY_CHECK_ERR );
    }
    /* recover OPT_SET register must before reading root key */
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, origin );
    DBH_OTP_SET_DIRECT_READ();
    /* !!! Must needed Direct read root key
       this time for recovering root key after
       being polluted by quality check */
    _DBH_READ_OTP_INFO( DBH_OTP_DEV_ROOT_KEY_OFFSET,
                    DBH_OTP_DEV_ROOT_KEY_SIZE, dev_key );
    DBH_OTP_SET_SHADOW_READ();
clean_up:
    /* recover OPT_SET register
       incase of LINE 350 */
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, origin );
_exit:
#if defined( DUBHE_FOR_RUNTIME )
    /* release command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return ret;
#endif
}

int arm_ce_otp_puf_initial_margin_read( arm_ce_otp_info_t area,
                                void *output,
                                size_t *size,
                                size_t offset )
{
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value = 0, origin = 0;
    int32_t ret = 0;
    arm_ce_dev_lcs_t lcs;
    if (NULL == size || NULL == output) {
        return DBH_OTP_PARAM_INVALID_ERR;
    }
#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    value = DBH_READ_REGISTER( OTP_MGR, OTP_SET );

    origin    = value;

    /* Set PTA[1:0] to 10, PAPUF to 0 */

    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PTA, value, 0x2 );
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PAPUF, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );

    lcs = _arm_ce_get_lcs( );
    if ( lcs == DBH_DEV_LCS_INVALID ) {
        ret = ( DBH_OTP_LCS_ERR );
        goto exit;
    }
    ret = _arm_ce_read_otp( lcs, area, output, *size, offset );
    /* Read badk data and recover*/
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, origin );
exit:
#if defined( DUBHE_FOR_RUNTIME )
    /* release command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return ret;
#endif
}

int arm_ce_otp_puf_pgm_margin_read( arm_ce_otp_info_t area,
                                void *output,
                                size_t *size,
                                size_t offset )
{
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value = 0, origin = 0;
    int ret = 0;
    arm_ce_dev_lcs_t lcs;
    if (NULL == size || NULL == output) {
        return DBH_OTP_PARAM_INVALID_ERR;
    }
#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    value = DBH_READ_REGISTER( OTP_MGR, OTP_SET );

    origin    = value;
    /* Set PTA[1:0] to 11, PAPUF to 0 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PTA, value, 0x3 );
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PAPUF, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );

    lcs = _arm_ce_get_lcs( );
    if ( lcs == DBH_DEV_LCS_INVALID ) {
        ret = ( DBH_OTP_LCS_ERR );
        goto exit;
    }
    ret = _arm_ce_read_otp( lcs, area, output, *size, offset );
    /* Read back data and recover*/
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, origin );
exit:
#if defined( DUBHE_FOR_RUNTIME )
    /* release command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return ( ret );
#endif
}

#if defined(DUBHE_SECURE)
static int arm_ce_enroll_dev_root_key( const uint32_t *input, size_t size )
{

    arm_ce_dev_lcs_t lcs = DBH_DEV_LCS_INVALID;

    lcs = _arm_ce_get_lcs( );
    if ( lcs == DBH_DEV_LCS_INVALID )
        return ( DBH_OTP_LCS_ERR );
    if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) )
        return ( DBH_OTP_LCS_ERR );
    if ( size > DBH_OTP_DEV_ROOT_KEY_SIZE )
        return ( DBH_OTP_OVERFLOW_ERR );

    if ( _is_region_lock(OTP_INFO_DEV_ROOT_KEY, 0, DBH_OTP_DEV_ROOT_KEY_SIZE) ) {
        return ( DBH_OTP_LOCK_ERR );
    }
    /* Write last word to trigger enrollment */
    _DBH_WRITE_OTP_INFO( DBH_OTP_DEV_ROOT_KEY_OFFSET + 0xC, size, input );
    return arm_ce_otp_wait_write_done();
}
#endif
int arm_ce_enroll_puf( void )
{
#if !defined( DUBHE_SECURE )
        return DBH_OTP_PERMISSION_DENY;
#else
    int ret = 0;
    volatile uint32_t value;
    uint32_t seed = DBH_OTP_ENROLL_SEED, origin = 0;
    uint32_t dev_root_key[DBH_OTP_DEV_ROOT_KEY_SIZE];
    arm_ce_otp_lock_t lock;

#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    if ( !DBH_OTP_PUF_READY() )
    {
        PAL_LOG_ERR("PUF not ready yet\n");
        ret = DBH_OTP_PERMISSION_DENY;
        goto exit;
    }
    /* ensure only do enrollment one time,
       otherwise duplicate enrollment might
       lead to unpredictable consequences */
    if( _is_enrollment_done( ) ) {
        PAL_LOG_INFO("already enrollmented\n");
        ret = DBH_OTP_INFO_INVALID_ERR;
        goto exit;
    }
    value = DBH_READ_REGISTER( OTP_MGR, OTP_SET );
    origin  = value;

    /* Set PTA[1:0] to 0, PAPUF to 0 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PTA, value, 0x0 );
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PAPUF, value, 0x0 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );

    /* Write the last word to trigger enrollment. */
    ret = arm_ce_enroll_dev_root_key( &seed, sizeof( uint32_t ) );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, origin );

    if ( 0 == ret ) {
        DBH_OTP_SET_DIRECT_READ();
        memset( &lock, 0x00, sizeof( lock ) );
        lock.locks.device_root_key = 1;
        _DBH_WRITE_OTP_INFO(
            DBH_OTP_LOCK_CTRL_OFFSET, DBH_OTP_LOCK_CTRL_SIZE, &lock.value );
        _DBH_READ_OTP_INFO(
            DBH_OTP_DEV_ROOT_KEY_OFFSET,
            DBH_OTP_DEV_ROOT_KEY_SIZE, dev_root_key );
        DBH_OTP_SET_SHADOW_READ();
    }else {
        PAL_LOG_ERR("arm_ce_enroll_dev_root_key failed:-0x%X\n", -ret);
    }
exit:
#if defined( DUBHE_FOR_RUNTIME )
    DBH_OTP_UNLOCK();
#endif
    return ( ret );
#endif
}

int arm_ce_otp_wakeup( void )
{
#if !defined( DUBHE_SECURE )
        return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value;
#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    value     = DBH_READ_REGISTER( OTP_MGR, OTP_SET );

    /* Set PDSTB to 1 */
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PDSTB, value, 0x1 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    /* according to ememory's EP032X32GF04DLU03_v0.5 doc there is a Tsas
         timing gap between pull up PDSTB and PRESETn  */
    pal_udelay( 100 );
    DBH_REG_FLD_SET( OTP_SET, GENERAL_CTRL_PRESETN, value, 0x1 );
    DBH_WRITE_REGISTER( OTP_MGR, OTP_SET, value );
    while (!DBH_OTP_PUF_READY())
    {
        pal_udelay(2);
    }
#if defined( DUBHE_FOR_RUNTIME )
    /* release command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return 0;
#endif
}

#endif

int arm_ce_otp_driver_init( void )
{
#if !defined( DUBHE_SECURE )
    return DBH_OTP_PERMISSION_DENY;
#else
    volatile uint32_t value = 0;
    if (DBH_OTP_INIT_DONE())
    {
        return 0;
    }

    /* clk enable */
    value = DBH_READ_REGISTER( TOP_CTRL, CLK_CTRL );
    DBH_REG_FLD_SET( CLK_CTRL, OTP_EN, value, 0x1 );
    DBH_WRITE_REGISTER( TOP_CTRL, CLK_CTRL, value );

    /* reset */
    value = DBH_READ_REGISTER( TOP_CTRL, RESET_CTRL );
    DBH_REG_FLD_SET( RESET_CTRL, OTP, value, 0x1 );
    DBH_WRITE_REGISTER( TOP_CTRL, RESET_CTRL, value );

    DBH_REG_FLD_SET( RESET_CTRL, OTP, value, 0x0 );
    DBH_WRITE_REGISTER( TOP_CTRL, RESET_CTRL, value );

#if defined( DBH_OTP_PUF_SUPPORT )
    arm_ce_otp_power_up( );
#endif
    DBH_OTP_SET_SHADOW_READ();
    arm_ce_otp_wait_init_done( );
    return 0;
#endif
}

static int _arm_ce_read_otp( arm_ce_dev_lcs_t lcs,
                            arm_ce_otp_info_t type,
                            void *output,
                            size_t size,
                            size_t offset )
{
    int ret = 0;
#if defined( DUBHE_SECURE )
    if ( !IS_SHADOW_REGISTER_REGION( type ) ) {
        if ( !DBH_OTP_PUF_READY() ) {
            PAL_LOG_DEBUG("otp not ready yet return err immediately\n");
            return DBH_OTP_RD_IL_ERR;
        }

        DBH_OTP_SET_DIRECT_READ();
    }
#endif
    switch ( type ) {
#if defined( DUBHE_SECURE )
    case OTP_INFO_SEC_BT_PK_HASH:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_SEC_BT_PK_HASH_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_SEC_BT_PK_HASH_OFFSET + ( offset ), size, output );
        break;
    case OTP_INFO_SEC_DBG_PK_HASH:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_SEC_DBG_PK_HASH_SIZE ) ) {
            ret =  ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_SEC_DBG_PK_HASH_OFFSET + offset, size, output );
        break;
    case OTP_INFO_LIFE_CYCLE:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_LCS_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO( DBH_OTP_LCS_OFFSET + offset, size, output );
        *( (uint32_t *)output ) &= DBH_LIFE_CYCLE_MASK;
        break;
    case OTP_INFO_LOCK_CTRL:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_LOCK_CTRL_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_LOCK_CTRL_OFFSET + offset, size, output );
        break;
    case OTP_INFO_USR_SEC_REGION:
        if ( lcs == DBH_DEV_LCS_DR ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_USR_SEC_REGION_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_USR_SEC_REGION_OFFSET + offset, size, output );
        break;
#else
    case OTP_INFO_SEC_BT_PK_HASH:
        if ( lcs == DBH_DEV_LCS_DD || lcs == DBH_DEV_LCS_DR ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_SEC_BT_PK_HASH_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_SEC_BT_PK_HASH_OFFSET + offset, size, output );
        break;
    case OTP_INFO_SEC_DBG_PK_HASH:
        if ( lcs == DBH_DEV_LCS_DD || lcs == DBH_DEV_LCS_DR ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_SEC_DBG_PK_HASH_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_SEC_DBG_PK_HASH_OFFSET + offset, size, output );
        break;
    case OTP_INFO_LIFE_CYCLE:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_LCS_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO( DBH_OTP_LCS_OFFSET + offset, size, output );
         *( (uint32_t *)output ) &= DBH_LIFE_CYCLE_MASK;
        break;
    case OTP_INFO_LOCK_CTRL:
        if ( lcs == DBH_DEV_LCS_DR || lcs == DBH_DEV_LCS_DD ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_LOCK_CTRL_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_LOCK_CTRL_OFFSET + offset, size, output );
        break;
    case OTP_INFO_USR_SEC_REGION:
        if ( lcs == DBH_DEV_LCS_DR || lcs == DBH_DEV_LCS_DD ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_USR_SEC_REGION_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_USR_SEC_REGION_OFFSET + offset, size, output );
        break;
#endif
    case OTP_INFO_MODEL_ID:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_MODEL_ID_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_MODEL_ID_OFFSET + offset, size, output );
        break;
    case OTP_INFO_MODEL_KEY:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret =  ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_MODEL_KEY_SIZE ) ) {
            ret =  ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_MODEL_KEY_OFFSET + offset, size, output );
        break;
    case OTP_INFO_DEV_ID:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_DEV_ID_SIZE ) ) {
            ret =  ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO( DBH_OTP_DEV_ID_OFFSET + offset, size, output );
        break;
    case OTP_INFO_DEV_ROOT_KEY:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret =  ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_DEV_ROOT_KEY_SIZE ) ) {
            ret =  ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_DEV_ROOT_KEY_OFFSET + offset, size, output );
        break;
    case OTP_INFO_USR_NON_SEC_REGION:
        if ( ACCESS_OVERFLOW( size , offset ,
                              DBH_OTP_USR_NON_SEC_REGION_SIZE ) ) {
            ret =  ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_USR_NON_SEC_REGION_OFFSET + offset, size, output );
        break;
    case OTP_INFO_TEST_REGION:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_TEST_REGION_SIZE ) ) {
            ret =  ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_READ_OTP_INFO(
            DBH_OTP_TEST_REGION_OFFSET + offset, size, output );
        break;
    default:
        ret = ( DBH_OTP_INFO_INVALID_ERR );
        break;
    }

exit:
#if defined( DUBHE_SECURE )
    if ( !IS_SHADOW_REGISTER_REGION( type ) ) {
        DBH_OTP_SET_SHADOW_READ();
    }
#endif

    return ( ret );
}

int arm_ce_get_otp_info( arm_ce_otp_info_t type,
                         void *output,
                         size_t size,
                         size_t offset )
{
    arm_ce_dev_lcs_t lcs;
    int ret = 0;
    uint8_t *p_out = (uint8_t *)output;

    if ( output == NULL )
        return ( DBH_OTP_PARAM_INVALID_ERR );
    if ( type < OTP_INFO_MODEL_ID || type > OTP_INFO_TEST_REGION )
        return ( DBH_OTP_INFO_INVALID_ERR );

#if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
    if ( type == OTP_INFO_MODEL_ID || type == OTP_INFO_DEV_ID
         || type == OTP_INFO_USR_NON_SEC_REGION
         || type == OTP_INFO_TEST_REGION )
        lcs = DBH_DEV_LCS_CM;
    else {
        lcs = _arm_ce_get_lcs( );
        if ( lcs == DBH_DEV_LCS_INVALID ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
    }
    ret = _arm_ce_read_otp( lcs, type, p_out, size, offset );
exit:
#if defined( DUBHE_FOR_RUNTIME )
    /* release command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return ret;
}

#if defined( DUBHE_SECURE )
static int arm_ce_otp_check_adv_lcs( arm_ce_dev_lcs_t lcs,
                                     uint32_t adv_value)
{
    switch ( lcs ) {
    case DBH_DEV_LCS_CM:
        if ( ( adv_value != DBH_DEV_LCS_DM )
             && ( adv_value != DBH_DEV_LCS_CM ) )
            return ( DBH_OTP_WR_IL_ERR );
        break;
    case DBH_DEV_LCS_DM:
        if ( ( adv_value != DBH_DEV_LCS_DD )
             && ( adv_value != DBH_DEV_LCS_DM ) )
            return ( DBH_OTP_WR_IL_ERR );
        break;
    case DBH_DEV_LCS_DD:
        if ( ( adv_value != DBH_DEV_LCS_DR )
             && ( adv_value != DBH_DEV_LCS_DD ) )
            return ( DBH_OTP_WR_IL_ERR );
        break;
    default:
        return ( DBH_OTP_WR_IL_ERR );
    }

    return ( 0 );
}

static inline int _arm_ce_opt_check_write_valid( arm_ce_otp_info_t type,
                                                 arm_ce_dev_lcs_t lcs,
                                                 const uint8_t *input,
                                                 size_t size,
                                                 size_t offset )
{
    uint8_t *readback_v = NULL;
    int ret = 0;

    if (0 == size) {
        return ret;
    }

    readback_v = (uint8_t *)pal_malloc(size);
    if (NULL == readback_v)
    {
        PAL_LOG_DEBUG("pal_malloc(%d) failed\n", size);
        return DBH_OTP_OVERFLOW_ERR;
    }

    memset(readback_v, 0x00, size);

    switch ( type ) {
    case OTP_INFO_MODEL_ID:
        _DBH_READ_OTP_INFO( DBH_OTP_MODEL_ID_OFFSET + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_MODEL_KEY:
        _DBH_READ_OTP_INFO( DBH_OTP_MODEL_KEY_OFFSET + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_DEV_ID:
        _DBH_READ_OTP_INFO(
            DBH_OTP_DEV_ID_OFFSET + offset, size, readback_v );
        break;
    case OTP_INFO_DEV_ROOT_KEY:
        _DBH_READ_OTP_INFO( DBH_OTP_DEV_ROOT_KEY_OFFSET + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_SEC_BT_PK_HASH:
        _DBH_READ_OTP_INFO( DBH_OTP_SEC_BT_PK_HASH_OFFSET + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_SEC_DBG_PK_HASH:
        _DBH_READ_OTP_INFO( DBH_OTP_SEC_DBG_PK_HASH_OFFSET + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_LIFE_CYCLE:
        _DBH_READ_OTP_INFO(
            DBH_OTP_LCS_OFFSET + offset, size, readback_v );
        break;
    case OTP_INFO_LOCK_CTRL:
        _DBH_READ_OTP_INFO( DBH_OTP_LOCK_CTRL_OFFSET + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_USR_NON_SEC_REGION:
        _DBH_READ_OTP_INFO( DBH_OTP_USR_NON_SEC_REGION_OFFSET
                                + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_USR_SEC_REGION:
        _DBH_READ_OTP_INFO( DBH_OTP_USR_SEC_REGION_OFFSET
                                + offset,
                            size,
                            readback_v );
        break;
    case OTP_INFO_TEST_REGION:
        _DBH_READ_OTP_INFO( DBH_OTP_TEST_REGION_OFFSET
                                + offset,
                            size,
                            readback_v );
        break;
    }

    for ( size_t i = 0; i < size; i++ ) {
        if ( ( input[i] & readback_v[i] ) != input[i] ) {
            PAL_LOG_ERR("readback mismatch offset:%d size:%d\n", offset, size);
            PAL_LOG_DUMP_BUF("input:", input, size);
            PAL_LOG_DUMP_BUF("readback:", readback_v, size);
            ret = DBH_OTP_WR_IL_ERR;
            goto exit;
        }
    }

    ret = 0;
exit:
    pal_free(readback_v);
    readback_v = NULL;
    return ret;
}

#endif

static int arm_ce_write_otp( arm_ce_otp_info_t type,
                             arm_ce_dev_lcs_t lcs,
                             const uint8_t *input,
                             size_t size,
                             size_t offset )
{
    int ret             = 0;
#if defined( DUBHE_SECURE )
    int32_t next_lcs = 0;

#endif

    /** Note:If you use the eMemory PUF OTP and need to write the OTP fields
     *  before enrollment, you need to disable the scramble of PUF OTP for
     *  HW and let macro DBH_OTP_SCRAMBLE undefined for SW.
     *  For more information, contact eMemory*/
#if defined( DBH_OTP_PUF_SUPPORT ) && defined( DBH_OTP_SCRAMBLE )
    switch (lcs) {
    case DBH_DEV_LCS_CM:
    case DBH_DEV_LCS_DM:
        /* because otp regions will be encrypted by root key before strored.
           And before enrollment it will be encrypted by default key (all zero)
           and after enrollment root key change to soemthing else, so you
           will get something else too when invoke get otp info apis. just make
           sure set something after enrollment. */
        if ( !_is_enrollment_done( ) ) {
            PAL_LOG_ERR("Pls enrollment first\n");
            return DBH_OTP_WR_INVALID_ERR;
        }
        break;

    case DBH_DEV_LCS_DD:
    case DBH_DEV_LCS_DR:
    default:
        break;
    }
#endif

    switch ( type ) {
#if defined( DUBHE_SECURE )
    case OTP_INFO_MODEL_ID:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_MODEL_ID_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_MODEL_ID, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO( DBH_OTP_MODEL_ID_OFFSET + offset, size, input );
        break;
    case OTP_INFO_MODEL_KEY:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_MODEL_KEY_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_MODEL_KEY, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO(
            DBH_OTP_MODEL_KEY_OFFSET + offset, size, input );
        break;
    case OTP_INFO_DEV_ID:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_DEV_ID_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_DEV_ID, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO( DBH_OTP_DEV_ID_OFFSET + offset, size, input );
        break;
    case OTP_INFO_DEV_ROOT_KEY:
    #if defined( DBH_OTP_PUF_SUPPORT )
        ret = DBH_OTP_WR_INVALID_ERR;
        goto exit;
    #endif
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_DEV_ROOT_KEY_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_DEV_ROOT_KEY, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO(
            DBH_OTP_DEV_ROOT_KEY_OFFSET + offset, size, input );
        break;
    case OTP_INFO_SEC_BT_PK_HASH:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_SEC_BT_PK_HASH_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_SEC_BT_PK_HASH, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO(
            DBH_OTP_SEC_BT_PK_HASH_OFFSET + offset, size, input );
        break;
    case OTP_INFO_SEC_DBG_PK_HASH:
        if ( ( lcs == DBH_DEV_LCS_DD ) || ( lcs == DBH_DEV_LCS_DR ) ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_SEC_DBG_PK_HASH_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_SEC_DBG_PK_HASH, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO(
            DBH_OTP_SEC_DBG_PK_HASH_OFFSET + offset, size, input );
        break;
    case OTP_INFO_LIFE_CYCLE:
        if ( lcs == DBH_DEV_LCS_DR ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_LCS_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        next_lcs = *(uint32_t *)input;
        ret = arm_ce_otp_check_adv_lcs( lcs, next_lcs );
        if ( ret != 0 ) {
            ret = ( ret );
            goto exit;
        }
        if ( lcs == *input ) {
            ret = 0;
            goto exit;
        }
        _DBH_WRITE_OTP_INFO( DBH_OTP_LCS_OFFSET + offset, size, input );
        break;
    case OTP_INFO_LOCK_CTRL:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_LOCK_CTRL_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO(
            DBH_OTP_LOCK_CTRL_OFFSET + offset, size, input );
        break;
    case OTP_INFO_USR_NON_SEC_REGION:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_USR_NON_SEC_REGION_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_USR_NON_SEC_REGION, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO( DBH_OTP_USR_NON_SEC_REGION_OFFSET
                                + offset,
                            size,
                            input );
        break;
    case OTP_INFO_USR_SEC_REGION:
        if ( lcs == DBH_DEV_LCS_DR ) {
            ret = ( DBH_OTP_LCS_ERR );
            goto exit;
        }
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_USR_SEC_REGION_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }

        if ( _is_region_lock(OTP_INFO_USR_SEC_REGION, offset, size) ) {
            ret = ( DBH_OTP_LOCK_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO( DBH_OTP_USR_SEC_REGION_OFFSET
                                + offset,
                            size,
                            input );
        break;
    case OTP_INFO_TEST_REGION:
        if ( ACCESS_OVERFLOW( size , offset , DBH_OTP_TEST_REGION_SIZE ) ) {
            ret = ( DBH_OTP_OVERFLOW_ERR );
            goto exit;
        }
        _DBH_WRITE_OTP_INFO( DBH_OTP_TEST_REGION_OFFSET
                                + offset,
                            size,
                            input );
        break;
#else
    case OTP_INFO_MODEL_ID:
    case OTP_INFO_MODEL_KEY:
    case OTP_INFO_DEV_ID:
    case OTP_INFO_DEV_ROOT_KEY:
    case OTP_INFO_SEC_BT_PK_HASH:
    case OTP_INFO_SEC_DBG_PK_HASH:
    case OTP_INFO_LIFE_CYCLE:
    case OTP_INFO_LOCK_CTRL:
    case OTP_INFO_USR_SEC_REGION:
    case OTP_INFO_USR_NON_SEC_REGION:
    case OTP_INFO_TEST_REGION:
        ret = ( DBH_OTP_WR_IL_ERR );
        goto exit;
#endif
    default:
        ret = ( DBH_OTP_INFO_INVALID_ERR );
        goto exit;
    }
#if defined( DUBHE_SECURE )
    ret = arm_ce_otp_wait_write_done( );

    if (ret != 0) {
        PAL_LOG_ERR("arm_ce_otp_wait_write_done failed:-0x%X\n ", ret);
        ret = DBH_OTP_WR_IL_ERR;
        goto exit;
    }
    ret = _arm_ce_opt_check_write_valid( type, lcs, input, size, offset );
#endif
exit:

    return ret;
}

int arm_ce_set_otp_info( arm_ce_otp_info_t type,
                         const void *input,
                         size_t size,
                         size_t offset )
{
    arm_ce_dev_lcs_t lcs = DBH_DEV_LCS_INVALID;
    uint8_t *p_in = (uint8_t *)input;
    int ret = 0;
    if ( input == NULL ) {
        ret = ( DBH_OTP_PARAM_INVALID_ERR );
        return ret;
    }
 #if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_LOCK();
#endif
#if defined( DUBHE_SECURE )
    if ( !DBH_OTP_PUF_READY() ) {
        PAL_LOG_ERR("otp not ready yet\n");
        ret = DBH_OTP_WR_IL_ERR;
        goto _exit;
    }
#endif
    if ( type < OTP_INFO_MODEL_ID || type > OTP_INFO_TEST_REGION ){
        ret = ( DBH_OTP_INFO_INVALID_ERR );
        goto _exit;
    }
    lcs = _arm_ce_get_lcs( );
    if ( lcs == DBH_DEV_LCS_INVALID ) {
        ret = ( DBH_OTP_LCS_ERR );
        goto _exit;
    }
    ret = ( arm_ce_write_otp( type, lcs, p_in, size, offset ) );
_exit:
 #if defined( DUBHE_FOR_RUNTIME )
    /* Get command queue lock */
    DBH_OTP_UNLOCK();
#endif
    return ret;
}

int arm_ce_otp_get_lock( arm_ce_otp_lock_t *lock )
{
    int ret = 0;

    if ( NULL == lock ) {
        ret = DBH_OTP_PARAM_INVALID_ERR;
        goto exit;
    }

    ret = arm_ce_get_otp_info(
        OTP_INFO_LOCK_CTRL, &lock->value, sizeof( arm_ce_otp_lock_t ), 0 );
exit:
    return ret;
}


int arm_ce_otp_set_lock( arm_ce_otp_lock_t *lock )
{
    int ret = 0;

    if ( NULL == lock ) {
        ret = DBH_OTP_PARAM_INVALID_ERR;
        goto exit;
    }

    ret = arm_ce_set_otp_info(
        OTP_INFO_LOCK_CTRL, &lock->value, sizeof( arm_ce_otp_lock_t ), 0 );
exit:
    return ret;
}

/*************************** The End Of File*****************************/
