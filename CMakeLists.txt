cmake_minimum_required(VERSION 3.4.0)
cmake_policy(VERSION 3.0.2)

project(Palm)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(JSON_BuildTests OFF)
option(BUILD_SHARED_LIBS ON)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES src/*.hpp)

find_package(Curses REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(CURL REQUIRED)
include_directories(${CURSES_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib/libvlcpp ${CMAKE_SOURCE_DIR}/lib/csv)

add_executable(palm ${HEADER_FILES} ${SOURCE_FILES})
add_subdirectory(lib/json)

target_link_libraries(palm ${CURSES_LIBRARIES} ${LIBXML2_LIBRARIES} ${CURL_LIBRARIES} nlohmann_json vlc)