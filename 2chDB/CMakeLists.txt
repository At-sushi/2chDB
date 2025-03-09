# CMakeList.txt : 2chDB の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
set(CMAKE_C_FLAGS "-funsigned-char -O2 -DHAVE_READ2CH_H")

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (2chDB "2chDB.cpp" "2chDB.h" "readcgi/read.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 2chDB PROPERTY CXX_STANDARD 20)
endif()

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
# 2chDB/CMakeLists.txt

find_package(Boost 1.87.0 REQUIRED)