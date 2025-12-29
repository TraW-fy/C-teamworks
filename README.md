# C++ 智能记账本 (Smart Accounting Book AI)

> 文档版本: 2.0
> 更新日期: 2025-12-29
> 项目路径: `d:\C++project\project\Smart_accounting_book_AI\C++teamwork\`

---

## 一、项目概述

### 1.1 项目简介

这是一个功能完整的 C++ 财务管理应用，提供收支记录、统计分析、预算管理和 AI 智能分析等功能。项目采用模块化设计，支持自动分类、预算预警和讯飞Spark AI 智能分析建议。

### 1.2 技术栈

| 技术 | 版本/说明 |
|------|----------|
| 编程语言 | C++11 |
| 编译器 | MinGW-w64 (GCC 12.2) |
| 构建工具 | g++ |
| 依赖库 | WinHTTP (Windows HTTP API) |
| 第三方库 | nlohmann/json 3.10.5 |
| 操作系统 | Windows 10/11 |

### 1.3 主要功能

| 功能 | 描述 | 状态 |
|------|------|------|
| 收支记录 | 添加/编辑/删除收支记录，自动分类 | ✅ |
| 记录查看 | 格式化表格显示所有记录 | ✅ |
| 记录搜索 | 按日期、类型、分类、金额搜索 | ✅ |
| 统计分析 | 日/周/月统计，消费分布分析 | ✅ |
| 预算管理 | 设置月度预算，支出预警提醒 | ✅ |
| 数据持久化 | 自动保存到 records.dat | ✅ |
| 数据导出 | 导出为 TXT 表格格式 | ✅ |
| AI智能分析 | 讯飞Spark API 财务建议 | ✅ |

---

## 二、目录结构

```
Smart_accounting_book_AI/
│
├── .vscode/                      # VS Code 工作区配置
│   ├── launch.json               # 调试启动配置
│   └── tasks.json                # 构建任务配置
│
├── C++teamwork/                  # 主项目目录
│   │
│   ├── .vscode/                 # VS Code 项目配置
│   │   ├── c_cpp_properties.json # C++ 语言服务器配置
│   │   ├── launch.json           # 调试配置
│   │   └── settings.json         # 编辑器设置
│   │
│   ├── 核心源文件 (*.cpp)
│   │   ├── main.cpp              # 主程序入口，UI交互
│   │   ├── ai_analysis.cpp       # AI智能分析模块
│   │   ├── budget.cpp            # 预算管理模块
│   │   ├── classification.cpp    # 自动分类模块
│   │   ├── file_io.cpp           # 文件读写模块
│   │   ├── statistics.cpp        # 统计分析模块
│   │   └── utils.cpp             # 工具函数模块
│   │
│   ├── 头文件 (*.h)
│   │   ├── finance_manager.h     # 核心数据结构定义
│   │   └── ai_analysis.h         # AI模块接口定义
│   │
│   ├── 第三方库
│   │   └── json.hpp              # nlohmann/json (JSON解析)
│   │
│   ├── 配置文件
│   │   ├── config.txt            # 运行时配置 (含API密钥)
│   │   └── config_example.txt    # 配置示例
│   │
│   ├── 数据文件
│   │   └── records.dat           # 收支记录数据文件
│   │
│   ├── 文档
│   │   ├── README.md             # 项目说明文档
│   │   └── RUNNING.md            # 运行说明
│   │
│   ├── 编译脚本
│   │   └── count_code_final.ps1  # PowerShell编译脚本
│   │
│   └── 可执行文件
│       └── app.exe               # 主程序可执行文件
│
└── .gitignore.txt               # Git忽略配置
```

---

## 三、核心模块详解

### 3.1 main.cpp（主程序入口）

**文件路径**: `C++teamwork/main.cpp`  
**代码行数**: ~560行

**功能描述**: 程序主入口文件，包含用户交互界面、主控制循环和菜单系统

**核心函数**:

| 函数名 | 功能 | 行号 |
|--------|------|------|
| `showMenu()` | 显示主菜单界面 | 16-32 |
| `addRecord()` | 添加收支记录 | 37-96 |
| `viewRecords()` | 查看所有记录 | 101-135 |
| `editRecord()` | 编辑/删除记录 | - |
| `searchRecords()` | 搜索记录 | - |
| `statisticsMenu()` | 统计分析菜单 | - |
| `manageBudget()` | 管理预算 | - |
| `exportRecordsToTxt()` | 导出为表格 | - |

**主菜单选项**:
```
┌─────────────────────────────────────────┐
│                智能记账本                 │
├─────────────────────────────────────────┤
│ 1. 添加收支记录                          │
│ 2. 查看所有记录                          │
│ 3. 编辑/删除记录                         │
│ 4. 搜索记录                              │
│ 5. 统计分析                              │
│ 6. 管理预算                              │
│ 7. 保存记录                              │
│ 8. 导出为表格                            │
│ 0. 退出系统                              │
└─────────────────────────────────────────┘
```

---

### 3.2 finance_manager.h（核心数据结构）

**文件路径**: `C++teamwork/finance_manager.h`  
**代码行数**: ~150行

**核心数据结构**:

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

// 收支记录结构体
struct Record {
    int id;                   // 记录ID
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

---

### 3.3 ai_analysis.cpp/h（AI智能分析）

**文件路径**: `C++teamwork/ai_analysis.cpp`  
**功能描述**: 调用讯飞Spark API提供智能财务分析

**核心函数**:

| 函数名 | 功能 |
|--------|------|
| `generateAIAnalysis()` | 生成AI财务分析报告 |
| `callSparkAPI()` | 调用讯飞Spark API |
| `callAIModel()` | 调用AI模型 |
| `generateHumorousPrompt()` | 生成幽默风格的提示词 |
| `parseAIResponse()` | 解析AI响应 |
| `generateMockResponse()` | 生成模拟响应（无API时） |
| `loadAPIKey()` | 加载API密钥 |

**API配置**:

```cpp
struct SparkConfig {
    std::string apiUrl;      // API地址
    std::string apiPassword; // API密钥
    std::string model;       // 模型名称
    int timeout;             // 超时时间(ms)
};
```

---

### 3.4 其他模块

| 模块 | 文件 | 功能 |
|------|------|------|
| 预算管理 | `budget.cpp` | 预算设置、预算预警检查 |
| 自动分类 | `classification.cpp` | 基于关键词的智能分类 |
| 文件IO | `file_io.cpp` | 记录存取、导入导出TXT |
| 统计分析 | `statistics.cpp` | 金额统计、支出分布饼图 |
| 工具函数 | `utils.cpp` | 日期处理、编码设置、类型转换 |

---

## 四、编译与运行

### 4.1 环境要求

- Windows 10/11
- MinGW-w64 (GCC 12.2+)
- VS Code (推荐) 或 CodeBlocks

### 4.2 编译方法

#### 方法一：VS Code（推荐）

1. 打开项目目录
2. 按 `Ctrl+Shift+B`
3. 选择 `build: finance_manager (all files)`

#### 方法二：PowerShell 手动编译

```powershell
cd "d:\C++project\project\Smart_accounting_book_AI\C++teamwork"

