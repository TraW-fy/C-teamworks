#ifndef FINANCE_MANAGER_H
#define FINANCE_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>

// Windows系统下的中文控制台支持
#ifdef _WIN32
#include <windows.h>
#endif

// 枚举类型定义收支类型和分类
enum class RecordType { INCOME, EXPENSE };
enum class Category {
    // 支出分类
    FOOD,       // 餐饮
    STUDY,      // 学习
    ENTERTAINMENT, // 娱乐
    TRANSPORT,  // 交通
    // 收入分类
    LIVING_ALLOWANCE, // 生活费
    PART_TIME   // 兼职
};

// 收支记录结构体
struct Record {
    int id;                 // 记录ID
    RecordType type;        // 收支类型
    Category category;      // 分类
    double amount;          // 金额
    std::string date;       // 日期 (YYYY-MM-DD)
    std::string description; // 备注
};

// 预算设置结构体
struct Budget {
    double totalBudget;     // 总预算
    double foodBudget;      // 餐饮预算
    double studyBudget;     // 学习预算
    double entertainmentBudget; // 娱乐预算
    double transportBudget; // 交通预算
};

// -----------------------
// 工具函数模块
// -----------------------

// 设置控制台编码为UTF-8
void setConsoleUTF8();

// 获取当前日期 (YYYY-MM-DD)
std::string getCurrentDate();

// 验证日期格式是否正确
bool isValidDate(const std::string& date);

// 分类枚举转字符串
std::string categoryToString(Category category);

// 字符串转分类枚举
Category stringToCategory(const std::string& categoryStr);

// 字符串转收支类型枚举
RecordType stringToRecordType(const std::string& typeStr);

// 收支类型枚举转字符串
std::string recordTypeToString(RecordType type);

// -----------------------
// 关键词分类模块
// -----------------------

// 根据描述和类型自动分类
Category autoClassify(const std::string& description, RecordType type);

// -----------------------
// 文件IO模块
// -----------------------

// 保存记录到文件
void saveRecordsToFile(const std::vector<Record>& records, const std::string& filename);

// 从文件加载记录
std::vector<Record> loadRecordsFromFile(const std::string& filename);

// 从TXT文件导入记录
std::vector<Record> importRecordsFromTxt(const std::string& filename);

// -----------------------
// 统计分析模块
// -----------------------

// 计算总金额
// type: 收支类型，默认为支出
// startDate: 开始日期，默认为空（不限制）
// endDate: 结束日期，默认为空（不限制）
double calculateTotalAmount(const std::vector<Record>& records, 
                           RecordType type = RecordType::EXPENSE, 
                           const std::string& startDate = "", 
                           const std::string& endDate = "");

// 计算某分类的总金额
// category: 分类
// startDate: 开始日期，默认为空（不限制）
// endDate: 结束日期，默认为空（不限制）
double calculateCategoryAmount(const std::vector<Record>& records, 
                              Category category, 
                              const std::string& startDate = "", 
                              const std::string& endDate = "");

// 打印支出分布（包含字符饼图）
// startDate: 开始日期，默认为空（不限制）
// endDate: 结束日期，默认为空（不限制）
void printExpenseDistribution(const std::vector<Record>& records, 
                             const std::string& startDate = "", 
                             const std::string& endDate = "");

// -----------------------
// 预算预警模块
// -----------------------

// 检查预算预警
// records: 记录列表
// budget: 预算设置
// month: 月份（格式：YYYY-MM）
void checkBudgetAlert(const std::vector<Record>& records, 
                     const Budget& budget, 
                     const std::string& month);

// -----------------------
// 界面模块
// -----------------------

// 显示主菜单
void showMenu();

// 添加收支记录
void addRecord(std::vector<Record>& records);

// 查看所有记录
void viewRecords(const std::vector<Record>& records);

// 编辑记录
void editRecord(std::vector<Record>& records);

// 删除记录
void deleteRecord(std::vector<Record>& records);

// 搜索记录
void searchRecords(const std::vector<Record>& records);

// 统计分析菜单
void statisticsMenu(const std::vector<Record>& records, const Budget& budget);

// 管理预算
void manageBudget(Budget& budget);

// -----------------------
// AI分析模块
// -----------------------

// 生成消费分析报告
// records: 记录列表
// startDate: 开始日期（格式：YYYY-MM-DD）
// endDate: 结束日期（格式：YYYY-MM-DD）
// 返回AI分析结果
struct AIAnalysisResult;

AIAnalysisResult generateAIAnalysis(const std::vector<Record>& records, 
                                   const std::string& startDate, 
                                   const std::string& endDate);

#endif // FINANCE_MANAGER_H
