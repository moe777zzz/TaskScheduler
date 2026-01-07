#include "HttpGetTask.h"
#include <iostream>
#include <fstream>
#include <sstream>

HttpGetTask::HttpGetTask(const std::string& url) : url(url) {}

HttpGetTask::~HttpGetTask() {
    // 清理WinHTTP资源
}

std::wstring HttpGetTask::StringToWString(const std::string& str) {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

bool HttpGetTask::DownloadUrl(const std::string& url, std::string& content) {
    // 使用WinHTTP下载URL内容
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;

    std::wstring wurl = StringToWString(url);

    // 解析URL
    URL_COMPONENTS urlComp;
    ZeroMemory(&urlComp, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);
    urlComp.dwHostNameLength = (DWORD)-1;
    urlComp.dwUrlPathLength = (DWORD)-1;
    urlComp.dwExtraInfoLength = (DWORD)-1;

    if (!WinHttpCrackUrl(wurl.c_str(), (DWORD)wurl.length(), 0, &urlComp)) {
        return false;
    }

    // 获取主机名和路径
    std::wstring hostName(urlComp.lpszHostName, urlComp.dwHostNameLength);
    std::wstring urlPath(urlComp.lpszUrlPath, urlComp.dwUrlPathLength);

    // 创建会话
    hSession = WinHttpOpen(L"TaskScheduler/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) return false;

    // 创建连接
    hConnect = WinHttpConnect(hSession, hostName.c_str(),
        urlComp.nPort, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 创建请求
    hRequest = WinHttpOpenRequest(hConnect, L"GET", urlPath.c_str(),
        NULL, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES,
        urlComp.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 发送请求
    bResults = WinHttpSendRequest(hRequest,
        WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0,
        0, 0);
    if (!bResults) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 接收响应
    bResults = WinHttpReceiveResponse(hRequest, NULL);
    if (!bResults) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // 读取响应数据
    std::stringstream responseStream;
    do {
        // 检查数据可用性
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            break;

        if (dwSize == 0)
            break;

        // 分配缓冲区
        pszOutBuffer = new char[dwSize + 1];
        if (!pszOutBuffer) {
            break;
        }

        // 读取数据
        ZeroMemory(pszOutBuffer, dwSize + 1);
        if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
            dwSize, &dwDownloaded)) {
            delete[] pszOutBuffer;
            break;
        }

        responseStream.write(pszOutBuffer, dwDownloaded);
        delete[] pszOutBuffer;

    } while (dwSize > 0);

    content = responseStream.str();

    // 清理
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);

    return bResults;
}

std::string HttpGetTask::GetName() const {
    return "HTTP GET任务: " + url;
}

void HttpGetTask::Execute() {
    std::string content;

    if (DownloadUrl(url, content)) {
        // 写入文件
        std::ofstream outFile("zen.txt");
        if (outFile) {
            outFile << content;
            outFile.close();
            std::cout << "HTTP响应已保存到 zen.txt" << std::endl;
        }
        else {
            throw std::runtime_error("无法写入文件 zen.txt");
        }
    }
    else {
        throw std::runtime_error("HTTP请求失败");
    }
}