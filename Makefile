default: build

build:
	mkdir -p bin
	gcc src/main.c -o bin/run -lSDL2 -lSDL2_image