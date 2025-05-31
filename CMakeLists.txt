cmake_minimum_required(VERSION 3.16)

project("SimpleValidator")
add_executable(validator main.cpp simple_validator.cpp scanner.cpp)
add_executable(checker checker.cpp simple_validator.cpp scanner.cpp)
