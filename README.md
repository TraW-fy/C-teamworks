# C++ 智能记账本项目文件说明文档

> 文档版本: 1.0
> 创建日期: 2025-12-29
> 项目路径: `d:\C++project\project\withAI2\C++teamwork\`

---

## 一、项目概述

### 1.1 项目简介

这是一个功能完整的 C++ 财务管理应用，提供收支记录、统计分析、预算管理和 AI 智能分析等功能。项目采用模块化设计，使用 C++11 标准开发，依赖 MinGW-w64 编译器编译运行。

### 1.2 技术栈

- **编程语言**: C++11
- **编译器**: MinGW-w64 (GCC 12.2)
- **构建工具**: g++ (GCC)
- **依赖库**: WinHTTP (Windows HTTP API)
- **第三方库**: nlohmann/json (JSON解析)
- **配置文件格式**: JSON、TXT

---

## 二、目录结构总览

```
d:\C++project\project\withAI2\
│
├── withAI2.sln                    # Visual Studio 解决方案文件（如有）
├── withAI2.vcxproj               # Visual Studio 项目文件（如有）
│
├── .vscode/                      # VS Code 工作区配置
│   ├── launch.json               # 调试启动配置
│   └── tasks.json                # 构建任务配置
│
└── C++teamwork/                  # 主项目目录
    │
    ├── .vscode/                 # VS Code 项目配置
    │   ├── c_cpp_properties.json # C++ 语言服务器配置
    │   ├── launch.json           # 调试配置
    │   └── settings.json         # 编辑器设置
    │
    ├── src/                     # 源代码目录（部分项目使用）
    │   ├── main.cpp
    │   ├── *.cpp
    │   └── *.h
    │
    ├── include/                 # 头文件目录（部分项目使用）
    │   └── *.h
    │
    ├── tests/                   # 测试目录（部分项目使用）
    │   └── *.cpp
    │
    ├── docs/                    # 文档目录（部分项目使用）
    │   └── *.md
    │
    ├── cmake/                   # CMake 脚本目录（部分项目使用）
    │   └── *.cmake
    │
    ├── .github/                 # GitHub CI 配置（部分项目使用）
    │   └── workflows/
    │
    ├── RUNNING.md               # 项目运行说明
    ├── README.md                # 项目自述文件
    ├── CHANGELOG.md             # 更新日志
    ├── LICENSE                  # 开源协议
    │
    ├── *.cpp                    # 源文件（核心模块）
    ├── *.h                      # 头文件（核心模块）
    ├── *.hpp                    # 第三方库头文件
    │
    ├── config.txt               # 运行时配置文件
    ├── config_example.txt       # 配置示例文件
    │
    ├── records.dat              # 数据存储文件
    ├── data.json                # JSON 格式数据（部分项目使用）
    │
    ├── *.exe                    # 编译生成的可执行文件
    ├── *.dll                    # 动态链接库（部分项目使用）
    │
    └── *.md                     # Markdown 文档
```

---

## 三、核心源文件详解

### 3.1 main.cpp（主程序入口）

**文件路径**: `C++teamwork/main.cpp`

**文件大小**: 约 19KB

**功能描述**: 程序主入口文件，包含用户交互界面和主控制循环

**核心函数**:
- `showMenu()` - 显示主菜单界面
- `addRecord()` - 添加收支记录
- `viewRecords()` - 查看所有记录
- `editRecord()` - 编辑或删除记录
- `searchRecords()` - 搜索记录
- `showStatistics()` - 显示统计分析菜单
- `manageBudget()` - 管理预算
- `saveRecords()` - 保存记录
- `importRecords()` - 导入TXT记录

**主菜单选项**:
```
1.  添加收支记录
2.  查看所有记录
3.  编辑/删除记录
4.  搜索记录
5.  统计分析
6.  管理预算
7.  保存记录
8.  导入TXT记录
0.  退出系统
```

**依赖关系**:
```
main.cpp
├── finance_manager.h
├── ai_analysis.h
├── iostream
├── vector
├── string
├── iomanip
└── algorithm
```

---

### 3.2 finance_manager.h（核心头文件）

**文件路径**: `C++teamwork/finance_manager.h`

**文件大小**: 约 5KB

**功能描述**: 定义项目核心数据结构、枚举类型和函数声明

**核心内容**:

#### 枚举类型

```cpp
// 收支类型枚举
enum class RecordType {
    INCOME,   // 收入
    EXPENSE   // 支出
};

// 分类枚举
enum class Category {
    // 支出分类
    FOOD,           // 餐饮
    STUDY,          // 学习
    ENTERTAINMENT,  // 娱乐
    TRANSPORT,      // 交通
    
