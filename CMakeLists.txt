cmake_minimum_required(VERSION 3.14)
project(AServer)

set(TARGET_NAME "AServer")

set(CMAKE_CXX_STANDARD 17)

# Header files
################################################################################
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
################################################################################

# Source files
################################################################################
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
################################################################################

# Dependencies
################################################################################
find_package(Boost 1.70.0 REQUIRED)
find_package(Threads REQUIRED)

set(ALL_DEPENDENCY ${BOOST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
################################################################################

message("DEP: ${ALL_DEPENDENCY}")
message("SRC: ${SOURCE_FILES}")
message("DIR: ${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(
    ${TARGET_NAME}
    main.cpp ${SOURCE_FILES}
)

target_link_libraries(
    ${TARGET_NAME}
    ${ALL_DEPENDENCY}
)
