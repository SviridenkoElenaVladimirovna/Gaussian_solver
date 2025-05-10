#pragma once
#include <Eigen/Dense>
#include <string>
#include <vector>
#include <iomanip>

class GaussianSolver {
public:
    static bool readSystemFromCSV(const std::string& filename, 
                                Eigen::MatrixXd& A, 
                                Eigen::VectorXd& b);
    
    static bool writeSolutionToCSV(const std::string& filename,
                                 const Eigen::VectorXd& x);
    
    static void generateRandomSystem(int size, 
                                   Eigen::MatrixXd& A, 
                                   Eigen::VectorXd& b,
                                   unsigned int seed = 42);
    
    static bool solve(Eigen::MatrixXd A,
                     Eigen::VectorXd b,
                     Eigen::VectorXd& x);
    
    static bool validateSolution(const Eigen::MatrixXd& A,
                               const Eigen::VectorXd& b,
                               const Eigen::VectorXd& x,
                               double tolerance = 1e-6);
};
