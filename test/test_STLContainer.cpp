#include "../googletest-1.14.0/googletest/include/gtest/gtest.h"
#include <iostream>
#include "../STL/vector.h"

TEST(ADDTEST, ADDTEST_TRUE) // ADDTEST_TRUE测试用例名称
{
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42); // 断言括号中的返回值是否符合预期
}
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS(); // 是 Google Test 提供的一个宏，它会自动运行所有注册的测试用例
}
