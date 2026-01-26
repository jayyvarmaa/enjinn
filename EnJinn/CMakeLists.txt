cmake_minimum_required(VERSION 3.16)
project(EnJinn)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fix: Include directory for the project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Dependencies managed by vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(box2d CONFIG REQUIRED)

add_executable(EnJinnCore
    main.cpp
    core/enjinnEditor/Editor.cpp
    core/enjinnEditor/Editor.h
    core/enjinnEditor/phaseManager/phaseManager.cpp
    core/enjinnEditor/phaseManager/phaseManager.h
)

target_link_libraries(EnJinnCore PRIVATE
    glfw
    glm::glm
    imgui::imgui
    box2d::box2d
)

enable_testing()
find_package(Catch2 CONFIG REQUIRED)

add_executable(EnJinnTests
    tests/test_main.cpp
    core/enjinnEditor/phaseManager/phaseManager.cpp
    # Add other sources to test here or refactor to a library
)

target_link_libraries(EnJinnTests PRIVATE
    Catch2::Catch2WithMain
)

add_test(NAME EnJinnTests COMMAND EnJinnTests)
