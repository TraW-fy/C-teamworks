#include "ai_analysis.h"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <algorithm>
#include "finance_manager.h"
#include "json.hpp"

using json = nlohmann::json;

static std::string currentApiKey = "";

bool loadAPIKey(std::string& apiKey) {
    const char* envKey = std::getenv("SPARK_API_KEY");
    if (envKey && strlen(envKey) > 0) {
        apiKey = envKey;
        currentApiKey = apiKey;
        return true;
    }
    
    std::ifstream configFile("config.txt");
    if (configFile.is_open()) {
        std::string line;
        while (std::getline(configFile, line)) {
            if (line.find("SPARK_API_KEY=") == 0) {
                apiKey = line.substr(14);
                currentApiKey = apiKey;
                configFile.close();
                return true;
            }
        }
        configFile.close();
    }
    
    return false;
}

std::string callSparkAPI(const std::string& prompt, const SparkConfig& config) {
    if (config.apiPassword.empty() && !loadAPIKey(const_cast<SparkConfig&>(config).apiPassword)) {
        std::cerr << "[AI] 未找到API密钥，将使用模拟回复" << std::endl;
        return "";
    }
    
#ifdef _WIN32
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = nullptr;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = nullptr, hConnect = nullptr, hRequest = nullptr;
    
    hSession = WinHttpOpen(L"FinanceManager/1.0", 
                           WINHTTP_ACCESS_TYPE_NO_PROXY,
                           WINHTTP_NO_PROXY_NAME, 
                           WINHTTP_NO_PROXY_BYPASS, 
                           0);
    
    if (!hSession) {
        std::cerr << "[AI] WinHttpOpen失败，错误码: " << GetLastError() << std::endl;
        return "";
    }
    
    std::wstring host = L"spark-api-open.xf-yun.com";
    hConnect = WinHttpConnect(hSession, host.c_str(), 443, 0);
    if (!hConnect) {
        std::cerr << "[AI] WinHttpConnect失败，错误码: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    std::wstring path = L"/v1/chat/completions";
    hRequest = WinHttpOpenRequest(hConnect, L"POST", path.c_str(),
                                  nullptr, WINHTTP_NO_REFERER,
                                  WINHTTP_DEFAULT_ACCEPT_TYPES,
                                  WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        std::cerr << "[AI] WinHttpOpenRequest失败，错误码: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    std::string authHeader = "Bearer " + config.apiPassword;
    std::wstring wAuthHeader = L"Authorization: " + std::wstring(authHeader.begin(), authHeader.end());
    bResults = WinHttpAddRequestHeaders(hRequest, 
                                        wAuthHeader.c_str(), 
                                        (DWORD)wAuthHeader.length(), 
                                        WINHTTP_ADDREQ_FLAG_ADD);
    
    if (!bResults) {
        std::cerr << "[AI] 设置请求头失败，错误码: " << GetLastError() << std::endl;
    }
    
    json requestBody = {
        {"model", config.model},
        {"messages", {
            {{"role", "user"}, {"content", prompt}}
        }},
        {"temperature", 0.7},
        {"max_tokens", 1024}
    };
    
    std::string requestStr = requestBody.dump();
    const BYTE* lpBytes = (const BYTE*)requestStr.c_str();
    
    bResults = WinHttpSendRequest(hRequest,
                                  L"Content-Type: application/json\r\n",
                                  (DWORD)-1,
                                  (LPVOID)lpBytes,
                                  (DWORD)requestStr.length(),
                                  (DWORD)requestStr.length(),
                                  0);
    
    if (!bResults) {
        std::cerr << "[AI] WinHttpSendRequest失败，错误码: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    bResults = WinHttpReceiveResponse(hRequest, nullptr);
    if (!bResults) {
        std::cerr << "[AI] WinHttpReceiveResponse失败，错误码: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    dwSize = 0;
    if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
        std::cerr << "[AI] WinHttpQueryDataAvailable失败，错误码: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    pszOutBuffer = new char[dwSize + 1];
    if (!pszOutBuffer) {
        std::cerr << "[AI] 内存分配失败" << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    ZeroMemory(pszOutBuffer, dwSize + 1);
    if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
        std::cerr << "[AI] WinHttpReadData失败，错误码: " << GetLastError() << std::endl;
        delete[] pszOutBuffer;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return "";
    }
    
    std::string response(pszOutBuffer);
    delete[] pszOutBuffer;
    
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    
    return response;
#else
    return "";
#endif
}

std::string parseAIResponse(const std::string& response) {
    if (response.empty()) return "";
    
    try {
        json resp = json::parse(response);
        if (resp.contains("choices") && resp["choices"].is_array() && !resp["choices"].empty()) {
            return resp["choices"][0]["message"]["content"];
        }
        if (resp.contains("error")) {
            std::cerr << "[AI] API错误: " << resp["error"]["message"] << std::endl;
            return "";
        }
    } catch (const std::exception& e) {
        std::cerr << "[AI] JSON解析错误: " << e.what() << std::endl;
        return "";
    }
    return "";
}

std::string generateHumorousPrompt(const std::vector<Record>& records,
                                  const std::string& startDate,
                                  const std::string& endDate) {
    double totalExpense = calculateTotalAmount(records, RecordType::EXPENSE, startDate, endDate);
    double totalIncome = calculateTotalAmount(records, RecordType::INCOME, startDate, endDate);
    
    double food = calculateCategoryAmount(records, Category::FOOD, startDate, endDate);
    double study = calculateCategoryAmount(records, Category::STUDY, startDate, endDate);
    double entertainment = calculateCategoryAmount(records, Category::ENTERTAINMENT, startDate, endDate);
    double transport = calculateCategoryAmount(records, Category::TRANSPORT, startDate, endDate);
    
    int recordCount = records.size();
    double balance = totalIncome - totalExpense;
    
    std::stringstream prompt;
    prompt << "你是一个幽默风趣的财务管家，请用轻松搞笑的方式分析以下消费数据并给出建议：\n\n";
    prompt << "【消费数据概览】\n";
    prompt << "- 时间范围: " << (startDate.empty() ? "所有时间" : startDate) 
           << " 至 " << (endDate.empty() ? "所有时间" : endDate) << "\n";
    prompt << "- 总收入: " << std::fixed << std::setprecision(2) << totalIncome << " 元\n";
    prompt << "- 总支出: " << std::fixed << std::setprecision(2) << totalExpense << " 元\n";
    prompt << "- 结余: " << std::fixed << std::setprecision(2) << balance << " 元\n";
    prompt << "- 记录数: " << recordCount << " 条\n\n";
    
    prompt << "【分类支出明细】\n";
    if (totalExpense > 0) {
        double foodPct = (food / totalExpense) * 100;
        double studyPct = (study / totalExpense) * 100;
        double entPct = (entertainment / totalExpense) * 100;
        double transPct = (transport / totalExpense) * 100;
        
        prompt << "- 餐饮: " << food << " 元 (" << std::fixed << std::setprecision(1) << foodPct << "%)\n";
        prompt << "- 学习: " << study << " 元 (" << std::fixed << std::setprecision(1) << studyPct << "%)\n";
        prompt << "- 娱乐: " << entertainment << " 元 (" << std::fixed << std::setprecision(1) << entPct << "%)\n";
        prompt << "- 交通: " << transport << " 元 (" << std::fixed << std::setprecision(1) << transPct << "%)\n";
    } else {
        prompt << "暂无支出记录，你的钱包正在休息中~\n";
    }
    
    prompt << "\n【分析要求】\n";
    prompt << "1. 用2-3个有趣又犀利的观点总结用户的消费特点（要带点吐槽风格但不要太过分）\n";
    prompt << "2. 针对支出最多的1-2个类别，给出3条幽默风趣但实用的省钱建议\n";
    prompt << "3. 用轻松的语气预测下个月的消费趋势（可以开玩笑）\n";
    prompt << "4. 最后来一句鼓励或调侃的话\n\n";
    prompt << "【回复格式】请用流畅的中文段落回复，不需要markdown格式，直接以'嘿，朋友！'或类似的轻松开场开始，用轻松的语气聊天就像在跟朋友吐槽一样~";
    
    return prompt.str();
}

std::string generateMockResponse(const std::vector<Record>& records,
                                const std::string& startDate,
                                const std::string& endDate) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 10);
    int seed = dist(rng);
    
    double totalExpense = calculateTotalAmount(records, RecordType::EXPENSE, startDate, endDate);
    double totalIncome = calculateTotalAmount(records, RecordType::INCOME, startDate, endDate);
    double balance = totalIncome - totalExpense;
    
    double food = calculateCategoryAmount(records, Category::FOOD, startDate, endDate);
    double study = calculateCategoryAmount(records, Category::STUDY, startDate, endDate);
    double entertainment = calculateCategoryAmount(records, Category::ENTERTAINMENT, startDate, endDate);
    double transport = calculateCategoryAmount(records, Category::TRANSPORT, startDate, endDate);
    
    std::string topCategory = "餐饮";
    double maxAmount = food;
    if (entertainment > maxAmount) { topCategory = "娱乐"; maxAmount = entertainment; }
    if (transport > maxAmount) { topCategory = "交通"; maxAmount = transport; }
    if (study > maxAmount) { topCategory = "学习"; maxAmount = study; }
    
    std::stringstream response;
    response << "嘿，朋友！让我来康康你的消费情况~(๑•̀ㅂ•́)و✧\n\n";
    
    if (records.empty()) {
        response << "咦？你这是" << (startDate.empty() ? "最近" : "这段时间") << "没怎么记账吗？\n";
        response << "是不是钱花得太快不敢记了？还是变成佛系记账选手了？\n\n";
        response << "建议：打开记账本，把那些偷偷溜走的钱都抓回来！毕竟，只有了解钱去哪了，才能更好地让它们回来嘛~💪";
        return response.str();
    }
    
    response << "【消费体检报告】\n";
    response << "总支出" << std::fixed << std::setprecision(2) << totalExpense << "元，";
    if (balance >= 0) {
        response << "恭喜！你成功守住了钱包没有大出血~🎉\n";
    } else {
        response << "emmm...你这是要上演'月光族'的现实版吗？钱包君表示很受伤(๑•́₃•̀๑)\n";
    }
    
    if (maxAmount > 0) {
        response << "在" << topCategory << "方面你贡献了" << std::fixed << std::setprecision(2) << maxAmount << "元，\n";
        if (topCategory == "餐饮") {
            if (food / totalExpense > 0.5) {
                response << "你是想把天下美食都尝一遍吗？外卖小哥都认识你了吧！🍜\n";
                response << "省钱秘籍：试着下厨吧！不仅省钱，没准还能意外解锁'中华小当家'技能~🍳\n";
            } else {
                response << "吃货本质暴露无遗，但至少你吃得开心呀！民以食为天嘛~😋\n";
            }
        } else if (topCategory == "娱乐") {
            response << "娱乐这块你是真的投入！追剧、打游戏、逛吃逛吃...快乐很重要但也要悠着点哦~🎮\n";
            response << "建议：把娱乐预算设个上限，超支就去图书馆或公园免费浪~🌳\n";
        } else if (topCategory == "交通") {
            response << "你是买了月票还是准备集齐所有交通方式召唤神龙？🚗🚌🚇\n";
            response << "省钱妙招：试试骑车或者步行？既省钱又健身，一箭双雕不香吗？🚴\n";
        } else if (topCategory == "学习") {
            response << "学习投资是最值的买卖！知识就是力量，这波你在大气层！📚\n";
            response << "继续保持！说不定下一个大牛就是你~🌟\n";
        }
    }
    
    response << "\n【下月消费预警】\n";
    if (seed % 3 == 0) {
        response << "根据玄学预测，下个月你可能会在某处有意外支出...建议提前预留一笔'快乐基金'，\n";
        response << "免得到时候看着账单心跳加速~💓\n";
    } else if (seed % 3 == 1) {
        response << "下个月你的财运还不错，但也要控制住寄几！别月初像大爷，月末像孙子~😅\n";
        response << "建议：月初先存一笔，剩下再花，亲测有效！\n";
    } else {
        response << "下个月建议佛系消费，韬光养晦。你的钱包会感谢你的！🍃\n";
        response << "毕竟，省下的钱就是以后浪的资本嘛~😎\n";
    }
    
    response << "\n【最后一句】\n";
    if (balance > 0) {
        response << "你已经是理财小能手了！继续保持，下个富豪就是你！💎";
    } else {
        response << "没关系！意识到要省钱就是成功的第一步，明天开始就是新的开始！加油！💪";
    }
    
    return response.str();
}

std::string callAIModel(const std::string& prompt) {
    SparkConfig config;
    
    if (!loadAPIKey(config.apiPassword)) {
        std::cerr << "[AI] 未配置API密钥，将生成模拟回复" << std::endl;
        return "";
    }
    
    std::cout << "[AI] 正在调用讯飞Spark Lite..." << std::endl;
    
    std::string response = callSparkAPI(prompt, config);
    
    if (response.empty()) {
        std::cerr << "[AI] API调用失败，将使用模拟回复" << std::endl;
        return "";
    }
    
    std::string content = parseAIResponse(response);
    if (content.empty()) {
        std::cerr << "[AI] 解析响应失败，将使用模拟回复" << std::endl;
        return "";
    }
    
    std::cout << "[AI] 成功获取AI回复" << std::endl;
    return content;
}

AIAnalysisResult generateAIAnalysis(const std::vector<Record>& records,
                                   const std::string& startDate,
                                   const std::string& endDate) {
    AIAnalysisResult result;
    
    double totalExpense = calculateTotalAmount(records, RecordType::EXPENSE, startDate, endDate);
    double totalIncome = calculateTotalAmount(records, RecordType::INCOME, startDate, endDate);
    double balance = totalIncome - totalExpense;
    
    double food = calculateCategoryAmount(records, Category::FOOD, startDate, endDate);
    double study = calculateCategoryAmount(records, Category::STUDY, startDate, endDate);
    double entertainment = calculateCategoryAmount(records, Category::ENTERTAINMENT, startDate, endDate);
    double transport = calculateCategoryAmount(records, Category::TRANSPORT, startDate, endDate);
    
    std::stringstream summary;
    summary << "消费分析报告";
    if (!startDate.empty() || !endDate.empty()) {
        summary << "（" << (startDate.empty() ? "所有时间" : startDate) 
                << " 至 " << (endDate.empty() ? "所有时间" : endDate) << "）";
    }
    summary << "\n\n";
    summary << "📊 收支概况\n";
    summary << "├── 总收入: " << std::fixed << std::setprecision(2) << totalIncome << " 元\n";
    summary << "├── 总支出: " << std::fixed << std::setprecision(2) << totalExpense << " 元\n";
    summary << "└── 结余: " << std::fixed << std::setprecision(2) << balance << " 元\n\n";
    
    summary << "📈 分类支出\n";
    std::map<std::string, double> categories = {
        {"餐饮", food}, {"学习", study}, {"娱乐", entertainment}, {"交通", transport}
    };
    for (const auto& cat : categories) {
        double pct = totalExpense > 0 ? (cat.second / totalExpense) * 100 : 0;
        summary << "├── " << cat.first << ": " << std::fixed << std::setprecision(2) 
                << cat.second << " 元 (" << std::fixed << std::setprecision(1) << pct << "%)\n";
    }
    
    result.summary = summary.str();
    
    std::string humorousPrompt = generateHumorousPrompt(records, startDate, endDate);
    std::string aiResponse = callAIModel(humorousPrompt);
    
    if (aiResponse.empty()) {
        std::cout << "[AI] 使用模拟回复模式" << std::endl;
        aiResponse = generateMockResponse(records, startDate, endDate);
    }
    
    std::stringstream insights;
    insights << "🎯 AI智能分析\n\n";
    insights << aiResponse;
    result.insights = insights.str();

    result.suggestions.clear();

    return result;
}
