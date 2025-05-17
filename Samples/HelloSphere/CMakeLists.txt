include_guard()

project(HelloSphere VERSION 1.0.0.0 LANGUAGES CXX)

set(INC_FILES 
    "main.cxx"
    "HelloSphere.cxx"
)

set(SRC_FILES
    "HelloSphere.h"
)

if("${CMAKE_BUILD_TYPE}" STREQUAL Debug)
    add_executable (${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})
else()
    add_executable (${PROJECT_NAME} WIN32 ${SRC_FILES} ${INC_FILES})
endif()

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LINK_FLAGS
            "/PROFILE"
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        RayTracingInAWeekend
    PRIVATE
        glm::glm
)