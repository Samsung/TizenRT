## HOW TO use IPC-1.0 test :

####  (1) <Enable IPC-1.0 test config>

In menuconfig enable IPC-1.0

project\realtek_amebaLite_va0_example\GCC-RELEASE\project_kr4\vsdk\config\.config

project\realtek_amebaLite_va0_example\GCC-RELEASE\project_km4\asdk\config\.config

set CONFIG_IMQ_TEST=y

```
CONFIG_IMQ_TEST=y
```

####  (2) <Test Case>

Run ipc_queue_test on KR4 and KM4：
ipc_queue_test
@ipc_queue_test

