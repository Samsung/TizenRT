## HOW TO use IPC-2.0 test :

####  (1) <Enable IPC-2.0 test config>
        MENUCONFIG FOR HP CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_2.0

        MENUCONFIG FOR AP CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_2.0


project\realtek_amebaLite_va0_example\GCC-RELEASE\project_ap\asdk\config\.config

project\realtek_amebaLite_va0_example\GCC-RELEASE\project_hp\asdk\config\.config

set CONFIG_IMQ2_TEST=y

```
CONFIG_IMQ2_TEST=y
```

####  (2) <Test Case>

run test_ipc_message：
test_ipc_message
~test_ipc_message

