cmake_minimum_required(VERSION 3.14)

project(Excalibur
        VERSION 1.0
        LANGUAGES C CXX
)


set(CMAKE_CXX_CPPCHECK "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads)
find_package(fmt REQUIRED)
find_package(nlohmann_json  REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

if (WIN32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    add_compile_definitions(WINVER=0x0A00)
    add_compile_definitions(_WIN32_WINNT=0x0A00)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/bigobj)
    include_directories($ENV{MSVC_INCLUDE_PATH})
endif ()

add_executable(${PROJECT_NAME} main.cpp
        include/EpollSocket.hpp
        src/EpollSocket.cpp
        src/StreamManager.cpp
        include/Structure.hpp
        include/LadderBuilder.hpp
        src/LadderBuilder.cpp
        include/StreamManager.hpp
        src/Recovery.cpp
        include/Recovery.hpp
        src/Excalibur.cpp
        include/Excalibur.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers nlohmann_json::nlohmann_json)

add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Excalibur.json ${CMAKE_CURRENT_BINARY_DIR})

target_precompile_headers(${PROJECT_NAME}
        INTERFACE
        <algorithm>
        <array>
        <chrono>
        <cstdint>
        <filesystem>
        <fstream>
        <iostream>
        <map>
        <memory>
        <ranges>
        <set>
        <sstream>
        <string>
        <string_view>
        <typeinfo>
        <unordered_map>
        <unordered_set>
        <variant>
        <vector>
        <thread>
        <memory>
)
