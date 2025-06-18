cmake_minimum_required(VERSION 3.16)
project(CookCookhNya-backend)

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
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-ftime-report -Wall -Wextra)
add_compile_options(-O0 -g)

# dependencies
find_package(TgBotStater REQUIRED)
find_package(httplib REQUIRED)

# main program
add_executable(main
    src/main.cpp
    src/utils.cpp
)

# setup
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main PRIVATE
    ${tgbotstater_LIBRARIES}
    ${httplib_LIBRARIES}
)
