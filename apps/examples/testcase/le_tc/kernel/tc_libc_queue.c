/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/// @file tc_libc_queue.c

/// @brief Test Case Example for Libc Queue API

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <stdio.h>
#include <errno.h>
#include <queue.h>
#include "tc_internal.h"

#define ITEM_COUNT  4
#define KEYVAL      5
#define ARRAY_SIZE  6
#define DATAVAL6    6
#define KEY_VAL     44

struct dq_struct {
	dq_entry_t node;
	int key;
	int data_value;
};

struct sq_struct {
	sq_entry_t node;
	int key;
	int data_value;
};

struct dq_struct g_dqstruct_array[ARRAY_SIZE];
struct sq_struct g_sqstruct_array[ARRAY_SIZE];
dq_queue_t g_active_dqlist;
sq_queue_t g_active_sqlist;

/**
* @fn                   :initialize_dq_addlast
* @description          :Function to add element to dq_queue at last
* @return               :int
*/
static void initialize_dq_addlast(void)
{
	int cur_idx;
	dq_init(&g_active_dqlist);
	for (cur_idx = 0; cur_idx < ITEM_COUNT; cur_idx++) {
		g_dqstruct_array[cur_idx].key = cur_idx;
		g_dqstruct_array[cur_idx].data_value = cur_idx + 1;
		dq_addlast(&g_dqstruct_array[cur_idx].node, &g_active_dqlist);
	}
}

/**
* @fn                   :initialize_dq_addfirst
* @description          :Function to add element to dq_queue at first
* @return               :int
*/
static void initialize_dq_addfirst(void)
{
	int cur_idx;
	dq_init(&g_active_dqlist);
	for (cur_idx = 0; cur_idx < ITEM_COUNT; cur_idx++) {
		g_dqstruct_array[cur_idx].key = cur_idx;
		g_dqstruct_array[cur_idx].data_value = cur_idx + 1;
		dq_addfirst(&g_dqstruct_array[cur_idx].node, &g_active_dqlist);
	}
}

/**
* @fn                   :initialize_sq_addlast
* @description          :Function to add element to sq_queue at last
* @return               :int
*/
static void initialize_sq_addlast(void)
{
	int cur_idx;
	sq_init(&g_active_sqlist);
	for (cur_idx = 0; cur_idx < ITEM_COUNT; cur_idx++) {
		g_sqstruct_array[cur_idx].key = cur_idx;
		g_sqstruct_array[cur_idx].data_value = cur_idx + 1;
		sq_addlast(&g_sqstruct_array[cur_idx].node, &g_active_sqlist);
	}
}

/**
* @fn                   :initialize_sq_addfirst
* @description          :Function to add element to sq_queue at first
* @return               :int
*/
static void initialize_sq_addfirst(void)
{
	int cur_idx;
	sq_init(&g_active_sqlist);
	for (cur_idx = 0; cur_idx < ITEM_COUNT; cur_idx++) {
		g_sqstruct_array[cur_idx].key = cur_idx;
		g_sqstruct_array[cur_idx].data_value = cur_idx + 1;
		sq_addfirst(&g_sqstruct_array[cur_idx].node, &g_active_sqlist);
	}
}

