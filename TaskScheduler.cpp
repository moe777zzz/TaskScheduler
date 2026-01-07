#include "TaskScheduler.h"
#include "LogWriter.h"
#include <iostream>

TaskScheduler& TaskScheduler::GetInstance() {
    static TaskScheduler instance;
    return instance;
}

TaskScheduler::~TaskScheduler() {
    Stop();
}

void TaskScheduler::AddOneTimeTask(std::shared_ptr<ITask> task) {
    std::lock_guard<std::mutex> lock(queueMutex);
    ScheduledTask scheduledTask{
        task,
        std::chrono::system_clock::now(),
        false,
        std::chrono::seconds(0)
    };
    taskQueue.push(scheduledTask);
    cv.notify_one();
}

void TaskScheduler::AddDelayedTask(std::shared_ptr<ITask> task, int delaySeconds) {
    std::lock_guard<std::mutex> lock(queueMutex);
    ScheduledTask scheduledTask{
        task,
        std::chrono::system_clock::now() + std::chrono::seconds(delaySeconds),
        false,
        std::chrono::seconds(0)
    };
    taskQueue.push(scheduledTask);
    cv.notify_one();
}

void TaskScheduler::AddPeriodicTask(std::shared_ptr<ITask> task, int intervalSeconds) {
    std::lock_guard<std::mutex> lock(queueMutex);
    ScheduledTask scheduledTask{
        task,
        std::chrono::system_clock::now() + std::chrono::seconds(intervalSeconds),
        true,
        std::chrono::seconds(intervalSeconds)
    };
    taskQueue.push(scheduledTask);
    cv.notify_one();
}

void TaskScheduler::Start() {
    if (running) return;

    running = true;
    workerThread = std::thread(&TaskScheduler::WorkerLoop, this);
    std::cout << "任务调度器已启动\n";
}

void TaskScheduler::Stop() {
    if (!running) return;

    running = false;
    cv.notify_one();

    if (workerThread.joinable()) {
        workerThread.join();
    }

    std::cout << "任务调度器已停止\n";
}

void TaskScheduler::WorkerLoop() {
    LogWriter logger("scheduler.log");

    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);

        if (taskQueue.empty()) {
            cv.wait(lock);
            continue;
        }

        auto& nextTask = taskQueue.top();
        auto now = std::chrono::system_clock::now();

        if (now < nextTask.runTime) {
            cv.wait_until(lock, nextTask.runTime);
            continue;
        }

        // 取出任务
        ScheduledTask currentTask = nextTask;
        taskQueue.pop();
        lock.unlock();

        // 执行任务
        try {
            std::cout << "执行任务: " << currentTask.task->GetName() << std::endl;
            logger.Write("开始执行任务: " + currentTask.task->GetName(), LogWriter::LogLevel::INFO);
            currentTask.task->Execute();
            logger.Write("任务执行完成: " + currentTask.task->GetName(), LogWriter::LogLevel::INFO);
        }
        catch (const std::exception& e) {
            logger.Write("任务执行失败: " + std::string(e.what()), LogWriter::LogLevel::ERROR);
        }

        // 如果是周期任务，重新加入队列
        if (currentTask.isPeriodic) {
            lock.lock();
            ScheduledTask newTask{
                currentTask.task,
                std::chrono::system_clock::now() + currentTask.interval,
                true,
                currentTask.interval
            };
            taskQueue.push(newTask);
            lock.unlock();
        }
    }
}