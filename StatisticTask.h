#pragma once
#include "ITask.h"
#include <vector>
#include <string>

class StatisticTask : public ITask {
public:
    StatisticTask(int count = 1000, int min = 0, int max = 100);
    std::string GetName() const override;
    void Execute() override;

private:
    int count;
    int minValue;
    int maxValue;

    std::vector<int> GenerateRandomNumbers();
    double CalculateMean(const std::vector<int>& numbers);
    double CalculateVariance(const std::vector<int>& numbers, double mean);
}; #pragma once
