include_guard()

project(RayTracingInAWeekend VERSION 1.0.0.0 LANGUAGES CXX)

set(INTERFACE_INC_FOLDER ${PROJECT_SOURCE_DIR}/inc)
set(PRIVATE_INC_FOLDER ${PROJECT_SOURCE_DIR}/inc/tsleddens)
set(PRIVATE_MATERIALS_INC_FOLDER ${PRIVATE_INC_FOLDER}/Materials)
set(PRIVATE_TEXTURES_INC_FOLDER ${PRIVATE_INC_FOLDER}/Textures)
set(SRC_FOLDER ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES
    ${SRC_FOLDER}/*.cxx
)

file(GLOB_RECURSE INC_FILES
    ${PRIVATE_INC_FOLDER}/*.h
    ${PRIVATE_MATERIALS_INC_FOLDER}/*.h
    ${PRIVATE_TEXTURES_INC_FOLDER}/*.h
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        ${SRC_FILES}
        ${INC_FILES}
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${INTERFACE_INC_FOLDER}
    PRIVATE
        ${PRIVATE_INC_FOLDER}
        ${stb_SOURCE_DIR}
)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC
        <Windows.h>
        <string>
        <spdlog/spdlog.h>
    PRIVATE
        [["Defines.h"]]
    INTERFACE
        [["tsleddens/Defines.h"]]
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        /W4
)

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        _UNICODE
        UNICODE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        spdlog::spdlog
        glm::glm
        stb::stb
        SDL3::SDL3
)