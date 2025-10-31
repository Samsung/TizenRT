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

#include "pal_log.h"
#include "pal_string.h"
#include "pal_heap.h"
#include "pal_mutex.h"
#include "pal_list.h"

#include "dubhe_sram_alloc.h"

#define INIT_VIRT_ADDR_MGR_MAGIC( _mgc )                                       \
    do {                                                                       \
        ( _mgc )[0] = 'V';                                                     \
        ( _mgc )[1] = 'A';                                                     \
        ( _mgc )[2] = 'M';                                                     \
        ( _mgc )[3] = 'G';                                                     \
    } while ( 0 )

#define IS_VIRT_ADDR_MGR_MAGIC_VALID( _mgc )                                   \
    ( ( 'V' == ( _mgc )[0] ) && ( 'A' == ( _mgc )[1] )                         \
      && ( 'M' == ( _mgc )[2] ) && ( 'G' == ( _mgc )[3] ) )

#define CLEANUP_VIRT_ADDR_MGR_MAGIC( _mgc )                                    \
    do {                                                                       \
        ( _mgc )[0] = 0;                                                       \
        ( _mgc )[1] = 0;                                                       \
        ( _mgc )[2] = 0;                                                       \
        ( _mgc )[3] = 0;                                                       \
    } while ( 0 )

#define LOCK( _mgr )                                                           \
    ( ( _mgr )->is_multi_thrd ? pal_mutex_lock( ( _mgr )->lock ) : 0 )
#define UNLOCK( _mgr )                                                         \
    ( ( _mgr )->is_multi_thrd ? pal_mutex_unlock( ( _mgr )->lock ) : 0 )

#define DBH_ROUND_UP( _val, _n ) ( ( ( _val ) + (_n) -1 ) & ( ~( (_n) -1 ) ) )
#define DBH_ROUND_DOWN( _val, _n ) ( ( _val ) & ( ~( (_n) -1 ) ) )

typedef struct __vrit_addr_node_t {
    pal_list_t node;
    void *base;
    uint32_t size;
} _virt_addr_node_t;

typedef struct __virt_addr_mgr_t {
    uint8_t magic[4];

    /* attr backup */
    uint32_t algn;
    bool is_multi_thrd;

    /* virt addr space */
    void *base;
    uint32_t size;

    /* ctrl data */
    pal_mutex_t lock; /* valid only if is_multi_thrd is true */
    pal_list_t used_list;
} _virt_addr_mgr_t;

virt_addr_mgr_t virt_addr_mgr_create_handle( void *base,
                                             uint32_t size,
                                             virt_addr_mgr_attr_t *attr )
{
    _virt_addr_mgr_t *mgr;

    if ( ( NULL == base ) || ( 0 == size ) || ( NULL == attr ) ) {
        PAL_LOG_ERR( "invalid param(s): 0x%08x, %d, 0x%08x\n",
                     (ulong_t) base,
                     size,
                     (ulong_t) attr );
        return NULL;
    }

    if ( attr->algn ) {
        /* base and size should be aligned */

        if ( (ulong_t) base & ( attr->algn - 1 ) ) {
            PAL_LOG_ERR( "base addr NOT aligned, 0x%08x\n", (ulong_t) base );
            return NULL;
        }

        if ( attr->algn && ( size & ( attr->algn - 1 ) ) ) {
            PAL_LOG_ERR( "size NOT aligned, %d\n", size );
            return NULL;
        }
    }
    mgr = pal_malloc( sizeof( _virt_addr_mgr_t ) );
    if ( mgr == NULL ) {
        PAL_LOG_ERR( "virt_addr_mgr_create_handle pal_malloc FAIL\n" );
        return NULL;
    }
    INIT_VIRT_ADDR_MGR_MAGIC( mgr->magic );
    mgr->is_multi_thrd = attr->is_multi_thrd;
    /* align again to make sure sizeof(void *) at least */
    mgr->base = (void *) DBH_ROUND_UP( (ulong_t) base, sizeof( void * ) );
    mgr->size = DBH_ROUND_DOWN( size - ( uint32_t )( mgr->base - base ),
                                sizeof( void * ) );
    mgr->algn = attr->algn;
    if ( mgr->is_multi_thrd ) {
        mgr->lock = pal_mutex_init( );
        PAL_ASSERT( NULL != mgr->lock );
    } else {
        mgr->lock = NULL;
    }
    pal_list_init_head( &mgr->used_list );

    /*PAL_LOG_DEBUG("virt addr mgr created, 0x%08x, %d\n",
                  (ulong_t)base, size);*/

    return (virt_addr_mgr_t) mgr;
}

