#include "gaussian_solver.h"
#include <iostream>
#include <chrono>

int main() {
    Eigen::MatrixXd A;
    Eigen::VectorXd b, x;

    if (GaussianSolver::readSystemFromCSV("/home/vboxuser/Desktop/Gaussian_solver/input.csv", A, b)) {
        auto start = std::chrono::high_resolution_clock::now();

        if (GaussianSolver::solve(A, b, x)) {
            GaussianSolver::writeSolutionToCSV("/home/vboxuser/Desktop/Gaussian_solver/output.csv", x);

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

            std::cout << "System solved in " << duration.count() << " ms\n";
            std::cout << "Solution written to output.csv\n";

            if (!GaussianSolver::validateSolution(A, b, x)) {
                std::cerr << "Warning: Solution validation failed!\n";
            }
        }
    }

    const int n = 1000;
    std::cout << "\nGenerating random " << n << "x" << n << " system...\n";

    auto gen_start = std::chrono::high_resolution_clock::now();
    GaussianSolver::generateRandomSystem(n, A, b);
    auto gen_end = std::chrono::high_resolution_clock::now();

    std::cout << "Generation time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(gen_end-gen_start).count()
              << " ms\n";

    auto solve_start = std::chrono::high_resolution_clock::now();
    bool success = GaussianSolver::solve(A, b, x);
    auto solve_end = std::chrono::high_resolution_clock::now();

    if (success) {
        std::cout << "Solved in "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(solve_end-solve_start).count()
                  << " ms\n";

        double residual = (A * x - b).norm();
        std::cout << "Residual norm: " << residual << "\n";
    }

    return 0;
}
