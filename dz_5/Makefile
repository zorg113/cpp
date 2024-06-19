#
.PHONY: all clean

all: 
	cmake -B build
	cmake --build build
	cmake --build build --target test
	cmake --build build --target doc
clean:
	rm -rf build