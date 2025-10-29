/**
 * @if copyright_display
 *      Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
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

#include "dubhe_aca_hw.h"
#include "dubhe_aca.h"
#include "dubhe_aca_internal.h"
#include "dubhe_aca_hw_defs.h"
#include "dubhe_regs.h"
#include "pal_string.h"
#include "dubhe_event.h"
#include "dubhe_sram_alloc.h"
#include "pal_log.h"

typedef struct _aca_reg_sram_t {
    unsigned int sram_addr;
    unsigned int sram_size;
    unsigned char is_sram_alloced;  // 0:not use 1: occupied
} aca_reg_sram_t;

typedef struct _aca_tmp_reg_t {
    unsigned int reg_index;
    unsigned char flag;  // 0:not use 1: occupied
    aca_reg_sram_t reg_sram;
} aca_tmp_reg_t;

typedef struct _aca_tmp_reg_info_t {
    aca_tmp_reg_t tmp_reg[ACA_AVAIL_TMP_REG_NUM];
    unsigned int free_num;
} aca_tmp_reg_info_t;

typedef struct _len_type_info_t {
    aca_len_types_t len_type;
    unsigned int op_len_bits;
    unsigned int full_len;
    unsigned char flag;  // 0:not use 1: occupied
} len_type_info_t;

typedef struct _aca_hw_res_t {
    unsigned int sram_base_addr;
    unsigned int sram_total_size;
    len_type_info_t len_type[LEN_TYPE_MAX];
    aca_tmp_reg_info_t tmp_reg_info;
    virt_addr_mgr_t vhandle;
} aca_hw_res_t;

static aca_hw_res_t _g_aca_hw_res;

static void aca_reg_sram_free( unsigned char *buffer );
extern void
aca_dump_word_data( char *prefix, uint32_t *data, unsigned int size );
unsigned int aca_get_sram_size( unsigned char reg_index )
{
    unsigned int size = 0;
    int i;

    if ( reg_index >= ACA_AVAIL_TMP_REG_NUM ) {
        PAL_LOG_ERR( "aca_get_sram_size invalid param reg:0x%08x\n",
                     reg_index );
        return 0;
    }

    dubhe_mutex_lock( DBH_ACA_MUTEX );
    for ( i = 0 /*here skip T0/T1 and N/NP*/; i < ACA_AVAIL_TMP_REG_NUM; i++ ) {
        if ( ( _g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index]
                   .reg_sram.is_sram_alloced )
             && ( _g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index].flag ) ) {
            size = _g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index]
                       .reg_sram.sram_size;
        } else {
            size = 0;
        }
    }
    dubhe_mutex_unlock( DBH_ACA_MUTEX );

    return size;
}

unsigned int aca_get_full_len( unsigned char len_id )
{
    unsigned int len = 0;

    if ( len_id >= LEN_TYPE_MAX ) {
        PAL_LOG_ERR( "aca_get_full_len invalid param len_id:0x%08x\n", len_id );
        return 0;
    }

    dubhe_mutex_lock( DBH_ACA_MUTEX );
    if ( !_g_aca_hw_res.len_type[len_id].flag ) {
        len = 0;
        PAL_LOG_ERR( "LEN_TYPE is invalid !\n" );
    } else {
        len = _g_aca_hw_res.len_type[len_id].full_len;
    }
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
    return len;
}

