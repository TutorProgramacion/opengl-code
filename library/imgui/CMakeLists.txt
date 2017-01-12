cmake_minimum_required(VERSION 3.4)

project("imgui")

set(SRC imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp)

set(HEADER include/imgui/imconfig.h
           include/imgui/imgui.h
           imgui/imgui_internal.h
           imgui/stb_rect_pack.h
           imgui/stb_textedit.h
           imgui/stb_truetype.h )

include_directories(include/imgui)

add_library(${PROJECT_NAME} STATIC ${SRC} ${HEADER})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER IMGUI)