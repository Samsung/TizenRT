## HOW TO use RPC test :

####  (1) <Enable RPC test config>

In menuconfig enable IPC-2.0

For rtl8726e:
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

