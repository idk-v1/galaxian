exe:	main.cpp
	g++ *.cpp src/*.cpp src/*.c galaxian/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes

debug:	main.cpp
	g++ *.cpp src/*.cpp src/*.c galaxian/*.cpp `sdl-config --cflags --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -Iincludes -fsanitize=address 

run:
	./a.out

r:
	./a.out

clean:
	rm a.out

c:
	rm a.out
