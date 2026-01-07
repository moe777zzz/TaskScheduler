#pragma once
#include "ITask.h"
#include <string>

class ReminderTask : public ITask {
public:
    ReminderTask(const std::string& message);
    std::string GetName() const override;
    void Execute() override;

private:
    std::string message;
    void ShowReminderDialog(const std::string& message);
}; #pragma once
