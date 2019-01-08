CC = g++
CFLAGS = -g -Wall

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

all: program1 program2 program3 program4 program5 program6

program1: program1.cpp
	$(CC) $(CFLAGS) -o program1 program1.cpp $(LIBS)

program2: program2.cpp
	$(CC) $(CFLAGS) -o program2 program2.cpp $(LIBS)

program3: program3.cpp
	$(CC) $(CFLAGS) -o program3 program3.cpp $(LIBS)

program4: program4.cpp
	$(CC) $(CFLAGS) -o program4 program4.cpp $(LIBS)

program5: program5.cpp
	$(CC) $(CFLAGS) -o program5 program5.cpp $(LIBS)

program6: program6.cpp
	$(CC) $(CFLAGS) -o program6 program6.cpp $(LIBS)

.cpp.o:
	g++ -c -g pkg-config --cflags opencv $<

.PHONY: clean

clean:
	rm -f core *.o program1 program2 program3 program4 program5 program6
