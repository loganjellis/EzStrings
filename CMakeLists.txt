cmake_minimum_required(VERSION 4.0)
project(EzStrings VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER "C:/ProgramData/mingw64/mingw64/bin/gcc.exe")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_SHARED_LIBS "Build using a shared library." ON)

set(LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/ez_strings.c")

add_library(ez_strings ${LIB_SRC})

target_compile_definitions(ez_strings PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:EZ_STRINGS_EXPORTS>)

target_include_directories(ez_strings PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ez_strings PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/libtimey.dll.a" "${CMAKE_CURRENT_SOURCE_DIR}/lib/libvibrant_logs.dll.a")

add_executable(ez_strings_test "${CMAKE_CURRENT_SOURCE_DIR}/example.c")
target_link_libraries(ez_strings_test PRIVATE ez_strings)

install(TARGETS ez_strings
	EXPORT ez_strings-targets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ez_strings-targets
	FILE ez_strings-targets.cmake
	NAMESPACE EzStrings::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EzStrings)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/ez_strings_config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/ez_strings_config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EzStrings)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/ez_strings_configVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion)

install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/ez_strings_config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/ez_strings_configVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/EzStrings)
