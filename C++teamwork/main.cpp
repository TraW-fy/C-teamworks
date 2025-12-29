#include "finance_manager.h"
#include "ai_analysis.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// 交互界面函数
// 显示主菜单
void showMenu() {
    system("cls");
    cout << "┌─────────────────────────────────────────┐" << endl;
    cout << "│                智能记账本                 │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    cout << "┌─────────────────────────────────────────┐" << endl;
    cout << "│ 1. 添加收支记录                          │" << endl;
    cout << "│ 2. 查看所有记录                          │" << endl;
    cout << "│ 3. 编辑/删除记录                         │" << endl;
    cout << "│ 4. 搜索记录                             │" << endl;
    cout << "│ 5. 统计分析                             │" << endl;
    cout << "│ 6. 管理预算                             │" << endl;
    cout << "│ 7. 保存记录                             │" << endl;
    cout << "│ 8. 导出为表格                           │" << endl;
    cout << "│ 0. 退出系统                             │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    cout << "请输入您的选择 (直接回车返回主菜单): ";
}

void addRecord(vector<Record>& records) {
    Record record;
    
    cout << "\n┌─────────────────────────────────────────┐" << endl;
    cout << "│              ➕ 添加收支记录             │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    
    // 生成ID
    if (records.empty()) {
        record.id = 1;
    } else {
        record.id = records.back().id + 1;
    }
    
    // 输入收支类型
    string typeStr;
    cout << "📝 请输入收支类型 (收入/支出): ";
    getline(cin, typeStr);
    record.type = stringToRecordType(typeStr);
    
    // 输入金额
    string amountStr;
    cout << "💰 请输入金额: ";
    getline(cin, amountStr);
    record.amount = stod(amountStr);
    
    // 输入日期
    string dateStr;
    cout << "📅 请输入日期 (YYYY-MM-DD, 回车使用当前日期): ";
    getline(cin, dateStr);
    if (dateStr.empty() || !isValidDate(dateStr)) {
        record.date = getCurrentDate();
        cout << "✅ 使用当前日期: " << record.date << endl;
    } else {
        record.date = dateStr;
    }
    
    // 输入备注
    cout << "💬 请输入备注: ";
    getline(cin, record.description);
    
    // 自动分类
    record.category = autoClassify(record.description, record.type);
    cout << "🔍 自动分类为: " << categoryToString(record.category) << endl;
    
    // 询问是否修改分类
    string modifyStr;
    cout << "🔄 是否需要修改分类? (y/n): ";
    getline(cin, modifyStr);
    if (modifyStr == "y" || modifyStr == "Y") {
        string categoryStr;
        cout << "📋 请输入新分类: ";
        getline(cin, categoryStr);
        record.category = stringToCategory(categoryStr);
    }
    
    // 添加到记录列表
    records.push_back(record);
    cout << "\n🎉 记录添加成功！" << endl;
}

void viewRecords(const vector<Record>& records) {
    if (records.empty()) {
        cout << "\n┌─────────────────────────────────────────┐" << endl;
        cout << "│           📝 没有记录！                │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;
        return;
    }
    
    cout << "\n┌────────────────────────────────────────────────────────────────────────────┐" << endl;
    cout << "│                              📋 收支记录                               │" << endl;
    cout << "├─────┬────────┬──────────┬────────────┬────────────┬───────────────────────┤" << endl;
    cout << "│  ID │  类型  │   分类   │  金额(元)  │    日期    │         备注          │" << endl;
    cout << "├─────┼────────┼──────────┼────────────┼────────────┼───────────────────────┤" << endl;
    
    for (const Record& record : records) {
        string typeStr = recordTypeToString(record.type);
        string categoryStr = categoryToString(record.category);
        
        // 确保备注不超过最大长度
        string description = record.description;
        if (description.length() > 23) {
            description = description.substr(0, 20) + "...";
        }
        
        cout << "│ " << right << setw(3) << record.id 
             << " │ " << left << setw(6) << typeStr 
             << " │ " << left << setw(8) << categoryStr 
             << " │ " << right << setw(10) << fixed << setprecision(2) << record.amount 
             << " │ " << left << setw(10) << record.date 
             << " │ " << left << setw(23) << description << " │" << endl;
    }
    
    cout << "└─────┴────────┴──────────┴────────────┴────────────┴───────────────────────┘" << endl;
}

