cmake_minimum_required(VERSION 3.4)

project("Tutoriales OpenGL Moderno")

set(EXTRA_LIB_DIR "library" CACHE STRING "Directorio de bibliotecas externas")

if(MSVC)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS "/MP")
else()
    include(CheckCXXCompilerFlag)

    check_cxx_compiler_flag("-std=c++14" HAS_CPP14_FLAG)
    check_cxx_compiler_flag("-std=c++11" HAS_CPP11_FLAG)

    if(NOT HAS_CPP14_FLAG OR NOT HAS_CPP11_FLAG)
        message(FATAL_ERROR "Se requiere un compilador con soporte para C++ 11 y C++ 14")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -std=c++14")    
endif()

add_subdirectory(${EXTRA_LIB_DIR}/glfw)
add_subdirectory(${EXTRA_LIB_DIR}/glad)
add_subdirectory(${EXTRA_LIB_DIR}/imgui)

include_directories(${EXTRA_LIB_DIR}/glfw/include)
include_directories(${EXTRA_LIB_DIR}/glad/include)
include_directories(${EXTRA_LIB_DIR}/glm/include)
include_directories(${EXTRA_LIB_DIR}/imgui/include)

set(OPENGL_LIBS glad glfw ${GLFW_LIBRARIES})

add_subdirectory(001-3DS)
add_subdirectory(002-IMGUI)
