#include "ReminderTask.h"
#include <windows.h>
#include <iostream>

ReminderTask::ReminderTask(const std::string& message) : message(message) {}

std::string ReminderTask::GetName() const {
    return "课堂提醒任务: " + message;
}

void ReminderTask::ShowReminderDialog(const std::string& message) {
    // 转换字符串到宽字符
    int len = MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, message.c_str(), -1, wstr, len);

    // 显示消息框
    MessageBoxW(NULL, wstr, L"课堂提醒", MB_OK | MB_ICONINFORMATION | MB_SETFOREGROUND);

    delete[] wstr;
}

void ReminderTask::Execute() {
    std::cout << "显示提醒: " << message << std::endl;
    ShowReminderDialog(message);
}