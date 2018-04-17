-   [Preface](#preface)
-   [KASan general info](#kasan-general-info)
    -   [Shadow bytes values](#shadow-bytes-values)
    -   [Memory to shadow mapping](#memory-to-shadow-mapping)
    -   [Instrumentation examples](#instrumentation-examples)
    -   [GCC flags](#gcc-flags)
-   [TizenRT KASan support status](#tizenrt-kasan-support-status)
-   [Implementation notes](#implementation-notes)
    -   [Heap](#heap)
    -   [Allocator](#allocator)
    -   [`use-after-free` detection](#use-after-free-detection)
    -   [`double-free` detection](#double-free-detection)
    -   [Calculating average time than memory chunks spent in
        quarantine](#calculating-average-time-than-memory-chunks-spent-in-quarantine)
    -   [Stack](#stack)
    -   [`use-after-scope` detection](#use-after-scope-detection)
    -   [Global variables](#global-variables)
-   [Report](#report)
-   [Options](#options)
-   [KASan overhead](#kasan-overhead)
    -   [Heap](#heap-1)
    -   [Stack](#stack-1)
    -   [Global variables](#global-variables-1)

Preface
=======

Kernel address sanitizer is a solution suitable to catch bugs known as
"use after free", "buffer-overflow", "use of non initialized memory",
etc. This system software helps to inspect kernel code dynamically
during its execution.

The scope of issues that can be detected is much higher than in other
memory checking solutions, the performance is also a good side of KASan.

So, use of the tool can greatly help developers to fix most of mistakes
made during their projects and prepare the solutions to be integrated in
the final product.

KASan general info
==================

KASan uses compile-time instrumentation for checking memory accesses,
therefore you need a GCC version 4.9.2 or later. GCC 5.0 or later is
required for detection of "out of bounds" accesses to stack or global
variables.

KASan uses shadow memory for detect unacceptable memory accesses. Shadow
memory covers memory available to kernel and contains metainfo about it.
Every 8 bytes of normal memory described by 1 byte in shadow memory.
Thus, shadow memory takes up 1/8 of all memory available to kernel.

Shadow bytes values
-------------------

Values of shadow bytes may be following:

-   0 - all 8 bytes of memory available
-   1 &lt;= n &lt;= 7 - first n bytes of available and last 8 - n bytes
    is not
-   n &lt; 0 - all 8 bytes is unavailable and corresponding negative
    shadow value denote why it so (redzone, for example)

Memory to shadow mapping
------------------------

For each instrumented memory access, KASan checks related shadow memory
region and decide acceptable it or not. For it KASan makes memory to
shadow mapping according to following formula:

`shadow_address = (memory_address >> 3) + shadow_offset`

Instrumentation examples
------------------------

One byte access:

-   Without instrumentation

    ``` {.c}
    *addr = ...
    ```

-   With instrumentation

    ``` {.c}
    signed char shadow_value = *((addr >> 3) + shadow_offset);
    /*
    * last byte of address
    * Actually, this is offset from 8 byte boundary
    */
    signed char last_byte = addr & 0x7;

    /*
    * If offset from 8 byte boundary (last_byte) >= shadow_value, then
    * access to this address is error.
    * This is true because if shadow value contains negative value, then all 8 bytes
    * is unavailable to access.
    * On the other hand, shadow value is positive and it
    * means that first n bytes is available to access. If offset from
    * beginning of this 8 bytes chunk is greater or equal to n, then addr falls
    * outside the available memory region and this is error.
    */
    if (shadow_value && last_byte >= shadow_value) {
        kasan_error(a);
    }

    *a = ...
    ```

GCC flags
---------

GCC has following address sanitizer related flags useful for kernel:

-   `-fsanitize=kernel-address`

    Address sanitizer for OS kernel. Differs from address sanitizer for
    userspace code by avoiding linkage with asan userspace library.
    Operating system has to implement asan functions taking into account
    implementation of memory management system.

-   `--param asan-globals=1`

    Instrumentation of global variables. Together with this option,
    `-fno-common` should be used. `-fno-common` make linker to generate
    multiple-definition error if one global variable declared in several
    compilation units without `extern` keyword.

-   `--param asan-stack=1`

    Stack instrumentation. GCC 5.0 or later is needed.

-   `-fasan-shadow-offset=<shadow_offset>`

    This flag informs compiler about offset of shadow memory. Used in
    conjunction with `--param asan-stack=1` because compiler needs to
    know offset of shadow memory for generating stack poisoning and
    unpoisoning code.

-   `--param asan-instrumentation-with-call-threshold=<call_threshold>`

    If number of memory accesses in function is greater or equal to this
    `call_threshold`, then use calls to asan functions directly instead
    of inlining it.

-   `-fsanitize-address-use-after-scope`

    Poison stack variables on exit from them scope. This flag supported
    only on GCC 7.0 and later.

TizenRT KASan support status
============================

Now KASan in TizenRT implements following features:

-   checking heap accesses
-   using memory after freeing it (`use-after-free`)
-   calculating average time that memory chunk spends in quarantine
-   freeing memory chunk several times (`double-free`)
-   checking stacks accesses
-   using memory after exiting from scope (`use-after-scope`)
-   checking globals variables

Implementation notes
====================

Heap
----

KASan now supported only in flat build configuration. This configuration
suppose only one heap for kernel and all applications. Actually, kernel
and all applications links in one executable file. Thus, memory map
looks following way:

    | .data | .bss | idle thread stack | heap |
    |_______|______|___________________|______|
    | <- CONFIG_RAM_START   CONFIG_RAM_END -> |

When KASan is enabled, memory map changes not significantly:

    | .data | .bss | idle thread stack | heap | shadow memory |
    |_______|______|___________________|______|_______________|
    | <- CONFIG_RAM_START                   CONFIG_RAM_END -> |
    |                                         |
    | <-       KASAN_SHADOW_OFFSET         -> |

Shadow memory occupy 1/8 of all available memory and this happens at the
expense of the heap. Shadow offset is calculated according to
aforementioned formula:

`KASAN_SHADOW_OFFSET = SHADOW_START - (CONFIG_RAM_START >> 3)`

Allocator
---------

In flat build configuration used `mm_heap` allocator. This allocator
very simple and stores information about memory chunk right before it in
structs `mm_allocnode_s` and `mm_freenode_s`, depends on is chunk free
or alloced. Due to this structs used only by allocator itself, we decide
not to add additional redzones between chunks and mark this structs as
redzones.

Heap memory:

       | mm_allocnode_s | alloced memory chunk | mm_freenode_s | free memory chunk |
    ___|________________|______________________|_______________|___________________|___

Shadow memory reflection:

       | redzone        | available memory     | redzone       | redzone           |
    ___|________________|______________________|_______________|___________________|___

But allocator still need to be able to work with this structures. This
means that we have to remove redzone poisoning from structs in
allocator's funstions and restore poisoning on exit from allocator's
functions.

Advantages of this approach:

-   Suitable for systems with little amount of memory because redzones
    didn't occupy additional memory
-   Additional security because nobody except memory allocator cannot
    access this structures

Limitations:

-   Errors may occurs due to incorrect poisoning/unpoisoning needed
    structures
-   Performance degradation may occurs if poisoning/unpoisoning
    functions are heavy and not inlined

In out implementation functions for poisoning/unpoisoning very simple
and inlining partially.

Also, KASan requires that all memory chunks are aligned to 8 bytes
boundaries. This caused by fact that bytes in shadow memory can reflect
poisoned bytes only at the end of 8 bytes region. But we don't need to
worry about it because allocator aligns all memory chuck sizes to
`MM_MIN_CHUNK` size which is 2^4^ or 2^5^, depending on configuration.

`use-after-free` detection
--------------------------

Quarantine used for catching memory accesses to chunks that already was
freed. This is achieved by modifying `mm_free` function for placing
memory chunk to quarantine queue instead of return it to allocator's
list of free chunks. Quarantine implemented as FIFO queue and size of
queue restricted by summary size of objects in it.

Quarantine queue has two watermarks:

-   `qrnt_watermark_max`

    Restricts summary size of objects in queue. Calculated as

    `heapsize / 100 * CONFIG_KASAN_QRNT_MAX`

-   `qrnt_watermark_low`

    Defines limit for quarantine purging. When quarantine size exceeds
    `qrnt_watermark_max`, memory chunks are returning to allocator until
    quarantine size is lower than `qrnt_watermark_low`. Calculated as

    `qrnt_watermak_max / 100 * CONFIG_KASAN_QRNT_LOW`

`double-free` detection
-----------------------

Freeing of already freed memory chunk considered as error because this
may corrupt allocators internal structures and bring it in inconsisten
state. Detection of such errors made with help of quarantine: when some
memory chunk come to `kasan_qrnt_enq` and shadow value behind this
memory chunk is `KASAN_QUARANTINE`, this means that this chunk is
already in quarantine (already was freed). If we found out that this
chunk is double-freed, then print report an return.

Calculating average time than memory chunks spent in quarantine
---------------------------------------------------------------

Such statistic may be useful for selecting optimal values for quarantine
size and watermarks. Perfectly, quarantine should store memory chunks as
long as possible with minimal calls to `kasan_qrnt_purge`. This will
provide maximum average time in quarantine for chunks, which in turn
will increase possibility to catch `use-after-free` and `double-free`
errors.

Value of system ticks counter for each memory chunk is remembered during
adding chunk to quarantine. Then on removing chunk from quarantine,
global average value updated accorgin to following formula:

`new_average = old_average + (new_value - old_average) / count_of_values`

This formula allows to calucate average iteratively without saving all
values. Average time printed after every call to `kasan_qrnt_purge`.

Stack
-----

For stack, compiler places variables to 64 byte borders and poisons all
space between variables.

       | <- 64 byte border              | <- 64 byte border              |
       |                                |                                |
       |   variable_1  |     redzone    | variable_2           | redzone |
    ___|_______________|________________|______________________|_________|___

`use-after-scope` detection
---------------------------

Implementing by filling stack variables with redzone on exiting from
their scope. But this feature don't include detection of using stack
variables after returning from function. For example:

``` {.c}
int *p;

if (true) {
    int i;

    printf("begin of scope\n");

    i = 42;
    p = &i;

    printf("end of scope\n");
}

printf("p = %d\n", *p);
```

This code cause `use-after-scope` error because it try to use variable
`i` outsize of it's scope.

Global variables
----------------

Compile generates constructors for global variables and places them in
`.init_section`. For poisoning globals variables we call this
constructors during boot in `s5j_boot` function. After that, we are able
to catch overflow and underflow of global variables.

Report
======

Report give some useful information about an error such as:

-   access type (read of write)
-   access zone (heap, stack, globals)
-   accessed address
-   is address aligned
-   size of access in bytes
-   return address (address after instruction which cause an error)
-   shadow memory scope around accessed address (scope is controlled by
    `KASAN_SHADOW_SCOPE` option)
-   call stack (`ARCH_STACKDUMP` option needed)

Example:

    kasan_report: ------ KASAN REPORT ------
    kasan_report: error: read from heap
    kasan_report: addr: 0x20a5efa
    kasan_report: aligned: 0
    kasan_report: size: 1
    kasan_report: ret_ip: 0x40e4aa4
    kasan_report: shadow region (0x20dfbd7 - 0x20dfbe7):
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0x00
    kasan_report:   0x00
    kasan_report:   0x00
    kasan_report:   0x00
    kasan_report:   0x00
    kasan_report: > 0x02
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report:   0xFE
    kasan_report: stack trace:
    unwind_backtrace_with_fp: Task(pid): kasan_test(9), TaskAddr: [0x20a47d0] \
    and [Current : kasan_test]
    unwind_backtrace_with_fp: stack size 0xfdc
    unwind_backtrace_with_fp: fp:0x20a5c94 , sp:0x20a5bc8 , pc:0x40c98f8
    unwind_backtrace_with_fp: Call Stack:
    unwind_backtrace_with_fp: [<0x40c98f8>]
    unwind_backtrace_with_fp: [<0x40c9d3c>]
    unwind_backtrace_with_fp: [<0x40c9e14>]
    unwind_backtrace_with_fp: [<0x40da49c>]
    unwind_backtrace_with_fp: [<0x40e4aa4>]
    unwind_backtrace_with_fp: [<0x40e3f3c>]
    unwind_backtrace_with_fp: [<0x40e4bd8>]
    unwind_backtrace_with_fp: [<0x40ce66c>]
    unwind_frame_with_fp: End of Callstack
    print_callstack: Call stack displayed for 1 of Tasks.
    kasan_report: --------------------------

Options
=======

-   `KASAN`

    Enables KASan if architecture supports it.

-   `KASAN_FUNCS_INLINE`

    Disable inlining of KASan functions for more convenient debugging.

-   `KASAN_SHADOW_SCOPE`

    Printing scope around incorrect accessed address. Default value
    is 64.

-   `KASAN_GLOBALS`

    Check memory addresses which belongs to global variables.

-   `KASAN_STACK`

    Check memory addresses which belongs to processes stacks. Supported
    by GCC since version 5.0. For detecting `use-after-scope` errors GCC
    version greater than 7.0 is needed.

-   `KASAN_IDLESTACK`

    Check memory addresses which belongs to idle thread stack.

-   `KASAN_HEAP`

    Check memory addresses which belongs to heap memory range.

-   `KASAN_QRNT`

    Enable placing memory chunk to queue (which called quarantine)
    instead of freeing. This allow us to catch `use-after-free` errors.

-   `KASAN_ALL`

    Enable all possible checks (heap, stack, globals).

-   `KASAN_MIN_STACK`

    Minimal size of idle thread, main user and pthread stacks. Needed
    for correct working of stack memory sanitizing.

-   `KASAN_QRNT_MAX`

    Percent from heap memory size for calculating max summary size of
    objects placed in quarantine (`watermark_max`). When summary size of
    objects in quarantine exceeds `watermark_max`, quarantine purged to
    `watermark_low` size. Default value is 10.

-   `KASAN_QRNT_LOW`

    Persent for `watermark_max` size of quarantine. Purging continues
    until quarantine size greater than `watermark_low`. Default value
    is 70.

-   `KASAN_INSTR_OUTLINE`

    Before every memory access compiler insert function call
    `__asan_load*` or `__asan_store*` depending access type. These
    functions performs check of shadow memory. This is slower than
    inline instrumentation, however it doesn't bloat size of kernel's
    `.text` section so much as inline does.

-   `KASAN_INSTR_INLINE`

    Compiler directly inserts code checking shadow memory before
    memory accesses. This is faster than outline (in some workloads it
    gives about x2 boost over outline instrumentation), but make
    kernel's `.text` size much bigger. This requires a GCC version of
    5.0 or later. TizenRT `tash` configurationd builds with no problems
    with this option, but `typical` configuration failed on
    linking stage. The reasong is that `.text` section inflated by
    inline KASan's code and this seciton doesn't fit to `FLASH` region.

    ld error:

        arm-none-eabi-ld: tinyara section `.text' will not fit in region `FLASH'
        arm-none-eabi-ld: region `FLASH' overflowed by 654336 bytes
        make[1]: *** [../build/output/bin/tinyara] Error 1
        make: *** [pass2] Error 2

-   `EXAMPLES_KASAN_TEST`

    Build KASan tests.

-   `KASAN_QRNT_CALC_AVR`

    If this option is enabled, then average time spent in quarantine by
    all memory chunks will be calculated.

KASan overhead
==============

Heap
----

Kasan always occupy 1/8 of heap memory region size for shadow memory.
Besides that, KASan doesn't take any additional memory because we don't
add additionals gaps for redzones between memory chunks. Instead of that
we mark `mm_freenode_s` and `mm_allocnode_s` structs as redzones. This
structs are placed between memory chunks in heap. More details in
[Allocator](#implementationnotes_allocator).

Stack
-----

Compilator aligns stack variables to 64 bytes boundaries and mark space
between them as redzones. Overhead in this situation depends on sizes of
variables and at worst stack increases 64 times in case of one byte
variables.

Global variables
----------------

Global variables also aligned to 64 byte boundaries and redoznes placed
between them. This can lead to bloating `.data` and `.bss` sections.
