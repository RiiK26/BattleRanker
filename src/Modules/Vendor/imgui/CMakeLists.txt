add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    backends/imgui_impl_win32.cpp
    backends/imgui_impl_dx12.cpp
)
target_include_directories(imgui PUBLIC . backends)
