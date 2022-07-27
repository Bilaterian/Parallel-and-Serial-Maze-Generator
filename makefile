CC = gcc

make: maze mazep

maze: maze.o
	$(CC) maze.o -o maze

maze.o: maze.c
	$(CC) -c maze.c -o maze.o

mazep: mazep.o
	$(CC) mazep.o -fopenmp -o mazep

mazep.o: maze.c
	$(CC) -fopenmp -c maze.c -o mazep.o

rm:
	rm *.o maze mazep