#include "gaussian_solver.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include <algorithm>

using namespace Eigen;

bool GaussianSolver::readSystemFromCSV(const std::string& filename, 
                                     Eigen::MatrixXd& A, 
                                     Eigen::VectorXd& b) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }

    std::vector<std::vector<double>> data;
    std::string line;

    std::getline(file, line);

    std::getline(file, line);
    std::vector<double> first_row;
    std::stringstream first_ss(line);
    std::string value;
    
    while (std::getline(first_ss, value, ',')) {
        try {
            first_row.push_back(std::stod(value));
        } catch (...) {
            return false;
        }
    }
    
    if (first_row.empty()) return false;
    const size_t expected_cols = first_row.size();
    data.push_back(first_row);

    while (std::getline(file, line)) {
        std::vector<double> row;
        std::stringstream ss(line);
        std::string value;
        size_t cols_read = 0;
        
        while (std::getline(ss, value, ',')) {
            try {
                row.push_back(std::stod(value));
                cols_read++;
            } catch (...) {
                return false;
            }
        }
        
       
        if (cols_read != expected_cols) {
            return false;
        }
        
        data.push_back(row);
    }

    if (data.empty()) return false;

    const size_t rows = data.size();
    const size_t cols = data[0].size();
    
    A.resize(rows, cols-1);
    b.resize(rows);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols-1; ++j) {
            A(i,j) = data[i][j];
        }
        b(i) = data[i][cols-1];
    }

    return true;
}

bool GaussianSolver::writeSolutionToCSV(const std::string& filename,
                                      const VectorXd& x) {
    std::ofstream file(filename);
    if (!file) return false;

    file << std::setprecision(12);
    for (int i = 0; i < x.size(); ++i) {
        file << x(i);
        if (i != x.size() - 1) file << ",";
    }

    return true;
}

void GaussianSolver::generateRandomSystem(int size, 
                                        MatrixXd& A, 
                                        VectorXd& b,
                                        unsigned int seed) {
    A.resize(size, size);
    b.resize(size);
    
    std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(-10.0, 10.0);

    A = MatrixXd::NullaryExpr(size, size, [&]() { return dist(gen); });
    b = VectorXd::NullaryExpr(size, [&]() { return dist(gen); });

    A.diagonal() += A.rowwise().norm();
}

bool GaussianSolver::solve(MatrixXd A, VectorXd b, VectorXd& x) {
    const int n = A.rows();
    if (n != A.cols() || n != b.size()) return false;

    for (int k = 0; k < n; ++k) {
        int max_row = k;
        A.col(k).tail(n-k).cwiseAbs().maxCoeff(&max_row);
        max_row += k;
        
        if (A(max_row, k) == 0.0) return false;
        
        if (max_row != k) {
            A.row(k).swap(A.row(max_row));
            std::swap(b(k), b(max_row));
        }

        VectorXd scale = A.col(k).tail(n-k-1) / A(k,k);
        A.bottomRightCorner(n-k-1, n-k) -= scale * A.row(k).tail(n-k);
        b.tail(n-k-1) -= scale * b(k);
    }

    x.resize(n);
    for (int k = n-1; k >= 0; --k) {
        x(k) = (b(k) - A.row(k).tail(n-k-1).dot(x.tail(n-k-1))) / A(k,k);
    }

    return true;
}

bool GaussianSolver::validateSolution(const MatrixXd& A,
                                    const VectorXd& b,
                                    const VectorXd& x,
                                    double tolerance) {
    return (A * x - b).norm() < tolerance;
}