void virt_addr_mgr_destroy_handle( virt_addr_mgr_t handle )
{
    _virt_addr_mgr_t *mgr;

    if ( handle == NULL ) {
        PAL_LOG_ERR( "virt_addr_mgr_destroy_handle invalid param(s): 0x%08x\n",
                     handle );
        return;
    }
    mgr = (_virt_addr_mgr_t *) handle;
    PAL_ASSERT( IS_VIRT_ADDR_MGR_MAGIC_VALID( mgr->magic ) );

    LOCK( mgr );

    if ( !pal_list_is_empty( &mgr->used_list ) ) {
        pal_list_t *entry, *tmp;
        PAL_LOG_WARN( "virt addr list NOT empty:\n" );
        pal_list_iter_safe( &mgr->used_list, entry, tmp )
        {
            _virt_addr_node_t *tmp =
                pal_list_entry( entry, _virt_addr_node_t, node );
            PAL_LOG_WARN( "0x%08x ~ 0x%08x\n",
                          (ulong_t) tmp->base,
                          (ulong_t) tmp->base + tmp->size );
            pal_list_del( entry );
            pal_free( tmp );
        }
    }
    if ( mgr->is_multi_thrd ) {
        PAL_ASSERT( NULL != mgr->lock );
        pal_mutex_destroy( mgr->lock );
    }
    CLEANUP_VIRT_ADDR_MGR_MAGIC( mgr->magic );
    /*PAL_LOG_DEBUG("virt addr mgr destroyed, 0x%08x, %d\n",
                  (ulong_t)mgr->base, mgr->size);*/
    pal_free( mgr );
}

void *virt_addr_mgr_alloc_vaddr( virt_addr_mgr_t handle, uint32_t size )
{
    _virt_addr_mgr_t *mgr;
    pal_list_t *entry, *next;
    _virt_addr_node_t *cur;

    if ( ( NULL == handle ) || ( 0 == size ) ) {
        PAL_LOG_ERR( "invalid param(s), 0x%08x, %d\n", (ulong_t) handle, size );
        return NULL;
    }

    mgr = (_virt_addr_mgr_t *) handle;
    if ( !IS_VIRT_ADDR_MGR_MAGIC_VALID( mgr->magic ) ) {
        PAL_LOG_ERR( "invalid magic: 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
                     mgr->magic[0],
                     mgr->magic[1],
                     mgr->magic[2],
                     mgr->magic[3] );
        return NULL;
    }

    if ( mgr->algn ) {
        if ( size & ( mgr->algn - 1 ) ) {
            PAL_LOG_ERR( "size NOT aligned, %d\n", size );
            return NULL;
        }
    } else {
        /* align to sizeof(void *) at least */
        size = DBH_ROUND_UP( size, sizeof( void * ) );
    }

    cur = pal_malloc( sizeof( _virt_addr_node_t ) );
    if ( NULL == cur ) {
        PAL_LOG_ERR( "out of mem, %d\n", sizeof( _virt_addr_node_t ) );
        return NULL;
    }

    LOCK( mgr );

    if ( pal_list_is_empty( &mgr->used_list ) ) {
        if ( size <= mgr->size ) {
            cur->base = mgr->base;
            cur->size = size;
            pal_list_add_tail( &cur->node, &mgr->used_list );
            UNLOCK( mgr );
            /*PAL_LOG_DEBUG("virt addr allocated, 0x%08x ~ 0x%08x\n",
                          (ulong_t)cur->base, (ulong_t)cur->base + size);*/
            return cur->base;
        } else {
            UNLOCK( mgr );
            pal_free( cur );
            // PAL_LOG_ERR("virt addr space NOT enough, %d\n", size);
            return NULL;
        }
    } else {
        /*
         * if list not empty, try the first empty zone
         * (between mgr->base and first->base) first.
         */

        pal_list_t *first = mgr->used_list.next;
        _virt_addr_node_t *n;

        PAL_ASSERT( NULL != first );
        n = pal_list_entry( first, _virt_addr_node_t, node );
        PAL_ASSERT( NULL != n );

        if ( ( uint32_t )( n->base - mgr->base ) >= size ) {
            cur->base = mgr->base;
            cur->size = size;
            pal_list_add( &cur->node, &mgr->used_list );
            UNLOCK( mgr );
            /*PAL_LOG_DEBUG("virt addr allocated, 0x%08x ~ 0x%08x\n",
                          (ulong_t)cur->base, (ulong_t)cur->base + size);*/
            return cur->base;
        }
    }

    /* iter the list to find an empty space */
    pal_list_iter_safe( &mgr->used_list, entry, next )
    {
        _virt_addr_node_t *e, *n;
        void *base;

        e = pal_list_entry( entry, _virt_addr_node_t, node );
        PAL_ASSERT( NULL != e );

        /* next is NOT the last node */
        if ( &mgr->used_list != next ) {
            n    = pal_list_entry( next, _virt_addr_node_t, node );
            base = n->base;
        } else {
            base = mgr->base + mgr->size;
        }

        if ( ( uint32_t )( base - ( e->base + e->size ) ) >= size ) {
            cur->base = e->base + e->size;
            cur->size = size;

            /* next is NOT the last node */
            if ( &mgr->used_list != next ) {
                /* insert to the postition between entry and next */
                cur->node.prev = entry;
                cur->node.next = next;
                entry->next    = &cur->node;
                next->prev     = &cur->node;
            } else {
                pal_list_add_tail( &cur->node, &mgr->used_list );
            }

            break;
        } else {
            /* reach the end of the list */
            if ( &mgr->used_list == next ) {
                UNLOCK( mgr );
                pal_free( cur );
                /*PAL_LOG_ERR(
                    "virt addr space NOT enough, size = 0x%08x\n", size);*/
                return NULL;
            } else {
                /* continue to check the next entry */
            }
        }
    }

    UNLOCK( mgr );

    /*PAL_LOG_DEBUG("virt addr allocated, 0x%08x ~ 0x%08x\n",
                  (ulong_t)cur->base, (ulong_t)cur->base + size);*/

    return cur->base;
}

