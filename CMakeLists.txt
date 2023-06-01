# This is an example of how to use FetchContent to easily
# manage 3rd-party dependencies
# First, check out src/CMakeLists.txt - it's really simple!
# Then, check out dependencies directory, it get a bit more complex
# there, but you'll see how it goes
#
# Options:
# * USE_SYSTEM_DEPS - try to find dependencies using find_package (OFF by default)
# * LINK_DEPS_STATIC - link to dependencies statically (ON by default)
#
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
find_package(Boost REQUIRED COMPONENTS system filesystem)

if (WIN32)
	add_compile_definitions(WIN32_LEAN_AND_MEAN)
	add_compile_definitions(WINVER=0x0A00)
	add_compile_definitions(_WIN32_WINNT=0x0A00)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	add_compile_options(/bigobj)
	include_directories($ENV{MSVC_INCLUDE_PATH})

endif ()

add_executable(${PROJECT_NAME} include/Structure.hpp main.cpp
	include/LadderBuilder.hpp
	src/LadderBuilder.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::headers)

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
