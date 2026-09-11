cmake_minimum_required(VERSION 3.20)

project(FlowGame)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(FlowGame

    main.cpp

    Game.cpp

    Level.cpp

)

target_include_directories(FlowGame PRIVATE

    C:/SFML/include

)

target_link_directories(FlowGame PRIVATE

    C:/SFML/lib

)

target_link_libraries(FlowGame PRIVATE

    sfml-graphics

    sfml-window

    sfml-system

)