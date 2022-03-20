cmake_minimum_required(VERSION 3.16)

project(shadertool LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)

pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
	libavcodec
	libavformat
	libavdevice
	libavutil
	libswscale
)

pkg_check_modules(OPENGL REQUIRED IMPORTED_TARGET
	glesv2
)

pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET
	glfw3
)

pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET
	glew
)

file(GLOB_RECURSE SOURCES src/*.cc)
include_directories(src/)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PkgConfig::FFMPEG PkgConfig::OPENGL PkgConfig::GLFW PkgConfig::GLEW)

