#include "LogWriter.h"
#include <iostream>
#include <chrono>
#include <iomanip>

std::mutex LogWriter::logMutex;

LogWriter::LogWriter(const std::string& filename, LogLevel level)
    : filename(filename), minLogLevel(level) {
    OpenFile();
}

LogWriter::~LogWriter() {
    CloseFile();
}

LogWriter::LogWriter(LogWriter&& other) noexcept
    : filename(std::move(other.filename))
    , minLogLevel(other.minLogLevel) {
    logFile = std::move(other.logFile);
}

LogWriter& LogWriter::operator=(LogWriter&& other) noexcept {
    if (this != &other) {
        CloseFile();
        filename = std::move(other.filename);
        minLogLevel = other.minLogLevel;
        logFile = std::move(other.logFile);
    }
    return *this;
}

bool LogWriter::OpenFile() {
    std::lock_guard<std::mutex> lock(logMutex);
    logFile.open(filename, std::ios::out | std::ios::app);
    return logFile.is_open();
}

void LogWriter::CloseFile() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
}

std::string LogWriter::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // 获取毫秒
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;

    std::tm tm;
    localtime_s(&tm, &time);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string LogWriter::LevelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:    return "DEBUG";
    case LogLevel::INFO:     return "INFO";
    case LogLevel::WARNING:  return "WARNING";
    case LogLevel::ERROR:    return "ERROR";
    case LogLevel::CRITICAL: return "CRITICAL";
    default:                 return "UNKNOWN";
    }
}

void LogWriter::Write(const std::string& message, LogLevel level) {
    if (level < minLogLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(logMutex);
    if (!logFile.is_open()) {
        std::cerr << "日志文件未打开: " << filename << std::endl;
        return;
    }

    std::string timestamp = GetCurrentTime();
    std::string levelStr = LevelToString(level);

    logFile << "[" << timestamp << "] "
        << "[" << levelStr << "] "
        << message << std::endl;

    // 确保立即写入
    logFile.flush();

    // 同时在控制台输出（可选）
    std::cout << "[" << timestamp << "] "
        << "[" << levelStr << "] "
        << message << std::endl;
}

void LogWriter::SetMinLogLevel(LogLevel level) {
    minLogLevel = level;
}