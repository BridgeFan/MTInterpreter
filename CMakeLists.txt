cmake_minimum_required(VERSION 3.20)
project(MTInterpreter)

set(CMAKE_CXX_STANDARD 17)

add_executable(MTInterpreter main.cpp Token/IdToken.cpp Token/Token.cpp Scaner.cpp Scaner.h DataSource/DataSource.h DataSource/FileDataSource.cpp DataSource/FileDataSource.h DataSource/StringDataSource.cpp DataSource/StringDataSource.h)
