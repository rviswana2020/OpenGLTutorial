CC = clang++-13
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lGLEW -lGL -lm

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -g
else
	CXXFLAGS += -DNDEBUG
endif

basecode: basecode.cpp
	$(info, $(CXXFLAGS))
	$(CC) $(CXXFLAGS) -o basecode basecode.cpp $(LDFLAGS)

.PHONY: test clean

test: basecode
	./basecode

clean:
	rm -rf basecode

#USAGE:
### make clean; 
### make DEBUG=0 test
### make DEBUG=1 test