    // 收入分类
    LIVING_ALLOWANCE, // 生活费
    PART_TIME         // 兼职
};
```

#### 结构体定义

```cpp
// 收支记录结构体
struct Record {
    int id;                   // 记录唯一标识
    RecordType type;          // 收支类型
    Category category;        // 分类
    double amount;            // 金额
    std::string date;         // 日期 (YYYY-MM-DD)
    std::string description;  // 备注描述
};

// 预算设置结构体
struct Budget {
    double totalBudget;           // 总预算
    double foodBudget;            // 餐饮预算
    double studyBudget;           // 学习预算
    double entertainmentBudget;   // 娱乐预算
    double transportBudget;       // 交通预算
};
```

#### 函数声明

工具函数:
- `setConsoleUTF8()` - 设置控制台UTF-8编码
- `getCurrentDate()` - 获取当前日期
- `isValidDate()` - 验证日期格式
- `categoryToString()` - 分类转字符串
- `stringToCategory()` - 字符串转分类
- `stringToRecordType()` - 字符串转收支类型
- `recordTypeToString()` - 收支类型转字符串

分类函数:
- `autoClassify()` - 根据描述自动分类

---

### 3.3 ai_analysis.cpp / ai_analysis.h（AI智能分析模块）

**文件路径**: 
- `C++teamwork/ai_analysis.cpp`
- `C++teamwork/ai_analysis.h`

**文件大小**:
- ai_analysis.cpp: 约 18KB
- ai_analysis.h: 约 1.5KB

**功能描述**: 提供AI智能财务分析功能，调用讯飞Spark API进行分析

**核心功能**:
- 财务健康度评估
- 支出异常检测
- 消费习惯分析
- 个性化理财建议
- 预算执行情况分析

**API配置**:
- 服务商: 讯飞Spark（星火认知大模型）
- 配置文件: config.txt
- 配置项: SPARK_API_KEY

**依赖**:
- WinHTTP库（Windows HTTP API）
- JSON解析库（nlohmann/json）

---

### 3.4 budget.cpp（预算管理模块）

**文件路径**: `C++teamwork/budget.cpp`

**文件大小**: 约 5KB

**功能描述**: 管理预算设置和预算预警功能

**核心功能**:
- 设置总预算和各分类预算
- 查询当前预算状态
- 预算超支预警
- 预算执行进度显示

**配置存储**:
- 文件: config.txt
- 格式: JSON
- 示例配置:
```json
{
    "monthly_budget": 5000.00,
    "food_budget": 1500.00,
    "study_budget": 500.00,
    "entertainment_budget": 800.00,
    "transport_budget": 300.00
}
```

---

### 3.5 statistics.cpp（统计分析模块）

**文件路径**: `C++teamwork/statistics.cpp`

**文件大小**: 约 5KB

**功能描述**: 提供多维度的收支统计分析功能

**统计维度**:
1. **按日统计** - 每日收支汇总
2. **按周统计** - 每周收支汇总
3. **按月统计** - 每月收支汇总
4. **消费分布** - 按分类统计消费占比
5. **预算预警** - 检查是否超预算
6. **AI智能分析** - AI驱动的深度分析

**输出格式**:
- 表格形式展示
- 支持金额汇总
- 支持分类占比计算

---

### 3.6 classification.cpp（自动分类模块）

**文件路径**: `C++teamwork/classification.cpp`

**文件大小**: 约 3KB

**功能描述**: 根据收支描述自动进行分类

**分类规则**:
- 餐饮类: 吃饭、外卖、餐厅、零食等
- 学习类: 书籍、课程、学费、文具等
- 娱乐类: 游戏、电影、音乐、旅游等
- 交通类: 地铁、公交、出租车、机票等
- 生活费: 父母转账、生活费等
- 兼职: 工作收入、外快等

**核心函数**:
- `autoClassify(const std::string& description, RecordType type)`

---

### 3.7 file_io.cpp（文件读写模块）

**文件路径**: `C++teamwork/file_io.cpp`

**文件大小**: 约 4KB

**功能描述**: 处理数据的持久化存储和读取

**存储文件**:
- `records.dat` - 二进制格式存储收支记录
- `config.txt` - JSON格式存储配置

**核心功能**:
- `saveRecords()` - 保存记录到dat文件
- `loadRecords()` - 从dat文件加载记录
- `exportToJSON()` - 导出为JSON格式
- `importFromTXT()` - 从TXT文件导入
- `saveConfig()` - 保存配置
- `loadConfig()` - 加载配置

**数据格式**:
```cpp
// records.dat 二进制格式
struct DataHeader {
    int version;           // 数据版本
    int recordCount;       // 记录数量
    time_t timestamp;      // 保存时间
};

