#pragma once
// MatrixTask.h
#pragma once
#include "ITask.h"
#include <vector>

class MatrixTask : public ITask {
public:
    MatrixTask(int size = 200);
    std::string GetName() const override;
    void Execute() override;

private:
    int matrixSize;

    using Matrix = std::vector<std::vector<double>>;
    Matrix MultiplyMatrices(const Matrix& a, const Matrix& b);
    Matrix GenerateRandomMatrix(int size);
};