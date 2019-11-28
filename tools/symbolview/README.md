# SymbolViewer

The Symbol viewer will help developer when they faced a error.
It will show some call stack information
when the application has been faced any abort.

### Prerequisites
Install python2.7, pyserial(at pip)

### How to USE

1. Enable Debug Options.
    Debug Options
      -> [*] Enable backtracking using Frame pointer register
2. Run Script
    $ python cmdautotest.py serial_port

    ex)
    $ python symbolviewer.py /dev/ttyUSB1

```
...
unwind_backtrace_with_fp: Task: [appmain], Pid: [6], TaskAddr: [0x206b7d0] and [Current : appmain]
unwind_backtrace_with_fp: stack size 0x27ec
unwind_backtrace_with_fp: Registers are Valid, We may be either Interrupt context/exception context
unwind_backtrace_with_fp: fp:0x2070bbc , sp:0x2070b90 , pc:0x412ba30
unwind_backtrace_with_fp: [<0x412ba30>]
 	: /root/tizenrt/framework/src/st_things/things_stack/framework/things_security_manager.c:702
unwind_backtrace_with_fp: [<0x412bc4c>]
 	: /root/tizenrt/framework/src/st_things/things_stack/framework/things_security_manager.c:678
unwind_backtrace_with_fp: [<0x4121e24>]
 	: /root/tizenrt/framework/src/st_things/things_stack/things_stack.c:317
unwind_backtrace_with_fp: [<0x411dc74>]
 	: /root/tizenrt/framework/src/st_things/st_things.c:372
unwind_backtrace_with_fp: [<0x40d71b8>]
 	: /root/tizenrt/apps/examples/st_things/light/sample_light_things.c:142
unwind_backtrace_with_fp: [<0x40d6dcc>]
 	: /root/tizenrt/apps/examples/st_things/st_things_sample_main.c:35
unwind_backtrace_with_fp: [<0x40cc990>]
 	: /root/tizenrt/os/kernel/task/task_start.c:180
unwind_frame_with_fp: End of Callstack
```