#include "finance_manager.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

// 检查预算预警
void checkBudgetAlert(const vector<Record>& records, const Budget& budget, const string& month) {
    string startDate = month + "-01";
    string endDate = month;
    
    // 获取当月最后一天
    int year = stoi(startDate.substr(0, 4));
    string monthStr = startDate.substr(5, 2);
    int monthNum = stoi(monthStr);
    int lastDay = 31;
    
    // 设置小月的最后一天
    if (monthNum == 4 || monthNum == 6 || monthNum == 9 || monthNum == 11) {
        lastDay = 30;
    }
    
    // 处理闰年2月
    if (monthNum == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeapYear) {
            lastDay = 29;
        } else {
            lastDay = 28;
        }
    }
    
    stringstream ss;
    ss << month << "-" << setw(2) << setfill('0') << lastDay;
    endDate = ss.str();
    
    // 计算各分类当月支出
    double foodAmount = calculateCategoryAmount(records, Category::FOOD, startDate, endDate);
    double studyAmount = calculateCategoryAmount(records, Category::STUDY, startDate, endDate);
    double entertainmentAmount = calculateCategoryAmount(records, Category::ENTERTAINMENT, startDate, endDate);
    double transportAmount = calculateCategoryAmount(records, Category::TRANSPORT, startDate, endDate);
    double totalExpense = foodAmount + studyAmount + entertainmentAmount + transportAmount;
    
    // 检查预算预警
    cout << "\n=== 预算预警 ===" << endl;
    cout << "当月总预算: " << fixed << setprecision(2) << budget.totalBudget << " 元" << endl;
    cout << "当月总支出: " << fixed << setprecision(2) << totalExpense << " 元" << endl;
    cout << "-------------------------------" << endl;
    
    // 检查总预算
    if (totalExpense >= budget.totalBudget * 0.8) {
        cout << "⚠️  警告：当月总支出已达到总预算的 " << fixed << setprecision(1) << (totalExpense / budget.totalBudget * 100) << "%！" << endl;
    }
    
    // 检查各分类预算
    if (budget.foodBudget > 0 && foodAmount >= budget.foodBudget * 0.8) {
        cout << "⚠️  警告：餐饮支出已达到预算的 " << fixed << setprecision(1) << (foodAmount / budget.foodBudget * 100) << "%！" << endl;
    }
    
    if (budget.studyBudget > 0 && studyAmount >= budget.studyBudget * 0.8) {
        cout << "⚠️  警告：学习支出已达到预算的 " << fixed << setprecision(1) << (studyAmount / budget.studyBudget * 100) << "%！" << endl;
    }
    
    if (budget.entertainmentBudget > 0 && entertainmentAmount >= budget.entertainmentBudget * 0.8) {
        cout << "⚠️  警告：娱乐支出已达到预算的 " << fixed << setprecision(1) << (entertainmentAmount / budget.entertainmentBudget * 100) << "%！" << endl;
    }
    
    if (budget.transportBudget > 0 && transportAmount >= budget.transportBudget * 0.8) {
        cout << "⚠️  警告：交通支出已达到预算的 " << fixed << setprecision(1) << (transportAmount / budget.transportBudget * 100) << "%！" << endl;
    }
    
    cout << "-------------------------------" << endl;
}

// 管理预算
void manageBudget(Budget& budget) {
    system("cls");
    cout << "=========================================" << endl;
    cout << "             预算管理                     " << endl;
    cout << "=========================================" << endl;
    cout << "当前预算设置：" << endl;
    cout << "总预算: " << fixed << setprecision(2) << budget.totalBudget << " 元" << endl;
    cout << "餐饮预算: " << fixed << setprecision(2) << budget.foodBudget << " 元" << endl;
    cout << "学习预算: " << fixed << setprecision(2) << budget.studyBudget << " 元" << endl;
    cout << "娱乐预算: " << fixed << setprecision(2) << budget.entertainmentBudget << " 元" << endl;
    cout << "交通预算: " << fixed << setprecision(2) << budget.transportBudget << " 元" << endl;
    cout << "=========================================" << endl;
    
    string choice;
    cout << "是否修改预算？(y/n): ";
    getline(cin, choice);
    
    if (choice == "y" || choice == "Y") {
        string totalBudgetStr;
        cout << "请输入总预算: ";
        getline(cin, totalBudgetStr);
        budget.totalBudget = stod(totalBudgetStr);
        
        string foodBudgetStr;
        cout << "请输入餐饮预算: ";
        getline(cin, foodBudgetStr);
        budget.foodBudget = stod(foodBudgetStr);
        
        string studyBudgetStr;
        cout << "请输入学习预算: ";
        getline(cin, studyBudgetStr);
        budget.studyBudget = stod(studyBudgetStr);
        
        string entertainmentBudgetStr;
        cout << "请输入娱乐预算: ";
        getline(cin, entertainmentBudgetStr);
        budget.entertainmentBudget = stod(entertainmentBudgetStr);
        
        string transportBudgetStr;
        cout << "请输入交通预算: ";
        getline(cin, transportBudgetStr);
        budget.transportBudget = stod(transportBudgetStr);
        
        cout << "预算已更新！" << endl;
    }
    
    cout << "按任意键返回主菜单...";
    getline(cin, choice);
}