struct Record {
    int id;
    RecordType type;
    Category category;
    double amount;
    char date[11];         // YYYY-MM-DD
    char description[256]; // 备注
};
```

---

### 3.8 utils.cpp（工具函数模块）

**文件路径**: `C++teamwork/utils.cpp`

**文件大小**: 约 3KB

**功能描述**: 提供通用工具函数

**核心函数**:
- `setConsoleUTF8()` - 设置控制台编码为UTF-8（Windows）
- `getCurrentDate()` - 获取当前系统日期
- `isValidDate()` - 验证日期格式有效性
- `categoryToString()` - 将Category枚举转为字符串
- `stringToCategory()` - 将字符串转为Category枚举
- `stringToRecordType()` - 将字符串转为RecordType枚举
- `recordTypeToString()` - 将RecordType枚举转为字符串

**平台适配**:
```cpp
#ifdef _WIN32
#include <windows.h>
// Windows特定实现
#endif
```

---

## 四、第三方依赖

### 4.1 json.hpp

**文件路径**: `C++teamwork/json.hpp`

**文件大小**: 约 953KB

**来源**: [nlohmann/json](https://github.com/nlohmann/json)

**版本**: 单文件版（header-only）

**功能**: 提供JSON格式的解析和生成能力

**使用示例**:
```cpp
#include "json.hpp"

using json = nlohmann::json;

// 解析JSON
json config = json::parse(config_file_content);

// 生成JSON
json output = {
    {"total_income", totalIncome},
    {"total_expense", totalExpense},
    {"balance", balance}
};
```

**许可**: MIT License

---

## 五、配置文件说明

### 5.1 config.txt

**文件路径**: `C++teamwork/config.txt`

**内容**:
```properties
# 讯飞Spark Lite API配置
SPARK_API_KEY=RdIgkNZQojPbzZOzxQkR:hLZFsqebrjBYiYLpoARr
```

**说明**: 运行时配置文件，包含AI分析的API密钥

**注意**: 此文件包含敏感信息，不应提交到版本控制

---

### 5.2 config_example.txt

**文件路径**: `C++teamwork/config_example.txt`

**内容**:
```properties
# 讯飞Spark Lite API配置
# 请将your_api_password替换为您的实际API密钥
SPARK_API_KEY=your_api_password
```

**说明**: 配置模板文件，用于参考配置格式

---

## 六、数据文件

### 6.1 records.dat

**文件路径**: `C++teamwork/records.dat`

**文件大小**: 约 137B（示例）

**格式**: 二进制文件

**结构**:
```
[文件头]
- version: int (4 bytes)
- recordCount: int (4 bytes)
- timestamp: time_t (8 bytes)

[记录列表]
- Record[0]: struct (约280 bytes)
- Record[1]: struct (约280 bytes)
- ...
```

**注意**: 此文件包含用户数据，不应删除

---

## 七、开发环境配置

### 7.1 .vscode 配置

#### 顶层配置 `withAI2/.vscode/`

**launch.json**:
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "C/C++ Runner: Debug Session",
      "type": "cppdbg",
      "request": "launch",
      "program": "${workspaceFolder}/build/Debug/outDebug",
      "args": [],
      "stopAtEntry": false,
      "externalConsole": true,
      "cwd": "${workspaceFolder}",
      "MIMode": "gdb",
      "miDebuggerPath": "gdb"
    }
  ]
}
```

**tasks.json**: 构建任务配置

---

#### 项目配置 `withAI2/C++teamwork/.vscode/`

**c_cpp_properties.json**:
```json
{
  "configurations": [
    {
      "name": "Win32",
      "includePath": [
        "${workspaceFolder}/**"
      ],
      "defines": [
        "_DEBUG",
        "UNICODE",
        "_UNICODE"
      ],
      "compilerPath": "C:/C++/Codeblock/CodeBlocks/MinGW/bin/g++.exe",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "intelliSenseMode": "windows-msvc-x64"
    }
  ],
  "version": 4
}
```

**launch.json**: 项目级调试配置

**settings.json**: 编辑器设置

---

### 7.2 编译命令

**完整编译命令**:
```powershell
g++ -std=c++11 -o finance_manager main.cpp ai_analysis.cpp budget.cpp classification.cpp file_io.cpp statistics.cpp utils.cpp -I. -lwinhttp -lkernel32
```

