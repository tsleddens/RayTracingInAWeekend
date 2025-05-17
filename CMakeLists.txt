cmake_minimum_required (VERSION 3.20)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

project (RayTracingInAWeekend VERSION 1.0.0.0 LANGUAGES CXX)

find_package(spdlog CONFIG REQUIRED) #vcpkg install spdlog
find_package(glm CONFIG REQUIRED) #vcpkg install glm

add_subdirectory(RayTracingInAWeekend)
add_subdirectory(Samples)