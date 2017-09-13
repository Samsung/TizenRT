
#ifndef _OICNODE_H_
#define _OICNODE_H_

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------

/**
 * The Node struct,
 * contains item and the pointer that point to next node.
 */
typedef struct OicNode
{
    int req;            // 0 : Message, 1 : MsgInfo
    void *item;
    struct OicNode* next;


    // struct OicNode* (*Next)(struct OicNode*);
} OicNode;


#endif