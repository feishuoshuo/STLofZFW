# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 链接gtest库: GTest是Google Test库，需要先安装并配置好环境
# Google Test是一个C++测试框架，用于编写和运行C++的单元测试
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# # 添加源文件
file(GLOB TEST_SOURCES "test_*.cpp")
# 明确指定源文件
# set(TEST_SOURCES test_STLContainer_deque.cpp test_STLContainer_hashtable.cpp)

#############所有.cpp汇总创建一个可执行文件 仅针对conmmon_test文件夹中普通测试######################
# # 添加一个可执行文件，包含所有测试用例
# add_executable(AllTests ${TEST_SOURCES})
# # 链接GTest库
# target_link_libraries(AllTests GTest::GTest GTest::Main)
# # 添加测试
# add_test(NAME AllTests COMMAND AllTests)
###########################################################

#############每个.cpp创建一个可执行文件######################
# # 添加可执行文件和测试
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} GTest::GTest GTest::Main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
###########################################################