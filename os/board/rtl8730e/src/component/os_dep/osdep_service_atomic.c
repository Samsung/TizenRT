/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *
 ******************************************************************************/
#include <osdep_service.h>
#include <stdio.h>

void ATOMIC_SET(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_store(v, i);
#else
	atomic_set(v, i);
#endif
}

int ATOMIC_READ(ATOMIC_T *v)
{
#if defined(STDATOMIC)
	return atomic_load(v);
#else
	return atomic_read(v);
#endif
}

void ATOMIC_ADD(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_add(v, i);
#else
	unsigned int irq_flags = save_and_cli();
	v->counter += i;
	restore_flags(irq_flags);
#endif
}

void ATOMIC_SUB(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_sub(v, i);
#else
	unsigned int irq_flags = save_and_cli();
	v->counter -= i;
	restore_flags(irq_flags);
#endif
}

void ATOMIC_INC(ATOMIC_T *v)
{
	ATOMIC_ADD(v, 1);
}

void ATOMIC_DEC(ATOMIC_T *v)
{
	ATOMIC_SUB(v, 1);
}

int ATOMIC_ADD_RETURN(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_add(v, i);
	return atomic_load(v);
#else
	int temp;

	unsigned int irq_flags = save_and_cli();
	temp = v->counter;
	temp += i;
	v->counter = temp;
	restore_flags(irq_flags);

	return temp;
#endif
}

int ATOMIC_SUB_RETURN(ATOMIC_T *v, int i)
{
#if defined(STDATOMIC)
	atomic_fetch_sub(v, i);
	return atomic_load(v);
#else
	int temp;

	unsigned int irq_flags = save_and_cli();
	temp = v->counter;
	temp -= i;
	v->counter = temp;
	restore_flags(irq_flags);

	return temp;
#endif
}

int ATOMIC_INC_RETURN(ATOMIC_T *v)
{
	return ATOMIC_ADD_RETURN(v, 1);
}

int ATOMIC_DEC_RETURN(ATOMIC_T *v)
{
	return ATOMIC_SUB_RETURN(v, 1);
}

int ATOMIC_DEC_AND_TEST(ATOMIC_T *v)
{
	return ATOMIC_DEC_RETURN(v) == 0;
}