void virt_addr_mgr_free_vaddr( virt_addr_mgr_t handle, void *base )
{
    _virt_addr_mgr_t *mgr;
    pal_list_t *entry, *next;
    _virt_addr_node_t *n;

    if ( ( NULL == handle ) || ( NULL == base ) ) {
        PAL_LOG_ERR( "invalid param(s), 0x%08x, 0x%08x\n",
                     (ulong_t) handle,
                     (ulong_t) base );
        PAL_ASSERT( 0 );
    }

    mgr = (_virt_addr_mgr_t *) handle;
    if ( !IS_VIRT_ADDR_MGR_MAGIC_VALID( mgr->magic ) ) {
        PAL_LOG_ERR( "invalid magic, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
                     mgr->magic[0],
                     mgr->magic[1],
                     mgr->magic[2],
                     mgr->magic[3] );
        return;
    }

    if ( mgr->algn ) {
        if ( (ulong_t) base & ( mgr->algn - 1 ) ) {
            PAL_LOG_ERR( "virt addr is NOT aligned, 0x%08x\n", (ulong_t) base );
            return;
        }
    } else {
        /* min algn */
        if ( (ulong_t) base & ( sizeof( void * ) - 1 ) ) {
            PAL_LOG_ERR( "virt addr is NOT aligned, 0x%08x\n", (ulong_t) base );
            return;
        }
    }

    LOCK( mgr );

    pal_list_iter_safe( &mgr->used_list, entry, next )
    {
        n = pal_list_entry( entry, _virt_addr_node_t, node );
        if ( base == n->base ) {
            n->base = NULL;
            n->size = 0;
            pal_list_del( entry );
            pal_free( n );
            UNLOCK( mgr );
            /*PAL_LOG_DEBUG("virt addr freed, 0x%08x\n", (ulong_t)base);*/
            return;
        }
    }

    UNLOCK( mgr );

    /* not found */
    PAL_LOG_ERR( "virt addr NOT found, 0x%08x\n", (ulong_t) base );
}

#if CONFIG_DBG_VIRT_ADDR_MGR && CONFIG_DBG_VERBOSE

void virt_addr_mgr_dump( virt_addr_mgr_t handle )
{
    _virt_addr_mgr_t *mgr;
    pal_list_t *e;
    _virt_addr_node_t *n;

    if ( NULL == handle ) {
        PAL_LOG_ERR( "invalid param, 0x00000000\n" );
        PAL_ASSERT( 0 );
    }

    mgr = (_virt_addr_mgr_t *) handle;
    if ( !IS_VIRT_ADDR_MGR_MAGIC_VALID( mgr->magic ) ) {
        PAL_LOG_ERR( "invalid magic, 0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
                     mgr->magic[0],
                     mgr->magic[1],
                     mgr->magic[2],
                     mgr->magic[3] );
        PAL_ASSERT( 0 );
    }

    LOCK( mgr );

    PAL_LOG_INFO( "idx         start      ~ end         len\n" );
    pal_list_iter( &mgr->used_list, e )
    {
        n = pal_list_entry( e, _virt_addr_node_t, node );
        PAL_LOG_INFO( "0x%08x: 0x%08x ~ 0x%08x, 0x%08x\n",
                      (ulong_t) n,
                      (ulong_t) n->base,
                      (ulong_t) n->base + n->size,
                      n->size );
    }

    UNLOCK( mgr );
}

#endif /* CONFIG_DBG_VIRT_ADDR_MGR && CONFIG_DBG_VERBOSE */
/*************************** The End Of File*****************************/
