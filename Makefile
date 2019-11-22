CFLAGS=-DDEBUG -g -std=c++14
proj=cache-sim

all: $(proj).o
	g++ -g $(proj).o -o $(proj)

$(proj).o: p2.cpp
	g++ -c $(CFLAGS) p2.cpp -o $(proj).o

run: all
	./$(proj) trace1.txt output.txt

clean:
	rm -rf *.o $(proj)
