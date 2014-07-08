#include "gtest/gtest.h"
#include "init.h"

int main(int argc, char **argv)
{
  mclisp::InitLisp();
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