int aca_get_full_len_by_opcode( unsigned char op_code,
                                unsigned int *op_len_bits,
                                unsigned int *full_len )
{
    if ( op_len_bits == NULL || full_len == NULL ) {
        return -1;
    }

    switch ( op_code ) {
    case ACA_OPCODE_ID_ADD:
        *op_len_bits = *op_len_bits + 1; /*1 extra bits for carry*/
        *full_len    = GET_FULL_OP_SIZE_32BIT_WORDS( *op_len_bits );
        break;
    case ACA_OPCODE_ID_MODMUL:
    case ACA_OPCODE_ID_MODMULNR:
    case ACA_OPCODE_ID_MODMLACC:
    case ACA_OPCODE_ID_MODMLACCNR:
    case ACA_OPCODE_ID_MODEXP:
        *full_len = GET_FULL_OP_SIZE_32BIT_WORDS( *op_len_bits + 2 );
        break;
    case ACA_OPCODE_ID_MODINV:
        /*HW support only the length bits >= 64bits*/
        if ( *op_len_bits < DBH_ACA_WORD_SIZE_IN_BITS ) {
            *op_len_bits = DBH_ACA_WORD_SIZE_IN_BITS;
        }
        /* 16 extra bit is needed for hw MODINV */
        *full_len = GET_FULL_OP_SIZE_32BIT_WORDS( *op_len_bits + 16 );
        break;
    case ACA_OPCODE_ID_SUB:
    case ACA_OPCODE_ID_MUL:
    case ACA_OPCODE_ID_DIV:
    case ACA_OPCODE_ID_AND:
    case ACA_OPCODE_ID_OR:
    case ACA_OPCODE_ID_XOR:
    case ACA_OPCODE_ID_SHR0:
    case ACA_OPCODE_ID_SHR1:
    case ACA_OPCODE_ID_SHL0:
    case ACA_OPCODE_ID_SHL1:
    case ACA_OPCODE_ID_MULHIGH:
    case ACA_OPCODE_ID_RED:
    case ACA_OPCODE_ID_MODADD:
    case ACA_OPCODE_ID_MODSUB:
        /*HW support only the length bits >= 64bits*/
        if ( *op_len_bits < DBH_ACA_WORD_SIZE_IN_BITS ) {
            *op_len_bits = DBH_ACA_WORD_SIZE_IN_BITS;
        }
        *full_len = GET_FULL_OP_SIZE_32BIT_WORDS( *op_len_bits );
        break;
    case ACA_VIRTUAL_OPCODE_LONG_DIV:
        if ( *op_len_bits < 3 * DBH_ACA_WORD_SIZE_IN_BITS ) {
            *full_len =
                2
                * GET_FULL_OP_SIZE_32BIT_WORDS( 3 * DBH_ACA_WORD_SIZE_IN_BITS );
        } else {
            *full_len = 2 * GET_FULL_OP_SIZE_32BIT_WORDS( *op_len_bits );
        }
        break;
    default:
        PAL_LOG_ERR( "==OPCODE not support!!!\n" );
        break;
    }

    return 0;
}
int aca_get_len_id( unsigned char *len_id,
                    unsigned int op_len_bits,
                    unsigned int full_len )
{
    int i = 0;

    dubhe_mutex_lock( DBH_ACA_MUTEX );

    for ( i = 0; i < LEN_TYPE_MAX; i++ ) {
        /*need to have a deep discussion, if op_len is the same but full_len is
         * different, do we need to use the same len_id?*/
        if ( ( _g_aca_hw_res.len_type[i].flag == 1 )
             && ( _g_aca_hw_res.len_type[i].op_len_bits == op_len_bits )
             && ( _g_aca_hw_res.len_type[i].full_len == full_len ) ) {
            break;
        }
    }

    if ( i == LEN_TYPE_MAX ) {
        for ( i = 0; i < LEN_TYPE_MAX; i++ ) {
            if ( _g_aca_hw_res.len_type[i].flag == 0 ) {
                _g_aca_hw_res.len_type[i].len_type    = i;
                _g_aca_hw_res.len_type[i].full_len    = full_len;
                _g_aca_hw_res.len_type[i].op_len_bits = op_len_bits;
                _g_aca_hw_res.len_type[i].flag        = 1;
                ACA_SET_REG_SIZE( op_len_bits, i );
                break;
            }
        }

        if ( i == LEN_TYPE_MAX ) {
            dubhe_mutex_unlock( DBH_ACA_MUTEX );
            return -1;
        }
    }

    *len_id = _g_aca_hw_res.len_type[i].len_type;
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
    return 0;
}

int aca_set_len_id( unsigned char len_id,
                    unsigned int op_len_bits,
                    unsigned int full_len )
{
    dubhe_mutex_lock( DBH_ACA_MUTEX );
    if ( _g_aca_hw_res.len_type[len_id].flag == 0 ) {
        _g_aca_hw_res.len_type[len_id].len_type    = len_id;
        _g_aca_hw_res.len_type[len_id].full_len    = full_len;
        _g_aca_hw_res.len_type[len_id].op_len_bits = op_len_bits;
        _g_aca_hw_res.len_type[len_id].flag        = 1;
        ACA_SET_REG_SIZE( op_len_bits, len_id );
    }
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
    return 0;
}
void aca_reset_len_id( void )
{
    int i = 0;
    dubhe_mutex_lock( DBH_ACA_MUTEX );
    pal_memset( _g_aca_hw_res.len_type, 0, sizeof( _g_aca_hw_res.len_type ) );
    for ( i = 0; i < LEN_TYPE_MAX; i++ ) {
        ACA_SET_REG_SIZE( 0, i );
    }
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
}
unsigned int aca_get_free_tmp_reg_num( void )
{
    unsigned int num;

    dubhe_mutex_lock( DBH_ACA_MUTEX );
    num = _g_aca_hw_res.tmp_reg_info.free_num;
    dubhe_mutex_unlock( DBH_ACA_MUTEX );

    return num;
}

