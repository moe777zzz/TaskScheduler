#include "BackupTask.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <windows.h>

namespace fs = std::filesystem;

BackupTask::BackupTask(const std::string& source, const std::string& dest)
    : sourcePath(source), destPath(dest) {
}

std::string BackupTask::GetName() const {
    return "文件备份任务";
}

std::string BackupTask::GenerateTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return oss.str();
}

void BackupTask::Execute() {
    // 检查源目录是否存在
    if (!fs::exists(sourcePath)) {
        throw std::runtime_error("源目录不存在: " + sourcePath);
    }

    // 创建目标目录
    if (!fs::exists(destPath)) {
        fs::create_directories(destPath);
    }

    // 生成备份文件名
    std::string timestamp = GenerateTimestamp();
    std::string zipFilename = "backup_" + timestamp + ".zip";
    std::string zipPath = fs::path(destPath) / zipFilename;

    // 在实际项目中，这里应该调用压缩库（如zlib）或系统命令
    // 这里简化为创建标记文件
    std::ofstream markerFile(zipPath);
    if (!markerFile) {
        throw std::runtime_error("无法创建备份文件: " + zipPath);
    }

    markerFile << "备份时间: " << timestamp << std::endl;
    markerFile << "源目录: " << sourcePath << std::endl;
    markerFile << "目标目录: " << destPath << std::endl;
    markerFile.close();

    std::cout << "备份完成: " << zipPath << std::endl;
}