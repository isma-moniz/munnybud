cmake_minimum_required(VERSION 3.20)
project(munnybud)

set(CMAKE_CXX_STANDARD 17)

add_executable(munnybud src/main.cpp src/commands.cpp src/expense.cpp src/income.cpp src/manager.cpp src/storage.cpp src/utils.cpp)
