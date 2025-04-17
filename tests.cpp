#include "gaussian_solver.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <sstream>

using namespace Eigen;

TEST(GaussianSolverTest, SimpleSystem) {
    MatrixXd A(2, 2);
    VectorXd b(2), x;

    A << 2, 1,
         5, 7;
    b << 11, 13;

    ASSERT_TRUE(GaussianSolver::solve(A, b, x));
    EXPECT_NEAR(x(0), 7.1111, 1e-4);
    EXPECT_NEAR(x(1), -3.2222, 1e-4);
}

TEST(GaussianSolverTest, SingularMatrix) {
    MatrixXd A(2, 2);
    VectorXd b(2), x;

    A << 1, 2,
         2, 4;
    b << 3, 6;

    ASSERT_FALSE(GaussianSolver::solve(A, b, x));
}

TEST(GaussianSolverTest, RandomSystemGeneration) {
    MatrixXd A;
    VectorXd b;

    GaussianSolver::generateRandomSystem(10, A, b, 42);
    EXPECT_EQ(A.rows(), 10);
    EXPECT_EQ(A.cols(), 10);
    EXPECT_EQ(b.size(), 10);
}

TEST(GaussianSolverTest, ReadSystemFromCSV) {
    std::ofstream file("test_input.csv");
    file << "Header1,Header2,Header3\n"  
         << "1.0,2.0,3.0\n"
         << "4.0,5.0,6.0\n";
    file.close();

    MatrixXd A;
    VectorXd b;
    
    ASSERT_TRUE(GaussianSolver::readSystemFromCSV("test_input.csv", A, b));
    EXPECT_EQ(A.rows(), 2);
    EXPECT_EQ(A.cols(), 2);
    EXPECT_EQ(b.size(), 2);
    EXPECT_DOUBLE_EQ(A(0,0), 1.0);
    EXPECT_DOUBLE_EQ(A(1,1), 5.0);
    EXPECT_DOUBLE_EQ(b(0), 3.0);
    EXPECT_DOUBLE_EQ(b(1), 6.0);

    std::remove("test_input.csv");
}

TEST(GaussianSolverTest, SolveSimpleSystem) {
    MatrixXd A(2,2);
    VectorXd b(2), x;
    A << 2, 1, 1, -1;
    b << 3, 0;

    ASSERT_TRUE(GaussianSolver::solve(A, b, x));
    EXPECT_NEAR(x(0), 1.0, 1e-10);
    EXPECT_NEAR(x(1), 1.0, 1e-10);
}

TEST(GaussianSolverTest, DegenerateSystem) {
    MatrixXd A(2,2);
    VectorXd b(2), x;
    A << 1, 2, 2, 4;
    b << 3, 6;

    EXPECT_FALSE(GaussianSolver::solve(A, b, x));
}

TEST(GaussianSolverTest, GenerateRandomSystemConsistency) {
    MatrixXd A1, A2;
    VectorXd b1, b2;

    GaussianSolver::generateRandomSystem(3, A1, b1, 123);
    GaussianSolver::generateRandomSystem(3, A2, b2, 123);

    EXPECT_TRUE(A1.isApprox(A2));
    EXPECT_TRUE(b1.isApprox(b2));
}
TEST(GaussianSolverTest, ReadSystemWithHeader) {
    std::ofstream file("test_with_header.csv");
    file << "X1,X2,B\n1.0,2.0,3.0\n4.0,5.0,6.0\n";
    file.close();

    MatrixXd A;
    VectorXd b;
    ASSERT_TRUE(GaussianSolver::readSystemFromCSV("test_with_header.csv", A, b));
    EXPECT_EQ(A.rows(), 2);
    EXPECT_EQ(A.cols(), 2);
    EXPECT_EQ(b.size(), 2);
    EXPECT_DOUBLE_EQ(A(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(A(1, 1), 5.0);
    EXPECT_DOUBLE_EQ(b(1), 6.0);
}

TEST(GaussianSolverTest, ReadSystemWithInvalidValue) {
    std::ofstream file("test_invalid.csv");
    file << "A,B,C\n1.0,2.0,x\n4.0,5.0,6.0\n";
    file.close();

    MatrixXd A;
    VectorXd b;
    ASSERT_FALSE(GaussianSolver::readSystemFromCSV("test_invalid.csv", A, b));
}

TEST(GaussianSolverTest, ReadEmptyCSVFile) {
    std::ofstream file("test_empty.csv");
    file.close();

    MatrixXd A;
    VectorXd b;
    ASSERT_FALSE(GaussianSolver::readSystemFromCSV("test_empty.csv", A, b));
}

TEST(GaussianSolverTest, ReadCSVWithInconsistentRowLengths) {
    std::ofstream file("test_inconsistent.csv");
    file << "A,B,C\n1.0,2.0,3.0\n4.0,5.0\n"; 
    file.close();

    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    
    
    bool result = GaussianSolver::readSystemFromCSV("test_inconsistent.csv", A, b);
    EXPECT_FALSE(result) << "Function should return false for inconsistent row lengths";

    std::remove("test_inconsistent.csv");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

