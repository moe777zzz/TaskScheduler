#pragma once
#include <string>
#include <fstream>
#include <mutex>

class LogWriter {
public:
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    explicit LogWriter(const std::string& filename, LogLevel level = LogLevel::INFO);
    ~LogWriter();

    // 禁用拷贝构造和赋值
    LogWriter(const LogWriter&) = delete;
    LogWriter& operator=(const LogWriter&) = delete;

    // 允许移动
    LogWriter(LogWriter&&) noexcept;
    LogWriter& operator=(LogWriter&&) noexcept;

    void Write(const std::string& message, LogLevel level = LogLevel::INFO);
    void SetMinLogLevel(LogLevel level);

private:
    std::string filename;
    std::ofstream logFile;
    LogLevel minLogLevel;
    static std::mutex logMutex;

    bool OpenFile();
    void CloseFile();
    std::string GetCurrentTime();
    std::string LevelToString(LogLevel level);
}; #pragma once
