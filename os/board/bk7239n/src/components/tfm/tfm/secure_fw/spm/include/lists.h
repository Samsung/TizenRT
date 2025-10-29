/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef __LISTS_H__
#define __LISTS_H__

/********* Bi-directional list operations ********/
/* Bi-directional list structure */
struct bi_list_node_t {
    struct bi_list_node_t *bprev;
    struct bi_list_node_t *bnext;
};

/* Init an empty node. */
#define BI_LIST_INIT_NODE(node) do {              \
    (node)->bnext = node;                         \
    (node)->bprev = node;                         \
} while (0)

/* Insert a new node after current node: (bnext) of current. */
#define BI_LIST_INSERT_AFTER(curr, node) do {     \
    (node)->bnext = (curr)->bnext;                \
    (node)->bprev = curr;                         \
    (curr)->bnext->bprev = node;                  \
    (curr)->bnext = node;                         \
} while (0)

/* Add one node into list as the tail: (bprev) of head. */
#define BI_LIST_INSERT_BEFORE(curr, node) do {    \
    (curr)->bprev->bnext = node;                  \
    (node)->bprev = (curr)->bprev;                \
    (curr)->bprev = node;                         \
    (node)->bnext = curr;                         \
} while (0)

/* Remove one node from the list. */
#define BI_LIST_REMOVE_NODE(node) do {            \
    (node)->bprev->bnext = (node)->bnext;         \
    (node)->bnext->bprev = (node)->bprev;         \
} while (0)

/* Is the head empty? */
#define BI_LIST_IS_EMPTY(head)      ((head)->bnext == (head))

/* The node's next node. */
#define BI_LIST_NEXT_NODE(node)     ((node)->bnext)

/* Go through each node of a list. */
#define BI_LIST_FOR_EACH(node, head)              \
    for (node = (head)->bnext; node != head; node = (node)->bnext)

/********* Uni-directional list operations ********/
/* Initialize the head node. */
#define UNI_LISI_INIT_NODE(head, link) do {             \
    if ((head) != NULL) {                               \
        (head)->link = NULL;                            \
    }                                                   \
} while (0)

/* Insert a new node after given position node. */
#define UNI_LIST_INSERT_AFTER(posi, node, link) do {    \
    (node)->link = (posi)->link;                        \
    (posi)->link = node;                                \
} while (0)

/* Is list empty? */
#define UNI_LIST_IS_EMPTY(node, link)                   \
    ((node == NULL) || ((node)->link == NULL))

/* Pick the next node. */
#define UNI_LIST_NEXT_NODE(node, link) ((node)->link)

/* Remove one node from the list. */
#define UNI_LIST_REMOVE_NODE(prev, node, link) do {     \
    (prev)->link = (node)->link;                        \
    (node)->link = NULL;                                \
} while (0)

/* Remove node by its pointer ('pointer = &prev_of_node->link'). */
#define UNI_LIST_REMOVE_NODE_BY_PNODE(pnode, link)      \
    *(pnode) = (*(pnode))->link

/* Move a node after posi node. */
#define UNI_LIST_MOVE_AFTER(posi, prev, node, link) do {\
    if (prev != NULL) {                                 \
        (prev)->link = (node)->link;                    \
        (node)->link = (posi)->link;                    \
        (posi)->link = node;                            \
    }                                                   \
} while (0)

/* Go through each node of a list. */
#define UNI_LIST_FOREACH(node, head, link)              \
    for (node = (head)->link; node != NULL; node = (node)->link)

/* Go through each node of a list with prev node recorded. */
#define UNI_LIST_FOREACH_NODE_PREV(prev, node, head, link)   \
    for (prev = NULL, node = (head)->link;                   \
                 node != NULL; prev = node, node = (prev)->link)

/* Go through each node of a list with recording node and its holder. */
#define UNI_LIST_FOREACH_NODE_PNODE(pnode, node, head, link) \
    for (pnode = &(head)->link, node = (head)->link;         \
         node != NULL;                                       \
         pnode = &(node)->link, node = (node)->link)

#endif /* __LISTS_H__ */
