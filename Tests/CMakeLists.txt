cmake_minimum_required(VERSION 2.6)
project(Google_tests)
# Locate GTest
find_package(GTest REQUIRED)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(Google_Tests_run Test.cpp
        ../Token/IdToken.cpp
        ../Token/Token.cpp
        ../Scaner.cpp
        ../DataSource/FileDataSource.cpp
        ../DataSource/StringDataSource.cpp
        ../ErrorHandler.cpp
        ../Token/LoopMod.cpp
        ../Token/TypeName.cpp
        ../Token/Assign.cpp
        ../Token/MultOp.cpp
        ../Token/RelOp.cpp
        ../Token/LogicOp.cpp
        ../Token/Conversion.cpp
        ../Token/StringToken.cpp
        ../Token/Number.cpp
        ../Token/ErrorToken.cpp ../Token/Util.cpp)

# linking Google_Tests_run with DateConverter_lib which will be tested
target_link_libraries(Google_Tests_run gtest gtest_main pthread)