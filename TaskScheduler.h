// TaskScheduler.h
#pragma once
#include <memory>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "ITask.h"

struct ScheduledTask {
    std::shared_ptr<ITask> task;
    std::chrono::system_clock::time_point runTime;
    bool isPeriodic;
    std::chrono::seconds interval;

    bool operator>(const ScheduledTask& other) const {
        return runTime > other.runTime;
    }
};

class TaskScheduler {
public:
    static TaskScheduler& GetInstance();

    void AddOneTimeTask(std::shared_ptr<ITask> task);
    void AddDelayedTask(std::shared_ptr<ITask> task, int delaySeconds);
    void AddPeriodicTask(std::shared_ptr<ITask> task, int intervalSeconds);

    void Start();
    void Stop();

private:
    TaskScheduler() = default;
    ~TaskScheduler();

    // 禁用拷贝
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler& operator=(const TaskScheduler&) = delete;

    std::priority_queue<
        ScheduledTask,
        std::vector<ScheduledTask>,
        std::greater<>
    > taskQueue;

    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> running{ false };
    std::thread workerThread;

    void WorkerLoop();
};