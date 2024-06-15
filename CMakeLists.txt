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

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-lz")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-lz")

find_package(ZLIB REQUIRED)
find_package(ROOT REQUIRED)

set(CMAKE_CXX_FLAGS ${ROOT_CXX_FLAGS})
include(${ROOT_USE_FILE})

# set(CMAKE_CXX_STANDARD 20)

set(INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDES})

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(main1 main1.cpp ${SOURCES})

target_include_directories(main1 PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_directories(main1 PUBLIC ${ROOT_LIBRARY_DIR})
target_link_libraries(main1 ${ROOT_LIBRARIES})
target_link_libraries(main1 ZLIB::ZLIB)

add_custom_target(run1 ./main1)

add_executable(main2 main2.cpp ${SOURCES})

target_include_directories(main2 PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_directories(main2 PUBLIC ${ROOT_LIBRARY_DIR})
target_link_libraries(main2 ${ROOT_LIBRARIES})
target_link_libraries(main2 ZLIB::ZLIB)

add_custom_target(run2 ./main2)

add_executable(main3 main3.cpp ${SOURCES})

target_include_directories(main3 PUBLIC ${ROOT_INCLUDE_DIRS})
target_link_directories(main3 PUBLIC ${ROOT_LIBRARY_DIR})
target_link_libraries(main3 ${ROOT_LIBRARIES})
target_link_libraries(main3 ZLIB::ZLIB)

add_custom_target(run3 ./main3)