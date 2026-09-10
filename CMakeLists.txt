cmake_minimum_required(VERSION 3.20)

project(FlowGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML 3.1 COMPONENTS Graphics Window System REQUIRED)

add_executable(FlowGame main.cpp)

target_link_libraries(FlowGame PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)