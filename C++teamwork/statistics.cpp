#include "finance_manager.h"

// 计算总金额
double calculateTotalAmount(const std::vector<Record>& records, 
                           RecordType type, 
                           const std::string& startDate, 
                           const std::string& endDate) {
    double total = 0.0;
    
    for (const Record& record : records) {
        // 检查收支类型
        if (record.type != type) {
            continue;
        }
        
        // 检查日期范围
        if (!startDate.empty() && record.date < startDate) {
            continue;
        }
        if (!endDate.empty() && record.date > endDate) {
            continue;
        }
        
        total += record.amount;
    }
    
    return total;
}

// 计算某分类的总金额
double calculateCategoryAmount(const std::vector<Record>& records, 
                              Category category, 
                              const std::string& startDate, 
                              const std::string& endDate) {
    double total = 0.0;
    
    for (const Record& record : records) {
        // 只计算支出
        if (record.type != RecordType::EXPENSE) {
            continue;
        }
        
        // 检查分类
        if (record.category != category) {
            continue;
        }
        
        // 检查日期范围
        if (!startDate.empty() && record.date < startDate) {
            continue;
        }
        if (!endDate.empty() && record.date > endDate) {
            continue;
        }
        
        total += record.amount;
    }
    
    return total;
}

// 打印支出分布（包含字符饼图）
void printExpenseDistribution(const std::vector<Record>& records, 
                             const std::string& startDate, 
                             const std::string& endDate) {
    // 计算总支出
    double totalExpense = calculateTotalAmount(records, RecordType::EXPENSE, startDate, endDate);
    
    if (totalExpense <= 0) {
        std::cout << "该时间段内没有支出记录！" << std::endl;
        return;
    }
    
    // 计算各分类支出
    double foodAmount = calculateCategoryAmount(records, Category::FOOD, startDate, endDate);
    double studyAmount = calculateCategoryAmount(records, Category::STUDY, startDate, endDate);
    double entertainmentAmount = calculateCategoryAmount(records, Category::ENTERTAINMENT, startDate, endDate);
    double transportAmount = calculateCategoryAmount(records, Category::TRANSPORT, startDate, endDate);
    
    // 计算各分类占比
    double foodPercent = (foodAmount / totalExpense) * 100;
    double studyPercent = (studyAmount / totalExpense) * 100;
    double entertainmentPercent = (entertainmentAmount / totalExpense) * 100;
    double transportPercent = (transportAmount / totalExpense) * 100;
    
    // 打印消费分布
    std::cout << "\n=== 消费分布 ===" << std::endl;
    std::cout << "总支出: " << std::fixed << std::setprecision(2) << totalExpense << " 元" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    std::cout << "餐饮: " << std::fixed << std::setprecision(2) << foodAmount << " 元 (" << std::fixed << std::setprecision(1) << foodPercent << "%)" << std::endl;
    std::cout << "学习: " << std::fixed << std::setprecision(2) << studyAmount << " 元 (" << std::fixed << std::setprecision(1) << studyPercent << "%)" << std::endl;
    std::cout << "娱乐: " << std::fixed << std::setprecision(2) << entertainmentAmount << " 元 (" << std::fixed << std::setprecision(1) << entertainmentPercent << "%)" << std::endl;
    std::cout << "交通: " << std::fixed << std::setprecision(2) << transportAmount << " 元 (" << std::fixed << std::setprecision(1) << transportPercent << "%)" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    
    // 打印字符饼图
    std::cout << "\n=== 消费占比饼图 ===" << std::endl;
    const int totalChars = 50; // 饼图总字符数
    
    // 计算各分类的字符数
    int foodChars = static_cast<int>((foodPercent / 100) * totalChars);
    int studyChars = static_cast<int>((studyPercent / 100) * totalChars);
    int entertainmentChars = static_cast<int>((entertainmentPercent / 100) * totalChars);
    int transportChars = totalChars - foodChars - studyChars - entertainmentChars;
    
    // 打印饼图
    std::cout << "餐饮 [";
    for (int i = 0; i < foodChars; i++) {
        std::cout << "█";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << foodPercent << "%" << std::endl;
    
    std::cout << "学习 [";
    for (int i = 0; i < studyChars; i++) {
        std::cout << "█";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << studyPercent << "%" << std::endl;
    
    std::cout << "娱乐 [";
    for (int i = 0; i < entertainmentChars; i++) {
        std::cout << "█";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << entertainmentPercent << "%" << std::endl;
    
    std::cout << "交通 [";
    for (int i = 0; i < transportChars; i++) {
        std::cout << "█";
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << transportPercent << "%" << std::endl;
}
