include_guard()

project(HelloSphere VERSION 1.0.0.0 LANGUAGES CXX)

set(ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

set(SRC_FILES
    "main.cxx"
)

set(INC_FILES
    "BouncingSpheres.h"
    "Earth.h"
    "HelloSphere.h"
)

add_executable (${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS"
        LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS"
        LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS"
        LINK_FLAGS "/PROFILE"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        RayTracingInAWeekend
    PRIVATE
        glm::glm
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSETS_DIR}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)