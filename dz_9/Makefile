#
.PHONY: all clean

all: 
	cmake -B build  -DCMAKE_BUILD_TYPE=Debug
	cmake --build build
	cmake --build build --target  test
	cmake --build build --target package
clean:
	rm -rf build