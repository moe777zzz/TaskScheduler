#pragma once
// TaskFactory.h
#pragma once
#include "ITask.h"
#include <memory>
#include <string>

class TaskFactory {
public:
    enum class TaskType {
        Backup,
        Matrix,
        HttpGet,
        Reminder,
        Statistic
    };

    static std::shared_ptr<ITask> CreateTask(TaskType type, const std::string& params = "");
};