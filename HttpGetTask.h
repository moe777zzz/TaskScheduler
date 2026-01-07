#pragma once
#include "ITask.h"
#include <string>
#include <windows.h>
#include <winhttp.h>

class HttpGetTask : public ITask {
public:
    HttpGetTask(const std::string& url);
    ~HttpGetTask();

    std::string GetName() const override;
    void Execute() override;

private:
    std::string url;

    // HTTP相关私有方法
    bool DownloadUrl(const std::string& url, std::string& content);
    std::wstring StringToWString(const std::string& str);
}; #pragma once
