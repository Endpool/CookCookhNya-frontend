cmake_minimum_required(VERSION 3.16)
project(CookCookhNya-frontend)

# Set the default build type
if(NOT CMAKE_BUILD_TYPE)
    set(default_build_type "Debug")

    message(STATUS "${CMAKE_PROJECT_NAME} Set the build type to `${default_build_type}` as none was specified.")
    set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "Choose the build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "${CMAKE_PROJECT_NAME} Build Type: ${CMAKE_BUILD_TYPE}")

# project version
set(PROJECT_VERSION 0.1.0)
message(STATUS "${CMAKE_PROJECT_NAME} version: ${PROJECT_VERSION}")

# options
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    # add_compile_options(-ftime-report)
endif()

# dependencies
find_package(TgBotStater REQUIRED)
find_package(httplib REQUIRED)
find_package(Boost REQUIRED COMPONENTS json)

# main program
add_executable(main
    src/main.cpp
    src/message_tracker.cpp
    src/patched_bot.cpp
)
include(src/backend/CMakeLists.txt)
include(src/handlers/CMakeLists.txt)
include(src/render/CMakeLists.txt)
include(src/utils/CMakeLists.txt)

# setup
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main PRIVATE
    tgbotstater::tgbotstater
    httplib::httplib
    boost::boost
)
