#include "TaskFactory.h"
#include "Tasks/BackupTask.h"
#include "Tasks/MatrixTask.h"
#include "Tasks/HttpGetTask.h"
#include "Tasks/ReminderTask.h"
#include "Tasks/StatisticTask.h"
#include <memory>
#include <sstream>

std::shared_ptr<ITask> TaskFactory::CreateTask(TaskType type, const std::string& params) {
    switch (type) {
    case TaskType::Backup:
        // 解析参数：源路径;目标路径
        if (params.empty()) {
            return std::make_shared<BackupTask>("C:\\Data", "D:\\Backup");
        }
        else {
            std::istringstream iss(params);
            std::string source, dest;
            std::getline(iss, source, ';');
            std::getline(iss, dest);
            return std::make_shared<BackupTask>(source, dest);
        }

    case TaskType::Matrix:
        // 解析参数：矩阵大小
        if (params.empty()) {
            return std::make_shared<MatrixTask>(200);
        }
        else {
            return std::make_shared<MatrixTask>(std::stoi(params));
        }

    case TaskType::HttpGet:
        // 解析参数：URL
        if (params.empty()) {
            return std::make_shared<HttpGetTask>("https://api.github.com/zen");
        }
        else {
            return std::make_shared<HttpGetTask>(params);
        }

    case TaskType::Reminder:
        // 解析参数：消息
        if (params.empty()) {
            return std::make_shared<ReminderTask>("休息5分钟");
        }
        else {
            return std::make_shared<ReminderTask>(params);
        }

    case TaskType::Statistic:
        // 解析参数：数量;最小值;最大值
        if (params.empty()) {
            return std::make_shared<StatisticTask>(1000, 0, 100);
        }
        else {
            std::istringstream iss(params);
            std::string countStr, minStr, maxStr;
            std::getline(iss, countStr, ';');
            std::getline(iss, minStr, ';');
            std::getline(iss, maxStr);
            return std::make_shared<StatisticTask>(
                std::stoi(countStr),
                std::stoi(minStr),
                std::stoi(maxStr)
            );
        }

    default:
        throw std::invalid_argument("未知的任务类型");
    }
}