#include "finance_manager.h"

// 设置控制台编码为UTF-8
void setConsoleUTF8() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

// 获取当前日期 (YYYY-MM-DD)
std::string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    std::stringstream ss;
    ss << 1900 + ltm->tm_year << "-" 
       << std::setw(2) << std::setfill('0') << 1 + ltm->tm_mon << "-" 
       << std::setw(2) << std::setfill('0') << ltm->tm_mday;
    
    return ss.str();
}

// 验证日期格式是否正确
bool isValidDate(const std::string& date) {
    if (date.length() != 10 || date[4] != '-' || date[7] != '-') {
        return false;
    }
    
    int year, month, day;
    std::stringstream ss(date);
    char dash1, dash2;
    
    if (!(ss >> year >> dash1 >> month >> dash2 >> day)) {
        return false;
    }
    
    if (year < 2000 || year > 2100 || month < 1 || month > 12 || day < 1) {
        return false;
    }
    
    // 检查每月天数
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // 处理闰年2月
    if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeapYear ? 29 : 28)) {
            return false;
        }
    } else {
        if (day > daysInMonth[month - 1]) {
            return false;
        }
    }
    
    return true;
}

// 分类枚举转字符串
std::string categoryToString(Category category) {
    switch (category) {
        // 支出分类
        case Category::FOOD:
            return "餐饮";
        case Category::STUDY:
            return "学习";
        case Category::ENTERTAINMENT:
            return "娱乐";
        case Category::TRANSPORT:
            return "交通";
        // 收入分类
        case Category::LIVING_ALLOWANCE:
            return "生活费";
        case Category::PART_TIME:
            return "兼职";
        default:
            return "未知";
    }
}

// 字符串转分类枚举
Category stringToCategory(const std::string& categoryStr) {
    if (categoryStr == "餐饮" || categoryStr == "food" || categoryStr == "FOOD") {
        return Category::FOOD;
    } else if (categoryStr == "学习" || categoryStr == "study" || categoryStr == "STUDY") {
        return Category::STUDY;
    } else if (categoryStr == "娱乐" || categoryStr == "entertainment" || categoryStr == "ENTERTAINMENT") {
        return Category::ENTERTAINMENT;
    } else if (categoryStr == "交通" || categoryStr == "transport" || categoryStr == "TRANSPORT") {
        return Category::TRANSPORT;
    } else if (categoryStr == "生活费" || categoryStr == "living" || categoryStr == "LIVING") {
        return Category::LIVING_ALLOWANCE;
    } else if (categoryStr == "兼职" || categoryStr == "part" || categoryStr == "PART") {
        return Category::PART_TIME;
    } else {
        return Category::FOOD; // 默认分类为餐饮
    }
}

// 字符串转收支类型枚举
RecordType stringToRecordType(const std::string& typeStr) {
    if (typeStr == "收入" || typeStr == "income" || typeStr == "INCOME") {
        return RecordType::INCOME;
    } else {
        return RecordType::EXPENSE;
    }
}

// 收支类型枚举转字符串
std::string recordTypeToString(RecordType type) {
    return (type == RecordType::INCOME) ? "收入" : "支出";
}
