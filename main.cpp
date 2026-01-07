#include "TaskScheduler.h"
#include "TaskFactory.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>

int main() {
    std::cout << "=== 轻量级多任务调度器 ===\n";
    std::cout << "版本: 1.0.0\n";
    std::cout << "作者: zzz (AI辅助编程实践)\n\n";

    try {
        // 获取调度器实例
        auto& scheduler = TaskScheduler::GetInstance();

        std::cout << "正在添加示例任务...\n";

        // 示例1：HTTP GET任务（立即执行）
        std::cout << "添加HTTP GET任务...\n";
        auto httpTask = TaskFactory::CreateTask(
            TaskFactory::TaskType::HttpGet
        );
        scheduler.AddOneTimeTask(httpTask);

        // 示例2：随机数统计任务（延迟10秒）
        std::cout << "添加随机数统计任务（延迟10秒）...\n";
        auto statTask = TaskFactory::CreateTask(
            TaskFactory::TaskType::Statistic
        );
        scheduler.AddDelayedTask(statTask, 10);

        // 示例3：矩阵乘法任务（周期，每5秒）
        std::cout << "添加矩阵乘法任务（每5秒执行）...\n";
        auto matrixTask = TaskFactory::CreateTask(
            TaskFactory::TaskType::Matrix
        );
        scheduler.AddPeriodicTask(matrixTask, 5);

        // 示例4：课堂提醒任务（周期，每1分钟）
        std::cout << "添加课堂提醒任务（每1分钟执行）...\n";
        auto reminderTask = TaskFactory::CreateTask(
            TaskFactory::TaskType::Reminder,
            "该休息一下了！看看窗外，放松眼睛。"
        );
        scheduler.AddPeriodicTask(reminderTask, 60);

        // 示例5：文件备份任务（延迟30秒）
        std::cout << "添加文件备份任务（延迟30秒）...\n";
        auto backupTask = TaskFactory::CreateTask(
            TaskFactory::TaskType::Backup
        );
        scheduler.AddDelayedTask(backupTask, 30);

        // 启动调度器
        std::cout << "\n启动任务调度器...\n";
        scheduler.Start();

        // 显示控制命令
        std::cout << "\n=== 控制命令 ===\n";
        std::cout << "按 'q' 键停止调度器并退出程序\n";
        std::cout << "按任意其他键查看当前状态\n";

        // 主控制循环
        while (true) {
            if (_kbhit()) {  // 检查键盘输入
                char ch = _getch();
                if (ch == 'q' || ch == 'Q') {
                    std::cout << "\n正在停止调度器...\n";
                    break;
                }
                else {
                    std::cout << "\n调度器运行中... 按 'q' 退出\n";
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // 停止调度器
        scheduler.Stop();

        std::cout << "\n所有任务已完成。\n";

    }
    catch (const std::exception& e) {
        std::cerr << "程序异常: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "=== 程序结束 ===\n";

    // 等待用户按任意键退出
    std::cout << "\n按任意键退出...\n";
    _getch();

    return 0;
}