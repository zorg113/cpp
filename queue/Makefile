#
.PHONY: all clean

all:
	cd build && conan install .. --output-folder=build -s build_type=Release --build=missing
	cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
	cmake --build build
	cmake --build build --target test
clean:
	rm -rf build
	mkdir build