/**
* @fn                   :tc_libc_queue_dq_addlast
* @brief                :dq_addlast API adds a 'node' to the end of 'queue'.
* @Scenario             :dqueue is initialized and filled value 0,1,2,3 using above api.
* @API'scovered         :dq_addlast()
* @Preconditions        :dqueue must be initialzed.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_addlast(void)
{
	int istart_node_val = 0;
	int cur_idx;
	dq_init(&g_active_dqlist);
	for (cur_idx = 0; cur_idx < ITEM_COUNT; cur_idx++) {
		g_dqstruct_array[cur_idx].key = cur_idx;
		g_dqstruct_array[cur_idx].data_value = cur_idx + 1;
		dq_addlast(&g_dqstruct_array[cur_idx].node, &g_active_dqlist);
	}

	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.head;
	while (conng) {
		TC_ASSERT_EQ("dq_addlast", conng->key, istart_node_val);
		TC_ASSERT_EQ("dq_addlast", conng->data_value, istart_node_val + 1);
		istart_node_val = istart_node_val + 1;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_dq_addfirst
* @brief                :dq_addfirst affs 'node' at the beginning of 'queue'.
* @Scenario             :dqueue is initialized and filled value 3,2,1,0 using above api.
* @API'scovered         :dq_addfirst()
* @Preconditions        :dqueue must be initialzed.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_addfirst(void)
{
	int istart_node_val = ITEM_COUNT - 1;
	int cur_idx;
	dq_init(&g_active_dqlist);
	for (cur_idx = 0; cur_idx < ITEM_COUNT; cur_idx++) {
		g_dqstruct_array[cur_idx].key = cur_idx;
		g_dqstruct_array[cur_idx].data_value = cur_idx + 1;
		dq_addfirst(&g_dqstruct_array[cur_idx].node, &g_active_dqlist);
	}

	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.head;
	while (conng) {
		TC_ASSERT_EQ("dq_addfirst", conng->key, istart_node_val);
		TC_ASSERT_EQ("dq_addfirst", conng->data_value, istart_node_val + 1);
		istart_node_val = istart_node_val - 1;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_dq_addafter
* @brief                :dq_addafter function adds 'node' after 'prev' in the 'queue.'.
* @Scenario             :dqueue is initialized and filled value 3,2,1,0 using dq_addfirst() api.
*                        adding a node with value 44 after a node with value 3. So updated order
*                        of node values of queue would be 3,44,2,1,0.
* @API'scovered         :dq_addafter()
* @Preconditions        :dqueue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_addafter(void)
{
	int istart_node_val = ITEM_COUNT - 1;
	int prev_key = -1;
	int added_key = KEY_VAL;
	int i_index = 0;
	initialize_dq_addfirst();
	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.head;
	/* verifying queue item inserted as expected or not. */
	while (conng) {
		i_index++;
		TC_ASSERT_EQ("dq_addfirst", conng->key, istart_node_val);
		istart_node_val = istart_node_val - 1;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}
	/* Adding key value KEY_VAL after key value 3 */
	g_dqstruct_array[i_index].key = KEY_VAL;
	g_dqstruct_array[i_index].data_value = KEY_VAL + 1;
	TC_ASSERT_GEQ("dq_addfirst", i_index, 1);
	dq_addafter(&g_dqstruct_array[i_index - 1].node, &g_dqstruct_array[i_index].node, &g_active_dqlist);
	conng = (struct dq_struct *)g_active_dqlist.head;
	/* verifying that newly queue item, inserted after key value 3 or not. */
	while (conng) {
		if (prev_key == (i_index - 1)) {
			TC_ASSERT_EQ("dq_addafter", conng->key, added_key);
			break;
		}
		prev_key = conng->key;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_dq_addbefore
* @brief                :dq_addbefore function adds 'node' before 'next' in 'queue'.
* @Scenario             :dqueue is initialized and filled value 0,1,2,3 using dq_addlast() api.
*                        adding a node with value 5 before a node with value 3. So updated order
*                        of node values of queue would be 0,1,2,5,3.
* @API'scovered         :dq_addbefore()
* @Preconditions        :dqueue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_addbefore(void)
{
	int prev_key = -1;
	int add_before_key = KEYVAL;
	int i_index = 0;
	initialize_dq_addlast();
	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.head;
	while (conng) {
		i_index++;
		conng = (struct dq_struct *)conng->node.flink;
	}
	conng = (struct dq_struct *)g_active_dqlist.head;
	/* Adding key value 5 before key value 3 */
	g_dqstruct_array[i_index].key = KEYVAL;
	g_dqstruct_array[i_index].data_value = DATAVAL6;
	dq_addbefore(&g_dqstruct_array[i_index - 1].node, &g_dqstruct_array[i_index].node, &g_active_dqlist);
	conng = (struct dq_struct *)g_active_dqlist.head;
	while (conng) {
		if (prev_key == 2) {
			TC_ASSERT_EQ("dq_addbefore", conng->key, add_before_key);
			conng = (FAR struct dq_struct *)conng->node.flink;
			TC_ASSERT_EQ("dq_addbefore", conng->key, 3);
		}
		prev_key = conng->key;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_dq_remfirst
* @brief                :dq_remfirst function removes 'node' from the head of 'queue'.
* @Scenario             :dqueue is initialized and filled value 3,2,1,0 using dq_addfirst() api.
*                        using api dq_remfirst() it will remove head node i.e 3. So updated order
*                        of node values in queue would be 2,1,0.
* @API'scovered         :dq_remfirst()
* @Preconditions        :dqueue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_remfirst(void)
{
	int arrlist[3] = { 2, 1, 0 };
	int cur_idx = 0;

	initialize_dq_addfirst();
	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.head;

	/* Return the entry from the head of the free list */
	dq_remfirst(&g_active_dqlist);
	conng = (struct dq_struct *)g_active_dqlist.head;

	while (conng) {
		/* Since first item is removed then dqueue item order would be 2,1,0 */
		TC_ASSERT_EQ("dq_remfirst", conng->key, arrlist[cur_idx]);
		cur_idx = cur_idx + 1;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_dq_remlast
* @brief                :dq_remlast function removes the last entry from 'queue'.
* @Scenario             :dqueue is initialized and filled value 3,2,1,0 using dq_addfirst() api.
*                        using api dq_remlast() it will remove tail node i.e 0. So updated order
*                        of node values in queue would be 3,2,1.
* @API'scovered         :dq_remlast()
* @Preconditions        :dqueue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_remlast(void)
{
	int ilast_node_val = 0;
	int icurrentlast_node_val = 1;
	/* dqueue item count is 4 and order would be ascending i.e. 3,2,1,0 */
	initialize_dq_addfirst();

	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.tail;
	TC_ASSERT_EQ("dq_addfirst", conng->key, ilast_node_val);

	dq_remlast(&g_active_dqlist);

	conng = (struct dq_struct *)g_active_dqlist.tail;
	TC_ASSERT_NEQ("dq_remlast", conng->key, ilast_node_val);
	TC_ASSERT_EQ("dq_remlast", conng->key, icurrentlast_node_val);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_dq_rem
* @brief                :dq_rem function removes a given 'node' from 'queue'.
* @Scenario             :dqueue is initialized and filled value 3,2,1,0 using dq_addfirst() api.
*                        Passing node 1 reference dq_rem() function, it will remove node 1. So updated order
*                        of node values in queue would be 3,2,0.
* @API'scovered         :dq_rem()
* @Preconditions        :dqueue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_dq_rem(void)
{
	int cur_idx = 0;
	int item_arr[3] = { 3, 2, 0 };

	initialize_dq_addfirst();

	struct dq_struct *conng = (struct dq_struct *)g_active_dqlist.head;

	dq_rem(&g_dqstruct_array[1].node, &g_active_dqlist);	//removes node 1 from queue, so final queue is {3,2,0}
	conng = (struct dq_struct *)g_active_dqlist.head;

	while (conng) {
		/* Since item 1 is removed then dqueue item order would be 3,2,0 */
		TC_ASSERT_EQ("dq_rem", conng->key, item_arr[cur_idx]);

		cur_idx = cur_idx + 1;
		conng = (FAR struct dq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_addlast
* @brief                :The sq_addlast function places the 'node' at the tail of the 'queue'.
* @Scenario             :squeue is initialized and filled value 0,1,2,3 using above api.
* @API'scovered         :sq_addlast()
* @Preconditions        :squeue must be initialzed.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_addlast(void)
{
	int istart_node_val = 0;

	initialize_sq_addlast();
	struct sq_struct *conng = (struct sq_struct *)g_active_sqlist.head;
	while (conng) {
		TC_ASSERT_EQ("sq_addlast", conng->key, istart_node_val);
		TC_ASSERT_EQ("sq_addlast", conng->data_value, istart_node_val + 1);

		istart_node_val = istart_node_val + 1;
		conng = (FAR struct sq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_addfirst
* @brief                :The sq_addfirst function places the 'node' at the head of the 'queue'.
* @Scenario             :squeue is initialized and filled value 3,2,1,0 using above api.
* @API'scovered         :sq_addfirst()
* @Preconditions        :squeue must be initialzed.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_addfirst(void)
{
	int istart_node_val = ITEM_COUNT - 1;

	initialize_sq_addfirst();
	struct sq_struct *conng = (struct sq_struct *)g_active_sqlist.head;
	while (conng) {
		TC_ASSERT_EQ("sq_addfirst", conng->key, istart_node_val);
		TC_ASSERT_EQ("sq_addfirst", conng->data_value, istart_node_val + 1);

		istart_node_val = istart_node_val - 1;
		conng = (FAR struct sq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_addafter
* @brief                :The sq_addafter function adds 'node' after 'prev' in the 'queue.'
* @Scenario             :squeue is initialized and filled value 3,2,1,0 using sq_addfirst() api.
*                        adding a node with value 44 after a node with value 3. So updated order
*                        of node values of queue would be 3,44,2,1,0.
* @API'scovered         :sq_addafter()
* @Preconditions        :squeue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_addafter(void)
{
	int istart_node_val = ITEM_COUNT - 1;
	int prev_key = -1;
	int addafterKey = KEY_VAL;
	int i_index = 0;

	initialize_sq_addfirst();
	struct sq_struct *conng = (struct sq_struct *)g_active_sqlist.head;
	while (conng) {
		i_index++;
		TC_ASSERT_EQ("sq_addfirst", conng->key, istart_node_val);

		istart_node_val = istart_node_val - 1;
		conng = (FAR struct sq_struct *)conng->node.flink;
	}
	/* Adding key value 44 after key value 3 */
	g_sqstruct_array[i_index].key = KEY_VAL;
	g_sqstruct_array[i_index].data_value = KEY_VAL + 1;
	sq_addafter(&g_sqstruct_array[i_index - 1].node, &g_sqstruct_array[i_index].node, &g_active_sqlist);
	conng = (struct sq_struct *)g_active_sqlist.head;
	while (conng) {
		if (prev_key == 3) {
			TC_ASSERT_EQ("sq_addafter", conng->key, addafterKey);
			break;
		}
		prev_key = conng->key;
		conng = (FAR struct sq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_remafter
* @brief                :The sq_remafter removes the entry following 'node; from the 'queue'
* @Scenario             :squeue is initialized and filled value 3,2,1,0 using sq_addfirst() api.
*                        removing item node value 2, i.e. a node with value 1 will be removed. So updated order
*                        of node values of queue would be 3,2,0.
* @API'scovered         :sq_remafter()
* @Preconditions        :squeue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_remafter(void)
{
	int cur_idx = 0;
	int arrlist[3] = { 3, 2, 0 };	/* since item 1 will be removed */

	initialize_sq_addfirst();
	struct sq_struct *conng = (struct sq_struct *)g_active_sqlist.head;

	/* removing item after key value 2 i.e. 1 */
	sq_remafter(&g_sqstruct_array[2].node, &g_active_sqlist);
	conng = (struct sq_struct *)g_active_sqlist.head;
	while (conng) {
		TC_ASSERT_EQ("sq_remafter", conng->key, arrlist[cur_idx]);
		cur_idx = cur_idx + 1;
		conng = (FAR struct sq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_remfirst
* @brief                :sq_remfirst function removes the first entry from 'queue'.
* @Scenario             :squeue is initialized and filled value 3,2,1,0 using sq_addfirst() api.
*                        using api sq_remfirst() it will remove head node i.e 3. So updated order
*                        of node values in queue would be 2,1,0.
* @API'scovered         :sq_remfirst()
* @Preconditions        :squeue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_remfirst(void)
{
	int ifirst_node_val = 3;
	int icurrentfirst_node_val = 2;
	initialize_sq_addfirst();	/* initialises queue with value {3,2,1,0} */
	struct sq_struct *conng = (struct sq_struct *)g_active_sqlist.head;

	TC_ASSERT_EQ("sq_addfirst", conng->key, ifirst_node_val);

	/* Removing first item from the queue i.e removed item is 3 */
	sq_remfirst(&g_active_sqlist);
	conng = (struct sq_struct *)g_active_sqlist.head;

	TC_ASSERT_NEQ("sq_remfirst", conng->key, ifirst_node_val);
	TC_ASSERT_EQ("sq_remfirst", conng->key, icurrentfirst_node_val);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_remlast
* @brief                :Removes the last entry in a singly-linked queue.
* @Scenario             :squeue is initialized and filled value 3,2,1,0 using sq_addfirst() api.
*                        using api sq_remlast() it will remove tail node i.e 0. So updated order
*                        of node values in queue would be 3,2,1.
* @API'scovered         :sq_remlast()
* @Preconditions        :squeue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_remlast(void)
{
	int ilast_node_value = 1;
	int ikeyVal;

	initialize_sq_addfirst();	/* initialises queue with value {3,2,1,0} */
	struct sq_struct *conng = (struct sq_struct *)g_active_sqlist.tail;
	ikeyVal = conng->key;		/* last value is 0 */
	sq_remlast(&g_active_sqlist);
	conng = (struct sq_struct *)g_active_sqlist.tail;
	/* Verifying last queue item value */

	TC_ASSERT_NEQ("sq_remlast", conng->key, ikeyVal);
	TC_ASSERT_EQ("sq_remlast", conng->key, ilast_node_value);

	TC_SUCCESS_RESULT();
}

/**
* @fn                   :tc_libc_queue_sq_rem
* @brief                :sq_rem function removes a given 'node' from 'queue'.
* @Scenario             :squeue is initialized and filled value 3,2,1,0 using sq_addfirst() api.
*                        Passing node 1 reference sq_rem() function, it will remove node 1. So updated order
*                        of node values in queue would be 3,2,0.
* @API'scovered         :sq_rem()
* @Preconditions        :squeue must be initialzed and filled with some node values.
* @Postconditions       :NA
* @return               :void
*/
static void tc_libc_queue_sq_rem(void)
{
	int cur_idx = 0;
	int item_arr[3] = { 3, 2, 0 };	/* since item 1 will be deleted */
	initialize_sq_addfirst();
	/* Since this is addfirst api so input key values must be descending order. */
	struct sq_struct *conng;
	conng = (struct sq_struct *)g_active_sqlist.head;
	sq_rem(&g_sqstruct_array[1].node, &g_active_sqlist);
	conng = (struct sq_struct *)g_active_sqlist.head;
	while (conng) {
		/* Since first item is removed then dqueue item order would be 3,2,0 */
		TC_ASSERT_EQ("sq_rem", conng->key, item_arr[cur_idx]);
		cur_idx = cur_idx + 1;
		conng = (FAR struct sq_struct *)conng->node.flink;
	}

	TC_SUCCESS_RESULT();
}

/****************************************************************************
 * Name: libc_queue
 ****************************************************************************/

int libc_queue_main(void)
{
	tc_libc_queue_dq_addlast();
	tc_libc_queue_dq_addfirst();
	tc_libc_queue_dq_addafter();
	tc_libc_queue_dq_addbefore();
	tc_libc_queue_dq_remfirst();
	tc_libc_queue_dq_remlast();
	tc_libc_queue_dq_rem();
	tc_libc_queue_sq_addlast();
	tc_libc_queue_sq_addfirst();
	tc_libc_queue_sq_addafter();
	tc_libc_queue_sq_remafter();
	tc_libc_queue_sq_remfirst();
	tc_libc_queue_sq_remlast();
	tc_libc_queue_sq_rem();

	return 0;
}
