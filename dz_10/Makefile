#
.PHONY: all clean

#-DCMAKE_EXE_LINKER_FLAGS="-fsanitize=thread"  -DCMAKE_BUILD_TYPE=Debug  
all: 
	cmake -B build 
	cmake --build build
	cmake --build build --target  test
	cmake --build build --target package
clean:
	rm -rf build