#ifndef AI_ANALYSIS_H
#define AI_ANALYSIS_H

#include <vector>
#include <string>
#include "finance_manager.h"

#ifdef _WIN32
#include <windows.h>
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
#endif

struct AIAnalysisResult {
    std::string summary;
    std::vector<std::string> suggestions;
    std::string insights;
};

struct SparkConfig {
    std::string apiUrl;
    std::string apiPassword;
    std::string model;
    int timeout;
    
    SparkConfig() : apiUrl("https://spark-api-open.xf-yun.com/v1/chat/completions"),
                    apiPassword(""),
                    model("lite"),
                    timeout(10000) {}
};

AIAnalysisResult generateAIAnalysis(const std::vector<Record>& records,
                                   const std::string& startDate,
                                   const std::string& endDate);

std::string callAIModel(const std::string& prompt);

std::string callSparkAPI(const std::string& prompt, const SparkConfig& config);

std::string generateHumorousPrompt(const std::vector<Record>& records,
                                  const std::string& startDate,
                                  const std::string& endDate);

std::string parseAIResponse(const std::string& response);

std::string generateMockResponse(const std::vector<Record>& records,
                                const std::string& startDate,
                                const std::string& endDate);

bool loadAPIKey(std::string& apiKey);

#endif // AI_ANALYSIS_H
