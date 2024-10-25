cmake_minimum_required(VERSION 3.29)
project(PERSONAJE3D)

set(CMAKE_CXX_STANDARD 20)

# Encuentra OpenGL
find_package(OpenGL REQUIRED)

# Encuentra GLUT
find_package(GLUT REQUIRED)

# Encuentra GLU
find_package(GLU REQUIRED)

add_executable(PERSONAJE3D main.cpp)

# Enlaza OpenGL, GLUT y GLU
target_link_libraries(PERSONAJE3D OpenGL::GL GLUT::GLUT GLU)
