cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(queue VERSION ${PROJECT_VESRION})
#####################################################################
find_package(Boost  COMPONENTS serialization program_options REQUIRED)
add_library(boost::serialization INTERFACE IMPORTED)
set_target_properties(boost::serialization PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${Boost_SERIALIZATION_LIBRARIES}"
)
#####################################################################
option(WITH_BOOST_TEST "Whether to build Boost test" ON)

configure_file(version.h.in version.h)

add_executable(net_queue_cli src/main.cpp)


add_library( queue_lib 
             src/base/boost_json.cpp 
             src/base/config.cpp
)

add_library( queue_lib_net 
             src/base/boost_json.cpp 
             src/base/config.cpp
             src/net/server.cpp
             src/net/session.cpp
)

set_target_properties(net_queue_cli PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

include_directories(
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/base" 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/net"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/serialize"
)


target_link_libraries(net_queue_cli PRIVATE
   queue_lib_net 
    ${Boost_LIBRARIES}
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_queue test/test_queue.cpp)

    set_target_properties(test_queue PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_queue PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )
    target_include_directories(test_queue 
    PRIVATE
    "${CMAKE_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/base" 
    "${CMAKE_CURRENT_SOURCE_DIR}/include/net"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/serialize"
)

    target_link_libraries(test_queue
        ${Boost_LIBRARIES}
        queue_lib    
        boost::serialization
    )
endif()

if (MSVC)
    target_compile_options(queue_cli PRIVATE
        /W4
    )
    target_compile_options(queue_lib PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_queue PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(net_queue_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(queue_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_queue PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS net_queue_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
#set(CPACK_GENERATOR RPM)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_queue test_queue)
endif()