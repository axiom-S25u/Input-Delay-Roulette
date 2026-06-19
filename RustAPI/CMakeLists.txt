cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(RustAPI)

include(FetchContent)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# ImGui & ImGuiCocos
set(IMGUI_VERSION "v1.90.8" CACHE STRING "What imgui version to download and use" FORCE)
FetchContent_Declare(
    gd_imgui_cocos
    GIT_REPOSITORY https://github.com/matcool/gd-imgui-cocos.git
    GIT_TAG        b93f08ccef778a53ebba09b20c347f6a63980119
)
FetchContent_MakeAvailable(gd_imgui_cocos)

find_program(CARGO_EXECUTABLE cargo)
if(NOT CARGO_EXECUTABLE)
    message(FATAL_ERROR "Cargo not found! Please install Rust.")
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/target/release/rustapi.lib
    COMMAND ${CARGO_EXECUTABLE} build --release --manifest-path ${CMAKE_CURRENT_SOURCE_DIR}/Cargo.toml
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Building Rust library"
)

add_custom_target(
    rust_lib
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/target/release/rustapi.lib
)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/bindings/cocos_nodes.cpp
    src/bindings/filesystem.cpp
    src/bindings/imgui_bindings.cpp
    src/bindings/json_base64.cpp
    src/bindings/keyboard.cpp
    src/bindings/keybind_web.cpp
    src/bindings/logging.cpp
    src/bindings/mod_info.cpp
    src/bindings/popup.cpp
    src/bindings/task_bindings.cpp
    src/bindings/version_color.cpp
    src/bindings/websocket_bindings.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE RUSTAPI_EXPORTING)
target_include_directories(${PROJECT_NAME} PUBLIC include src $<TARGET_PROPERTY:imgui-cocos,INTERFACE_INCLUDE_DIRECTORIES>)
target_link_libraries(${PROJECT_NAME} geode-sdk imgui-cocos ${CMAKE_CURRENT_SOURCE_DIR}/target/release/rustapi.lib ws2_32 userenv bcrypt ntdll)
setup_geode_mod(${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} rust_lib)

if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<NOT:$<CONFIG:Debug>>:LINKER:/OPT:REF>
        $<$<NOT:$<CONFIG:Debug>>:LINKER:/OPT:ICF>)
endif()
