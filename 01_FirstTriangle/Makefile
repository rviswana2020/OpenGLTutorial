CC = clang++-13
CFLAGS = -std=c++17 -O2
LDFLAGS = -lGL -lGLEW -lglfw -lm

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -g
else
	CXXFLAGS += -DNDEBUG
endif

FirstTriangle: FirstTriangle.cpp
	$(CC) $(CFLAGS) -o FirstTriangle FirstTriangle.cpp $(LDFLAGS)

.PHONY: test clean

test: FirstTriangle
	./FirstTriangle

clean:
	rm FirstTriangle
