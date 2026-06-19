cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RustAPITestMod)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# C++ wrapper library
add_library(${PROJECT_NAME} SHARED
    src/Mod.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(${PROJECT_NAME} geode-sdk ${CMAKE_CURRENT_SOURCE_DIR}/target/release/testmod.lib ws2_32 userenv bcrypt ntdll)
setup_geode_mod(${PROJECT_NAME})

if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<NOT:$<CONFIG:Debug>>:LINKER:/OPT:REF>
        $<$<NOT:$<CONFIG:Debug>>:LINKER:/OPT:ICF>)
endif()