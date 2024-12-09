SECTIONS
{
  . = ORIGIN(uflash);
  _text_start_flash = ABSOLUTE(.);

  .text :
    {
      _stext_flash = . ;
      /* Place holder to keep the heap object at the top
       * 4 bytes is reserved at the top of the text segment,
       * which can be used to place user heap object, which
       * eventually used by user space memory allocator
       */
      LONG(0);
      KEEP(*(.userspace))
      
      . = ALIGN(32);

      *(.text)
      *(.text.*)
      *(.gnu.warning)
      *(.stub)
      *(.glue_7)
      *(.glue_7t)
      *(.jcr)

      /* C++ support:  The .init and .fini sections contain specific logic
       * to manage static constructors and destructors.
       */

      *(.gnu.linkonce.t.*)
      *(.init)             /* Old ABI */
      *(.fini)             /* Old ABI */
      _etext_flash = . ;
      
      *(.ARM.extab*)

      . = ALIGN(32);

      __exidx_start = .; 
      *(.ARM.exidx*)
      __exidx_end = .;

      . = ALIGN(32);

      _srodata = . ;
      *(.rodata)
      *(.rodata1)
      *(.rodata.*)
      *(.gnu.linkonce.r*)
      _erodata = . ;
      _eronly = .;

  /* C++ support. For each global and static local C++ object,
   * GCC creates a small subroutine to construct the object. Pointers
   * to these routines (not the routines themselves) are stored as
   * simple, linear arrays in the .ctors section of the object file.
   * Similarly, pointers to global/static destructor routines are
   * stored in .dtors.
   */

      . = ALIGN(32);	

      _sctors = . ;
      *(.ctors)       /* Old ABI:  Unallocated */
      *(.init_array)  /* New ABI:  Allocated */
      _ectors = . ;

      . = ALIGN(32);

      _sdtors = . ;
      *(.dtors)       /* Old ABI:  Unallocated */
      *(.fini_array)  /* New ABI:  Allocated */
      _edtors = . ;
    
      _sdata_app1 = ABSOLUTE(.);
    } > usram AT > uflash
   


   _sdata_app2 = ABSOLUTE(.);    

   .data :
    {
      _sdata_app = LOADADDR(.data);
      _sdata = . ;
      LONG(0);
      *(.curapp)
      *(.appheaptable)
      *(.data)
      *(.data1)
      *(.data.*)
      *(.gnu.linkonce.d*)
      _edata = . ;
    } > usram AT > uflash

  .bss :
    {
      _sbss = . ;
      *(.bss)
      *(.bss.*)
      *(.sbss)
      *(.sbss.*)
      *(.gnu.linkonce.b*)
      *(COMMON)
      _ebss = . ;
      . = ALIGN(32);
      _sapp_heap = ABSOLUTE(.);
    } > usram

    /* Stabs debugging sections.    */

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
