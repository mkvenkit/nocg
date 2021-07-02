cmake_minimum_required(VERSION 3.10)

project(nocg LANGUAGES C CXX)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenGL)

# add common library 
add_library(nocg_common 
    src/common/Axis3D.cpp
    src/common/Camera.cpp
    src/common/Utils.cpp
    src/common/Plane.cpp
    src/common/RenderApp.cpp
)

# add glad 
add_library(glad 
    external/glad/src/glad.c)

set(COMMON_LIBS nocg_common glad glfw3 ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})

# torus
add_executable(torus 
    src/torus/Torus.cpp
    src/torus/TorusApp.cpp
    src/torus/main.cpp
)
target_link_libraries(torus PUBLIC ${COMMON_LIBS})

# Cube - Tessellation and Geometry shaders
add_executable(cubetg
    src/cubetg/CubeTGApp.cpp
    src/cubetg/PlaneTG.cpp
    src/cubetg/main.cpp
)
target_link_libraries(cubetg PUBLIC ${COMMON_LIBS})


include_directories(src/common)
include_directories(external/glfw-3.3.4/include)
include_directories(external/glm-0.9.9.8/glm)
include_directories(external/glad/include)
include_directories(external/stb)
