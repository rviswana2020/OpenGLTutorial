CC = clang++-13
CXXFLAGS = -std=c++17 -O2
LDFLAGS = -lGL -lGLEW -lglfw -lm

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG -g
else
	CXXFLAGS += -DNDEBUG
endif

Shaders: Shaders.cpp
	$(CC) $(CXXFLAGS) -o Shaders main.cpp Shaders.cpp $(LDFLAGS)

.PHONY: test clean

test: Shaders
	./Shaders

clean:
	rm Shaders
