cmake_minimum_required(VERSION 3.20)

project(chess_2d LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v7.0.3
)

FetchContent_MakeAvailable(ftxui)

add_executable(chess_2d main.cpp)

target_link_libraries(
    chess_2d
    PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component
)
