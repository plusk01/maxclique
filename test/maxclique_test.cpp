/**
 * @file maxclique_test.cpp
 * @brief Testing maximum clique solver
 * @author Parker Lusk <plusk@mit.edu>
 * @date 20 December 2021
 */

#include <gtest/gtest.h>
#include <vector>
#include <Eigen/Dense>

#include <maxclique/maxclique.h>

TEST(maxclique, exact) {
  static constexpr int m = 6;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(m, m);
  A << 0, 1, 1, 1, 0, 0,
       1, 0, 1, 1, 0, 0,
       1, 1, 0, 1, 0, 0,
       1, 1, 1, 0, 1, 0,
       0, 0, 0, 1, 0, 1,
       0, 0, 0, 0, 1, 0;

  maxclique::Params params;
  params.method = maxclique::Method::EXACT;
  params.verbose = true;

  const std::vector<int> mc = maxclique::solve(A, params);

  std::cout << std::endl << std::endl;
  for (size_t i=0; i<mc.size(); ++i) {
    std::cout << mc[i] << " ";
  }
  std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

TEST(maxclique, heuristic) {
  static constexpr int m = 6;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(m, m);
  A << 0, 1, 1, 1, 0, 0,
       1, 0, 1, 1, 0, 0,
       1, 1, 0, 1, 0, 0,
       1, 1, 1, 0, 1, 0,
       0, 0, 0, 1, 0, 1,
       0, 0, 0, 0, 1, 0;

  maxclique::Params params;
  params.method = maxclique::Method::HEU;
  params.verbose = true;

  const std::vector<int> mc = maxclique::solve(A, params);

  std::cout << std::endl << std::endl;
  for (size_t i=0; i<mc.size(); ++i) {
    std::cout << mc[i] << " ";
  }
  std::cout << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

TEST(maxclique, kcore) {
  static constexpr int m = 6;
  Eigen::MatrixXd A = Eigen::MatrixXd::Zero(m, m);
  A << 0, 1, 1, 1, 0, 0,
       1, 0, 1, 1, 0, 0,
       1, 1, 0, 1, 0, 0,
       1, 1, 1, 0, 1, 0,
       0, 0, 0, 1, 0, 1,
       0, 0, 0, 0, 1, 0;

  maxclique::Params params;
  params.method = maxclique::Method::KCORE;
  params.verbose = true;

  const std::vector<int> mc = maxclique::solve(A, params);

  std::cout << std::endl << std::endl;
  for (size_t i=0; i<mc.size(); ++i) {
    std::cout << mc[i] << " ";
  }
  std::cout << std::endl;
}
