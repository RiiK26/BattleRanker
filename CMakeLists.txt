cmake_minimum_required(VERSION 3.20)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/config.json" VERSION_JSON_CONTENT)
string(JSON PARSED_VERSION GET "${VERSION_JSON_CONTENT}" "game_version")
string(JSON PARSED_PROJECT_NAME GET "${VERSION_JSON_CONTENT}" "project_name")

project(${PARSED_PROJECT_NAME}
    VERSION ${PARSED_VERSION}
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "-static -static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-static -static-libgcc -static-libstdc++")

# Dependencies (compiled directly into the DLL)
file(GLOB MINHOOK_SRC "src/Modules/Vendor/minhook/src/*.c" "src/Modules/Vendor/minhook/src/hde/*.c")

add_library(${PROJECT_NAME} MODULE
    src/main.cpp
    src/Modules/Hooks/Hooks.cpp
    src/Modules/Hooks/Signatures.cpp
    src/Modules/Menu/Menu.cpp
    src/Modules/Utils/MonoUtils.cpp
    src/Features/Player/GodMode.cpp
    src/Features/Player/Upgrades.cpp
    src/Features/Player/DetailedStats.cpp
    src/Features/Currency/InfiniteCurrency.cpp
    src/Features/Currency/SetCurrency.cpp
    src/Features/Misc/BypassACTk.cpp

    # ImGui sources
    src/Modules/Vendor/imgui/imgui.cpp
    src/Modules/Vendor/imgui/imgui_draw.cpp
    src/Modules/Vendor/imgui/imgui_tables.cpp
    src/Modules/Vendor/imgui/imgui_widgets.cpp
    src/Modules/Vendor/imgui/backends/imgui_impl_win32.cpp
    src/Modules/Vendor/imgui/backends/imgui_impl_dx12.cpp

    # MinHook sources
    ${MINHOOK_SRC}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release"
    LINK_FLAGS "-static -static-libgcc -static-libstdc++"
)

add_compile_definitions(
    PROJECT_VERSION=\"${PROJECT_VERSION}\"
    PROJECT_ROOT=\"${CMAKE_BINARY_DIR}/release\"
    NDEBUG # Disables asserts, hiding ImGui file paths
)

target_include_directories(${PROJECT_NAME} PRIVATE src/Modules/Utils src/Modules/Menu src/Modules/Hooks src/Modules/Vendor/minhook/include src/Modules/Vendor/imgui src/Modules/Vendor/imgui/backends)
target_link_libraries(${PROJECT_NAME} PRIVATE d3d12 dxgi dwmapi d3dcompiler)

# Build the injector separately
add_executable(injector src/injector.cpp)
set_target_properties(injector PROPERTIES
    SUFFIX ".exe"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/release"
)
