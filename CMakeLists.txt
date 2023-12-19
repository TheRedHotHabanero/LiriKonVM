cmake_minimum_required(VERSION 3.16)
project(LiriKonVM)

set(CMAKE_CXX_STANDARD 17)

set(COMPILE_OPTIONS
	-Wall
	-Werror
	-Wextra
	#-Wconversion
	#-Wconversion
	-Wwrite-strings
	-Wcast-qual
	-Wundef
	-Wlogical-op
)

# Создаем директорию для сборки
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(CMAKE_BUILD_TYPE Debug)

# Создаем директорию сборки, если она не существует
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})

add_compile_options(${COMPILE_OPTIONS})

add_executable(LiriKonVM
	${CMAKE_SOURCE_DIR}/src/lexer.cpp
	${CMAKE_SOURCE_DIR}/src/parser.cpp
	${CMAKE_SOURCE_DIR}/src/allocator.cpp
	${CMAKE_SOURCE_DIR}/src/frame.cpp
    ${CMAKE_SOURCE_DIR}/src/decoder.cpp
    ${CMAKE_SOURCE_DIR}/src/interpreter.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp)

include_directories(include)
include_directories(isa)