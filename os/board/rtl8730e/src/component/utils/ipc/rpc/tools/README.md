## HOW TO use RPC procedure :

####  (1) <编辑 Script 档>

编辑一个 script 文件，放在script文件夹中, 注意 program id 和 version id 要和别人不同!
                   示例参考  "Test1RPC_System.idl",
                   这是 Test demo 的 在KR4和KM4通讯 所需的 funciton 和 data structure
                   在 script 前几行, 可加入一些 .insert <...>
                   在 generate 出来的 .h 文件中, 就会有这几行 #include...

```c
.insert <RPCBaseDS.h>
    struct AA {
        long sum;
        char t_char;
    }
    program ITest {
    	version VERSION {
        	int VE_RPC_RequestWriteSpace(long)=1;
        	struct AA TestFunction(char)=2;
    	}=0;
    }=0;
```

program ID 编号规则：0~100 给 RPC library 使用
                   1000~9999 给 各个模块使用

####  (2) <Run rpcgen 来产生 code>

​     目录在 component/ipc/rpc/tools/rpcgen, 平台为 Linux
​     执行 ./rpcgen Test1RPC_System.idl会产生 .h  _data.h  _svc.c _clnt.c _xdr.c 这五个文件。

####  (3) <产生文件说明>

​    svc.c 放在被呼叫的那一端 (示例是 component/ipc/rpc/tests/aidl)
​    _clnt.c 放在呼叫的那一端  (示例是 component/ipc/rpc/tests/aidl)
   .h data.h _xdr.c 这三个文件, 呼叫端和被呼叫端都需要放

####  (4) <被呼叫端: 程式实作>

​    编辑被呼叫的 function 文件 (示例是 component/ipc/rpc/tests/aidl)
​    如果当初在 .idl 内所定义的 function name 是 TEST_RPC_ToAgent_Create_0();
​    那被呼叫端相对应的 function name 就是 TEST_RPC_ToAgent_Create数字_svc();
​    可使用 (2) 步骤来产生 template 文件

```c
RPCRES_LONG *TEST_RPC_ToAgent_Create_0_svc(TEST_RPC_INSTANCE *pParam, RPC_STRUCT *pRpcStruct, RPCRES_LONG *pRes)
{
    if (p_TEST_RPC_ToAgent_Create_0_svc) {
        p_TEST_RPC_ToAgent_Create_0_svc(pParam, pRpcStruct, pRes);
        return pRes;
    } else {
        return pRes;
    }
}
```

####  (5) <呼叫端：使用方法>

​    呼叫前, 记得先使用 function:
​    struct CLNT_STRUCT clnt = prepareCLNT(mode, PROGRAM NAME, VERSION NAME);
​    其中 mode 有 BLOCK_MODE, NONBLOCK_MODE, USE_INTR_BUF, USE_POLL_BUF
​    然后再呼叫 function:
​    return参数 = TEST_RPC_ToAgent_Create_0(&自订参数, &clnt);
​    其中 clnt 是刚刚用 prepareCLNT(); 得到的结果

```c
int *result;
long arg = 123;

CLNT_STRUCT clnt = prepareCLNT(BLOCK_MODE | USE_POLL_BUF, ITest, Version);
result = TEST_RPC_ToAgent_Create_0(&arg, &clnt);

if (result) == (int *)-1)
    printf("function send error/n");
else
    printf("function send success, return value: %d /n", *result);

free(result);     // NOTE*** 必须手动 free result !!!!!!!
```

####  (6) <注册register>
在rpc_common.c 中RPC_InitThread函数中注册register。
```c
struct REG_STRUCT * rnode = NULL;

// 以下是 register function 们
struct REG_STRUCT *pReg = NULL;
pReg = TEST1_SYSTEM_0_register(pReg);
pReg = ReplyHandler_register(pReg);
RPC_InitProxy(pReg, RPC_CHANNEL_TEST1);
```

####  (7) <测试方法>

在cmd输入以下命令会在KR4和KM4上运行rpc_test：
rpc_test
@rpc_test