int aca_reg_index_alloc( unsigned int *reg_index )
{
    int i, ret = -1;

    dubhe_mutex_lock( DBH_ACA_MUTEX );
    if ( _g_aca_hw_res.tmp_reg_info.free_num == 0 ) {
        ret = DBH_ACA_REG_INDEX_ALLOC_FAIL_ERR;
        goto finish;
    }

    for ( i = 0; i < ACA_AVAIL_TMP_REG_NUM; i++ ) {
        /*PAL_LOG_ERR(" %d flag:%d \
        free_num:%d\n",i,_g_aca_hw_res.tmp_reg_info.tmp_reg[i].flag,_g_aca_hw_res.tmp_reg_info.free_num);*/
        if ( _g_aca_hw_res.tmp_reg_info.tmp_reg[i].flag == 0 ) {
            _g_aca_hw_res.tmp_reg_info.tmp_reg[i].flag = 1;
            _g_aca_hw_res.tmp_reg_info.free_num--;
            _g_aca_hw_res.tmp_reg_info.tmp_reg[i].reg_index = i;
            *reg_index = _g_aca_hw_res.tmp_reg_info.tmp_reg[i].reg_index;
            ret        = 0;
            /*PAL_LOG_DEBUG(
                "aca_reg_index_alloc 111 reg:%d flag:%d\n",
               *reg_index,_g_aca_hw_res.tmp_reg_info.tmp_reg[*reg_index].flag);*/
            break;
        }
    }
finish:
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
    return ret;
}

void aca_reg_index_free( unsigned int reg_index )
{
    if ( reg_index >= ACA_AVAIL_TMP_REG_NUM ) {
        PAL_LOG_ERR( "aca_reg_index_free invalid param reg:0x%08x\n",
                     reg_index );
        return;
    }

    /*PAL_LOG_DEBUG(
        "aca_reg_index_free 111 reg:%d flag:%d\n",
       reg_index,_g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index].flag);*/
    dubhe_mutex_lock( DBH_ACA_MUTEX );

    if ( _g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index].flag ) {
        _g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index].flag      = 0;
        _g_aca_hw_res.tmp_reg_info.tmp_reg[reg_index].reg_index = 0xFF;
        _g_aca_hw_res.tmp_reg_info.free_num++;
    }
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
}

static int aca_reg_sram_alloc( unsigned char **buffer, unsigned int size )
{
    *buffer = (unsigned char *) virt_addr_mgr_alloc_vaddr(
        _g_aca_hw_res.vhandle, size );
    if ( *buffer == NULL ) {
        return -1;
    }

    return 0;
}

int aca_reg_sram_alloc_from_reg( unsigned char reg, unsigned char len_id )
{
    unsigned char *buffer_addr;
    unsigned int sram_addr, sram_size;
    int ret = 0;

    if ( reg >= ACA_AVAIL_TMP_REG_NUM || len_id >= LEN_TYPE_MAX ) {
        PAL_LOG_ERR( "aca_reg_sram_alloc_from_reg invalid param reg:0x%08x "
                     "len_id:0x%08x\n",
                     reg,
                     len_id );
        return DBH_ACA_BAD_PARAM_ERR;
    }

    dubhe_mutex_lock( DBH_ACA_MUTEX );

    if ( _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.is_sram_alloced ) {
        PAL_LOG_ERR(
            "reg:%d already alloced, the address is 0x%08x\n",
            reg,
            _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_addr );
        ret = DBH_ACA_GENERIC_ERR;
        goto finish;
    }

    sram_size = _g_aca_hw_res.len_type[len_id].full_len;  // + 2;

    if ( aca_reg_sram_alloc( &buffer_addr, sram_size ) != 0 ) {
        ret = DBH_ACA_REG_SRAM_ALLOC_FAIL_ERR;
        goto finish;
    }
    if ( (unsigned int) buffer_addr < ACA_REG_SRAM_BASE_ADDR
         || (unsigned int) buffer_addr - ACA_REG_SRAM_BASE_ADDR
                > _g_aca_hw_res.sram_total_size ) {
        ret = DBH_ACA_REG_SRAM_ALLOC_FAIL_ERR;
        goto finish;
    }
    sram_addr = (unsigned int) buffer_addr - ACA_REG_SRAM_BASE_ADDR;
    ACA_SET_REG_ADDRESS( reg, sram_addr );
    ACA_HW_CLEAR_ACA_MEM( sram_addr, sram_size );
    _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.is_sram_alloced = 1;
    _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_addr = sram_addr;
    _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_size = sram_size;
    /*PAL_LOG_DEBUG("aca_reg_sram_alloc_from_reg reg:%d address:0x%08x
       size:%d\n", reg, sram_addr, sram_size);*/

finish:
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
    return ret;
}

