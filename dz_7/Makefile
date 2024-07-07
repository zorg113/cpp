#
.PHONY: all clean

all: 
	cmake -B build
	cmake --build build
	cmake --build build --target  test
clean:
	rm -rf build