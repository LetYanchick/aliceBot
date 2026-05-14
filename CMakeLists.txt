cmake_minimum_required(VERSION 3.16)
project(DungeonSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task
    main.cpp
    Room.cpp
    DungeonState.cpp
    Parser.cpp
    Logger.cpp
    Pathfinder.cpp
    AliceBot.cpp
)

target_include_directories(task PRIVATE ${CMAKE_SOURCE_DIR})
