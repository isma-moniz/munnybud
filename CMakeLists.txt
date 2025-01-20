cmake_minimum_required(VERSION 3.20)
project(munnybud)

set(CMAKE_CXX_STANDARD 20)

add_executable(munnybud src/main.cpp src/commands.cpp src/transaction.cpp src/interface.cpp src/income.cpp src/manager.cpp src/storage.cpp src/utils.cpp)
