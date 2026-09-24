cmake_minimum_required(VERSION 3.16)

project(rendrX LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(rendrX ${SOURCES})

target_sources(rendrX PRIVATE
    external/glad/src/gl.c
)

target_link_libraries(rendrX PRIVATE
    OpenGL::GL
    glfw
    glm::glm
)

target_include_directories(rendrX PRIVATE
    src
    external/glad/include
)

target_compile_options(rendrX PRIVATE
    -O3
)

if(MINGW)
	target_link_options(rendrX PRIVATE
		-static
		-static-libgcc
		-static-libstdc++
	)
endif()
