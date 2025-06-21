cmake_minimum_required (VERSION 3.20)

project (RayTracingInAWeekend VERSION 1.0.0.0 LANGUAGES CXX)

# fetch dependencies
include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.3
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
  sdl
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.16
)
FetchContent_MakeAvailable(sdl)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
)
FetchContent_MakeAvailable(stb)

# stb does not have CMake support: adding it here
add_library(stb_interface INTERFACE)
target_include_directories(stb_interface INTERFACE ${stb_SOURCE_DIR})
add_library(stb::stb ALIAS stb_interface)

# add subdirectories
add_subdirectory(RayTracingInAWeekend)
add_subdirectory(Samples)

# add clang-format target
find_program(CLANG_FORMAT_BIN clang-format)
if(CLANG_FORMAT_BIN)
    file(GLOB_RECURSE ALL_SOURCE_FILES
         "*.h"
         "*.cxx")

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_BIN}
                -i
                --style=file
                ${ALL_SOURCE_FILES}
        COMMENT "Formatting all source files with clang-format"
    )
endif()
