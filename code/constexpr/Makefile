CWD := $(PWD)
CXXFLAGS := -std=c++1y
WFLAGS := -Wall -Wextra -pedantic -Wno-unused-const-variable
INCLUDES := -I $(CWD)/..
CC := clang++-3.5

all: integral.test list.test type.test if.test

build_dir:
	mkdir -p build

%.test: test/%.cpp build_dir
	$(CC) $(WFLAGS) $(CXXFLAGS) $(INCLUDES) -o build/$@ $< && ./build/$@
