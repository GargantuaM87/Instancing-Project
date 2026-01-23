cmake_minimum_required(VERSION 3.10.0)
project(renderProject VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(renderProject 
src/main.cpp
src/shaderClass.cpp
src/VAO.cpp
src/VBO.cpp
src/EBO.cpp
src/FBO.cpp
src/glad.h 
src/glad.c
src/stb.cpp
src/texture.cpp
src/camera.cpp
src/uniformClass.cpp
src/model.cpp
src/mesh.cpp
src/imgui/imgui_demo.cpp
src/imgui/imgui_draw.cpp
src/imgui/imgui_impl_glfw.cpp
src/imgui/imgui_impl_opengl3.cpp
src/imgui/imgui_tables.cpp
src/imgui/imgui_widgets.cpp
src/imgui/imgui.cpp)

target_include_directories(renderProject PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/headers
	${CMAKE_CURRENT_SOURCE_DIR}/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(renderProject glfw assimp OpenGL::GL)
