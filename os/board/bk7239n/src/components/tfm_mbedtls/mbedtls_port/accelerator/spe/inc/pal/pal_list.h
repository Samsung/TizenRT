/**
 * @if copyright_display
 *		Copyright (C), 2018-2018, Arm Technology (China) Co., Ltd.
 *		All rights reserved
 *
 *	        The content of this file or document is CONFIDENTIAL and
 *PROPRIETARY to Arm Technology (China) Co., Ltd. It is subject to the terms of
 *a License Agreement between Licensee and Arm Technology (China) Co., Ltd
 *		restricting among other things, the use, reproduction,
 *distribution and transfer.  Each of the embodiments, including this
 *information and any derivative work shall retain this copyright notice.
 * @endif
 */

#ifndef _PAL_LIST_H_
#define _PAL_LIST_H_

#include "pal.h"

typedef unsigned long ulong_t;

/* vvv list vvv */

typedef struct _pal_list_t {
    struct _pal_list_t *prev;
    struct _pal_list_t *next;
} pal_list_t;

#define pal_list_entry( _addr, _type, _mbr )                                   \
    ( {                                                                        \
        _type no_use;                                                          \
        ulong_t offset = ( ulong_t )( &no_use._mbr ) - (ulong_t) &no_use;      \
        (_type *) ( ( ulong_t )(_addr) -offset );                              \
    } )

static inline void pal_list_init_head( pal_list_t *head )
{
    head->next = head;
    head->prev = head;
}

static inline void pal_list_add( pal_list_t *entry, pal_list_t *head )
{
    entry->next      = head->next;
    entry->prev      = head;
    head->next->prev = entry;
    head->next       = entry;
}

static inline void pal_list_add_tail( pal_list_t *entry, pal_list_t *head )
{
    entry->next      = head;
    entry->prev      = head->prev;
    head->prev->next = entry;
    head->prev       = entry;
}

static inline void pal_list_del( pal_list_t *entry )
{
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->prev       = NULL;
    entry->next       = NULL;
}

static inline bool pal_list_is_empty( pal_list_t *head )
{
    if ( ( head->next == head ) && ( head->prev == head ) ) {
        return true;
    } else {
        return false;
    }
}

#define pal_list_iter( _head, _entry )                                         \
    for ( ( _entry ) = ( _head )->next; ( _entry ) != ( _head );               \
          ( _entry ) = ( _entry )->next )

#define pal_list_iter_safe( _head, _entry, _tmp )                              \
    for ( ( _entry ) = ( _head )->next, ( _tmp ) = ( _entry )->next;           \
          ( _entry ) != ( _head );                                             \
          ( _entry ) = ( _tmp ), ( _tmp ) = ( _entry )->next )

/* ^^^ list ^^^ */

#endif /* _PAL_LIST_H_ */

/*************************** The End Of File*****************************/
