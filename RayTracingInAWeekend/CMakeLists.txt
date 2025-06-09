include_guard()

project(RayTracingInAWeekend VERSION 1.0.0.0 LANGUAGES CXX)

set(INTERFACE_INC_FOLDER ${PROJECT_SOURCE_DIR}/inc)
set(PRIVATE_INC_FOLDER ${PROJECT_SOURCE_DIR}/inc/tsleddens)
set(PRIVATE_MATERIALS_INC_FOLDER ${PRIVATE_INC_FOLDER}/Materials)
set(PRIVATE_TEXTURES_INC_FOLDER ${PRIVATE_INC_FOLDER}/Textures)
set(INC_FILES 
    ${PRIVATE_INC_FOLDER}/AABB.h
    ${PRIVATE_INC_FOLDER}/AppBase.h
    ${PRIVATE_INC_FOLDER}/Aligned2DArray.h
    ${PRIVATE_INC_FOLDER}/BufferedGdiBitmap.h
    ${PRIVATE_INC_FOLDER}/BvhNode.h
    ${PRIVATE_INC_FOLDER}/Camera.h
    ${PRIVATE_INC_FOLDER}/Defines.h
    ${PRIVATE_INC_FOLDER}/DeFocusDisk.h
    ${PRIVATE_INC_FOLDER}/HitResult.h
    ${PRIVATE_INC_FOLDER}/IRayTraceable.h
    ${PRIVATE_INC_FOLDER}/Range.h
    ${PRIVATE_INC_FOLDER}/Ray.h
    ${PRIVATE_INC_FOLDER}/RtwImage.h
    ${PRIVATE_INC_FOLDER}/Sphere.h
    ${PRIVATE_INC_FOLDER}/Win32Rasterizer.h
    ${PRIVATE_INC_FOLDER}/World.h
    
    ${PRIVATE_MATERIALS_INC_FOLDER}/Dielectric.h
    ${PRIVATE_MATERIALS_INC_FOLDER}/IMaterial.h
    ${PRIVATE_MATERIALS_INC_FOLDER}/Lambertian.h
    ${PRIVATE_MATERIALS_INC_FOLDER}/Metal.h
    
    ${PRIVATE_TEXTURES_INC_FOLDER}/ITexture.h
    ${PRIVATE_TEXTURES_INC_FOLDER}/ColorTexture.h
    ${PRIVATE_TEXTURES_INC_FOLDER}/CheckerTexture.h
    ${PRIVATE_TEXTURES_INC_FOLDER}/ImageTexture.h
)

set(SRC_FOLDER ${PROJECT_SOURCE_DIR}/src)
set(SRC_FILES 
    ${SRC_FOLDER}/AABB.cxx
    ${SRC_FOLDER}/AppBase.cxx
    ${SRC_FOLDER}/BufferedGdiBitmap.cxx
    ${SRC_FOLDER}/BvhNode.cxx
    ${SRC_FOLDER}/Camera.cxx
    ${SRC_FOLDER}/DeFocusDisk.cxx
    ${SRC_FOLDER}/Sphere.cxx
    ${SRC_FOLDER}/Win32Rasterizer.cxx
    ${SRC_FOLDER}/World.cxx
)

add_library(${PROJECT_NAME} STATIC
    ${SRC_FILES}
    ${INC_FILES})

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${INTERFACE_INC_FOLDER}
    PRIVATE
        ${PRIVATE_INC_FOLDER}
        ${Stb_INCLUDE_DIR}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}\\inc\\tsleddens PREFIX "Header Files" FILES ${INC_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}\\src PREFIX "Source Files" FILES ${SRC_FILES})

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