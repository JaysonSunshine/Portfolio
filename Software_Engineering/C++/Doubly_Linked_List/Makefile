CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)
DEPS = Sequence.h
STRING = Sequence.o StringTest.o
LONG = Sequence.o longTest.o

String: $(STRING)
	$(CC) $(LFLAGS) $(STRING) -o String

Long: $(LONG)
	$(CC) $(LFLAGS) $(LONG) -o Long

All: String Long

longTest.o: Sequence.h longTest.cpp
	$(CC) $(CFLAGS) longTest.cpp

Sequence.o: Sequence.h Sequence.cpp
	$(CC) $(CFLAGS) Sequence.cpp

StringTest.o: Sequence.h StringTest.cpp
	$(CC) $(CFLAGS) StringTest.cpp

clean:
	\rm *.o String Long