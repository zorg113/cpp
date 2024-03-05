add_executable(hello-tests test_version.cpp)

target_link_libraries(hello-tests hello CONAN_PKG::boost)

add_test(HelloTest "${CMAKE_BINARY_DIR}/tests/hello-tests")