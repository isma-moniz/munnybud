cmake_minimum_required(VERSION 4.0)
project(munnybud)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(munnybud 
src/main.cpp 
src/commands.cpp 
src/transaction.cpp 
src/interface.cpp 
src/storage.cpp 
src/utils.cpp 
src/indexmanager.cpp)

target_link_libraries(munnybud Qt5::Widgets)