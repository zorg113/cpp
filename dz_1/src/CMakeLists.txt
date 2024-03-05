add_library(hello lib.cpp)

target_compile_features(hello PUBLIC cxx_std_17)
target_include_directories(hello INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(hello_cli main.cpp)
target_link_libraries(hello_cli hello)