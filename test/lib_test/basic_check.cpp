//
// Created by tobias on 01/10/16.
//

#include "gtest/gtest.h"

TEST(basic_check, test_eq) {
  EXPECT_EQ(1, 1);
}

TEST(basic_check, test_neq) {
  EXPECT_NE(1, 0);
}
