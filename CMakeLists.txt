cmake_minimum_required(VERSION 3.16)
project(Surpher)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_compile_options("-O3")

include_directories(src)

add_executable(Surpher
        src/Lexer.cpp
        src/Lexer.hpp
        src/Surpher.cpp
        src/Token.cpp
        src/Token.hpp src/Expr.hpp src/Expr.cpp src/Parser.hpp src/Parser.cpp src/Error.hpp src/Error.cpp src/Interpreter.hpp 
        src/Interpreter.cpp src/Stmt.hpp src/Stmt.cpp src/Environment.hpp src/Environment.cpp src/SurpherCallable.hpp 
        src/SurpherCallable.cpp src/Resolver.hpp src/Resolver.cpp src/SurpherInstance.hpp src/SurpherInstance.cpp 
        src/SurpherNamespace.hpp src/SurpherNamespace.cpp src/built_in_utils/IO.hpp src/built_in_utils/IO.cpp src/built_in_utils/Global.hpp 
        src/built_in_utils/Global.cpp src/built_in_utils/NativeFunction.hpp src/built_in_utils/Math.cpp src/built_in_utils/Math.hpp 
        src/built_in_utils/Utils.hpp src/built_in_utils/Utils.cpp)
