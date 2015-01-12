CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = Board.h Game.h globals.h Player.h
OBJS = Board.o Game.o Player.o main.o

p1: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p1

Board.o: Board.h Game.h globals.h Board.cpp
	$(CC) $(CFLAGS) Board.cpp

main.o: Game.h Player.h main.cpp
	$(CC) $(CFLAGS) main.cpp

Game.o: Board.h Game.h globals.h Player.h Game.cpp
	$(CC) $(CFLAGS) Game.cpp

Player.o: Board.h Game.h Player.h globals.h Player.cpp
	$(CC) $(CFLAGS) Player.cpp

clean:
	\rm *.o *~ p1