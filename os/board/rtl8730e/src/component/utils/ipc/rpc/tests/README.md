## HOW TO use RPC test :

####  (1) <Enable RPC test config>

For rtl8720e:
        MENUCONFIG FOR KM4 CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_1.0

        MENUCONFIG FOR KR4 CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_1.0
For rtl8730e:
        MENUCONFIG FOR HP CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_2.0

        MENUCONFIG FOR AP CONFIG  --->
        IPC Message Queue Config  --->
        Enable CONFIG IPC Message Queue->Enable IPC Message Queue->IPC_Message_Queue_2.0


For rtl8720e:
project\realtek_amebaLite_va0_example\GCC-RELEASE\project_kr4\vsdk\config\.config
project\realtek_amebaLite_va0_example\GCC-RELEASE\project_km4\asdk\config\.config

For rtl8730e:
project\realtek_amebaLite_va0_example\GCC-RELEASE\project_ap\asdk\config\.config
project\realtek_amebaLite_va0_example\GCC-RELEASE\project_hp\asdk\config\.config

set CONFIG_RPC_TEST=y

```shell
CONFIG_RPC_TEST=y
```

In project\realtek_amebaLite_va0_example\inc\inc_km4\platform_autoconf.h define CONFIG_RPC_TEST

```c
#define CONFIG_RPC_TEST 1
```

####  (2) <Test case>

run rpc_test:
rpc_test
@rpc_test