static void aca_reg_sram_free( unsigned char *buffer )
{
    PAL_ASSERT( buffer != NULL );

    virt_addr_mgr_free_vaddr( _g_aca_hw_res.vhandle, (void *) buffer );
}

void aca_reg_sram_free_from_reg( unsigned char reg )
{
    unsigned int sram_addr;

    if ( reg >= ACA_AVAIL_TMP_REG_NUM ) {
        PAL_LOG_ERR( "aca_reg_sram_free_from_reg invalid param reg:0x%08x\n",
                     reg );
        return;
    }
    dubhe_mutex_lock( DBH_ACA_MUTEX );

    if ( _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.is_sram_alloced ) {
        ACA_GET_REG_ADDRESS( reg, sram_addr );

        if ( sram_addr
             != _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_addr ) {
            PAL_LOG_ERR(
                "aca_reg_sram_free_from_reg ERROR! reg:%d 0x%08x != 0x%08x\n",
                reg,
                _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_addr,
                sram_addr );
            dubhe_mutex_unlock( DBH_ACA_MUTEX );
            return;
        }

        /*PAL_LOG_DEBUG("aca_reg_sram_free_from_reg reg:%d address:0x%08x\n",
                     reg,
                     sram_addr);*/
        ACA_HW_CLEAR_ACA_MEM(
            sram_addr,
            _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_size );
        aca_reg_sram_free(
            (unsigned char *) ( sram_addr + ACA_REG_SRAM_BASE_ADDR ) );
        _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_addr       = 0;
        _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.is_sram_alloced = 0;
        _g_aca_hw_res.tmp_reg_info.tmp_reg[reg].reg_sram.sram_size       = 0;
    }
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
}

#if defined( ACA_REG_DEBUG )
void aca_print_all_allocated_reg_sram_info( int index )
{
    int i = 0;
    unsigned int sram_addr;
    unsigned int sram_size;
    uint32_t buf[256];

    pal_memset( buf, 0, sizeof( buf ) );
    PAL_LOG_ERR( "\n\n{" );
    if ( index == -1 ) {
        for ( i = 0; i < ACA_AVAIL_TMP_REG_NUM; i++ ) {
            if ( _g_aca_hw_res.tmp_reg_info.tmp_reg[i]
                     .reg_sram.is_sram_alloced ) {
                ACA_GET_REG_ADDRESS( i, sram_addr );

                if ( sram_addr
                     != _g_aca_hw_res.tmp_reg_info.tmp_reg[i]
                            .reg_sram.sram_addr ) {
                    PAL_LOG_ERR( "aca_reg_sram_free_from_reg ERROR! reg:%d "
                                 "0x%08x != 0x%08x\n",
                                 i,
                                 _g_aca_hw_res.tmp_reg_info.tmp_reg[i]
                                     .reg_sram.sram_addr,
                                 sram_addr );
                    return;
                }
                sram_size =
                    _g_aca_hw_res.tmp_reg_info.tmp_reg[i].reg_sram.sram_size;
                PAL_LOG_ERR( "reg %d sram addr:%d sram_size:%d \n",
                             i,
                             sram_addr,
                             sram_size );
                aca_copy_data_from_reg( buf, sram_size, i );
                aca_dump_word_data( "reg_sram", buf, sram_size );
                pal_memset( buf, 0, sizeof( buf ) );
            }
        }
    } else {
        if ( _g_aca_hw_res.tmp_reg_info.tmp_reg[index]
                 .reg_sram.is_sram_alloced ) {
            ACA_GET_REG_ADDRESS( index, sram_addr );

            if ( sram_addr
                 != _g_aca_hw_res.tmp_reg_info.tmp_reg[index]
                        .reg_sram.sram_addr ) {
                PAL_LOG_ERR( "aca_reg_sram_free_from_reg ERROR! reg:%d 0x%08x "
                             "!= 0x%08x\n",
                             index,
                             _g_aca_hw_res.tmp_reg_info.tmp_reg[index]
                                 .reg_sram.sram_addr,
                             sram_addr );
                return;
            }
            sram_size =
                _g_aca_hw_res.tmp_reg_info.tmp_reg[index].reg_sram.sram_size;
            PAL_LOG_ERR( "reg %d sram addr:%d sram_size:%d \n",
                         index,
                         sram_addr,
                         sram_size );
            aca_copy_data_from_reg( buf, sram_size, index );
            aca_dump_word_data( "reg_sram", buf, sram_size );
            pal_memset( buf, 0, sizeof( buf ) );
        }
    }
    PAL_LOG_ERR( "}\n\n" );
}
#endif
void aca_hw_resources_cleanup( void )
{
    dubhe_mutex_lock( DBH_ACA_MUTEX );
    virt_addr_mgr_destroy_handle( _g_aca_hw_res.vhandle );
    pal_memset( &_g_aca_hw_res, 0, sizeof( _g_aca_hw_res ) );
    dubhe_mutex_unlock( DBH_ACA_MUTEX );
}

