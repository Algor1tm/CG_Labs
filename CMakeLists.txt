cmake_minimum_required (VERSION 3.24)

set(BUILD_FOLDER "${CMAKE_SOURCE_DIR}/Build/Binaries")
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/ThirdParty")
set(PROJECT_NAME Lab1)

macro(EnableMPCompilation TARGET_NAME)
    if(MSVC)
	    target_compile_options(${TARGET_NAME} PUBLIC /MP)
    endif(MSVC)
endmacro()


function(AddLibrary TARGET_NAME TARGET_PATH)
    add_subdirectory("${TARGET_PATH}")

    EnableMPCompilation(${TARGET_NAME})
    set_target_properties(${TARGET_NAME}
	PROPERTIES
    FOLDER Dependencies
	ARCHIVE_OUTPUT_DIRECTORY "${BUILD_FOLDER}/ThirdParty/${TARGET_NAME}"
	LIBRARY_OUTPUT_DIRECTORY "${BUILD_FOLDER}/ThirdParty/${TARGET_NAME}"
	RUNTIME_OUTPUT_DIRECTORY "${BUILD_FOLDER}/ThirdParty/${TARGET_NAME}")

    target_link_libraries(${PROJECT_NAME} PRIVATE ${TARGET_NAME})
endfunction()


project(${PROJECT_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS 
	"Source/*.h" 
	"Source/*.cpp" 
    "Shaders/*.vert"
    "Shaders/*.frag"
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC_FILES} )

add_executable(${PROJECT_NAME} ${SRC_FILES})

EnableMPCompilation(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
    CXX_STANDARD 20
    ARCHIVE_OUTPUT_DIRECTORY "${BUILD_FOLDER}/${PROJECT_NAME}"
	LIBRARY_OUTPUT_DIRECTORY "${BUILD_FOLDER}/${PROJECT_NAME}"
	RUNTIME_OUTPUT_DIRECTORY "${BUILD_FOLDER}/${PROJECT_NAME}"
    VS_DEBUGGER_WORKING_DIRECTORY  "${CMAKE_SOURCE_DIR}"
)


target_include_directories(${PROJECT_NAME} PRIVATE 
	"Source" 
    "${THIRD_PARTY_DIR}/glad/include"
    "${THIRD_PARTY_DIR}/GLFW/include"
    "${THIRD_PARTY_DIR}/glm"
)

AddLibrary("glad" "${THIRD_PARTY_DIR}/glad")
AddLibrary("GLFW" "${THIRD_PARTY_DIR}/GLFW")
AddLibrary("glm" "${THIRD_PARTY_DIR}/glm")
