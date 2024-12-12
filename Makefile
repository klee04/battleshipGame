cppFileNames := $(shell find ./src -type f -name "*.cpp")

all: compile

compile:	
	mkdir -p bin
	g++ -std=c++17 $(cppFileNames) -I/include/SFML/include -o bin/app -L/include/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network