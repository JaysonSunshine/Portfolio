CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = Sequence.h
OBJS = CheckType.o longTest.o Sequence.o StringTest.o

p1: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p1

CheckType.o: Sequence.h CheckType.cpp
	$(CC) $(CFLAGS) CheckType.cpp

longTest.o: Sequence.h longTest.cpp
	$(CC) $(CFLAGS) longTest.cpp

Sequence.o: Sequence.h Sequence.cpp
	$(CC) $(CFLAGS) Sequence.cpp

StringTest.o: Sequence.h StringTest.cpp
	$(CC) $(CFLAGS) StringTest.cpp

clean:
	\rm *.o p1