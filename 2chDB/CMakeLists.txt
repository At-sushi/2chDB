# CMakeList.txt : 2chDB の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (2chDB "2chDB.cpp" "2chDB.h" "readcgi/read.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 2chDB PROPERTY CXX_STANDARD 20)
endif()

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
# 2chDB/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(2chDB)

set(CMAKE_CXX_STANDARD 20)

# Add CPPUnit
find_package(CPPUnit REQUIRED)
include_directories(${CPPUNIT_INCLUDE_DIR})

add_executable(2chDB main.cpp 2chDB.cpp)
target_link_libraries(2chDB ${CPPUNIT_LIBRARIES})

# Add test executable
add_executable(2chDB_test test_2chDB.cpp)
target_link_libraries(2chDB_test ${CPPUNIT_LIBRARIES})

# Enable testing
enable_testing()
add_test(NAME 2chDB_test COMMAND 2chDB_test)
