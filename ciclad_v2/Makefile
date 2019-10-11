#working with g++ version 4.9+ ---> version 8.1.3

default: isci

isci.o: isci.h isci.cpp
	g++ -O3 -c isci.cpp -o $@

isci: main.cpp isci.o
	g++ -O3 -fno-pic -static -std=c++11 -DNDEBUG -I ./ -o $@ $^

test: isci
	/bin/time ./isci testdb_gen2.txt

test_d: isci_d
	/bin/time ./isci_d testdb_gen2.txt

isci_d: main.cpp isci.cpp
	g++ -O3 -g -static -fno-pic -std=c++11 -DDEBUG -o $@ $^

leak: isci_d
	valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes ./isci_d testdb_gen2.txt

profile: isci_d
	valgrind --tools=callgrind ./isci_d testdb_gen2.txt

clean:
	rm -f isci isci_d

.PHONY: clean leak profile
