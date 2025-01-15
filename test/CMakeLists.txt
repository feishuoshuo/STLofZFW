add_executable(test_sequence_containers test_STLContainer_list.cpp)# 添加测试用例

# 链接gtest库: GTest是Google Test库，需要先安装并配置好环境
# Google Test是一个C++测试框架，用于编写和运行C++的单元测试
find_package(GTest REQUIRED)
target_link_libraries(test_sequence_containers GTest::GTest GTest::Main)


add_test(NAME test_sequence_containers COMMAND test_sequence_containers)# 添加测试