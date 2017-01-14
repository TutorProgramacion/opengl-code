project(003-PAR_SHAPES)

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}/")

set(SOURCE_FILES shapes par_shapes.h)

add_executable(${PROJECT_NAME}  ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "TUTORIALES OPENGL MODERNO")
