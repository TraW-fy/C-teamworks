#include "finance_manager.h"

// 保存记录到文件
void saveRecordsToFile(const std::vector<Record>& records, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件 " << filename << " 进行保存！" << std::endl;
        return;
    }
    
    for (const Record& record : records) {
        file << record.id << "|" 
             << (record.type == RecordType::INCOME ? "收入" : "支出") << "|" 
             << categoryToString(record.category) << "|" 
             << std::fixed << std::setprecision(2) << record.amount << "|" 
             << record.date << "|" 
             << record.description << std::endl;
    }
    
    file.close();
}

// 从文件加载记录
std::vector<Record> loadRecordsFromFile(const std::string& filename) {
    std::vector<Record> records;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        // 文件不存在，返回空向量
        return records;
    }
    
    std::string line;
    while (getline(file, line)) {
        Record record;
        std::stringstream ss(line);
        std::string field;
        
        // 读取ID
        getline(ss, field, '|');
        record.id = std::stoi(field);
        
        // 读取类型
        getline(ss, field, '|');
        record.type = stringToRecordType(field);
        
        // 读取分类
        getline(ss, field, '|');
        record.category = stringToCategory(field);
        
        // 读取金额
        getline(ss, field, '|');
        record.amount = std::stod(field);
        
        // 读取日期
        getline(ss, field, '|');
        record.date = field;
        
        // 读取备注
        getline(ss, field, '|');
        record.description = field;
        
        records.push_back(record);
    }
    
    file.close();
    return records;
}

// 从TXT文件导入记录
std::vector<Record> importRecordsFromTxt(const std::string& filename) {
    std::vector<Record> records;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "无法打开文件 " << filename << " 进行导入！" << std::endl;
        return records;
    }
    
    std::string line;
    int lineNum = 0;
    while (getline(file, line)) {
        lineNum++;
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> fields;
        
        // 按逗号分隔字段
        while (getline(ss, field, ',')) {
            // 去除首尾空格
            field.erase(0, field.find_first_not_of(" \t"));
            field.erase(field.find_last_not_of(" \t") + 1);
            fields.push_back(field);
        }
        
        // 检查字段数量
        if (fields.size() != 4) {
            std::cout << "第 " << lineNum << " 行格式错误，跳过导入！" << std::endl;
            continue;
        }
        
        Record record;
        
        // 设置默认ID（实际导入时会重新分配）
        record.id = 0;
        
        // 读取类型
        record.type = stringToRecordType(fields[0]);
        
        // 读取金额
        try {
            record.amount = std::stod(fields[1]);
        } catch (const std::exception& e) {
            std::cout << "第 " << lineNum << " 行金额格式错误，跳过导入！" << std::endl;
            continue;
        }
        
        // 读取日期
        if (isValidDate(fields[2])) {
            record.date = fields[2];
        } else {
            // 日期格式错误，使用当前日期
            record.date = getCurrentDate();
            std::cout << "第 " << lineNum << " 行日期格式错误，使用当前日期替代！" << std::endl;
        }
        
        // 读取备注并自动分类
        record.description = fields[3];
        record.category = autoClassify(record.description, record.type);
        
        records.push_back(record);
    }
    
    file.close();
    return records;
}
