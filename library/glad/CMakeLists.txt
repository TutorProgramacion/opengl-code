cmake_minimum_required(VERSION 2.8)

project(GLAD)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include_directories(include)

add_library(glad STATIC src/glad.c)

set_target_properties(glad PROPERTIES FOLDER "GLAD")