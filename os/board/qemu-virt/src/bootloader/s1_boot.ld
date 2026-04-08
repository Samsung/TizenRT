/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

MEMORY
{
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 32K
    RAM (rwx) : ORIGIN = 0x40100000, LENGTH = 1K
}

ENTRY(_start)

SECTIONS
{
    .vectors :
    {
        . = ALIGN(4);
        *qemu_s1_boot_entry.o(.vectors)
    } > FLASH

    .text :
    {
        . = ALIGN(4);
        *qemu_s1_boot_entry.o(.text*)
        *qemu_s1_boot.o(.text*)
        . = ALIGN(4);
    } > FLASH

    .rodata :
    {
        . = ALIGN(4);
        *qemu_s1_boot.o(.rodata*)
        . = ALIGN(4);
    } > FLASH

    __ram_start = ORIGIN(RAM);

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *qemu_s1_boot.o(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM

    /* Define the stack region to be at the top of the RAM */
    . = ALIGN(4);
    . = ORIGIN(RAM) + LENGTH(RAM);
    __stack_top = .;
}
