cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD_REQUIRED 17)

if(DEFINED ENV{GITHUB_RUN_NUMBER})
  set(PATCH_VERSION $ENV{GITHUB_RUN_NUMBER})
else()
  set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
endif()


project(hello
  LANGUAGES CXX
  VERSION 0.0.${PATCH_VERSION}
)


include_directories(${CMAKE_BINARY_DIR})
include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup(TARGETS)

configure_file(version.h.in version.h)

option(BUILD_TESTS "Build unittests" OFF)



add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS hello_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)