void editRecord(vector<Record>& records) {
    if (records.empty()) {
        cout << "\n┌─────────────────────────────────────────┐" << endl;
        cout << "│       ✏️  没有记录可以编辑！            │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;
        return;
    }
    
    cout << "\n┌─────────────────────────────────────────┐" << endl;
    cout << "│              ✏️  编辑记录                │" << endl;
    cout << "├─────────────────────────────────────────┤" << endl;
    cout << "│ 1. 修改记录信息                        │" << endl;
    cout << "│ 2. 删除记录                            │" << endl;
    cout << "│ 3. 一键删除所有记录                    │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    cout << "请选择操作 (直接回车返回): ";
    
    string input;
    getline(cin, input);
    
    if (input.empty()) {
        return;
    }
    
    int subChoice = stoi(input);
    
    if (subChoice == 2) {
        deleteRecord(records);
        return;
    }
    
    if (subChoice == 3) {
        if (records.empty()) {
            cout << "\n┌─────────────────────────────────────────┐" << endl;
            cout << "│       🗑️  没有记录可以删除！            │" << endl;
            cout << "└─────────────────────────────────────────┘" << endl;
            return;
        }
        
        cout << "\n┌─────────────────────────────────────────┐" << endl;
        cout << "│ ⚠️  警告：此操作将删除所有记录，且无法撤销！ │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;
        cout << "确认删除所有记录？请输入 'y' 确认，其他按键取消: ";
        getline(cin, input);
        
        if (input == "y" || input == "Y") {
            records.clear();
            cout << "所有记录已删除！" << endl;
        } else {
            cout << "已取消删除操作。" << endl;
        }
        return;
    }
    
    if (subChoice != 1) {
        cout << "\n❌ 无效的选择！" << endl;
        return;
    }
    
    viewRecords(records);
    
    cout << "请输入要编辑的记录ID (直接回车返回): ";
    getline(cin, input);
    
    if (input.empty()) {
        return;
    }
    
    int id = stoi(input);
    
    // 查找记录
    auto it = find_if(records.begin(), records.end(), 
        [id](const Record& record) { return record.id == id; });
    
    if (it == records.end()) {
        cout << "找不到ID为 " << id << " 的记录！" << endl;
        return;
    }
    
    // 显示当前记录信息
    cout << "\n┌─────────────────────────────────────────┐" << endl;
    cout << "│           📊 当前记录信息              │" << endl;
    cout << "├─────────────────────────────────────────┤" << endl;
    cout << "│ 类型: " << left << setw(27) << recordTypeToString(it->type) << " │" << endl;
    cout << "│ 分类: " << left << setw(27) << categoryToString(it->category) << " │" << endl;
    cout << "│ 金额: " << left << setw(27) << fixed << setprecision(2) << it->amount << " 元" << " │" << endl;
    cout << "│ 日期: " << left << setw(27) << it->date << " │" << endl;
    cout << "│ 备注: " << left << setw(27) << it->description << " │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    
    // 输入新信息
    
    cout << "请输入新的收支类型 (回车不修改): ";
    getline(cin, input);
    if (!input.empty()) {
        it->type = stringToRecordType(input);
    }
    
    cout << "请输入新的金额 (回车不修改): ";
    getline(cin, input);
    if (!input.empty()) {
        it->amount = stod(input);
    }
    
    cout << "请输入新的日期 (YYYY-MM-DD, 回车不修改): ";
    getline(cin, input);
    if (!input.empty() && isValidDate(input)) {
        it->date = input;
    }
    
    cout << "请输入新的备注 (回车不修改): ";
    string oldDescription = it->description;
    getline(cin, input);
    if (!input.empty()) {
        it->description = input;
    }
    
    // 如果修改了备注，重新自动分类
    if (it->description != oldDescription) {
        it->category = autoClassify(it->description, it->type);
        cout << "根据新备注重新分类为: " << categoryToString(it->category) << endl;
    }
    
    // 询问是否修改分类
    cout << "是否需要修改分类? (y/n): ";
    getline(cin, input);
    if (input == "y" || input == "Y") {
        string categoryStr;
        cout << "请输入新分类: ";
        getline(cin, categoryStr);
        it->category = stringToCategory(categoryStr);
    }
    
    cout << "记录编辑成功！" << endl;
}

