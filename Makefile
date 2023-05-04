default: build

build:
	mkdir -p bin
	gcc src/main.c -o bin/run -lSDL2 -lSDL2_image -lhmxobj

genclang: 
	bear -- clang -lSDL2 -lSDL2_image -lhmxobj src/main.c -o bin/run.clang
	rm bin/run.clang