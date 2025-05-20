include_guard()

project(RayTracingInAWeekend VERSION 1.0.0.0 LANGUAGES CXX)

set(INTERFACE_INC_FOLDER ${PROJECT_SOURCE_DIR}/inc)
set(PRIVATE_INC_FOLDER ${PROJECT_SOURCE_DIR}/inc/tsleddens)
set(INC_FILES 
    ${PRIVATE_INC_FOLDER}/IRayTraceable.h
    ${PRIVATE_INC_FOLDER}/IMaterial.h
    ${PRIVATE_INC_FOLDER}/Defines.h
    ${PRIVATE_INC_FOLDER}/HitResult.h
    ${PRIVATE_INC_FOLDER}/Material.h
    ${PRIVATE_INC_FOLDER}/Ray.h
    ${PRIVATE_INC_FOLDER}/Sphere.h
    ${PRIVATE_INC_FOLDER}/Win32Rasterizer.h
)

set(SRC_FOLDER ${PROJECT_SOURCE_DIR}/src)
set(SRC_FILES 
    ${SRC_FOLDER}/Material.cxx
    ${SRC_FOLDER}/Sphere.cxx
    ${SRC_FOLDER}/Win32Rasterizer.cxx
)

add_library(${PROJECT_NAME} STATIC
    ${SRC_FILES}
    ${INC_FILES})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${INTERFACE_INC_FOLDER}
    PRIVATE
        ${PRIVATE_INC_FOLDER}
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

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
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
)