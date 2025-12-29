#include "finance_manager.h"

// 根据描述和类型自动分类
Category autoClassify(const std::string& description, RecordType type) {
    // 如果是收入类型，直接根据关键词分类
    if (type == RecordType::INCOME) {
        if (description.find("生活费") != std::string::npos || 
            description.find(" allowance") != std::string::npos ||
            description.find("生活费") != std::string::npos) {
            return Category::LIVING_ALLOWANCE;
        } else if (description.find("兼职") != std::string::npos || 
                   description.find("part-time") != std::string::npos ||
                   description.find("兼职") != std::string::npos ||
                   description.find("打工") != std::string::npos) {
            return Category::PART_TIME;
        } else {
            return Category::LIVING_ALLOWANCE; // 默认收入分类为生活费
        }
    }
    
    // 支出类型的关键词匹配
    // 餐饮类关键词
    std::vector<std::string> foodKeywords = {"食堂", "吃饭", "午餐", "晚餐", "早餐", "外卖", "奶茶", "咖啡", "餐馆", "饭店", "食物", "food", "meal", "dinner", "lunch", "breakfast", "takeout", "milk tea", "coffee"};
    for (const std::string& keyword : foodKeywords) {
        if (description.find(keyword) != std::string::npos) {
            return Category::FOOD;
        }
    }
    
    // 学习类关键词
    std::vector<std::string> studyKeywords = {"买书", "教材", "文具", "笔记本", "笔", "书店", "考试", "培训", "学习", "book", "textbook", "stationery", "notebook", "pen", "exam", "study"};
    for (const std::string& keyword : studyKeywords) {
        if (description.find(keyword) != std::string::npos) {
            return Category::STUDY;
        }
    }
    
    // 娱乐类关键词
    std::vector<std::string> entertainmentKeywords = {"电影", "游戏", "娱乐", "游玩", "旅游", "聚会", "唱歌", "电影票", "game", "movie", "entertainment", "travel", "party", "sing"};
    for (const std::string& keyword : entertainmentKeywords) {
        if (description.find(keyword) != std::string::npos) {
            return Category::ENTERTAINMENT;
        }
    }
    
    // 交通类关键词
    std::vector<std::string> transportKeywords = {"公交", "地铁", "打车", "出租车", "车票", "交通", "bus", "subway", "taxi", "ticket", "transport"};
    for (const std::string& keyword : transportKeywords) {
        if (description.find(keyword) != std::string::npos) {
            return Category::TRANSPORT;
        }
    }
    
    // 默认分类为餐饮
    return Category::FOOD;
}
