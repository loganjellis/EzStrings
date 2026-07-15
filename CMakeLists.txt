cmake_minimum_required(VERSION 4.0)
project(EzStrings VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")

option(BUILD_SHARED_LIBS "Build using a shared library." ON)

include(FetchContent)
FetchContent_Declare(vibrant_logs GIT_REPOSITORY https://github.com/loganjellis/VibrantLogs.git GIT_TAG main)
FetchContent_MakeAvailable(vibrant_logs)

add_library(ez_strings "${CMAKE_CURRENT_SOURCE_DIR}/src/ez_strings.c")

target_compile_definitions(ez_strings PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:EZ_STRINGS_EXPORTS>)

target_include_directories(ez_strings PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(ez_strings PUBLIC vibrant_logs)
