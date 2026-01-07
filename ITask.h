// ITask.h
#pragma once
#include <string>

class ITask {
public:
    virtual ~ITask() = default;
    virtual std::string GetName() const = 0;
    virtual void Execute() = 0;
};