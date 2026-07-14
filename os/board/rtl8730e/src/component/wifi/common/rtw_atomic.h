/**
  ******************************************************************************
  * @file    rtw_atomic.h
  * @author
  * @version
  * @date
  * @brief
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */


#ifndef __RTW_ATOMIC_H_
#define __RTW_ATOMIC_H_

#if defined(STDATOMIC)
//#include <stdatomic.h>
#ifndef __cplusplus
#include <stdatomic.h>
#define ATOMIC_T atomic_int
#define atomic_t atomic_int
#else
#include <atomic>
#define _Atomic(X) std::atomic< X >
#define ATOMIC_T std::atomic_int
#define atomic_t std::atomic_int
#endif

#undef atomic_read
#define atomic_read(v)  atomic_load(v)
#undef atomic_set
#define atomic_set(v,i) atomic_store(v, i)
#else

/* old implement */
typedef struct {
	volatile int counter;
} atomic_t;

#define ATOMIC_T atomic_t

//----- ------------------------------------------------------------------
// Atomic Operation
//----- ------------------------------------------------------------------

/*
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_read
#define atomic_read(v)  ((v)->counter)

/*
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.  Note that the guaranteed
 * useful range of an atomic_t is only 24 bits.
 */
#undef atomic_set
#define atomic_set(v,i) ((v)->counter = (i))
#endif

#define GFP_ATOMIC			1



/*************************** ATOMIC Integer *******************************/

/**
 * @brief  This function atomically sets the value of the variable.
 * @param[in] v: Pointer of type atomic_t that to be set value.
 * @param[in] i: Required value.
 * @return	  None
 * @note    The guaranteed useful range of an atomic_t is only 24 bits.
*/
void 	ATOMIC_SET(ATOMIC_T *v, int i);

/**
 * @brief  This function atomically reads the value of the variable.
 * @param[in] v: Pointer of type atomic_t that to be read.
 * @return	  The value of the variable.
 * @note	The guaranteed useful range of an atomic_t is only 24 bits.
*/
int		ATOMIC_READ(ATOMIC_T *v);

/**
 * @brief  This function adds "i" to the contained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to add.
 * @return	  None
*/
void 	ATOMIC_ADD(ATOMIC_T *v, int i);

/**
 * @brief  This function subtracts "i" from th econtained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to subtract.
 * @return	  None
*/
void 	ATOMIC_SUB(ATOMIC_T *v, int i);

/**
 * @brief  This function adds 1 to the contained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
void 	ATOMIC_INC(ATOMIC_T *v);

/**
 * @brief  This function subtracts 1 from th econtained "v".
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
void 	ATOMIC_DEC(ATOMIC_T *v);

/**
 * @brief  This function adds "i" to the contained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to add.
 * @return	  None
*/
int 	ATOMIC_ADD_RETURN(ATOMIC_T *v, int i);

/**
 * @brief  This function subtracts "i" from th econtained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @param[in] i: value to subtract.
 * @return	  None
*/
int 	ATOMIC_SUB_RETURN(ATOMIC_T *v, int i);

/**
 * @brief  This function adds 1 to the contained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
int 	ATOMIC_INC_RETURN(ATOMIC_T *v);

/**
 * @brief  This function subtracts 1 from th econtained "v" and returns the result.
 * @param[in] v: Pointer of type atomic_t.
 * @return	  None
*/
int 	ATOMIC_DEC_RETURN(ATOMIC_T *v);

/**
 * @brief  This function subtracts 1 from th econtained "v" and test if the result equals 0.
 * @param[in] v: Pointer of type atomic_t.
 * @return	  0: The result after subtracting 1 is 0
 * @return	 -1: The result after subtracting 1 is not 0
*/
int ATOMIC_DEC_AND_TEST(ATOMIC_T *v);
/*************************** End ATOMIC *******************************/
#endif