void deleteRecord(vector<Record>& records) {
    if (records.empty()) {
        cout << "\n┌─────────────────────────────────────────┐" << endl;
        cout << "│       🗑️  没有记录可以删除！            │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;
        return;
    }
    
    viewRecords(records);
    
    int id;
    cout << "请输入要删除的记录ID: ";
    cin >> id;
    cin.ignore();
    
    // 查找记录
    auto it = find_if(records.begin(), records.end(), 
        [id](const Record& record) { return record.id == id; });
    
    if (it == records.end()) {
        cout << "找不到ID为 " << id << " 的记录！" << endl;
        return;
    }
    
    // 确认删除
    string confirm;
    cout << "确定要删除这条记录吗? (y/n): ";
    getline(cin, confirm);
    
    if (confirm == "y" || confirm == "Y") {
        records.erase(it);
        cout << "记录删除成功！" << endl;
    } else {
        cout << "删除操作已取消！" << endl;
    }
}

void searchRecords(const vector<Record>& records) {
    if (records.empty()) {
        cout << "\n┌─────────────────────────────────────────┐" << endl;
        cout << "│       🔍 没有记录可以搜索！            │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;
        return;
    }
    
    cout << "\n┌─────────────────────────────────────┐" << endl;
    cout << "│              🔍 搜索记录                │" << endl;
    cout << "├─────────────────────────────────────┤" << endl;
    cout << "│ 1. 按日期搜索                          │" << endl;
    cout << "│ 2. 按类型搜索                          │" << endl;
    cout << "│ 3. 按分类搜索                          │" << endl;
    cout << "│ 4. 按金额范围搜索                      │" << endl;
    cout << "└─────────────────────────────────────┘" << endl;
    cout << "请选择搜索方式: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    vector<Record> results;
    
    switch (choice) {
        case 1: {
            string date;
            cout << "请输入日期 (YYYY-MM-DD): ";
            getline(cin, date);
            
            for (const Record& record : records) {
                if (record.date == date) {
                    results.push_back(record);
                }
            }
            break;
        }
        case 2: {
            string typeStr;
            cout << "请输入类型 (收入/支出): ";
            getline(cin, typeStr);
            RecordType type = stringToRecordType(typeStr);
            
            for (const Record& record : records) {
                if (record.type == type) {
                    results.push_back(record);
                }
            }
            break;
        }
        case 3: {
            string categoryStr;
            cout << "请输入分类: ";
            getline(cin, categoryStr);
            Category category = stringToCategory(categoryStr);
            
            for (const Record& record : records) {
                if (record.category == category) {
                    results.push_back(record);
                }
            }
            break;
        }
        case 4: {
            double minAmount, maxAmount;
            cout << "请输入最小金额: ";
            cin >> minAmount;
            cout << "请输入最大金额: ";
            cin >> maxAmount;
            cin.ignore();
            
            for (const Record& record : records) {
                if (record.amount >= minAmount && record.amount <= maxAmount) {
                    results.push_back(record);
                }
            }
            break;
        }
        default:
            cout << "\n❌ 无效的选择！" << endl;
            return;
    }
    
    // 显示搜索结果
    if (results.empty()) {
        cout << "\n┌─────────────────────────────────────────┐" << endl;
        cout << "│       ❌ 没有找到符合条件的记录！      │" << endl;
        cout << "└─────────────────────────────────────────┘" << endl;
    } else {
        cout << "\n┌────────────────────────────────────────────────────────────────────────────┐" << endl;
        cout << "│             🔍 搜索结果 (共 " << setw(2) << results.size() << " 条)              │" << endl;
        cout << "├─────┬────────┬──────────┬────────────┬────────────┬───────────────────────┤" << endl;
        cout << "│  ID │  类型  │   分类   │  金额(元)  │    日期    │         备注          │" << endl;
        cout << "├─────┼────────┼──────────┼────────────┼────────────┼───────────────────────┤" << endl;
        
        for (const Record& record : results) {
            string typeStr = recordTypeToString(record.type);
            string categoryStr = categoryToString(record.category);
            
            // 确保备注不超过最大长度
            string description = record.description;
            if (description.length() > 23) {
                description = description.substr(0, 20) + "...";
            }
            
            cout << "│ " << right << setw(3) << record.id 
                 << " │ " << left << setw(6) << typeStr 
                 << " │ " << left << setw(8) << categoryStr 
                 << " │ " << right << setw(10) << fixed << setprecision(2) << record.amount 
                 << " │ " << left << setw(10) << record.date 
                 << " │ " << left << setw(23) << description << " │" << endl;
        }
        cout << "└─────┴────────┴──────────┴────────────┴────────────┴───────────────────────┘" << endl;
    }
}

