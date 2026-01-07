#include "MatrixTask.h"
#include <iostream>
#include <random>
#include <chrono>

MatrixTask::MatrixTask(int size) : matrixSize(size) {}

std::string MatrixTask::GetName() const {
    return "矩阵乘法任务 (大小: " + std::to_string(matrixSize) + ")";
}

MatrixTask::Matrix MatrixTask::GenerateRandomMatrix(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 10.0);

    Matrix matrix(size, std::vector<double>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
    return matrix;
}

MatrixTask::Matrix MatrixTask::MultiplyMatrices(const Matrix& a, const Matrix& b) {
    int size = a.size();
    Matrix result(size, std::vector<double>(size, 0.0));

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

void MatrixTask::Execute() {
    auto start = std::chrono::high_resolution_clock::now();

    // 生成两个随机矩阵
    Matrix A = GenerateRandomMatrix(matrixSize);
    Matrix B = GenerateRandomMatrix(matrixSize);

    // 执行矩阵乘法
    Matrix C = MultiplyMatrices(A, B);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "矩阵乘法完成 (" << matrixSize << "x" << matrixSize
        << "), 耗时: " << duration.count() << "ms" << std::endl;
}