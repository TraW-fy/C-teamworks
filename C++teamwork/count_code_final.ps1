# 最终版代码统计脚本 - 简化语法
$files = @(
    "ai_analysis.cpp", "budget.cpp", "classification.cpp", 
    "file_io.cpp", "main.cpp", "statistics.cpp", 
    "utils.cpp", "ai_analysis.h", "finance_manager.h"
)

$total = 0

foreach ($file in $files) {
    if (Test-Path $file) {
        $content = Get-Content $file
        $count = 0
        $inComment = $false
        
        foreach ($line in $content) {
            $trimmed = $line.Trim()
            
            # 空行跳过
            if ($trimmed -eq "") { continue }
            
            # 多行注释处理
            if ($inComment) {
                if ($trimmed -like "*/*") { $inComment = $false }
                continue
            }
            
            # 单行注释跳过
            if ($trimmed -like "//*") { continue }
            
            # 开始多行注释
            if ($trimmed -like "/**") {
                $inComment = $true
                continue
            }
            
            # 有效代码行
            $count++
        }
        
        Write-Host "$file : $count lines"
        $total += $count
    }
}

Write-Host ""
Write-Host "Total valid code lines: $total"