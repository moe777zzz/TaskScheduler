#pragma once
// BackupTask.h
#pragma once
#include "ITask.h"
#include <string>

class BackupTask : public ITask {
public:
    BackupTask(const std::string& source, const std::string& dest);
    std::string GetName() const override;
    void Execute() override;

private:
    std::string sourcePath;
    std::string destPath;

    std::string GenerateTimestamp() const;
};