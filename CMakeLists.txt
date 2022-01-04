cmake_minimum_required(VERSION 3.20)

project(MTInterpreter)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(Tests)
add_executable(MTInterpreter main.cpp
        Token/IdToken.cpp
        Token/Token.cpp
        Scaner.cpp
        DataSource/FileDataSource.cpp
        DataSource/StringDataSource.cpp
        ErrorHandler.cpp
        Token/LoopMod.cpp
        Token/TypeName.cpp
        Token/Assign.cpp
        Token/MultOp.cpp
        Token/RelOp.cpp
        Token/LogicOp.cpp
        Token/Conversion.cpp
        Token/StringToken.cpp
        Token/Number.cpp
        Token/ErrorToken.cpp
        Token/Util.cpp
        Parser.cpp
        Interpreter/SemanticAnalizer.cpp)