**编译参数说明**:
- `-std=c++11`: 使用C++11标准
- `-o finance_manager`: 输出文件名
- `*.cpp`: 所有源文件
- `-I.`: 当前目录查找头文件
- `-lwinhttp`: 链接WinHTTP库
- `-lkernel32`: 链接kernel32库

---

## 八、文件依赖关系图

```
                        ┌─────────────────┐
                        │    main.cpp     │
                        │  (主程序入口)   │
                        └────────┬────────┘
                                 │
                    ┌────────────┼────────────┐
                    │            │            │
            ┌───────▼──────┐    │    ┌───────▼──────┐
            │ finance_     │    │    │  ai_analysis │
            │ manager.h    │    │    │     .h       │
            │ (核心头文件) │    │    └───────┬──────┘
            └───────┬──────┘    │            │
                    │           │            │
    ┌───────────────┼───────────┼────────────┤
    │               │           │            │
┌───▼───┐    ┌──────▼────┐ ┌───▼──┐   ┌────▼────────┐
│utils  │    │classifi-  │ │budget│   │statistics   │
│.cpp   │    │cation.cpp │ │.cpp  │   │.cpp         │
└───────┘    └───────────┘ └──┬───┘   └─────────────┘
                               │
                               │
                    ┌──────────▼──────────┐
                    │     file_io.cpp     │
                    │   (文件读写模块)     │
                    └──────────┬──────────┘
                               │
                    ┌──────────▼──────────┐
                    │     json.hpp        │
                    │  (第三方JSON库)      │
                    └─────────────────────┘
```

---

## 九、文件功能速查表

| 模块 | 源文件 | 头文件 | 主要功能 |
|------|--------|--------|----------|
| **主程序** | main.cpp | - | 用户界面、主循环 |
| **核心结构** | - | finance_manager.h | 数据结构定义 |
| **AI分析** | ai_analysis.cpp | ai_analysis.h | 智能财务分析 |
| **预算** | budget.cpp | - | 预算管理 |
| **统计** | statistics.cpp | - | 数据统计分析 |
| **分类** | classification.cpp | - | 自动分类 |
| **文件IO** | file_io.cpp | - | 数据持久化 |
| **工具** | utils.cpp | - | 通用工具函数 |
| **JSON** | - | json.hpp | JSON解析 |

---

## 十、构建产物

### 10.1 finance_manager.exe

**文件路径**: `C++teamwork/finance_manager.exe`

**文件大小**: 约 2MB

**类型**: Windows 可执行文件 (PE32+)

**依赖**:
- libstdc++-6.dll
- libgcc_s_seh-1.dll
- libwinpthread-1.dll
- kernel32.dll
- winhttp.dll

**运行方法**:
```powershell
.\finance_manager.exe
```

---

## 十一、常见问题

### Q1: 编译时出现 WinHTTP 链接错误

**错误信息**:
```
undefined reference to `__imp_WinHttpOpen'
```

**解决方法**:
```powershell
g++ -o program *.cpp -lwinhttp -lkernel32
```

---

### Q2: 控制台中文显示乱码

**解决方法**:
在程序开始时调用 `setConsoleUTF8()` 函数设置UTF-8编码

---

### Q3: records.dat 文件损坏

**解决方法**:
1. 备份现有数据
2. 删除 records.dat 文件
3. 重新运行程序（会创建新的空数据文件）

---

## 十二、扩展开发指南

### 12.1 添加新功能步骤

1. 在对应模块编写功能代码
2. 更新头文件声明
3. 在 main.cpp 中添加菜单选项
4. 编译测试

### 12.2 添加新分类

编辑 `classification.cpp` 中的 `autoClassify()` 函数：
```cpp
Category autoClassify(const std::string& description, RecordType type) {
    // 添加新的分类判断逻辑
    if (description.find("关键词") != string::npos) {
        return Category::NEW_CATEGORY;
    }
    // ...
}
```

### 12.3 修改AI分析逻辑

编辑 `ai_analysis.cpp` 中的分析函数，调用新的API或算法

---

## 十三、版本历史

| 版本 | 日期 | 更新内容 |
|------|------|----------|
| 1.0 | 2025-12-29 | 初始版本，完整功能实现 |

---

## 十四、参考资源

- C++ 参考: https://en.cppreference.com/
- nlohmann/json: https://github.com/nlohmann/json
- MinGW: https://www.mingw-w64.org/
- 讯飞Spark: https://www.xfyun.cn/doc/spark/

---

**文档维护**: 请在修改项目结构后同步更新此文档

**最后更新**: 2025-12-29