void statisticsMenu(const vector<Record>& records, const Budget& budget) {
    cout << "\n┌─────────────────────────────────────────┐" << endl;
    cout << "│              📈 统计分析                │" << endl;
    cout << "├─────────────────────────────────────────┤" << endl;
    cout << "│ 1. 按日统计                            │" << endl;
    cout << "│ 2. 按周统计                            │" << endl;
    cout << "│ 3. 按月统计                            │" << endl;
    cout << "│ 4. 查看消费分布                        │" << endl;
    cout << "│ 5. 预算预警                            │" << endl;
    cout << "│ 6. AI智能分析                          │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    cout << "请选择统计方式: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string startDate, endDate, month;
    
    switch (choice) {
        case 1: {
            cout << "请输入日期 (YYYY-MM-DD): ";
            string date;
            getline(cin, date);
            startDate = date;
            endDate = date;
            break;
        }
        case 2: {
            cout << "请输入周的开始日期 (YYYY-MM-DD): ";
            getline(cin, startDate);
            cout << "请输入周的结束日期 (YYYY-MM-DD): ";
            getline(cin, endDate);
            break;
        }
        case 3: {
            cout << "请输入月份 (YYYY-MM): ";
            getline(cin, month);
            startDate = month + "-01";
            // 简单处理，假设每月31天
            endDate = month + "-31";
            break;
        }
        case 4: {
            cout << "请输入开始日期 (YYYY-MM-DD, 回车不限制): ";
            getline(cin, startDate);
            cout << "请输入结束日期 (YYYY-MM-DD, 回车不限制): ";
            getline(cin, endDate);
            printExpenseDistribution(records, startDate, endDate);
            return;
        }
        case 5: {
            cout << "请输入月份 (YYYY-MM, 回车使用当前月份): ";
            getline(cin, month);
            if (month.empty()) {
                // 使用当前月份
                time_t now = time(0);
                tm* ltm = localtime(&now);
                stringstream ss;
                ss << 1900 + ltm->tm_year << "-" 
                   << setw(2) << setfill('0') << 1 + ltm->tm_mon;
                month = ss.str();
            }
            checkBudgetAlert(records, budget, month);
            return;
        }
        case 6: {
            cout << "\n┌─────────────────────────────────────────┐" << endl;
            cout << "│            🤖 AI智能分析                │" << endl;
            cout << "└─────────────────────────────────────────┘" << endl;
            
            // 获取日期范围
            string startDate, endDate;
            cout << "请输入开始日期 (YYYY-MM-DD, 回车不限制): ";
            getline(cin, startDate);
            cout << "请输入结束日期 (YYYY-MM-DD, 回车不限制): ";
            getline(cin, endDate);
            
            // 生成AI分析
            cout << "\n🔍 正在分析消费情况..." << endl;
            AIAnalysisResult result = generateAIAnalysis(records, startDate, endDate);
            
            // 显示分析结果
            cout << "\n┌─────────────────────────────────────────┐" << endl;
            cout << "│           📊 消费分析报告              │" << endl;
            cout << "└─────────────────────────────────────────┘" << endl;
            cout << result.summary << endl;
            
            cout << "\n┌─────────────────────────────────────────┐" << endl;
            cout << "│           💡 消费洞察                  │" << endl;
            cout << "└─────────────────────────────────────────┘" << endl;
            cout << result.insights << endl;

            return;
        }
        default:
            cout << "\n❌ 无效的选择！" << endl;
            return;
    }
    
    // 计算并显示统计结果
    double totalIncome = calculateTotalAmount(records, RecordType::INCOME, startDate, endDate);
    double totalExpense = calculateTotalAmount(records, RecordType::EXPENSE, startDate, endDate);
    double balance = totalIncome - totalExpense;
    
    cout << "\n┌─────────────────────────────────────────┐" << endl;
    cout << "│             📊 统计结果                │" << endl;
    cout << "├─────────────────────────────────────────┤" << endl;
    cout << "│ 开始日期: " << left << setw(23) << (startDate.empty() ? "不限" : startDate) << " │" << endl;
    cout << "│ 结束日期: " << left << setw(23) << (endDate.empty() ? "不限" : endDate) << " │" << endl;
    cout << "├─────────────────────────────────────────┤" << endl;
    cout << "│ 总收入: " << left << setw(25) << fixed << setprecision(2) << totalIncome << " 元" << " │" << endl;
    cout << "│ 总支出: " << left << setw(25) << fixed << setprecision(2) << totalExpense << " 元" << " │" << endl;
    cout << "│ 结余: " << left << setw(27) << fixed << setprecision(2) << balance << " 元" << " │" << endl;
    cout << "└─────────────────────────────────────────┘" << endl;
    
    // 显示各分类支出
    cout << "各分类支出: " << endl;
    cout << "餐饮: " << fixed << setprecision(2) << calculateCategoryAmount(records, Category::FOOD, startDate, endDate) << " 元" << endl;
    cout << "学习: " << fixed << setprecision(2) << calculateCategoryAmount(records, Category::STUDY, startDate, endDate) << " 元" << endl;
    cout << "娱乐: " << fixed << setprecision(2) << calculateCategoryAmount(records, Category::ENTERTAINMENT, startDate, endDate) << " 元" << endl;
    cout << "交通: " << fixed << setprecision(2) << calculateCategoryAmount(records, Category::TRANSPORT, startDate, endDate) << " 元" << endl;
}

