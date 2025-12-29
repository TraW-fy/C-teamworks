# C++ 智能记账本 (Finance Manager)

## 项目简介

这是一个功能完整的 C++ 财务管理应用，提供收支记录、统计分析、预算管理和 AI 智能分析等功能。

## 项目结构

```
C++teamwork/
├── src/                        # 源代码文件
│   ├── main.cpp               # 主程序入口
│   ├── finance_manager.h      # 核心数据结构定义
│   ├── ai_analysis.cpp/h      # AI智能分析模块
│   ├── budget.cpp             # 预算管理模块
│   ├── statistics.cpp         # 统计分析模块
│   ├── classification.cpp     # 分类管理模块
│   ├── file_io.cpp            # 文件读写模块
│   └── utils.cpp              # 工具函数模块
├── include/                    # 头文件目录
├── tests/                      # 单元测试
├── config.txt                  # 运行时配置文件
├── config_example.txt          # 配置示例文件
├── records.dat                 # 数据存储文件
└── RUNNING.md                  # 运行说明文档
```

## 功能特性

### 核心功能
1. **添加收支记录** - 支持收入/支出类型，自动分类
2. **查看所有记录** - 格式化显示所有收支记录
3. **编辑/删除记录** - 修改记录信息或删除记录，支持一键清空
4. **搜索记录** - 按日期、类型、分类、金额等条件搜索
5. **统计分析** - 支持按日/周/月统计，消费分布分析，预算预警
6. **管理预算** - 设置月度预算，监控支出
7. **保存记录** - 将记录保存到文件
8. **导入TXT记录** - 从文本文件批量导入记录
9. **AI智能分析** - 提供财务建议和分析报告（需要网络）

## 技术栈

- **编程语言**: C++11/17
- **编译器**: MinGW-w64 (CodeBlocks)
- **构建工具**: g++ (GCC)
- **依赖库**: WinHTTP (Windows HTTP API)
- **配置文件**: JSON 格式

## 环境要求

- Windows 操作系统
- MinGW-w64 编译器 (GCC 12.2+)
- VS Code (推荐) 或 CodeBlocks

## 编译方法

### 方法一：VS Code (推荐)

1. 打开项目目录
2. 按 `Ctrl+Shift+B` 选择构建任务
3. 或在终端执行: `终端 → 运行任务 → build: finance_manager`

### 方法二：PowerShell 手动编译

```powershell
# 切换到项目目录
cd "d:\C++project\project\withAI2\C++teamwork"

# 编译所有源文件
g++ -std=c++11 -o finance_manager main.cpp ai_analysis.cpp budget.cpp classification.cpp file_io.cpp statistics.cpp utils.cpp -I. -lwinhttp -lkernel32

# 运行程序
.\finance_manager.exe
```

### 方法三：使用现有编译脚本

```powershell
# 使用 PowerShell 脚本编译
.\count_code_final.ps1
```

## 运行程序

```powershell
.\finance_manager.exe
```

程序启动后会显示主菜单：

```
=========================================
           C++ 智能记账本
=========================================
1.  添加收支记录
2.  查看所有记录
3.  编辑/删除记录
4.  搜索记录
5.  统计分析
6.  管理预算
7.  保存记录
8.  导入TXT记录
0.  退出系统
=========================================
```

## 使用说明

### 添加记录
1. 选择菜单 `1`
2. 输入收支类型（收入/支出）
3. 输入金额
4. 输入日期（可选，默认当前日期）
5. 输入备注描述
6. 系统会自动分类，支持手动修改

### 搜索记录
1. 选择菜单 `4`
2. 按不同条件搜索：日期、类型、分类、金额
3. 支持模糊匹配

### 统计分析
1. 选择菜单 `5`
2. 可选择：
   - 按日统计
   - 按周统计
   - 按月统计
   - 查看消费分布
   - 预算预警
   - AI智能分析

### AI智能分析
- 需要网络连接
- 提供财务健康度评估
- 支出异常预警
- 个性化理财建议

### 预算管理
1. 选择菜单 `6`
2. 设置月度预算上限
3. 系统会在超预算时发出预警

## 配置文件

### config.txt
```
# API配置
API_KEY=your_api_key_here

# 预算配置
MONTHLY_BUDGET=5000.00

# 主题配置
THEME=dark
```

### config_example.txt
```
# API配置
API_KEY=your_api_key_here

# 预算配置
MONTHLY_BUDGET=5000.00

# 主题配置
THEME=dark
```

## 数据存储

- **records.dat**: 二进制格式存储所有收支记录
- **config.txt**: JSON 格式存储配置信息
- 支持自动保存和手动保存

## 常见问题

### 编译错误：undefined reference to WinHTTP
**解决方法**: 添加 `-lwinhttp -lkernel32` 链接参数

### 找不到 g++.exe
**解决方法**: 确保 MinGW 已添加到系统 PATH，或使用完整路径调用

### 程序无法启动
**解决方法**: 检查是否缺少必需的 DLL 文件（WinHTTP 相关）

## 扩展开发

### 添加新功能
1. 在对应模块文件编写功能代码
2. 更新 finance_manager.h 头文件
3. 在 main.cpp 中添加菜单选项和调用

### 修改分类
编辑 classification.cpp 中的 `autoClassify` 函数

### 添加 AI 分析
编辑 ai_analysis.cpp 中的 AI 分析逻辑

## 开发团队

- 主要开发者: C++全栈工匠
- 项目版本: 2.0
- 最后更新: 2025-12-29

## License

MIT License

## 贡献

欢迎提交 Issue 和 Pull Request！
