# Objects
OBJS = main.o Console.o GameCtrl.o Grid.o Map.o Point.o Snake.o

# Compiler
CC = g++ -std=c++11

# Library
LIB = -pthread

Snake: $(OBJS)
	$(CC) -o Snake $(OBJS) $(LIB)

main.o: main.cpp
	$(CC) -c main.cpp

GameCtrl.o: GameCtrl.cpp
	$(CC) -c GameCtrl.cpp

Console.o: Console.cpp
	$(CC) -c Console.cpp

Grid.o: Grid.cpp
	$(CC) -c Grid.cpp

Map.o: Map.cpp
	$(CC) -c Map.cpp
	
Point.o: Point.cpp
	$(CC) -c Point.cpp

Snake.o: Snake.cpp
	$(CC) -c Snake.cpp

clean:
	rm Snake ${OBJS}

run:
	./Snake