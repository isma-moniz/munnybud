cmake_minimum_required(VERSION 4.0)
project(munnybud)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP OFF)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(munnybud 
src/main.cpp 
src/commands.cpp 
src/transaction.cpp 
src/interface.cpp 
src/storage.cpp 
src/utils.cpp 
src/indexmanager.cpp)

target_link_libraries(munnybud ${CURSES_LIBRARIES})