g++ -std=c++11 -o app.exe *.cpp -I. -lwinhttp -lkernel32
```

#### 方法三：使用编译脚本

```powershell
.\count_code_final.ps1
```

### 4.3 运行程序

```powershell
.\app.exe
```

---

## 五、数据格式

### 5.1 records.dat 格式

```
ID|类型|分类|金额|日期|备注
1|收入|生活费|20000.00|2025-12-12|中奖
2|支出|学习|200.00|2025-12-12|学习资料
```

### 5.2 分类规则

| 分类 | 关键词示例 |
|------|-----------|
| 餐饮 | 食堂、吃饭、外卖、奶茶、咖啡、火锅 |
| 学习 | 买书、教材、文具、书店、考试、培训 |
| 娱乐 | 电影、游戏、旅游、聚会、唱歌 |
| 交通 | 公交、地铁、打车、出租车 |
| 生活费 | 生活费、allowance |
| 兼职 | 兼职、part-time、打工 |

---

## 六、配置说明

### 6.1 config.txt 格式

```
SPARK_API_KEY=your_api_key_here
```

### 6.2 获取API密钥

1. 注册讯飞Spark API
2. 在 [讯飞开放平台](https://xinghuo.xfyun.cn/) 创建应用
3. 将API密钥填入 config.txt

---

## 七、更新日志

### v2.0 (2025-12-29)
- 修复菜单重复显示问题
- 更新项目文档
- 优化代码结构

### v1.0 (2025-12-29)
- 初始版本
- 实现核心收支管理功能
- 添加AI智能分析

---

## 八、已知问题

1. ⚠️ API密钥暴露在 config.txt 中，建议加入 .gitignore
2. ⚠️ 仅支持Windows平台（依赖WinHTTP）
3. ℹ️ 缺少单元测试用例
4. ℹ️ 缺少CMake构建系统

---

## 九、后续优化建议

- [ ] 添加CMake构建系统
- [ ] 编写单元测试（GoogleTest/Catch2）
- [ ] 添加GitHub Actions CI/CD
- [ ] 支持跨平台（Linux/macOS）
- [ ] 添加数据可视化图表
- [ ] 支持数据库存储（SQLite）

---

## 十、许可证

本项目仅供学习使用。

---

**作者**: C++全栈工匠  
**联系方式**: GitHub项目仓库
