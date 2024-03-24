cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(test)

# prevent for in-source building
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")

find_package(ROOT REQUIRED)

set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})
include(${ROOT_USE_FILE})

# set(CMAKE_CXX_STANDARD 20)

set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${ROOT_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES})

add_custom_target(run ./${PROJECT_NAME})