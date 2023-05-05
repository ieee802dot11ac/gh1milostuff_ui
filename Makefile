default: genclang build

build:
	mkdir -p bin
	mkdir -p obj
	gcc -c src/main.c -o obj/run.o -g
	gcc obj/run.o -lSDL2 -lSDL2_image -lhmxobj -g -o bin/run


genclang: 
	bear -- clang -lSDL2 -lSDL2_image -lhmxobj src/main.c -o bin/run.clang
	rm bin/run.clang
