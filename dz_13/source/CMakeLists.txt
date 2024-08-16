cmake_minimum_required(VERSION 3.8)

project(cv LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_CATCH2_TEST "Whether to build Catch2 test" ON)

set(lib_src 
    reader.hpp
    reader.cpp
    interface_binary_classifier.hpp
    logreg_classifier.hpp
    logreg_classifier.cpp        
)
add_library(lib_fashion STATIC ${lib_src})

add_executable(fashion_mnist
    main.cpp
    
)
target_link_libraries(fashion_mnist PUBLIC lib_fashion)
########################################################################
#        TEST
########################################################################
if(WITH_CATCH2_TEST)
    Include(FetchContent)
    FetchContent_Declare( 
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.4.0
    )
    
    FetchContent_MakeAvailable(Catch2)
    
    add_executable(test_fashion ${PROJECT_SOURCE_DIR}/test/test_fashion.cpp)

    set_target_properties(test_fashion PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    target_link_libraries(test_fashion      
        PRIVATE lib_fashion
        PRIVATE Catch2::Catch2WithMain        
    )   
endif()

if(WITH_CATCH2_TEST)
    enable_testing()
    add_test(NAME test  COMMAND test_fashion)
endif()
########################################################################