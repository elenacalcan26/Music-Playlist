build: tema1

tema1: MusicPlayer.o utils.o
	gcc -Wall -Wextra -std=c99 MusicPlayer.o utils.o -o tema1

utils.o: utils.h utils.c
	gcc -Wall -Wextra -std=c99 utils.c -c -o utils.o

MusicPlayer.o: MusicPlayer.c
	gcc -Wall -Wextra -std=c99 MusicPlayer.c -c -o MusicPlayer.o

clean:
		rm *.o
		rm tema1	
