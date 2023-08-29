## HOW TO use IPC-1.0 test :

####  (1) <Enable IPC-1.0 test config>
        MENUCONFIG FOR HP CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_1.0

        MENUCONFIG FOR AP CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_1.0

project\realtek_amebaLite_va0_example\GCC-RELEASE\project_ap\asdk\config\.config

project\realtek_amebaLite_va0_example\GCC-RELEASE\project_hp\asdk\config\.config

set CONFIG_IMQ_TEST=y

```
CONFIG_IMQ_TEST=y
```

####  (2) <Test Case>

Run test_ipc：
test_ipc
~test_ipc

