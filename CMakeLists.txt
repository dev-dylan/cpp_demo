cmake_minimum_required(VERSION 3.5)
project(OpenCVExample)

# 引用 OpenCV
# set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/opencv-4.9.0/build_cpp)
set(OpenCV_DIR /Users/admin/Documents/Public/opencv4.9.0/build_cpp)
# 根据指定的目录，查找 opencv 库
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(ZLIB REQUIRED)

# message("This is a normal message. ${Poco_INCLUDE_DIRS}")
# 添加所有的源代码文件
set(SOURCES
    main.cpp
)
file(GLOB libs "libs/*.cpp" "libs/*.hpp")
file(GLOB src "src/*.cpp" "src/*.hpp")
file(GLOB tests "tests/*.cpp")

# 添加所有的头文件
set(HEADERS
    libs/base64.h
)

# message("11111xzxxxx")
# message("Project Version: ${CMAKE_INSTALL_PREFIX}")
# message(${ANDROID_ABI})

# add_executable(OpenCVExample ${SOURCES} ${src} ${libs})
# set_property(TARGET OpenCVExample PROPERTY CXX_STANDARD 11)
# target_link_libraries(OpenCVExample ${OpenCV_LIBS} ZLIB::ZLIB)

include(FetchContent)
# 下載和引入 Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 開啟 CMake 測試
enable_testing()

add_executable(OpenCVExample ${SOURCES} ${src} ${libs} ${tests})
set_property(TARGET OpenCVExample PROPERTY CXX_STANDARD 17)
target_link_libraries(OpenCVExample ${OpenCV_LIBS} ZLIB::ZLIB GTest::gtest_main)

include(GoogleTest)
# gtest_discover_tests(OpenCVExample)

