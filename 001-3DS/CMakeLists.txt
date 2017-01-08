project(001-3DS)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

add_executable(${PROJECT_NAME} 3DSLoader.cpp)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TUTORIALES OPENGL MODERNO")
