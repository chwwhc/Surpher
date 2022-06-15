cmake_minimum_required(VERSION 3.16)
project(Surpher)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(Surpher
        src/Lexer.cpp
        src/Lexer.hpp
        src/Surpher.cpp
        src/Token.cpp
        src/Token.hpp src/Expr.hpp src/Expr.cpp src/AstPrinter.hpp src/AstPrinter.cpp src/Parser.hpp src/Parser.cpp src/Error.hpp src/Error.cpp src/Interpreter.hpp src/Interpreter.cpp src/Stmt.hpp src/Stmt.cpp src/Environment.hpp src/Environment.cpp)
