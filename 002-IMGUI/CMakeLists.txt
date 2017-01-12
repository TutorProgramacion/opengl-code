project(002-IMGUI)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

set(SOURCE_FILES main.cpp imgui_impl_glfw_glad.cpp imgui_impl_glfw_glad.h)
set(IMGUI_LIB imgui)

add_executable(${PROJECT_NAME} ${SHADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBS} ${IMGUI_LIB})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TUTORIALES OPENGL MODERNO")
