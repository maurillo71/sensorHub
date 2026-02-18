cmake_minimum_required(VERSION 3.13)

project(SensorHubProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)



# Set the sysroot
set(CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/rpi2-sysroot")

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -mcpu=cortex-a7")

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Add the executable
add_executable(readSensorI2C 
    main.cpp 
    I2CReader.cpp 
    SensorHub.cpp 
    SensorDataDb.cpp
)

# Find and link SQLite3
find_package(SQLite3 REQUIRED)
target_link_libraries(readSensorI2C PRIVATE SQLite::SQLite3)

# Add other libraries
target_link_libraries(readSensorI2C PRIVATE stdc++ dl)

# Add test executable
add_executable(SensorDataDb_test SensorDataDb_test.cpp SensorDataDb.cpp)

# Link test executable against GTest and other libraries
target_link_libraries(SensorDataDb_test PRIVATE gtest gmock gtest_main pthread SQLite::SQLite3)

if(NOT CMAKE_CROSSCOMPILING)
  # Add test to CTest
  include(GoogleTest)
  gtest_discover_tests(SensorDataDb_test)
endif()
