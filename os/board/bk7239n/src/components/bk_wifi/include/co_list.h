/**
 ****************************************************************************************
 *
 * @file co_list.h
 *
 * Copyright (C) RivieraWaves 2011-2019
 *
 * @brief Common list structures definitions
 *
 ****************************************************************************************
 */

#ifndef _CO_LIST_H_
#define _CO_LIST_H_

/**
 ****************************************************************************************
 * @defgroup CO_LIST CO_LIST
 * @ingroup COMMON
 * @brief  List management.
 *
 * This module contains the list structures and handling functions.
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
// for NULL and size_t
#include <stddef.h>

// for target dependent directives
#include "rwnx_config.h"

// for __BK_INLINE
#include "bk_compiler.h"


/*
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */
/// structure of a list element header
struct co_list_hdr
{
    /// Pointer to the next element in the list
    struct co_list_hdr *next;
};

/// structure of a list
struct co_list
{
    /// pointer to first element of the list
    struct co_list_hdr *first;
    /// pointer to the last element
    struct co_list_hdr *last;
};


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize a list to defaults values.
 * @param[in] list           Pointer to the list structure.
 ****************************************************************************************
 */
void co_list_init(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Initialize a pool to default values, and initialize the relative free list.
 *
 * @param[in] list           Pointer to the list structure
 * @param[in] pool           Pointer to the pool to be initialized
 * @param[in] elmt_size      Size of one element of the pool
 * @param[in] elmt_cnt       Nb of elements available in the pool
 * @param[in] default_value  Pointer to the default value of each element (may be NULL)
 ****************************************************************************************
 */
void co_list_pool_init(struct co_list *list,
                       void *pool,
                       size_t elmt_size,
                       uint32_t elmt_cnt,
                       void *default_value);

/**
 ****************************************************************************************
 * @brief Add an element as last on the list.
 *
 * @param[in] list           Pointer to the list structure
 * @param[in] list_hdr       Pointer to the header to add at the end of the list
 ****************************************************************************************
 */
void co_list_push_back(struct co_list *list,
                       struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Add an element as first on the list.
 *
 * @param[in] list           Pointer to the list structure
 * @param[in] list_hdr       Pointer to the header to add at the beginning of the list
 ****************************************************************************************
 */
void co_list_push_front(struct co_list *list,
                        struct co_list_hdr *list_hdr);
/**
 ****************************************************************************************
 * @brief Extract the first element of the list.
 *
 * @param[in] list           Pointer to the list structure
 *
 * @return The pointer to the element extracted, and NULL if the list is empty.
 ****************************************************************************************
 */
struct co_list_hdr *co_list_pop_front(struct co_list *list);

/**
 ****************************************************************************************
 * @brief Search for a given element in the list, and extract it if found.
 *
 * @param[in] list           Pointer to the list structure
 * @param[in] list_hdr       Pointer to the searched element
 *
 * @return CO_EMPTY if the list is empty, CO_FAIL if the element not found in the list,
 * CO_OK else.
 ****************************************************************************************
 */
void co_list_extract(struct co_list *list,
                     struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Searched a given element in the list.
 *
 * @param[in] list           Pointer to the list structure
 * @param[in] list_hdr       Pointer to the searched element
 *
 * @return true if the element is found in the list, false otherwise
 ****************************************************************************************
 */
bool co_list_find(struct co_list *list,
                  struct co_list_hdr *list_hdr);

/**
 ****************************************************************************************
 * @brief Insert an element in a sorted list.
 *
 * This primitive use a comparison function from the parameter list to select where the
 * element must be inserted.
 *
 * @param[in]  list     Pointer to the list.
 * @param[in]  element  Pointer to the element to insert.
 * @param[in]  cmp      Comparison function (return true if first element has to be inserted
 *                      before the second one).
 *
 * @return              Pointer to the element found and removed (NULL otherwise).
 ****************************************************************************************
 */
void co_list_insert(struct co_list * const list,
                    struct co_list_hdr * const element,
                    bool (*cmp)(struct co_list_hdr const *elementA,
                                struct co_list_hdr const *elementB));

/**
 ****************************************************************************************
 * @brief Insert an element in a sorted list after the provided element.
 *
 * This primitive use a comparison function from the parameter list to select where the
 * element must be inserted.
 *
 * @param[in]  list           Pointer to the list.
 * @param[in]  prev_element   Pointer to the element to find in the list
 * @param[in]  element        Pointer to the element to insert.
 *
 * If prev_element is not found, the provided element is not inserted
 ****************************************************************************************
 */
void co_list_insert_after(struct co_list * const list,
                          struct co_list_hdr * const prev_element,
                          struct co_list_hdr * const element);

/**
 ****************************************************************************************
 * @brief Insert an element in a list after the provided element.
 *
 * Same as @ref co_list_insert_after except that if @p prev_element is not NULL no check
 * is done to ensure it is part of the list.
 *
 * @param[in]  list           Pointer to the list.
 * @param[in]  prev_element   Pointer to the element after which the new element must
 *                            be added
 * @param[in]  element        Pointer to the element to insert.
 *
 ****************************************************************************************
 */
void co_list_insert_after_fast(struct co_list * const list,
                               struct co_list_hdr * const prev_element,
                               struct co_list_hdr * const element);

/**
 ****************************************************************************************
 * @brief Insert an element in a sorted list before the provided element.
 *
 * This primitive use a comparison function from the parameter list to select where the
 * element must be inserted.
 *
 * @param[in]  list           Pointer to the list.
 * @param[in]  next_element   Pointer to the element to find in the list
 * @param[in]  element        Pointer to the element to insert.
 *
 * If next_element is not found, the provided element is not inserted
 ****************************************************************************************
 */
void co_list_insert_before(struct co_list * const list,
                           struct co_list_hdr * const next_element,
                           struct co_list_hdr * const element);

/**
 ****************************************************************************************
 * @brief Concatenate two lists.
 * The resulting list is the list passed as the first parameter. The second list is
 * emptied.
 *
 * @param[in]  list1          First list (will get the result of the concatenation)
 * @param[in]  list2          Second list (will be emptied after the concatenation)
 ****************************************************************************************
 */
void co_list_concat(struct co_list *list1, struct co_list *list2);

/**
 ****************************************************************************************
 * @brief Remove the element in the list after the provided element.
 *
 * This primitive removes an element in the list. It is assume that element is part of
 * the list.
 *
 * @param[in] list          Pointer to the list.
 * @param[in] prev_element  Pointer to the previous element.
 *                          NULL if @p element is the first element in the list
 * @param[in] element       Pointer to the element to remove.
 *
 ****************************************************************************************
 */
void co_list_remove(struct co_list *list,
                    struct co_list_hdr *prev_element,
                    struct co_list_hdr *element);
/**
 ****************************************************************************************
 * @brief Test if the list is empty.
 *
 * @param[in] list           Pointer to the list structure.
 *
 * @return true if the list is empty, false else otherwise.
 ****************************************************************************************
 */
__BK_INLINE bool co_list_is_empty(const struct co_list *const list)
{
    return (list->first == NULL);
}

/**
 ****************************************************************************************
 * @brief Return the number of element of the list.
 *
 * @param[in] list           Pointer to the list structure.
 *
 * @return The number of elements in the list.
 ****************************************************************************************
 */
uint32_t co_list_cnt(const struct co_list *const list);

/**
 ****************************************************************************************
 * @brief Pick the first element from the list without removing it.
 *
 * @param[in] list           Pointer to the list structure.
 *
 * @return First element address. Returns NULL pointer if the list is empty.
 ****************************************************************************************
 */
__BK_INLINE struct co_list_hdr *co_list_pick(const struct co_list *const list)
{
    return(list->first);
}

/**
 ****************************************************************************************
 * @brief Pick the last element from the list without removing it.
 *
 * @param[in] list           Pointer to the list structure.
 *
 * @return Last element address. Returns invalid value if the list is empty.
 ****************************************************************************************
 */
__BK_INLINE struct co_list_hdr *co_list_pick_last(const struct co_list *const list)
{
    return(list->last);
}

/**
 ****************************************************************************************
 * @brief Return following element of a list element.
 *
 * @param[in] list_hdr     Pointer to the list element.
 *
 * @return The pointer to the next element.
 ****************************************************************************************
 */
__BK_INLINE struct co_list_hdr *co_list_next(const struct co_list_hdr *const list_hdr)
{
    return(list_hdr->next);
}

#if BK_MAC
/**
 * @brief iterate over list of given type
 * @param[in] pos     the type * to use as a loop counter.
 * @param[in] list    the head for your co_list list.
 * @param[in] member  the name of the co_list_hdr within the struct.
 */
#define co_list_for_each_entry(pos, list, member)                                      \
    for (pos = (__typeof__(pos))(list)->first; pos; pos = (__typeof__(pos))pos->member.next)


/**
 * @brief iterate over a list
 * @param[in] pos    the &struct co_list_hdr to use as a loop counter.
 * @param[in] list   the co_list for your list.
 */
#define co_list_for_each(pos, list) \
    for (pos = (list)->next; pos; pos = pos->next)
#endif
/// @} end of group COLIST

#endif // _CO_LIST_H_
