cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(allocate VERSION ${PROJECT_VESRION})

option(WITH_CATCH2_TEST "Whether to build Catch2 test" ON)

configure_file(version.h.in version.h)

add_executable(allocate_cli main.cpp)
add_library(allocate lib.cpp)

set_target_properties(allocate_cli allocate PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(allocate
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(allocate_cli PRIVATE
    allocate
)

if(WITH_CATCH2_TEST)
    Include(FetchContent)
    FetchContent_Declare(
      Catch2 
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)
    add_executable(test_alloc test_allocate.cpp)

    set_target_properties(test_alloc PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(test_alloc
        PRIVATE allocate
        PRIVATE Catch2::Catch2WithMain
    )
endif()

if (MSVC)
    target_compile_options(allocate_cli PRIVATE
        /W4
    )
    target_compile_options(allocate PRIVATE
        /W4
    )
  if(WITH_CATCH2_TEST)
        target_compile_options(test PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(allocate_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(allocate PRIVATE
        -Wall -Wextra -pedantic -Werror
    )     
endif()

install(TARGETS allocate_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
#set(CPACK_GENERATOR RPM)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_CATCH2_TEST)
    enable_testing()
    add_test(test test_alloc)
endif()