void dubhe_aca_clear_all_sram( void )
{
    unsigned int buf1[2 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS];
    unsigned int buf2[2 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS];

    pal_memset( buf1, 0, sizeof( buf1 ) );
    pal_memset( buf2, 0, sizeof( buf2 ) );
    ACA_HW_CLEAR_ACA_MEM( 0, 2 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS );
    ACA_HW_READ_BLOCK_FROM_ACA_MEM(
        0, buf1, 2 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS );
    if ( pal_memcmp( buf1, buf2, sizeof( buf1 ) ) == 0 )
        PAL_LOG_ERR( "SRAM has fully cleared\n" );
    else
        PAL_LOG_ERR( "SRAM has not fully cleared\n" );
}
int dubhe_aca_write_data_to_sram( unsigned int offset,
                                  unsigned int *buf,
                                  size_t len )
{
    if ( offset > 2 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS - len ) {
        PAL_LOG_ERR( " invalid len\n" );
        return -1;
    }
    ACA_HW_READ_BLOCK_FROM_ACA_MEM( offset, buf, len );
    return 0;
}

void aca_hw_resources_init( void )
{
    unsigned int status = 0;
    virt_addr_mgr_attr_t attr;

    PAL_LOG_DEBUG( "aca_hw_resources_init start\n" );
    _g_aca_hw_res.sram_base_addr = ACA_REG_SRAM_BASE_ADDR;
    ACA_GET_STATUS_SRAM_SIZE_MODE( status );

    PAL_LOG_DEBUG( "ACA_GET_STATUS_SRAM_SIZE_MODE status:%d\n", status );
    switch ( status ) {
    case ACA_SRAM_4K:
        _g_aca_hw_res.sram_total_size = DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS;
        break;
    case ACA_SRAM_8K:
        _g_aca_hw_res.sram_total_size =
            2 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS;
        break;
    case ACA_SRAM_16K:
        _g_aca_hw_res.sram_total_size =
            4 * DBH_ACA_MIN_SRAM_SIZE_IN_32BIT_WORDS;
        break;
    default:
        return;
    }

    attr.algn          = 0;
    attr.is_multi_thrd = 0;

    _g_aca_hw_res.vhandle =
        virt_addr_mgr_create_handle( (void *) _g_aca_hw_res.sram_base_addr,
                                     _g_aca_hw_res.sram_total_size,
                                     &attr );
    pal_memset(
        &_g_aca_hw_res.tmp_reg_info, 0, sizeof( _g_aca_hw_res.tmp_reg_info ) );
    _g_aca_hw_res.tmp_reg_info.free_num = ACA_AVAIL_TMP_REG_NUM;
    pal_memset( _g_aca_hw_res.len_type, 0, sizeof( _g_aca_hw_res.len_type ) );
}

/*************************** The End Of File*****************************/
