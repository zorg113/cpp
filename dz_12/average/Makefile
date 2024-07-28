.PHONY: all clean

all: bin/mapper bin/reducer

bin/mapper: mapper.cpp
	mkdir -p bin
	g++ -o $@ mapper.cpp

bin/reducer: reducer.cpp
	mkdir -p bin
	g++ -o $@ reducer.cpp

clean:
	rm -rf bin output