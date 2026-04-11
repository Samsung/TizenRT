/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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

SECTIONS
{
  .text ORIGIN(uflash) :
    {
      _stext_flash = .;
      LONG(0);
      KEEP(*(.userspace))
      *(.text)
      *(.text.*)
      *(.gnu.warning)
      *(.stub)
      *(.glue_7)
      *(.glue_7t)
      *(.jcr)
      *(.gnu.linkonce.t.*)
      *(.init)
      *(.fini)
      _etext_flash = .;
    } > uflash

  .ARM.extab : {
      *(.ARM.extab*)
  } > uflash

  __fexidx_start = ABSOLUTE(.);
  .ARM.exidx : {
      __exidx_start = .;
      *(.ARM.exidx*)
      __exidx_end = .;
  } > uflash
  __fexidx_end = ABSOLUTE(.);

  .rodata :
    {
      _srodata = .;
      *(.rodata)
      *(.rodata1)
      *(.rodata.*)
      *(.gnu.linkonce.r*)
      _erodata = .;
      _eronly = .;
    } > uflash

  .ctors :
    {
      _sctors = .;
      *(SORT(.ctors.*))
      *(.ctors)
      *(SORT(.init_array.*))
      *(.init_array)
      _ectors = .;
    } > uflash

  .dtors :
    {
      _sdtors = .;
      *(SORT(.dtors.*))
      *(.dtors)
      *(SORT(.fini_array.*))
      *(.fini_array)
      _edtors = .;
    } > uflash

  _sdata_app = ABSOLUTE(.);

  .data :
    {
      _sdata = .;
      LONG(0);
      *(.curapp)
      *(.appheaptable)
      *(.data)
      *(.data1)
      *(.data.*)
      *(.gnu.linkonce.d*)
      _edata = .;
    } > usram AT > uflash

  .bss :
    {
      _sbss = .;
      *(.bss)
      *(.bss.*)
      *(.sbss)
      *(.sbss.*)
      *(.gnu.linkonce.b*)
      *(COMMON)
      _ebss = .;
      . = ALIGN(32);
      _sapp_heap = ABSOLUTE(.);
    } > usram

  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
  .debug_abbrev 0 : { *(.debug_abbrev) }
  .debug_info 0 : { *(.debug_info) }
  .debug_line 0 : { *(.debug_line) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  .debug_aranges 0 : { *(.debug_aranges) }

  _eapp_heap = ABSOLUTE(ORIGIN(usram) + LENGTH(usram));
  _sapp_ram = ABSOLUTE(ORIGIN(usram));
  _eapp_ram = ABSOLUTE(ORIGIN(usram) + LENGTH(usram));
  _eapp_flash = ABSOLUTE(ORIGIN(uflash) + LENGTH(uflash));
}
