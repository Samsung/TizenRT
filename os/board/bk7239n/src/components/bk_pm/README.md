# BK-PM 电源管理模块文档

## 概述

BK-PM（Beken Power Management）是Beken系列芯片的电源管理模块，负责管理系统的功耗状态转换、模块电源控制和唤醒源配置。该模块通过精细化的电源管理策略，在保证系统功能完整性的前提下，最大程度降低系统功耗。

## 架构设计

### 核心组件

BK-PM模块采用分层架构设计，包含以下核心组件：

1. **电源管理核心（PM Core）**：负责睡眠状态机管理和整体功耗策略
2. **模块电源控制（Power Control）**：管理各硬件模块的电源状态
3. **唤醒源管理（Wakeup Source）**：配置和管理系统唤醒源
4. **时钟管理（Clock Management）**：控制各模块时钟频率和开关
5. **调试支持（Debug Support）**：提供电源状态调试和日志功能

### 文件结构

```
bk_pm/
├── include/              # 头文件目录
│   ├── pm.h              # 主接口定义
│   ├── pm_debug.h        # 调试接口
│   ├── pm_interface.h    # 内部接口
│   ├── pm_power.h        # 电源控制接口
│   ├── pm_sleep.h        # 睡眠管理接口
│   └── pm_wakeup_source.h # 唤醒源接口
├── src/                  # 源码目录
│   ├── core/
│   │   └── pm_core.c     # 电源管理核心实现
│   ├── services/         # 服务实现
│   │   ├── pm_power.c    # 电源控制服务
│   │   ├── pm_sleep.c    # 睡眠管理服务
│   │   ├── pm_clock.c    # 时钟管理服务
│   │   ├── pm_wakeup_source.c # 唤醒源服务
│   │   ├── pm_psram.c    # PSRAM管理服务
│   │   ├── pm_debug.c    # 调试服务
│   │   ├── pm_misc.c     # 杂项功能
│   ├── pm_client.c       # 客户端实现
|   ├── pm_mailbox.c      # 客户端与服务端通信
│   └── pm.c              # 主实现文件
├── CMakeLists.txt        # 构建配置文件
├── Kconfig               # 内核配置选项
└── README.md             # 模块文档（本文档）
```

## 版本变更

### Gerrit:69047 提交变更

本次提交对BK-PM模块进行了以下重构：

#### 文件结构调整
- **新增文件**：
  - `README.md` - 模块文档
  - `include/pm_debug.h` - 调试接口头文件
  - `include/pm_interface.h` - 内部接口头文件
  - `include/pm_power.h` - 电源控制接口头文件
  - `include/pm_sleep.h` - 睡眠管理接口头文件
  - `include/pm_types.h` - 类型定义头文件
  - `include/pm_wakeup_source.h` - 唤醒源接口头文件
  - `src/core/pm_core.c` - 电源管理核心实现
  - `src/pm.c` - 主实现文件
  - `src/pm_client.c` - 客户端实现
  - `src/pm_mailbox.c` - 客户端与服务端通信
  - `src/services/pm_clock.c` - 时钟管理服务
  - `src/services/pm_debug.c` - 调试服务
  - `src/services/pm_misc.c` - 杂项功能
  - `src/services/pm_power.c` - 电源控制服务
  - `src/services/pm_sleep.c` - 睡眠管理服务
  - `src/services/pm_wakeup_source.c` - 唤醒源服务

- **移除文件**：
  - `pm_model.c` - 未使用的代码
  - `low_vol.c` - 功能整合到主实现
  - `pm_misc.c` - 功能分散到其他文件
  - `pm.c` - 功能分散到其他文件
  上述文件均保留源文件方便检索修改和发现问题后的紧急回退，正常情况下不会进入编译

#### 代码重构
- **CMakeLists.txt**：重新组织源文件结构，采用条件编译
- **Kconfig**：保留PM_CLIENT选项，调整配置结构
- **架构调整**：采用分层架构设计，将功能模块化分离

#### 功能优化
- 保持所有现有API接口不变
- 优化代码组织结构，提高可维护性
- 新增完整模块文档和接口定义
- 实现更清晰的模块职责分离

---