cmake_minimum_required(VERSION 3.6)
project(minigl)

include_directories(${minigl_SOURCE_DIR})


set(ULTIMATE_WARNINGS "-Wdisabled-optimization -Wlogical-op -Wwrite-strings -Wmissing-include-dirs -Wredundant-decls -Wsign-promo -Wswitch-default -Wundef")
set(EXTRA_WARNINGS "-Woverloaded-virtual -Wcast-qual -Wcast-align -Wunreachable-code -Wold-style-cast -Wuseless-cast -Wnon-virtual-dtor -Wodr")
# no -Wzero-as-null-pointer-constant because of VK_NULL_HANDLE

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_definitions("${EXTRA_WARNINGS}")
add_definitions("${ULTIMATE_WARNINGS}")
add_definitions("-std=c++17 -pedantic -Wall -Wextra")


file(GLOB_RECURSE SOURCE_FILES
		"main.cpp"

		"src/*.h"
		"src/*.cpp"

		"shaders/*"
	)

# Shader files, they are here so the IDE can find them
file(GLOB_RECURSE SHADER_FILES
		"shaders/*.frag"
		"shaders/*.vert"
		"shaders/*.geom"
		"shaders/*.comp"
		"shaders/*.glsl"
	)


# y lib, change this to wherever it it
include_directories(../y)
link_directories(../y/build)

add_executable(minigl "${SOURCE_FILES}")

target_link_libraries(minigl y)
target_link_libraries(minigl opengl32)
target_link_libraries(minigl glew32)

target_link_libraries(minigl mingw32 SDL2 SDL2main SDL2.dll)



