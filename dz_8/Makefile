#
.PHONY: all clean

all: 
	cmake -DCMAKE_BUILD_TYPE=Debug -B  build 
	cmake --build build
	cmake --build build --target test
clean:
	rm -rf build