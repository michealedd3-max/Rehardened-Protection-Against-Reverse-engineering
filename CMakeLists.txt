cmake_minimum_required(VERSION 3.16)
project(rehardened_pc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(rehardened_launcher
    launcher/main.cpp
    launcher/integrity.cpp
    launcher/rust_policy.cpp
)

target_include_directories(rehardened_launcher PRIVATE launcher)
