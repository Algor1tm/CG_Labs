cmake_minimum_required (VERSION 3.24)

project(glad C)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
	"include/glad/glad.h" 
    "include/KHR/khrplatform.h" 
	"src/glad.c" 
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES} )

add_library(glad ${SRC_FILES})

target_include_directories(glad PRIVATE 
	"include" 
)