int main() {
    // 设置控制台编码为UTF-8
    setConsoleUTF8();
    
    vector<Record> records;
    Budget budget = {1500.0, 500.0, 300.0, 300.0, 200.0}; // 默认预算设置
    
    // 加载已保存的记录
    records = loadRecordsFromFile("records.dat");
    
    string input;
    int choice;
    do {
        showMenu();
        getline(cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        choice = stoi(input);
        
        switch (choice) {
            case 1:
                addRecord(records);
                break;
            case 2:
                viewRecords(records);
                break;
            case 3:
                editRecord(records);
                break;
            case 4:
                searchRecords(records);
                break;
            case 5:
                statisticsMenu(records, budget);
                break;
            case 6:
                manageBudget(budget);
                break;
            case 7:
                saveRecordsToFile(records, "records.dat");
                cout << "\n💾 记录已保存！" << endl;
                break;
            case 8:
                {
                    cout << "\n┌─────────────────────────────────────────┐" << endl;
                    cout << "│              � 导出为表格             │" << endl;
                    cout << "└─────────────────────────────────────────┘" << endl;
                    string filename;
                    cout << "📁 请输入要导出的txt文件名(包含.txt后缀): ";
                    getline(cin, filename);
                    if (records.empty()) {
                        cout << "❌ 没有记录可以导出！" << endl;
                    } else {
                        exportRecordsToTxt(records, filename);
                        cout << "✅ 成功导出 " << records.size() << " 条记录到文件: " << filename << endl;
                    }
                }
                break;
            case 0:
                saveRecordsToFile(records, "records.dat");
                cout << "\n┌─────────────────────────────────────────┐" << endl;
                cout << "│  🎉 感谢使用智能记账本！再见！          │" << endl;
                cout << "└─────────────────────────────────────────┘" << endl;
                break;
            default:
                cout << "\n❌ 无效的选择，请重新输入！" << endl;
        }
        
        if (choice != 0) {
            cout << "按回车键继续...";
            cin.get();
        }
        
    } while (choice != 0);
    
    return 0;
}