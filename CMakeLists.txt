# cmake_minimum_required(VERSION 2.8)# 需求的最低的cmake程序版本
# project(STLofZFW)#本工程的名字
# set(CMAKE_BUILD_TYPE "Debug") #打断点调试代码必设置，Cmake默认是Release会导致断点失效
# set(CMAKE_CXX_STANDARD 11)#本工程支持的c++版本
# include_directories(${PROJECT_SOURCE_DIR}/include)#指定头文件在哪里查找

# aux_source_directory(STL SRCS)# 搜索STL文件夹下所有的源文件，加入SRCS变量中
# message(STATUS "SRCS: ${SRCS}")
# add_library(calc ${SRCS})# 创建名为calcCMake的库，使用SRCS变量中的源文件

# # 指定主程序源文件
# set(EXECUTABLE_SOURCES
# src/zfw.cpp
# src/other_source.cpp
# )

# # 创建可执行文件
# add_executable(softRenderer ${EXECUTABLE_SOURCES})

# # 链接库到可执行文件
# target_link_libraries(softRenderer calc)
