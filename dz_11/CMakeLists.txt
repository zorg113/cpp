cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VESRION})


option(WITH_CATCH2_TEST "Whether to build Catch2 test" ON)

set(Boost_USE_STATIC_LIBS ON)

configure_file( ${PROJECT_SOURCE_DIR}/lib/version.h.in version.h)

find_package(Boost COMPONENTS program_options filesystem REQUIRED)


include_directories( ${PROJECT_SOURCE_DIR}/lib/include )

set(database_lib_src 
            lib/src/database.cpp
            lib/src/parser.cpp            
)
 

add_library(libdatabase_static STATIC ${database_lib_src})
add_library(libdatabase SHARED ${database_lib_src})

set_target_properties(libdatabase PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(libdatabase_static PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(libdatabase
                           PRIVATE 
                          "${CMAKE_BINARY_DIR}"                          
)


target_include_directories(libdatabase_static
                             PRIVATE 
                            "${CMAKE_BINARY_DIR}"                          
)

Find_package(Threads)

target_link_libraries( libdatabase_static
                ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries( libdatabase 
                ${CMAKE_THREAD_LIBS_INIT})

###########################################
######## Examples test ####################
add_executable(join_server "server/main.cpp" "server/server.cpp" "server/session.cpp")

set_target_properties(
    join_server
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(join_server PUBLIC libdatabase ${Boost_LIBRARIES})
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
    
    add_executable(test_database ${PROJECT_SOURCE_DIR}/test/test_parse_cmd.cpp)

    set_target_properties(test_database PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(test_database      
        PRIVATE libdatabase
        PRIVATE Catch2::Catch2WithMain        
    )   
endif()



#####################################
#### Compiler options ###############
######################################
if (MSVC)
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(bulk_async_cli PRIVATE
        /W4
    )
  if(WITH_CATCH2_TEST)
        target_compile_options(test PRIVATE
            /W4
        )
    endif()
else ()
   target_compile_options( libdatabase  PRIVATE
        -Wall -Wextra -g3 -pedantic -Werror 
    )
    #-fPIC -pie -g  -fsanitize=thread 
    target_compile_options(join_server PRIVATE
         -Wall -Wextra -g3 -pedantic -Werror
    )
    if(WITH_CATCH2_TEST)
        target_compile_options(test_database PRIVATE
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
install(TARGETS libdatabase RUNTIME DESTINATION bin/lib/)
install(TARGETS libdatabase_static RUNTIME DESTINATION bin/lib/)
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
    add_test(NAME test  COMMAND test_database )
endif()

