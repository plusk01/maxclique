/**
 * @file main.cpp
 * @brief Entry point for all test suites
 * @author Parker Lusk <plusk@mit.edu>
 * @date 29 November 2021
 */

#include <gtest/gtest.h>

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}