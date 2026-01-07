#include "StatisticTask.h"
#include <random>
#include <iostream>
#include <cmath>
#include <algorithm>

StatisticTask::StatisticTask(int count, int min, int max)
    : count(count), minValue(min), maxValue(max) {
}

std::string StatisticTask::GetName() const {
    return "随机数统计任务 (" + std::to_string(count) + "个数, 范围[" +
        std::to_string(minValue) + "-" + std::to_string(maxValue) + "])";
}

std::vector<int> StatisticTask::GenerateRandomNumbers() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(minValue, maxValue);

    std::vector<int> numbers;
    numbers.reserve(count);

    for (int i = 0; i < count; ++i) {
        numbers.push_back(dis(gen));
    }

    return numbers;
}

double StatisticTask::CalculateMean(const std::vector<int>& numbers) {
    if (numbers.empty()) return 0.0;

    double sum = 0.0;
    for (int num : numbers) {
        sum += num;
    }
    return sum / numbers.size();
}

double StatisticTask::CalculateVariance(const std::vector<int>& numbers, double mean) {
    if (numbers.size() <= 1) return 0.0;

    double variance = 0.0;
    for (int num : numbers) {
        double diff = num - mean;
        variance += diff * diff;
    }
    return variance / (numbers.size() - 1);
}

void StatisticTask::Execute() {
    auto numbers = GenerateRandomNumbers();
    double mean = CalculateMean(numbers);
    double variance = CalculateVariance(numbers, mean);
    double stdDev = std::sqrt(variance);

    // 找到最大值和最小值
    int maxNum = *std::max_element(numbers.begin(), numbers.end());
    int minNum = *std::min_element(numbers.begin(), numbers.end());

    std::cout << "统计结果:" << std::endl;
    std::cout << "  数量: " << numbers.size() << std::endl;
    std::cout << "  均值: " << mean << std::endl;
    std::cout << "  方差: " << variance << std::endl;
    std::cout << "  标准差: " << stdDev << std::endl;
    std::cout << "  最大值: " << maxNum << std::endl;
    std::cout << "  最小值: " << minNum << std::endl;
}