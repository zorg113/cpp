cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bulk VERSION ${PROJECT_VESRION})

option(WITH_CATCH2_TEST "Whether to build Catch2 test" ON)

configure_file( ${PROJECT_SOURCE_DIR}/lib/version.h.in version.h)




include_directories( ${PROJECT_SOURCE_DIR}/lib/include )

set( aysnc_lib_src 
            lib/src/async.cpp
            lib/src/lib.cpp
            lib/src/bulk.cpp
            lib/src/parse_cmd.cpp
)
 

add_library(libasync_static STATIC ${aysnc_lib_src})
add_library(libasync SHARED ${aysnc_lib_src})

set_target_properties(libasync PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(libasync_static PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(libasync
                           PRIVATE 
                          "${CMAKE_BINARY_DIR}"                          
)


target_include_directories(libasync_static
                             PRIVATE 
                            "${CMAKE_BINARY_DIR}"                          
)

Find_package(Threads)

target_link_libraries( libasync_static
                ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries( libasync 
                ${CMAKE_THREAD_LIBS_INIT})

###########################################
######## Examples test ####################
add_executable(bulk_async_cli "test/main.cpp")

set_target_properties(
    bulk_async_cli
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(bulk_async_cli PUBLIC libasync)
####################################
###### Test block ##################
if(WITH_CATCH2_TEST)
    Include(FetchContent)
    FetchContent_Declare( 
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.4.0
    )
    
    FetchContent_MakeAvailable(Catch2)
    
    add_executable(test_parse_cmd ${PROJECT_SOURCE_DIR}/test/test_parse_cmd.cpp)

    set_target_properties(test_parse_cmd PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(test_parse_cmd      
        PRIVATE libasync
        PRIVATE Catch2::Catch2WithMain        
    )   
endif()



#####################################
#### Compiler options ###############
######################################
if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
    target_compile_options(parse_cmd_lib PRIVATE
        /W4
    )
  if(WITH_CATCH2_TEST)
        target_compile_options(test PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(bulk_async_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_CATCH2_TEST)
        target_compile_options(test_parse_cmd PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

###############################
###### Documentation ##########
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    configure_file(Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile)
    add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile -o ${CMAKE_BINARY_DIR}/doc)
else()
    message(STATUS "WARNING: doxygen not found - target \"doc\" will not be available")
endif()

###############################
####### Install Package ######
##############################
install(TARGETS libasync RUNTIME DESTINATION bin/lib/)
install(TARGETS libasync_static RUNTIME DESTINATION bin/lib/)
#install(TARGETS ${PROJECT_SOURCE_DIR}/lib/include/async.h DESTINATION bin/include/)

set(CPACK_GENERATOR DEB)
#set(CPACK_GENERATOR RPM)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_CATCH2_TEST)
    enable_testing()
    add_test(NAME test  COMMAND test_parse_cmd )
endif()

