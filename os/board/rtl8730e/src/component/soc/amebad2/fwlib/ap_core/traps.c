/*
 * Copyright (c) 2020 Realtek Semiconductor Corp.	All rights reserved.
 *
 * Author: PSP Software Group
 */

#include <stdio.h>
#include "basic_types.h"

uint32_t UndefinedExceptionAddr;
uint32_t DataAbortAddr;
uint32_t PrefetchAbortAddr;

_WEAK void IRQHandler(void)
{
	for (;;);
}

_WEAK void FIQHandler(void)
{
	for (;;);
}

_WEAK void UndefinedExceptionHandler(void)
{
	printf("Address of the undefined instruction 0x%08x\n", (unsigned int)UndefinedExceptionAddr);

	for (;;);
}

_WEAK void DataAbortHandler(void)
{
	uint32_t FaultStatus;

	asm volatile("mrc	p15, 0, %0, c5, c0, 0" : "=r"(FaultStatus));

	printf("Data abort with Data Fault Status Register  0x%08x\n", (unsigned int)FaultStatus);
	printf("Address of Instruction causing Data abort 0x%08x\n", (unsigned int)DataAbortAddr);

	for (;;);
}

_WEAK void PrefetchAbortHandler(void)
{
	uint32_t FaultStatus;

	asm volatile("mrc	p15, 0, %0, c5, c0, 1" : "=r"(FaultStatus));

	printf("Prefetch abort with Instruction Fault Status Register 0x%08x\n", (unsigned int)FaultStatus);
	printf("Address of Instruction causing Data abort 0x%08x\n", (unsigned int)PrefetchAbortAddr);

	for (;;);
}

